#include "TCA9548.h"
#include "I2C_Driver.h"

uint8_t TCA9548_Add=0xe0;

/** 			Lower API			**/
uint8_t TCA9548_I2C_Write(uint8_t *Data)
{
	I2C2_Write_Operation(TCA9548_Add,1,0,Data,Data);
	
	return 0;
}


uint8_t TCA9548_I2C_Read(uint8_t *data)
{
//	I2C2_Read_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata);
	
	return 0;
}


/** 			User API			**/

uint8_t TCA9548_Switch(uint8_t data)
{
	if(TCA9548_I2C_Write(&data)!=0)
	{
		return 1;
	}
	return 0;
}

