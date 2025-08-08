#ifndef __SPI_Cmd_H
#define __SPI_Cmd_H
#include <stdint.h>

extern const uint8_t SPI_WRITE[];
extern const uint8_t SPI_READ[];
void SPI_Write_Cmd(uint8_t *buff);
void SPI_Read_Cmd(uint8_t *buff);

#endif
