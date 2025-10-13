#include "stm32f10x.h"
#include "delay.h"
#define IIC_SDA  GPIOB
#define IIC_SCL  GPIOB

#define IIC_SDA_PIN GPIO_Pin_3
#define IIC_SCL_PIN GPIO_Pin_2

#define IIC_SDA_H GPIO_SetBits(IIC_SDA,IIC_SDA_PIN)
#define IIC_SDA_L GPIO_ResetBits(IIC_SDA,IIC_SDA_PIN)

#define IIC_SCL_H GPIO_SetBits(IIC_SCL,IIC_SCL_PIN)
#define IIC_SCL_L GPIO_ResetBits(IIC_SCL,IIC_SCL_PIN)

static uint8_t IIC_DELAY_US=1;
void IIC_Init(void)
{
	static uint8_t isInit=0;
	if(isInit==1)return;
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;//SDA
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=IIC_SCL_PIN;//SCL
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(IIC_SCL,&GPIO_InitStructure);
	IIC_STOP();
	IIC_DELAY_US=2;
	isInit=1;
}
void IIC_OUT(void)//SDA?????
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;  //??????
	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
}
void IIC_IN(void)//SDA?????
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin=IIC_SDA_PIN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IPU;  //??????
	GPIO_Init(IIC_SDA,&GPIO_InitStructure);
}
void IIC_DELAY(void)
{
	delay_us(IIC_DELAY_US);
}
void IIC_Set_Delay_Us(uint8_t us)
{
	IIC_DELAY_US=us;
}
void IIC_START()//IIC_SCL????? IIC_SDA??
{
	IIC_OUT();
	IIC_SCL_L;IIC_SDA_H;IIC_DELAY();
	
	IIC_SCL_H;IIC_SDA_L;IIC_DELAY();
	IIC_SCL_L;
}
void IIC_STOP()//IIC_SCL????? IIC_SDA??
{
	IIC_OUT();
	IIC_SCL_L;IIC_SDA_L;IIC_DELAY();
	
	IIC_SCL_H;IIC_SDA_H;IIC_DELAY();
	IIC_SCL_L;
}
void IIC_ACK()//SCL?????? IIC_SDA????
{
	IIC_OUT();
	IIC_SCL_L;IIC_SDA_L;IIC_DELAY();

	IIC_SCL_H;IIC_DELAY();
	IIC_SCL_L;
}
void IIC_NACK()
{
	IIC_OUT();
	IIC_SCL_L;IIC_SDA_H;IIC_DELAY();

	IIC_SCL_H;IIC_DELAY();
	IIC_SCL_L;
}
uint8_t IIC_WAIT_ACK(void)
{
	uint8_t rpt=0;
	IIC_IN();
	IIC_SCL_L;IIC_SDA_H;IIC_DELAY();

	IIC_SCL_H;
	while(GPIO_ReadInputDataBit(IIC_SDA,IIC_SDA_PIN)==Bit_SET)   //??????
	{
		rpt++;
		if(rpt>254){
			IIC_STOP();
			return 1;
		}
	}
	IIC_SCL_L;
	return 0;
}
void IIC_SEND_BYTE(uint8_t byte)
{
	IIC_OUT();
	uint8_t i=0;
	for(i=0;i<8;i++){
		uint8_t bit=(byte>>(7-i)&0x01);
		IIC_SCL_L;
		if(bit){
			IIC_SDA_H;
		}else{
			IIC_SDA_L;
		}
		IIC_DELAY();		
		IIC_SCL_H;IIC_DELAY();
		IIC_SCL_L;
	}
}
uint8_t IIC_RECEIVE_BYTE(uint8_t ack)
{
	uint8_t temp,i;
	IIC_IN();
	for(i=0;i<8;i++)
	{
		IIC_SCL_L;IIC_DELAY();
		
		IIC_SCL_H;IIC_DELAY();
		temp<<=1;
		if(GPIO_ReadInputDataBit(IIC_SDA,IIC_SDA_PIN)==Bit_SET)temp|=0x01;
		IIC_SCL_L;	
	}					 
	if(!ack)
		 IIC_NACK();        //??NACK
	else
		 IIC_ACK();         //??ACK   
	return temp;
}
//R/W#   1/0
uint8_t IIC_READ(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data)
{
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()){
		IIC_STOP();
		return 1;
	}
	IIC_SEND_BYTE(reg);
	if(IIC_WAIT_ACK()){
		IIC_STOP();
		return 1;
	}
	
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|1);
	if(IIC_WAIT_ACK()){
		IIC_STOP();
		return 1;
	}
	while(len){
		if(len==1)*data=IIC_RECEIVE_BYTE(0);
		else *data=IIC_RECEIVE_BYTE(1);
		len--;
		data++;	
	}
	IIC_STOP();
	return 0;
}
uint8_t IIC_WRITE(uint8_t addr,uint8_t reg,uint8_t len,uint8_t* data)
{
	IIC_START();
	IIC_SEND_BYTE((addr<<1)|0);
	if(IIC_WAIT_ACK()){
		IIC_STOP();
		return 1;
	}
	
	IIC_SEND_BYTE(reg);
	if(IIC_WAIT_ACK()){
		IIC_STOP();
		return 1;
	}
	while(len){
		IIC_SEND_BYTE(*data);
		if(IIC_WAIT_ACK()){
			IIC_STOP();	
			return 1;
		}	
		len--;
		data++;
	}
	IIC_STOP();
	return 0;
}


