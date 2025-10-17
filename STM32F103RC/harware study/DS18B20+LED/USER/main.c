#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "DS18B20.h"
#include "I2C.h"
int main()
{
	uint16_t temp;
	GPIO_init();
	RCC_init();
	USART1_init();
	DS18B20_Init();
	TM1637_init(0x07);
	while(1)
	{
		SysTick_ms(10);
		temp = DS18B20_ReadTemperature();
		TM1637_Display(temp*625/100);
	}
}	



