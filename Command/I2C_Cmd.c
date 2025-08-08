#include "I2C_Cmd.h"
#include "I2C_Driver.h"
#include "messageout.h"
/*
I2C WRITE(1,0x90,0x02,0x00,0x00,0x03,0x41,0x42,0x43)--i2c1,chipid,addlen,regadd...,size,data1...
*/
const uint8_t		I2C_WRITE[]={"I2C WRITE"};
/*
I2C READ(3,0x90,0x02,0x00,0xff,0x03,0x65,0x66,0x67)--i2c3,chipid,addlen,regadd...,size
*/
const uint8_t		I2C_READ[]={"I2C READ"};
/*
I2C DIRECT READ(3,0x90,0x02)--i2c3,chipid,size
*/
const uint8_t		I2C_DIRECT_READ[]={"I2C DIRECT READ"};
/*
I2C WRITE(1,0x90,0x02,0x00,0x00,0x03,0x41,0x42,0x43)--i2c1,chipid,addlen,regadd...,size,data1...
*/
void I2C_Write_Cmd(uint8_t *buff)
{
	uint8_t i;
	uint8_t chipid;
	uint8_t addlen=0;
	uint8_t regadd[4]="";
	uint8_t datalen=0;
	uint8_t regbuff[256]="";
	
	if(buff[11]==',' && buff[16]==',' && buff[21]==',' && buff[26]==',')
	{
		chipid=StrToHEX(buff[14],buff[15]);
		addlen=StrToHEX(buff[19],buff[20]);
		if(addlen>4)
		{
			MessageSend("IC Address more than 4 bytes\r\n");
			return ;
		}
		for(i=0;i<addlen;i++)
		{
			regadd[i]=StrToHEX(buff[24+i*5],buff[25+i*5]);
		}
		datalen=StrToHEX(buff[24+addlen*5],buff[25+addlen*5]);
		for(i=0;i<datalen;i++)
		{
			regbuff[i]=StrToHEX(buff[29+addlen*5+i*5],buff[30+addlen*5+i*5]);
		}
		regbuff[datalen]='\0';
	#ifdef MCUDebug
		MessageSend("chipid:0x%X\r\n",chipid);
		MessageSend("addlen:0x%X\r\n",addlen);
		MessageSend("RegAdd:");
		for(i=0;i<addlen;i++)
		{
			MessageSend("0x%X,",regadd[i]);
		}
		MessageSend("\r\n");	
		MessageSend("strlen:0x%X\r\n",datalen);		
		MessageSend("dataToWrite:");	
		for(i=0;i<datalen;i++)
		{
			MessageSend("0x%X ",regbuff[i]);
		}
		MessageSend("\r\n");
	#endif
		if (buff[10]=='1')
		{		
			if(I2C1_Write_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)	
				MessageSend("I2C Write Fail\r\n");	
			else
				MessageSend("I2C Write OK\r\n");
		}
		else if (buff[10]=='2')
		{
			if(I2C2_Write_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)		
				MessageSend("I2C Write Fail\r\n");	
			else
				MessageSend("I2C Write OK\r\n");	
		}
		else
		{
			MessageSend("Error;1(I2C1) or 2(I2C2)\r\n");
		}

		
	}
	else
		MessageSend("I2C write format error\r\n");
}

/*
I2C READ(3,0x90,0x02,0x00,0xff,0x03,0x65,0x66,0x67)--i2c3,chipid,addlen,regadd...,size
*/
void I2C_Read_Cmd(uint8_t *buff)
{
	uint8_t chipid;
	uint8_t	addlen;
	uint8_t regadd[4]="";
	uint8_t datalen;
	uint8_t regbuff[256]="";
	uint8_t i;
	if(buff[8]=='('  &&  buff[10]==','  &&  buff[15]==','  &&  buff[20]==',')
	{
		chipid=StrToHEX(buff[13],buff[14]);
		addlen=StrToHEX(buff[18],buff[19]);
		if(addlen>4)
		{
			MessageSend("IC Address more than 4 bytes\r\n");
			return ;
		}
		for(i=0;i<addlen;i++)
		{
			regadd[i]=StrToHEX(buff[23+i*5],buff[24+i*5]);
		}
		datalen=StrToHEX(buff[23+addlen*5],buff[24+addlen*5]);
		
		#ifdef MCUDebug
		MessageSend("chipid:0x%X\r\n",chipid);
		MessageSend("addlen:0x%X\r\n",addlen);
		MessageSend("RegAdd:");
		for(i=0;i<addlen;i++)
		{
			MessageSend("0x%X,",regadd[i]);
		}
		MessageSend("\r\n");
		MessageSend("strlen:0x%X\r\n",datalen);
		#endif
		if(buff[9]=='1')
		{
			if(I2C1_Read_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)
			{
				MessageSend("I2C Read Fail\r\n");
			}
			else
			{
				MessageSend("I2C Read:");	
				for(i=0;i<datalen;i++)
				{
					MessageSend("0x%X ",regbuff[i]);
				}
				MessageSend("\r\n");
			}
		}
		else if(buff[9]=='2')
		{
			if(I2C2_Read_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)
			{
				MessageSend("I2C Read Fail\r\n");
			}
			else
			{
				MessageSend("I2C Read:");	
				for(i=0;i<datalen;i++)
				{
					MessageSend("0x%X ",regbuff[i]);
				}
				MessageSend("\r\n");
			}
		}
		else
			MessageSend("Error;1(I2C1) or 3(I2C3)\r\n");													
	}
	else
		MessageSend("I2C write format error\r\n");	
}
/*
I2C DIRECT READ(3,0x90,0x02)--i2c3,chipid,size
*/
void I2C_Direct_Read_Cmd(uint8_t *buff)
{
	uint8_t chipid;
	uint8_t datalen;
	uint8_t regbuff[256]="";
	uint8_t i;
	if(buff[15]=='('  &&  buff[17]==','  &&  buff[22]==',')
	{
		chipid=StrToHEX(buff[20],buff[21]);
		datalen=StrToHEX(buff[25],buff[26]);
		
		
		#ifdef MCUDebug
		MessageSend("chipid:0x%X\r\n",chipid);
		MessageSend("strlen:0x%X\r\n",datalen);
		#endif
		if(buff[16]=='1')
		{
			if(I2C1_Read_Direct(chipid,datalen,regbuff)!=0)
			{
				MessageSend("I2C Read Fail\r\n");
			}
			else
			{
				MessageSend("I2C Read:");	
				for(i=0;i<datalen;i++)
				{
					MessageSend("0x%X ",regbuff[i]);
				}
				MessageSend("\r\n");
			}
		}
		else if(buff[16]=='2')
		{
			if(I2C2_Read_Direct(chipid,datalen,regbuff)!=0)
			{
				MessageSend("I2C Read Fail\r\n");
			}
			else
			{
				MessageSend("I2C Read:");	
				for(i=0;i<datalen;i++)
				{
					MessageSend("0x%X ",regbuff[i]);
				}
				MessageSend("\r\n");
			}
		}
		else
			MessageSend("Error;1(I2C1) or 3(I2C3)\r\n");													
	}
	else
		MessageSend("I2C write format error\r\n");	
}

