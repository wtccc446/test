#include "EXT_AD5663.h"
#include "SPI_Driver.h"

#define EXT_AD5663_Reference_Voltage 5000
/**********************************
Read Me:
	EXT_AD5663 is fall edge Driver.
	SPI MODE3,Data right shift 1bit!!!
	SPI MODE0 is OK
***********************************/
#define EXT_AD5663_SPI_MODE3 	1
/******************** Lower API *********************/

void EXT_AD5663_SPI_Write(uint8_t *data,uint8_t len)
{
	SPI1_Write(2,data,len);
}
void EXT_AD5663_Ldac_Control(uint8_t type)
{

}
void EXT_AD5663_Clear_Control(uint8_t type)
{

}
/********************** Internal Function ***********************/
void EXT_AD5663_DAC_Config(uint8_t ch,uint16_t data)
{
	uint32_t all_data;
	uint8_t tmp[3]={0};
	if (ch==1)
	{
		tmp[0]=Ext_Write_Register | Ext_DAC_A;
	}
	else if(ch==2)
	{
		tmp[0]=Ext_Write_Register | Ext_DAC_B;
	}
	else if(ch==3)
	{
		tmp[0]=Ext_Write_Register | Ext_DAC_ALL;
	}
	all_data=tmp[0]<<16;
	all_data=all_data | data;
#ifdef EXT_AD5663_SPI_MODE3
	all_data=all_data<<1;
#endif
	tmp[0]=(all_data>>16) & 0x000000ff;
	tmp[1]=(all_data >> 8) & 0x000000ff;
	tmp[2]=(all_data ) & 0x000000ff;
	EXT_AD5663_SPI_Write(tmp,3);
}
/********************** USER API ***********************/
void EXT_AD5663_DAC_Init(void)
{
	uint8_t tmp[3]={0};
	uint32_t all_data=0x00030003;
#ifdef EXT_AD5663_SPI_MODE3
	all_data=all_data<<1;
#endif
	tmp[0]=(all_data>>16) & 0x000000ff;
	tmp[1]=(all_data >> 8) & 0x000000ff;
	tmp[2]=(all_data ) & 0x000000ff;
	EXT_AD5663_SPI_Write(tmp,3);
}
void EXT_AD5663_DAC_Set(uint8_t ch,float voltage)
{
	uint16_t code;
	code = (uint16_t)(voltage/EXT_AD5663_Reference_Voltage*65535);
	EXT_AD5663_DAC_Config(ch,code);
}
void EXT_AD5663_DAC_Reset(void)
{
	EXT_AD5663_DAC_Config(3,0);
}
