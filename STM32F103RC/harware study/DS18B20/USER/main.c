#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "DS18B20.h"
#include "u8g2.h"
#include <stdio.h>
#include "U8G2_USER.h"
#include "stm32f10x_tim.h"
#include "misc.h"

u8g2_t u8g2;
uint8_t i;
uint32_t temp;
char buf[32];

// 统计变量（ISR / 主循环共享）
volatile uint32_t u8g2_frame_count = 0;
volatile uint32_t u8g2_fps = 0;

// 使用 TIM2 作为 1Hz 定时器
static void TIM2_1Hz_Init(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    // 使能 TIM2 时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

    // 设定：预分频和自动重装载，使中断频率为 1 Hz
    // 计算方法：PSC = SystemCoreClock/1000 - 1; ARR = 1000 - 1 => 1Hz
    uint32_t prescaler = SystemCoreClock / 1000;
    if (prescaler == 0) prescaler = 1;
    if (prescaler > 0xFFFF) prescaler = 0xFFFF;

    TIM_TimeBaseStructure.TIM_Period = 1000 - 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t)(prescaler - 1);
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

    // 使能更新中断
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

    // NVIC 配置
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    // 启动定时器
    TIM_Cmd(TIM2, ENABLE);
}

// TIM2 中断处理函数：每秒把计数拷贝并清零
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        u8g2_fps = u8g2_frame_count;
        u8g2_frame_count = 0;
    }
}
int main(void)
{
    GPIO_init();
    RCC_init();
    USART1_init();
    DS18B20_init();
    u8g2_init(&u8g2,1);
    u8g2_SetFont(&u8g2, u8g2_font_7x13_te);

    // 初始化 TIM2 为 1Hz，用来统计 FPS
    TIM2_1Hz_Init();

    while(1)
    {
            //temp = DS18B20_ReadTemp();
            u8g2_ClearBuffer(&u8g2);
            sprintf(buf, "Temp:%d.%02dC FPS:%lu", temp / 100, temp % 100, (unsigned long)u8g2_fps);
            u8g2_DrawBox(&u8g2, i, 40, 10, 10);  // 绘制方块
            u8g2_DrawStr(&u8g2, 0, 10, buf);
            u8g2_SendBuffer(&u8g2);
            // 每次发送后计数帧
            u8g2_frame_count++;

            if (i < 128)
            {
                i++;
            }
            else
            {
                i = 0;
            }   
    }
}







