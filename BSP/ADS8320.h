#ifndef	 	__ADS8320_H
#define		__ADS8320_H
#include <stdint.h>

/************Error Board A14*****************/
void ADS8320_Config(void);


float ADS8320ReadVolt(void);
float ADS8320AverageRead(uint8_t num);
float ADS8320MiddleRead(uint8_t num);
uint8_t ADS8320WAVERead(uint16_t num,float *volt);



#endif 
