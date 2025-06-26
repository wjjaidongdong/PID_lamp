#include "com.h"
#include "motor.h"

void CAN_RealValueTransmit(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData[8];
	uint32_t TxMailBox;
	
	TxData[0]=LightRealValue.LightIntensity>>24;
	TxData[1]=LightRealValue.LightIntensity>>16;
	TxData[2]=LightRealValue.LightIntensity>>8;
	TxData[3]=LightRealValue.LightIntensity;
	
	TxData[4]=LightSetValue.PWM>>24;
	TxData[5]=LightSetValue.PWM>>16;
	TxData[6]=LightSetValue.PWM>>8;
	TxData[7]=LightSetValue.PWM;
	
	TxHeader.DLC=8;
	TxHeader.IDE=CAN_ID_STD;
	TxHeader.RTR=CAN_RTR_DATA;
	TxHeader.StdId=0x123;
	TxHeader.TransmitGlobalTime=DISABLE;
	
	HAL_CAN_AddTxMessage(&hcan,&TxHeader,TxData,&TxMailBox);
}


void CanFilter_Config(void)
{
	  CAN_FilterTypeDef  sFilterConfig;
    
    /*ÅäÖÃCAN¹ýÂËÆ÷*/
    sFilterConfig.FilterBank = 0;                     
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDLIST;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_16BIT;
    sFilterConfig.FilterIdHigh = 0x111<<5;              
    sFilterConfig.FilterIdLow = 0x666<<5;
    sFilterConfig.FilterMaskIdHigh = 0x333<<5;          
    sFilterConfig.FilterMaskIdLow = 0x444<<5;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;          
    sFilterConfig.SlaveStartFilterBank = 14;
	
	if(HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)  
  {  
    Error_Handler();  
  } 
}
void CAN_ZERO(uint32_t ID)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint8_t TxData1[8];
	uint32_t TxMailBox;
	
	TxData1[0]=LightSetValue.PWM>>24;
	TxData1[1]=LightSetValue.PWM>>16;
	TxData1[2]=LightSetValue.PWM>>8;
	TxData1[3]=LightSetValue.PWM;
	
	TxData1[4]=LightRealValue.LightIntensity>>24;
	TxData1[5]=LightRealValue.LightIntensity>>16;
	TxData1[6]=LightRealValue.LightIntensity>>8;
	TxData1[7]=LightRealValue.LightIntensity;
	
	TxHeader.DLC=8;
	TxHeader.IDE=CAN_ID_STD;
	TxHeader.RTR=CAN_RTR_DATA;
	TxHeader.StdId=ID;
	TxHeader.TransmitGlobalTime=DISABLE;
	
	HAL_CAN_AddTxMessage(&hcan,&TxHeader,TxData1,&TxMailBox);
}