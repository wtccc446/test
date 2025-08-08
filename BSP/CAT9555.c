#include "CAT9555.h"

uint8_t BIT_AND[8]={0xfe,0xfd,0xfb,0xf7,0xef,0xdf,0xbf,0x7f};
uint8_t	BIT_OR[8]={0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80};

/********************** Hard Lever API *********************/
//return 1 mean FAIL ,return 0 mean OK.
uint8_t Cat9555_I2cApiWrite(uint8_t bus,uint8_t add,uint8_t regadd,uint8_t *data,uint8_t len)
{
	/*** need edit api ***/
	if(bus==1)
	{
		if(I2C1_Write_Operation(add,1,len,&regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	else if(bus==2)
	{
		if(I2C2_Write_Operation(add,1,len,&regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	
	return 0;
}

uint8_t Cat9555_I2cApiRead(uint8_t bus,uint8_t add,uint8_t regadd,uint8_t *data,uint8_t len)
{
	/*** need edit api ***/
	if(bus==1)
	{
		if(I2C1_Read_Operation(add,1,len,&regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	else if(bus==2)
	{
		if(I2C2_Read_Operation(add,1,len,&regadd,data)!=0)
			return 1;
		else
			return 0;
	}
	
	return 0;
}

uint8_t Cat9555_I2cBusSwitch(uint8_t bus,uint8_t chipid,uint8_t ch)
{
	if(bus==1)
	{
		if(I2C1_Write_Operation(chipid,0,1,&ch,&ch)!=0)
			return 1;
		else
			return 0;
	}
	else if(bus==2)
	{
		if(I2C2_Write_Operation(chipid,0,1,&ch,&ch)!=0)
			return 1;
		else
			return 0;
	}
	
	return 1;
}


/********************** User API *********************/

uint8_t Cat9555_Config(uint8_t bus,uint8_t ICx,uint8_t *configdata)//ICx is 1 to 8
{
	uint8_t add=0;
	uint8_t temp[]={0x00,0x00};
	add=((ICx-1) << 1) + 0x40;
	if(Cat9555_I2cApiWrite(bus,add,0x06,configdata,2)!=0)
	{
		return 1;
	}
	if(Cat9555_I2cApiWrite(bus,add,0x02,temp,2)!=0)
	{
		return 1;
	}
	return 0;
	
}


void Cat9555_Init_8IC(void)
{
	uint8_t cfg[2]={0x00,0x00};
	Cat9555_I2cBusSwitch(2,0xe0,0x01);
	for(int i=1;i<9;i++)
	{
		Cat9555_Config(2,i,cfg);
	}
	Cat9555_I2cBusSwitch(2,0xe0,0x02);
	for(int i=1;i<9;i++)
	{
		Cat9555_Config(2,i,cfg);
	}
	Cat9555_Config(2,1,cfg);
	
}


uint8_t Cat9555_Output(uint8_t bus,uint8_t ch,uint8_t value)//control each IO; ch1 to ch128
{
	uint8_t chx,ic_add,reg_add,io,temp;

	chx=ch-1;
	ic_add=((chx/16)<<1)+0x40;
	reg_add=chx%16/8+2;
	io=chx%16%8;

	if (Cat9555_I2cApiRead(bus,ic_add,reg_add,&temp,1)!=0)
		return 1;
	if (value==1)
		temp=temp | BIT_OR[io];
	else
		temp=temp & BIT_AND[io];
	if (Cat9555_I2cApiWrite(bus,ic_add,reg_add,&temp,1)!=0)	
		return 1;
	else
		return 0;

}
uint8_t Cat9555_Input(uint8_t bus,uint8_t ch,uint8_t *value)
{
	uint8_t chx,ic_add,reg_add,io,temp;

	chx=ch-1;
	ic_add=((chx/16)<<1)+0x40;
	reg_add=chx%16/8+2;
	io=chx%16%8;
	if (Cat9555_I2cApiRead(bus,ic_add,reg_add,&temp,1)!=0)
		return 1;
	temp=temp && BIT_OR[io];
	if (temp==BIT_OR[io])
		*value=1;
	else
		*value=0;
	return 0;

}
