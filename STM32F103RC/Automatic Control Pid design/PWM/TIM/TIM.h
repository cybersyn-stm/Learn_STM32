#ifndef __TIM_H_
#define __TIM_H_

#include "stm32f10x.h"


void TIM_Config_Init(void);
void TIM_Ctrl(uint16_t arr,uint16_t psc,uint16_t pluse);
void TIM_Handler(uint16_t arr,uint16_t psc);
























#endif



