#include "I2C_Driver.h"
#include "Eeprom_Cmd.h"
#include "messageout.h"
#include "Cmd_Support.h"
#include <stdlib.h>

/*
eeprom write(I2C2,0xa0,2,0x00,0x00,0x04,ADSD)
eeprom write(I2C2,0xa0,1,0x00,0x04,ADSD)

*/
const uint8_t EEPROM_WRITE[]="EEPROM WRITE";
/*
eeprom read(I2C1,0xa0,2,0x00,0x00,0x04)
eeprom read(I2C1,0xa0,1,0x00,0x04)
*/
const uint8_t EEPROM_READ[]="EEPROM READ";		

/*
eeprom write(I2C2,0xa0,2,0x00,0x00,0x04,ADSD)
eeprom write(I2C2,0xa0,1,0x00,0x04,ADSD)
*/

const uint8_t CALIBRATION_WRITE[]="CALIBRATION WRITE";
/*
calibration write(i2c2,0xa0,2,0x00,0x00,1.213231)
*/
const uint8_t CALIBRATION_READ[]="CALIBRATION READ";
/*
calibration read(i2c2,0xa0,2,0x00,0x00)
*/

void Eeprom_Write_Cmd(uint8_t *buff)
{
	uint8_t i;
	uint8_t i2c_ch;
	uint8_t chipid;
	uint8_t regadd[2]={0x00,0x00};
	uint8_t addlen=0;
	uint8_t datalen=0;
	uint8_t regbuff[100]="";
	if(buff[12]=='(' && buff[17]==',' && buff[22]==',' && buff[24]==',')
	{
		i2c_ch=buff[16];
		if (i2c_ch=='1' || i2c_ch=='2')
		{
			i2c_ch=i2c_ch-'0';
		}
		else
		{
			MessageSend("Must Be I2C1 Or I2C2\r\n");
			return ;
		}
		chipid=StrToHEX(buff[20],buff[21]);
		addlen=buff[23]-'0';
		if (addlen==1)
		{
			regadd[0]=StrToHEX(buff[27],buff[28]);
		}
		else if (addlen==2)
		{
			regadd[0]=StrToHEX(buff[27],buff[28]);
			regadd[1]=StrToHEX(buff[32],buff[33]);
		}
		else
		{
			MessageSend("Must Be 1(1byte) Or 2(2bytes) Address\r\n");
			return ;
		}
		datalen=StrToHEX(buff[27+5*addlen],buff[28+5*addlen]);
		for(i=0;i<datalen;i++)
		{
			regbuff[i]=buff[30+5*addlen+i];
		}
		#ifdef MCUDebug
		MessageSend("chipid:0x%X\r\n",chipid);	
		MessageSend("addlen:0x%X\r\n",addlen);
		MessageSend("Reg Add:");
		for(int c=0;c<addlen;c++)
		{
			MessageSend("0x%X ",regadd[c]);
		}
		MessageSend("\r\n");
		
		MessageSend("strlen:0x%X\r\n",datalen);
		MessageSend("dataToWrite:%s\r\n",regbuff);
		#endif
		if (i2c_ch==1)
		{
			if(I2C1_Write_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)		
				MessageSend("EEPROM Write Fail\r\n");	
			else
				MessageSend("EEPROM Write OK\r\n");
		}
		else
		{
			if(I2C2_Write_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)		
				MessageSend("EEPROM Write Fail\r\n");	
			else
				MessageSend("EEPROM Write OK\r\n");
		}
	}
	else
		MessageSend("eeprom write format error\r\n");
}

