#include "SPI_Driver.h"




#ifdef SPI_1
/*					Lower API						*/
#define SPI1_Pin_Clock	RCC_APB2Periph_GPIOA

#define SPI1_SCK_Port		GPIOA
#define	SPI1_SCK_Pin		GPIO_Pin_5

#define SPI1_MISO_Port	GPIOA
#define	SPI1_MISO_Pin		GPIO_Pin_6

#define SPI1_MOSI_Port	GPIOA
#define	SPI1_MOSI_Pin		GPIO_Pin_7

#define SPI1_CS1_Port		GPIOA
#define	SPI1_CS1_Pin		GPIO_Pin_2

#define SPI1_CS2_Port		GPIOA
#define	SPI1_CS2_Pin		GPIO_Pin_3

#define SPI1_CS3_Port		GPIOA
#define	SPI1_CS3_Pin		GPIO_Pin_4

#define SPI1_CS_ADC_Port		GPIOA
#define	SPI1_CS_ADC_Pin			GPIO_Pin_8
uint8_t SPI1_MISO_Read(void)
{
	if(GPIO_ReadInputDataBit(SPI1_MISO_Port,SPI1_MISO_Pin)==Bit_RESET)
		return 0;
	else
		return 1;
}
void SPI1_CS_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS1_Pin;	
	GPIO_Init(SPI1_CS1_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS2_Pin;	
	GPIO_Init(SPI1_CS2_Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = SPI1_CS3_Pin;	
	GPIO_Init(SPI1_CS3_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_CS_ADC_Pin;	
	GPIO_Init(SPI1_CS_ADC_Port, &GPIO_InitStructure);
	
	GPIO_SetBits(SPI1_CS1_Port,SPI1_CS1_Pin);
	GPIO_SetBits(SPI1_CS2_Port,SPI1_CS2_Pin);
	GPIO_SetBits(SPI1_CS3_Port,SPI1_CS3_Pin);
	GPIO_SetBits(SPI1_CS_ADC_Port,SPI1_CS_ADC_Pin);
#endif
}
void SPI1_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI1_Pin_Clock, ENABLE);

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	GPIO_InitStructure.GPIO_Pin = SPI1_SCK_Pin;	
	GPIO_Init(SPI1_SCK_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_MOSI_Pin;	
	GPIO_Init(SPI1_MOSI_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI1_MISO_Pin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI1_MISO_Port, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure);
	SPI_Cmd(SPI1,ENABLE);
#endif
	SPI1_CS_Init();
}
void SPI1_CS_Ctr(uint8_t CS_CH,uint8_t type)
{
//	uint16_t delay=0;
#ifdef STDLib	
	if(CS_CH==1)		
	{
		if(type==0)
			GPIO_ResetBits(SPI1_CS1_Port,SPI1_CS1_Pin);
		else
			GPIO_SetBits(SPI1_CS1_Port,SPI1_CS1_Pin);
	}
	else if(CS_CH==2)		
	{
		if(type==0)
			GPIO_ResetBits(SPI1_CS2_Port,SPI1_CS2_Pin);
		else
			GPIO_SetBits(SPI1_CS2_Port,SPI1_CS2_Pin);			
	}
	else if(CS_CH==3)		
	{
		if(type==0)
			GPIO_ResetBits(SPI1_CS3_Port,SPI1_CS3_Pin);
		else
			GPIO_SetBits(SPI1_CS3_Port,SPI1_CS3_Pin);
	}
	else if(CS_CH==4)		
	{
		if(type==0)
			GPIO_ResetBits(SPI1_CS_ADC_Port,SPI1_CS_ADC_Pin);
		else
			GPIO_SetBits(SPI1_CS_ADC_Port,SPI1_CS_ADC_Pin);
	}
#endif
}
uint8_t SPI1_Write(uint8_t ch,uint8_t *TxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI1_CS_Ctr(ch,0);
	pd=TxData;
	for( i=0;i<len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI1,*pd);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI1_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI1_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI1_Read(int8_t ch,uint8_t *RxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI1_CS_Ctr(ch,0);
	pd=RxData;
	SPI_I2S_ReceiveData(SPI1);
	for( i=0;i<len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI1,0xff);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*pd=SPI_I2S_ReceiveData(SPI1);
		pd++;
	}
	SPI1_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI1_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI1_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;
	
	timeout=2000;
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI1_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI_I2S_ReceiveData(SPI1);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI1,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI1);
		Rpd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI1_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI1_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI1_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	Tpd=TxData;
	Rpd=RxData;
	SPI_I2S_ReceiveData(SPI1);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI1,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}

		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI1);
		Rpd++;
	}
	return 0;
}
uint8_t SPI1_Data_In_Check(void)
{
	return SPI1_MISO_Read();
}
uint8_t SPI1_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI1_CS_Ctr(ch,0);
	pd=reg;
	for(i=0;i<reg_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI1,*pd);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	pd=TxData;
	for(i=0;i<Tx_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI1,*pd);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI1_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI1_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI1_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI1_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;
	for( i=0;i<Tx_Len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI1,*Tpd);
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		Tpd++;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI1);
	}
	for( i=0;i<Rx_Len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI1,0x00);	
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI1_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI1);
		Rpd++;
	}
	SPI1_CS_Ctr(ch,1);
	return 0;
}
#endif






