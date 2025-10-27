#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "I2C.h"
#include "TIM.h"
#include "USART.h"
#include "TM1650.h"
#include "u8g2.h"
#include "U8G2_USER.h"
#include "KEYBOARD.h"
#include <stdio.h>
char key_buffer[34];
uint8_t key;
uint16_t i;
u8g2_t u8g2;
I2C_T i2c_t;
int main()
{
	GPIO_init();
	RCC_init();
	USART1_init();
	TM1650_init();
	u8g2_init(&u8g2,1);
	KEYBOARD_Init(&i2c_t);
	while(1)
	{
		key = KEYBOARD_Scan(&i2c_t);
		USART1_SendChar(key);
		
		sprintf(key_buffer, "key:%lu",key);
		u8g2_ClearBuffer(&u8g2);
    u8g2_SetFont(&u8g2, u8g2_font_7x13_te);
    u8g2_DrawStr(&u8g2, 0, 20, key_buffer);
    u8g2_SendBuffer(&u8g2);
	}
}	



