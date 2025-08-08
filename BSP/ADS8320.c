#include "ADS8320.h"
#include "Delay.h"
#include "SPI_Driver.h"

/**************Hard Lever API************/
//#include "stm32f10x_gpio.h"

#define ADS8320_VREF	5000
/************Error Board A14*****************/
void ADS8320_Config(void)
{

 GPIO_InitTypeDef GPIO_InitStructure;
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOF, ENABLE);
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//CLK
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 GPIO_InitStructure.GPIO_Mode =GPIO_Mode_Out_PP;//GPIO_Mode_Out_OD;// 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		//CS
 GPIO_Init(GPIOC, &GPIO_InitStructure);   
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;		//CS
 GPIO_Init(GPIOF, &GPIO_InitStructure);
 
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;		//DOUT
 GPIO_InitStructure.GPIO_Mode =GPIO_Mode_IPU; 
 GPIO_Init(GPIOB, &GPIO_InitStructure);	
 
GPIO_SetBits(GPIOC, GPIO_Pin_13);
GPIO_ResetBits(GPIOB, GPIO_Pin_13);
 
GPIO_SetBits(GPIOF,GPIO_Pin_2);//CS_AC

}
uint16_t ADS8320_Read(void)
{
 uint8_t i=0;
 uint16_t readdata=0;
 uint8_t delay_t=5;
 delay_us(delay_t);
 GPIO_ResetBits(GPIOC, GPIO_Pin_13);//CS
 delay_us(delay_t);

 for (i = 0; i <6; i++)
 {
	 GPIO_SetBits(GPIOB, GPIO_Pin_13);
	 delay_us(delay_t);
	 GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	 delay_us(delay_t);
 }
 for (i = 0; i < 16; i++)
 {
	 readdata=readdata<<1;
	 if(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)!=0)
	 {
		 readdata=readdata+1;
	 }
	 GPIO_SetBits(GPIOB, GPIO_Pin_13);
	 delay_us(delay_t);
	 GPIO_ResetBits(GPIOB, GPIO_Pin_13);
	 delay_us(delay_t);
 } 
 GPIO_SetBits(GPIOC, GPIO_Pin_13);//CS
 delay_us(4*delay_t);
 return (readdata);  
}
 

/********************** Lower API *********************/
//void ADS8320_Read(uint8_t *data,uint8_t len)
//{
//	SPI2_Read(2,data,len);
//}

///********************** User API *********************/

//float ADS8320ReadVolt(void)
//{
//	uint16_t code;
//	float volt;
//	uint8_t temp[3]={0x00,0x00,0x00};
//	ADS8320_Read(temp,3);
//	code =temp[0]&0x03;
//	code =((code<<8)&0xff00) |temp[1];
//	code =((code<<6)&0xffc0) |((temp[2]>>2)&0x3f);
//	volt=(float)code/65535*ADS8320_VREF;
//	return volt;
//}
float ADS8320ReadVolt(void)
{
	uint16_t code;
	uint8_t i,a,b;
	float v_temp;
	float v_buff[10]={0};
	float volt=0;
	
	for(i=0;i<10;i++)
	{
		code=ADS8320_Read();
		v_buff[i]=(float)code/65535*ADS8320_VREF;
	}
	for(a=0;a<9;a++)
	{
		for(b=0;b<(9-a);b++)
		{
			if(v_buff[b]>v_buff[b+1])
			{
				v_temp=v_buff[b];
				v_buff[b]=v_buff[b+1];
				v_buff[b+1]=v_temp;
			}
		}
	}
	for(i=1;i<9;i++)
	{
		volt=volt+v_buff[i];
	}
	volt=volt/8;
	
	return volt;
}
float ADS8320AverageRead(uint8_t num)
{
	float retVolt=0;
	if(num==0)
		return 0;
	for(int i=0;i<num;i++)
	{
		retVolt += ADS8320ReadVolt();
	}
	retVolt=retVolt/num;
	return retVolt;
}
float ADS8320MiddleRead(uint8_t num)
{
	float BuffTemp[100]={0};
	float volt,temp;
	uint8_t a,b;
	if(num>100)
	{
		volt=-110;
		return volt;
	}
	for(a=0;a<num;a++)
	{
		BuffTemp[a]=ADS8320ReadVolt();
	}
	for(a=0;a<num-1;a++)
	{
		for(b=0;b<(num-a-1);b++)
		{
			if(BuffTemp[b]>BuffTemp[b+1])
			{
				temp=BuffTemp[b];
				BuffTemp[b]=BuffTemp[b+1];
				BuffTemp[b+1]=temp;
			}
		}		
	}
	volt=BuffTemp[num/2];
	return volt;
}
uint8_t ADS8320WAVERead(uint16_t num,float *volt)
{
	uint16_t i;
	float *p_v;
	p_v=volt;
	for(i=0;i<num;i++)
	{
		*p_v=ADS8320ReadVolt();
		p_v++;
	}
	return 0;
}
