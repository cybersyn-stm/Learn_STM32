#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= 1<<4;//OPEN GPIOC CLOCK
}
void GPIO_init(void)
{
	GPIO_clock_init();
	GPIOC->CRL &= 0xFFF00FF;
	GPIOC->CRL |= 0x0001100;//PC2 PC3 OUT PUT/UP
}
