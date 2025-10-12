#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f10x.h"
#include "RCC.h"

#define I2C_SDA_GPIO GPIOC //SDA PC11 
#define I2C_SCL_GPIO GPIOC//SCL PC12
#define I2C_SDA_Pin GPIO_Pin_11
#define I2C_SCL_Pin GPIO_Pin_12
//SDA CTRL
#define I2C_SDA1 GPIO_SetBits(I2C_SDA_GPIO,I2C_SDA_Pin)
#define I2C_SDA0 GPIO_ResetBits(I2C_SDA_GPIO,I2C_SDA_Pin)

#define I2C_SCL1 GPIO_SetBits(I2C_SCL_GPIO,I2C_SCL_Pin)
#define I2C_SCL0 GPIO_ResetBits(I2C_SCL_GPIO,I2C_SCL_Pin)

#define I2C_ReadSDA GPIO_ReadInputDataBit(I2C_SDA_GPIO,I2C_SDA_Pin)


void I2C_GPIO_Init(void);
void I2C_Write(uint8_t address,uint8_t data);











#endif









