#ifndef	 	__ADC_Cmd_H
#define		__ADC_Cmd_H
#include <stdint.h>

extern const uint8_t ADC_READ[];
extern const uint8_t ADC_MULT_READ[];

void ADC_Read_Cmd(void);
void ADC_Mult_Read_Cmd(uint8_t *buff);


#endif 
