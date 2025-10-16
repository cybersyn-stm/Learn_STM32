#include "I2C.h"

void I2C_INIT(I2C_T *i2c_t)
{
	i2c_t->SDA_WRITE(I2C_PIN_HIGH);
	i2c_t->SCL_WRITE(I2C_PIN_HIGH);
	
	i2c_t->DELAY_US(1);
}
void I2C_START(I2C_T *i2c_t)
{
	i2c_t->SCL_WRITE(I2C_PIN_HIGH);
	i2c_t->SDA_WRITE(I2C_PIN_HIGH);
	i2c_t->SDA_WRITE(I2C_PIN_LOW);
	i2c_t->SCL_WRITE(I2C_PIN_LOW);
}
void I2C_STOP(I2C_T *i2c_t)
{
	i2c_t->SDA_WRITE(I2C_PIN_LOW);
	i2c_t->SCL_WRITE(I2C_PIN_HIGH);
	i2c_t->SDA_WRITE(I2C_PIN_HIGH);
	i2c_t->SCL_WRITE(I2C_PIN_LOW);
}
void I2C_WRITE_BYTE(I2C_T *i2c_t)
{

}














