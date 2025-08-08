#include "SHT85_Cmd.h"
#include "SHT85.h"
#include "messageout.h"

const uint8_t SHT85_READ[]="SHT85 READ\r\n";

void SHT85_Read_Cmd(void)
{
	float temp,humi;
//	uint32_t temp_d,humi_d;
	
	if(SHT85_Read(&temp,&humi)!=0)
	{
		MessageSend("SHT85 Work Error\r\n");
		return ;
	}
//	temp_d=(uint32_t )temp;
//	humi_d=(uint32_t )humi;
	
	MessageSend("Temp:%f\r\n",temp);
	MessageSend("Humi:%f\r\n",humi);
}

