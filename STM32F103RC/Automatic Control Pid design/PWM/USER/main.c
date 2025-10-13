#include "stm32f10x.h"
#include "DELAY.h"
#include "USART.h"
#include "TIM.h"
#include "DCmotor.h"
#include "I2C.h"
#include "DS1306.h"
#include "OLD.h"
#include "pid.h"
#define N 20
#define S 6.19
unsigned int key;
unsigned int j;
uint16_t TIM2_Number = 0;
uint16_t TIM5_Number = 0;
uint16_t Count = 0;
uint16_t PWM_Ctrl_Mode = 3;
uint16_t PWM_SPEED_SET = 0,PWM_SPEED_SET_COUNT = 0;
uint32_t speed_sum[N+1] = {0};
uint16_t change = 0;
unsigned int speed_count = 0;
unsigned int speed_ctrl = 0;
float RPM;
uint16_t Return_Speed(void)
{
	uint16_t Temp;
	Temp = TIM_GetCounter(TIM4);
	return Temp;
}
float Motor_Speed(uint16_t temp)
{
		float speed = 0;
		if(PWM_Ctrl_Mode == 0)
		{
			speed = temp*(S);
		}
		if(PWM_Ctrl_Mode == 1)
		{
			if(temp != 0)
			{
				speed = (65535 - temp)*(S);
			}
			if(temp == 0)
			{
				speed = 0;
			}
		}
		return speed;
}
void TIM2_IRQHandler(void)
{
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM2_Number++;
}
void TIM5_IRQHandler(void)
{	
	TIM_ClearFlag(TIM5,TIM_FLAG_Update);
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	if(TIM5_Number == 2)
	{
		if(key == 12)
		{
			pid.Sv += 1;
		}
		if(key == 13)
		{
			pid.Sv -= 1;
		}
		if(key == 10)
		{
			PWM_Ctrl_Mode = 0;
		}
		if(key == 11)
		{
			PWM_Ctrl_Mode = 1;
		}
		TIM5_Number = 0;
	}
	TIM5_Number++;
}
void display(float speed_number)
{
		unsigned int speed,speed2;
		speed2 = pid.Sv;
		speed = speed_number;
		Write_Oled(0,2,0,oled);
		Write_Oled(1,2,2,oled);
		Write_Oled(0,0,4,oled);
		Write_Oled(1,0,6,oled);
		
		Write_Oled(5,1,8,oled);
		Write_Oled(6,1,10,oled);
		Write_Oled_English(14,1,11,oled_math);
		Write_Oled_English(15,1,PWM_Ctrl_Mode,oled_english);
		
		Write_Oled_English(4,0,11,oled_math);
		Write_Oled_English(5,0,speed2/1000,oled_math);
		Write_Oled_English(6,0,speed2/100%10,oled_math);
		Write_Oled_English(7,0,speed2/10%10,oled_math);
		Write_Oled_English(8,0,speed2%10,oled_math);
	
		Write_Oled_English(4,2,11,oled_math);
		Write_Oled_English(5,2,speed/1000,oled_math);
		Write_Oled_English(6,2,speed/100%10,oled_math);
		Write_Oled_English(7,2,speed/10%10,oled_math);
		Write_Oled_English(8,2,speed%10,oled_math);
}
float ValueFilter(uint32_t data)
{
	unsigned int i;
	unsigned int Filter_coe[20] = {1,2,3,4,5,6,7,8,9,10,11,20,13,14,15,16,17,18,19,20};
	uint64_t Sum = 0,Value = 0;
	speed_sum[N] = data;
	for(i=0;i<N;i++)
	{
		speed_sum[i] = speed_sum[i+1];
		Sum += (speed_sum[i] * Filter_coe[i]);
	}	
	Value = Sum/210;		
	return Value;
}
int main(void)
{
  SysTick_Init();
//	PID_Init();
	I2C_Config_Init();
	DS1306_Config_Init ();
	USART_Config_Init();
//	TIM_Config_Init();
//	TIM_Ctrl(100,720,0);
//	TIM_SetCompare1(TIM3,0);
//	TIM_Handler(100,7200-1);	
//	Motor_Config();
//	Motor_Ctrl(3);
	while(1)
	{
//		if(TIM2_Number == 5)//50ms
//		{
//			if(change > 0)
//			{
//				change--;
//			}
//			key = key_ctrl();
//			if(key == 11 || key == 10)
//			{
//				change = 20;
//			}
//			motor(key);
//			Usart_Send_16bit_Data(RPM);
//			Count = Return_Speed();
//			RPM = Motor_Speed(Count);
//			RPM = ValueFilter(RPM);
//			display(RPM);	
//			if(change == 0)
//			{
//				PID_Calc(RPM);
//			}
//			TIM_SetCompare1(TIM3,pid.OUT);		
//			TIM_SetCounter(TIM4,0);
//			TIM2_Number = 0;
//				delay_ms(1000);
//				key_ctrl();
		}
	}

