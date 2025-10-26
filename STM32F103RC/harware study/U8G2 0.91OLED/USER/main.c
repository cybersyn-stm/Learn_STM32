#include "stm32f10x.h"
#include "u8g2.h"
#include "RCC.h"
#include "GPIO.h"
#include "USART.h"
#include "bmp.h"
#include "u8g2_user.h"
u8g2_t u8g2;//Init u8g2 structure
int main()
{    
    uint8_t t;
	RCC_init();
	GPIO_init();
	USART1_init();
    u8g2_init(&u8g2,0);//Hardware I2C
	RCC_ClocksTypeDef RCC_Clocks;
	while(1)
	{
        RCC_GetClocksFreq(&RCC_Clocks);
        u8g2_ClearBuffer(&u8g2); 
//		if(++t >= 32) t = 1;
//		u8g2_DrawCircle(&u8g2,64,32,t,U8G2_DRAW_ALL);
//		u8g2_DrawCircle(&u8g2,32,32,t,U8G2_DRAW_ALL);
//		u8g2_DrawCircle(&u8g2,96,32,t,U8G2_DRAW_ALL);
		  u8g2_SetFont(&u8g2, u8g2_font_7x13_te); /*????*/
    	u8g2_DrawStr(&u8g2, 0, 20, "U");
		u8g2_SendBuffer(&u8g2);  
	}
}	















