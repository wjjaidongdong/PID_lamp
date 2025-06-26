#include "motor.h"
#include "tim.h"
#include "math.h"

LIGHT_VALUE LightRealValue={0,0};
LIGHT_VALUE LightSetValue={0,0};

PID LightLoop={0};
uint8_t Mode_Motor=0;

void PIDParameterInit(void)
{
		LightLoop.P=0.04;
	LightLoop.I=0.01; 
//LightLoop.D=0.02;  
	
	LightLoop.U_DownLimit=0;
	LightLoop.U_UpLimit=70;
	LightLoop.I_Saturation=1;
	LightLoop.flag_Feedforward=0;

}


void LightPIDCalculate(PID *Pid,int SetValue,int RealValue)
{			
	//计算误差e_k
	Pid->e_k = SetValue - RealValue;
	
	//前馈计算
	if(Pid->flag_Feedforward==1)
	{
		
	}
	
	//比例项计算
	Pid->U_p = Pid->e_k * Pid->P;
	
	//积分项计算
	if(Pid->I_Saturation>0)
	{
		if(Pid->U_last>=Pid->U_UpLimit)
		{
			if(Pid->e_k<0)
			   Pid->U_i = Pid->U_i + Pid->I * Pid->e_k;
		}
		else if(Pid->U_last<=Pid->U_DownLimit)
		{
			if(Pid->e_k>0)
			   Pid->U_i = Pid->U_i + Pid->I * Pid->e_k;
		}
		else
		{
			Pid->U_i = Pid->U_i + Pid->I * Pid->e_k;
		}
	}
	else
	{
		Pid->U_i = Pid->U_i + Pid->I * Pid->e_k;
	}
	//微分项计算
	Pid->U_d = Pid->D * (Pid->e_k - Pid->e_k_1);

	
	Pid->U=Pid->U_p+Pid->U_i+Pid->U_d+Pid->Feedforward+15;
	
	//输出限幅
	if(Pid->U > Pid->U_UpLimit)
		Pid->U = Pid->U_UpLimit;
	if(Pid->U < Pid->U_DownLimit)
		Pid->U = Pid->U_DownLimit;
	
	
	Pid->U_last = Pid->U;
	Pid->e_k_1 = Pid->e_k;
}


void LightSetPWM(void)
{
	__HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, fabs(LightSetValue.PWM)*500/100);
}

