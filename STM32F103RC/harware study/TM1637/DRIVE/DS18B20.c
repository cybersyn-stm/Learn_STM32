#include "DS18B20.h"
#include "stm32f10x.h"
 
 /**************************************************************************************
 * ?  ? : ??DS18B20???I/O?
 * ?  ? : ?
 * ??? : ?
 **************************************************************************************/
static void DS18B20_GPIO_Config(void)
{		
  GPIO_InitTypeDef GPIO_InitStructure;
 
  RCC_APB2PeriphClockCmd(DS18B20_CLK, ENABLE); 
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //????  
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
	
  GPIO_SetBits(DS18B20_PORT, DS18B20_PIN);	 //DS18B20???????????????
}
 
 /**************************************************************************************
 * ?  ? : ???DS18B20-DATA????????
 * ?  ? : ?
 * ??? : ?
 **************************************************************************************/
static void DS18B20_Mode_IPU(void)
{
 	  GPIO_InitTypeDef GPIO_InitStructure;
	
	  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN; 
	  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;	  //????
	  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}
 
 /**************************************************************************************
 * ?  ? : ???DS18B20-DATA????????
 * ?  ? : ?
 * ??? : ?
 **************************************************************************************/
static void DS18B20_Mode_Out_PP(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
															   
  GPIO_InitStructure.GPIO_Pin = DS18B20_PIN;	
 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //????   
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(DS18B20_PORT, &GPIO_InitStructure);
}
 
 /**************************************************************************************
 * ?  ? : ???????????
 * ?  ? : ?
 * ??? : ?
 **************************************************************************************/
static void DS18B20_Rst(void)
{
	DS18B20_Mode_Out_PP();     //????????? 
	
	DS18B20_DATA_OUT(LOW);     //??????480us????????
	Delay_us(750);
	DS18B20_DATA_OUT(HIGH);   //??????????,??????
	Delay_us(15);   //?????????????,??15~60us???????????
}
 
 /**************************************************************************************
 * ?  ? : ??????????????
 * ?  ? : ?
 * ??? : 0:??   1:??  
 **************************************************************************************/
static u8 DS18B20_Presence(void)
{
	u8 pulse_time = 0;
	
	DS18B20_Mode_IPU();    //?????????
	
	/* ?????????,???????60~240us?????? 
	 * ???????????????,?????????????,??15~60us???????????
	 */
	while( DS18B20_DATA_IN() && pulse_time<100 )
	{
		pulse_time++;
		Delay_us(1);
	}	
 
	if( pulse_time >=100 )  //??100us?,??????????
		return 1;             //????
	else                 //??100us?,??????
		pulse_time = 0;    //??????
	
	while( !DS18B20_DATA_IN() && pulse_time<240 )  // ??????,??????????240us
	{
		pulse_time++;
		Delay_us(1);
	}	
	if( pulse_time >=240 ) // ??????,?????????240us
		return 1;        //????
	else
		return 0;
}
 
 /**************************************************************************************
 * ?  ? : ?DS18B20????bit
 * ?  ? : ?
 * ??? : u8 
 **************************************************************************************/
static u8 DS18B20_Read_Bit(void)
{
	u8 dat;
	
	/* ?0??1????????60us */	
	DS18B20_Mode_Out_PP();
	/* ??????:??????? >1us <15us ?????? */
	DS18B20_DATA_OUT(LOW);
	Delay_us(10);
	
	/* ?????,????,???????????? */
	DS18B20_Mode_IPU();
	
	if( DS18B20_DATA_IN() == SET )
		dat = 1;
	else
		dat = 0;
	
	/* ???????????? */
	Delay_us(45);
	
	return dat;
}
 
 /**************************************************************************************
 * ?  ? : ?DS18B20?????,????
 * ?  ? : ?
 * ??? : u8  
 **************************************************************************************/
u8 DS18B20_Read_Byte(void)
{
	u8 i, j, dat = 0;	
	
	for(i=0; i<8; i++) 
	{
		j = DS18B20_Read_Bit();		//?DS18B20????bit
		dat = (dat) | (j<<i);
	}
	
	return dat;																																																																																
}
 
 /**************************************************************************************
 * ?  ? : ??????DS18B20,????
 * ?  ? : u8
 * ??? : ?  
 **************************************************************************************/
void DS18B20_Write_Byte(u8 dat)
{
	u8 i, testb;
	DS18B20_Mode_Out_PP();
	
	for( i=0; i<8; i++ )
	{
		testb = dat&0x01;
		dat = dat>>1;		
		/* ?0??1????????60us */
		if (testb)
		{			
			DS18B20_DATA_OUT(LOW);
			Delay_us(8);   //1us < ???? < 15us
			
			DS18B20_DATA_OUT(HIGH);
			Delay_us(58);    //58us+8us>60us
		}		
		else
		{			
			DS18B20_DATA_OUT(LOW);  
			/* 60us < Tx 0 < 120us */
			Delay_us(70);
			
			DS18B20_DATA_OUT(HIGH);			
			/* 1us < Trec(????) < ???*/
			Delay_us(2);
		}
	}
}
 
 /**************************************************************************************
 * ?  ? : ??DS18B20
 * ?  ? : ?
 * ??? : ?  
 **************************************************************************************/
void DS18B20_Start(void)
{
	DS18B20_Rst();	           //???????????
	DS18B20_Presence();	       //??????????????
	DS18B20_Write_Byte(0XCC);		 // ?? ROM 
	DS18B20_Write_Byte(0X44);		 // ???? 
}
 
 /**************************************************************************************
 * ?  ? : DS18B20?????
 * ?  ? : ?
 * ??? : u8
 **************************************************************************************/
u8 DS18B20_Init(void)
{
	DS18B20_GPIO_Config();   
	DS18B20_Rst();
	
	return DS18B20_Presence();
}
 
 /**************************************************************************************
 * ?  ? : ?DS18B20?????
 * ?  ? : ?  
 * ??? : float 
 **************************************************************************************/
float DS18B20_Get_Temp(void)
{
	u8 tpmsb, tplsb;
	short s_tem;
	float f_tem;
	
	DS18B20_Rst();	   
	DS18B20_Presence();	 
	DS18B20_Write_Byte(0XCC);				/* ?? ROM */
	DS18B20_Write_Byte(0X44);				/* ???? */
	
	DS18B20_Rst();
    DS18B20_Presence();
	DS18B20_Write_Byte(0XCC);				/* ?? ROM */
    DS18B20_Write_Byte(0XBE);				/* ???? */
	
	tplsb = DS18B20_Read_Byte();		 
	tpmsb = DS18B20_Read_Byte(); 
	USART1_SendChar(tpmsb);
	USART1_SendChar(tplsb);

	s_tem = tpmsb<<8;
	s_tem = s_tem | tplsb;
	
	if( s_tem < 0 )		/* ??? */
		f_tem = (~s_tem+1) * 0.0625;	
	else
		f_tem = (s_tem * 0.625);
	  
    //???????????????????????
    //?????????????
	  
	return f_tem/10.0; 	
}


 
 
 


