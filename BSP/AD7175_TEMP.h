#ifndef __AD7175_TEMP_H
#define __AD7175_TEMP_H

#include "ad717x.h"



void AD7175_Temp_Init(void);


uint32_t AD7175_Temp_ReadID(void);
uint8_t AD7175_Single_Read(uint8_t ch,float *vol_data,uint32_t timeout);
float *AD7175_Continue_Read(uint8_t ch,uint8_t len,uint32_t timeout);

#endif
