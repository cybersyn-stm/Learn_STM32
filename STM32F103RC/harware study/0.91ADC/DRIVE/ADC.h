#ifndef __ADC_H__
#define __ADC_H__
#include "stm32f10x.h"
#include "RCC.h"
void ADC1_init(void);
uint16_t ADC1_getChannelValue(uint8_t channel);



#endif 
