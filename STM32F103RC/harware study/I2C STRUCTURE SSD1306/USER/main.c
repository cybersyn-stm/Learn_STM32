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
void I2C_Write_SDA(uint8_t bit)
{
	if(bit)
	{
		GPIO_SetBits(GPIOC,GPIO_Pin_11);
	}
	else
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_11);
	}
}
void I2C_Write_SCL(uint8_t bit)
{
	GPIO_SetBits(GPIOC,GPIO_Pin_12);
}
uint8_t I2C_Read_SDA()
{
	return GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11);
}
void SSD1306_128X64_INIT(I2C_T *i2c_t)
{
	i2c_t->I2C_SETTING_INIT = Setting_I2C_GPIO;
	i2c_t->SDA_WRITE = I2C_Write_SDA;
	i2c_t->SCL_WRITE = I2C_Write_SCL;
	i2c_t->SDA_READ = I2C_Read_SDA;
	i2c_t->DELAY_US = Systick_us;

	i2c_t->I2C_SETTING_INIT();
}
I2C_T i2c_t;
int main()
{
	SSD1306_128X64_INIT(&i2c_t);
}	
