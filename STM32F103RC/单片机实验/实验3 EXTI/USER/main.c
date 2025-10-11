#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "EXTI.h"
#define KEY2 ((GPIOC->IDR>>13)|1)
#define KEY1 ((GPIOA->IDR>>0)|1)
int main()
{
	RCC_init();
	GPIO_init();
	EXTI_init();
	while(1)
	{

	}
}	
void EXTI15_10_IRQHandler(void)
{
	if(KEY1)
	{
		EXTI->PR = 1<<13;
		GPIOC->ODR ^= 1<<2;
	}
}
