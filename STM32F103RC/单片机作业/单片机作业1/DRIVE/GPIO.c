#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= 1<<3;//OPEN GPIOB CLOCK
	RCC->APB2ENR |= 1<<4;//OPEN GPIOC CLOCK
}
void GPIO_init(void)
{
	GPIO_clock_init();
	GPIOB->CRL |= 0x00001001;//PB0 PB3 OUT
	GPIOC->CRL |= 0x00000011;//PC0 PC1 OUT
}
