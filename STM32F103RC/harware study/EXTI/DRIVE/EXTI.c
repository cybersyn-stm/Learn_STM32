#include "EXTI.h"
void EXTI_init(void)
{
	NVIC_InitTypeDef NVIC_Instruct;
	EXTI_InitTypeDef EXTI_Instruct;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);//4bit/0bit
	NVIC_Instruct.NVIC_IRQChannel = EXTI0_IRQn;//CHANNEL EXTI0
	NVIC_Instruct.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_Instruct.NVIC_IRQChannelCmd = ENABLE;//ENABLE EXTI0
	NVIC_Init(&NVIC_Instruct);
	
	
	EXTI_Instruct.EXTI_Line = EXTI_Line0;//LINE PX0
	EXTI_Instruct.EXTI_LineCmd = ENABLE;//PX0 ENABLE
	EXTI_Instruct.EXTI_Mode = EXTI_Mode_Interrupt;//MODE USE INTERRUPT
	EXTI_Instruct.EXTI_Trigger = EXTI_Trigger_Rising;//Rising 
	EXTI_Init(&EXTI_Instruct);
}


