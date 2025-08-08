#include "DAC_Cmd.h"
#include "DAC_Driver.h"
#include "messageout.h"

const uint8_t DAC_SET[]="DAC SET";	//DAC SET(1,1002)		unit mV

void DAC_Set_Cmd(uint8_t *buff)
{
	uint8_t i=0;
	char temp[10];
	uint8_t ch=0;
	float volt;
	uint16_t code;
	if(buff[7]=='('&&buff[9]==',')
	{
		while(buff[10+i]!=')' && i<8)
		{
			temp[i]=buff[10+i];
			i++;
		}
		volt=atof(temp);
		if(volt<0 ||volt>2500)
		{
			MessageSend("DAC Set Value Error\r\n");
			return ;
		}
		if(buff[8]=='1')
			ch=1;
		else if(buff[8]=='2')
			ch=2;
		else
		{
			MessageSend("DAC Set CH Error\r\n");
			return ;
		}
		//DAC_SET
		volt=volt/2500*4096;
		code=(uint16_t )volt;
		#ifdef	MCUDebug
			MessageSend("DAC Set:\r\n");
			MessageSend("CH:%d\r\n",ch);
			MessageSend("CODE:0x%X\r\n",code);    
    #endif
		DAC_Set(ch,code);
		MessageSend("DAC Set OK\r\n");
	}
	else
		MessageSend("DAC Set Format Error\r\n");
}


