#include "VK36N16I.h"
uint16_t Key_Read(uint8_t device_address)
{
	uint16_t low_data = 0,high_data = 0,return_data;
	
	I2C_START();
	I2C_WriteByte(device_address);
	I2C_ACK();
	high_data = I2C_ReadByte();
	I2C_ACK();
	low_data = I2C_ReadByte();
	I2C_STOP();
	
	USART1_SendChar(high_data);
	USART1_SendChar(low_data);
	
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

