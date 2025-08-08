#include "AD5663.h"
#include "SPI_Driver.h"
#include "GPIO.h"
#include "messageout.h"
#define AD5663_Reference_Voltage 4096
/**********************************
Read Me:
	AD5663 is fall edge Driver.
	SPI MODE3,Data right shift 1bit!!!
	SPI MODE0 is OK
***********************************/
//#define AD5663_SPI_MODE3 	1
/******************** Lower API *********************/

void AD5663_SPI_Write(uint8_t *data,uint8_t len)
{
	SPI3_Write(4,data,len);
}
void AD5663_Ldac_Control(void)
{
	uint16_t i=0;
	AD5663_LDAC(0);
	for(i=0;i<1000;i++);
	AD5663_LDAC(1);
}
void AD5663_Clear_Control(uint8_t type)
{
		
}
/**
 *	
 *
 */
/********************** Internal Function ***********************/
void AD5663_DAC_Config(uint8_t ch,uint16_t data)
{
	uint32_t all_data;
	uint8_t tmp[3]={0};
	if (ch==1)
	{
		tmp[0]=Write_Updata_n | DAC_A;
	}
	else if(ch==2)
	{
		tmp[0]=Write_Updata_n | DAC_B;
	}
	else if(ch==3)
	{
		tmp[0]=Write_Updata_n | DAC_ALL;
	}
	all_data=tmp[0]<<16;
	all_data=all_data | data;
#ifdef AD5663_SPI_MODE3
	all_data=all_data<<1;
#endif
	tmp[0]=(all_data>>16) & 0x000000ff;
	tmp[1]=(all_data >> 8) & 0x000000ff;
	tmp[2]=(all_data ) & 0x000000ff;
	AD5663_SPI_Write(tmp,3);
	MessageSend("AD5663 SPI writed.");//debug massege
	AD5663_Ldac_Control();
}
/********************** USER API ***********************/
void AD5663_DAC_Init(void)
{
	AD5663_DAC_Config(3,0);
}
void AD5663_DAC_Set(uint8_t ch,float voltage)
{
	uint16_t code;
	code = (uint16_t)((voltage/AD5663_Reference_Voltage)*65535);
	MessageSend("%d\r\n",code);//debug massege
	AD5663_DAC_Config(ch,code);
}
void AD5663_DAC_Reset(void)
{
	AD5663_DAC_Config(3,0);
}
