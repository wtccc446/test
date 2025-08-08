#include "AD7175_IMPD_Cmd.h"
#include "messageout.h"
#include "AD7175_IMPD.h"
#include "Cmd_Support.h"

const uint8_t AD7175_IMPD_READ[]="AD7175 IMPD READ";
const uint8_t AD7175_IMPD_SET[]="AD7175 IMPD SET";
const uint8_t AD7175_IMPD_ID[]="AD7175 IMPD ID\r\n";

/*
AD7175 IMPD READ(CH1,DC)
AD7175 IMPD READ(CH2,AC)
*/

void IMPD_AD7175_Read_Cmd(uint8_t *buff)
{
	uint8_t ret;
	uint8_t ch;
	uint8_t type;	//DC 0x01;AC 0x02
	uint16_t i;
	float v_data;
	float	*p_voltag;
	if (buff[16]=='(' && buff[23]==')')
	{
		if(buff[19]=='1')
			ch=0;
		else if (buff[19]=='2')
			ch=1;
		else if (buff[19]=='3')
			ch=2;
		else
		{
			MessageSend("Channel Error\r\n");
			return ;
		}
		
		if (buff[21]=='D' || buff[21]=='d')
			type=0x01;
		else if (buff[21]=='A' || buff[21]=='a')
			type=0x02;
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
		ret=IMPD_AD7175_Single_Read(ch,&v_data,100000);
		if(ret==0)
			MessageSend("AD7175 Read:%fmV\r\n",v_data);
		else
			MessageSend("AD7175 Read Timeout\r\n");
	}
	else if (type==0x02)
	{
		p_voltag=IMPD_AD7175_Continue_Read(ch,200,100000);
		MessageSend("AD7175 AC Read:\r\n");
		for(i=0;i<200;i++)
		{
			MessageSend("%f\r\n",*p_voltag);
			p_voltag++;
		}
		MessageSend("mV\r\n");
	}
}
/*
AD7175 IMPD SET(CH2,0x10)
*/
void IMPD_AD7175_Set(uint8_t *buff)
{
	uint8_t speed=0;
	uint8_t ch=0;
	if(buff[18]=='1')
		ch=0;
	else if(buff[18]=='2')
		ch=1;
	else if(buff[18]=='3')
		ch=2;
	else
	{
		MessageSend("Channel is error\r\n");
		return ;
	}
	if(buff[15]=='(' && buff[24]==')')
	{
		speed=StrToHEX(buff[22],buff[23]);
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
	if(IMPD_AD7175_Speed_Set(ch,speed)==0)
		MessageSend("AD7175 Speed Set OK\r\n");
	else
		MessageSend("AD7175 Speed Set Error\r\n");
}
void IMPD_AD7175_ID_Cmd(void)
{
	uint32_t ad7175_id_value;
	ad7175_id_value =IMPD_AD7175_ReadID();
	MessageSend("AD7175 ID:0x%X\r\n",ad7175_id_value);
}
