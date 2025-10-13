#include "DS1306.h"
#include "delay.h"
void DS1306_DELAY(void)
{
}
void DS1306_Config_Init(void)
{
	GPIO_InitTypeDef GPIO_DS1306_Init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_DS1306_Init.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_DS1306_Init.GPIO_Pin = DS1306_SCL_PIN | DS1306_SDA_PIN;
	GPIO_DS1306_Init.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC,&GPIO_DS1306_Init);
	delay_ms(1000);
	DS1306_Write(1,0XAE);
	
	DS1306_Write(1,0x00);
	DS1306_Write(1,0xB0);
	
	DS1306_Write(1,0x81);
	
	DS1306_Write(1,0XC8);
	DS1306_Write(1,0x14);

	DS1306_Write(1,0xD5);
	DS1306_Write(1,0xD8);
	DS1306_Write(1,0xD9);
	DS1306_Write(1,0xDA);
	DS1306_Write(1,0xDB);
	DS1306_Write(1,0xAF);
	DS1306_Write(1,0XA5);
//	DS1306_Clear_Display();
}
void DS1306_START(void)
{
	DS1306_SDA1;
	DS1306_DELAY();
	DS1306_SCL1;
	DS1306_DELAY();
	DS1306_SDA0;
	DS1306_DELAY();
	DS1306_SCL0;
}
void DS1306_STOP(void)
{
	DS1306_SDA0;
	DS1306_DELAY();
	DS1306_SCL1;
	DS1306_DELAY();
	DS1306_SDA1;
	DS1306_DELAY();
}
void DS1306_ACK(void)
{
	DS1306_SCL0;
	DS1306_DELAY();
	DS1306_SDA0;
	DS1306_DELAY();
	DS1306_SCL1;
	DS1306_DELAY();
	DS1306_SCL0;
	DS1306_DELAY();
	DS1306_SDA1;
}
unsigned char DS1306_WAIT_ACK(void)
{
	unsigned int x;
	DS1306_SCL0;
	DS1306_DELAY();
	DS1306_SDA1;
	DS1306_DELAY();
	x = DS1306_READ_SDA;
	DS1306_SCL1;
	DS1306_DELAY();
	DS1306_SCL0;
	if(x == 1)
	{
		return 0;	
	}
	if(x == 0)
	{
		return 1;
	}
	
	return 0;
}
void DS1306_SEND(uint8_t data)
{
	uint8_t send_data;
	unsigned int i = 0;
	send_data = data;
	DS1306_SCL0;
	DS1306_DELAY();
	for(i=0;i<8;i++)
	{
			if((send_data&0x80) == 0x80)
			{
				DS1306_SDA1;
			}
			else
			{
				DS1306_SDA0;
			}
			send_data<<=1;
			DS1306_SCL1;
			DS1306_DELAY();
			DS1306_SCL0;
			DS1306_DELAY();
	}
}
void DS1306_Write(unsigned int ctrl,uint8_t data)
{
	DS1306_START();
	DS1306_SEND(0x78);//send add & write
	DS1306_WAIT_ACK();
	if(ctrl == 1)
	{
		DS1306_SEND(0x00);//cmd ctrl
	}
	if(ctrl == 0)
	{
		DS1306_SEND(0x40);//data ctrl
	}
	DS1306_WAIT_ACK();
	DS1306_SEND(data);
	DS1306_WAIT_ACK();
	DS1306_STOP();

}
void DS1306_Clear_Display(void)
{
	uint8_t i,n;
	for(i=0;i<4;i++)
	{
		DS1306_Write(1,0xb4+i);
		DS1306_Write(1,0x00);
		DS1306_Write(1,0x10);
		for(n=0;n<128;n++)
		{
			DS1306_Write(0,0xf0);
		}
	}
	DS1306_Write(1,0xb4);
	DS1306_Write(1,0x00);
	DS1306_Write(1,0x10);
}
void Write_Oled(unsigned char x_start,unsigned y_start,unsigned z_start,unsigned char OLED[][16])
{
	unsigned int i;
	DS1306_Write(1,0x20);
	DS1306_Write(1,0x10);
	DS1306_Write(1,0xb4 + y_start);
	DS1306_Write(1,(((x_start*16 & 0xf0)>>4) | 0x10));
	DS1306_Write(1,(x_start*16 & 0x0f));
	for(i = 0;i < 16;i++)
	{
		DS1306_Write(0,OLED[z_start][i]);
	}
	DS1306_Write(1,0xb4 + y_start + 1);
	DS1306_Write(1,(((x_start*16 & 0xf0)>>4) | 0x10));
	DS1306_Write(1,(x_start*16 & 0x0f));
	for(i = 0;i < 16;i++)
	{
		DS1306_Write(0,OLED[z_start+1][i]);
	}
}
void Write_Oled_English(unsigned char x_start,unsigned y_start,unsigned z_start,unsigned char OLED[][16])
{
	unsigned int i;
	DS1306_Write(1,0x20);
	DS1306_Write(1,0x10);
	DS1306_Write(1,0xb4 + y_start);
	DS1306_Write(1,(((x_start*8 & 0xf0)>>4) | 0x10));
	DS1306_Write(1,(x_start*8 & 0x0f));
	for(i = 0;i < 8;i++)
	{
		DS1306_Write(0,OLED[z_start][i]);
	}
	DS1306_Write(1,0xb4 + y_start + 1);
	DS1306_Write(1,(((x_start*8 & 0xf0)>>4) | 0x10));
	DS1306_Write(1,(x_start*8 & 0x0f));
	for(i = 8;i < 16;i++)
	{
		DS1306_Write(0,OLED[z_start][i]);
	}
}
void Set_px(unsigned char x,unsigned char y)
{
	DS1306_Write(1,0xb4 + y);
	DS1306_Write(1,((x & 0xf0)>>4 | 0x10));
	DS1306_Write(1,(x & 0x0f));
}
void Write_BMP(unsigned char x_start,unsigned char y_start,unsigned char x_stop,unsigned char y_stop,const unsigned char OLED[])
{
	unsigned int i,j,k;
	k = 0;
	Set_px(x_start,y_start);
	for(i=0;i<y_stop;i++)
	{
		for(j=0;j<x_stop;j++,k++)
		{
			DS1306_Write(0,OLED[k]);
		}
		Set_px(x_start,++y_start);
	}
}




