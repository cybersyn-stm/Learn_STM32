#include "stm32f10x.h"
#include "LED.h"
#include "RCC.h"
void LED_Ctrl(uint32_t led)
{
	//PB0
	if(((led>>0)&1) == 1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_0);
	}
	if(((led>>0)&1) == 0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}	
	//PC1
	if(((led>>1)&1) == 1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_1);
	}
	if(((led>>1)&1) == 0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_1);
	}
	//PC0
	if(((led>>2)&1) == 1)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_0);
	}
	if(((led>>2)&1) == 0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_0);
	}	
	//PB3
	if(((led>>3)&1) == 1)
	{
		GPIO_SetBits(GPIOB,GPIO_Pin_3);
	}
	if(((led>>3)&1) == 0)
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_3);
	}
}
void LED_FLASH1(void)
{
	unsigned int i,j;
	j=0x01;
	for(i=0;i<4;i++)
	{
		LED_Ctrl(j);
		j = j << 1;
		SysTick_ms(500);
	}
}
void LED_FLASH2(void)
{
	unsigned int i,j;
	j = 0x00;
	for(i=0;i<3;i++)
	{
		LED_Ctrl(j);
		j = j | (1<<i);
		SysTick_ms(500);
	}
}

