#include "PWM_Cmd.h"
#include "messageout.h"
#include <string.h>
#include "PWM.h"

const uint8_t PWM_SET[]="PWM SET";

/*
PWM SET(1,21)
PWM SET(21)
*/

void PWM_SET_Cmd(uint8_t *buff)
{
	uint8_t num_str[5]={0};
	uint8_t percentage,i;
	uint8_t ch;
	if (buff[7]=='(' && buff[9]==',')
	{
		if (buff[8]=='1')
			ch=1;
		else if (buff=='2')
			ch=2;
		else
		{
			MessageSend("PWM Set Channel Error\r\n");
			return ;
		}
		for ( i = 0; i < 5; ++i)
		{
			if (i>3)
			{
				MessageSend("PWM Format Error\r\n");
				return ;
			}
			if (buff[10+i]==')')
			{
				break;
			}	
			else
			{
				num_str[i]=buff[10+i];
			}	
		}
		percentage=atoi(num_str);
		if (percentage<101)
		{
			PWM_Control(ch,percentage);
			MessageSend("PWM Set OK\r\n");
		}
	}
	else
		MessageSend("PWM SET Error Format\r\n");
}