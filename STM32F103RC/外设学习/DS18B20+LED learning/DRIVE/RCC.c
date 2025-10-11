#include "RCC.h"
void RCC_init(void)//Systick = 72Mhz
{
	RCC->CR |= 1<<16;//OPEN HSE
	RCC->CR |= 1<<24;//OPEN PLL
	RCC->CFGR = 0x00000000;//CLEAN CFGR
	RCC->CFGR |= 1<<1;//SYSCLK = PLL
	RCC->CFGR |= 1<<16;
	RCC->CFGR |= 1<<10;
	SysTick->CTRL |= 1<<2;//SYSTICK USE HCLK
}
void Systick_us(unsigned int time)
{
	uint32_t temp;
	SysTick->LOAD = (time * 72)-1;
	SysTick->VAL |= 0x00;//CLEAN SYSTICK
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//OPEN SYSTICK
	
	do{
		temp = SysTick->CTRL;
	}while(temp & 0x01 && !(temp & (1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL |= 0X00;
}
void SysTick_ms(unsigned int time)
{
	for(;time>0;time--)
	{
		Systick_us(1000);
	}
}
