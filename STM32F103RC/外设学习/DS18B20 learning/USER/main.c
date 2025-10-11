#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "DS18B20.h"
int main(void)
{
	GPIO_init();
	RCC_init();
	USART1_init();
	DS18B20_init();
	while(1)
	{
		DS18B20_ReadTemp();
	}
}	







