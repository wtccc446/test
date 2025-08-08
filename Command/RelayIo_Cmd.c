#include "RelayIo_Cmd.h"
#include "I2C_Driver.h"
#include "messageout.h"
#include "Cmd_Support.h"
#include "ADS8320.h"
#include "Delay.h"



const uint8_t RELAY_CONTROL[]="RELAY CONTROL";
const uint8_t RELAY_ADC_CONTROL[]="RELAY ADC CONTROL";
const uint8_t RELAY_SHORT_BUS[]="RELAY SHORT BUS EXCEPT";
const uint8_t RELAY_ADC_SHORT_BUS[]="RELAY ADC SHORT BUS EXCEPT";
const uint8_t RELAY_SHORT_ONE_BUS_EXCEPT[]={"RELAY SHORT ONE BUS EXCEPT"};
const uint8_t RELAY_ADC_SHORT_ONE_BUS_EXCEPT[]={"RELAY ADC SHORT ONE BUS EXCEPT"};
const uint8_t RELAY_ADC_SHORT_ONE_AND_RELAY[]={"RELAY ADC SHORT ONE AND RELAY"};
const uint8_t RELAY_RESET[]={"RELAY RESET\r\n"};

static uint8_t BIT8_OR[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
static uint8_t BIT8_AND[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
/*********************   Aim At Relay Board   ***********************/
static uint8_t I2C_BUS_IO[2][8][2]={		//IC Add,Port
	{
		{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00},
		{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00}
	},
	{
		{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00},
		{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00}
	}
};

/***********************Internal API*********************/
uint8_t ExpendIoOP(uint16_t *ch,uint8_t *value, uint8_t len)	//channel
{
	uint8_t io_bus=0,io_add=0,io_port=0;
	uint8_t bus2flag=0;
	uint8_t pinbuff[4]={0x00,0x00,0x00,0x00};//bus,ic,port,pin
	uint16_t *pCH;
	uint8_t *pVa;
	uint16_t vch;
	pCH=ch;
	pVa=value;
	for(uint16_t i=0;i<len;i++)
	{
		vch=*pCH;
		pinbuff[0]=vch/128;
		io_bus=BIT8_OR[pinbuff[0]];

		vch=vch%128;
		pinbuff[1]=vch/16;
		io_add=(0x20+pinbuff[1])<<1;

		pinbuff[2]=vch%16/8;
		io_port=pinbuff[2]+0x02;

		pinbuff[3]=vch%8;
		
		if (*pVa==0)
		{
			I2C_BUS_IO[pinbuff[0]][pinbuff[1]][pinbuff[2]]=I2C_BUS_IO[pinbuff[0]][pinbuff[1]][pinbuff[2]] & BIT8_AND[pinbuff[3]];
		}
		else
		{
			I2C_BUS_IO[pinbuff[0]][pinbuff[1]][pinbuff[2]]=I2C_BUS_IO[pinbuff[0]][pinbuff[1]][pinbuff[2]] | BIT8_OR[pinbuff[3]];
		}

		pCH++;
		pVa++;

		if (I2C1_Write_Operation(0xe0,0,1,&io_bus,&io_bus)!=0)			
			return 1;
		if (I2C1_Write_Operation(io_add,1,1,&io_port,&I2C_BUS_IO[pinbuff[0]][pinbuff[1]][pinbuff[2]])!=0)
			return 2;

	}
	return 0;
}

uint8_t RelayBoardReset(void)
{
	uint8_t zero[2]={0x00,0x00};
	uint8_t icadd,i,regadd,data;
	uint8_t a,b,c;
	for (int a = 0; a < 2; a++)
	{
		for (int b = 0; b < 8; b++)
		{
			for (int c = 0; c < 2; c++)
			{
				I2C_BUS_IO[a][b][c]=0x00;
			}
		}
	}
	regadd=0x02;
	data=0x01;

	if (I2C1_Write_Operation(0xe0,0,1,&data,&data)!=0)			
		return 1;	
	for(i=0;i<8;i++)
	{
		icadd=(0x20+i)<<1;
		if (I2C1_Write_Operation(icadd,1,2,&regadd,zero)!=0)
			return 2;
	}
	data=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&data,&data)!=0)			
		return 1;	
	for(i=0;i<8;i++)
	{
		icadd=(0x20+i)<<1;
		if (I2C1_Write_Operation(icadd,1,2,&regadd,zero)!=0)
			return 2;
	}
	return 0;
	
}
/**************  Internal API  ****************/

