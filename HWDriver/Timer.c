#include "Timer.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x.h" 

#include "Interface_Cmd.h"
#include "Usart.h"	  

uint32_t gTimerBase=0;

#ifdef Tim_1
void Timer1_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_BaseInitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);		//外设时钟开启
	
	//设置中断优先级
	NVIC_InitStruct.NVIC_IRQChannel=TIM1_UP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=3;
  NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;	
  NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;   
	NVIC_Init(&NVIC_InitStruct);
	
	//Time=(99+1)*(719+1)/72M=1ms
	TIM_BaseInitStructure.TIM_Period = 99;					
	TIM_BaseInitStructure.TIM_Prescaler = 719;
	TIM_BaseInitStructure.TIM_ClockDivision = 0;
	TIM_BaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_BaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM1, &TIM_BaseInitStructure);

	TIM_ClearFlag(TIM1, TIM_FLAG_Update);
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	TIM_Cmd(TIM1, ENABLE);
}

/*----------------------------------------------------------------------------
  TIM1_UP_IRQHandler
  Handles TIM1_UP_IRQHandler global interrupt request.
 *----------------------------------------------------------------------------*/
__weak void Tim1_Callback(void)
{
	
}
void TIM1_UP_IRQHandler(void)
{

	Tim1_Callback();
	gTimerBase++;
	if(gTimerBase==100000)
		gTimerBase=0;
	TIM_ClearITPendingBit(TIM1, TIM_FLAG_Update); 
}


#endif


