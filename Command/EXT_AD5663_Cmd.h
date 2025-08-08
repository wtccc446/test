#ifndef	 	__EXT_AD5663_Cmd_H
#define		__EXT_AD5663_Cmd_H
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t EXT_AD5663_SET[];
extern const uint8_t EXT_AD5663_RESET[];

void EXT_AD5663_Set_Cmd(uint8_t *buff);
void EXT_AD5663_Reset_Cmd(void);


#endif 
