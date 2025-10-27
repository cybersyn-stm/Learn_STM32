#ifndef __KEYBOARD_H_
#define __KEYBOARD_H_

#include "stm32f10x.h"
#include "I2C.h"
#include "RCC.h"

#define KEYBOARD_SCL_PIN     GPIO_Pin_12
#define KEYBOARD_SDA_PIN     GPIO_Pin_11
#define KEYBOARD_GPIO_PORT   GPIOC

void KEYBOARD_Init(I2C_T *keyboard_i2c);
uint8_t KEYBOARD_Scan(I2C_T *keyboard_i2c);

#endif

