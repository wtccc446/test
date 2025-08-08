#ifndef	 	__RelayIo_Cmd_H
#define		__RelayIo_Cmd_H
#include <stdint.h>

extern const uint8_t RELAY_CONTROL[];
extern const uint8_t RELAY_ADC_CONTROL[];
extern const uint8_t RELAY_SHORT_BUS[];
extern const uint8_t RELAY_ADC_SHORT_BUS[];
extern const uint8_t RELAY_SHORT_ONE_BUS_EXCEPT[];
extern const uint8_t RELAY_ADC_SHORT_ONE_BUS_EXCEPT[];
extern const uint8_t RELAY_ADC_SHORT_ONE_AND_RELAY[];
extern const uint8_t RELAY_RESET[];

void RelayCtr_Cmd(uint8_t *buff);
void Relay_ADC_Ctr_Cmd(uint8_t *buff);
void RelayShort_Cmd(uint8_t *buff);
void Relay_ADC_Short_Cmd(uint8_t *buff);
void RelayShortOneBus_Cmd(uint8_t *buff);
void Relay_ADC_ShortOneBus_Cmd(uint8_t *buff);
void Relay_ADC_ShortOneAndRelay_Cmd(uint8_t *buff);

void RelayReset_Cmd(void);


#endif 
