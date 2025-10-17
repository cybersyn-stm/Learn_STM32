#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "TM1637.h"
#include "TIM.h"
uint16_t i = 0;
int main(void)
{
	GPIO_init();
	RCC_init();
	USART1_init();
	TM1637_init(0x07);
	while(1)
	{
		Delay_ms(1000);
	}
}	
void USART1_IRQHandler(void)
{
	if((USART1->SR>>5)&1)
	{
		uint8_t data;
		data = USART1->DR;
		TM1637_Seg((data>>4),(data&0x0f));
	}
}
void TIM2_IRQHandler(void)
{
	if(TIM2->SR&(1<<0))
	{
	}
}








