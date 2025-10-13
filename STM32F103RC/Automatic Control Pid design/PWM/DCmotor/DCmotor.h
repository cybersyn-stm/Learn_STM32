#ifndef __DCmotor_H_
#define __DCmotor_H_

#include "stm32f10x.h"
#include "DELAY.h"
#define IN1_GPIO GPIOC
#define IN2_GPIO GPIOC
#define Speed_GPIO GPIOB
#define IN1_PIN GPIO_Pin_11
#define IN2_PIN GPIO_Pin_12
#define A_Speed_PIN GPIO_Pin_6 //Green CH1
#define B_Speed_PIN GPIO_Pin_7 //Yellow CH2

#define Set_IN1 GPIO_SetBits(IN1_GPIO,IN1_PIN)
#define Set_IN2 GPIO_SetBits(IN2_GPIO,IN2_PIN)
#define Reset_IN1 GPIO_ResetBits(IN1_GPIO,IN1_PIN)
#define Reset_IN2 GPIO_ResetBits(IN2_GPIO,IN2_PIN)

void Motor_Ctrl(unsigned int ctrl);
void Motor_Config(void);
int16_t Motor_Speed_Get(void);
void motor(unsigned int key);



#endif


