#include "GPIO.h"
void GPIO_rcc_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}
void GPIO_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_rcc_init();
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIOC->ODR |= 1<<2;
	GPIOC->ODR &= ~(1<<3);
}
