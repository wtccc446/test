#ifndef	 	__AD7175_TEMP_Cmd_H
#define		__AD7175_TEMP_Cmd_H
#include <stdint.h>

extern const uint8_t AD7175_DMM_READ[];
extern const uint8_t AD7175_DMM_SET[];
extern const uint8_t AD7175_DMM_ID[];

void DMM_AD7175_Read_Cmd(uint8_t *buff);
void DMM_AD7175_Set(uint8_t *buff);
void DMM_AD7175_ID_Cmd(void);

#endif 
