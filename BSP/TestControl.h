#ifndef __TestControl_H
#define __TestControl_H
#include "I2C_Driver.h"
#include <stdint.h>
uint8_t Test_Init_8IC(uint8_t value);
uint8_t Test_Reset(uint8_t value);
uint8_t Test_ExpendIoOP(uint16_t *ch,uint8_t *value, uint8_t len);
uint8_t Test_Cat9555_Output(uint8_t bus,uint8_t ch,uint8_t value);



#endif
