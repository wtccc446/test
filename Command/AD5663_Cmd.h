#ifndef	 	__AD5663_Cmd_H
#define		__AD5663_Cmd_H
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t AD5663_SET[];
extern const uint8_t AD5663_RESET[];

void AD5663_Set_Cmd(uint8_t *buff);
void AD5663_Reset_Cmd(void);


#endif 
