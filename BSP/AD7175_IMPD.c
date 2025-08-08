#include "ad7175_IMPD.h"
#include "AD717x.h"

struct ad717x_device IMPD_AD7175;

#define IMPD_AD7175_Reference 5000
/***************Define Start******************/
ad717x_st_reg IMPD_ad7175_2_regs[] =
{
	{ AD717X_STATUS_REG, 0x00, 1 },
	{AD717X_ADCMODE_REG,0x0000,2 },
	{
		AD717X_IFMODE_REG, 
		0x0000 ,
		2
	},
	{ AD717X_REGCHECK_REG, 0x0000, 3},
	{ AD717X_DATA_REG, 0x0000, 3 },
	
	{ AD717X_GPIOCON_REG, 0x0000, 2 },
	{ AD717X_ID_REG, 0x0000, 2 },
	{
		AD717X_CHMAP0_REG, 
		AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_AINNEG(2),
		2
	},
	{	AD717X_CHMAP1_REG, 0x0000, 2 },
	{ AD717X_CHMAP2_REG, 0x0000, 2 },
	
	{ AD717X_CHMAP3_REG, 0x0000, 2 },
	{ AD717X_SETUPCON0_REG, 0x1000, 2 },
	{ AD717X_SETUPCON1_REG,	0x0000,	2 },
	{ AD717X_SETUPCON2_REG, 0x0000,	2 },
	{ AD717X_SETUPCON3_REG, 0x0000, 2 },
	
	{
		AD717X_FILTCON0_REG,
		AD717X_FILT_CONF_REG_ENHFILT(2) | AD717X_FILT_CONF_REG_ODR(10),
		2
	},
//	{	AD717X_FILTCON0_REG,0x80FA,2	},
	{
		AD717X_FILTCON1_REG,
		AD717X_FILT_CONF_REG_ENHFILT(2),
		2
	},
	{
		AD717X_FILTCON2_REG,
		AD717X_FILT_CONF_REG_ENHFILT(2),
		2
	},
	{
		AD717X_FILTCON3_REG,
		AD717X_FILT_CONF_REG_ENHFILT(2),
		2
	},
	{ AD717X_OFFSET0_REG, 0, 3 },
	
	{ AD717X_OFFSET1_REG, 0, 3 },
	{ AD717X_OFFSET2_REG, 0, 3 },
	{ AD717X_OFFSET3_REG, 0, 3 },
	{ AD717X_GAIN0_REG, 0, 3 },
	{ AD717X_GAIN1_REG, 0, 3 },
	
	{ AD717X_GAIN2_REG, 0, 3 },
	{ AD717X_GAIN3_REG, 0, 3 },
};
#define AD7175_2_IMPD_ID    0x02
/****************Define End*****************/
void IMPD_AD7175_Config(void)
{
	ad717x_st_reg *ReadReg;
/**************** ADC MODE *****************/
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_ADCMODE_REG);
	ReadReg->value=AD717X_ADCMODE_REG_REF_EN | AD717X_ADCMODE_SING_CYC | AD717X_ADCMODE_REG_MODE(0) | AD717X_ADCMODE_REG_CLKSEL(3);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_ADCMODE_REG);	
	
/**************Interface MODE******************/
//	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_IFMODE_REG);
//	ReadReg->value=AD717X_IFMODE_REG_CONT_READ;
//	AD717X_WriteRegister(&IMPD_AD7175,AD717X_IFMODE_REG);
	
/**************** CH 0 *****************/
	/********AD717X_Channel0_REG*********/ //
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP0_REG);//Setup 0;AIN0 +;AIN1 -
	ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP0_REG);
	/********AD717X_SETUPCON0_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_SETUPCON0_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_SETUPCON0_REG);
	/**********AD717X_FILTCON0_REG**********/	//Sinc1+5;15;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON0_REG);
//	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(0x0a);//1000 SPS;
	ReadReg->value=  AD717X_FILT_CONF_REG_ODR(0x0a);//1000 SPS;
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON0_REG);

