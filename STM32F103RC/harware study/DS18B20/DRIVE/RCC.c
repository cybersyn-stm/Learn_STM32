#include "RCC.h"
void RCC_init(void)
{
 RCC_DeInit();

    /* 打开 HSE */
    RCC_HSEConfig(RCC_HSE_ON);
    if (RCC_WaitForHSEStartUp() == SUCCESS)
    {
        /* 总线分频设置 */
        RCC_HCLKConfig(RCC_SYSCLK_Div1);   /* HCLK = SYSCLK */
        RCC_PCLK2Config(RCC_HCLK_Div1);    /* APB2 = HCLK */
        RCC_PCLK1Config(RCC_HCLK_Div2);    /* APB1 = HCLK/2 (<=36MHz) */

        /* PLL = HSE * 9 -> 如果 HSE = 8MHz 则 PLL = 72MHz */
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {;}

        /* 选择 PLL 作为系统时钟 */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        while (RCC_GetSYSCLKSource() != 0x08) {;} /* 0x08 表示 PLL 作为系统时钟 */

        /* 更新库中的 SystemCoreClock 变量 */
        SystemCoreClockUpdate();
    }
	SysTick->CTRL |= 1<<2;//SYSTICK USE HCLK
}
void Systick_us(unsigned int time)
{
	uint32_t temp;
	SysTick->LOAD = (time * 72)-1;
	SysTick->VAL |= 0x00;//CLEAN SYSTICK
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//OPEN SYSTICK
	
	do{
		temp = SysTick->CTRL;
	}while(temp & 0x01 && !(temp & (1<<16)));
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;
	SysTick->VAL |= 0X00;
}
void Systick_ms(unsigned int time)
{
	for(;time>0;time--)
	{
		Systick_us(1000);
	}
}
