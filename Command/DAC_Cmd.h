#ifndef	 	__DAC_Cmd_H
#define		__DAC_Cmd_H
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t DAC_SET[];		//DAC SET(1002)		unit mV

void DAC_Set_Cmd(uint8_t *buff);


#endif 
