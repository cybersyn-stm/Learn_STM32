#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"
void TIM_rcc_init(void);
void TIM2_init(uint32_t ms);
void TIM2_IRQHandler(void);
#endif
