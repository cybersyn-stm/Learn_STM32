#ifndef __TM1650C_H_
#define __TM1650C_H_

#include "stm32f10x.h"
#include "RCC.h"

#define TM1650_SDA_GPIO GPIOC //SDA PC4 
#define TM1650_SCL_GPIO GPIOC//SCL PC5
#define TM1650_SDA_Pin GPIO_Pin_5
#define TM1650_SCL_Pin GPIO_Pin_4
//SDA CTRL
#define TM1650_SDA1 GPIO_SetBits(TM1650_SDA_GPIO,TM1650_SDA_Pin)
#define TM1650_SDA0 GPIO_ResetBits(TM1650_SDA_GPIO,TM1650_SDA_Pin)

#define TM1650_SCL1 GPIO_SetBits(TM1650_SCL_GPIO,TM1650_SCL_Pin)
#define TM1650_SCL0 GPIO_ResetBits(TM1650_SCL_GPIO,TM1650_SCL_Pin)

#define TM1650_ReadSDA GPIO_ReadInputDataBit(TM1650_SDA_GPIO,TM1650_SDA_Pin)


void TM1650_1650GPIO_Init(void);
void TM1650_Write_1650(uint8_t address,uint8_t data);
void TM1650_init(void);
void TM1650_Dislay(uint16_t data);
void TM1650_Cotrl(uint8_t seg_duan,uint8_t seg_data);









#endif









