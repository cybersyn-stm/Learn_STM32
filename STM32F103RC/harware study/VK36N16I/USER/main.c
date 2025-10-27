#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "I2C.h"
#include "TIM.h"
#include "USART.h"
#include "VK36N16I.h"
#include "TM1650.h"
uint8_t key;
uint16_t i;
I2C_T i2c;
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	VK36N16I_init(&i2c);
	TM1650_init();
	while(1)
	{
		key = Key_Scan(&i2c);
		USART1_SendChar(key);
	}
}	



