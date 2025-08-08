#ifndef __TCA9548_Cmd_H
#define __TCA9548_Cmd_H
#include <stdint.h>

extern const uint8_t TCA9548_SWITCH[];

void TCA9548_Switch_Cmd(uint8_t *buff);

#endif
