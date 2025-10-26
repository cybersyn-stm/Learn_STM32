#include "Head_FILE.h"
u8g2_t u8g2;//Init u8g2 structure
char ADC_buf[32];
int main()
{    
    uint32_t ADC_Value,i,j,ADC_Value_Sum;
    RCC_init();
    GPIO_init();
    USART1_init();
    u8g2_init(&u8g2,0);//Hardware I2C
    //RCC_ClocksTypeDef RCC_Clocks;
    ADC1_init();
    TIM_rcc_gpio_init();
    PWM_Setting(0);
    while(1)
    {
        //RCC_GetClocksFreq(&RCC_Clocks);
        for (i = 0,ADC_Value_Sum = 0; i < 256; i++)
        {
            ADC_Value_Sum += ADC1_getChannelValue(ADC_Channel_0);
        }
        ADC_Value = (ADC_Value_Sum / 256);
        uint32_t mv = (uint32_t)ADC_Value * 3300 / 4095;
        sprintf(ADC_buf, "Conversion:%lumV",mv);
        u8g2_ClearBuffer(&u8g2);
        u8g2_SetFont(&u8g2, u8g2_font_7x13_te);
        u8g2_DrawStr(&u8g2, 0, 20, ADC_buf);
        u8g2_SendBuffer(&u8g2);
        PWM_Setting(j++);
		Systick_ms(1);
    }
}
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update); 
    }
}















