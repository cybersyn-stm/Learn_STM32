#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= (1<<2);//OPEN GPIOA CLOCK
	RCC->APB2ENR |= (1<<5);//OPEN GPIOD CLOCK
	RCC->APB2ENR |= (1<<4);//OPEN GPIOC CLOCK
}
void GPIO_init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_clock_init();
	GPIOA->CRH &= 0X0FFFFFF0;//PA8 PUT/UP OUT
	
	GPIOA->CRH |= 0X00000001;
	
	GPIOD->CRL &= 0XFFFFF0FF;//PD2 PUT/UP OUT
	GPIOD->CRL |= 0X00000100;



 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                 
 GPIO_InitStructure.GPIO_Mode= GPIO_Mode_IPD;        
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;         
 GPIO_Init(GPIOA, &GPIO_InitStructure);                     
	
}
