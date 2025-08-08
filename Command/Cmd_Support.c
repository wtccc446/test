#include "Cmd_Support.h"


unsigned char DECoffset(unsigned char num)
{
	unsigned char num1=num;

	if(('0'<=num1)&&(num1<='9'))
	{
		num1=num1-0x30;
	}
	else if(('A'<=num1)&&(num1<='F'))
	{
		num1=num1-0x41+10;
	}
	else if(('a'<=num1)&&(num1<='f'))
	{
		num1=num1-0x61+10;
	}

	return num1;
}
unsigned char StrToHEX(unsigned char str2,unsigned char str1)
{
	uint8_t num1,num2,num;
	num2=DECoffset(str2);
	num1=DECoffset(str1);
	num=num2*16+num1;
	return num;
}
unsigned char HTCoffset(unsigned char num)
{ 
	unsigned char num1=num;

	if(num1 <= 9)
	{
		num1 = num1 + 0x30;
	}
	else if(( 10 <= num1)&&(num1 <= 15))
	{
		num1=num1 - 10 + 'A';
	}

	return num1;
}
int stringTOnum(int par,char array[])
{
	char str;
	int int_num;
	int s_num;
	int x,y;

	int e_num;
	int return_num=0;
	for(x=0;x<par;x++)
	{
		if(array[x]>='0' && array[x]<='9')
		{
			e_num=1;
			str=array[x]-0x30;
			int_num=str;
			
			s_num=par-x-1;
			for(y=0;y<s_num;y++)
			{
				e_num=e_num*10;
			}
			return_num=return_num+int_num*e_num;			
		}
	}

	return return_num;
}




