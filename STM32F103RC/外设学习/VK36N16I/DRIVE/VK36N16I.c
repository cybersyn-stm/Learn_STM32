#include "VK36N16I.h"
void VK36N16I_init()
{
	I2C_GPIO_Init();
}
uint16_t Key_Read(uint8_t device_address)
{
	uint16_t low_data = 0,high_data = 0,return_data;
	
	I2C_START();
	I2C_WriteByte(device_address);
	I2C_ACK();
	low_data = I2C_ReadByte();
	I2C_ACK();
	high_data = I2C_ReadByte();
	I2C_STOP();
	
	return_data = high_data;
	return_data = return_data<<8;
	return_data |= low_data;
	return return_data;
}
uint16_t Key_return(void)
{
	uint16_t data;
	data = Key_Read(0XCB);
	return data;
}
uint8_t Key_Scan(void)
{
	uint16_t key = 0x00;
	uint8_t return_key = 0x00;
	key = Key_return();
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









