#include "stm32f10x.h"
#include "GPIO.h"
#include "RCC.h"
#include "TIM.h"
int main()
{
	RCC_init();
	GPIO_init();
	TIM2_init(1000);
	while(1)
	{
	
	}
}	

