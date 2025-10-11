#ifndef __RCC_H
#define __RCC_H
#include "stm32f10x.h"
void RCC_init(void);

void SysTick_us(unsigned int time);
void SysTick_ms(unsigned int time);
#endif



