#include "SSD1306.h"
void SSD1306_Command(uint8_t data)
{
	I2C_START();
	I2C_WriteByte(0x78);//Address
	I2C_ACK();
	I2C_WriteByte(0x00);//Write Command = 0x00
	I2C_ACK();
	I2C_WriteByte(data);//data
	I2C_ACK();
	I2C_STOP();
}
void SSD1306_Data(uint8_t data)
{
	I2C_START();
	I2C_WriteByte(0x78);//Address
	I2C_ACK();
	I2C_WriteByte(0x40);//Write Command = 0x40
	I2C_ACK();
	I2C_WriteByte(data);//data
	I2C_ACK();
	I2C_STOP();
}
void SSD1306_Init(void)
{
	I2C_GPIO_Init();
	SSD1306_Command(0XAE);
//	
//	SSD1306_Command(0XD5);
//	SSD1306_Command(0X80);
//	
//	SSD1306_Command(0XA8);
//	SSD1306_Command(0X3F);
//	
//	SSD1306_Command(0XD3);
//	SSD1306_Command(0X00);
//	
//	SSD1306_Command(0X40);
//	SSD1306_Command(0XA1);

//	SSD1306_Command(0XC8);
//	SSD1306_Command(0XDA);
//	
//	SSD1306_Command(0X12);
//	SSD1306_Command(0X81);
//	
//	SSD1306_Command(0XCF);
//	SSD1306_Command(0XD9);
//	
//	SSD1306_Command(0XF1);
//	SSD1306_Command(0XDB);
//	
//	SSD1306_Command(0X30);
//	SSD1306_Command(0XA5);
//	
//	SSD1306_Command(0XA6);
//	SSD1306_Command(0X8D);
//	
//	SSD1306_Command(0X14);
//	SSD1306_Command(0XAF);

}







