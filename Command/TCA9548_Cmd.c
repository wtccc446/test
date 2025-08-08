#include "TCA9548_Cmd.h"
#include "TCA9548.h"
#include "messageout.h"
#include "Cmd_Support.h"

const uint8_t TCA9548_SWITCH[]="TCA9548 SWITCH";


/*
TCA9548 SWITCH(0x01)
*/
void TCA9548_Switch_Cmd(uint8_t *buff)
{
	uint8_t data;
	if(buff[14]=='(' && buff[19]==')')
	{
		data=StrToHEX(buff[17],buff[18]);
		if(TCA9548_Switch(data)==0)
		{
			MessageSend("TCA9548 Switch OK\r\n");
		}
		else
		{
			MessageSend("TCA9548 Switch Error\r\n");
		}
	}
}

