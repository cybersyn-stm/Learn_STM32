#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= 1<<2;//OPEN GPIOA
}
void GPIO_init(void)
{               
	GPIO_clock_init();
	//PA9 TXD /PA10 RXD
	GPIOA->CRH &= 0xFFFFFFF0;
	GPIOA->CRH |= 0x00000000;//PA8 PUT UP OUT/ PA9 PUT UP OUT /PA10 FLOAT IN 
}
