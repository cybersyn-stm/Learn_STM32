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
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	VK36N16I_init();
	TM1650_init();
	while(1)
	{
		key = Key_Scan();
		USART1_SendChar(key);
	
		for(i = 0;i< 9999;i++)
		{
			Systick_ms(100);
			TM1650_Dislay(i);
		}
	}
}	



