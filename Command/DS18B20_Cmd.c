#include "DS18B20_Cmd.h"
#include "Messageout.h"
#include "DS18B20.h"

const uint8_t DS18B20_READ[]="DS18B20 READ\r\n";

void DS18B20_Read_Cmd(void)
{
	float temp;
	uint8_t ret;
	ret=DS18B20_ReadTemp(&temp);
	if (ret==0)
		MessageSend("Temperature Read:%fC\r\n",temp);
	else
		MessageSend("Temperature Read Error\r\n");
}


