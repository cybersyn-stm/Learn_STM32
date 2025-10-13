#ifndef __I2C_H

#include "stm32f10x.h"
#include "DELAY.h"


#define I2C_SCL_GPIO GPIOC
#define I2C_SDA_GPIO GPIOC
#define I2C_SCL_PIN GPIO_Pin_12
#define I2C_SDA_PIN GPIO_Pin_11

#define I2C_SCL1 GPIO_SetBits(I2C_SCL_GPIO,I2C_SCL_PIN);
#define I2C_SCL0 GPIO_ResetBits(I2C_SCL_GPIO,I2C_SCL_PIN);
#define I2C_SDA1 GPIO_SetBits(I2C_SDA_GPIO,I2C_SDA_PIN);
#define I2C_SDA0 GPIO_ResetBits(I2C_SDA_GPIO,I2C_SDA_PIN);
#define I2C_ReadSDA GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
void I2C_STOP(void);
void I2C_START(void);
void I2C_SEND_ADD(void);
static uint8_t I2C_READ(void);
void I2C_Config_Init(void);
unsigned int I2C_ACK(void);
uint16_t KEY_scan(void); //KEY SCAN
void I2C_IN(void);
unsigned int key_ctrl(void);
void key_s(uint16_t *key);
#endif




