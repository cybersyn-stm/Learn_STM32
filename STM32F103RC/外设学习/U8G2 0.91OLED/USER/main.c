#include "stm32f10x.h"
#include "u8g2.h"
#include "RCC.h"
#include "GPIO.h"
#include "USART.h"
static const unsigned char bmp1[] U8X8_PROGMEM ={
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X01,0X00,0X00,0X00,
0X00,0X00,0X00,0X80,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X80,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0XC0,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0XC0,0X00,0XE0,0X0F,0X00,
0X00,0X00,0X00,0X60,0X00,0XFC,0X7F,0X00,0X00,0X00,0X00,0X70,0X00,0XFF,0XFF,0X01,
0X00,0X00,0X00,0X70,0X80,0XFF,0XFF,0X03,0X00,0X00,0X00,0X78,0X80,0XFF,0XFF,0X07,
0X00,0XFC,0X01,0X38,0XC0,0XFF,0XFF,0X0F,0X00,0XFF,0X07,0X3C,0XC0,0XFF,0XFF,0X0F,
0X80,0XFF,0X0F,0X3E,0XE0,0XFF,0XFF,0X1F,0XC0,0XFF,0X3F,0X3E,0XE0,0XFF,0XFF,0X1F,
0XE0,0XFF,0X3F,0X3F,0X80,0XFF,0XFF,0X1F,0XE0,0XFF,0X9F,0XFF,0X07,0XFD,0XFF,0X1F,
0XF0,0XFF,0X9F,0XFF,0X7F,0XC0,0XFF,0X1F,0XF0,0XFF,0XCF,0XFF,0XFF,0X0F,0XFC,0X1F,
0XF0,0XFF,0XC7,0XFF,0XFF,0XFF,0XF9,0X1F,0XF8,0XFF,0XE7,0XFF,0XFF,0XFF,0XFD,0X1F,
0XF8,0XFF,0XF7,0XFF,0XFF,0XFF,0XFD,0X1F,0XF8,0XFF,0XF3,0XFF,0X01,0XFF,0XFE,0X1F,
0XF8,0XFF,0XF9,0X01,0X80,0X7F,0XFF,0X0F,0XF0,0XFF,0X00,0X00,0XC0,0XBF,0XFF,0X0F,
0XF0,0XFF,0X00,0XE0,0XC0,0XDF,0XFF,0X07,0XF0,0X7F,0XFF,0X7F,0XC0,0XCF,0XFF,0X03,
0XE0,0XFF,0XFF,0X7F,0XE0,0XE7,0XFF,0X03,0XE0,0XFF,0XFF,0X7F,0XF0,0XF7,0XFF,0X01,
0XC0,0XFF,0XFF,0X3F,0XF0,0XFB,0XFF,0X00,0XC0,0XFF,0XFF,0X3F,0XF8,0XFD,0X7F,0X00,
0X80,0XFF,0XFF,0X3F,0XF8,0XFC,0X3F,0X00,0X80,0XFF,0XFF,0X1F,0X7C,0XFE,0X1F,0X00,
0X00,0XFF,0XFF,0X1F,0X3C,0XFF,0X0F,0X00,0X00,0XFE,0XFF,0X1F,0XBE,0XFF,0X07,0X00,
0X00,0XFC,0XFF,0X0F,0XBE,0XFF,0X03,0X00,0X00,0XFC,0XFF,0X0F,0XCF,0XFF,0X00,0X00,
0X00,0XF8,0XFF,0X0F,0XF7,0X7F,0X00,0X00,0X00,0XF0,0XFF,0X87,0XF3,0XBF,0X01,0X00,
0X00,0XE0,0XFF,0XC7,0XF9,0X8F,0X00,0X00,0X80,0XF0,0XFF,0XC7,0XF9,0X63,0X0E,0X00,
0XF0,0XC9,0XFF,0XFB,0XFD,0XE5,0X91,0X3E,0X60,0X73,0XFF,0X73,0XFF,0X73,0XE8,0X14,
0XB0,0X65,0XFD,0XF3,0X7F,0XB2,0X08,0X02,0X90,0X32,0XF9,0XD9,0X2F,0X91,0X4C,0X01,
0X78,0X74,0XF3,0XCD,0X67,0XFB,0X99,0X07,0X08,0X00,0XE0,0XE5,0X00,0X00,0X20,0X00,
0X04,0X00,0XC0,0X06,0X00,0X00,0X00,0X00,0X1E,0X00,0X00,0X02,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,
0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,0X00,

};
uint8_t u8x8_gpio_and_delay(u8x8_t *u8x8, uint8_t msg, uint8_t arg_int, void *arg_ptr) {
    switch (msg) {
        case U8X8_MSG_DELAY_100NANO: // delay arg_int * 100 nano seconds
            __NOP();
            break;
        case U8X8_MSG_DELAY_10MICRO: // delay arg_int * 10 micro seconds
            for (uint16_t n = 0; n < 320; n++)
                __NOP();
            break;
        case U8X8_MSG_DELAY_MILLI:   // delay arg_int * 1 milli second
            Systick_ms(1);
            break;
        case U8X8_MSG_DELAY_I2C:     // arg_int is the I2C speed in 100KHz, e.g. 4 = 400 KHz
            Systick_us(5);
            break;                    // arg_int=1: delay by 5us, arg_int = 4: delay by 1.25us
        case U8X8_MSG_GPIO_I2C_CLOCK: // arg_int=0: Output low at I2C clock pin
            arg_int ? GPIO_SetBits(GPIOC, GPIO_Pin_12) : GPIO_ResetBits(GPIOC, GPIO_Pin_12);  
            break;                    // arg_int=1: Input dir with pullup high for I2C clock pin
        case U8X8_MSG_GPIO_I2C_DATA:  // arg_int=0: Output low at I2C data pin
            arg_int ? GPIO_SetBits(GPIOC, GPIO_Pin_11) : GPIO_ResetBits(GPIOC, GPIO_Pin_11);  
            break;                    // arg_int=1: Input dir with pullup high for I2C data pin
        case U8X8_MSG_GPIO_MENU_SELECT:
            u8x8_SetGPIOResult(u8x8, /* get menu select pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_NEXT:
            u8x8_SetGPIOResult(u8x8, /* get menu next pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_PREV:
            u8x8_SetGPIOResult(u8x8, /* get menu prev pin state */ 0);
            break;
        case U8X8_MSG_GPIO_MENU_HOME:
            u8x8_SetGPIOResult(u8x8, /* get menu home pin state */ 0);
            break;
        default:
            u8x8_SetGPIOResult(u8x8, 1); // default return value
            break;
    }
    return 1;
}
void u8g2_init(u8g2_t *u8g2)
{
	GPIO_InitTypeDef I2C_GPIO_Sturct;
	//OPEN GPIOC CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//SDA P11 SCL P12 OUT_OD
	I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
	I2C_GPIO_Sturct.GPIO_Pin = GPIO_Pin_11|GPIO_Pin_12;
	I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&I2C_GPIO_Sturct);
	
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2,U8G2_R0,u8x8_byte_sw_i2c,u8x8_gpio_and_delay);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2,0);
	u8g2_ClearBuffer(u8g2);
}
void ssd1306_init(void)
{
	u8g2_t u8g2;
	u8g2_init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_u8glib_4_tf);
	u8g2_DrawXBM(&u8g2, 32, 0, 64, 64, bmp1);
//	u8g2_DrawStr(&u8g2, 0, 4, "Hello,world.");
//	u8g2_DrawStr(&u8g2, 0, 9, "I hate this world.");
	u8g2_SendBuffer(&u8g2);
}
int main()
{
	RCC_init();
	GPIO_init();
	USART1_init();
	ssd1306_init();
	while(1)
	{
		
	}
}	















