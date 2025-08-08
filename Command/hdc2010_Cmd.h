#ifndef __HDC2010_CMD_H
#define __HDC2010_CMD_H
#include <stdint.h>

extern const uint8_t HDC2010_READ[];
extern const uint8_t HDC2010_HEATER[];
extern const uint8_t HDC2010_ID[];

void HDC2010_Read_Cmd(void);
void HDC2010_Heater_Cmd(uint8_t *buff);
void HDC2010_ID_Cmd(void);

#endif
