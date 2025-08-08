#ifndef	 	__One_Wire_H
#define		__One_Wire_H
#include <stdint.h>

#ifdef STDLib
#include "stm32f10x_gpio.h"

#endif

void One_Wire_Init();
void One_Wire_Reset(uint32_t lowtime,uint32_t delaytime);
uint8_t One_Wire_Check(uint32_t timeout,uint32_t delay);
void One_Wire_Write1B(uint8_t bit,uint32_t starttime,uint32_t delay,uint32_t endtime);
uint8_t One_Wire_Read1B(uint32_t starttime,uint32_t delay);
void One_Wire_Write8B(uint8_t byte,uint32_t starttime,uint32_t delay,uint32_t endtime);
uint8_t One_Wire_Read8B(uint32_t starttime,uint32_t delay);


#endif 
