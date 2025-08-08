#include "I2C_Driver.h"


#ifdef HWI2C

#endif

#ifdef SWI2C
void SWI2C_Delay(uint32_t num)
{
	uint32_t x,i;
	for(x=0;x<num;x++)
	{	
		for(i=0;i<3;i++);
	}
}
#ifdef My_I2C1

/*
100K
SCK 5us
SDA	10us
*/

void I2C1_Init(void)
{
  	/*Configure GPIO pins : 
		PB6 :SCL
		PB7	:SDA
	*/
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef HALLib
   
#endif
	
}
/**********Internal API***********/
void 	I2C1_SDA_TurnOut()
{
#ifdef STDLib
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef HALLib
   
#endif
	
}
void 	I2C1_SDA_TurnIn()
{
#ifdef STDLib
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
#endif
#ifdef HALLib
   
#endif
	
}
void I2C1_SDA(char i)
{
#ifdef STDLib
	if(i==0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_7);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_7);
#endif
#ifdef HALLib
   
#endif
}
void I2C1_SCL(char i)
{
#ifdef STDLib
  if(i==0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_6);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_6);
#endif
#ifdef HALLib
   
#endif
}
char I2C1_SDA_In()
{
#ifdef STDLib
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)==0)
		 return 0;
	 else
		 return 1;
#endif
#ifdef HALLib
   
#endif
}
/**********Internal API***********/


void I2C1_Start(void)
{
	I2C1_SDA_TurnOut();
	I2C1_SDA(1);
	I2C1_SCL(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SDA(0);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(0);
}
void I2C1_Stop(void)
{
	I2C1_SDA_TurnOut();
	I2C1_SCL(0);
	I2C1_SDA(0);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SDA(1);	
	SWI2C_Delay(SWI2C1_DELAY_TIME);
}
unsigned char I2C1_Wait_Ack(void)
{
	unsigned char ucErrTime=0;	
	I2C1_SDA(1);
	I2C1_SDA_TurnIn();
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	while(I2C1_SDA_In()!=0)
	{
		ucErrTime++;
		SWI2C_Delay(SWI2C1_DELAY_TIME);
		if(ucErrTime>50)
		{
			return 1;
		}
	}
	I2C1_SCL(0);
	return 0;
}
void I2C1_Ack(void)
{
	I2C1_SCL(0);
	I2C1_SDA_TurnOut();
	I2C1_SDA(0);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(0);
}
void I2C1_NAck(void)
{
	I2C1_SCL(0);
	I2C1_SDA_TurnOut();
	I2C1_SDA(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(1);
	SWI2C_Delay(SWI2C1_DELAY_TIME);
	I2C1_SCL(0);
}
/**************************************************/
void I2C1_Write_Byte(unsigned char txd)
{
	unsigned char t;
	I2C1_SDA_TurnOut();
	I2C1_SCL(0);
	for(t=0;t<8;t++)
	{
		I2C1_SDA((txd&0x80)>>7);
		txd<<=1;
		SWI2C_Delay(SWI2C1_DELAY_TIME);
		I2C1_SCL(1);
		SWI2C_Delay(SWI2C1_DELAY_TIME);
		I2C1_SCL(0);
		SWI2C_Delay(SWI2C1_DELAY_TIME);
	}
}
/**************************************************/
unsigned char I2C1_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	I2C1_SDA_TurnIn();
	for(i=0;i<8;i++)
	{
		I2C1_SCL(0);
		SWI2C_Delay(SWI2C1_DELAY_TIME);
		I2C1_SCL(1);
		receive<<=1;
		if(I2C1_SDA_In()!=0)
			receive++;
		SWI2C_Delay(SWI2C1_DELAY_TIME);
	}
	if(!ack) 
		I2C1_NAck();
	else 
		I2C1_Ack();
	return receive;
}
uint8_t I2C1_Write_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata)
{
	uint16_t i;
	I2C1_Start();
	//Æ÷¼þÐ´µØÖ·
	I2C1_Write_Byte(chipid);
	//µÈ´ýÓ¦´ð
	if(I2C1_Wait_Ack()!=0)
	{
		I2C1_Stop();
		return 1;		
	}
	//¼Ä´æÆ÷µØÖ·
	for(i=0;i<add_len;i++)
	{
		I2C1_Write_Byte(regadd[i]);
		if(I2C1_Wait_Ack()!=0)
		{
			I2C1_Stop();
			return 1;			
		}
	}
	//¼Ä´æÆ÷Êý¾Ý
	for(i=0;i<data_len;i++)
	{
		I2C1_Write_Byte(regdata[i]);
		if(I2C1_Wait_Ack()!=0)
		{
			I2C1_Stop();
			return 1;			
		}
	}
	I2C1_Stop();
	return 0;
}
uint8_t I2C1_Read_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata)
{
	uint16_t i;
	uint8_t *pdata;
	pdata=regdata;
	I2C1_Start();
	I2C1_Write_Byte(chipid);
	if(I2C1_Wait_Ack()!=0)
	{
		I2C1_Stop();
		return 1;
	}
	for(i=0;i<add_len;i++)
	{
		I2C1_Write_Byte(regadd[i]);
		if(I2C1_Wait_Ack()!=0)
		{
			I2C1_Stop();
			return 1;			
		}
	}
		
	I2C1_Start();
	I2C1_Write_Byte(chipid+1);;
	if(I2C1_Wait_Ack()!=0)
	{
		I2C1_Stop();
		return 1;
	}		
			
	for(i=0;i<data_len;i++)
	{
		if(i==data_len-1)
			*pdata=I2C1_Read_Byte(0);
		else
		{
			*pdata=I2C1_Read_Byte(1);
			pdata++;
		}			
	}	
	I2C1_Stop();
	return 0;
}
uint8_t I2C1_Read_Direct(uint8_t chipid,uint8_t data_len,uint8_t *regdata)
{
	uint16_t i;
	uint8_t *pdata;
	pdata=regdata;
	I2C1_Start();
	I2C1_Write_Byte(chipid+1);;
	if(I2C1_Wait_Ack()!=0)
	{
		I2C1_Stop();
		return 1;
	}		
			
	for(i=0;i<data_len;i++)
	{
		if(i==data_len-1)
			*pdata=I2C1_Read_Byte(0);
		else
		{
			*pdata=I2C1_Read_Byte(1);
			pdata++;
		}			
	}	
	I2C1_Stop();
	return 0;
}
#endif	//My_I2C1
#ifdef My_I2C2

