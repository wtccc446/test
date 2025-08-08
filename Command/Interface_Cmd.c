#include "Interface_Cmd.h"
#include "string.h"
#include "messageout.h"
#include "I2C_Cmd.h"
#include "GPIO.h"
#include "Eeprom_Cmd.h"
#include "TestControl_Cmd.h"
#include "RelayControl_Cmd.h"

#include "AD9833_Cmd.h"
#include "SPI_Cmd.h"
#include "AD5663_Cmd.h"
#include "EXT_AD5663_Cmd.h"
#include "SHT85_Cmd.h"
#include "hdc2010_Cmd.h"
#include "TCA9548_Cmd.h"

#include "AD7175_DMM_Cmd.h"
#include "AD7175_IMPD_Cmd.h"
#include "CAT9555.h"
#include "TMP117.h"

#include "hdc3021.h"
#include "hdc3021_Cmd.h"

uint8_t VDATE[]=__DATE__;
uint8_t VTIME[]=__TIME__;


const uint8_t   	CMD_VERSION[]="VERSION\r\n";	
//const uint8_t   	CMD_DELAY[]="DELAY\r\n";   				
const uint8_t   	CMD_HELP[]="HELP\r\n";   					
const uint8_t   	EMPTY[]="\r\n";
const uint8_t			END_FLAG[]="(^_^)\r\n";
const uint8_t			CMD_ERROR[]="Command Error!\r\n";
const uint8_t   	SBITXXX[]={"SBIT\r\n"};
const uint8_t     IOSET[]={"IO SET\r\n"};
const uint8_t     IOREAD[]={"IO READ\r\n"};
const uint8_t     RESETALLIO[]={"RESET ALL IO\r\n"};
const uint8_t     MP5048_ON[]={"MP5048_ON\r\n"};
const uint8_t     MP5048_OFF[]={"MP5048_OFF\r\n"};
const uint8_t 		FW_NAME[]="MILL_Star_Light";
const uint8_t 		FW_VERSION[]="V1.0_2024_0327\r\n";
const uint8_t 		TMP117[]="TMP117 GET TMP\r\n";
//const uint8_t 		HDC3021[]="HDC3021 GET\r\n";
/************Internal API*****************/
unsigned char string_compare_len_start(const char *String1,char *String2,char start,unsigned char LEN)
{
	unsigned char i=0;
	const char *p1=String1;
	char *p2=String2;
	p1=p1+start;
	p2=p2+start;
	for(i=0;i<LEN;i++)
	{
		if(*p1!=*p2)
			return 1;		
		p1++;
		p2++;
	}
	return 0;
}
unsigned char string_compare_len(const uint8_t *String1,uint8_t *String2,uint8_t LEN)
{
	unsigned char i=0;
	const uint8_t *p1=String1;
	uint8_t *p2=String2;
	for(i=0;i<LEN;i++)
	{
		if(*p1!=*p2)
			return 1;
		p1++;
		p2++;
	}
	return 0;
}
unsigned char string_compare(const uint8_t *String1, uint8_t *String2)
{
	unsigned char i=0;
	unsigned char String1_Length=0;
	unsigned char String2_Length=0;
	const uint8_t *p1;
	uint8_t *p2;
	p1=String1;
	p2=String2;
	while(*p1!='\r' && *p1!='\0')
	{
		p1++;
		String1_Length++;
		if(String1_Length>254)
		{
			return 1;
		}
	}
	while(*p2 !='\r' && *p2 !='\0')
	{
		p2++;
		String2_Length++;
		if(String2_Length>254)
		{
			return 1;
		}
	}
	if(String1_Length==String2_Length)
	{
		p1=String1;
		p2=String2;
		for(i=0;i<String1_Length;i++)
		{
			if(*p1!=*p2) 
				return 1;
			
			p1++;
			p2++;
		}
		return 0;
	}
	else 
		return 1;
}	
void StrProcess(uint8_t *indata,uint8_t *outdata)
{
	uint8_t i;
	uint8_t *p_in;
	uint8_t *p_out;
	p_in=indata;
	p_out=outdata;
	for(i=0;p_in[i]!=0x00;i++)
	{
		
		if(p_in[i]>='a' && p_in[i]<='z')
			p_out[i]=p_in[i]-'a'+'A';
		else
			p_out[i]=p_in[i];
		if(i==200)
			break;
	}
}

