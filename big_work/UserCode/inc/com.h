#ifndef __COM_H__
#define __COM_H__

#include "stm32f1xx_hal.h"
#include "can.h"

extern void CAN_RealValueTransmit(void);
extern void CAN_ZERO(uint32_t ID);
extern void CanFilter_Config(void);

#endif
