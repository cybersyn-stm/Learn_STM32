#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
int main(void)
{
	GPIO_init();
	RCC_init();
	USART1_init();
	while(1)
	{
		USART1_SendChar(0xfe);
	}
}	







