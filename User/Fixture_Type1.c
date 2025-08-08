#include "Fixture_Type1.h"
#include "Timer.h"
#include "messageout.h"
#include "GPIO.h"


/*
Fixture Type1 :
  just push start key and send "START TEST"
*/
uint8_t Start_Flag=0;
uint32_t Fixture_Time=0;
uint8_t Fixture_Over_Flag=0;
enum Start_Key_Status
{
	Start_Idle=0,
	Start_Press,
	Start_Release,
} Fixture_Start_Status;

/*Lower API*/
uint8_t Fixture_Send(char *str)
{
	MessageSend(str);
	
	return 0;
}
uint8_t Fixture_Start_Read(void)
{
	uint8_t ret;
	ret=Data_14_Read();
	return ret;
}
/*Internal API*/
void Start_Idle_Scan(void)
{
	if (Fixture_Start_Read()==0)
	{
		Fixture_Time=gTimerBase+20;
		Fixture_Start_Status=Start_Press;
		Start_Flag=0;
		if (Fixture_Time>100000)
		{
			Fixture_Time=Fixture_Time-100000;
			Fixture_Over_Flag=1;
		}
	}
}
void Start_Press_Scan(void)
{
	if (Fixture_Over_Flag==0)
	{
		if (gTimerBase>Fixture_Time)
		{
			if (Fixture_Start_Read()==0)
			{
				if (Start_Flag==0)
				{
					Fixture_Send("START TEST\r\n");
					Start_Flag=1;
				}
			}
			else
			{
				Fixture_Start_Status=Start_Release;
				Fixture_Time=gTimerBase+1000;
				if (Fixture_Time>100000)
				{
					Fixture_Time=Fixture_Time-100000;
					Fixture_Over_Flag=1;
				}
			}
		}
	}
	else
	{
		if (gTimerBase<90000)
		{
			Fixture_Over_Flag=0;
		}
	}
}
void Start_Release_Scan(void)
{
	if (Fixture_Over_Flag==0)
	{
		if (Fixture_Time<gTimerBase)
		{
			if (Fixture_Start_Read()!=0)
			{
				Fixture_Start_Status=Start_Idle;
			}
		}
	}
	else
	{
		if (gTimerBase<90000)
		{
			Fixture_Over_Flag=0;
		}
	}
}
/*************User API**************/
void Fixture_Init(void)
{
	Fixture_Start_Status=Start_Idle;
}

void Star_Key_Scan(void)
{
	switch(Fixture_Start_Status)
	{
		case Start_Idle:
			Start_Idle_Scan();
			break;
		case Start_Press:
			Start_Press_Scan();
			break;
		case Start_Release:
			Start_Release_Scan();
			break;
	}
}
