#ifndef __I2C_H
#define __I2C_H
#include "stm32f10x.h"
#include "RCC.h"
void TM1637_Cmd(uint8_t cmd);
void TM1637_Data(uint8_t address,uint8_t data);
void TM1637_init(uint8_t light);
void TM1637_Seg(uint8_t address,uint8_t data);
void TM1637_Display(unsigned int data);
#endif


