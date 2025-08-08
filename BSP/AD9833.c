#include "AD9833.h"
#include "SPI_Driver.h"	

/**********************************
Read Me:
	AD5663 is fall edge Driver.
	SPI MODE3,Data right shift 1bit!!!
	SPI MODE0 is OK
***********************************/

/**************Hard Lever API************/
void AD9833_SpiWrite(uint8_t *data,uint8_t len)
{
	SPI3_Write(2,data,len);
}
unsigned char AD9833_Init(void)
{
    // SPI_Init(0, 1000000, 1, 1);
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
	
    return (1);
}

/***************************************************************************//**
 * @brief Sets the Reset bit of the AD9833.
 *
 * @return None.
*******************************************************************************/
void AD9833_Reset(void)
{
    AD9833_SetRegisterValue(AD9833_REG_CMD | AD9833_RESET);
}

/***************************************************************************//**
 * @brief Clears the Reset bit of the AD9833.
 *
 * @return None.
*******************************************************************************/
void AD9833_ClearReset(void)
{
	AD9833_SetRegisterValue(AD9833_REG_CMD);
}
/***************************************************************************//**
 * @brief Writes the value to a register.
 *
 * @param -  regValue - The value to write to the register.
 *
 * @return  None.    
*******************************************************************************/
void AD9833_SetRegisterValue(unsigned short regValue)
{
	unsigned char data[5] = {0x03, 0x00, 0x00};	
	
	data[0] = (unsigned char)((regValue & 0xFF00) >> 8);
	data[1] = (unsigned char)((regValue & 0x00FF) >> 0);
	
	// ADI_CS_LOW;	    
	// SPI_Write(data,2);
	// ADI_CS_HIGH;
	AD9833_SpiWrite(data,2);
	
}

/***************************************************************************//**
 * @brief Writes to the frequency registers.
 *
 * @param -  reg - Frequence register to be written to.
 * @param -  val - The value to be written.
 *
 * @return  None.    
*******************************************************************************/
void AD9833_SetFrequency(unsigned short reg, unsigned long val)
{
	unsigned short freqHi = reg;
	unsigned short freqLo = reg;
	
	freqHi |= (val & 0xFFFC000) >> 14 ;
	freqLo |= (val & 0x3FFF);
	AD9833_SetRegisterValue(AD9833_B28);
	AD9833_SetRegisterValue(freqLo);
	AD9833_SetRegisterValue(freqHi);
}
/***************************************************************************//**
 * @brief Writes to the phase registers.
 *
 * @param -  reg - Phase register to be written to.
 * @param -  val - The value to be written.
 *
 * @return  None.    
*******************************************************************************/
void AD9833_SetPhase(unsigned short reg, unsigned short val)
{
	unsigned short phase = reg;
	phase |= val;
	AD9833_SetRegisterValue(phase);
}
/***************************************************************************//**
 * @brief Selects the Frequency,Phase and Waveform type.
 *
 * @param -  freq  - Frequency register used.
 * @param -  phase - Phase register used.
 * @param -  type  - Type of waveform to be output.
 *
 * @return  None.    
*******************************************************************************/
void AD9833_Setup(unsigned short freq,
				  unsigned short phase,
			 	  unsigned short type)
{
	unsigned short val = 0;
	
	val = freq | phase | type;
	AD9833_SetRegisterValue(val);
}
/***************************************************************************//**
 * @brief Sets the type of waveform to be output.
 *
 * @param -  type - type of waveform to be output.
 *
 * @return  None.    
*******************************************************************************/
void AD9833_SetWave(unsigned short type)
{
	AD9833_SetRegisterValue(type);
}


void AD9833_Cofig(unsigned long val,unsigned short type)
{
	AD9833_SetFrequency(AD9833_REG_FREQ0,val);
	AD9833_SetPhase(AD9833_REG_PHASE0,0);	//0xc0 0x00
	switch(type)
	{
		case 0:
			AD9833_Setup(AD9833_FSEL0,AD9833_PSEL0,AD9833_OUT_SINUS);
			break;
		case 1:
			AD9833_Setup(AD9833_FSEL0,AD9833_PSEL0,AD9833_OUT_TRIANGLE);
			break;
		case 2:
			AD9833_Setup(AD9833_FSEL0,AD9833_PSEL0,AD9833_OUT_MSB);
			break;
		case 3:
			AD9833_Setup(AD9833_FSEL0,AD9833_PSEL0,AD9833_OUT_MSB2);
			break;
		default :
			break;
	}	
//	printf("frq:%X\r\n",val);
//	printf("type:%d\r\n",type);
}
