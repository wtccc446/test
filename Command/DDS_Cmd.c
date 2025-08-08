#include "DDS_Cmd.h"
#include "messageout.h"

const uint8_t DDS_SET[]="DDS SET";	//DDS SET(1002)		unit Hz

void DDS_Set_Cmd(uint8_t *buff)
{
	uint8_t i=0;
	uint8_t temp[20];
	float volt;
	if(buff[7]=='(')
	{
		while(buff[8+i]!=')' && i<15)
		{
			temp[i]=buff[8+i];
			i++;
		}
		volt=atof(temp);
		//DDS_SET
		MessageSend("DDS Set OK\r\n");
	}
	else
		MessageSend("DDS Set Format Error\r\n");
}


