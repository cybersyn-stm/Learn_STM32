#include "TM1650.h"
#include "stm32f10x.h"
unsigned char LED_SEG[17] = {0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0X00};
unsigned char LED_DISPLAY[8] = {0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x00};
unsigned char LED_DIG[4] = {0x68,0x6a,0x6c,0x6e};
void SDA_IN(void)
{
	GPIO_InitTypeDef GPIO_SDA_IN;
	GPIO_SDA_IN.GPIO_Pin = TM1650_SDA_PIN;
	GPIO_SDA_IN.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(TM1650_SDA_GPIO,&GPIO_SDA_IN);
}
void SDA_OUT(void)
{
	GPIO_InitTypeDef GPIO_SDA_OUT;
	GPIO_SDA_OUT.GPIO_Pin = TM1650_SDA_PIN;
	GPIO_SDA_OUT.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_SDA_OUT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TM1650_SDA_GPIO,&GPIO_SDA_OUT);
}
void START(void)
{
	SDA_OUT();
	TM1650_SDA1;
	TM1650_SCL1;
	delay_us(4);
	TM1650_SDA0;
	delay_us(4);
	TM1650_SCL0;
}
void STOP(void)
{
	SDA_OUT();
	TM1650_SCL0;
	TM1650_SDA0;
	delay_us(4);
	TM1650_SCL1;
	delay_us(4);
	TM1650_SDA1;
}
void ACK(void)
{
	TM1650_SCL0;
	SDA_OUT();
	TM1650_SDA0;
	delay_us(4);
	TM1650_SCL1;
	delay_us(4);
	TM1650_SCL0;
}
void Send_Data(unsigned char Data)
{
	unsigned char t;
	SDA_OUT();
	for(t=0;t<8;t++)
	{
		if((Data&0x80) == 0x80)
		{
			TM1650_SDA1;
		}
		else
		{
			TM1650_SDA0;
		}
		Data<<=1;
		delay_us(4);
		TM1650_SCL1;
		delay_us(4);
		TM1650_SCL0;
		delay_us(4);
	}
}
void TM1650_Write_Data(uint16_t Add,uint16_t Data)
{
	START();
	Send_Data(Add);
	ACK();
	Send_Data(Data);
	ACK();
	STOP();
}
void TM1650_SetDisplay(int x,int ctrl)//
{
	if(ctrl ==1)
	{
		TM1650_Write_Data(0x48,(LED_DISPLAY[x]|0x01));
	}
	else
	{
		TM1650_Write_Data(0x48,0x00);
	}
}
void TM1650_Config_Init(unsigned int luminance)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_InitTypeDef GPIO_SCL_OUT;
	GPIO_SCL_OUT.GPIO_Pin = TM1650_SCL_PIN;
	GPIO_SCL_OUT.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_SCL_OUT.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(TM1650_SCL_GPIO,&GPIO_SCL_OUT);
	
	GPIO_SCL_OUT.GPIO_Pin = TM1650_SDA_PIN;
	GPIO_Init(TM1650_SDA_GPIO,&GPIO_SCL_OUT);
	
	TM1650_SCL0;
	TM1650_SDA0;
	
	TM1650_SetDisplay(7,0);
	TM1650_DigitalTube(16,1);
	TM1650_DigitalTube(16,2);
	TM1650_DigitalTube(16,3);
	TM1650_DigitalTube(16,4);
	
	TM1650_SetDisplay(luminance,1);
}
void TM1650_DigitalTube(int NUMBER,int DIG)
{
	TM1650_Write_Data(LED_DIG[DIG-1],LED_SEG[NUMBER]);
}









