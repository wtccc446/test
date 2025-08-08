#include "Eeprom.h"
#include "I2C_Driver.h"

uint8_t Eeprom_Write(uint8_t add,uint8_t add_len,uint8_t *regadd,uint8_t len,uint8_t *data)
{
	if(add_len<3)
	{
		if(I2C1_Write_Operation(add,add_len,len,regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	else
	{
		return 1;
	}
}

uint8_t Eeprom_Read(uint8_t add,uint8_t add_len,uint8_t *regadd,uint8_t len,uint8_t *data)
{
	if(add_len<3)
	{
		if(I2C1_Read_Operation(add,add_len,len,regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	else
	{
		return 1;
	}
}





