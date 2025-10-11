#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "LED.h"
unsigned int x[4] = {0x01,0x02,0x04,0x08};
int main()
{
	GPIO_init();
	RCC_init();
	while(1)
	{
		LED_FLASH1();
		LED_FLASH2();
	}
}	
