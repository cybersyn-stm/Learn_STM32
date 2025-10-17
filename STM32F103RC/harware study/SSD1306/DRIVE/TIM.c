#include "TIM.h"
RCC_ClocksTypeDef RCC_Clocks;
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
void TIM2_init(uint32_t ms)
{
	TIM_rcc_init();
	RCC_GetClocksFreq(&RCC_Clocks);
	TIM2->PSC = 72000000/10000-1;
	TIM2->ARR = 10*ms;//1Khz= 1ms
	TIM2->DIER |= 1<<0;//ENABLE UPDATA INTERRUPT
	TIM2->CR1 |= 0x01;//ENABLE TIM2
	TIM2_NVIC();
}

