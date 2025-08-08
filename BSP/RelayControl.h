#ifndef __RelayControl_H
#define __RelayControl_H 
#include <stdint.h>
#include "I2C_Driver.h"

uint8_t Relay_Init_8IC(uint8_t value);
uint8_t Relay_Reset(uint8_t value);
uint8_t Relay_ExpendIoOP(uint16_t *ch,uint8_t *value, uint8_t len);
uint8_t Relay_Cat9555_Output(uint8_t bus,uint8_t ch,uint8_t value);



#endif
