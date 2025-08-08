#ifndef __HDC3021_CMD_H
#define __HDC3021_CMD_H
#include <stdint.h>

extern const uint8_t HDC3021_READ[];
//extern const uint8_t HDC3021_HEATER[];
extern const uint8_t HDC3021_ID[];

void HDC3021_Read_Cmd(void);
//void HDC3021_Heater_Cmd(uint8_t *buff);
void HDC3021_ID_Cmd(void);

#endif
