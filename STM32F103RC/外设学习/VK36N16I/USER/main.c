#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "I2C.h"
#include "TIM.h"
#include "USART.h"
#include "VK36N16I.h"
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	I2C_GPIO_Init();
	while(1)
	{
		Systick_ms(1000);
		Key_return();
	}
}	



