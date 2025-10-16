#ifndef _I2C_H_
#define _I2C_H_

#include "stm32f10x.h"

#define I2C_PIN_LOW 0
#define I2C_PIN_HIGH 1

typedef uint8_t (*I2C_PIN_READ_CALLBACK_T)(void);
typedef void (*I2C_PIN_WRITE_CALLBACK_T)(uint8_t data);
typedef void (*I2C_PIN_GPIOMODE_CALLBACK_T)(uint8_t MODE);
typedef void (*I2C_DELAY_US_CALLACK_T)(uint32_t MICROSECONDS);

typedef struct{

	I2C_PIN_READ_CALLBACK_T SDA_READ;
	I2C_PIN_WRITE_CALLBACK_T SDA_WRITE;
	I2C_PIN_WRITE_CALLBACK_T SCL_WRITE;
	
	I2C_DELAY_US_CALLACK_T DELAY_US;
	
	uint32_t CLOCK_DELAY_US;
}I2C_T;

void I2C_INIT(I2C_T *init);




#endif 

