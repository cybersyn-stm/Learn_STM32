#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#define KEY2 ((GPIOC->IDR>>13)&1)
#define KEY1 ((GPIOA->IDR>>0)&1)
int main()
{
	RCC_init();
	GPIO_init();
	while(1)
	{
		if(KEY2)
		{
			GPIOC->ODR &= ~(1<<2);
		}
		else
		{
			GPIOC->ODR |= 1<<2;
		}	
		if(KEY1)
		{
			GPIOC->ODR &= ~(1<<3);
		}
		else
		{
			GPIOC->ODR |= 1<<3;
		}
	}
}	

