#include "AD9833_Cmd.h"
#include "messageout.h"
#include "AD9833.h"

const uint8_t AD9833_SET[]="AD9833 SET";
const uint8_t AD9833_CLEAN_SET[]="AD9833 CLEAN SET\r\n";
/*
AD9833 SET(1,1000000)
*/
void AD9833_Set_Cmd(uint8_t *buff)
{
	float frq;
	float val;
	uint32_t code;
	uint8_t i;
	uint8_t type;
	uint8_t temp[11]="";
	if(buff[10]=='(')
	{
		switch(buff[11])
		{
			case '1':
				type=0;
				break;
			case '2':
				type=1;
				break;
			case '3':
				type=2;
				break;		
			case '4':
				type=3;
				break;
			default	:
				MessageSend("Type is error!\r\n");
				return;
		}
		if (buff[12]!=',')
		{
			MessageSend("Format error\r\n");
			return ;
		}
		for(i=0;i<12;i++)
		{
			if(buff[i+13]!=')' && i<10 )
				temp[i]=buff[i+13];
			else if (i>10)
			{
				MessageSend("Frequency error\r\n");
				return ;
			}
			else
				break;
		}
		frq=atof((const char *)temp);
		#ifdef MCUDebug
		MessageSend("AD9833 Fre:%f\r\n",frq);
		#endif
		if(frq==0)
		{
			MessageSend("dds set format error\r\n");
			return ;
		}		
		else
		{
//			val=frq/25000000*268435456;
			val=frq/10000000*268435456;
			code=(uint32_t )val;
			AD9833_Cofig(code,type);
			MessageSend("AD9833 Set OK\r\n");
		}
	}
	else
	{
		MessageSend("dds set format error\r\n");
	}
}
void AD9833_Reset_Cmd(void)
{
	AD9833_Reset();
	MessageSend("AD9833 Clean OK\r\n");
}


