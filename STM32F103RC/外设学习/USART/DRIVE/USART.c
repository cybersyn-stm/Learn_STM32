#include "USART.h"
void USART1_init(void)
{
	USART1->BRR = ((8000000/(9600*16))<<4);
	USART1->CR1 &= ~(1<<12);
	USART1->CR2 &= ~USART_CR2_STOP;
	USART1->CR1 &= ~(1<<10);
	USART1->CR1 |= 1<<5;
	USART1->CR1 |= USART_CR1_TE|USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
	
	
	SCB->AIRCR |= 0X5FA070;//0101 0BIT/4BIT
	NVIC->ISER[1] |= 1<<5;//USAR1_IRQn ENABLE
	
}
void USART1_SendChar(uint8_t data)
{
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = data;
	while(!(USART1->SR & USART_SR_TC));
}
void USART1_IRQHandler(void)
{
	if((USART1->SR>>5)&1)
	{
		uint8_t data;
		data = USART1->DR;
		if(data == 0xff)
		{
			GPIOA->ODR |= 1<<8;
			USART1_SendChar(data);
		}
		if(data == 0x00)
		{
			GPIOA->ODR &= ~(1<<8);
			USART1_SendChar(data);
		}
	}
}
