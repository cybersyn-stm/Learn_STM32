#include "DELAY.h"
void SysTick_Init(void)
{
	RCC->CFGR &= ~(1<<0);
	RCC->CFGR |= (1<<0);
	RCC->CFGR &= ~(1<<1);
	
	RCC->CFGR &= ~(1<<7);
	RCC->CFGR |= (1<<7);
	//ENABLE HSE
	RCC->CR &= ~(3<<16);
	RCC->CR |= (3<<16);
	//DISENABLE HSI
	RCC->CR &= ~(3<<0);
	//SYSTICK USEING HSE 
	RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);//SYSCLK 72MHZ
	
	RCC_HCLKConfig(RCC_SYSCLK_Div1);
	SysTick->CTRL &= ~(0<<2);
	SysTick->CTRL |= 0<<2;
	
			SysTick->CTRL &= ~(1<<0);
	SysTick->CTRL |= 1<<0;
}
void delay_us(unsigned int xus)
{
	
	SysTick->CTRL |= 1<<0;
	SysTick->LOAD = xus * 1000000/1000/1000 - 1;
	SysTick->CTRL &=~(0X01 << 1);
	SysTick->VAL = 0X00;
	SysTick->CTRL |= (0x01<<0);
	while((SysTick->CTRL&(1<<16)) == 0);
	SysTick->CTRL &=~ (0x01<<0);
	SysTick->VAL = 0X00;
}
void delay_ms(unsigned int xms)
{
	
	SysTick->CTRL |= 1<<0;
	SysTick->LOAD = (xms * 1000000/1000);
	SysTick->CTRL &=~(0X01 << 1);
	SysTick->VAL = 0X00;
	SysTick->CTRL |= (0x01<<0);
	while((SysTick->CTRL&(1<<16)) == 0)
	{
		SysTick->CTRL |= (0x01<<0);
	};
	SysTick->CTRL &=~ (0x01<<0);
	SysTick->VAL = 0X00;
}


