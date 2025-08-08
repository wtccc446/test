#ifndef	 	__SPI_Driver_H
#define		__SPI_Driver_H
#include <stdint.h>

#define SPI_1
#define SPI_2
#define SPI_3

#ifdef STDLib
#include "stm32f10x_gpio.h"
#include "stm32f10x_spi.h"
#endif

#ifdef SPI_1
void SPI1_Init(void);
void SPI1_CS_Ctr(uint8_t CS_CH,uint8_t type);
uint8_t SPI1_Write(uint8_t ch,uint8_t *TxData,uint8_t len);
uint8_t SPI1_Read(int8_t ch,uint8_t *RxData,uint8_t len);
uint8_t SPI1_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI1_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI1_Data_In_Check(void);
uint8_t SPI1_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len);
uint8_t SPI1_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len);

#endif

#ifdef SPI_2
void SPI2_Init(void);
void SPI2_CS_Ctr(uint8_t CS_CH,uint8_t type);
uint8_t SPI2_Write(uint8_t ch,uint8_t *TxData,uint8_t len);
uint8_t SPI2_Read(int8_t ch,uint8_t *RxData,uint8_t len);
uint8_t SPI2_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI2_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI2_Data_In_Check(void);
uint8_t SPI2_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len);
uint8_t SPI2_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len);

#endif

#ifdef SPI_3
void SPI3_Init(void);
void SPI3_CS_Ctr(uint8_t CS_CH,uint8_t type);
uint8_t SPI3_Write(uint8_t ch,uint8_t *TxData,uint8_t len);
uint8_t SPI3_Read(int8_t ch,uint8_t *RxData,uint8_t len);
uint8_t SPI3_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI3_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len);
uint8_t SPI3_Data_In_Check(void);
uint8_t SPI3_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len);
uint8_t SPI3_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len);

#endif


#endif 
