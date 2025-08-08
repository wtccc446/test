#include "ADC_Cmd.h"
#include "messageout.h"
#include "ADS8320.h"

const uint8_t ADC_READ[]="ADC READ\r\n";
const uint8_t ADC_MULT_READ[]="ADC MULT READ";

void ADC_Read_Cmd(void)
{
	float volt;
	volt=ADS8320ReadVolt();
	MessageSend("ADC Read:%fmV\r\n",volt);
}

/*
ADC MULT READ(1,10)		
type ,number
type: 1,average
			2,middle
number ;3-99
*/
void ADC_Mult_Read_Cmd(uint8_t *buff)
{
	uint8_t tmp[5]="";
	uint8_t num=0;
	uint8_t *pbit;
	float volt_buff[100];
	float volt;
	for(int a=0;a<4;a++)
	{
		if(buff[16+a]==')')
		{
			break;
		}
		else if(buff[16+a]<0x3a && buff[16+a]>0x2f)
		{
			tmp[a]=buff[16+a];
		}
		else if(a>2)
		{
			MessageSend("Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Format Error\r\n");
			return ;
		}
	}
	num =atoi(tmp);
	if(num>100 || num==0)
	{
		MessageSend("Number Is Too Big:%d\r\n",num);
		return ;
	}
	if(buff[14]=='1')
	{
		volt=ADS8320AverageRead(num);
		MessageSend("ADC Average Read:%fmV\r\n",volt);
	}
	else if(buff[14]=='2')
	{
		volt=ADS8320MiddleRead(num);
		MessageSend("ADC Middle Read:%fmV\r\n",volt);
	}
	else if(buff[14]=='3')
	{
		ADS8320WAVERead(num,volt_buff);
		MessageSend("ADC WAVE READ:{\r\n");
		for(uint8_t i=0;i<99;i++)
		{
			MessageSend("%f\r\n",volt_buff[i]);
		}
		MessageSend("}mv\r\n");
	}
	else
	{
		MessageSend("Error ADC Read Type\r\n");
		return ;
	}
}
