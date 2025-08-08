#ifndef	 	__AD7175_IMPD_Cmd_H
#define		__AD7175_IMPD_Cmd_H

#include <stdint.h>

extern const uint8_t AD7175_IMPD_READ[];
extern const uint8_t AD7175_IMPD_SET[];
extern const uint8_t AD7175_IMPD_ID[];

void IMPD_AD7175_Read_Cmd(uint8_t *buff);
void IMPD_AD7175_Set(uint8_t *buff);
void IMPD_AD7175_ID_Cmd(void);

#endif 