/**************************/
void VersionPrint(void)
{
	MessageSend("%s %s\r\n%s %s\r\n",FW_NAME,FW_VERSION,VDATE,VTIME);
}
void HelpInformation()
{
	MessageSend("%s",CMD_VERSION);
	MessageSend("%s",CMD_HELP);
	
	//MessageSend("%s",SBITXXX);
	//MessageSend("For example:SBITxxx,xxx,xxx,xxx=x,x,x,x\r\n");
	MessageSend("%s",IOSET);
	MessageSend("For example:IO SET(4,bitx=x,bitxx=x,bitxxx=x,bitxxx=0)\r\n");
	//MessageSend("%s",IOREAD);
	//MessageSend("For example:IO READ(3,bitx,bitxx,bitxxx)\r\n");
	MessageSend("%s\r\n",RESETALLIO);
	
	MessageSend("%s\r\n",EEPROM_WRITE);
	MessageSend("%s\r\n",EEPROM_READ);
	//Add 2023 1219
	MessageSend("%s\r\n",AD5663_SET);
	MessageSend("AD5663 SET(1,1000)\r\n");
	MessageSend("%s\r\n",AD5663_RESET);
	
	MessageSend("%s\r\n",TMP117_GET_TMP);
	
//	MessageSend("%s\r\n",HDC3021);
	
}
void CMD_Process(uint8_t *buff,uint8_t len,uint8_t *flag)
{
	uint8_t cmd_buff[255]="";
	uint8_t bit_array[15]={0};		//only can set 15 IO at one cmd
	uint8_t set_array[15]={0};		//only can set 15 IO at one cmd
	uint8_t *p_cmd_buff;
	uint8_t *p_buff;
	unsigned char io_num=0,io_count=0,io_statu=0;//io_total_count=0,
//	unsigned char bit_num=0,bit_count=0;
//	unsigned int length;
//	unsigned int temp1=0,temp2=0,temp=0;
//	unsigned int BitNum16;
//	unsigned int led_temp = 0;

	StrProcess(buff,cmd_buff);
	p_cmd_buff=&cmd_buff[0];
	p_buff=buff;
	/********   Interface  ************/
	if (string_compare(CMD_VERSION,cmd_buff)==0)
	{
		VersionPrint();
	}
	else if (string_compare(EMPTY,cmd_buff)==0)
	{
		
	}
	else if (string_compare(CMD_HELP,cmd_buff)==0)
	{
		HelpInformation();
	}
	
	else if (string_compare_len(IOSET,cmd_buff,6)==0)
	{
				for(int j=0;j<strlen((char*)cmd_buff)-2;j++)
				{
					if(cmd_buff[j] == '=')
					{
						if( cmd_buff[j-1]-'0' >= 0 && cmd_buff[j-1]-'0' <=9 && cmd_buff[j-2]-'0' >= 0 && cmd_buff[j-2]-'0' <= 9)
						{
							bit_array[io_count] = 10*(cmd_buff[j-2]-'0') + cmd_buff[j-1]-'0';
							set_array[io_count] = cmd_buff[j+1]-'0';
						}
						else if( cmd_buff[j-1]-'0' >= 0 && cmd_buff[j-1]-'0' <=9 )
						{
							bit_array[io_count] = cmd_buff[j-1]-'0';
							set_array[io_count] = cmd_buff[j+1]-'0';
						}
						else
						{
							MessageSend("error command!\r\n");
						}
						io_count++; 					//check the '=' to count how many IO need to set
					}
				}
				for(int k=0;k<io_count;k++)
				{
					io_num = bit_array[k];
					io_statu = set_array[k];
					switch(io_num)
					{
						//case 0:							//do not use case 0, there is bug!
							//BIT8(io_statu);
							//MessageSend("bit0 ");
						case 1:
							Cat9555_Output(2, 1, io_statu);
							MessageSend("bit1 ");
							break;
						case 2:
							Cat9555_Output(2, 2, io_statu);
							MessageSend("bit2 ");
							break;
						case 3:
							Cat9555_Output(2, 3, io_statu);
							MessageSend("bit3 ");
							break;
						case 4:
							Cat9555_Output(2, 4, io_statu);
							MessageSend("bit4 ");
							break;
						case 5:
							Cat9555_Output(2, 5, io_statu);
							MessageSend("bit5 ");
							break;
						case 6:
							Cat9555_Output(2, 6, io_statu);
							MessageSend("bit6 ");
							break;
						case 7:
							Cat9555_Output(2, 7, io_statu);
							MessageSend("bit7 ");
							break;
						case 8:
							Cat9555_Output(2, 8, io_statu);
							MessageSend("bit8 ");
							break;
						case 9:
							Cat9555_Output(2, 9, io_statu);
							MessageSend("bit9 ");
							break;
						case 10:
							Cat9555_Output(2, 10, io_statu);
							MessageSend("bit10 ");
							break;
						case 11:
							Cat9555_Output(2, 11, io_statu);
							MessageSend("bit11 ");
							break;
						case 12:
							Cat9555_Output(2, 12, io_statu);
							MessageSend("bit12 ");
							break;
						case 13:
							Cat9555_Output(2, 13, io_statu);
							MessageSend("bit13 ");
							break;
						case 14:
							Cat9555_Output(2, 14, io_statu);
							MessageSend("bit14 ");
							break;
						case 15:
							Cat9555_Output(2, 15, io_statu);
							MessageSend("bit15 ");
							break;
						case 16:
							Cat9555_Output(2, 16, io_statu);
							MessageSend("bit16 ");
							break;
						case 17:
							Cat9555_Output(2, 17, io_statu);
							MessageSend("bit17 ");
							break;
						case 18:
							Cat9555_Output(2, 18, io_statu);
							MessageSend("bit18 ");
							break;
						case 19:
							Cat9555_Output(2, 19, io_statu);
							MessageSend("bit19 ");
							break;
						case 20:
							Cat9555_Output(2, 20, io_statu);
							MessageSend("bit20 ");
							break;
						case 21:
							Cat9555_Output(2, 21, io_statu);
							MessageSend("bit21 ");
							break;
						case 22:
							Cat9555_Output(2, 22, io_statu);
							MessageSend("bit22 ");
							break;
						case 23:
							Cat9555_Output(2, 23, io_statu);
							MessageSend("bit23 ");
							break;
						case 24:
							Cat9555_Output(2, 24, io_statu);
							MessageSend("bit24 ");
							break;
						case 25:
							Cat9555_Output(2, 25, io_statu);
							MessageSend("bit25 ");
							break;
						case 26:
							Cat9555_Output(2, 26, io_statu);
							MessageSend("bit26 ");
							break;
						case 27:
							Cat9555_Output(2, 27, io_statu);
							MessageSend("bit27 ");
							break;
						case 28:
							Cat9555_Output(2, 28, io_statu);
							MessageSend("bit28 ");
							break;
						case 29:
							Cat9555_Output(2, 29, io_statu);
							MessageSend("bit29 ");
							break;
						case 30:
							Cat9555_Output(2, 30, io_statu);
							MessageSend("bit30 ");
							break;
						case 31:
							Cat9555_Output(2, 31, io_statu);
							MessageSend("bit31 ");
							break;
						case 32:
							Cat9555_Output(2, 32, io_statu);
							MessageSend("bit32 ");
							break;
						case 33:
							Cat9555_Output(2, 33, io_statu);
							MessageSend("bit33 ");
							break;
						case 34:
							Cat9555_Output(2, 34, io_statu);
							MessageSend("bit34 ");
							break;
						case 35:
							Cat9555_Output(2, 35, io_statu);
							MessageSend("bit35 ");
							break;
						case 36:
							Cat9555_Output(2, 36, io_statu);
							MessageSend("bit36 ");
							break;
						case 37:
							Cat9555_Output(2, 37, io_statu);
							MessageSend("bit37 ");
							break;
						case 38:
							Cat9555_Output(2, 38, io_statu);
							MessageSend("bit38 ");
							break;
						case 39:
							Cat9555_Output(2, 39, io_statu);
							MessageSend("bit39 ");
							break;
						case 40:
							Cat9555_Output(2, 40, io_statu);
							MessageSend("bit40 ");
							break;
						case 41:
							Cat9555_Output(2, 41, io_statu);
							MessageSend("bit41 ");
							break;
						case 42:
							Cat9555_Output(2, 42, io_statu);
							MessageSend("bit42 ");
							break;
						case 43:
							Cat9555_Output(2, 43, io_statu);
							MessageSend("bit43 ");
							break;
						case 44:
							Cat9555_Output(2, 44, io_statu);
							MessageSend("bit44 ");
							break;
						case 45:
							Cat9555_Output(2, 45, io_statu);
							MessageSend("bit45 ");
							break;
						case 46:
							Cat9555_Output(2, 46, io_statu);
							MessageSend("bit46 ");
							break;
						case 47:
							Cat9555_Output(2, 47, io_statu);
							MessageSend("bit47 ");
							break;
						case 48:
							Cat9555_Output(2, 48, io_statu);
							MessageSend("bit48 ");
							break;
						case 49:
							Cat9555_Output(2, 49, io_statu);
							MessageSend("bit49 ");
							break;
						case 50:
							Cat9555_Output(2, 50, io_statu);
							MessageSend("bit50 ");
							break;
						case 51:
							Cat9555_Output(2, 51, io_statu);
							MessageSend("bit51 ");
							break;
						case 52:
							Cat9555_Output(2, 52, io_statu);
							MessageSend("bit52 ");
							break;
						case 53:
							Cat9555_Output(2, 53, io_statu);
							MessageSend("bit53 ");
							break;
						case 54:
							Cat9555_Output(2, 54, io_statu);
							MessageSend("bit54 ");
							break;
						case 55:
							Cat9555_Output(2, 55, io_statu);
							MessageSend("bit55 ");
							break;
						case 56:
							Cat9555_Output(2, 56, io_statu);
							MessageSend("bit56 ");
							break;
						case 57:
							Cat9555_Output(2, 57, io_statu);
							MessageSend("bit57 ");
							break;
						case 58:
							Cat9555_Output(2, 58, io_statu);
							MessageSend("bit58 ");
							break;
						case 59:
							Cat9555_Output(2, 59, io_statu);
							MessageSend("bit59 ");
							break;
						case 60:
							Cat9555_Output(2, 60, io_statu);
							MessageSend("bit60 ");
							break;
						case 61:
							Cat9555_Output(2, 61, io_statu);
							MessageSend("bit61 ");
							break;
						case 62:
							Cat9555_Output(2, 62, io_statu);
							MessageSend("bit62 ");
							break;
						case 63:
							Cat9555_Output(2, 63, io_statu);
							MessageSend("bit63 ");
							break;
						case 64:
							Cat9555_Output(2, 64, io_statu);
							MessageSend("bit64 ");
							break;
						case 65:
							Cat9555_Output(2, 65, io_statu);
							MessageSend("bit65 ");
							break;
						case 66:
							Cat9555_Output(2, 66, io_statu);
							MessageSend("bit66 ");
							break;
						case 67:
							Cat9555_Output(2, 67, io_statu);
							MessageSend("bit67 ");
							break;
						case 68:
							Cat9555_Output(2, 68, io_statu);
							MessageSend("bit68 ");
							break;
						case 69:
							Cat9555_Output(2, 69, io_statu);
							MessageSend("bit69 ");
							break;
						case 70:
							Cat9555_Output(2, 70, io_statu);
							MessageSend("bit70 ");
							break;
						case 71:
							Cat9555_Output(2, 71, io_statu);
							MessageSend("bit71 ");
							break;
						case 72:
							Cat9555_Output(2, 72, io_statu);
							MessageSend("bit72 ");
							break;
						case 73:
							Cat9555_Output(2, 73, io_statu);
							MessageSend("bit73 ");
							break;
						case 74:
							Cat9555_Output(2, 74, io_statu);
							MessageSend("bit74 ");
							break;
						case 75:
							Cat9555_Output(2, 75, io_statu);
							MessageSend("bit75 ");
							break;
						case 76:
							Cat9555_Output(2, 76, io_statu);
							MessageSend("bit76 ");
							break;
						case 77:
							Cat9555_Output(2, 77, io_statu);
							MessageSend("bit77 ");
							break;
						case 78:
							Cat9555_Output(2, 78, io_statu);
							MessageSend("bit78 ");
							break;
						case 79:
							Cat9555_Output(2, 79, io_statu);
							MessageSend("bit79 ");
							break;
						case 80:
							Cat9555_Output(2, 80, io_statu);
							MessageSend("bit80 ");
							break;

						default: MessageSend("io num error 1!\r\n");
						break;
					}
				}
				MessageSend("OK\r\n");
	}
	else if (string_compare_len(RESETALLIO,cmd_buff,11)==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		Cat9555_Init_8IC();
		MessageSend("IO Reset OK\r\n");
	}
		/********   EEPROM  ************/
	else if (string_compare_len(EEPROM_WRITE,cmd_buff,12)==0)
	{
		Eeprom_Write_Cmd(buff);
	}
	else if (string_compare_len(EEPROM_READ,cmd_buff,11)==0)
	{
		Eeprom_Read_Cmd(buff);
	}
	else if (string_compare_len(CALIBRATION_WRITE,cmd_buff,17)==0)
	{
		Calibration_Write_Cmd(buff);
	}
	else if (string_compare_len(CALIBRATION_READ,cmd_buff,16)==0)
	{
		Calibration_Read_Cmd(buff);
	}
	/********   I2C  Switch************/
	else if (string_compare_len(TCA9548_SWITCH,cmd_buff,14)==0)
	{
		TCA9548_Switch_Cmd(buff);
	}
	/********   I2C  ************/
	else if (string_compare_len(I2C_WRITE,cmd_buff,9)==0)
	{
		I2C_Write_Cmd(buff);
	}
	else if (string_compare_len(I2C_READ,cmd_buff,8)==0)
	{
		I2C_Read_Cmd(buff);
	}
	else if (string_compare_len(I2C_DIRECT_READ,cmd_buff,15)==0)
	{
		I2C_Direct_Read_Cmd(buff);
	}
	else if (string_compare_len(MP5048_ON,cmd_buff,9)==0)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_9);	//MP5048 MOS-SW on
		MessageSend("MP5048 ON\r\n");
	}
	else if (string_compare_len(MP5048_OFF,cmd_buff,10)==0)
	{
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);		//MP5048 MOS-SW off
		MessageSend("MP5048 OFF\r\n");
	}
	
	else if (string_compare_len(AD5663_SET,cmd_buff,10)==0)
	{
		AD5663_Set_Cmd(buff);
	}	
	else if (string_compare_len(AD5663_RESET,cmd_buff,12)==0)
	{
		AD5663_Reset_Cmd();
	}
	
	else if (string_compare_len(TMP117_GET_TMP,cmd_buff,14)==0)
	{
		TMP117_Read_Cmd(buff);
	}	
	//HDC3021_READ[]="HDC3021 READ\r\n";
	else if (string_compare_len(HDC3021_READ,cmd_buff,12)==0)
	{
		HDC3021_Read_Cmd();
	}
	else if (string_compare_len(HDC3021_ID,cmd_buff,10)==0)
	{
		HDC3021_ID_Cmd();
	}
	
	else
	{
		MessageSend("%s",CMD_ERROR);	
//		MessageSend("%s",buff);
	}
	MessageSend("%s",END_FLAG);
	for (int i = 0; i < len; ++i)
	{
		*p_cmd_buff=0x00;
		*p_buff=0x00;
		p_cmd_buff++;
		p_buff++;
	}
	*flag=0;
}

