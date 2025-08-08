#ifndef __RelayControl_Cmd_H
#define __RelayControl_Cmd_H 
#include <stdint.h>


extern const uint8_t RELAY_CONTROL[];
extern const uint8_t RELAY_RESET[];

void Relay_Ctr_Cmd(uint8_t *buff);
void Relay_Reset_Cmd(uint8_t *buff);

#endif
