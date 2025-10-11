#include "USART.h"
void USART1_init(void)
{
	USART1->BRR = ((8000000/(9600*16))<<4);
	USART1->CR1 &= ~(1<<12);
	USART1->CR2 &= ~USART_CR2_STOP;
	USART1->CR1 &= ~(1<<10);
	
	USART1->CR1 |= USART_CR1_TE|USART_CR1_RE;
	USART1->CR1 |= USART_CR1_UE;
}
void USART1_SendChar(uint8_t data)
{
	while(!(USART1->SR & USART_SR_TXE));
	USART1->DR = data;
	while(!(USART1->SR & USART_SR_TC));
}
void USART1_IRQHandler(void);