/*
eeprom read(I2C1,0xa0,2,0x00,0x00,0x04)
eeprom read(I2C1,0xa0,1,0x00,0x04)
*/
void Eeprom_Read_Cmd(uint8_t *buff)
{
//	uint8_t i;
	uint8_t i2c_ch;
	uint8_t chipid;
	uint8_t addlen=0;
	uint8_t regadd[2]={0x00,0x00};
	uint8_t datalen=0;
	uint8_t regbuff[100]="";
	if(buff[11]=='(' && buff[16]==',' && buff[21]==',' && buff[23]==',')
	{
		i2c_ch=buff[15];
		if (i2c_ch=='1' || i2c_ch=='2')
		{
			i2c_ch=i2c_ch-'0';
		}
		else
		{
			MessageSend("Must Be I2C1 Or I2C2\r\n");
			return ;
		}
		chipid=StrToHEX(buff[19],buff[20]);
		addlen=buff[22]-'0';
		if (addlen==1)
		{
			regadd[0]=StrToHEX(buff[26],buff[27]);
		}
		else if (addlen==2)
		{
			regadd[0]=StrToHEX(buff[26],buff[27]);
			regadd[1]=StrToHEX(buff[31],buff[32]);
		}
		else
		{
			MessageSend("Must Be 1(1byte) Or 2(2bytes) Address\r\n");
			return ;
		}
		datalen=StrToHEX(buff[26+5*addlen],buff[27+5*addlen]);
		if(datalen>100)
		{
			MessageSend("Data Len Must Be Lesser Than 100\r\n");
			return ;
		}
	#ifdef MCUDebug
		MessageSend("Chip ID:0x%X\r\n",chipid);
		MessageSend("Add Len:0x%d\r\n",addlen);
		MessageSend("Reg Add:");
		for(int c=0;c<addlen;c++)
		{
			MessageSend("0x%X ",regadd[c]);
		}
		MessageSend("\r\n");
		MessageSend("Data Len:0x%d\r\n",datalen);
	#endif
		if (i2c_ch==1)
		{
			if(I2C1_Read_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)		
				MessageSend("EEPROM Write Fail\r\n");	
			else
				MessageSend("EEPROM Read:%s\r\n",regbuff);
		}
		else
		{
			if(I2C2_Read_Operation(chipid,addlen,datalen,regadd,regbuff)!=0)		
				MessageSend("EEPROM Write Fail\r\n");	
			else
				MessageSend("EEPROM Read:%s\r\n",regbuff);
		}				
	}	
	else
		MessageSend("eeprom write format error\r\n");
}
/*
calibration write(i2c2,0xa0,2,0x00,0x00,1.213231)
calibration write(i2c2,0xa0,1,0x00,1.213231)
*/
void Calibration_Write_Cmd(uint8_t *buff)
{
//	uint8_t i;
	uint8_t i2c_ch;
	uint8_t chipid;
	uint8_t regadd[2]={0x00,0x00};
	uint8_t addlen=0;
//	uint8_t str_len=0;
	uint8_t *p_buff;
	char float_buff[100]={0};
	uint32_t float_hex_data_32B[4]={0};
	uint8_t float_hex_data[4]={0};
	float float_value;
	uint32_t int_value;
	if(buff[17]=='(' && buff[22]==',' && buff[27]==',' && buff[29]==',')
	{
		i2c_ch=buff[21];
		if (i2c_ch=='1' || i2c_ch=='2')
		{
			i2c_ch=i2c_ch-'0';
		}
		else
		{
			MessageSend("Must Be I2C1 Or I2C2\r\n");
			return ;
		}
		chipid=StrToHEX(buff[25],buff[26]);
		addlen=buff[28]-'0';
		if (addlen==1)
		{
			regadd[0]=StrToHEX(buff[32],buff[33]);
			p_buff=&buff[35];
		}
		else if (addlen==2)
		{
			regadd[0]=StrToHEX(buff[32],buff[33]);
			regadd[1]=StrToHEX(buff[37],buff[38]);
			p_buff=&buff[40];
		}
		else
		{
			MessageSend("Must Be 1(1byte) Or 2(2bytes) Address\r\n");
			return ;
		}
		for (int x = 0; x < 30; ++x)
		{
			if (*p_buff==')')
			{
				float_value=atof(float_buff);
//				str_len=x;
//				if (float_value==NULL)// if float ==0 
//				{
//					MessageSend("Float Format Error\r\n");
//					return ;
//				}
				break;
			}
			if (x==25)
			{
				MessageSend("Float Value Must Be Lesser Than 25\r\n");
				return ;
			}
			float_buff[x]=*p_buff;
			p_buff++;
		}
		int_value =*(long *)&float_value;
		for (int z = 0; z < 4; ++z)
		{
			float_hex_data_32B[3-z]=(int_value>>(z*8)) & 0x000000ff;
			float_hex_data[3-z]=(uint8_t )float_hex_data_32B[3-z];
		}
		#ifdef MCUDebug
		MessageSend("chipid:0x%X\r\n",chipid);	
		MessageSend("addlen:0x%X\r\n",addlen);
		MessageSend("Reg Add:");
		for(int c=0;c<addlen;c++)
		{
			MessageSend("0x%X ",regadd[c]);
		}
		MessageSend("\r\n");

		MessageSend("Str:");
		for(int c=0;c<str_len;c++)
		{
			MessageSend("0x%X ",float_buff[c]);
		}
		MessageSend("\r\n");
		
		MessageSend("Float:%f\r\n",float_value);

		MessageSend("Float Hex:");
		for(int c=0;c<4;c++)
		{
			MessageSend("0x%X ",float_hex_data[c]);
		}
		MessageSend("\r\n");
		#endif
		if (i2c_ch==1)
		{
			if(I2C1_Write_Operation(chipid,addlen,4,regadd,float_hex_data)!=0)		
				MessageSend("Calibration Write Fail\r\n");	
			else
				MessageSend("Calibration Write OK\r\n");
		}
		else
		{
			if(I2C2_Write_Operation(chipid,addlen,4,regadd,float_hex_data)!=0)		
				MessageSend("Calibration Write Fail\r\n");	
			else
				MessageSend("Calibration Write OK\r\n");
		}
	}
	else
		MessageSend("eeprom write format error\r\n");
}
/*
calibration read(i2c2,0xa0,2,0x00,0x00)
calibration read(i2c2,0xa0,1,0x00)
*/
void Calibration_Read_Cmd(uint8_t *buff)
{
//	uint8_t i;
	uint8_t i2c_ch;
	uint8_t chipid;
	uint8_t regadd[2]={0x00,0x00};
	uint8_t addlen=0;

	uint32_t float_hex_data_32B[4]={0};
	uint8_t float_hex_data[4]={0};
	float float_value;
	uint32_t int_value;
	if(buff[16]=='(' && buff[21]==',' && buff[26]==',' && buff[28]==',')
	{
		i2c_ch=buff[20];
		if (i2c_ch=='1' || i2c_ch=='2')
		{
			i2c_ch=i2c_ch-'0';
		}
		else
		{
			MessageSend("Must Be I2C1 Or I2C2\r\n");
			return ;
		}
		chipid=StrToHEX(buff[24],buff[25]);
		addlen=buff[27]-'0';
		if (addlen==1)
		{
			regadd[0]=StrToHEX(buff[31],buff[32]);
		}
		else if (addlen==2)
		{
			regadd[0]=StrToHEX(buff[31],buff[32]);
			regadd[1]=StrToHEX(buff[36],buff[37]);
		}
		else
		{
			MessageSend("Must Be 1(1byte) Or 2(2bytes) Address\r\n");
			return ;
		}
		
		#ifdef MCUDebug
			MessageSend("chipid:0x%X\r\n",chipid);	
			MessageSend("addlen:0x%X\r\n",addlen);
			MessageSend("Reg Add:");
			for(int c=0;c<addlen;c++)
			{
				MessageSend("0x%X ",regadd[c]);
			}
			MessageSend("\r\n");

			// MessageSend("Float:%f\r\n",int_value);
			
			// MessageSend("Float Hex:");
			// for(int c=0;c<4;c++)
			// {
			// 	MessageSend("0x%X ",float_hex_data[c]);
			// }
			// MessageSend("\r\n");
		#endif
		if (i2c_ch==1)
		{
			if(I2C1_Read_Operation(chipid,addlen,4,regadd,float_hex_data)!=0)		
			{
				MessageSend("Calibration Read Fail\r\n");	
				return ;
			}	
		}
		else
		{
			if(I2C2_Read_Operation(chipid,addlen,4,regadd,float_hex_data)!=0)		
			{
				MessageSend("Calibration Read Fail\r\n");	
				return ;
			}	
		}

		
		for (int z = 0; z < 4; ++z)
		{
			float_hex_data_32B[z]=(uint32_t )float_hex_data[z];
		}
		int_value=(float_hex_data_32B[0]<<24)+(float_hex_data_32B[1]<<16)+(float_hex_data_32B[2]<<8)+float_hex_data_32B[3];
		float_value=*(float *)&int_value;
		#ifdef MCUDebug
			MessageSend("Float Hex:");
			for(int c=0;c<4;c++)
			{
				MessageSend("0x%X ",float_hex_data[c]);
			}
			MessageSend("\r\n");
			MessageSend("Float All:0x%X\r\n",int_value);
		#endif
		MessageSend("Float Data Read:%f\r\n",float_value);
	}
	else
		MessageSend("eeprom write format error\r\n");
}
