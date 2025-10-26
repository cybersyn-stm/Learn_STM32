#ifndef __RCC_H
#define __RCC_H
#include "stm32f10x.h"
void RCC_init(void);

void Systick_us(unsigned int time);
void Systick_ms(unsigned int time);
#endif



