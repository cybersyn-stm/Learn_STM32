#ifndef __DS1306_
#define __DS1306_
#include "stm32f10x.h"
#include "USART.h"


#define DS1306_SDA_GPIO GPIOC
#define DS1306_SCL_GPIO GPIOC
#define DS1306_SDA_PIN GPIO_Pin_11
#define DS1306_SCL_PIN GPIO_Pin_12

#define DS1306_SCL1 GPIO_SetBits(DS1306_SCL_GPIO,DS1306_SCL_PIN)
#define DS1306_SCL0 GPIO_ResetBits(DS1306_SCL_GPIO,DS1306_SCL_PIN)
#define DS1306_SDA1 GPIO_SetBits(DS1306_SDA_GPIO,DS1306_SDA_PIN)
#define DS1306_SDA0 GPIO_ResetBits(DS1306_SDA_GPIO,DS1306_SDA_PIN)

#define DS1306_READ_SDA GPIO_ReadInputDataBit(DS1306_SDA_GPIO,DS1306_SDA_PIN)

void DS1306_START(void);


void DS1306_Config_Init(void);
void DS1306_Write(unsigned int ctrl,uint8_t data);
void DS1306_Clear_Display(void);

void Write_Oled(unsigned char x_start,unsigned y_start,unsigned z_start,unsigned char OLED[][16]);
void Set_px(unsigned char x,unsigned char y);
void Write_Oled_English(unsigned char x_start,unsigned y_start,unsigned z_start,unsigned char OLED[][16]);
void Set_px(unsigned char x,unsigned char y);
void Write_BMP(unsigned char x_start,unsigned char y_start,unsigned char x_stop,unsigned char y_stop,const unsigned char OLED[]);



#endif

