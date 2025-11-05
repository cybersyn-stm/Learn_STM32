#include "u8g2_user.h"
#include "stm32f10x_dma.h"
void I2C1_DMA_Config(uint8_t *data, uint16_t size)
{
    DMA_InitTypeDef DMA_InitStructure;

    DMA_DeInit(DMA1_Channel6);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&I2C1->DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)data;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = size;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(DMA1_Channel6, &DMA_InitStructure);
}
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
            break;
        case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
            break;
        case U8X8_MSG_DELAY_MILLI:   // delay arg_int * 1 milli second
            Systick_ms(1);
            break;
        case U8X8_MSG_DELAY_I2C:     // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
            arg_int ? (GPIOB->ODR |= 1<<6) : (GPIOB->ODR &= ~(1<<6));  
            break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
            arg_int ? (GPIOB->ODR |= 1<<7) : (GPIOB->ODR &= ~(1<<7));  
            break;                    // arg_int=1: Input dir with pullup high for I2C data pin
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
            u8x8_SetGPIOResult(u8x8, 1); // default return value
            break;
    }
    return 1;
}
uint8_t u8x8_byte_hw_i2c(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    uint8_t* data = (uint8_t*) arg_ptr;
    switch(msg) {
        case U8X8_MSG_BYTE_SEND:
            if (arg_int == 0) {
                break;
            }
            I2C1_DMA_Config(data, arg_int);
            DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);
            I2C_DMACmd(I2C1, ENABLE);
            DMA_Cmd(DMA1_Channel6, ENABLE);
            while (!DMA_GetFlagStatus(DMA1_FLAG_TC6)) {
                continue;
            }
            DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6);
            DMA_Cmd(DMA1_Channel6, DISABLE);
            I2C_DMACmd(I2C1, DISABLE);
            while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_TXE)) {
                continue;
            }
            while (!I2C_GetFlagStatus(I2C1, I2C_FLAG_BTF)) {
                continue;
            }
            break;
        case U8X8_MSG_BYTE_INIT:
        {
        /* add your custom code to init i2c subsystem */
            RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);
            RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
            I2C_InitTypeDef I2C_InitStructure = {
                .I2C_Mode = I2C_Mode_I2C,
                .I2C_DutyCycle = I2C_DutyCycle_2,
                .I2C_OwnAddress1 = 0x10,
                .I2C_Ack = I2C_Ack_Enable,
                .I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit,
                .I2C_ClockSpeed = 1090000
            };
            I2C_Init(I2C1, &I2C_InitStructure);
            I2C_Cmd(I2C1, ENABLE);
            I2C_DMACmd(I2C1, DISABLE);
        }
            break;
        case U8X8_MSG_BYTE_SET_DC:
        /* ignored for i2c */
            break;
        case U8X8_MSG_BYTE_START_TRANSFER:
            while(I2C_GetFlagStatus(I2C1, I2C_FLAG_BUSY));
            I2C_GenerateSTART(I2C1, ENABLE);
            while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_MODE_SELECT))
                continue;
            I2C_Send7bitAddress(I2C1, 0x78, I2C_Direction_Transmitter);
            while(!I2C_CheckEvent(I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
                continue;
            break;
        case U8X8_MSG_BYTE_END_TRANSFER:
            I2C_GenerateSTOP(I2C1, ENABLE);
            break;
        default:
            return 0;
    }
    return 1;
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
void u8g2_I2C_Mode(u8g2_t *u8g2,uint8_t set)
{
    if (set)
    {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_hw_i2c,u8x8_gpio_and_delay);
    }
    else
    {
        u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_sw_i2c,u8x8_gpio_and_delay);
    }
    
}
void u8g2_init(u8g2_t *u8g2,uint8_t I2C_Set)
{
    u8g2_GPIo_Setting(I2C_Set);//Hardware
	u8g2_I2C_Mode(u8g2,I2C_Set);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2,0);
	u8g2_ClearBuffer(u8g2);
}










