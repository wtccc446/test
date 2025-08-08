#include "AD5663_Cmd.h"
#include "SPI_Driver.h"
#include "messageout.h"
#include "AD5663.h"

const uint8_t AD5663_SET[]="AD5663 SET";
const uint8_t AD5663_RESET[]="AD5663 RESET\r\n";
/*
AD5663 SET(1,100)						//unit mV
*/
void AD5663_Set_Cmd(uint8_t *buff)
{
	uint8_t i=0;
	char temp[10];
	uint8_t ch=0;
	float volt;
	if(buff[10]=='('&&buff[12]==',')
	{
		while(buff[13+i]!=')' && i<8)
		{
			temp[i]=buff[13+i];
			i++;
		}
		volt=atof(temp);
		if(volt<0 ||volt>5000)
		{
			MessageSend("AD5663 Set Value Error\r\n");
			MessageSend("%f\r\n",volt);
		}
		if(buff[11]=='1')
		{
			ch=1;
		}
		else if(buff[11]=='2')
		{
			ch=2;
		}
		else if(buff[11]=='3')//DAC1 and DAC2
		{
			ch=3;
		}
		else
		{
			MessageSend("AD5663 Set CH Error\r\n");
			return ;
		}
		#ifdef	MCUDebug
			MessageSend("AD5663 Set:\r\n");
			MessageSend("CH:%d\r\n",ch);
			MessageSend("CODE:0x%f\r\n",volt);    
    #endif
		AD5663_DAC_Set(ch,volt);
		MessageSend("AD5663 Set OK\r\n");
	}
	else
		MessageSend("AD5663 Set Format Error\r\n");
}

void AD5663_Reset_Cmd()
{
	AD5663_DAC_Reset();
	MessageSend("AD5663 Reset OK\r\n");
}


