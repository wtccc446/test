#include "AD7175_DMM_Cmd.h"
#include "messageout.h"
#include "AD7175_DMM.h"
#include "Cmd_Support.h"

const uint8_t AD7175_DMM_READ[]="AD7175 DMM READ";
const uint8_t AD7175_DMM_SET[]="AD7175 DMM SET";
const uint8_t AD7175_DMM_ID[]="AD7175 DMM ID\r\n";
uint32_t DMM_AD7175_Timeout=100000;
uint16_t DMM_AD7175_AC_Len=3072;
/*
AD7175 DMM READ(CH1,DC)
AD7175 DMM READ(CH2,AC)
AD7175 DMM READ(CH2,AC,1000)
*/

void DMM_AD7175_Read_Cmd(uint8_t *buff)
{
	uint8_t ret;
	uint8_t ch;
	uint8_t type;	//DC 0x01;AC 0x02
	uint16_t i;
	uint8_t temp[10]={0};
	uint16_t num=0;
	float v_data;
	float	*p_voltag;
	if (buff[15]=='(')
	{
		if(buff[18]=='1')
			ch=0;
		else if (buff[18]=='2')
			ch=1;
		else if (buff[18]=='3')
			ch=2;
		else
		{
			MessageSend("Channel Error\r\n");
			return ;
		}
		
		if (buff[20]=='D' || buff[20]=='d')
			type=0x01;
		else if (buff[20]=='A' || buff[20]=='a')
		{
			type=0x02;
			for(i=0;i<12;i++)
			{
				if(buff[i+23]!=')' && i<10 )
					temp[i]=buff[i+23];
				else if (i>10)
				{
					MessageSend("ADC Read Number error\r\n");
					return ;
				}
				else
					break;
			}
			num=atoi((const char *)temp);		
			if(num>3072)
			{
				MessageSend("ADC Read Number Too Big\r\n");
				return ;
			}
		}
		else
		{
			MessageSend("ADC Read Type Error\r\n");
			return ;
		}
	}
	else
	{
		MessageSend("Format Error\r\n");
		return ;
	}
	if (type==0x01)
	{
		ret=DMM_AD7175_Single_Read(ch,&v_data,DMM_AD7175_Timeout);
		if(ret==0)
			MessageSend("AD7175 Read:%fmV\r\n",v_data);
		else
			MessageSend("AD7175 Read Timeout\r\n");
	}
	else if (type==0x02)
	{
		p_voltag=DMM_AD7175_Continue_Read(ch,num,100000);
		MessageSend("AD7175 AC Read:\r\n");
		for(i=0;i<num;i++)
		{
			MessageSend("%f\r\n",*p_voltag);
			p_voltag++;
		}
		MessageSend("mV\r\n");
	}
}
/*
AD7175 DMM SET(CH2,0x10)
*/
void DMM_AD7175_Set(uint8_t *buff)
{
	uint8_t speed=0;
	uint8_t ch=0;
	if(buff[17]=='1')
		ch=0;
	else if(buff[17]=='2')
		ch=1;
	else
	{
		MessageSend("Channel is error\r\n");
		return ;
	}
	if(buff[14]=='(' && buff[23]==')')
	{
		speed=StrToHEX(buff[21],buff[22]);
	}
	else
	{
		MessageSend("Format Error\r\n");
		return ;
	}
	if(speed>0x14)
	{
		MessageSend("Value is bigger than 0x14\r\n");
		return ;
	}
	if(DMM_AD7175_Speed_Set(ch,speed)==0)
		MessageSend("AD7175 Speed Set OK\r\n");
	else
		MessageSend("AD7175 Speed Set Error\r\n");
}
void DMM_AD7175_ID_Cmd(void)
{
	uint32_t ad7175_id_value;
	ad7175_id_value =DMM_AD7175_ReadID();
	MessageSend("AD7175 ID:0x%X\r\n",ad7175_id_value);
}
