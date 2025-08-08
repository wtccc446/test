#ifndef	 	__Eeprom_H
#define		__Eeprom_H

#include <stdint.h>

uint8_t Eeprom_Write(uint8_t add,uint8_t add_len,uint8_t *regadd,uint8_t len,uint8_t *data);
uint8_t Eeprom_Read(uint8_t add,uint8_t add_len,uint8_t *regadd,uint8_t len,uint8_t *data);


#endif