/**************** CH 1 *****************/
	/********AD717X_Channel1_REG*********/ //
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;AIN3 -
	ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(3);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP1_REG);
	/********AD717X_SETUPCON1_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_SETUPCON1_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) | AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_SETUPCON1_REG);
	/**********AD717X_FILTCON1_REG**********/	//Sinc1+5;100SPS;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON1_REG);
//	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(0x0a);	//1000 SPS
	ReadReg->value=AD717X_FILT_CONF_REG_ODR(0x0a);	//1000 SPS
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON1_REG);
	
/**************** CH 2 *****************/
	/********AD717X_Channel2_REG*********/ //
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN4 +;REF- -
	ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(4) | AD717X_CHMAP_REG_AINNEG(22);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP2_REG);
	/********AD717X_SETUPCON2_REG*********/ //disable buff;ext Vref;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_SETUPCON2_REG);
	ReadReg->value=AD717X_SETUP_CONF_REG_BI_UNIPOLAR | AD717X_SETUP_CONF_REG_AIN_BUF(3) |AD717X_SETUP_CONF_REG_REF_SEL(0);
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_SETUPCON2_REG);
	/**********AD717X_FILTCON2_REG**********/	//Sinc1+5;1000SPS;
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON2_REG);
//	ReadReg->value=AD717X_FILT_CONF_REG_ENHFILTEN | AD717X_FILT_CONF_REG_ENHFILT(5) | AD717X_FILT_CONF_REG_ODR(0x0a);//1000 SPS
	ReadReg->value=AD717X_FILT_CONF_REG_ODR(0x0a);//1000 SPS
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON2_REG);
}
void IMPD_AD7175_Setup(void)
{
//	ad717x_st_reg *ReadReg;
	
	AD717X_Setup(&IMPD_AD7175,AD7175_2_IMPD_ID,IMPD_ad7175_2_regs,27);
	IMPD_AD7175_Config();
/**********AD7175 Bug!!!!!!!!!!**********/		
	AD717X_SPI_Continue_Read(&IMPD_AD7175,gADCBuff_int,200,100000);
	IMPD_AD7175_Config();
}

void IMPD_AD7175_Channel_Select(uint8_t ch)
{
	ad717x_st_reg *ReadReg;

/**************** CH 0 *****************/
	if (ch==0)
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP0_REG);//AIN0 +;AIN1 -
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP0_REG);
	}
	else
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP0_REG);//AIN0 +;AIN1 -
		ReadReg->value= AD717X_CHMAP_REG_SETUP_SEL(0) | AD717X_CHMAP_REG_AINPOS(0) | AD717X_CHMAP_REG_AINNEG(1);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP0_REG);
	}
/**************** CH 1 *****************/
	if (ch==1)
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;AIN3 -
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(3);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP1_REG);	
	}
	else
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP1_REG);//Setup 1;AIN2 +;AIN3 -
		ReadReg->value=AD717X_CHMAP_REG_SETUP_SEL(1) | AD717X_CHMAP_REG_AINPOS(2) | AD717X_CHMAP_REG_AINNEG(3);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP1_REG);
	}
/**************** CH 2 *****************/
	if (ch==2)
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN4 +;REF- -
		ReadReg->value=AD717X_CHMAP_REG_CH_EN | AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(4) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP2_REG);
	}
	else
	{
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_CHMAP2_REG);//Setup 2;AIN4 +;REF- -
		ReadReg->value= AD717X_CHMAP_REG_SETUP_SEL(2) | AD717X_CHMAP_REG_AINPOS(4) | AD717X_CHMAP_REG_AINNEG(22);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_CHMAP2_REG);	
	}
	/**********AD717X_ADCMODE_REG**********/ 