/**************  External API & Command**********************/
/*
RELAY CONTROL(2,CH31=1,CH21=0)
*/
void RelayCtr_Cmd(uint8_t *buff)
{
	uint8_t num,dbuff;
	uint16_t ch;
	uint16_t opbuff_ch[100]={'\0'};	//channel  
	uint8_t opbuff_value[100]={'\0'}; //H&L
	char tmp[5]="";
	uint8_t startbit,endbit,pbit;
	uint8_t ret=0;
	for(int a=0;a<6;a++)
	{
		if(buff[14+a]==',')
		{
			pbit=13+a;
			break;
		}
		else if(buff[14+a]<0x3a && buff[14+a]>0x2f)
		{
			tmp[a]=buff[14+a];
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
	ret=ExpendIoOP(opbuff_ch,opbuff_value,num);
	if (ret!=0)
	{
		if (ret==1)
			MessageSend("I2C SWITCH ERROR\r\n");
		else
			MessageSend("CAT9555 OUTPUT ERROR\r\n");
	}
	MessageSend("Relay Control OK\r\n");
}
/*
RELAY ADC CONTROL(1000,2,CH31=1,CH21=0)
*/
void Relay_ADC_Ctr_Cmd(uint8_t *buff)
{
	uint8_t num,dbuff;
	uint16_t ch;
	uint16_t opbuff_ch[100]={'\0'};	//channel  
	uint8_t opbuff_value[100]={'\0'}; //H&L
	char tmp[5]="";
	uint8_t startbit,endbit,pbit;
	uint8_t ret=0;

	float volt;
	uint32_t adcdelaytime;
	uint8_t delay1s_num;
	char delaybuff[10]="";	
	for(int a=0;a<8;a++)
	{
		if(buff[18+a]==',')
		{
			pbit=19+a;
			break;
		}
		else if(buff[18+a]<0x3a && buff[18+a]>0x2f)
		{
			delaybuff[a]=buff[18+a];
		}
		else if(a>6)
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
	adcdelaytime=atoi(delaybuff);
	if (adcdelaytime>100000)
	{
		MessageSend("Delay Time Too Long\r\n");
		return ;
	}
	for(int a=0;a<6;a++)
	{
		if(buff[pbit+a]==',')
		{
			pbit=pbit+a;
			break;
		}
		else if(buff[pbit+a]<0x3a && buff[pbit+a]>0x2f)
		{
			tmp[a]=buff[pbit+a];
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
	ret=ExpendIoOP(opbuff_ch,opbuff_value,num);
	if (ret!=0)
	{
		if (ret==1)
			MessageSend("I2C SWITCH ERROR\r\n");
		else
			MessageSend("CAT9555 OUTPUT ERROR\r\n");
		return ;
	}
	delay1s_num=adcdelaytime/1000;
	for (int i = 0; i < delay1s_num; ++i)
	{
		delay_ms(1000);
	}
	if(adcdelaytime%1000!=0)
		delay_ms(adcdelaytime%1000);


	volt=ADS8320ReadVolt();
	for (int i = 0; i < num; ++i)
	{
		opbuff_value[i]=0;
	}
	ret=ExpendIoOP(opbuff_ch,opbuff_value,num);
	if (ret!=0)
	{
		if (ret==1)
			MessageSend("I2C SWITCH ERROR\r\n");
		else
			MessageSend("CAT9555 OUTPUT ERROR\r\n");
		return ;
	}
	MessageSend("ADC Read:%fmV\r\n",volt);
}
/*
RELAY SHORT BUS EXCEPT(B,2,CH31,CH21)
*/
void RelayShort_Cmd(uint8_t *buff)
{
	uint8_t short_bus[2]={0x00,0x00};
  uint8_t ic_temp[16][2]={{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff}};
  uint8_t IgnoreBUS=0;
  uint8_t num,dbuff,ic_add,reg_add;
	uint16_t ch;
	uint8_t i2c_ch;
  uint8_t opbuff[100][4]={'\0'}; //chip port pin
  uint8_t sendbuff[2]={0x00,0x00};
  uint8_t tmp[5]="";
  uint8_t startbit,endbit,pbit;
  if (buff[23]=='A' || buff[23]=='a')
    IgnoreBUS=0;
  else if(buff[23]=='B' || buff[23]=='b')
    IgnoreBUS=1;
  else if(buff[23]=='C' || buff[23]=='c')
    IgnoreBUS=2;
  else if(buff[23]=='D' || buff[23]=='d')
    IgnoreBUS=3;
  else
  {
    MessageSend("BUS Error\r\n");
    return ;
  }
	for(int a=0;a<6;a++)
	{
		if(buff[25+a]==',')
		{
			pbit=25+a;
			break;
		}
		else if(buff[25+a]<0x3a && buff[25+a]>0x2f)
		{
			tmp[a]=buff[25+a];
		}
		else if(a>4)
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Num Format Error\r\n");
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
	#ifdef MCUDebug
	MessageSend("RELAY_SHORT_BUS num:%d\r\n",num);
	MessageSend("RELAY_SHORT_BUS pbit:%d\r\n",pbit);
	#endif

  
  for (int i = 0; i < num; i++)
  {
    for (int b = pbit; b < pbit+200; ++b)
    {
      if (buff[b]=='H'||buff[b]=='h')
      {
        startbit=b+1;
        pbit=b+1;
        break;
      }
      if (b>(100+pbit))
      {
        MessageSend("Format Error\r\n");
        return ;
      }
    }
    for (int c = 0; c < 6; ++c)
    {
      if (buff[c+startbit]==',' || buff[c+startbit]==')')
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
        MessageSend("CH Format Error\r\n");
        return ;
      }
      else
      {
        MessageSend("CH Format Error\r\n");
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
		opbuff[i][3]=ch/128;		//i2c bus
		ch=ch%128;

    opbuff[i][0]=ch/16;                   //chip
    opbuff[i][1]=ch%16/8;                 //output regist
    opbuff[i][2]=ch%16%8;                 //bit
    for(int y=0;y<5;y++)
    {
      tmp[y]=0;
    }
  }
  for (int d = 0; d < num; ++d)
  {
    ic_temp[opbuff[d][3] * 8 + opbuff[d][0]][opbuff[d][1]]=ic_temp[opbuff[d][3] * 8 + opbuff[d][0]][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  }

	reg_add=0x02;

	i2c_ch=0x01;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
  for (int g = 0; g < 4; ++g)
  {
    if(g!=IgnoreBUS)
    {
      ic_add=0x40+((2*g)<<1);
      sendbuff[0]=ic_temp[2*g][0];
      sendbuff[1]=ic_temp[2*g][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      sendbuff[0]=ic_temp[2*g+1][0];
      sendbuff[1]=ic_temp[2*g+1][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
    }
		else
		{
			ic_add=0x40+((2*g)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
		}
  }
  i2c_ch=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
  for (int g = 0; g < 4; ++g)
  {
    if(g!=IgnoreBUS)
    {
      ic_add=0x40+((2*g)<<1);
      sendbuff[0]=ic_temp[2*g][0];
      sendbuff[1]=ic_temp[2*g][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      sendbuff[0]=ic_temp[2*g+1][0];
      sendbuff[1]=ic_temp[2*g+1][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
    }
		else
		{
			ic_add=0x40+((2*g)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
		}
  }
  MessageSend("Relay Short One Bus OK\r\n");
}
/*
RELAY ADC SHORT BUS EXCET(1000,B,2,CH31,CH21)
*/
void Relay_ADC_Short_Cmd(uint8_t *buff)
{

	uint8_t short_bus[2]={0x00,0x00};
  uint8_t ic_temp[16][2]={{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff},{0xff,0xff}};
  uint8_t IgnoreBUS=0;
  uint8_t num,dbuff,ic_add,reg_add;
	uint16_t ch;
	uint8_t i2c_ch;
  uint8_t opbuff[100][4]={'\0'}; //chip port pin
  uint8_t sendbuff[2]={0x00,0x00};
  uint8_t tmp[5]="";
  uint8_t startbit,endbit,pbit;

  float volt;
	uint32_t adcdelaytime;
	uint8_t delay1s_num;
	char delaybuff[10]="";	
  for(int a=0;a<8;a++)
	{
		if(buff[27+a]==',')
		{
			pbit=28+a;
			break;
		}
		else if(buff[27+a]<0x3a && buff[27+a]>0x2f)
		{
			delaybuff[a]=buff[27+a];
		}
		else if(a>6)
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
	adcdelaytime=atoi(delaybuff);

  if (buff[pbit]=='A' || buff[pbit]=='a')
    IgnoreBUS=0;
  else if(buff[pbit]=='B' || buff[pbit]=='b')
    IgnoreBUS=1;
  else if(buff[pbit]=='C' || buff[pbit]=='c')
    IgnoreBUS=2;
  else if(buff[pbit]=='D' || buff[pbit]=='d')
    IgnoreBUS=3;
  else
  {
    MessageSend("BUS Error\r\n");
    return ;
  }
  pbit=pbit+2;
	for(int a=0;a<6;a++)
	{
		if(buff[pbit+a]==',')
		{
			pbit=pbit+a;
			break;
		}
		else if(buff[pbit+a]<0x3a && buff[pbit+a]>0x2f)
		{
			tmp[a]=buff[pbit+a];
		}
		else if(a>4)
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Num Format Error\r\n");
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
	#ifdef MCUDebug
	MessageSend("RELAY_SHORT_BUS num:%d\r\n",num);
	MessageSend("RELAY_SHORT_BUS pbit:%d\r\n",pbit);
	#endif

  
  for (int i = 0; i < num; i++)
  {
    for (int b = pbit; b < pbit+200; ++b)
    {
      if (buff[b]=='H'||buff[b]=='h')
      {
        startbit=b+1;
        pbit=b+1;
        break;
      }
      if (b>(100+pbit))
      {
        MessageSend("Format Error\r\n");
        return ;
      }
    }
    for (int c = 0; c < 6; ++c)
    {
      if (buff[c+startbit]==',' || buff[c+startbit]==')')
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
        MessageSend("CH Format Error\r\n");
        return ;
      }
      else
      {
        MessageSend("CH Format Error\r\n");
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
		opbuff[i][3]=ch/128;		//i2c bus
		ch=ch%128;

    opbuff[i][0]=ch/16;                   //chip
    opbuff[i][1]=ch%16/8;                 //output regist
    opbuff[i][2]=ch%16%8;                 //bit
    for(int y=0;y<5;y++)
    {
      tmp[y]=0;
    }
  }
  for (int d = 0; d < num; ++d)
  {
    ic_temp[opbuff[d][3] * 8 + opbuff[d][0]][opbuff[d][1]]=ic_temp[opbuff[d][3] * 8 + opbuff[d][0]][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  }

	reg_add=0x02;

	i2c_ch=0x01;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
  for (int g = 0; g < 4; ++g)
  {
    if(g!=IgnoreBUS)
    {
      ic_add=0x40+((2*g)<<1);
      sendbuff[0]=ic_temp[2*g][0];
      sendbuff[1]=ic_temp[2*g][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      sendbuff[0]=ic_temp[2*g+1][0];
      sendbuff[1]=ic_temp[2*g+1][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
    }
		else
		{
			ic_add=0x40+((2*g)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
		}
  }
  i2c_ch=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
  for (int g = 0; g < 4; ++g)
  {
    if(g!=IgnoreBUS)
    {
      ic_add=0x40+((2*g)<<1);
      sendbuff[0]=ic_temp[2*g][0];
      sendbuff[1]=ic_temp[2*g][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      sendbuff[0]=ic_temp[2*g+1][0];
      sendbuff[1]=ic_temp[2*g+1][1];
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
    }
		else
		{
			ic_add=0x40+((2*g)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
      ic_add=0x40+((2*g+1)<<1);
      if (I2C1_Write_Operation(ic_add,1,2,&reg_add,short_bus)!=0)
      {
        MessageSend("CAT9555 OUTPUT ERROR\r\n");
        return ;
      }
		}
  }
  delay1s_num=adcdelaytime/1000;
	for (int i = 0; i < delay1s_num; ++i)
	{
		delay_ms(1000);
	}
	if(adcdelaytime%1000!=0)
		delay_ms(adcdelaytime%1000);


	volt=ADS8320ReadVolt();
	if(RelayBoardReset()!=0)
	{
		MessageSend("Relay Error\r\n");
		return ;
	}
	MessageSend("ADC Read:%fmV\r\n",volt);
}
/*
RELAY SHORT ONE BUS EXCEPT(A,3,ch12,ch21,ch112)
*/
void RelayShortOneBus_Cmd(uint8_t *buff)
{
	uint8_t short_bus[2]={0x00,0x00};
  uint8_t ic_Htemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint8_t ic_Ltemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint8_t Bus=0;
  uint8_t i2c_ch;
  uint16_t	L_lower,L_upper,H_lower,H_upper;
	uint16_t ch;
  uint8_t num,dbuff,ic_add,reg_add;
  uint8_t opbuff[100][4]={'\0'}; //chip port pin
  uint8_t sendbuff[2]={0x00,0x00};
  uint8_t tmp[5]="";
  uint8_t startbit,endbit,pbit;
  if (buff[27]=='A' || buff[27]=='a')
  {
    Bus=0;
    L_lower=0;
    L_upper=33;
    H_lower=128;
    H_upper=161;
  }
  else if(buff[27]=='B' || buff[27]=='b')
  {
    Bus=1;
    L_lower=32;
    L_upper=65;
    H_lower=160;
    H_upper=193;
  }
  else if(buff[27]=='D' || buff[27]=='d')
  {
    Bus=2;
    L_lower=64;
    L_upper=97;
    H_lower=192;
    H_upper=225;
  }
  else if(buff[27]=='E' || buff[27]=='e')
  {
    Bus=3;
    L_lower=96;
    L_upper=129;
    H_lower=224;
    H_upper=257;
  }
  else
  {
    MessageSend("BUS Error\r\n");
    return ;
  }
	for(int a=0;a<6;a++)
	{
		if(buff[29+a]==','||buff[29+a]==')')
		{
			pbit=29+a;
			break;
		}
		else if(buff[29+a]<0x3a && buff[29+a]>0x2f)
		{
			tmp[a]=buff[29+a];
		}
		else if(a>4)
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Num Format Error\r\n");
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
	#ifdef MCUDebug
	MessageSend("RELAY_SHORT_BUS num:%d\r\n",num);
	MessageSend("RELAY_SHORT_BUS pbit:%d\r\n",pbit);
	#endif
  
  for (int i = 0; i < num; i++)
  {
    for (int b = pbit; b < pbit+200; ++b)
    {
      if (buff[b]=='H'||buff[b]=='h')
      {
        startbit=b+1;
        pbit=b+1;
        break;
      }
      if (b>(100+pbit))
      {
        MessageSend("Format Error\r\n");
        return ;
      }
    }
    for (int c = 0; c < 6; ++c)
    {
      if (buff[c+startbit]==',' || buff[c+startbit]==')')
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
        MessageSend("CH Format Error\r\n");
        return ;
      }
      else
      {
        MessageSend("CH Format Error\r\n");
        return ;
      }
      
    }

    ch=atoi((char *)tmp);
    // ch=ch-16;
    if (ch>1024)
		{
			MessageSend("CH Too Big:%d\r\n",ch);
			return ;
		}
		if ((ch>L_lower && ch<L_upper) || (ch>H_lower && ch<H_upper))
		{
			ch=ch-1;	
			opbuff[i][3]=ch/128;		//i2c bus
			ch=ch%128;

			opbuff[i][0]=ch/16;                   //chip
			opbuff[i][1]=ch%16/8;                 //output regist
			opbuff[i][2]=ch%16%8;                 //bit
			for(int y=0;y<5;y++)
			{
				tmp[y]=0;
			}
		}
		else
		{
			MessageSend("CH Must fit Short Bus\r\n");
			return ;
		}	
  }
  for (int d = 0; d < num; ++d)
  {
  	if (opbuff[d][3]==0)
  	{
    	ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  	else
  	{
  		ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  }

	reg_add=0x02;
	i2c_ch=0x01;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Ltemp[0][0];
  sendbuff[1]=ic_Ltemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Ltemp[1][0];
  sendbuff[1]=ic_Ltemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
	i2c_ch=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Htemp[0][0];
  sendbuff[1]=ic_Htemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Htemp[1][0];
  sendbuff[1]=ic_Htemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  
  MessageSend("Relay Short Bus OK\r\n");
}
/*
RELAY ADC SHORT ONE BUS EXCEPT(1000,A,3,ch12,ch21,ch112)
*/
void Relay_ADC_ShortOneBus_Cmd(uint8_t *buff)
{
	uint8_t short_bus[2]={0x00,0x00};
  uint8_t ic_Htemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint8_t ic_Ltemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint8_t Bus=0;
  uint8_t i2c_ch;
  uint16_t	L_lower,L_upper,H_lower,H_upper;
	uint16_t ch;
  uint8_t num,dbuff,ic_add,reg_add;
  uint8_t opbuff[100][4]={'\0'}; //chip port pin
  uint8_t sendbuff[2]={0x00,0x00};
  uint8_t tmp[5]="";
  uint8_t startbit,endbit,pbit;

  float volt;
	uint32_t adcdelaytime;
	uint8_t delay1s_num;
	char delaybuff[10]="";	
  for(int a=0;a<8;a++)
	{
		if(buff[31+a]==',')
		{
			pbit=32+a;
			break;
		}
		else if(buff[31+a]<0x3a && buff[31+a]>0x2f)
		{
			delaybuff[a]=buff[31+a];
		}
		else if(a>6)
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
	adcdelaytime=atoi(delaybuff);
	#ifdef MCUDebug
		MessageSend("ADC Delay:%d",adcdelaytime);
	#endif

  if (buff[pbit]=='A' || buff[pbit]=='a')
  {
    Bus=0;
    L_lower=0;
    L_upper=33;
    H_lower=128;
    H_upper=161;
  }
  else if(buff[pbit]=='B' || buff[pbit]=='b')
  {
    Bus=1;
    L_lower=32;
    L_upper=65;
    H_lower=160;
    H_upper=193;
  }
  else if(buff[pbit]=='D' || buff[pbit]=='d')
  {
    Bus=2;
    L_lower=64;
    L_upper=97;
    H_lower=192;
    H_upper=225;
  }
  else if(buff[pbit]=='E' || buff[pbit]=='e')
  {
    Bus=3;
    L_lower=96;
    L_upper=129;
    H_lower=224;
    H_upper=257;
  }
  else
  {
    MessageSend("BUS Error\r\n");
    return ;
  }
  pbit=pbit+2;
	for(int a=0;a<6;a++)
	{
		if(buff[pbit+a]==','||buff[pbit+a]==')')
		{
			pbit=pbit+a;
			break;
		}
		else if(buff[pbit+a]<0x3a && buff[pbit+a]>0x2f)
		{
			tmp[a]=buff[pbit+a];
		}
		else if(a>4)
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Num Format Error\r\n");
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
	#ifdef MCUDebug
	MessageSend("RELAY_SHORT_BUS num:%d\r\n",num);
	MessageSend("RELAY_SHORT_BUS pbit:%d\r\n",pbit);
	#endif
  
  for (int i = 0; i < num; i++)
  {
    for (int b = pbit; b < pbit+200; ++b)
    {
      if (buff[b]=='H'||buff[b]=='h')
      {
        startbit=b+1;
        pbit=b+1;
        break;
      }
      if (b>(100+pbit))
      {
        MessageSend("Format Error\r\n");
        return ;
      }
    }
    for (int c = 0; c < 6; ++c)
    {
      if (buff[c+startbit]==',' || buff[c+startbit]==')')
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
        MessageSend("CH Format Error\r\n");
        return ;
      }
      else
      {
        MessageSend("CH Format Error\r\n");
        return ;
      }
      
    }

    ch=atoi((char *)tmp);
    // ch=ch-16;
    if (ch>1024)
		{
			MessageSend("CH Too Big:%d\r\n",ch);
			return ;
		}
		if ((ch>L_lower && ch<L_upper) || (ch>H_lower && ch<H_upper))
		{
			ch=ch-1;	
			opbuff[i][3]=ch/128;		//i2c bus
			ch=ch%128;

			opbuff[i][0]=ch/16;                   //chip
			opbuff[i][1]=ch%16/8;                 //output regist
			opbuff[i][2]=ch%16%8;                 //bit
			for(int y=0;y<5;y++)
			{
				tmp[y]=0;
			}
		}
		else
		{
			MessageSend("CH Must fit Short Bus\r\n");
			return ;
		}	
  }
  for (int d = 0; d < num; ++d)
  {
  	if (opbuff[d][3]==0)
  	{
    	ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  	else
  	{
  		ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  }

	reg_add=0x02;
	i2c_ch=0x01;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Ltemp[0][0];
  sendbuff[1]=ic_Ltemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Ltemp[1][0];
  sendbuff[1]=ic_Ltemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
	i2c_ch=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Htemp[0][0];
  sendbuff[1]=ic_Htemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Htemp[1][0];
  sendbuff[1]=ic_Htemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  
 	delay1s_num=adcdelaytime/1000;
	for (int i = 0; i < delay1s_num; ++i)
	{
		delay_ms(1000);
	}
	if(adcdelaytime%1000!=0)
		delay_ms(adcdelaytime%1000);


	volt=ADS8320ReadVolt();
	if(RelayBoardReset()!=0)
	{
		MessageSend("Relay Error\r\n");
		return ;
	}
	MessageSend("ADC Read:%fmV\r\n",volt);

}

/*
RELAY ADC SHORT ONE AND RELAY(1000,A,3,ch12,ch21,ch112|2,ch34=1,ch36=1)
*/
void Relay_ADC_ShortOneAndRelay_Cmd(uint8_t *buff)
{
	uint8_t short_bus[2]={0x00,0x00};
  uint8_t ic_Htemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint8_t ic_Ltemp[2][2]={{0xff,0xff},{0xff,0xff}};
  uint16_t opbuff_ch[100]={'\0'};	//channel  
	uint8_t opbuff_value[100]={'\0'}; //H&L
  uint8_t Bus=0;
  uint8_t i2c_ch,ret;
  uint16_t	L_lower,L_upper,H_lower,H_upper;
	uint16_t ch;
  uint8_t short_num,relay_num,dbuff,ic_add,reg_add;
  uint8_t opbuff[100][4]={'\0'}; //chip port pin
  uint8_t sendbuff[2]={0x00,0x00};
  char tmp[5]="";
  uint8_t startbit,endbit,pbit;

  float volt;
	uint32_t adcdelaytime;
	uint8_t delay1s_num;
	char delaybuff[10]="";	
/****************** Delay Time ********************/
  for(int a=0;a<8;a++)
	{
		if(buff[30+a]==',')
		{
			pbit=31+a;
			break;
		}
		else if(buff[30+a]<0x3a && buff[30+a]>0x2f)
		{
			delaybuff[a]=buff[30+a];
		}
		else if(a>6)
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
	adcdelaytime=atoi(delaybuff);
	if (adcdelaytime>100000)
	{
		MessageSend("Delay Time Too Long\r\n");
		return ;
	}
	#ifdef MCUDebug
		MessageSend("ADC Delay:%d",adcdelaytime);
	#endif
/****************** Delay Time ********************/

/****************** Short Bus ********************/
  if (buff[pbit]=='A' || buff[pbit]=='a')
  {
    Bus=0;
    L_lower=0;
    L_upper=33;
    H_lower=128;
    H_upper=161;
  }
  else if(buff[pbit]=='B' || buff[pbit]=='b')
  {
    Bus=1;
    L_lower=32;
    L_upper=65;
    H_lower=160;
    H_upper=193;
  }
  else if(buff[pbit]=='D' || buff[pbit]=='d')
  {
    Bus=2;
    L_lower=64;
    L_upper=97;
    H_lower=192;
    H_upper=225;
  }
  else if(buff[pbit]=='E' || buff[pbit]=='e')
  {
    Bus=3;
    L_lower=96;
    L_upper=129;
    H_lower=224;
    H_upper=257;
  }
  else
  {
    MessageSend("BUS Error\r\n");
    return ;
  }
/****************** Short Bus ********************/

/****************** Short Number ********************/
  pbit=pbit+2;
	for(int a=0;a<6;a++)
	{
		if(buff[pbit+a]==','||buff[pbit+a]==')')
		{
			pbit=pbit+a;
			break;
		}
		else if(buff[pbit+a]<0x3a && buff[pbit+a]>0x2f)
		{
			tmp[a]=buff[pbit+a];
		}
		else if(a>4)
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
		else
		{
			MessageSend("Num Format Error\r\n");
			return ;
		}
	}
	short_num =atoi(tmp);
	for(int a=0;a<5;a++)
	{
		tmp[a]=0;
	}
	if(short_num>100)
	{
		MessageSend("Control Num More Than 100\r\n");
		return ;
	}
	#ifdef MCUDebug
	MessageSend("RELAY_SHORT_BUS num:%d\r\n",short_num);
	MessageSend("RELAY_SHORT_BUS pbit:%d\r\n",pbit);
	#endif
/****************** Short Number ********************/
  
/****************** Short Except Bit ********************/
	// pbit=pbit+1;
  for (int i = 0; i < short_num; i++)
  {
    for (int b = pbit; b < pbit+200; ++b)
    {
      if (buff[b]=='H'||buff[b]=='h')
      {
        startbit=b+1;
        pbit=b+1;
        break;
      }
      if (b>(100+pbit))
      {
        MessageSend("Format Error\r\n");
        return ;
      }
    }
    for (int c = 0; c < 6; ++c)
    {
      if (buff[c+startbit]==',' || buff[c+startbit]=='|')
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
        MessageSend("CH Format Error\r\n");
        return ;
      }
      else
      {
        MessageSend("CH Format Error\r\n");
        return ;
      }
      
    }

    ch=atoi(tmp);
    // ch=ch-16;
    if (ch>1024)
		{
			MessageSend("CH Too Big:%d\r\n",ch);
			return ;
		}
		if ((ch>L_lower && ch<L_upper) || (ch>H_lower && ch<H_upper))
		{
			ch=ch-1;	
			opbuff[i][3]=ch/128;		//i2c bus
			ch=ch%128;

			opbuff[i][0]=ch/16;                   //chip
			opbuff[i][1]=ch%16/8;                 //output regist
			opbuff[i][2]=ch%16%8;                 //bit
			for(int y=0;y<5;y++)
			{
				tmp[y]=0;
			}
		}
		else
		{
			MessageSend("CH Must fit Short Bus\r\n");
			return ;
		}	
  }
/****************** Short Except Bit ********************/

/****************** Relay Number ********************/
  pbit=pbit+1;
	for(int a=0;a<6;a++)
	{
		if(buff[pbit+a]==',')
		{
			pbit=pbit+a;
			break;
		}
		else if(buff[pbit+a]<0x3a && buff[pbit+a]>0x2f)
		{
			tmp[a]=buff[pbit+a];
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
	relay_num =atoi(tmp);
	for(int a=0;a<5;a++)
	{
		tmp[a]=0;
	}
	if(relay_num>100)
	{
		MessageSend("Control Num More Than 100\r\n");
		return ;
	}

/****************** Relay Number ********************/

/****************** Relay Bit ********************/
	for (int i = 0; i < relay_num; i++)
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
/****************** Relay Bit ********************/

/****************** Short operation ********************/  
 	for (int d = 0; d < short_num; ++d)
  {
  	if (opbuff[d][3]==0)
  	{
    	ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Ltemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  	else
  	{
  		ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]]=ic_Htemp[opbuff[d][0]-Bus*2][opbuff[d][1]] & BIT8_AND[opbuff[d][2]];
  	}
  }

	reg_add=0x02;
	i2c_ch=0x01;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Ltemp[0][0];
  sendbuff[1]=ic_Ltemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Ltemp[1][0];
  sendbuff[1]=ic_Ltemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
	i2c_ch=0x02;
	if (I2C1_Write_Operation(0xe0,0,1,&i2c_ch,&i2c_ch)!=0)
	{
		MessageSend("I2C SWITCH ERROR\r\n");
		return ;
	}
	ic_add=0x40+((2*Bus)<<1);
  sendbuff[0]=ic_Htemp[0][0];
  sendbuff[1]=ic_Htemp[0][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
  ic_add=0x40+((2*Bus+1)<<1);
  sendbuff[0]=ic_Htemp[1][0];
  sendbuff[1]=ic_Htemp[1][1];
  if (I2C1_Write_Operation(ic_add,1,2,&reg_add,sendbuff)!=0)
  {
    MessageSend("CAT9555 OUTPUT ERROR\r\n");
    return ;
  }
/****************** Short Operation ********************/ 

/****************** Relay Operation ********************/  
	ret=ExpendIoOP(opbuff_ch,opbuff_value,relay_num);
	if (ret!=0)
	{
		if (ret==1)
			MessageSend("I2C SWITCH ERROR\r\n");
		else
			MessageSend("CAT9555 OUTPUT ERROR\r\n");
	}
/****************** Relay Operation ********************/  

/****************** ADC And Delay Operation ********************/    
 	delay1s_num=adcdelaytime/1000;
	for (int i = 0; i < delay1s_num; ++i)
	{
		delay_ms(1000);
	}
	if(adcdelaytime%1000!=0)
		delay_ms(adcdelaytime%1000);

	volt=ADS8320ReadVolt();
/****************** ADC And Delay Operation ********************/    

	if(RelayBoardReset()!=0)
	{
		MessageSend("Relay Error\r\n");
		return ;
	}
	MessageSend("ADC Read:%fmV\r\n",volt);

}

void RelayReset_Cmd(void)
{
	uint8_t ret;
	ret=RelayBoardReset();
	if(ret!=0)
	{
		if(ret==1)
		{
			MessageSend("I2C SWITCH ERROR\r\n");
			return ;
		}
		else
		{
			MessageSend("CAT9555 OUTPUT ERROR\r\n");
			return ;
		}
	}
  MessageSend("Relay Reset OK\r\n");
}
