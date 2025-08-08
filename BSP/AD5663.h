#ifndef __AD5663_H
#define __AD5663_H

#include <stdint.h>

/**** Command ****/
#define	Write_Register				0x00
#define Updata_DAC_n					0x08
#define Write_n_Updata_All		0x10
#define Write_Updata_n				0x18
#define Power_Down_DAC				0x20
#define AD5663_Reset					0x28
#define LDAC_Register_Setup		0x30

/**** Address ****/

#define	DAC_A					0x00
#define DAC_B					0x01
#define DAC_ALL				0x07

/******* Function ********/

void AD5663_SPI_Write(uint8_t *data,uint8_t len);
void AD5663_Ldac_Control(void);
void AD5663_DAC_Config(uint8_t ch,uint16_t data);
void AD5663_DAC_Init(void);
void AD5663_DAC_Set(uint8_t ch,float voltage);
void AD5663_DAC_Reset(void);


#endif

