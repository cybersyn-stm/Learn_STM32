#include "stm32f10x.h"
#include "RCC.h"
#include "GPIO.h"
#include "TIM.h"
int main()
{
    GPIO_init();
    RCC_init();
    TIM2_init(1000);//1s
    while (1)
    {
        /* code */
    }
    
}	

