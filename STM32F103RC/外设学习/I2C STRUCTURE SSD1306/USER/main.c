#include "stm32f10x.h"
#include "RCC.h"
#include "GPIO.h"
#include "USART.h"
#include "I2C.h"
void Setting_I2C_GPIO(void)
{
	GPIO_InitTypeDef I2C_GPIO_Sturct;
	//OPEN GPIOC CLOCK
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//SDA P11 SCL P12 OUT_OD
	I2C_GPIO_Sturct.GPIO_Mode = GPIO_Mode_Out_OD;
	I2C_GPIO_Sturct.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11;
	I2C_GPIO_Sturct.GPIO_Speed = GPIO_Speed_50MHz;	
	GPIO_Init(GPIOC,&I2C_GPIO_Sturct);
}
I2C_T *i2c_t;
int main()
{
	i2c_t->I2C_SETTING_INIT = Setting_I2C_GPIO;
	
}	
