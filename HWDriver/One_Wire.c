#include "One_Wire.h"

#include "Delay.h"

/***********Lower Lever API*************/
void One_Wire_DelayTimeBase(uint32_t delay)
{
	delay_us(delay);
}

void One_Wire_GpioInit()
{
	#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
#endif
#ifdef HALLib
   
#endif
}
void One_Wire_ChangeOut(void)
{
	#ifdef STDLib

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	#endif
#ifdef HALLib
   
#endif 
}
void One_Wire_ChangeIn(void)
{
	#ifdef STDLib

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);	

	
	#endif
#ifdef HALLib
   
#endif 
}
uint8_t One_Wire_GpioRead(void)
{
	#ifdef STDLib
	 if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8)==0)
		 return 0;
	 else
		 return 1;
	#endif
#ifdef HALLib
   
#endif 
}
void One_Wire_GpioWrite(uint8_t val)
{
	if(val==0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_8);
}

/***************User API*****************/
void One_Wire_Init()
{
	One_Wire_GpioInit();	
}
void One_Wire_Reset(uint32_t lowtime,uint32_t delaytime)
{
	One_Wire_ChangeOut();
	One_Wire_GpioWrite(0);
	One_Wire_DelayTimeBase(lowtime);
	One_Wire_GpioWrite(1);
	One_Wire_DelayTimeBase(delaytime);	
}
/*
return :
0:OK
1:Error
*/
uint8_t One_Wire_Check(uint32_t timeout,uint32_t delay)
{
	uint8_t err_flag=1;
	uint32_t i;
	One_Wire_ChangeIn();
	for(i=0;i<timeout;i++)
	{
		if(One_Wire_GpioRead()==0)
		{
			err_flag=0;
			break;
		}
	}
	One_Wire_DelayTimeBase(delay);
	if(err_flag==0)
		return 0;
	else
		return 1;
}
void One_Wire_Write1B(uint8_t bit,uint32_t starttime,uint32_t delay,uint32_t endtime)
{
	One_Wire_ChangeOut();
	One_Wire_GpioWrite(0);
	One_Wire_DelayTimeBase(starttime);
	One_Wire_GpioWrite(bit);
	One_Wire_DelayTimeBase(delay);
	One_Wire_GpioWrite(1);
	One_Wire_DelayTimeBase(endtime);
}
uint8_t One_Wire_Read1B(uint32_t starttime,uint32_t delay)
{
	uint8_t bit;
	One_Wire_ChangeOut();
	One_Wire_GpioWrite(0);
	One_Wire_DelayTimeBase(starttime);
	One_Wire_ChangeIn();	
	bit=One_Wire_GpioRead();
	One_Wire_DelayTimeBase(delay);
	return bit;
}

void One_Wire_Write8B(uint8_t byte,uint32_t starttime,uint32_t delay,uint32_t endtime)
{
	uint8_t bit,i,data;
	bit=0;
	for(i=0;i<8;i++)
	{
		bit=byte&0x01;
		One_Wire_Write1B(bit,starttime,delay,endtime);
		byte=byte>>1;
	}
}
uint8_t One_Wire_Read8B(uint32_t starttime,uint32_t delay)
{
	uint8_t data,i,bit;
	data=0;
	for(i=0;i<8;i++)
	{
		data=data>>1;
		bit=One_Wire_Read1B(starttime,delay);
		data=data+(bit<<7);
	}
	return data;
}