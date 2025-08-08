/***************************************************
name：TEST
NOTE:
1.First Version  2016/02/25
功能描述：设置继电器开关
Editer：Xin
***************************************************/
#include "GPIO.h"

/***************************************************
name：testboard_int
NOTE:
1.First Version  2016/02/25
功能描述：Test Board Reley Int
Editor：Xin
***************************************************/

void gpio_int(void)
{	
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |RCC_APB2Periph_GPIOD , ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
	/**************Output*************/
	//PA1
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
//	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_12;
//	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
//	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	GPIO_SetBits(GPIOA,GPIO_Pin_12);
	//PB9	
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//PC13 PC14 PC15	
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_14;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_IPU;	
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	//PC0 PC1 PC2	PC3
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);
	//PC6 PC7 PC8 PC11
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_11;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);
	//PC9,PC10
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	//GPIO_SetBits(GPIOC,GPIO_Pin_9);
	//GPIO_SetBits(GPIOC,GPIO_Pin_10);
	
	//PB6 PB7 PB0 PB1 
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_0|GPIO_Pin_1;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//PA0 
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//PB3 PB4 PB5 
	//GPIO_InitStruct.GPIO_Pin=GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5;
	//GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	//GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	//GPIO_Init(GPIOB, &GPIO_InitStruct);

	//GPIO_SetBits(GPIOA,GPIO_Pin_11);
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8|GPIO_Pin_9;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_8);
	GPIO_ResetBits(GPIOC,GPIO_Pin_9);
}

void led_control(char i)
{
	if (i==0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_1);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_1);
}
void AD5663_LDAC(char i)
{
	if (i==0)
		GPIO_ResetBits(GPIOA,GPIO_Pin_11);
	else
		GPIO_SetBits(GPIOA,GPIO_Pin_11);
}
void W5500_RST_Control(char i)
{
	if (i==0)
		GPIO_ResetBits(GPIOB,GPIO_Pin_9);
	else
		GPIO_SetBits(GPIOB,GPIO_Pin_9);
}
uint8_t W5500_Add_Read(void)
{
	uint8_t ret=0x07;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)==0)
		ret=ret & 0xFE;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_14)==0)
		ret=ret & 0xFD;
	if(GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_15)==0)
		ret=ret & 0xFB;
	return ret;
}
uint8_t Data_15_Read(void)
{
	uint8_t ret=0;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_15)==0)
		ret=0;
	else
		ret=1;
	return ret;
}
uint8_t Data_14_Read(void)
{
	uint8_t ret=0;
	if(GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_14)==0)
		ret=0;
	else
		ret=1;
	return ret;
}
/***************************File End*******************************/
