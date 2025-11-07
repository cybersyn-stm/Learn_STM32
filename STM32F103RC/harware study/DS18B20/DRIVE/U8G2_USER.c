#include "u8g2_user.h"
#include <string.h>

static volatile uint8_t dma_transfer_complete = 0;
static uint8_t g_i2c_hw_mode = 0; // 1=硬件I2C，0=软件I2C

/* 新增：I2C DMA发送缓冲 */
#define U8G2_I2C_DMA_BUF_MAX 8800
static struct {
    uint16_t len;
    uint8_t  active;
    uint8_t  buf[U8G2_I2C_DMA_BUF_MAX];
} i2c_tx_ctx;

uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO:
            break;
        case U8X8_MSG_DELAY_10MICRO:
            break;
        case U8X8_MSG_DELAY_MILLI:
            Systick_ms(arg_int); // 修正：按参数延时
            break;
        case U8X8_MSG_DELAY_I2C:
            break;
        case U8X8_MSG_GPIO_I2C_CLOCK:
            if (!g_i2c_hw_mode) { // 硬件I2C时不驱动引脚，避免与外设冲突
                arg_int ? (GPIOB->ODR |= 1<<6) : (GPIOB->ODR &= ~(1<<6));
            }
            break;
        case U8X8_MSG_GPIO_I2C_DATA:
            if (!g_i2c_hw_mode) {
                arg_int ? (GPIOB->ODR |= 1<<7) : (GPIOB->ODR &= ~(1<<7));
            }
            break;
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1);
            break;
    }
    return 1;
}

void DMA1_Channel6_IRQHandler(void)
{
    if (DMA_GetITStatus(DMA1_IT_TC6))
    {
        DMA_ClearITPendingBit(DMA1_IT_TC6);

        /* DMA收尾并发送STOP（短轮询确保最后字节推出） */
        DMA_Cmd(DMA1_Channel6, DISABLE);
        I2C_DMACmd(I2C1, DISABLE);

        uint32_t guard = 10000;
        while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF) && --guard);
        I2C_GenerateSTOP(I2C1, ENABLE);

        i2c_tx_ctx.active = 0;
        dma_transfer_complete = 1;
    }
}

// 初始化DMA用于I2C传输
static void I2C_DMA_Init(void)
{
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    DMA_InitTypeDef DMA_InitStructure;
    DMA_DeInit(DMA1_Channel6); // I2C1_TX -> DMA1_Channel6

    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = 0; // 发送时设置
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;     // 发送时设置
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(DMA1_Channel6, &DMA_InitStructure);

    DMA_ITConfig(DMA1_Channel6, DMA_IT_TC, ENABLE);

    NVIC_InitTypeDef NVIC_InitStructure;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

uint8_t u8x8_byte_hw_i2c_dma(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    uint8_t* data = (uint8_t*) arg_ptr;
    switch(msg) {
        case U8X8_MSG_BYTE_SEND:
        {
            /* 精简：仅把数据收集到缓冲区 */
            if (arg_int == 0) break;
            uint16_t remain = U8G2_I2C_DMA_BUF_MAX - i2c_tx_ctx.len;
            uint16_t n = (arg_int <= remain) ? arg_int : remain;
            memcpy(&i2c_tx_ctx.buf[i2c_tx_ctx.len], data, n);
            i2c_tx_ctx.len += n;
        }
            break;

        case U8X8_MSG_BYTE_INIT:
        {
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            I2C_InitTypeDef I2C_InitStructure = {
                .I2C_Mode = I2C_Mode_I2C,
                .I2C_DutyCycle = I2C_DutyCycle_2,
                .I2C_OwnAddress1 = 0x10,
                .I2C_Ack = I2C_Ack_Enable,
                .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
                .I2C_ClockSpeed = 1000000 // F1最高1MHz
            };
            I2C_Init(I2C1, &I2C_InitStructure);
            I2C_Cmd(I2C1, ENABLE);
            I2C_DMA_Init();
        }
            break;

        case U8X8_MSG_BYTE_SET_DC:
            break;

        case U8X8_MSG_BYTE_START_TRANSFER:
            while (I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
            i2c_tx_ctx.len = 0;
            i2c_tx_ctx.active = 0;
            dma_transfer_complete = 0;

            I2C_GenerateSTART(I2C1, ENABLE);
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT));
            I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter); // 0x3C<<1
            while (!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));
            break;

        case U8X8_MSG_BYTE_END_TRANSFER:
            /* 非阻塞：一次性启动DMA，不在此等待或发送STOP */
            if (i2c_tx_ctx.len && !i2c_tx_ctx.active) {
                DMA_Cmd(DMA1_Channel6, DISABLE);
                DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_HT6 | DMA1_FLAG_TE6);

                DMA1_Channel6->CMAR  = (uint32_t)i2c_tx_ctx.buf;
                DMA1_Channel6->CNDTR = i2c_tx_ctx.len;

                i2c_tx_ctx.active = 1;
                dma_transfer_complete = 0;

                I2C_DMACmd(I2C1, ENABLE);
                DMA_Cmd(DMA1_Channel6, ENABLE);
            }
            break;

        default:
            return 0;
    }
    return 1;
}

void u8g2_I2C_Mode(u8g2_t *u8g2,uint8_t set)
{
    g_i2c_hw_mode = (set ? 1 : 0);
    if (set) {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c_dma, u8x8_gpio_and_delay);
    } else {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8x8_gpio_and_delay);
    }
}
void u8g2_GPIo_Setting(uint8_t set)
{
    GPIO_InitTypeDef I2C_GPIO_Sturct;
    if (set)//Hardware GPIO SET
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
        RCC_APB2PeriphClockCmd(RCC_APB2ENR_AFIOEN,ENABLE);
        //SDA P11 SCL P12 OUT_OD
        I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_AF_OD;
        I2C_GPIO_Sturct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
    }
    else//Software GPIO SET
    {
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
        //SDA P11 SCL P12 OUT_OD
        I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
        I2C_GPIO_Sturct.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
        I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
    }
    GPIO_Init(GPIOB,&I2C_GPIO_Sturct);
}
void u8g2_init(u8g2_t *u8g2,uint8_t I2C_Set)
{
    u8g2_GPIo_Setting(I2C_Set);//Hardware
	u8g2_I2C_Mode(u8g2,I2C_Set);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2,0);
	u8g2_ClearBuffer(u8g2);
}










