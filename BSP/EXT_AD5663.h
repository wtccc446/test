#ifndef __EXT_AD5663_H
#define __EXT_AD5663_H

#include <stdint.h>

/**** Command ****/
#define	Ext_Write_Register				0x00
#define Ext_Updata_DAC_n					0x08
#define Ext_Write_n_Updata_All		0x10
#define Ext_Write_Updata_n				0x18
#define Ext_Power_Down_DAC				0x20
#define Ext_AD5663_Reset					0x28
#define Ext_LDAC_Register_Setup		0x30

/**** Address ****/

#define	Ext_DAC_A					0x00
#define Ext_DAC_B					0x01
#define Ext_DAC_ALL				0x07

/******* Function ********/
void EXT_AD5663_DAC_Config(uint8_t ch,uint16_t data);
void EXT_AD5663_DAC_Set(uint8_t ch,float voltage);
void EXT_AD5663_DAC_Reset(void);


#endif

