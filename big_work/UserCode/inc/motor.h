#ifndef __MOTOR_H__
#define __MOTOR_H__

#include "stm32f1xx_hal.h"
#include "visual_scope.h"

//�ṹ�����͵�ʵ��/����ֵ
typedef struct
{
	int LightIntensity;
	int PWM;
}LIGHT_VALUE;

typedef struct
{
	float e_k;       //���
	float e_k_1;     //��һ�����
	float U_i;       //������
	float U_p;       //������
	float U_d;       //΢����
	float U;         //���������
	float U_last;    //��һʱ�����
	float P;         //����ϵ��
	float I;         //����ϵ��
	float D;         //΢��ϵ��
	int I_Saturation;  //���ֱ��ͱ�־λ
	int flag_Feedforward;  //ǰ��ʹ��
	int U_UpLimit;   //�������
	int U_DownLimit; //�������
	float Feedforward;//ǰ��
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

