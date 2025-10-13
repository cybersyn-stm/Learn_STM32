#ifndef __PID_H
#define __PID_H


typedef struct Pid
{
	double Sv;
	double Pv;
	
	float Kp;
	float Ki;
	float Kd;
	
	float Ek;
	float Ek_1;
	float SEk;
	float De1Ek;
	
	float Pout;
	float Iout;
	float Dout;
	
	char OUT0;
	
	char OUT;
	
	char Clms;
	char T;
	
	int pwmcycle;


}PID;

extern PID pid;
extern int time;

void PID_Init(void);
void PID_Calc(float data);









#endif
