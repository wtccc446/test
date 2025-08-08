#include "SHT85.h"
#include "i2c_driver.h"

/*			Lower API			*/
uint8_t SHT85_I2C_Write(uint8_t chipid,uint8_t H_regadd,uint8_t L_regadd,uint8_t *data,uint8_t len)
{
	uint8_t regadd[2]={0};
	regadd[0]=H_regadd;
	regadd[1]=L_regadd;
	if(I2C1_Write_Operation(chipid,2,len,regadd,data)!=0)
		return 1;
	return 0;
}

/*	must wait bigger than 1ms after write 	*/
uint8_t SHT85_I2C_Read(uint8_t chipid,uint8_t H_regadd,uint8_t L_regadd,uint8_t *data,uint8_t len)
{
	uint8_t regadd[2]={0};
	regadd[0]=H_regadd;
	regadd[1]=L_regadd;
	if(I2C1_Read_Operation(chipid,2,len,regadd,data)!=0)
		return 1;
	return 0;
}
uint8_t SHT85_I2C_OnlyRead(uint8_t chipid,uint8_t *data,uint8_t len)
{
	return 0;
}


void SHT85_Delay(uint32_t num)
{
//	uint32_t x,y;
	for (int x = 0; x < num; ++x)
	{
		for (int y = 0; y < 1000; ++y); 
	}
}
/*			Internal API			*/

uint8_t SHT85_Write_Command(uint16_t SHT85_Cmd)
{
	uint8_t msb_code,lsb_code;
	msb_code=(uint8_t)(SHT85_Cmd>>8);
	lsb_code=(uint8_t)(SHT85_Cmd);
	if(SHT85_I2C_Write(SHT85_Address,msb_code,lsb_code,&msb_code,0)!=0)
		return 1;
	return 0;

}
uint8_t SHT85_Read_Hex(uint16_t* V_Temperature,uint16_t *V_Humidity)
{
	uint8_t data[6];

	if (SHT85_I2C_Read(SHT85_Address,0xe0,0x00,data,6)!=0)
		return 1;
	*V_Temperature=data[0];
	*V_Temperature=(*V_Temperature<<8)+data[1];
	*V_Humidity=data[3];
	*V_Humidity=(*V_Humidity<<8)+data[4];
	return 0;
}

/*			User API			*/
uint8_t SHT85_Read(float *Temp,float *Humi)
{
	uint16_t Hex_Temp=0;
	uint16_t Hex_Humi=0;
	if(SHT85_Write_Command(SHT85_10mps_High)!=0)
		return 1;
	SHT85_Delay(10000);
	if(SHT85_Read_Hex(&Hex_Temp,&Hex_Humi)!=0)
		return 1;
	if(SHT85_Write_Command(SHT85_Stop_Periodic)!=0)
		return 1;
	*Temp=(float )Hex_Temp/65535*175-45;
	*Humi=(float )Hex_Humi/65535*100;
	return 0;
}

