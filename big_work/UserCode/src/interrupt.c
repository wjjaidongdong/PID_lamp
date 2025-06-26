#include "interrupt.h"
#include "visual_scope.h"
#include "com.h"
#include "bh1750.h"
#include "OLED.h"

uint8_t temp;
uint16_t Led_Cnt=0;
uint16_t Visualscope_Cnt=0;
uint16_t Encoder_Cnt=0;
uint16_t SpeedLoop_Cnt=0;
uint8_t Search_Zero=0;
int Counter=0;


void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)    
{
	if(htim->Instance==TIM4)                     //1ms进一次
	{
		Counter++;
		Led_Cnt++;
		Encoder_Cnt++;
		Visualscope_Cnt++;
		if(Encoder_Cnt>=200)                         //200ms一次PID计算，光照强度控制
		{
			Encoder_Cnt=0;
			
					if(Mode_Motor==Mode_Position){
						
                LightSetValue.PWM = 0;
							LightSetPWM();
							
				}
					
				
				if(Mode_Motor==Mode_Speed)	{
			LightPIDCalculate(&LightLoop,LightSetValue.LightIntensity,LightRealValue.LightIntensity); //转速环PID计算
			LightSetValue.PWM=LightLoop.U;
			LightSetPWM();
				CAN_ZERO(0x112);
				}
				
					if(Mode_Motor==Mode_PWM){
					LightSetPWM();
				CAN_ZERO(0x111);
					}
	
			
		}
		if(Visualscope_Cnt>=20)            //虚拟示波器发送周期
		{
			Visualscope_Cnt=0;
			VS4Channal_Send(LightSetValue.LightIntensity,LightRealValue.LightIntensity,LightSetValue.PWM,0);
		}
		if(Led_Cnt>=500)                   //led跳变周期
		{
			HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
			Led_Cnt=0;
		}	

	}
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	CAN_RxHeaderTypeDef RxHeader;
	uint8_t RxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_FILTER_FIFO0, &RxHeader, RxData);
		if(RxHeader.StdId==0x111)
	{
				Mode_Motor=RxData[0];
		if(Mode_Motor==Mode_PWM)
		{
			LightSetValue.PWM = (RxData[4]<<24) + (RxData[5]<<16) + (RxData[6]<<8) + (RxData[7]);
		}else
		if(Mode_Motor==Mode_Speed)
		{
			LightSetValue.LightIntensity = (RxData[4]<<24) + (RxData[5]<<16) + (RxData[6]<<8) + (RxData[7]);
		}
	}
}
void OLED_mode(void){
if(Mode_Motor==Mode_PWM)
		{
			OLED_ShowString(2, 8, "1");
		}else
		if(Mode_Motor==Mode_Speed)
		{
			OLED_ShowString(2, 8, "2");
		}else
if(Mode_Motor==Mode_Position)
		{
			OLED_ShowString(2, 8, "3");
		}else{
		OLED_ShowString(2, 8, "0");
		}
}