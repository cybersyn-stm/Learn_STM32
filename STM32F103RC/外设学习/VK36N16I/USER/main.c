#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "I2C.h"
#include "TIM.h"
#include "USART.h"
#include "VK36N16I.h"
#include "TM1650.h"
uint8_t key;
uint8_t seg[16] = {0x3f, 0x06, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07, 0x7f, 0x6f, 0x77, 0x7c, 0x39, 0x5e, 0x79, 0x71};
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	I2C_GPIO_Init();
	I2C_1650GPIO_Init();
	I2C_Write_1650(0x48,0x01);
	while(1)
	{
		key = Key_Scan();
		if(key!=125)
		{
			USART1_SendChar(key);
			I2C_Write_1650(0x6A,seg[key]);
		}
	}
}	



