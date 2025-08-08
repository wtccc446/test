#ifndef	 	__Eeprom_Cmd_H
#define		__Eeprom_Cmd_H
#include <stdint.h>

extern const uint8_t EEPROM_WRITE[];
extern const uint8_t EEPROM_READ[];		
extern const uint8_t CALIBRATION_WRITE[];
extern const uint8_t CALIBRATION_READ[];
void Eeprom_Write_Cmd(uint8_t *buff);
void Eeprom_Read_Cmd(uint8_t *buff);
void Calibration_Write_Cmd(uint8_t *buff);
void Calibration_Read_Cmd(uint8_t *buff);


#endif 
