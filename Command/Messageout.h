#ifndef __Messageout_h
#define __Messageout_h


void MessageSend(const char *format,...);
__weak void Message_Send_Callback(unsigned char *str,unsigned char len);


#endif

