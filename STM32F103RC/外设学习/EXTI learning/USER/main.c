#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "EXTI.h"
#define KEY2 ((GPIOC->IDR>>13)&1)
#define KEY1 ((GPIOA->IDR>>0)&1)
int main()
{
	RCC_init();
	GPIO_init();
	EXTI_init();
	while(1)
	{

	}
}	
void EXTI0_IRQHandler(void)
{
	GPIOA->ODR ^= 1<<8;
	SysTick_us(10);
	EXTI_ClearITPendingBit(EXTI_Line0);
}
