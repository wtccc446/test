#ifndef __DS18B20_H
#define __DS18B20_H

#include <stdint.h>

#define DS18B20_Resolution_9  0x1f
#define DS18B20_Resolution_10 0x3f
#define DS18B20_Resolution_11 0x5f
#define DS18B20_Resolution_12 0x7f

void DS18B20_Init();
void DS18B20_SetResolution(uint8_t data);
void DS18B20_Convert();
uint8_t DS18B20_ReadTemp(float *temp);


#endif
