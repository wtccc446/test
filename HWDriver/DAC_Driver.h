#ifndef	 	__DAC_Driver_H
#define		__DAC_Driver_H
#include <stdint.h>

#define My_DAC1
#define My_DAC2

#ifdef My_DAC1
void DAC1_Init(void);
#endif
#ifdef My_DAC2
void DAC2_Init(void);
#endif
void DAC_Set(uint8_t ch,uint16_t data);



#endif