//	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_ADCMODE_REG);
//	ReadReg->value=0x800C;
//	AD717X_WriteRegister(&IMPD_AD7175,AD717X_ADCMODE_REG);	

}
/*************** Internal API *****************/
uint8_t IMPD_AD7175_IFMODE(uint8_t type)
{
	ad717x_st_reg *ReadReg;
	/**************Interface MODE******************/
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_IFMODE_REG);
	if(type==0)
		ReadReg->value=0x00;
	else
		ReadReg->value=AD717X_IFMODE_REG_CONT_READ;
	AD717X_WriteRegister(&IMPD_AD7175,AD717X_IFMODE_REG);
	
	return 0;
}
uint8_t IMPD_AD7175_Channel_Change_Speed(uint8_t ch,uint8_t speed_type)
{
	ad717x_st_reg *ReadReg;
	if(speed_type>0x14)
		return 1;
	if(ch==0)
	{
		/**********AD717X_FILTCON0_REG**********/
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON0_REG);
		ReadReg->value= AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON0_REG);
	}
	else if(ch==1)
	{
		/**********AD717X_FILTCON1_REG**********/
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON1_REG);
		ReadReg->value=AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON1_REG);
	}
	else if(ch==2)
	{
		/**********AD717X_FILTCON2_REG**********/
		ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_FILTCON2_REG);
		ReadReg->value=AD717X_FILT_CONF_REG_ODR(speed_type);
		AD717X_WriteRegister(&IMPD_AD7175,AD717X_FILTCON2_REG);
	}
	else
		return 1;
	return 0;
}
void IMPD_AD7175_Int2float(uint32_t *input,float *output,uint16_t len )
{
	uint32_t *p_in;
	float *p_out;
	float Ref;
	p_in=input;
	p_out=output;
	for (int i = 0; i < len; ++i)
	{
		if (*p_in<0x800000)
 		{
 			Ref=0-IMPD_AD7175_Reference;
 			*p_in=0x800000-*p_in;
 		}
 		else
 		{
 			Ref=IMPD_AD7175_Reference;
 			*p_in=*p_in-0x800000;
 		}
 		*p_out=*p_in;
 		*p_out=*p_out/8388607*Ref;
		p_in++;
		p_out++;
	}
}
/*************** USER API ***************/
void IMPD_AD7175_Init(void)
{
	IMPD_AD7175_Setup();
}
uint32_t IMPD_AD7175_ReadReg(uint8_t Radd)
{
	uint32_t ret;
	ad717x_st_reg *ReadReg;
	AD717X_ReadRegister(&IMPD_AD7175,Radd);
	ReadReg=AD717X_GetReg(&IMPD_AD7175,Radd);
	ret=ReadReg->value;
	return ret;
}
uint32_t IMPD_AD7175_ReadID(void)
{	
	uint32_t ad7175_id_value;
	ad717x_st_reg *ReadReg;
	AD717X_ReadRegister(&IMPD_AD7175,AD717X_ID_REG);
	ReadReg=AD717X_GetReg(&IMPD_AD7175,AD717X_ID_REG);
	ad7175_id_value=ReadReg->value;
	return ad7175_id_value;
}

uint8_t IMPD_AD7175_Speed_Set(uint8_t ch,uint8_t speed_type)
{
	uint8_t ret;
	ret=IMPD_AD7175_Channel_Change_Speed(ch,speed_type);
	return ret;
}
uint8_t IMPD_AD7175_Single_Read(uint8_t ch,float *vol_data,uint32_t timeout)
{
	uint32_t data,i;
	IMPD_AD7175_Channel_Select(ch);
	for(i=0;i<1000;i++);
	if(AD717X_SPI_Continue_Read(&IMPD_AD7175,&data,1,timeout)!=0)
		return 1;
	IMPD_AD7175_Int2float(&data,vol_data,1);
	return 0;
}
float *IMPD_AD7175_Continue_Read(uint8_t ch,uint8_t len,uint32_t timeout)
{
	uint32_t i;
	uint32_t data;
//	float err_ret=1;
//	IMPD_AD7175_IFMODE(1);
	IMPD_AD7175_Channel_Select(ch);
	for(i=0;i<1000;i++);
	AD717X_SPI_Continue_Read(&IMPD_AD7175,&data,1,timeout);
	AD717X_SPI_Continue_Read(&IMPD_AD7175,gADCBuff_int,len,timeout);
	IMPD_AD7175_Int2float(gADCBuff_int,gADCBuff_float,len);
//	IMPD_AD7175_IFMODE(0);	
	return gADCBuff_float;
}









