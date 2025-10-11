#include "I2C.h"
const uint8_t seg[17] = 
{
	//0,	1,	 2,	  3,	 4,	  5,	 6,   7,   8,   9,   A,   b,  C,    d,   E,   F,  	
	0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F,0x77,0x7C,0x39,0x5E,0x79,0x71,0x00
};
const uint8_t tub[4] =
{

	0xC0,0XC1,0XC2,0XC3
};
void I2C_init(void)
{
	RCC->APB2ENR |= 1<<4;//OPEN PC CLOCK
	GPIOC->CRL &= 0xFF00FFFF;
	GPIOC->CRL |= 0x00770000;//PC4 CLK PC5 SDA
	GPIOC->ODR |= 1<<4;
	GPIOC->ODR |= 1<<5;
}
void I2C_Write_SCL(uint8_t BIT)
{
	if(BIT == 1)
	{
		GPIOC->ODR |= 1<<4;
	}
	if(BIT == 0)
	{
		GPIOC->ODR &= ~(1<<4);
	}
	Systick_us(1);
}
void I2C_Write_SDA(uint8_t BIT)
{
	if(BIT == 1)
	{
		GPIOC->ODR |= 1<<5;
	}
	if(BIT == 0)
	{
		GPIOC->ODR &= ~(1<<5);
	}
	Systick_us(10);
}
uint8_t I2C_Read_SDA()
{
	uint8_t data;
	data = GPIOC->IDR;
	Systick_us(1);
	return data;
}
void I2C_Start(void)
{
	I2C_Write_SDA(1);
	I2C_Write_SCL(1);
	I2C_Write_SDA(0);
	I2C_Write_SCL(0);
}
void I2C_Stop(void)
{
	I2C_Write_SCL(0);
	I2C_Write_SDA(0);
	I2C_Write_SCL(1);
	I2C_Write_SDA(1);
}
uint8_t I2C_ACK(void)
{
	unsigned int ACK_Bit;
	I2C_Write_SDA(1);
	I2C_Write_SCL(1);
	ACK_Bit = I2C_Read_SDA();
	I2C_Write_SCL(0);
	return ACK_Bit;
}
void TM1637_Write_Byte(uint8_t data)
{
	uint8_t i;
	I2C_Write_SCL(0);
	
	for(i=0;i<8;i++)
	{
		if(data & 0x01)
		{
			I2C_Write_SDA(1);
		}
		else
		{
			I2C_Write_SDA(0);
		}
		data = data >> 1;
		
		I2C_Write_SCL(1);
		I2C_Write_SCL(0);
	}
}
void TM1637_Cmd(uint8_t cmd)
{
	I2C_Start();
	TM1637_Write_Byte(cmd);
	I2C_ACK();
	I2C_Stop();
}
void TM1637_Data(uint8_t address,uint8_t data)
{
	I2C_Start();
	TM1637_Write_Byte(address);
	I2C_ACK();
	TM1637_Write_Byte(data);
	I2C_ACK();
	I2C_Stop();
}
void TM1637_Clean(void)
{
	TM1637_Data(0xC0,0x00);
	TM1637_Data(0xC1,0x00);
	TM1637_Data(0xC2,0x00);
	TM1637_Data(0xC3,0x00);
}
void TM1637_init(uint8_t light)
{
	I2C_init();
	TM1637_Cmd(0x88|light);
	TM1637_Cmd(0x44);
	TM1637_Clean();
}
void TM1637_Seg(uint8_t address,uint8_t data)
{
	TM1637_Data(tub[address],seg[data]);
}
void TM1637_Display(unsigned int data)
{
	if(data>9999)
	{
		data = 9999;
	}
	if(data > 999)
	{
		TM1637_Seg(0,data/1000);
		TM1637_Seg(1,data/100%10);
		TM1637_Seg(2,data/10%10);
		TM1637_Seg(3,data%10);
	}
	else if(data > 99)
	{
		TM1637_Seg(0,16);
		TM1637_Seg(1,data/100);
		TM1637_Seg(2,data/10%10);
		TM1637_Seg(3,data%10);
	}
	else if(data > 9)
	{
		TM1637_Seg(0,16);
		TM1637_Seg(1,16);
		TM1637_Seg(2,data/10);
		TM1637_Seg(3,data%10);
	}
	else
	{
		TM1637_Seg(0,16);
		TM1637_Seg(1,16);
		TM1637_Seg(2,16);
		TM1637_Seg(3,data);
	}
}










