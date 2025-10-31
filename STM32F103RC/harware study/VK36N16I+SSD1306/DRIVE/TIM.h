#ifndef __TIM_H
#define __TIM_H
#include "stm32f10x.h"
void TIM_rcc_gpio_init(void);
void PWM_Setting(uint32_t duty);
void TIM_NVIC();
#endif