/*
100K
SCK 5us
SDA	10us
*/

void I2C2_Init(void)
{
  	/*Configure GPIO pins : 
		PD12 	:SCL
		PD11	:SDA
	*/
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef HALLib
   
#endif
	
}
/**********Internal API***********/
void 	I2C2_SDA_TurnOut()
{
#ifdef STDLib
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
#endif
#ifdef HALLib
   
#endif
	
}
void 	I2C2_SDA_TurnIn()
{
#ifdef STDLib
  GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure); 
#endif
#ifdef HALLib
   
#endif
	
}
void I2C2_SDA(char i)
{
#ifdef STDLib
	if(i==0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_11);
#endif
#ifdef HALLib
   
#endif
}
void I2C2_SCL(char i)
{
#ifdef STDLib
  if(i==0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_10);
#endif
#ifdef HALLib
   
#endif
}
char I2C2_SDA_In()
{
#ifdef STDLib
   if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)==0)
		 return 0;
	 else
		 return 1;
#endif
#ifdef HALLib
   
#endif
}
/**********Internal API***********/


void I2C2_Start(void)
{
	I2C2_SDA_TurnOut();
	I2C2_SDA(1);
	I2C2_SCL(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SDA(0);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(0);
}
void I2C2_Stop(void)
{
	I2C2_SDA_TurnOut();
	I2C2_SCL(0);
	I2C2_SDA(0);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SDA(1);	
	SWI2C_Delay(SWI2C2_DELAY_TIME);
}
unsigned char I2C2_Wait_Ack(void)
{
	unsigned char ucErrTime=0;	
	I2C2_SDA(1);
	I2C2_SDA_TurnIn();
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	while(I2C2_SDA_In()!=0)
	{
		ucErrTime++;
		SWI2C_Delay(SWI2C2_DELAY_TIME);
		if(ucErrTime>50)
		{
			return 1;
		}
	}
	I2C2_SCL(0);
	return 0;
}
void I2C2_Ack(void)
{
	I2C2_SCL(0);
	I2C2_SDA_TurnOut();
	I2C2_SDA(0);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(0);
}
void I2C2_NAck(void)
{
	I2C2_SCL(0);
	I2C2_SDA_TurnOut();
	I2C2_SDA(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(1);
	SWI2C_Delay(SWI2C2_DELAY_TIME);
	I2C2_SCL(0);
}
/**************************************************/
void I2C2_Write_Byte(unsigned char txd)
{
	unsigned char t;
	I2C2_SDA_TurnOut();
	I2C2_SCL(0);
	for(t=0;t<8;t++)
	{
		I2C2_SDA((txd&0x80)>>7);
		txd<<=1;
		SWI2C_Delay(10);
		I2C2_SCL(1);
		SWI2C_Delay(20);
		I2C2_SCL(0);
		SWI2C_Delay(10);
	}
}
/**************************************************/
unsigned char I2C2_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	I2C2_SDA_TurnIn();
	for(i=0;i<8;i++)
	{
		I2C2_SCL(0);
		SWI2C_Delay(10);
		I2C2_SCL(1);
		receive<<=1;
		if(I2C2_SDA_In()!=0)receive++;
		SWI2C_Delay(10);
	}
	if(!ack) 
		I2C2_NAck();
	else 
		I2C2_Ack();
	return receive;
}
uint8_t I2C2_Write_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata)
{
	uint16_t i;
	I2C2_Start();
	I2C2_Write_Byte(chipid);
	if(I2C2_Wait_Ack()!=0)
	{
		I2C2_Stop();
		return 1;		
	}
	for(i=0;i<add_len;i++)
	{
		I2C2_Write_Byte(regadd[i]);
		if(I2C2_Wait_Ack()!=0)
		{
			I2C2_Stop();
			return 1;			
		}
	}
	
	for(i=0;i<data_len;i++)
	{
		I2C2_Write_Byte(regdata[i]);
		if(I2C2_Wait_Ack()!=0)
		{
			I2C2_Stop();
			return 1;			
		}
	}
	I2C2_Stop();
	return 0;
}
uint8_t I2C2_Read_Operation(uint8_t chipid,uint8_t add_len,uint8_t data_len,uint8_t *regadd,uint8_t *regdata)
{
	uint16_t i;
	uint8_t *pdata;
	pdata=regdata;
	I2C2_Start();
	I2C2_Write_Byte(chipid);;
	if(I2C2_Wait_Ack()!=0)
	{
		I2C2_Stop();
		return 1;
	}
	for(i=0;i<add_len;i++)
	{
		I2C2_Write_Byte(regadd[i]);
		if(I2C2_Wait_Ack()!=0)
		{
			I2C2_Stop();
			return 1;			
		}
	}
		
	I2C2_Start();
	I2C2_Write_Byte(chipid+1);;
	if(I2C2_Wait_Ack()!=0)
	{
		I2C2_Stop();
		return 1;
	}		
			
	for(i=0;i<data_len;i++)
	{
		if(i==data_len-1)
			*pdata=I2C2_Read_Byte(0);
		else
		{
			*pdata=I2C2_Read_Byte(1);
			pdata++;
		}			
	}	
	I2C2_Stop();
	return 0;
}
uint8_t I2C2_Read_Direct(uint8_t chipid,uint8_t data_len,uint8_t *regdata)
{
	uint16_t i;
	uint8_t *pdata;
	pdata=regdata;
	I2C2_Start();
	I2C2_Write_Byte(chipid+1);;
	if(I2C2_Wait_Ack()!=0)
	{
		I2C2_Stop();
		return 1;
	}		
			
	for(i=0;i<data_len;i++)
	{
		if(i==data_len-1)
			*pdata=I2C2_Read_Byte(0);
		else
		{
			*pdata=I2C2_Read_Byte(1);
			pdata++;
		}			
	}	
	I2C2_Stop();
	return 0;
}
#endif	//My_I2C2
#endif	//SWI2C
