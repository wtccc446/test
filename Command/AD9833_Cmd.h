#ifndef	 	__AD9833_Cmd_H
#define		__AD9833_Cmd_H
#include <stdint.h>
#include <stdlib.h>

extern const uint8_t AD9833_SET[];
extern const uint8_t AD9833_CLEAN_SET[];

void AD9833_Set_Cmd(uint8_t *buff);
void AD9833_Reset_Cmd(void);


#endif 
