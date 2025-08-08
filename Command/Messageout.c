//
//  main.c
//  Practice
//
//  Created by Xin.Zhang on 9/8/18.
//  Copyright © 2018? Xin.Zhang. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include "stdarg.h"
#include "Messageout.h"
#include "Usart.h"	  
#include "w5500_interface.h"

__weak void Message_Send_Callback(unsigned char *str,unsigned char len)
{
	
}
void MessageApi(char *str)
{
	Message_Send_Callback((unsigned char *)str,strlen(str));
}
void MessageSend(const char *format,...)
{
    va_list ap;
    char sendstr[200]="";
    char single[2]={0x00,0x00};
    char temp_str[40]="";
    char val_char;
    char *val_str;
    int val_int;
    float val_float;
    va_start(ap, format);
    while (*format)
    {
        if (*format!='%')
        {
            single[0]=*format;
						single[1]=0x00;
            strcat(sendstr,single);
            format++;
        }
        else
        {
            format++;
            switch (*format)
            {
                case 'c':
                    val_char=va_arg(ap, int);
                    sprintf(temp_str, "%c",val_char);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,20);
                    format++;
                    break;
                case 'd':
                    val_int=va_arg(ap, int);
                    sprintf(temp_str, "%d",val_int);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,sizeof(temp_str));
                    format++;
                    break;
                case 's':
                    val_str=va_arg(ap, char *);
                    sprintf(temp_str, "%s",val_str);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,sizeof(temp_str));
                    format++;
                    break;
                case 'f':
                    val_float=va_arg(ap, double);
                    sprintf(temp_str, "%f",val_float);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,sizeof(temp_str));
                    format++;
                    break;
                case 'x':
                    val_int=va_arg(ap, int);
                    sprintf(temp_str, "%x",val_int);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,sizeof(temp_str));
                    format++;
                    break;
                case 'X':
                    val_int=va_arg(ap, int);
                    sprintf(temp_str, "%X",val_int);
                    strcat(sendstr,temp_str);
                    memset(temp_str,0x00,sizeof(temp_str));
                    format++;
                    break;
                default:
                    format++;
                    break;
            }
        }
    }
    MessageApi(sendstr);
    memset(sendstr,0x00,strlen(sendstr));
    va_end(ap);
}