#ifdef SPI_2
/*					Lower API						*/
#define SPI2_Pin_Clock	RCC_APB2Periph_GPIOB

#define SPI2_SCK_Port		GPIOB
#define	SPI2_SCK_Pin		GPIO_Pin_13

#define SPI2_MISO_Port	GPIOB
#define	SPI2_MISO_Pin		GPIO_Pin_14

#define SPI2_MOSI_Port	GPIOB
#define	SPI2_MOSI_Pin		GPIO_Pin_15

#define SPI2_CS1_Port		GPIOB
#define	SPI2_CS1_Pin		GPIO_Pin_12

//#define SPI2_CS2_Port		GPIOB
//#define	SPI2_CS2_Pin		GPIO_Pin_12

// #define SPI2_CS3_Port	
// #define	SPI2_CS3_Pin	
uint8_t SPI2_MISO_Read(void)
{
	if(GPIO_ReadInputDataBit(SPI2_MISO_Port,SPI2_MISO_Pin)==Bit_RESET)
		return 0;
	else
		return 1;
}
void SPI2_CS_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPI2_CS1_Pin;	
	GPIO_Init(SPI2_CS1_Port, &GPIO_InitStructure);

//	GPIO_InitStructure.GPIO_Pin = SPI2_CS2_Pin;	
//	GPIO_Init(SPI2_CS2_Port, &GPIO_InitStructure);

	// GPIO_InitStructure.GPIO_Pin = SPI2_CS3_Pin;	
	// GPIO_Init(SPI2_CS3_Port, &GPIO_InitStructure);
	
	GPIO_SetBits(SPI2_CS1_Port,SPI2_CS1_Pin);
	// GPIO_SetBits(SPI2_CS2_Port,SPI2_CS2_Pin);
	// GPIO_SetBits(SPI2_CS3_Port,SPI2_CS3_Pin);
