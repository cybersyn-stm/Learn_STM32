#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"
#include "RCC.h"
#include "USART.h"
void DS18B20_init(void);
uint16_t DS18B20_ReadTemp(void);






#endif

