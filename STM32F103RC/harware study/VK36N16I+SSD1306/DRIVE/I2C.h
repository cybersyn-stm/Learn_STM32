#ifndef __I2C_H_
#define __I2C_H_

#include "stm32f10x.h"
#include <stdint.h>

#define I2C_PIN_LOW  0
#define I2C_PIN_HIGH 1

typedef void (*I2C_SETTING_INIT_CALLBACK_T)(void);
typedef void (*I2C_PIN_WRITE_CALLBACK_T)(uint8_t level);
typedef uint8_t (*I2C_PIN_READ_CALLBACK_T)(void);
typedef void (*I2C_DELAY_US_CALLBACK_T)(uint32_t us);

typedef struct {
    I2C_SETTING_INIT_CALLBACK_T I2C_SETTING_INIT;
    I2C_PIN_READ_CALLBACK_T SDA_READ;
    I2C_PIN_WRITE_CALLBACK_T SDA_WRITE;
    I2C_PIN_WRITE_CALLBACK_T SCL_WRITE;
    I2C_DELAY_US_CALLBACK_T DELAY_US;
} I2C_T;
/* 基本 I2C 操作，均通过回调实现 */
void I2C_START(I2C_T *i2c);
void I2C_STOP(I2C_T *i2c);
void I2C_ACK(I2C_T *i2c);
void I2C_WRITE_BYTE(I2C_T *i2c, uint8_t data);
uint8_t I2C_READ_BYTE(I2C_T *i2c);

/* 方便的发送函数（写寄存器） */
void I2C_SEND(I2C_T *i2c, uint8_t device_address, uint8_t reg, uint8_t data);

#endif









