#include "I2C.h"
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
void I2C_ACK(I2C_T *i2c_t)
{
	i2c_t->SCL_WRITE(I2C_PIN_LOW);
	i2c_t->SDA_WRITE(I2C_PIN_HIGH);
	i2c_t->SCL_WRITE(I2C_PIN_HIGH);
	i2c_t->SCL_WRITE(I2C_PIN_LOW);
}
void I2C_WRITE_BYTE(I2C_T *i2c_t,uint8_t data)
{
	uint8_t i;
	for(i = 0;i < 8;i++)
	{
		i2c_t->SCL_WRITE(I2C_PIN_LOW);//SCL0
		if(((data&0x80)>>7) == 1)
		{
			i2c_t->SDA_WRITE(I2C_PIN_HIGH);//SDA1
		}
		else
		{
			i2c_t->SDA_WRITE(I2C_PIN_LOW);//SDA0
		}
		data<<=1;
		i2c_t->SCL_WRITE(I2C_PIN_HIGH);//SCL1
	}
}
uint8_t I2C_READ_BYTE(I2C_T *i2c_t)
{
	uint8_t i,redata,data;
	i2c_t->SDA_WRITE(I2C_PIN_HIGH);//SCL1
	for(i = 0;i < 8;i++)
	{
		i2c_t->SCL_WRITE(I2C_PIN_LOW);//READ SDA
		redata<<=1;
		data = (i2c_t->SDA_READ());
		if(data == 1)
		{
			redata |= 0x01;
		}
		i2c_t->SCL_WRITE(I2C_PIN_LOW);//SCL0
	}
	i2c_t->SCL_WRITE(I2C_PIN_LOW);
	return redata;
}
void I2C_SEND_SSD1306(I2C_T *i2c_t,uint8_t device_address,uint8_t data_or_command,uint8_t data)
{
	I2C_START(i2c_t);
	I2C_WRITE_BYTE(i2c_t,device_address);
	I2C_ACK(i2c_t);
	I2C_WRITE_BYTE(i2c_t,data_or_command);
	I2C_ACK(i2c_t);
	I2C_WRITE_BYTE(i2c_t,data);
	I2C_ACK(i2c_t);
	I2C_STOP(i2c_t);
}







