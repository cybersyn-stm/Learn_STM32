#include "TM1650.h"
void I2C_1650GPIO_Init(void)
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
void I2C_1650START()
{
	I2C_SCL1;
	I2C_SDA1;
	I2C_SDA0;
	I2C_SCL0;
}
void I2C_1650STOP()
{
	I2C_SDA0;
	I2C_SCL1;
	I2C_SDA1;
	I2C_SCL0;
}
void I2C_1650ReadACK(void)
{
	I2C_SCL0;
	I2C_SDA0;
	I2C_SCL1;
	I2C_SDA1;
}
void I2C_1650WriteByte(uint8_t data)
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
void I2C_Write_1650(uint8_t address,uint8_t data)
{
	I2C_1650START();
	I2C_1650WriteByte(address);
	I2C_1650ReadACK();
	I2C_1650WriteByte(data);
	I2C_1650ReadACK();
	I2C_1650STOP();
}








