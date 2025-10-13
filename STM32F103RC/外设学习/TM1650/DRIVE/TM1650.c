#include "TM1650.h"
uint8_t seg[17] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71 , 0x00};
void TM1650_1650GPIO_Init(void)
{
	GPIO_InitTypeDef TM1650_GPIO_Sturct;
	//OPEN GPIOC CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//SDA P11 SCL P12 OUT_OD
	TM1650_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
	TM1650_GPIO_Sturct.GPIO_Pin = TM1650_SDA_Pin|TM1650_SCL_Pin;
	TM1650_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&TM1650_GPIO_Sturct);
}
void TM1650_1650START()
{
	TM1650_SCL1;
	TM1650_SDA1;
	TM1650_SDA0;
	TM1650_SCL0;
}
void TM1650_1650STOP()
{
	TM1650_SDA0;
	TM1650_SCL1;
	TM1650_SDA1;
	TM1650_SCL0;
}
void TM1650_1650ReadACK(void)
{
	TM1650_SCL0;
	TM1650_SDA0;
	TM1650_SCL1;
	TM1650_SDA1;
}
void TM1650_1650WriteByte(uint8_t data)
{
	uint8_t i;
	for(i=0;i<8;i++)
	{
		TM1650_SCL0;
		if(((data&0x80)>>7))
		{
			TM1650_SDA1;
		}
		else
		{
			TM1650_SDA0;
		}
		data <<= 1;
		TM1650_SCL1;
	}
}
void TM1650_Write_1650(uint8_t address,uint8_t data)
{
	TM1650_1650START();
	TM1650_1650WriteByte(address);
	TM1650_1650ReadACK();
	TM1650_1650WriteByte(data);
	TM1650_1650ReadACK();
	TM1650_1650STOP();
}
void TM1650_init(void)
{
	TM1650_1650GPIO_Init();
	TM1650_Write_1650(0x48,0x01);
}
void TM1650_Cotrl(uint8_t seg_duan,uint8_t seg_data)
{
	TM1650_Write_1650(seg_duan*2 + 0X68,seg[seg_data]);
}
void TM1650_Clean(uint8_t data)
{
	if(data == 1)
	{
		TM1650_Write_1650(0X68,0x00);
	}
	if(data == 2)
	{
		TM1650_Write_1650(0X68,0x00);
		TM1650_Write_1650(1*2 + 0X68,0x00);
	}
	if(data == 3)
	{
		TM1650_Write_1650(0X68,0x00);
		TM1650_Write_1650(1*2 + 0X68,0x00);
		TM1650_Write_1650(2*2 + 0X68,0x00);
	}
	if(data == 4)
	{
		TM1650_Write_1650(0X68,0x00);
		TM1650_Write_1650(1*2 + 0X68,0x00);
		TM1650_Write_1650(2*2 + 0X68,0x00);
		TM1650_Write_1650(3*2 + 0X68,0x00);
	}
}
void TM1650_Dislay(uint16_t data)
{
	if(data >9999)
	{
		data = 9999;
	}
	else if(data < 10)
	{
		TM1650_Clean(3);
		TM1650_Cotrl(3,data%10);
	}
	else if(data < 100)
	{
		TM1650_Clean(2);
		TM1650_Cotrl(2,data/10%10);
		TM1650_Cotrl(3,data%10);
	}
	else if(data < 1000)
	{
		TM1650_Clean(1);
		TM1650_Cotrl(1,data/100%10);
		TM1650_Cotrl(2,data/10%10);
		TM1650_Cotrl(3,data%10);
	}
	else if(data < 10000)
	{
		TM1650_Cotrl(0,data/1000%10);
		TM1650_Cotrl(1,data/100%10);
		TM1650_Cotrl(2,data/10%10);
		TM1650_Cotrl(3,data%10);
	}
}








