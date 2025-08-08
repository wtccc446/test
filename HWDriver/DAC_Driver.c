#include "DAC_Driver.h"

#ifdef STDLib
#include "stm32f10x_dac.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#endif

#ifdef My_DAC1
void DAC1_GpioInit()
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
#endif
}

void DAC1_Init(void)
{
#ifdef STDLib
	DAC_InitTypeDef		DAC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC1_GpioInit();
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_1, ENABLE);
	DAC_SetChannel1Data(DAC_Align_12b_R,0);
#endif
}
#endif
#ifdef My_DAC2
void DAC2_GpioInit()
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure); 
#endif
}

void DAC2_Init(void)
{
#ifdef STDLib
	DAC_InitTypeDef		DAC_InitStructure;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	
	DAC2_GpioInit();
	
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_None;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);
	DAC_Init(DAC_Channel_2, &DAC_InitStructure);
	
	DAC_Cmd(DAC_Channel_2, ENABLE);
	DAC_SetChannel2Data(DAC_Align_12b_R,0);
#endif
}
#endif
void DAC_Set(uint8_t ch,uint16_t data)
{
	switch(ch)
	{
		case 1:
			DAC_SetChannel1Data(DAC_Align_12b_R,data&0x0FFF);
			break;
		case 2:
			DAC_SetChannel2Data(DAC_Align_12b_R,data&0x0FFF);
			break;
		default:
			break;
	}
}
