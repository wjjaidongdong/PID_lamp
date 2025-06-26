#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f1xx_hal.h"
#include "visual_scope.h"

//结构体类型的实际/测量值
typedef struct
{
	int LightIntensity;
	int PWM;
}LIGHT_VALUE;

typedef struct
{
	float e_k;       //误差
	float e_k_1;     //上一次误差
	float U_i;       //积分项
	float U_p;       //比例项
	float U_d;       //微分项
	float U;         //控制器输出
	float U_last;    //上一时刻输出
	float P;         //比例系数
	float I;         //积分系数
	float D;         //微分系数
	int I_Saturation;  //积分饱和标志位
	int flag_Feedforward;  //前馈使能
	int U_UpLimit;   //输出上限
	int U_DownLimit; //输出下限
	float Feedforward;//前馈
}PID;

#define Mode_Idle 0
#define Mode_PWM 1
#define Mode_Speed 2
#define Mode_Position 3

extern LIGHT_VALUE LightRealValue;
extern LIGHT_VALUE LightSetValue;

extern PID LightLoop;
extern uint8_t Mode_Motor;

extern void LightPIDCalculate(PID *Pid,int SetValue,int RealValue);
extern void PIDParameterInit(void);
extern void LightSetPWM(void);
#endif

