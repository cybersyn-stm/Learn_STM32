#ifndef __TM1650_H
#define __TM1605_H

#include "stm32f10x.h"
#include "DELAY.h"

#define TM1650_SCL_GPIO GPIOC
#define TM1650_SDA_GPIO GPIOC
#define TM1650_SCL_PIN GPIO_Pin_3
#define TM1650_SDA_PIN GPIO_Pin_2

#define TM1650_SCL1 GPIO_SetBits(TM1650_SCL_GPIO,TM1650_SCL_PIN)
#define TM1650_SCL0 GPIO_ResetBits(TM1650_SCL_GPIO,TM1650_SCL_PIN)
#define TM1650_SDA1 GPIO_SetBits(TM1650_SDA_GPIO,TM1650_SDA_PIN)
#define TM1650_SDA0 GPIO_ResetBits(TM1650_SDA_GPIO,TM1650_SDA_PIN)

void TM1650_Write_Data(uint16_t Add,uint16_t Data);
void TM1650_SetDisplay(int x,int ctrl);//
void TM1650_Config_Init(unsigned int luminance);
void TM1650_DigitalTube(int NUMBER,int DIG);

#endif

