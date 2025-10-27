#include "Keyboard.h"
void KEYBOARD_SCL_Write(uint8_t data)
{
    if (data)
    {
        GPIO_SetBits(GPIOC,GPIO_Pin_12);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_12);
    }
     Systick_us(100);
}
void KEYBOARD_SDA_Write(uint8_t data)
{
    if (data)
    {
       GPIO_SetBits(GPIOC,GPIO_Pin_11);
    }
    else
    {
        GPIO_ResetBits(GPIOC,GPIO_Pin_11);
    }
    Systick_us(100);
}
uint8_t KEYBOARD_SDA_Read(void)
{
    Systick_us(100);
   	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
}
void KEYBOARD_GPIO_Init(void)
{
    //KEY BOARD GPIO Init
	GPIO_InitTypeDef I2C_GPIO_Sturct;
	//OPEN GPIOC CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//SDA P11 SCL P12 OUT_OD
	I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
	I2C_GPIO_Sturct.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&I2C_GPIO_Sturct);
}
void KEYBOARD_Init(I2C_T *keyboard_i2c)
{
    keyboard_i2c->I2C_SETTING_INIT = KEYBOARD_GPIO_Init;
    keyboard_i2c->SCL_WRITE = KEYBOARD_SCL_Write;
    keyboard_i2c->SDA_WRITE = KEYBOARD_SDA_Write;
    keyboard_i2c->SDA_READ = KEYBOARD_SDA_Read; 
    keyboard_i2c->DELAY_US = Systick_us;
    keyboard_i2c->I2C_SETTING_INIT();
}
uint16_t KEYBOARD_Read(I2C_T *keyboard_i2c)
{
    uint16_t low_data = 0,high_data = 0,key_data;

    I2C_START(keyboard_i2c);
    I2C_WRITE_BYTE(keyboard_i2c, 0xCB);
    I2C_ACK(keyboard_i2c);
    low_data = I2C_READ_BYTE(keyboard_i2c);
    I2C_ACK(keyboard_i2c);  
    high_data = I2C_READ_BYTE(keyboard_i2c);
    I2C_STOP(keyboard_i2c);


    key_data = high_data;
    key_data <<= 8;
    key_data |= low_data;   
    return key_data;
}
uint8_t KEYBOARD_Scan(I2C_T *keyboard_i2c)
{
    uint16_t key = 0x00;
	uint8_t return_key = 0x00;
	key = KEYBOARD_Read(keyboard_i2c);
	switch(key)
	{
		case 0x0001:return_key = 1;break;
		case 0x0002:return_key = 4;break;
		case 0x0004:return_key = 7;break;
		case 0x0008:return_key = 15;break;
		
		case 0x0010:return_key = 2;break;
		case 0x0020:return_key = 5;break;
		case 0x0040:return_key = 8;break;
		case 0x0080:return_key = 0;break;
		
		case 0x0100:return_key = 3;break;
		case 0x0200:return_key = 6;break;
		case 0x0400:return_key = 9;break;
		case 0x0800:return_key = 14;break;
		
		case 0x1000:return_key = 10;break;
		case 0x2000:return_key = 11;break;
		case 0x4000:return_key = 12;break;
		case 0x8000:return_key = 13;break;
		default:return_key = 125;break;
	}
	return return_key;
}
















