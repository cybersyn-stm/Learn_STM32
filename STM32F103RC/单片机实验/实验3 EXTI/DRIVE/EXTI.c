#include "EXTI.h"
void EXTI_init(void)
{
	//EXTI0
	SCB->AIRCR |= 0x5FA0070;//0101 0BIT/4BIT
	NVIC->ISER[1] |= 1<<8;//EXTI15_10 ENABLE
	AFIO->EXTICR[3] &= ~(0XFFFF<<0);
	AFIO->EXTICR[3] |= 0X0020;
	
	EXTI->IMR |= 1<<13;
	EXTI->RTSR |= 1<<13;
}


