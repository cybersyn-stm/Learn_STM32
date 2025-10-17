#include "I2C.h"
void I2C_GPIO_Init(void)
{
	GPIO_InitTypeDef I2C_GPIO_Sturct;
	//OPEN GPIOC CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//SDA P11 SCL P12 OUT_OD
	I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
	I2C_GPIO_Sturct.GPIO_Pin = I2C_SDA_Pin|I2C_SCL_Pin;
	I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&I2C_GPIO_Sturct);
}
void I2C_START()
{
	I2C_SCL1;
	I2C_SDA1;
	I2C_SDA0;
	I2C_SCL0;
}
void I2C_STOP()
{
	I2C_SDA0;
	I2C_SCL1;
	I2C_SDA1;
	I2C_SCL0;
}
void I2C_ACK()
{
	I2C_SCL0;
	I2C_SDA0;
	I2C_SCL1;
	I2C_SCL0;
}
void I2C_WriteByte(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		I2C_SCL0;
		if(((data&0x80)>>7))
		{
			I2C_SDA1;
		}
		else
		{
			I2C_SDA0;
		}
		data <<= 1;
		I2C_SCL1;
	}
}
uint8_t I2C_ReadByte(void)
{
	uint8_t i,redata = 0,x;
	I2C_SDA1;
	for(i=0;i<8;i++)
	{
		I2C_SCL0;
		redata = redata<<1;
		x = I2C_ReadSDA;
		if(x == 1)
		{
			redata |= 0x01;
		}
		I2C_SCL1;
	}
	I2C_SCL0;
	return redata;
}
void I2C_Write(uint8_t device_address,uint8_t address,uint8_t data)
{
	I2C_START();
	I2C_WriteByte(device_address);
	I2C_ACK();
	I2C_WriteByte(data);
	I2C_ACK();
	I2C_WriteByte(data);
	I2C_STOP();
}








