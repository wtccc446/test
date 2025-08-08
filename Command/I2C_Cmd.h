#ifndef	 	__I2C_Cmd_H
#define		__I2C_Cmd_H
#include "Cmd_Support.h"

extern const uint8_t		I2C_WRITE[];
extern const uint8_t		I2C_READ[];
extern const uint8_t		I2C_DIRECT_READ[];

void I2C_Write_Cmd(uint8_t *buff);
void I2C_Read_Cmd(uint8_t *buff);
void I2C_Direct_Read_Cmd(uint8_t *buff);

#endif 
