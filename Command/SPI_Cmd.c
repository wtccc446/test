#include "SPI_Cmd.h"
#include "messageout.h"
#include "Cmd_Support.h"

#include "SPI_Driver.h"
/*
SPI WRITE(1,0x05,0x00,0x00,0x03,0x41,0x42)--SPI1,WriteLen,data...
*/
const uint8_t SPI_WRITE[]="SPI WRITE";
/*
SPI READ(3,0x04,0x02,0xff,0x03)--SPI3,ReadLen,WriteLen,data...
*/
const uint8_t SPI_READ[]="SPI READ";

void SPI_CMD_WriteRead(uint8_t spix, uint8_t ch,uint8_t *txbuff,uint8_t *rxbuff,uint8_t len)
{
	switch(spix)
	{
		case 1:
			SPI1_WriteRead(ch,txbuff,rxbuff,len);
			break;
		case 2:
			SPI2_WriteRead(ch,txbuff,rxbuff,len);
			break;
		case 3:
			SPI3_WriteRead(ch,txbuff,rxbuff,len);
			break;
		default :
			break;
	}
}
/*
SPI WRITE(1,2,0x05,0x00,0x00,0x03,0x41,0x42)--SPI1,WriteLen,data...
*/
void SPI_Write_Cmd(uint8_t *buff)
{
	uint8_t i;
	uint8_t Tx_len=0;
	
	uint8_t Tx_buff[256]="";
	uint8_t Rx_buff[256]="";
	if(buff[9]=='(' && buff[11]==',' && buff[13]==',' && buff[18]==',')
	{
		
		Tx_len=StrToHEX(buff[16],buff[17]);
		for(i=0;i<Tx_len;i++)
		{
			Tx_buff[i]=StrToHEX(buff[21+i*5],buff[22+i*5]);
		}
		Tx_buff[Tx_len]='\0';

	#ifdef MCUDebug
		MessageSend("Tx_len:0x%X\r\n",Tx_len);
			
		MessageSend("dataToWrite:");	
		for(i=0;i<Tx_len;i++)
		{
			MessageSend("0x%X ",Tx_buff[i]);
		}
		MessageSend("\r\n");
	#endif
		if (buff[10]=='1')
		{		
			if(buff[12]<'5' && buff[12]>'0')
				SPI_CMD_WriteRead(1,buff[12]-'0',Tx_buff,Rx_buff,Tx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}
			
		}
		else if (buff[10]=='2')
		{
			if(buff[12]<'5' && buff[12]>'0')
				SPI_CMD_WriteRead(2,buff[12]-'0',Tx_buff,Rx_buff,Tx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}
			
		}
		else if (buff[10]=='3')
		{
			if(buff[12]<'5' && buff[12]>'0')
				SPI_CMD_WriteRead(3,buff[12]-'0',Tx_buff,Rx_buff,Tx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}
		}
		else
		{
			MessageSend("Error:1(SPI1) | 2(SPI2) | 3(SPI3)\r\n");
			return;
		}

		MessageSend("SPI Write OK\r\n");
	}
	else
		MessageSend("I2C write format error\r\n");
}

/*
SPI READ(3,2,0x04,0x02,0xff,0x03)--SPI3,ReadLen,WriteLen,data...
*/
void SPI_Read_Cmd(uint8_t *buff)
{
	uint8_t Rx_len;
	uint8_t	Tx_len;
	
	uint8_t Tx_buff[256]="";
	uint8_t Rx_buff[256]="";
	uint8_t i;
	if(buff[8]=='('  &&  buff[10]==','  &&  buff[12]==',' &&  buff[17]==','  &&  buff[22]==',')
	{
		Rx_len=StrToHEX(buff[15],buff[16]);
		Tx_len=StrToHEX(buff[20],buff[21]);
		for(i=0;i<Tx_len;i++)
		{
			Tx_buff[i]=StrToHEX(buff[25+i*5],buff[26+i*5]);
		}
		
		#ifdef MCUDebug
		MessageSend("Rx_len:0x%X\r\n",Rx_len);
		MessageSend("Tx_len:0x%X\r\n",Tx_len);
		MessageSend("Tx_buff:");
		for(i=0;i<Tx_len;i++)
		{
			MessageSend("0x%X,",Tx_buff[i]);
		}
		MessageSend("\r\n");
		#endif
		if(buff[9]=='1')
		{
			if(buff[11]<'5' && buff[11]>'0')
				SPI_CMD_WriteRead(1,buff[11]-'0',Tx_buff,Rx_buff,Rx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}
				
		}
		else if(buff[9]=='2')
		{
			if(buff[11]<'5' && buff[11]>'0')
				SPI_CMD_WriteRead(2,buff[11]-'0',Tx_buff,Rx_buff,Rx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}

		}
		else if(buff[9]=='3')
		{
			if(buff[11]<'5' && buff[11]>'0')
				SPI_CMD_WriteRead(3,buff[11]-'0',Tx_buff,Rx_buff,Rx_len);
			else
			{
				MessageSend("Channel Must Be 1 or 2 or 3\r\n");
				return ;
			}

		}
		else
		{
			MessageSend("Error:1(SPI1) | 2(SPI2) | 3(SPI3)\r\n");
			return;											
		}
		MessageSend("SPI Read:");	
		for(i=0;i<Rx_len;i++)
		{
			MessageSend("0x%X ",Rx_buff[i]);
		}
		MessageSend("\r\n");
	}
	else
		MessageSend("I2C write format error\r\n");	
}
