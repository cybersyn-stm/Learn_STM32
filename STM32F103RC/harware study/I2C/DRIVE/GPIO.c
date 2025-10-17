#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= 1<<2;//OPEN GPIOA
	//RCC->APB2ENR |= 1<<14;//OPEN USTART1
}
void GPIO_init(void)
{               
	GPIO_clock_init();
	//PA9 TXD /PA10 RXD
	GPIOA->CRH &= 0xFFFFF000;
	GPIOA->CRH |= 0x00000491;//PA8 PUT UP OUT/ PA9 PUT UP OUT /PA10 FLOAT IN 
}
