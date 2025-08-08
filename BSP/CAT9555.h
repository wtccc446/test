#ifndef __CAT9555_H
#define __CAT9555_H
#include "I2C_Driver.h"

void Cat9555_Init_8IC(void);
uint8_t Cat9555_Config(uint8_t bus,uint8_t ICx,uint8_t *configdata);//ICx is 1 to 8
uint8_t Cat9555_Output(uint8_t bus,uint8_t ch,uint8_t value);//control each IO; ch1 to ch128
uint8_t Cat9555_Input(uint8_t bus,uint8_t ch,uint8_t *value);




#endif
