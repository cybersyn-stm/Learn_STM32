#ifndef __USART_H
#define __USART_H
#include "stm32f10x.h"
#include "string.h"
#include "stdio.h"
void USART_GPIO_Init(void);
void USART_Config_Init(void);
void USART_SendByte(uint16_t Byte);
void USART_SendHalfWord(uint16_t Byte);
void Usart_Send_16bit_Data(uint16_t value);
#endif
