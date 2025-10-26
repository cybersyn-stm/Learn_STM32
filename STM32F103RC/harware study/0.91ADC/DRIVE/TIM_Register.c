// ...existing code...
#include "TIM.h"
#include "stm32f10x.h"

/* 使用寄存器直接配置 TIM2 CH2(PA1) 为 PWM 输出
   freqHz: 目标 PWM 频率 (Hz)
   duty_percent: 初始占空比 0-100
   返回 0 成功，-1 参数错误或无法配置 */
int TIM2_pwm_init_reg(uint32_t freqHz, uint8_t duty_percent)
{
    if (freqHz == 0 || duty_percent > 100) return -1;

    /* 1) 使能时钟：GPIOA + TIM2 */
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;      // GPIOA
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;      // TIM2

    /* 2) 配置 PA1 为复用推挽 (AF push-pull)
       CRL 每 4 位为一个引脚，PIN1 位偏移 4 */
    GPIOA->CRL &= ~(0xFUL << (4 * 1));
    /* MODE = 11 (50MHz), CNF = 10 (AF push-pull) => 0b1011 = 0xB */
    GPIOA->CRL |= (0xB << (4 * 1));

    /* 3) 计算定时器时钟 (tim_clk)
       注意 STM32F1: 当 APB1 分频 >1 时，定时器时钟 = PCLK1 * 2 */
    uint32_t ppre1 = (RCC->CFGR >> 8) & 0x7;
    uint32_t pclk1_div;
    if (ppre1 < 4) pclk1_div = 1;
    else pclk1_div = 1u << (ppre1 - 3); /* 4->2,5->4,6->8,7->16 */

    uint32_t pclk1 = SystemCoreClock / pclk1_div;
    uint32_t tim_clk = (ppre1 < 4) ? pclk1 : (pclk1 * 2u);

    /* 4) 计算 (PSC+1)*(ARR+1) = period_counts = tim_clk / freqHz
       为保证 ARR <= 0xFFFF，选择合适 PSC */
    uint64_t period_counts = tim_clk / freqHz;
    if (period_counts == 0) period_counts = 1;

    uint32_t psc = (uint32_t)((period_counts - 1) / 0x10000u); /* floor(period/65536) */
    if (psc > 0xFFFFu) psc = 0xFFFFu;
    uint32_t arr = (uint32_t)(period_counts / (psc + 1u));
    if (arr == 0) arr = 1;
    if (arr > 0xFFFFu) arr = 0xFFFFu;
    arr = arr - 1u;

    /* 5) 配置时基寄存器 */
    TIM2->PSC = psc;          /* 预分频 */
    TIM2->ARR = arr;          /* 自动重装载 */
    TIM2->EGR = TIM_EGR_UG;   /* 产生更新事件，立刻加载 PSC/ARR */

    /* 6) 配置 CH2 为 PWM1 模式并开启 CCR2 预装载
       CCMR1: OC2M[14:12] = 110 (PWM mode 1), OC2PE (bit11) = 1 */
    TIM2->CCMR1 &= ~((7u << 12) | (1u << 11));
    TIM2->CCMR1 |= (6u << 12) | (1u << 11);

    /* 7) 设置极性并使能 CH2 输出：CCER CC2E (bit4) = 1, CC2P = 0(高电平有效) */
    TIM2->CCER &= ~((1u << 5)); /* 确保为主动高 */
    TIM2->CCER |= (1u << 4);    /* 使能输出 */

    /* 8) 使能 ARR 预装载 */
    TIM2->CR1 |= TIM_CR1_ARPE;

    /* 9) 设置初始占空比 */
    uint32_t ccr2 = ((uint32_t)duty_percent * ( (uint32_t)arr + 1u )) / 100u;
    TIM2->CCR2 = ccr2;

    /* 10) 启动定时器计数器 */
    TIM2->CR1 |= TIM_CR1_CEN;

    return 0;
}

/* 运行时设置占空比 (0-100) */
void TIM2_setDuty_reg(uint8_t duty_percent)
{
    if (duty_percent > 100) duty_percent = 100;
    uint32_t arr = TIM2->ARR;
    uint32_t ccr2 = ((uint32_t)duty_percent * (arr + 1u)) / 100u;
    TIM2->CCR2 = ccr2;
}
// ...existing code...