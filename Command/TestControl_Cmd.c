#include "TestControl_Cmd.h"
#include "TestControl.h"
#include "messageout.h"
#include "Cmd_Support.h"

const uint8_t TEST_CONTROL[]="TEST CONTROL";
const uint8_t TEST_RESET[]="TEST RESET";

/*
TEST CONTROL(2,CH1=1,ch32=0)
*/
void Test_Ctr_Cmd(uint8_t *buff)
{
	uint8_t num;
	uint16_t ch;
	uint16_t opbuff_ch[100]={'\0'};	//channel  
	uint8_t opbuff_value[100]={'\0'}; //H&L
	char tmp[5]="";
	uint8_t startbit,endbit,pbit;
	uint8_t ret=0;
	for(int a=0;a<6;a++)
	{
		if(buff[13+a]==',')
		{
			pbit=12+a;
			break;
		}
		else if(buff[13+a]<0x3a && buff[13+a]>0x2f)
		{
			tmp[a]=buff[13+a];
		}
		else if(a>4)
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
	for(int a=0;a<5;a++)
	{
		tmp[a]=0;
	}
	if(num>100)
	{
		MessageSend("Control Num More Than 100\r\n");
		return ;
	}
	for (int i = 0; i < num; i++)
	{
		for (int b = pbit; b < (pbit+200); ++b)
		{
			if (buff[b]=='H'||buff[b]=='h')
			{
				startbit=b+1;
				pbit=b+1;
				break;
			}
			if (b>(pbit+150))
			{
				MessageSend("Format Error\r\n");
				return ;
			}
		}
		for (int c = 0; c < 6; ++c)
		{
			if (buff[c+startbit]=='=')
			{
				endbit=c+pbit;
				pbit=endbit;
				break;
			}
			else if (buff[c+pbit]<0x3a && buff[c+pbit]>0x2f)
			{				
				tmp[c]=buff[c+pbit];
			}
			else if (c==4)
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

		ch=atoi((char *)tmp)-1;
		// ch=ch-16;
		if (ch>1024)
		{
			MessageSend("CH Too Big:%d\r\n",ch);
			return ;
		}
		opbuff_ch[i]=ch;											//channel
		opbuff_value[i]=buff[endbit+1]-'0';			//H & L
		for(int y=0;y<5;y++)
		{
			tmp[y]=0;
		}
	}
	ret=Test_ExpendIoOP(opbuff_ch,opbuff_value,num);
	if (ret!=0)
		MessageSend("Test OUTPUT ERROR\r\n");
	else
		MessageSend("Test Control OK\r\n");
}
/*
TEST RESET(0xF8)
*/
void Test_Reset_Cmd(uint8_t *buff)
{
	uint8_t ic_place;

	if (buff[10]=='(' && buff[15]==')')
	{
		ic_place=StrToHEX(buff[13],buff[14]);
		if (Test_Reset(ic_place)!=0)
		{
			MessageSend("Test Reset Fail\r\n");
			return ;
		}
		else
			MessageSend("Test Reset OK\r\n");
	}
	else
		MessageSend("Test Reset Format Error\r\n");
}


