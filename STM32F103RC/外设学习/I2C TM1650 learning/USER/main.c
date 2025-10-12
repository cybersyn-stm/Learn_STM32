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
	I2C_Write(0x48,0x01);
	I2C_Write(0x68,0x3fW);
	while(1)
	{
		
	}
}	




