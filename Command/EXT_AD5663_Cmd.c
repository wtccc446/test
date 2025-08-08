#include "EXT_AD5663_Cmd.h"
#include "messageout.h"
#include "EXT_AD5663.h"

const uint8_t EXT_AD5663_SET[]="EXT AD5663 SET";
const uint8_t EXT_AD5663_RESET[]="EXT AD5663 RESET\r\n";
/*
EXT AD5663 SET(1,100)						//unit mV
*/
void EXT_AD5663_Set_Cmd(uint8_t *buff)
{
	uint8_t i=0;
	char temp[10];
	uint8_t ch=0;
	float volt;
	if(buff[14]=='('&&buff[16]==',')
	{
		while(buff[17+i]!=')' && i<8)
		{
			temp[i]=buff[17+i];
			i++;
		}
		volt=atof(temp);
		if(volt<0 ||volt>5000)
		{
			MessageSend("EXT_AD5663 Set Value Error\r\n");
			return ;
		}
		if(buff[15]=='1')
			ch=1;
		else if(buff[15]=='2')
			ch=2;
		else if(buff[15]=='3')
			ch=3;
		else
		{
			MessageSend("EXT_AD5663 Set CH Error\r\n");
			return ;
		}
		#ifdef	MCUDebug
			MessageSend("EXT_AD5663 Set:\r\n");
			MessageSend("CH:%d\r\n",ch);
			MessageSend("CODE:0x%f\r\n",volt);    
    #endif
		EXT_AD5663_DAC_Set(ch,volt);
		MessageSend("EXT_AD5663 Set OK\r\n");
	}
	else
		MessageSend("EXT_AD5663 Set Format Error\r\n");
}

void EXT_AD5663_Reset_Cmd()
{
	EXT_AD5663_DAC_Reset();
	MessageSend("EXT_AD5663 Reset OK\r\n");
}


