#include "USART.h"
void USART_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_USART_Init;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	GPIO_USART_Init.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_USART_Init.GPIO_Pin = GPIO_Pin_9;
	GPIO_USART_Init.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_USART_Init);
}
void USART_Config_Init(void)
{
	USART_GPIO_Init();
	USART_InitTypeDef USART_Config;
	USART_DeInit(USART1);
	USART_Config.USART_BaudRate  = 9600;
	USART_Config.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Config.USART_Mode = USART_Mode_Tx;
	USART_Config.USART_Parity = USART_Parity_No;
	USART_Config.USART_StopBits = USART_StopBits_1;
	USART_Config.USART_WordLength = USART_WordLength_8b;
	USART_Init(USART1,&USART_Config);
	
	USART_Cmd(USART1,ENABLE);
}
void USART_SendByte(uint16_t Byte)
{
	USART_SendData(USART1,Byte);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}
void USART_SendHalfWord(uint16_t Byte)
{
	uint8_t temp_h,temp_l;
	temp_h = (Byte>>8)&0xFF;
	temp_l = Byte&0xff;
	USART_SendData(USART1,temp_h);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
	
	USART_SendData(USART1,temp_l);
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE)==RESET);
}
void  Usart_Send_char(u8 ch)
{

while((USART1->SR&0x40) == 0);

USART1->DR = (u8) ch;
}
void Usart_Send_String(char *string)
{
   while(*string)
   {
       Usart_Send_char(*string);
       string++;
   }
}
void Usart_Send_16bit_Data(uint16_t value)
{
    char strings[8];
    char Enter[3]="\r\n";
    sprintf(strings,"%d",value);
    strcat(strings,Enter);
    Usart_Send_String(strings);
}





