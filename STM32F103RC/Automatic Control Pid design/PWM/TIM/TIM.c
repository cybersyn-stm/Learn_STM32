#include "TIM.h"
void TIM_Config_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3|RCC_APB1Periph_TIM2,ENABLE);//OPEN TIM3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//OPEN GPIOA

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM3,ENABLE);
	
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_InternalClockConfig(TIM3);
	TIM_OC1PreloadConfig(TIM3,TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM3,ENABLE);
}
void TIM_Ctrl(uint16_t arr,uint16_t psc,uint16_t pluse)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	TIM_InternalClockConfig(TIM3);
	
	TIM_TimeBaseStructure.TIM_Period =arr-1;
	TIM_TimeBaseStructure.TIM_Prescaler = psc-1;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = pluse;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;
	TIM_OC1Init(TIM3,&TIM_OCInitStructure);
	
	TIM_Cmd(TIM3,ENABLE);
}
void TIM_Handler(uint16_t arr,uint16_t psc)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,ENABLE);
	NVIC_InitTypeDef NVIC_TIM2;
	NVIC_InitTypeDef NVIC_TIM5;
	TIM_TimeBaseInitTypeDef TIM_Handler_Init;
	TIM_TimeBaseInitTypeDef TIM_Handler_TIM5_Init;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_TIM2.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_TIM2.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_TIM2.NVIC_IRQChannelSubPriority = 0;
	NVIC_TIM2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TIM2);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	NVIC_TIM5.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_TIM5.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_TIM5.NVIC_IRQChannelSubPriority = 0;
	NVIC_TIM5.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_TIM5);
	
	TIM_Handler_Init.TIM_Period = arr -1 ;
	TIM_Handler_Init.TIM_Prescaler = psc;
	TIM_Handler_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Handler_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM2,&TIM_Handler_Init);
	
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM2,ENABLE);
	
	TIM_Handler_TIM5_Init.TIM_Period = 100;
	TIM_Handler_TIM5_Init.TIM_Prescaler = 3200;
	TIM_Handler_TIM5_Init.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Handler_TIM5_Init.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM5,&TIM_Handler_TIM5_Init);
	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);
	
	TIM_Cmd(TIM5,ENABLE);
}














