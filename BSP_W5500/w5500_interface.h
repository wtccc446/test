#ifndef _W5500_INTERFACE_H_
#define _W5500_INTERFACE_H_

#include "stm32f10x.h"
#include "stdio.h"
#include "types.h"
//#define __GNUC__

typedef  void (*pFunction)(void);

extern uint8  	remote_ip[4];															 /*Զ��IP��ַ*/
extern uint16 	remote_port;															 /*Զ�˶˿ں�*/
extern uint16		local_port;																 /*���屾�ض˿�*/
extern uint8  	use_dhcp;																	 /*�Ƿ�ʹ��DHCP��ȡIP*/
extern uint8  	use_eeprom;																 /*�Ƿ�ʹ��EEPROM�е�IP������Ϣ*/

#define W5500_Debug_Define

#ifdef W5500_Debug_Define
#define W5500_Debug(...) printf(__VA_ARGS__)
#else
#define W5500_Debug(...)
#endif

#define W5500_Debug_Net_Info(...)	printf(__VA_ARGS__)


#define FW_VER_HIGH  						1               
#define FW_VER_LOW    					0
#define ON	                 		1
#define OFF	                 		0
#define HIGH	           	 			1
#define LOW		             			0

#define MAX_BUF_SIZE		 				1460											 /*����ÿ�����ݰ��Ĵ�С*/
#define KEEP_ALIVE_TIME	     		30	                       /*30sec*/
#define TX_RX_MAX_BUF_SIZE      2048							 
#define EEPROM_MSG_LEN        	sizeof(EEPROM_MSG)

#define IP_FROM_DEFINE	        0                          /*ʹ�ó�ʼ�����IP��Ϣ*/
#define IP_FROM_DHCP	          1                          /*ʹ��DHCP��ȡIP��Ϣ*/
#define IP_FROM_EEPROM	        2							             /*ʹ��EEPROM�����IP��Ϣ*/
extern uint8	ip_from;											               /*ѡ��IP��Ϣ����Դ*/
#pragma pack(1)
/*�˽ṹ�嶨����W5500�ɹ����õ���Ҫ����*/
typedef struct _CONFIG_MSG											
{
  uint8 mac[6];																						 /*MAC��ַ*/
  uint8 lip[4];																						 /*local IP����IP��ַ*/
  uint8 sub[4];																						 /*��������*/
  uint8 gw[4];																						 /*����*/	
  uint8 dns[4];																						 /*DNS��������ַ*/
  uint8 rip[4];																						 /*remote IPԶ��IP��ַ*/
	uint8 sw_ver[2];																				 /*����汾��*/
}CONFIG_MSG;
#pragma pack()

#pragma pack(1)
/*�˽ṹ�嶨����eepromд��ļ����������ɰ����޸�*/
typedef struct _EEPROM_MSG	                    
{
	uint8 mac[6];																							/*MAC��ַ*/
  uint8 lip[4];																							/*local IP����IP��ַ*/
  uint8 sub[4];																							/*��������*/
  uint8 gw[4];																							/*����*/
}EEPROM_MSG_STR;
#pragma pack()

extern EEPROM_MSG_STR EEPROM_MSG;
extern CONFIG_MSG  	ConfigMsg;
extern uint8 dhcp_ok;																				/*DHCP��ȡ�ɹ�*/
extern uint32	dhcp_time;																		/*DHCP���м���*/
extern vu8	ntptimer;																				/*NPT�����*/

/*MCU������غ���*/
void reboot(void);																					/*STM32��λ*/

/*W5500SPI��غ���*/
//void IINCHIP_WRITE( uint32 addrbsb,  uint8 data);						/*д��һ��8λ���ݵ�W5500*/
//uint8 IINCHIP_READ(uint32 addrbsb);													/*��W5500����һ��8λ����*/
//uint16 wiz_write_buf(uint32 addrbsb,uint8* buf,uint16 len);	/*��W5500д��len�ֽ�����*/
//uint16 wiz_read_buf(uint32 addrbsb, uint8* buf,uint16 len);	/*��W5500����len�ֽ�����*/
uint8  W5500_SPI_Write(uint32 reg,uint8 *data ,uint16 len);
uint8 W5500_SPI_Read(uint32 reg,uint8 *data ,uint16 len );

void W5500_Socket_WriteByte(uint32 reg,uint8 data);
uint8 W5500_Socket_ReadByte(uint32 reg);

void W5500_Delay(uint16_t num);

/*W5500����������غ���*/
void reset_w5500(void);																			/*Ӳ��λW5500*/
void set_w5500_mac(void);																		/*����W5500��MAC��ַ*/
void set_w5500_ip(void);																		/*����W5500��IP��ַ*/

/*��Ҫ�ö�ʱ�ĵ�Ӧ�ú���*/
void dhcp_timer_init(void);																	/*dhcp�õ��Ķ�ʱ����ʼ��*/
void ntp_timer_init(void);																	/*npt�õ��Ķ�ʱ����ʼ��*/

extern uint16 W5500_tcp_server_port;

/************* User API *************/
void W5500_Init(void);
void do_tcp_server(void);//TCP Server�ػ���ʾ����
uint16 W5500_Sent(uint8 *buff,uint32_t len);
__weak void W5500_Command_Process_Callback(uint8 *buff,uint16_t len);

#endif
