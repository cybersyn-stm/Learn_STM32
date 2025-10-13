#include "pid.h"

PID pid;

void PID_Init(void)
{
	pid.Sv = 0;
	pid.Kp = 0.10244;//比例
  pid.Ki=	0.003989;       //积分
	pid.Kd=	0.00055;           //微分
	pid.pwmcycle=100;   //pwm周期100us
  pid.T=30;           //PID计算周期30ms
	pid.OUT0=0;
  pid.SEk=0;
  pid.Ek=0;
  pid.Ek_1=0;
  pid.De1Ek=0;
	pid.Dout=0;
  pid.Iout=0;
  pid.Pout=0;
	
	pid.Clms = 0;
}
void PID_Calc(float data)
{
	float out;
	pid.Pv = data;
	pid.Ek = pid.Sv - pid.Pv;
	pid.Pout = pid.Kp * pid.Ek;
	pid.SEk += pid.Ek;
	if(pid.SEk < (-10))
	{
		pid.SEk=(-10);
	}
	pid.De1Ek=pid.Ek-pid.Ek_1;  //最近两次偏差之差
   pid.Iout=pid.Ki*pid.SEk;    //积分输出
	if(pid.Iout < (-20))
	{
		pid.Iout=(-20);
	}
	pid.Dout=pid.Kd*pid.De1Ek;  //微分输出
	out = pid.Pout + pid.Iout + pid.Dout;// + pid.Dout;
	if(out > pid.pwmcycle)
	{
		pid.OUT = pid.pwmcycle;
	}
	else if(out <= 0)
	{
		pid.OUT = pid.OUT0;
	}
	else
	{
		pid.OUT = out+0.5f;
	}
	pid.Ek_1 = pid.Ek;
	pid.Clms = 0;
}
