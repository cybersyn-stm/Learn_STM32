#include "stm32f10x.h"
typedef uint8_t (*architecture)(int);
uint8_t My_architecture(int x)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	if(x)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
	}
	else
	{
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	}
	return 0;
}
struct Set
{
	architecture my_gpio;
};
void register_architecture(struct Set *set, architecture arch , int value)//register function
{
    set->my_gpio = arch;
		set->my_gpio(value);
}
int main()
{
	struct Set my_set;
	register_architecture(&my_set, My_architecture,0);
}	
