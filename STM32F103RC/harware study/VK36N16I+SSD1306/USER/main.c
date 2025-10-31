#include "stm32f10x.h"
#include "u8g2.h"
#include "RCC.h"
#include "U8G2_USER.h"
#include "VK36N16I.h"
#include "USART.h"
#include "TIM.h"
#include "ADC.h"
#include <stdio.h>
#include <string.h>
u8g2_t u8g2;
I2C_T i2c;
uint8_t key = 0,key_flag = 0,u8g2_flag = 0;
uint32_t ADC_Value = 0,ADC_Value_Sum = 0,i = 0;
uint32_t pwm_duty = 0;
char key_buff[32],ADC_buff[32],pwm_buff[32];
char key_set[12];
int main()
{
    RCC_init();
    USART1_init();
    TIM_rcc_gpio_init();
    u8g2_init(&u8g2,0);//harware i2c
    VK36N16I_init(&i2c);
    ADC1_init();
    PWM_Setting(0);
    while(1)    
    {
        PWM_Setting(pwm_duty);
        for (i = 0,ADC_Value_Sum = 0; i < 256; i++)
        {
            ADC_Value_Sum += ADC1_getChannelValue(ADC_Channel_0);
        }
        ADC_Value = (ADC_Value_Sum / 256);
        uint32_t mv = (uint32_t)ADC_Value * 3300 / 4095;
        if (key_flag == 1)
        {
            key = Key_Scan(&i2c);
            key_flag = 0;
            if (key == 10 && key != 11)
            {
                strcpy(key_set,"UP");
                pwm_duty++;
            }
            if(key == 11 && key != 10)
            {
                strcpy(key_set,"DOWN");
                pwm_duty--;
            }
            if (key == 0xff)
            {
                strcpy(key_set,"");
            }
        }
        if (u8g2_flag == 1)
        {
            u8g2_flag = 0;
            sprintf(ADC_buff,"ADC Setting: %lumV\n",mv);
            sprintf(key_buff,"Key Value: %s\n",key_set);
            sprintf(pwm_buff,"PWM Duty: %lu\n",pwm_duty);
            u8g2_ClearBuffer(&u8g2);
            u8g2_SetFont(&u8g2,u8g2_font_7x13_te);
            u8g2_DrawStr(&u8g2,0,10,key_buff);  
            u8g2_DrawStr(&u8g2,0,30,ADC_buff);
            u8g2_DrawStr(&u8g2,0,50,pwm_buff);
            u8g2_SendBuffer(&u8g2); 
        }
    }
}	
void TIM3_IRQHandler()
{
    if(TIM3->SR&(1<<0))
    {
        key_flag = 1;
        u8g2_flag = 1;
        TIM3->SR &= ~(1<<0);
    }
}