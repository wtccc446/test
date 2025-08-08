#include "ad7175_TEMP.h"
#include "ad7175_2_regs.h"

struct ad717x_device Board_AD7175;

#define Board_AD7175_Reference 5000
/***************SPI Interface Start******************/

/****************SPI Interface End*****************/
void AD7175_Temp_Setup(void)
{
	ad717x_st_reg *ReadReg;
	AD717X_Setup(&Board_AD7175,AD7175_2_SLAVE_1_ID,ad7175_2_regs,27);
/**************** ADC MODE *****************/

	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_ADCMODE_REG);
	ReadReg->value=AD717X_ADCMODE_REG_REF_EN | AD717X_ADCMODE_SING_CYC | AD717X_ADCMODE_REG_MODE(0) | AD717X_ADCMODE_REG_CLKSEL(3);
	AD717X_WriteRegister(&Board_AD7175,AD717X_ADCMODE_REG);	
	
/**************Interface MODE******************/
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_IFMODE_REG);
	ReadReg->value=AD717X_IFMODE_REG_CONT_READ;
	AD717X_WriteRegister(&Board_AD7175,AD717X_IFMODE_REG);
	
/**************** CH 0 *****************/
	/********AD717X_Channel0_REG*********/ //
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP0_REG);//Setup 0;AIN0 +;AIN1 -
	ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
	AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP0_REG);
	/********AD717X_SETUPCON0_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON0_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON0_REG);
	/**********AD717X_FILTCON0_REG**********/	//Sinc1+5;15,625SPS;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON0_REG);
	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(10);//1000 SPS;
//	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILT(2) | AD717X_FILT_CONF_REG_ODR(13);//200SPS;
	AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON0_REG);

/**************** CH 1 *****************/
	/********AD717X_Channel1_REG*********/ //
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;REF- -
	ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(22);
	AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP1_REG);
	/********AD717X_SETUPCON1_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON1_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON1_REG);
	/**********AD717X_FILTCON1_REG**********/	//Sinc1+5;100SPS;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON1_REG);
	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(0x0e);	//100 SPS
	AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON1_REG);
	
/**************** CH 2 *****************/
	/********AD717X_Channel2_REG*********/ //
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN3 +;REF- -
	ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(3) | AD717X_CHMAP_REG_AINNEG(22);
	AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP2_REG);
	/********AD717X_SETUPCON2_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON2_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) |AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON2_REG);
	/**********AD717X_FILTCON2_REG**********/	//Sinc1+5;1000SPS;
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON2_REG);
	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(0x0a);//1000 SPS
	AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON2_REG);
	
	/**********AD7175 Bug!!!!!!!!!!**********/		
	AD717X_SPI_Continue_Read(&Board_AD7175,gADCBuff_int,200,100000);
	
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON0_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON0_REG);
	
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON1_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON1_REG);
	
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_SETUPCON2_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&Board_AD7175,AD717X_SETUPCON2_REG);
}

void AD7175_Channel_Select(uint8_t ch)
{
	ad717x_st_reg *ReadReg;

/**************** CH 0 *****************/
	if (ch==0)
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP0_REG);
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP0_REG);
	}
	else
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP0_REG);
		ReadReg->value= AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP0_REG);
	}
/**************** CH 1 *****************/
	if (ch==1)
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;REF- -
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP1_REG);	
	}
	else
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;REF- -
		ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP1_REG);
	}
/**************** CH 2 *****************/
	if (ch==2)
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN3 +;REF- -
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(3) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP2_REG);
	}
	else
	{
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN3 +;REF- -
		ReadReg->value= AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(3) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&Board_AD7175,AD717X_CHMAP2_REG);	
	}
	/**********AD717X_ADCMODE_REG**********/ 
//	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_ADCMODE_REG);
//	ReadReg->value=0x800C;
//	AD717X_WriteRegister(&Board_AD7175,AD717X_ADCMODE_REG);	

}
/*************** Internal API *****************/

uint8_t AD7175_Channel_Change_Speed(uint8_t ch,uint8_t speed_type)
{
	ad717x_st_reg *ReadReg;
	if(speed_type>0x14)
		return 1;
	if(ch==0)
	{
		/**********AD717X_FILTCON0_REG**********/
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON0_REG);
		ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON0_REG);
	}
	else if(ch==1)
	{
		/**********AD717X_FILTCON1_REG**********/
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON1_REG);
		ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON1_REG);
	}
	else if(ch==2)
	{
		/**********AD717X_FILTCON2_REG**********/
		ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_FILTCON2_REG);
		ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&Board_AD7175,AD717X_FILTCON2_REG);
	}
	else
		return 1;
	return 0;
}
void AD7175_Int2float(uint32_t *input,float *output,uint16_t len )
{
	uint32_t *p_in;
	float *p_out;
	float Ref;
	p_in=input;
	p_out=output;
	for (int i = 0; i < len; ++i)
	{
		if (*p_in<0x7fffff)
 		{
 			Ref=0-Board_AD7175_Reference;
 			*p_in=0x800000-*p_in;
 		}
 		else
 		{
 			Ref=Board_AD7175_Reference;
 			*p_in=*p_in-0x800000;
 		}
 		*p_out=*p_in;
 		*p_out=*p_out/8388607*Ref;
		p_in++;
		p_out++;
	}
}
/*************** USER API ***************/
void AD7175_Temp_Init(void)
{
	AD7175_Temp_Setup();
}
uint32_t AD7175_Temp_ReadReg(uint8_t Radd)
{
	uint32_t ret;
	ad717x_st_reg *ReadReg;
	AD717X_ReadRegister(&Board_AD7175,Radd);
	ReadReg=AD717X_GetReg(&Board_AD7175,Radd);
	ret=ReadReg->value;
	return ret;
}
uint32_t AD7175_Temp_ReadID(void)
{	
	uint32_t ad7175_id_value;
	ad717x_st_reg *ReadReg;
	AD717X_ReadRegister(&Board_AD7175,AD717X_ID_REG);
	ReadReg=AD717X_GetReg(&Board_AD7175,AD717X_ID_REG);
	ad7175_id_value=ReadReg->value;
	return ad7175_id_value;
}

uint8_t AD7175_Single_Read(uint8_t ch,float *vol_data,uint32_t timeout)
{
	uint32_t data,i;
	AD7175_Channel_Select(ch);
	for(i=0;i<1000;i++);
	if(AD717X_SPI_Continue_Read(&Board_AD7175,&data,1,timeout)!=0)
		return 1;
	AD7175_Int2float(&data,vol_data,1);
	return 0;
}
float *AD7175_Continue_Read(uint8_t ch,uint8_t len,uint32_t timeout)
{
	uint32_t i;
	AD7175_Channel_Select(ch);
	for(i=0;i<1000;i++);
	AD717X_SPI_Continue_Read(&Board_AD7175,gADCBuff_int,len,timeout);
	AD7175_Int2float(gADCBuff_int,gADCBuff_float,len);
	
	return gADCBuff_float;
}









