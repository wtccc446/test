#ifndef __USART_H
#define __USART_H
#include <stdint.h>	

	  	
#define UART_1

#ifdef STDLib
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"

#ifdef UART_1

extern uint8_t Uart1RxFlag;
extern uint8_t RX1buff[];
extern uint8_t Uart1Len;
void Uart1_Init(uint32_t bound);
void Uart1_SendString(char *nbuff);
void Uart1_SendByte(uint8_t Data);
void Uart1_SendArray(uint8_t *buff,uint16_t len);

#endif



#endif

#endif
