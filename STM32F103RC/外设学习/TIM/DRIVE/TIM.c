#include "TIM.h"

void TIM_rcc_init(void)
{

	RCC->APB1ENR |= 1<<0;//OPEN TIM2
}
void TIM2_NVIC(void)//TIM2_IRQn = 28
{
	SCB->AIRCR |= 0x05FA0070;//0101 0BIT/4BIT
	NVIC->ISER[0] |= 0x10000000;//TIM2 NVIC ENABLE
	NVIC->IP[28] &= 0x0000<<4;
}
void TIM2_init(unsigned int ms)
{
	unsigned int APB1;
	RCC_ClocksTypeDef RCC_CLK;
	RCC_GetClocksFreq(&RCC_CLK);
	APB1 = RCC_CLK.PCLK1_Frequency;//GET APB1Freq
	
	TIM_rcc_init();
	TIM2->PSC = (APB1/1000)-1;//8000-1;//APB1= = 8Mhz -> PSC = 8Mhz/8000 = 1Khz
	TIM2->ARR = ms-1;//1Khz * 1000 = 1s
	TIM2->DIER |= 1<<0;//ENABLE UPDATA INTERRUPT
	TIM2->CR1 |= 0x01;//ENABLE TIM2
	TIM2_NVIC();
}
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&(1<<0))
	{
		GPIOA->ODR ^= 1<<8;
	}
	TIM2->SR &= ~(1<<0);
}

