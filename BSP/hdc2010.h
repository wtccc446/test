#ifndef __HDC2010_H
#define __HDC2010_H
#include <stdint.h>


#define HDC2010_DeviceID			0xfe
#define HDC2010_Temperature 	0x00
#define HDC2010_Humidity 			0x02
#define HDC2010_Interrupt			0x07
#define HDC2010_Confige				0x0e
#define HDC2010_easure				0x0f

uint8_t HDC2010_Read_ID(uint16_t *hdc2010_id);
uint8_t HDC2010_Heater(uint8_t status);
uint8_t HDC2010_Temp_Humi_Read(float *temp,float *humi);


#endif
