/*----------------------------------------------------------------------------
 * Name:    Timer.h
 *----------------------------------------------------------------------------*/
    
#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>

/* Private define ------------------------------------------------------------*/


#define Tim_1
#ifdef Tim_1
void Timer1_Init(void);
__weak void Tim1_Callback(void);

extern uint32_t gTimerBase;


#endif

#endif

