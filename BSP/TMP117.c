#include "TMP117.h"
#include "I2C_Driver.h"
#include "delay.h"

//TMP117 GET TMP
const uint8_t TMP117_GET_TMP[]="TMP117 GET TMP";

unsigned char TMP117_Write(unsigned char addr_reg, unsigned char data_reg, unsigned char byte1, unsigned char byte2)
{
	addr_reg = addr_reg << 1;
	//I2C1_Init();
	I2C1_Start();
	I2C1_Write_Byte(addr_reg);//Ð´µØÖ·
	I2C1_Ack();
	I2C1_Write_Byte(data_reg);
	I2C1_Ack();
	I2C1_Write_Byte(byte1);
	I2C1_Ack();
	I2C1_Write_Byte(byte2);
	I2C1_Ack();
	I2C1_Stop();
	delay_ms(1);
	return 0;
	
}

unsigned int TMP117_Read(unsigned char addr_reg, unsigned char data_reg)
{
	unsigned int data;
	addr_reg = addr_reg << 1;
  //I2C1_Init();
	I2C1_Start();
	I2C1_Write_Byte(addr_reg);
	I2C1_Wait_Ack();
	I2C1_Write_Byte(data_reg);
	I2C1_Wait_Ack();
	I2C1_Start();
	I2C1_Write_Byte(addr_reg + 1);
	I2C1_Wait_Ack();
	data = I2C1_Read_Byte(1);//get High 8-bit data
	I2C1_Wait_Ack();
	data = data << 8;
	data |=  I2C1_Read_Byte(1);//get Low 8-bit data
	I2C1_Ack();
	I2C1_Stop();
	return data;
}

//resolution = 0.007815c
float TMP117_Get_Temp(void)
{
	uint16_t value;
	float temp_data;
	//ÅäÖÃ¼Ä´æÆ÷
	TMP117_Write(TMP117_ADDR_1,TMP117_CFG_REG,0x00,0x60);//64 pcs average £¿£¿
	//¶ÁÈ¡ÎÂ¶ÈÖµ
	value = TMP117_Read(TMP117_ADDR_1,TMP117_VALUE_REG);
//	if((value & 0x8000) == 0x8000)//temp value < 0
//	{
//		value = ~value;
//		value += 1;
//	}
	temp_data = value * 0.007815;
	MessageSend("debug value: %d\r\n",value);
	
	return temp_data;
}


//TMP117 GET TMP
void TMP117_Read_Cmd(uint8_t *buff)
{
	float v;
	if(buff[0]=='T' && buff[5]=='7' && buff[13]=='P')
	{
		v = TMP117_Get_Temp();
		MessageSend("Temp value: %f\r\n",v);
	}
}

