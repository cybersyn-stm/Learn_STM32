#include "GPIO.h"
void GPIO_rcc_init(void)
{
	RCC->APB2ENR |= (1<<2);//OPEN GPIOA CLOCK
	RCC->APB2ENR |= (1<<5);//OPEN GPIOD CLOCK
}
void GPIO_init(void)
{
	GPIO_rcc_init();
	GPIOA->CRH &= 0XFFFFFFF0;//PA8 PUT/UP OUT
	GPIOA->CRH |= 0X00000001;
	
	GPIOD->CRL &= 0XFFFFF0FF;//PD2 PUT/UP OUT
	GPIOD->CRL |= 0X00000100;
	
	GPIOD->ODR |= 1<<2;
	GPIOA->ODR |= 1<<8;
}
