#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "USART.h"
#include "DS18B20.h"
#include "u8g2.h"
#include <stdio.h>
#include "U8G2_USER.h"
u8g2_t u8g2;
uint32_t temp;
char buf[32];
int main(void)
{
	GPIO_init();
	RCC_init();
	USART1_init();
	DS18B20_init();
	u8g2_init(&u8g2,1);
	while(1)
	{
		temp = DS18B20_ReadTemp();
		u8g2_ClearBuffer(&u8g2);
		u8g2_SetFont(&u8g2, u8g2_font_7x13_te);
		sprintf(buf, "Temp: %d.%02d C", temp / 100, temp % 100);
		u8g2_DrawStr(&u8g2, 0, 10, buf);
		u8g2_SendBuffer(&u8g2);
	}
}	







