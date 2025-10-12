#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "TIM.h"
#include "I2C.h"
#include "USART.h"
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	I2C_GPIO_Init();
	while(1)
	{
		
	}
}	




