#ifndef __AD7175_DMM_H
#define __AD7175_DMM_H

#include "ad717x.h"




void DMM_AD7175_Init(void);


uint32_t DMM_AD7175_ReadID(void);
uint8_t DMM_AD7175_Speed_Set(uint8_t ch,uint8_t speed_type);
uint8_t DMM_AD7175_Single_Read(uint8_t ch,float *vol_data,uint32_t timeout);
float *DMM_AD7175_Continue_Read(uint8_t ch,uint16_t len,uint32_t timeout);

#endif
