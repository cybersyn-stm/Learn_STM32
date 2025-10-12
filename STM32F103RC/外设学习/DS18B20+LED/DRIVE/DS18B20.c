#include "DS18B20.h"
void DS18B20_Mode(uint8_t mode)
{
	GPIOA->CRL &= 0xfffffff0;
	if(mode == 0)
	{
		GPIOA->CRL |= 0x00000003;//PA0 PP OUT
		GPIOA->ODR |= 1<<0;
	}
	if(mode == 1)
	{
		GPIOA->CRL |= 0x00000008;//PA PUT/UP IN 
	}
}
void DS18B20_Write(uint8_t data)
{
	if(data == 1)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_0);
	}
	if(data == 0)
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_0);
	}
}
void DS18B20_Init(void)
{
	uint8_t time = 0;
	DS18B20_Mode(0);
	DS18B20_Write(0);
	Systick_us(500);
	DS18B20_Write(1);
	DS18B20_Mode(1);
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) == 1 && time<100)
	{
		time++;
		Systick_us(1);
	}
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)) == 0 && time<240)
	{
		time++;
		Systick_us(1);
	}
}
void DS18B20_Write_Data(uint8_t data)
{
	uint8_t i,testb;
	DS18B20_Mode(0);
	for(i=8;i>0;i--)
	{
		testb = data&0x01;
		data = data>>1;
		if(testb)
		{
			DS18B20_Write(0);
			Systick_us(6);
			DS18B20_Write(1);
			Systick_us(60);
		}
		else
		{
			DS18B20_Write(0);
			Systick_us(70);
			DS18B20_Write(1);
			Systick_us(1);
		}
	}
}
uint8_t DS18B20_Read_Data(void)
{
	uint8_t data,i;
	DS18B20_Mode(0);
	for(i=8;i>0;i--)
	{
		DS18B20_Write(0);
		Systick_us(5);
		DS18B20_Write(1);
		if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0) == SET)
		{
			data |= 0x80;
			data = data>>1;
		}
		else
		{
			data |= 0x00;
			data = data>>1;
		}
		Systick_us(55);
	}
	return data;
}
uint16_t DS18B20_ReadTemperature(void)
{
	uint8_t low_data,high_data;
	uint16_t data;
	//DS18B20 START TRANS
	DS18B20_Init();
	DS18B20_Write_Data(0xcc);
	DS18B20_Write_Data(0x44);
	//DS18B20 READ DATA
	DS18B20_Init();
	DS18B20_Write_Data(0xcc);
	DS18B20_Write_Data(0xbe);
	
	low_data = DS18B20_Read_Data();
	high_data = DS18B20_Read_Data();
	USART1_SendChar(high_data);
	USART1_SendChar(low_data);
	data = high_data;
	data = data<<8;
	data |= low_data;
	
	return data;
}


