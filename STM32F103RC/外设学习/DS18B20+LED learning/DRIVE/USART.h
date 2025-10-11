#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
void USART1_init(void);
void USART1_SendChar(uint8_t data);
void USART1_IRQHandler(void);




#endif
