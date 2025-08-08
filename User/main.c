#include "Interface_Cmd.h"
#include "Usart.h"
#include "Delay.h"
#include "GPIO.h"
#include "TestControl.h"
#include "RelayControl.h"
#include "Timer.h"
#include "CAT9555.h"
#include "SPI_Driver.h"
#include "AD9833.h"
#include "ad7175_DMM.h"
#include "ad7175_IMPD.h"
#include "w5500_interface.h"
#include "Fixture_Type1.h"
#include "AD5663.h"
#include "TMP117.h"


void NVIC_Configuration(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
}
int main(void)
{	
	NVIC_Configuration();
  delay_init();
	gpio_int();
	Uart1_Init(115200);
	Timer1_Init();
	I2C1_Init();
	I2C2_Init();
	Cat9555_Init_8IC();
	delay_ms(10);
	GPIO_SetBits(GPIOC, GPIO_Pin_8);	//12V softstart 		--- WEI --- 20230918 ----
	SPI1_Init();
	delay_ms(10);
	SPI2_Init();
	delay_ms(10);
	SPI3_Init();
	delay_ms(100);
	AD5663_DAC_Init();
//	Test_Init_8IC(0xff);
//	Relay_Init_8IC(0xff);
//	W5500_Init();
	Uart1_SendString("Hello World\r\n");
//	IMPD_AD7175_Init();
//	DMM_AD7175_Init();
	
//	Fixture_Init();
	while(1)
	{
//		do_tcp_server();
		if(gTimerBase%1000==0)
		{
			led_control(0);
		}
		if(gTimerBase%1000==500)
		{
			led_control(1);
		}
	}
}
/******************* Communication Control ********************/
//#define UART_COMMAND
//#define W5500_COMMAND

void Message_Send_Callback(unsigned char *str,unsigned char len)
{
//	Tcp_Send(str,strlen(str));
#ifdef UART_COMMAND
	Uart1_SendArray((unsigned char *)str,strlen((char *)str));
#endif
#ifdef W5500_COMMAND
	W5500_Sent((unsigned char *)str,strlen(str));
#endif
}
#ifdef UART_COMMAND

void Tim1_Callback(void)
{
	if (Uart1RxFlag==1)
	{
		CMD_Process(RX1buff,Uart1Len,&Uart1RxFlag);
	}
}
#endif
#ifdef W5500_COMMAND

void W5500_Command_Process_Callback(uint8 *buff,uint16_t len)
{
	uint8 flag=0;
	CMD_Process(buff,len,&flag);
}
#endif
