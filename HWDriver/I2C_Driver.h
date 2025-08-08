#ifndef	 	__I2C_Driver_H
#define		__I2C_Driver_H
#include <stdint.h>

#define SWI2C
#define My_I2C1
#define My_I2C2

#ifdef STDLib
#include "stm32f10x_gpio.h"

#endif

#ifdef My_I2C1
#define SWI2C1_DELAY_TIME	2	//10 50.2k
void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_Stop(void);
void I2C1_Ack(void);
unsigned char I2C1_Wait_Ack(void);
void I2C1_Write_Byte(unsigned char txd);
unsigned char I2C1_Read_Byte(unsigned char ack);
uint8_t I2C1_Write_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata);
uint8_t I2C1_Read_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata);
uint8_t I2C1_Read_Direct(uint8_t chipid,uint8_t data_len,uint8_t *regdata);

#endif

#ifdef My_I2C2
#define SWI2C2_DELAY_TIME	20
void I2C2_Init(void);
void I2C2_Start(void);
void I2C2_Stop(void);
void I2C2_Ack(void);
void I2C2_Write_Byte(unsigned char txd);
unsigned char I2C1_Read_Byte(unsigned char ack);
uint8_t I2C2_Write_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata);
uint8_t I2C2_Read_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata);
uint8_t I2C2_Read_Direct(uint8_t chipid,uint8_t data_len,uint8_t *regdata);

#endif

#endif 
