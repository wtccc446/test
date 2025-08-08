#ifndef __TMP117_H
#define __TMP117_H

#include "I2C_Driver.h"
#include "messageout.h"

//according to HW design
#define TMP117_ADDR_1  						0x48 		//address pin pull to GND
#define TMP117_ADDR_2  						0x49 		//address pin pull to VDD
#define TMP117_ADDR_3  						0x4A  	//address pin pull to SDA
#define TMP117_ADDR_4  						0x4B 		//address pin pull to SCL

//TMP117 point reg
#define TMP117_VALUE_REG 					0x00
#define TMP117_CFG_REG   					0x01
#define TMP117_HIGH_LIMIT 				0x02
#define TMP117_LOW_LIMIT  				0x03

#define TMP117_EEPROM_Uclock      0x04
#define TMP117_EEPROM1            0x05
#define TMP117_EEPROM2            0x06
#define TMP117_EEPROM3            0x08  //

extern const uint8_t TMP117_GET_TMP[];

void TMP117_INIT(void);
unsigned char TMP117_Write(unsigned char addr_reg, unsigned char data_reg, unsigned char byte1, unsigned char byte2);
unsigned int TMP117_Read(unsigned char addr_reg, unsigned char data_reg);
float TMP117_Get_Temp(void);
void TMP117_Read_Cmd(uint8_t *buff);

#endif
