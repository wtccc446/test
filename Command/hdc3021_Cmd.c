#include "hdc3021_Cmd.h"
#include "messageout.h"
#include "hdc3021.h"

const uint8_t HDC3021_READ[]="HDC3021 READ\r\n";
//const uint8_t HDC3021_HEATER[]="HDC3021 HEATER";
const uint8_t HDC3021_ID[]="HDC3021 ID\r\n";

void HDC3021_Read_Cmd(void)
{
	float temp,humi;
	uint8_t ret;
	ret=HDC3021_Temp_Humi_Read(&temp,&humi);
	if (ret==1)
	{
		MessageSend("HDC3021 Communication Error\r\n");
	}
	else if (ret==2)
	{
		MessageSend("HDC3021 GET CRC Error\r\n");
	}
	else
	{
		MessageSend("Temp:%f\r\n",temp);
		MessageSend("Humi:%f\r\n",humi);
	}

}
/*
HDC3021 HEATER(ON)
*/
//void HDC3021_Heater_Cmd(uint8_t *buff)
//{
//	if (buff[14]=='(')
//	{
//		if (buff[16]=='N' || buff[16]=='n')
//		{
//			if(HDC3021_Heater(1)!=0)
//				MessageSend("HDC3021 Communication Error\r\n");
//			else
//				MessageSend("HDC3021 Heater Turn ON\r\n");
//		}
//		else if (buff[16]=='F' || buff[16]=='f')	
//		{
//			if(HDC3021_Heater(0)!=0)
//				MessageSend("HDC3021 Communication Error\r\n");
//			else
//				MessageSend("HDC3021 Heater Turn OFF\r\n");
//		}
//		else
//		{
//			MessageSend("HDC3021 HEATER Parameter Error\r\n");
//		}
//	}
//	else
//	{
//		MessageSend("HDC3021 HEATER Parameter Error\r\n");
//	}
//}

void HDC3021_ID_Cmd(void)
{
	uint16_t HDC3021_id;
	if (HDC3021_Read_ID(&HDC3021_id)!=0)
	{
		MessageSend("HDC3021 Communication Error\r\n");
		return ;
	}
	MessageSend("HDC3021 ID:0x%X\r\n",HDC3021_id);
}
