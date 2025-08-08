#ifndef __PWM_h
#define __PWM_h 
#include <stdint.h>

void Tim5_Pwm_Init(void);
void PWM_Control(uint8_t ch,uint8_t percentage);


#endif