#endif
}
void SPI2_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI2_Pin_Clock, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	GPIO_InitStructure.GPIO_Pin = SPI2_SCK_Pin;	
	GPIO_Init(SPI2_SCK_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_MOSI_Pin;	
	GPIO_Init(SPI2_MOSI_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI2_MISO_Pin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI2_MISO_Port, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI2, &SPI_InitStructure);
	SPI_Cmd(SPI2,ENABLE);
#endif
	SPI2_CS_Init();
}
void SPI2_CS_Ctr(uint8_t CS_CH,uint8_t type)
{
//	uint16_t delay=0;
#ifdef STDLib	
	if(CS_CH==1)		
	{
		if(type==0)
			GPIO_ResetBits(SPI2_CS1_Port,SPI2_CS1_Pin);
		else
			GPIO_SetBits(SPI2_CS1_Port,SPI2_CS1_Pin);
	}
//	else if(CS_CH==2)		
//	{
//		if(type==0)
//			GPIO_ResetBits(SPI2_CS2_Port,SPI2_CS2_Pin);
//		else
//			GPIO_SetBits(SPI2_CS2_Port,SPI2_CS2_Pin);			
//	}
	// else if(CS_CH==3)		//DDS
	// {
	// 	if(type==0)
	// 		GPIO_ResetBits(SPI2_CS3_Port,SPI2_CS3_Pin);
	// 	else
	// 		GPIO_SetBits(SPI2_CS3_Port,SPI2_CS3_Pin);
	// }
#endif
}
uint8_t SPI2_Write(uint8_t ch,uint8_t *TxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI2_CS_Ctr(ch,0);
	pd=TxData;
	for( i=0;i<len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI2,*pd);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI2_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI2_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI2_Read(int8_t ch,uint8_t *RxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI2_CS_Ctr(ch,0);
	pd=RxData;
	SPI_I2S_ReceiveData(SPI2);
	for( i=0;i<len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI2,0xff);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*pd=SPI_I2S_ReceiveData(SPI2);
		pd++;
	}
	SPI2_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI2_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI2_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;
	
	timeout=2000;
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI2_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI_I2S_ReceiveData(SPI2);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI2,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		
		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI2);
		Rpd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI2_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI2_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI2_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	Tpd=TxData;
	Rpd=RxData;
	SPI_I2S_ReceiveData(SPI2);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI2,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI2);
		Rpd++;
	}
	return 0;
}
uint8_t SPI2_Data_In_Check(void)
{
	return SPI2_MISO_Read();
}
uint8_t SPI2_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI2_CS_Ctr(ch,0);
	pd=reg;
	for(i=0;i<reg_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI2,*pd);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	pd=TxData;
	for(i=0;i<Tx_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI2,*pd);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI2_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI2_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI2_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI2_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;
	for( i=0;i<Tx_Len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI2,*Tpd);
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		Tpd++;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI2);
	}


	for( i=0;i<Rx_Len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI2,0x00);	
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI2_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI2);
		Rpd++;
	}
	SPI2_CS_Ctr(ch,1);
	return 0;
}
#endif


#ifdef SPI_3
/*					Lower API						*/
#define SPI3_Pin_Clock	RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOB

#define SPI3_SCK_Port		GPIOB
#define	SPI3_SCK_Pin		GPIO_Pin_3

#define SPI3_MISO_Port	GPIOB
#define	SPI3_MISO_Pin		GPIO_Pin_4

#define SPI3_MOSI_Port	GPIOB
#define	SPI3_MOSI_Pin		GPIO_Pin_5

#define SPI3_CS1_Port		GPIOB
#define	SPI3_CS1_Pin		GPIO_Pin_0

#define SPI3_CS2_Port		GPIOB
#define	SPI3_CS2_Pin		GPIO_Pin_1

#define SPI3_CS3_Port		GPIOA
#define	SPI3_CS3_Pin		GPIO_Pin_0

#define SPI3_CS_DAC_Port		GPIOA
#define	SPI3_CS_DAC_Pin			GPIO_Pin_15

