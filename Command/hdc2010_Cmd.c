#include "hdc2010_Cmd.h"
#include "messageout.h"
#include "hdc2010.h"

const uint8_t HDC2010_READ[]="HDC2010 READ\r\n";
const uint8_t HDC2010_HEATER[]="HDC2010 HEATER";
const uint8_t HDC2010_ID[]="HDC2010 ID\r\n";

void HDC2010_Read_Cmd(void)
{
	float temp,humi;
	uint8_t ret;
	ret=HDC2010_Temp_Humi_Read(&temp,&humi);
	if (ret==1)
	{
		MessageSend("HDC2010 Communication Error\r\n");
	}
	else if (ret==2)
	{
		MessageSend("HDC2010 INT Pin Timeout\r\n");
	}
	else if (ret==3)
	{
		MessageSend("HDC2010 INT Pin Error\r\n");
	}
	else
	{
		MessageSend("Temp:%f\r\n",temp);
		MessageSend("Humi:%f\r\n",humi);
	}

}
/*
HDC2010 HEATER(ON)
*/
void HDC2010_Heater_Cmd(uint8_t *buff)
{
	if (buff[14]=='(')
	{
		if (buff[16]=='N' || buff[16]=='n')
		{
			if(HDC2010_Heater(1)!=0)
				MessageSend("HDC2010 Communication Error\r\n");
			else
				MessageSend("HDC2010 Heater Turn ON\r\n");
		}
		else if (buff[16]=='F' || buff[16]=='f')	
		{
			if(HDC2010_Heater(0)!=0)
				MessageSend("HDC2010 Communication Error\r\n");
			else
				MessageSend("HDC2010 Heater Turn OFF\r\n");
		}
		else
		{
			MessageSend("HDC2010 HEATER Parameter Error\r\n");
		}
	}
	else
	{
		MessageSend("HDC2010 HEATER Parameter Error\r\n");
	}
}
void HDC2010_ID_Cmd(void)
{
	uint16_t hdc2010_id;
	if (HDC2010_Read_ID(&hdc2010_id)!=0)
	{
		MessageSend("HDC2010 Communication Error\r\n");
		return ;
	}
	MessageSend("HDC2010 ID:0x%X\r\n",hdc2010_id);
}
