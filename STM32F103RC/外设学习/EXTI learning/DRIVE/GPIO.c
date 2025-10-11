#include "GPIO.h"
void GPIO_clock_init(void)
{
	RCC->APB2ENR |= 1<<5;//OPEN GPIOD CLOCK
	RCC->APB2ENR |= 1<<2;//OPEN GPIOA CLOCK
	RCC->APB2ENR |= 1<<0;//OPEN AFIO CLOCK
}
void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_clock_init();
	GPIOA->CRH &= 0xFFFFFFF0;
	GPIOA->CRH |= 0x00000001;//PA8 OUT

	GPIOD->CRL &= 0xFFFFF0FF;
	GPIOD->CRL |= 0x00000100;//PD2 OUT
	
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPD;//PUT DOWN
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);//Init PA0 IN PUT DOWN
}
