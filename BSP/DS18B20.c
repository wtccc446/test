#include "DS18B20.h"
#include "One_Wire.h"
#include "Delay.h"


/***************Lower API****************/
uint8_t DS18B20_Setup()
{
	One_Wire_Reset(500,20);
	if (One_Wire_Check(100,400)!=0)
		return 1;
	else
		return 0;
}
void DS18B20_WriteByte(uint8_t data)
{
	One_Wire_Write8B(data,2,60,2);
}
uint8_t DS18B20_ReadByte(void)
{
	One_Wire_Read8B(2,45);
}
void DS18B20_Delay(uint16_t tim)
{
	delay_ms(tim);
}
/***************User API****************/
void DS18B20_Init()
{
	One_Wire_Init();
	DS18B20_SetResolution(DS18B20_Resolution_10);
}
void DS18B20_SetResolution(uint8_t data)
{
	if (DS18B20_Setup()!=0)
	{
		return ;
	}
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x4e);
	DS18B20_WriteByte(0xff);
	DS18B20_WriteByte(0xff);
	DS18B20_WriteByte(data);
}
void DS18B20_Convert()
{
	if (DS18B20_Setup()!=0)
	{
		return ;
	}
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0x44);
	DS18B20_Delay(500);
}
uint8_t DS18B20_ReadTemp(float *temp)
{
	uint8_t Ldata,Hdata;
	uint16_t data;
	DS18B20_Convert();
	
	if (DS18B20_Setup()!=0)
	{
		return 1;
	}
	DS18B20_WriteByte(0xcc);
	DS18B20_WriteByte(0xbe);
	Ldata=DS18B20_ReadByte();
	Hdata=DS18B20_ReadByte();
	data=Hdata;
	data=data<<8;
	data=data+Ldata;
	*temp=(float )(data>>4)+((float )(data&0x000F)*0.0625);
	return 0;
}

