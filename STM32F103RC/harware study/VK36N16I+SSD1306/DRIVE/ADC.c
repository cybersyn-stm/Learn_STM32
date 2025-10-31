#include "ADC.h"
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_adc.h"

void ADC1_init(void)
{
    GPIO_InitTypeDef ADC_GPIO_Initstructure;
    ADC_InitTypeDef ADC_Setting_Instructure;

    /* 开时钟：GPIOA + ADC1 */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_ADC1, ENABLE);

    /* 设置 ADC 时钟为 PCLK2/6（确保 ADCCLK 在规范范围内） */
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);

    /* GPIO 配置 PA0 为模拟输入 */
    ADC_GPIO_Initstructure.GPIO_Mode = GPIO_Mode_AIN;
    ADC_GPIO_Initstructure.GPIO_Pin = GPIO_Pin_0;
    ADC_GPIO_Initstructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &ADC_GPIO_Initstructure);

    /* ADC 复位并初始化结构体 */
    ADC_DeInit(ADC1);
    ADC_Setting_Instructure.ADC_Mode = ADC_Mode_Independent;
    ADC_Setting_Instructure.ADC_ScanConvMode = DISABLE;
    ADC_Setting_Instructure.ADC_ContinuousConvMode = DISABLE; /* 单次转换 */
    ADC_Setting_Instructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    ADC_Setting_Instructure.ADC_DataAlign = ADC_DataAlign_Right;
    ADC_Setting_Instructure.ADC_NbrOfChannel = 1;

    /* 写入 ADC 配置 */
    ADC_Init(ADC1, &ADC_Setting_Instructure);

    /* 配置通道0（rank=1），采样时间较长以提高稳定性 */
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_239Cycles5);

    /* 使能 ADC，并在使能后短延时再校准 */
    ADC_Cmd(ADC1, ENABLE);
    for (volatile int i = 0; i < 1000; ++i) { __asm__("nop"); }

    /* 校准 */
    ADC_ResetCalibration(ADC1);
    while (ADC_GetResetCalibrationStatus(ADC1));
    ADC_StartCalibration(ADC1);
    while (ADC_GetCalibrationStatus(ADC1));
}

uint16_t ADC1_getChannelValue(uint8_t channel)
{
    if (channel > 15) return 0;

    ADC_RegularChannelConfig(ADC1, channel, 1, ADC_SampleTime_239Cycles5);
    ADC_ClearFlag(ADC1, ADC_FLAG_EOC);
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);

    while (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == RESET);

    return (uint16_t)ADC_GetConversionValue(ADC1);
}



