#ifndef __GPIO_H_
#define __GPIO_H_
#include "stm32f10x_gpio.h"


void gpio_int(void);
void led_control(char i);
void AD5663_LDAC(char i);
void W5500_RST_Control(char i);
uint8_t W5500_Add_Read(void);
uint8_t Data_15_Read(void);
uint8_t Data_14_Read(void);

#define BIT1(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_0, Bit_RESET);}
#define BIT2(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_1, Bit_RESET);}
#define BIT3(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_2, Bit_RESET);}
#define BIT4(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_3, Bit_RESET);}
#define BIT5(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_6, Bit_RESET);}
#define BIT6(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);}
#define BIT7(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_8, Bit_RESET);}
#define BIT8(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_9, Bit_RESET);}
#define BIT9(x)  {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_10, Bit_RESET);}
#define BIT10(x) {if(x==1)GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_SET);else GPIO_WriteBit(GPIOC, GPIO_Pin_11, Bit_RESET);}

#define BIT11(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_6, Bit_RESET);}
#define BIT12(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_7, Bit_RESET);}
#define BIT13(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_0, Bit_RESET);}
#define BIT14(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_1, Bit_RESET);}

#define BIT15(x) {if(x==1)GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_SET);else GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);}

#define BIT16(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_3, Bit_RESET);}
#define BIT17(x) {if(x==1)GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_SET);else GPIO_WriteBit(GPIOB, GPIO_Pin_4, Bit_RESET);}


#define RD_BIT1  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0)
#define RD_BIT2  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define RD_BIT3  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_2)
#define RD_BIT4  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_3)
#define RD_BIT5  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_6)
#define RD_BIT6  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_7)
#define RD_BIT7  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_8)
#define RD_BIT8  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_9)
#define RD_BIT9  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10)
#define RD_BIT10  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11)
#define RD_BIT11  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_6)
#define RD_BIT12  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_7)
#define RD_BIT13  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_0)
#define RD_BIT14  GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1)
#define RD_BIT15  GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0)
#define RD_BIT16  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)
#define RD_BIT17  GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4)

#endif

/***************************File End*******************************/
