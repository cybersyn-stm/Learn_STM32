#include "TIM.h"
void TIM_rcc_gpio_init(void)
{
    // OPEN TIM2 / GPIOA / AFIO CLOCK
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    // GPIO INIT: PA1 -> TIM2_CH2 (复用推挽)
    GPIO_InitTypeDef TIM_GPIO_InitStruct;
    TIM_GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF_PP;
    TIM_GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_1;         // PA1 TIM2_CH2
    TIM_GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &TIM_GPIO_InitStruct);

    // TIM BASE INIT: 1 kHz
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_TimeBaseInitStruct.TIM_Period        = 1000 - 1; // ARR = 999
    TIM_TimeBaseInitStruct.TIM_Prescaler     = 72 - 1;   // 72MHz/72 = 1MHz
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);
    TIM_ARRPreloadConfig(TIM2, ENABLE);

    // TIM2 CH2 PWM
    TIM_OCInitTypeDef TIM_OCInitStruct;
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // 关键：打开通道输出
    TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse       = 0; // 初始占空比 0%
    TIM_OC2Init(TIM2, &TIM_OCInitStruct);
    TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);

    TIM_Cmd(TIM2, ENABLE);

    //TIM 3
    TIM_TimeBaseInitTypeDef TIM3_Base_InitStruct;
    TIM3_Base_InitStruct.TIM_Period        = 65536 - 1; // ARR = 999
    TIM3_Base_InitStruct.TIM_Prescaler     = 72 - 1;
    TIM3_Base_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM3_Base_InitStruct.TIM_CounterMode   = TIM_CounterMode_Up;            
    TIM_TimeBaseInit(TIM3, &TIM3_Base_InitStruct);
    TIM_Cmd(TIM3, ENABLE);

    // 使能 TIM3 更新中断（必要）
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    // 配置 NVIC
    TIM_NVIC();

}

void PWM_Setting(uint32_t duty)
{
    // duty 按计数值设定，占空比 = duty/(ARR+1)
    uint16_t arr = TIM2->ARR;           // 999
    if (duty > arr) duty = arr;         // 防越界（main 里传 1000 时将变为 100%）
    TIM_SetCompare2(TIM2, (uint16_t)duty);
}
void TIM_NVIC()
{
    NVIC_InitTypeDef NVIC_InitStructure;

    // 设置优先级分组（按需选择 NVIC_PriorityGroup_0.._4）
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 16;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 16;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

