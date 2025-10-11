#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
int main()
{
	RCC_init();
	GPIO_init();
	while(1)
	{
		SysTick_ms(500);
		GPIOC->ODR ^= 1<<2;
		SysTick_ms(500);
		GPIOC->ODR ^= 1<<3;
	}
}	

