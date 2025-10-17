#ifndef __DS18B20_H
#define __DS18B20_H
#include "stm32f10x.h"
#include "RCC.h"
#include "USART.h"

#define HIGH  1
#define LOW   0
 
#define DS18B20_CLK     RCC_APB2Periph_GPIOA
#define DS18B20_PIN     GPIO_Pin_0                  
#define DS18B20_PORT		GPIOA
 
//???,???????????,?????????
#define DS18B20_DATA_OUT(a)	if (a)	\
					GPIO_SetBits(GPIOA,GPIO_Pin_0);\
					else		\
					GPIO_ResetBits(GPIOA,GPIO_Pin_0)
 //???????
#define  DS18B20_DATA_IN()	   GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
 
typedef struct
{
	u8  temp_int;	 	//???????
	u8  temp_deci;	 	//???????
	u8  check_sum;	 	//???
		                 
}DS18B20_Data_TypeDef;
 
u8 DS18B20_Init(void);
float DS18B20_Get_Temp(void);
 





#endif

