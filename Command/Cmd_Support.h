#ifndef	 	__Cmd_Support_H
#define		__Cmd_Support_H

#include <string.h>
#include <stdlib.h>
#include <stdint.h>


unsigned char DECoffset(unsigned char num);
unsigned char StrToHEX(unsigned char str2,unsigned char str1);
unsigned char HTCoffset(unsigned char num);
int stringTOnum(int par,char array[]);

#endif
