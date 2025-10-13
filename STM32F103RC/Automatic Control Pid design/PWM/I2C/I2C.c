#include "I2C.h"
#include "TM1650.h"
#include "USART.h"
void I2C_DELAY(void)
{
	unsigned int i;
 	for(i=0;i<20;i++);
}
void I2C_Config_Init(void)
{
	GPIO_InitTypeDef GPIO_I2C_INIT;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	GPIO_I2C_INIT.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_I2C_INIT.GPIO_Pin = I2C_SDA_PIN | I2C_SCL_PIN;
	GPIO_I2C_INIT.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_I2C_INIT);
}
void I2C_OUT(void)
{
	GPIO_InitTypeDef GPIO_I2C_OUT;
	
	GPIO_I2C_OUT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_I2C_OUT.GPIO_Pin = I2C_SDA_PIN;
	GPIO_I2C_OUT.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_I2C_OUT);
}
void I2C_IN(void)
{
	GPIO_InitTypeDef GPIO_I2C_IN;
	
	GPIO_I2C_IN.GPIO_Pin = I2C_SDA_PIN;
	GPIO_I2C_IN.GPIO_Mode = GPIO_Mode_IPU;

	GPIO_Init(GPIOB,&GPIO_I2C_IN);
	
}
void I2C_START(void)
{
	I2C_SDA1;
	I2C_SCL1;
	I2C_DELAY();
	I2C_SDA0;
	I2C_DELAY();
	I2C_SCL0;
	I2C_DELAY();
}
void I2C_STOP(void)
{
	I2C_SDA0;
	I2C_SCL1;
	I2C_DELAY();
	I2C_SDA1;
	I2C_DELAY();
	I2C_SCL0;
}
void I2C_SEND_ADD()
{
	unsigned int i = 0;
	unsigned int add = 0Xcb;
	I2C_SCL0;
	I2C_DELAY();
	for(i=0;i<8;i++)
	{
		if((add&0x80) == 0x80)
		{
			I2C_SDA1;
		}
		else
		{
			I2C_SDA0;
		}
		add<<=1;
		I2C_SCL1;
		I2C_DELAY();
		I2C_SCL0;
		I2C_DELAY();
	}
}
static uint8_t I2C_READ()
{
	uint8_t key = 0X00;
	unsigned int i = 0;
	unsigned int x = 0;
	for(i=0;i<8;i++)
	{
		I2C_SCL0;
		I2C_DELAY();
		I2C_SCL1;
		I2C_DELAY();
		x = I2C_ReadSDA;
		key <<= 1;
		if(x == 1)
		{
			key++;
		}
		I2C_DELAY();
		I2C_SCL0;
	}
	return key;
}
unsigned int I2C_ACK(void)
{
	unsigned int ack;
	I2C_SDA0;
	I2C_DELAY();
	I2C_SCL1;
	I2C_DELAY();
	I2C_SCL0;
	I2C_DELAY();
	I2C_SDA1;
	delay_us(100);
	return ack;
}
unsigned int I2C_NACK(void)
{
	I2C_SDA1;
	I2C_DELAY();
	I2C_SCL1;
	I2C_DELAY();
	I2C_SCL0;
	I2C_DELAY();
	
	return 0;
}
uint16_t KEY_scan(void)
{
	uint8_t byte_h,byte_l;
	uint16_t keyreturn = 0x0000;
	I2C_START();
	I2C_SEND_ADD();
	I2C_ACK();
	byte_l = I2C_READ();
	I2C_ACK();
	byte_h = I2C_READ();
	I2C_NACK();
	I2C_STOP();
	
	USART_SendByte(byte_h);
	USART_SendByte(byte_l);
	keyreturn = ((keyreturn|byte_h) <<8);
	keyreturn = keyreturn|byte_l;
	return keyreturn;
}
unsigned int key_ctrl(void)
{
	uint16_t key;
	unsigned int return_key;
	key = KEY_scan();
	if(key != 0x0000 && key != return_key)
	{
		switch(key)
		{
			case 0x0001:return_key = 1;break;
			case 0x0002:return_key = 4;break;
			case 0x0004:return_key = 7;break;
			case 0x0008:return_key = 14;break;
			
			case 0x0010:return_key = 2;break;
			case 0x0020:return_key = 5;break;
			case 0x0040:return_key = 8;break;
			case 0x0080:return_key = 0;break;
			
			case 0x0100:return_key = 3;break;
			case 0x0200:return_key = 6;break;
			case 0x0400:return_key = 9;break;
			case 0x0800:return_key = 15;break;
			
			case 0x1000:return_key = 10;break;
			case 0x2000:return_key = 11;break;
			case 0x4000:return_key = 12;break;
			case 0x8000:return_key = 13;break;
			case 0x0000:return_key = 16;break;
		}
			return return_key;
	}
	return 0xff;
}
