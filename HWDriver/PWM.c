#include "PWM.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"


uint16_t PWM_Count=99;
void Tim5_Pwm_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct; // 定义的TIM属性结构体变量
	GPIO_InitTypeDef GPIO_InitStruct; // 定义GPIO类型变量
	TIM_OCInitTypeDef TIM_OCInitStruct; // 定义复用功能的变量

	// 1.初始化时钟：TIM5 和 PA3
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/* TIM5 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	// 2.配GPIO引脚为复用功能
	/* GPIOC Configuration: TIM5 CH4 (PA3) */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_3; // 选择引脚为PA3
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP; // 设置为复用功能
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; // 
	GPIO_Init(GPIOA, &GPIO_InitStruct);  // 安装参数

	// 3.将TIM和引脚的复用功能连接：TIM5和PA3连接起来
//	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_TIM5);

	// 4.配置TIM定时器的参数
	TIM_TimeBaseInitStruct.TIM_Period = PWM_Count; // 设置重装载值 ARR（控制频率）
	TIM_TimeBaseInitStruct.TIM_Prescaler =6; // 设置预分频系数：周期（次）  100K
	TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1; // 设置再分频值：TIM_CKD_DIV1就是不分频
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 设置计数模式
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseInitStruct);

	// 5.配置复用功能：PWM
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1; // 配置为PWM模式1
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable; // 开启输出使能
	//TIM_OCInitStruct.TIM_Pulse = CCR1_Val; // 初始化配置比较值寄存器
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High; // 配置为高电平有效

	// 6.TIM5通道4初始化
	TIM_OC4Init(TIM5, &TIM_OCInitStruct); // TIM14通道1初始化

	// 7.设置自动重载比较值CCR1初值，不断产生PWM脉冲
	TIM_OC4PreloadConfig(TIM5, TIM_OCPreload_Enable);


	// 8.设置自动重装载值（ARR），不断产生PWM脉冲
	TIM_ARRPreloadConfig(TIM5, ENABLE);

	/* 9.使能定时器5 */
	TIM_Cmd(TIM5, ENABLE);
	
	

	// 10.使能TIM1PWM输出(高级定时器)
	TIM_SetCompare4(TIM5,0x00);
//	TIM_CtrlPWMOutputs(TIM5, ENABLE);
}

void PWM_Control(uint8_t ch,uint8_t percentage)
{

	uint16_t value;
	float cal;
	cal=(float )percentage/100*(float )PWM_Count+1;
	value=(uint16_t )cal;
	TIM_SetCompare4(TIM5,value);
}
