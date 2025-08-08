#ifndef __AD7175_IMPD_H
#define __AD7175_IMPD_H

#include "ad717x.h"



void IMPD_AD7175_Init(void);


uint32_t IMPD_AD7175_ReadID(void);
uint8_t IMPD_AD7175_Speed_Set(uint8_t ch,uint8_t speed_type);
uint8_t IMPD_AD7175_Single_Read(uint8_t ch,float *vol_data,uint32_t timeout);
float *IMPD_AD7175_Continue_Read(uint8_t ch,uint8_t len,uint32_t timeout);

#endif
