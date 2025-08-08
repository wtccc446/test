#ifndef __TestControl_Cmd_H
#define __TestControl_Cmd_H 
#include <stdint.h>


extern const uint8_t TEST_CONTROL[];
extern const uint8_t TEST_RESET[];

void Test_Ctr_Cmd(uint8_t *buff);
void Test_Reset_Cmd(uint8_t *buff);

#endif
