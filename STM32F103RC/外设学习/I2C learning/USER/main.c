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
	I2C_Write(0x00,0xAe);
	I2C_Write(0x00,0x05);
	Systick_ms(1000);
	I2C_Write(0x00,0XAF);
	while(1)
	{
		
	}
}	




