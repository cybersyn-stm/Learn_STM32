#include "DCmotor.h"
void DCmotor_GPIO_Init(void)
{
	//OPEN GPIO_RCC
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	//GPIO_Speed
	GPIO_InitTypeDef GPIO_Motor_Speed_GPIO;
	//GPIO_IN
	GPIO_InitTypeDef GPIO_Motor_IN_GPIO;
	//Motor_Speed TIM4_CH1|TIM4_CH2
	GPIO_Motor_Speed_GPIO.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Motor_Speed_GPIO.GPIO_Pin = A_Speed_PIN|B_Speed_PIN;
	GPIO_Motor_Speed_GPIO.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Speed_GPIO,&GPIO_Motor_Speed_GPIO);
	//Morot_IN 
	GPIO_Motor_IN_GPIO.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Motor_IN_GPIO.GPIO_Pin = IN1_PIN|IN2_PIN;
	GPIO_Motor_IN_GPIO.GPIO_Speed  = GPIO_Speed_50MHz;
	GPIO_Init(IN1_GPIO,&GPIO_Motor_IN_GPIO);
}
void DCmotor_TIM_Init(void)
{
	//OPEN RCC_TIM4
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	//TIM 
	TIM_TimeBaseInitTypeDef TIM_Motor_Speed;
	TIM_ICInitTypeDef TIM_ICInitStructures;
	//TIM_Init
	TIM_Motor_Speed.TIM_Period = 65536 - 1;
	TIM_Motor_Speed.TIM_Prescaler = 1 - 1;
	TIM_Motor_Speed.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_Motor_Speed.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4,&TIM_Motor_Speed);
	//TIM_IC
	TIM_ICInitStructures.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructures.TIM_ICFilter = 0X0F;
	TIM_ICInit(TIM4,&TIM_ICInitStructures);
	TIM_ICInitStructures.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructures.TIM_ICFilter = 0X0F;
	TIM_ICInit(TIM4,&TIM_ICInitStructures);
	
	TIM_EncoderInterfaceConfig(TIM4,TIM_EncoderMode_TI12,TIM_ICPolarity_Rising,TIM_ICPolarity_Rising);
}
void Motor_Config(void)
{
	DCmotor_GPIO_Init();
	DCmotor_TIM_Init();
	TIM_Cmd(TIM4,ENABLE);
}
void Motor_Ctrl(unsigned int ctrl)
{
	switch(ctrl)
	{
		case 1://forward
		{
			Reset_IN1;
			Set_IN2;
		};break;
		case 2://reversal
		{
			Reset_IN2;
			Set_IN1;
		};break;
		case 3://stop
		{
			Reset_IN1;
			Reset_IN2;
		};break;
	}
}
void motor(unsigned int key)
{
		if(key == 10)
		{
			Motor_Ctrl(1);
		}
		if(key == 11)
		{
			Motor_Ctrl(2);
		}
		if(key == 15)
		{
			Motor_Ctrl(3);
		}
}











