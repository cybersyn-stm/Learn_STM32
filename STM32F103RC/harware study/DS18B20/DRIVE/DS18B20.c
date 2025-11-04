#include "DS18B20.h"
void DS18B20_Mode(uint8_t mode)
{
	GPIOC->CRL &= 0xff0fffff;
	if(mode == 0)
	{
		GPIOC->CRL |= 0x00300000;//PC5 PP OUT
		GPIOC->ODR |= 1<<5;
	}
	if(mode == 1)
	{
		GPIOC->CRL |= 0x00800000;//PC4 PUT/UP IN
	}
}
void DS18B20_Write(uint8_t data)
{
	if(data == 1)
	{
		//GPIO_SetBits(GPIOC,GPIO_Pin_5);
		GPIOC->ODR |= 1<<5;
	}
	if(data == 0)
	{
		//GPIO_ResetBits(GPIOC,GPIO_Pin_5);
		GPIOC->ODR &= ~(1<<5);
	}
}
void DS18B20_init()
{
	DS18B20_Mode(0);
	DS18B20_Write(0);
	Systick_us(750);
	DS18B20_Write(1);
	Systick_us(15);
}
void DS18B20_Write_Data(uint8_t data)
{
	uint8_t i,testb;
	DS18B20_Mode(0);
	for(i=8;i>0;i--)
	{
		testb = data&0x01;
		data = data >> 1;
		if(testb)
		{
			DS18B20_Write(0);
			Systick_us(8);
			DS18B20_Write(1);
			Systick_us(58);
		}
		else
		{
			DS18B20_Write(0);
			Systick_us(70);
			DS18B20_Write(1);
			Systick_us(2);
		}
	}
}
uint8_t DS18B20_Read_Bit(void)
{
	uint8_t data;
	DS18B20_Mode(0);
	DS18B20_Write(0);
	Systick_us(10);
	DS18B20_Write(1);
	if(GPIOC->IDR & (1<<5))
	{
		data = 1;
	}
	else
	{
		data = 0;
	}
	Systick_us(45);
	
	return data;
}
//uint8_t DS18B20_Read_Data(void)
//{
//	uint8_t i,j,data = 0;
//	for(i = 0;i < 8;i++)
//	{
//		j = DS18B20_Read_Bit();
//		data = (data) | (j<<i);
//	}
//	return data;
//}
uint8_t DS18B20_ACK(void)
{
	uint8_t time = 0;
	
	DS18B20_Mode(1);
	while((GPIOC->IDR & (1<<5)) && time<100)
	{
		time++;
		Systick_us(1);
	}
	while(!GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5))
	{
		time++;
		Systick_us(1);
	}
	return 0;
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
		if(GPIOC->IDR & (1<<5))
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
uint16_t DS18B20_ReadTemp(void)
{
	uint8_t temp_high,temp_low;
	DS18B20_init();
	DS18B20_ACK();
	DS18B20_Write_Data(0xcc);
	DS18B20_Write_Data(0x44);
	DS18B20_init();
	DS18B20_ACK();
	DS18B20_Write_Data(0xcc);
	DS18B20_Write_Data(0xbe);
	
	temp_low = DS18B20_Read_Data();
	temp_high = DS18B20_Read_Data();
	
	return ((temp_high<<8)|temp_low)*0.0625*100;
}
