#ifndef __DELAY_H
#define __DELAY_H
#include "stm32f10x.h"
void SysTick_Init(void);
void delay_us(unsigned int xus);
void delay_ms(unsigned int xms);

#endif

