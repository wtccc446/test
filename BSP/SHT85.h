#ifndef __SHT85_H
#define __SHT85_H
#include <stdint.h>

#define SHT85_Address				0x88
#define SHT85_10mps_High		0x2737
#define SHT85_ReadOut_Reg		0xe000
#define SHT85_Stop_Periodic	0x3093

uint8_t SHT85_Read(float *Temp,float *Humi);

#endif
