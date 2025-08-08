#include "hdc2010.h"
#include "i2c_driver.h"
#include "GPIO.h"
#include "delay.h"
/*
IC:hdc2010
Standard I2C
*/
uint8_t hdc2010_Config_value=0x74;	//5Hz  DRDY enable 
/*				Lower API			*/
uint8_t hdc2010_id=0x80;	//ADDR Connect to GND

uint8_t hdc2010_I2C_Write(uint8_t regadd,uint8_t *data,uint8_t len)
{
	return I2C1_Write_Operation(hdc2010_id,1,len,&regadd,data);
}
uint8_t hdc2010_I2C_Read(uint8_t regadd,uint8_t *data,uint8_t len)
{
	return I2C1_Read_Operation(hdc2010_id,1,len,&regadd,data);
}
uint8_t hdc2010_DRDY_Read(void)
{
	return Data_15_Read();
}
uint8_t hdc2010_Delay(uint32_t time)
{
	delay_ms(time);
	
	return 0;
}


/*						Internal Function								*/
uint8_t hdc2010_Timeout(uint32_t num)
{
	uint32_t i;
	for(i=0;i<num;i++)
	{
		hdc2010_Delay(1);
		if(hdc2010_DRDY_Read()==0)
			return 0;
	}
	return 1;
}
uint8_t hdc2010_Config(void)
{
	uint8_t interrupt_value=0x80;
	if(hdc2010_I2C_Write(HDC2010_Confige,&hdc2010_Config_value,1)!=0)
		return 1;
	if(hdc2010_I2C_Write(HDC2010_Interrupt,&interrupt_value,1)!=0)
		return 1;
	return 0;
}

uint8_t hdc_start_measure(void)
{
	uint8_t value=0x01;
	if(hdc2010_I2C_Write(HDC2010_easure,&value,1)!=0)
		return 1;
	return 0;
}
uint8_t hdc2010_Temperature_Read(float *temperature)
{
	uint8_t temp_8b[2]={0};
	uint16_t temp_16b=0;

	if(hdc2010_I2C_Read(HDC2010_Temperature,temp_8b,2)!=0)
		return 1;
	temp_16b=(uint16_t)temp_8b[0]+((uint16_t)temp_8b[1]<<8);
	*temperature=((float)temp_16b/65536)*165-40;
	return 0;
}
uint8_t hdc2010_Humidity_Read(float *humidity)
{
	uint8_t temp_8b[2]={0};
	uint16_t temp_16b=0;
	if(hdc2010_I2C_Read(HDC2010_Humidity,temp_8b,2)!=0)
		return 1;
	temp_16b=(uint16_t)temp_8b[0]+((uint16_t)temp_8b[1]<<8);
	*humidity=((float)temp_16b/65536)*100;
	return 0;
}
/*					User API						*/
uint8_t HDC2010_Read_ID(uint16_t *hdc2010_id)
{
	uint8_t tmp[2];
	uint16_t id_value;
	if(hdc2010_I2C_Read(HDC2010_DeviceID,tmp,2)!=0)
		return 1;
	id_value=(uint16_t )tmp[1];
	id_value=(id_value<<8)+(uint16_t )tmp[0];
	*hdc2010_id=id_value;
	return 0;
}
uint8_t HDC2010_Heater(uint8_t status)
{
//	uint8_t temp;
	if (status==0)
	{
		hdc2010_Config_value=hdc2010_Config_value & 0xf7;
	}
	else
	{
		hdc2010_Config_value=hdc2010_Config_value | 0x08;
	}
	if(hdc2010_I2C_Write(HDC2010_Confige,&hdc2010_Config_value,1)!=0)
		return 1;
//	hdc2010_I2C_Write(HDC2010_Confige,&hdc2010_Config_value,1);
//	for(temp=0;temp<200;temp++);
//	if (hdc2010_I2C_Read(HDC2010_Confige,&temp,1)!=0)
//		return 1;
//	if(temp!=hdc2010_Config_value)
//		return 2;
	return 0;
}
uint8_t HDC2010_Temp_Humi_Read(float *temp,float *humi)
{
	if(hdc2010_Config()!=0)
		return 1;
	hdc_start_measure();
	if(hdc2010_Timeout(1000)!=0)
		return 2;
	hdc2010_Temperature_Read(temp);
	hdc2010_Humidity_Read(humi);
	if(hdc2010_DRDY_Read()==0)
		return 3;
	return 0;
}