uint8_t SPI3_MISO_Read(void)
{
	if(GPIO_ReadInputDataBit(SPI3_MISO_Port,SPI3_MISO_Pin)==Bit_RESET)
		return 0;
	else
		return 1;
}
void SPI3_CS_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_InitStructure.GPIO_Pin = SPI3_CS1_Pin;	
	GPIO_Init(SPI3_CS1_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_CS2_Pin;	
	GPIO_Init(SPI3_CS2_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_CS3_Pin;	
	GPIO_Init(SPI3_CS3_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_CS_DAC_Pin;	
	GPIO_Init(SPI3_CS_DAC_Port, &GPIO_InitStructure);
	
	GPIO_SetBits(SPI3_CS1_Port,SPI3_CS1_Pin);
	GPIO_SetBits(SPI3_CS2_Port,SPI3_CS2_Pin);
	GPIO_SetBits(SPI3_CS3_Port,SPI3_CS3_Pin);
	GPIO_SetBits(SPI3_CS_DAC_Port,SPI3_CS_DAC_Pin);
#endif
}
void SPI3_Init(void)
{
#ifdef STDLib
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(SPI3_Pin_Clock, ENABLE);

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	

	GPIO_InitStructure.GPIO_Pin = SPI3_SCK_Pin;	
	GPIO_Init(SPI3_SCK_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_MOSI_Pin;	
	GPIO_Init(SPI3_MOSI_Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = SPI3_MISO_Pin;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(SPI3_MISO_Port, &GPIO_InitStructure);
	
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI3, &SPI_InitStructure);
	SPI_Cmd(SPI3,ENABLE);
#endif
	SPI3_CS_Init();
}
void SPI3_CS_Ctr(uint8_t CS_CH,uint8_t type)
{
//	uint16_t delay=0;
#ifdef STDLib	
	if(CS_CH==1)		
	{
		if(type==0)
			GPIO_ResetBits(SPI3_CS1_Port,SPI3_CS1_Pin);
		else
			GPIO_SetBits(SPI3_CS1_Port,SPI3_CS1_Pin);
	}
	else if(CS_CH==2)		
	{
		if(type==0)
			GPIO_ResetBits(SPI3_CS2_Port,SPI3_CS2_Pin);
		else
			GPIO_SetBits(SPI3_CS2_Port,SPI3_CS2_Pin);			
	}
	else if(CS_CH==3)		
	{
		if(type==0)
			GPIO_ResetBits(SPI3_CS3_Port,SPI3_CS3_Pin);
		else
			GPIO_SetBits(SPI3_CS3_Port,SPI3_CS3_Pin);
	}
	else if(CS_CH==4)		
	{
		if(type==0)
			GPIO_ResetBits(SPI3_CS_DAC_Port,SPI3_CS_DAC_Pin);
		else
			GPIO_SetBits(SPI3_CS_DAC_Port,SPI3_CS_DAC_Pin);
	}
#endif
}
uint8_t SPI3_Write(uint8_t ch,uint8_t *TxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI3_CS_Ctr(ch,0);
	pd=TxData;
	
	for( i=0;i<len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI3,*pd);
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI3_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI3_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI3_Read(int8_t ch,uint8_t *RxData,uint8_t len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI3_CS_Ctr(ch,0);
	pd=RxData;
	SPI_I2S_ReceiveData(SPI3);
	for( i=0;i<len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI3,0xff);
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*pd=SPI_I2S_ReceiveData(SPI3);
		pd++;
	}
	SPI3_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI3_WriteRead(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI3_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;

	timeout=2000;
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI3_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI_I2S_ReceiveData(SPI3);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI3,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		
		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI3);
		Rpd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI3_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI3_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI3_WriteRead_NOCS(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	Tpd=TxData;
	Rpd=RxData;
	SPI_I2S_ReceiveData(SPI3);
	for(i=0;i<len;i++)
	{
		SPI_I2S_SendData(SPI3,*Tpd);
		Tpd++;
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI3);
		Rpd++;
	}
	return 0;
}
uint8_t SPI3_Data_In_Check(void)
{
	return SPI3_MISO_Read();
}
uint8_t SPI3_Write_Reg(uint8_t ch,uint8_t *reg,uint8_t *TxData,uint8_t reg_len,uint8_t Tx_len)
{
	uint8_t *pd;
	uint16_t timeout;
	uint16_t i;
	SPI3_CS_Ctr(ch,0);
	pd=reg;
	for(i=0;i<reg_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI3,*pd);
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	pd=TxData;
	for(i=0;i<Tx_len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI3,*pd);
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		pd++;
	}
	timeout=2000;	
	while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_BSY)!=RESET)
	{
		timeout--;
		if(timeout==0)
		{
			SPI3_CS_Ctr(ch,1);
			return 1;				
		}
	}
	SPI3_CS_Ctr(ch,1);
	return 0;
}
uint8_t SPI3_WriteRead_T2R(int8_t ch,uint8_t *TxData,uint8_t *RxData,uint8_t Tx_Len,uint8_t Rx_Len)
{
	
	uint8_t *Tpd,*Rpd;
	uint16_t timeout;
	uint16_t i;
	SPI3_CS_Ctr(ch,0);
	Tpd=TxData;
	Rpd=RxData;
	for( i=0;i<Tx_Len;i++)
	{
		timeout=2000;	
		SPI_I2S_SendData(SPI3,*Tpd);
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		Tpd++;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI3);
	}
	for( i=0;i<Rx_Len;i++)
	{
		timeout=2000;
		SPI_I2S_SendData(SPI3,0x00);	
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_TXE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}		
		timeout=2000;
		while(SPI_I2S_GetFlagStatus(SPI3,SPI_I2S_FLAG_RXNE)==RESET)
		{
			timeout--;
			if(timeout==0)
			{
				SPI3_CS_Ctr(ch,1);
				return 1;				
			}
		}
		*Rpd=SPI_I2S_ReceiveData(SPI3);
		Rpd++;
	}
	SPI3_CS_Ctr(ch,1);
	return 0;
}
#endif



