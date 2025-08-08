#include "TestControl.h"
uint8_t TEST_BIT_AND[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uint8_t	TEST_BIT_OR[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};
static uint8_t Test_IO_BUFF[8][2]={		//IC Add,Port
	{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00},
	{0x00,0x00},{0x00,0x00},{0x00,0x00},{0x00,0x00}
};
/********************** Hard Lever API *********************/
//return 1 mean FAIL ,return 0 mean OK.
uint8_t Test_CAT9555_Write(uint8_t ic_add,uint8_t regadd,uint8_t *data,uint8_t datalen)
{
	uint8_t ret;
	ret=I2C1_Write_Operation(ic_add,1,datalen,&regadd,data);
	return ret;
}
uint8_t Test_CAT9555_Read(uint8_t ic_add,uint8_t regadd,uint8_t *data,uint8_t datalen)
{
	uint8_t ret;
	ret=I2C1_Read_Operation(ic_add,1,datalen,&regadd,data);
	return ret;
}

/********************** User API *********************/

uint8_t Test_Init_8IC(uint8_t value)
{
	uint8_t ic_add=0;
	uint8_t ret=0;
	uint8_t config[2]={0x00,0x00};
	for (int i = 0; i < 8; ++i)
	{
		if ((value & TEST_BIT_OR[i]) == TEST_BIT_OR[i] )
		{
			Test_IO_BUFF[i][0]=0x00;
			Test_IO_BUFF[i][1]=0x00;
			
			ic_add=(0x20+i)<<1;
			if (Test_CAT9555_Write(ic_add,0x02,config,2)!=0)
				ret=1;
			if (Test_CAT9555_Write(ic_add,0x06,config,2)!=0)
				ret=1;

		}
	}
	return ret;
}

uint8_t Test_Reset(uint8_t value)
{
	uint8_t ic_add=0;
	uint8_t config[2]={0x00,0x00};
	for (int i = 0; i < 8; ++i)
	{
		if ((value & TEST_BIT_OR[i]) == TEST_BIT_OR[i] )
		{
			Test_IO_BUFF[i][0]=0x00;
			Test_IO_BUFF[i][1]=0x00;
			ic_add=(0x20+i)<<1;
			if (Test_CAT9555_Write(ic_add,0x02,config,2)!=0)
				return 1;
		}
	}
	return 0;
}

uint8_t Test_ExpendIoOP(uint16_t *ch,uint8_t *value, uint8_t len)	//channel
{
	uint8_t io_add=0,io_port=0;
	uint8_t pinbuff[3]={0x00,0x00,0x00};//ic,port,pin
	uint16_t *pCH;
	uint8_t *pVa;
	uint16_t vch;
	pCH=ch;
	pVa=value;
	for(uint16_t i=0;i<len;i++)
	{
		vch=*pCH;
		vch=vch%128;
		pinbuff[0]=vch/16;
		io_add=(0x20+pinbuff[0])<<1;

		pinbuff[1]=vch%16/8;
		io_port=pinbuff[1]+0x02;

		pinbuff[2]=vch%8;
		
		if (*pVa==0)
		{
			Test_IO_BUFF[pinbuff[0]][pinbuff[1]]=Test_IO_BUFF[pinbuff[0]][pinbuff[1]] & TEST_BIT_AND[pinbuff[2]];
		}
		else
		{
			Test_IO_BUFF[pinbuff[0]][pinbuff[1]]=Test_IO_BUFF[pinbuff[0]][pinbuff[1]] | TEST_BIT_OR[pinbuff[2]];
		}

		pCH++;
		pVa++;
	
		if (Test_CAT9555_Write(io_add,io_port,&Test_IO_BUFF[pinbuff[0]][pinbuff[1]],1)!=0)
			return 1;

	}
	return 0;
}

uint8_t Test_Cat9555_Output(uint8_t bus,uint8_t ch,uint8_t value)//control each IO; ch1 to ch128
{
	uint8_t chx,ic_add,reg_add,io,temp;

	chx=ch-1;
	ic_add=((chx/16)<<1)+0x40;
	reg_add=chx%16/8+2;
	io=chx%16%8;

	if (Test_CAT9555_Read(ic_add,reg_add,&temp,1)!=0)
		return 1;
	if (value==1)
		temp=temp | TEST_BIT_OR[io];
	else
		temp=temp & TEST_BIT_AND[io];
	if (Test_CAT9555_Read(ic_add,reg_add,&temp,1)!=0)	
		return 1;
	else
		return 0;
}

