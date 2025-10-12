#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#define KEY0 GPIOA->IDR & (0x01 << 0)
RCC_ClocksTypeDef get_rcc_clock;
void delay()
{
	int x,y;
	for(x =1500;x>0;x--)
	{
		for(y = 1500;y>0;y--);
	}
}
int main(void)
{
	volatile long pupd = GPIOA->IDR;
	GPIO_init();
	RCC_init();

	while(1)
	{
		pupd = GPIOA->IDR & (0x01 << 0);
		if(pupd)
		{
			GPIO_ResetBits(GPIOD,GPIO_Pin_2);
		}
		else
		{
			GPIO_SetBits(GPIOD,GPIO_Pin_2);
		}
	}
}	







