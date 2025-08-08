
#include <stdio.h> 
#include <string.h>
#include "w5500_interface.h"
#include "w5500.h"
#include "socket.h"

#include "SPI_Driver.h"
#include "gpio.h"
#include "Timer.h"

CONFIG_MSG  ConfigMsg;																	/*���ýṹ��*/
EEPROM_MSG_STR EEPROM_MSG;															/*EEPROM�洢��Ϣ�ṹ��*/

uint8_t W5500_Connect_Flag[8]={0};
uint8_t W5500_Hello[]="Hello World\r\n";
/*MAC��ַ���ֽڱ���Ϊż��*/
/*������W5500�����������ͬһ�ֳ���������ʹ�ò�ͬ��MAC��ַ*/
uint8 mac[6]={0x00,0x08,0xdc,0x11,0x11,0x11};

/*����Ĭ��IP��Ϣ*/
uint8 local_ip[4]={169,254,1,92};												/*����W5500Ĭ��IP��ַ*/
uint8 subnet[4]={255,255,255,0};												/*����W5500Ĭ����������*/
uint8 gateway[4]={169,254,1,1};													/*����W5500Ĭ������*/
uint8 dns_server[4]={114,114,114,114};									/*����W5500Ĭ��DNS*/

uint16 local_port=8080;	                    						/*���屾�ض˿�*/

/*����Զ��IP��Ϣ*/
uint8  remote_ip[4]={169,254,1,160};										/*Զ��IP��ַ*/
uint16 remote_port=8000;																/*Զ�˶˿ں�*/

/*IP���÷���ѡ��������ѡ��*/
uint8  ip_from=IP_FROM_DEFINE;				

uint8  dhcp_ok=0;																				/*dhcp�ɹ���ȡIP*/
uint32 ms=0;																						/*�������*/
uint32 dhcp_time= 0;																		/*DHCP���м���*/
vu8    ntptimer = 0;																		/*NPT�����*/


/*******************Lower API**************************/
/**
*@brief		STM32 SPI1��д8λ����
*@param		dat��д���8λ����
*@return	��
*/
uint8  W5500_SPI_Write(uint32 reg,uint8 *data ,uint16 len)
{
	uint8_t ret;
	uint8 reg_buf[3];
	reg_buf[0]=(reg & 0x00FF0000)>>16;
	reg_buf[1]=(reg & 0x0000FF00)>>8;
	reg_buf[2]=(reg & 0x000000F8)+4;
	ret=SPI2_Write_Reg(1,reg_buf,data,3,len);
	return ret;
}
uint8 W5500_SPI_Read(uint32 reg,uint8 *data ,uint16 len )
{
	uint8_t ret;
	uint8 reg_buf[3];
	reg_buf[0]=(reg & 0x00FF0000)>>16;
	reg_buf[1]=(reg & 0x0000FF00)>>8;
	reg_buf[2]=(reg & 0x000000F8);
	ret=SPI2_WriteRead_T2R(1,reg_buf,data,3,len);
	return ret;
}

void W5500_Delay(uint16_t num)
{
	uint32_t x,y;
	for(x=0;x<num;x++)
		for(y=0;y<10000;y++);
}
/**
*@brief		W5500��λ���ú���
*@param		��
*@return	��
*/
void reset_w5500(void)
{
	W5500_RST_Control(0);
	W5500_Delay(100);  
	W5500_RST_Control(1);
	W5500_Delay(100);	

}
/*************************Internal Function***************************/
/**
*@brief		����W5500��IP��ַ
*@param		��
*@return	��
*/
void W5500_Add_Config(void)
{
	uint8_t ret=0x00;
	ret=W5500_Add_Read();
	local_ip[3]=local_ip[3]+ret;
	mac[5]=mac[5]+ret;
}
void set_w5500_ip(void)
{	
		
  /*���ƶ����������Ϣ�����ýṹ��*/
	memcpy(ConfigMsg.mac, mac, 6);
	memcpy(ConfigMsg.lip,local_ip,4);
	memcpy(ConfigMsg.sub,subnet,4);
	memcpy(ConfigMsg.gw,gateway,4);
	memcpy(ConfigMsg.dns,dns_server,4);
	
	/*����������Ϣ��������Ҫѡ��*/	
	ConfigMsg.sw_ver[0]=FW_VER_HIGH;
	ConfigMsg.sw_ver[1]=FW_VER_LOW;	

	/*��IP������Ϣд��W5500��Ӧ�Ĵ���*/	
	setSUBR(ConfigMsg.sub);
	setGAR(ConfigMsg.gw);
	setSIPR(ConfigMsg.lip);
	
	getSIPR (local_ip);			
	W5500_Debug_Net_Info("W5500 IP Address: %d.%d.%d.%d\r\n", local_ip[0],local_ip[1],local_ip[2],local_ip[3]);
	getSUBR(subnet);
	W5500_Debug_Net_Info("W5500 Subnet    : %d.%d.%d.%d\r\n", subnet[0],subnet[1],subnet[2],subnet[3]);
	getGAR(gateway);
	W5500_Debug_Net_Info("W5500 Gatway    : %d.%d.%d.%d\r\n", gateway[0],gateway[1],gateway[2],gateway[3]);
	W5500_Debug_Net_Info("W5500 Prot      : %d\r\n",local_port);
}

/**
*@brief		����W5500��MAC��ַ
*@param		��
*@return	��
*/
void set_w5500_mac(void)
{
	memcpy(ConfigMsg.mac, mac, 6);
	setSHAR(ConfigMsg.mac);	/**/
//	memcpy(DHCP_GET.mac, mac, 6);
}

void W5500_Socket_WriteByte(uint32 reg,uint8 data)
{
	W5500_SPI_Write(reg,&data,1);
}
uint8 W5500_Socket_ReadByte(uint32 reg)
{
	uint8 ret;
	W5500_SPI_Read(reg,&ret,1);
	return ret;
}


/*******************User API********************/


uint8 W5500_Rxbuff[2048]={0};				                              	        /*����һ��2KB�Ļ���*/
uint8_t gSocketNum=0;
void W5500_Init(void)
{
	reset_w5500();
	W5500_Add_Config();
//  PHY_check();
  set_w5500_mac();
  set_w5500_ip();
  socket_buf_init(txsize, rxsize);	
}
/**
*@brief		TCP Server�ػ���ʾ������
*@param		��
*@return	��
*/
void do_tcp_server(void)
{	
	uint16 len=0;  
	uint8_t SR_ret,IR_ret;
	SR_ret=getSn_SR(gSocketNum);			/*��ȡsocket��״̬*/
	switch(SR_ret)											            	
	{
		case SOCK_CLOSED:													                  /*socket���ڹر�״̬*/
			socket(gSocketNum ,Sn_MR_TCP,local_port,Sn_MR_ND);	        /*��socket*/
//			setKPALVTR(gSocketNum,1);
			break;     
		
		case SOCK_INIT:														                  /*socket�ѳ�ʼ��״̬*/
			listen(gSocketNum);												                /*socket��������*/			
			W5500_Connect_Flag[gSocketNum]=0;
			break;
		
		case SOCK_ESTABLISHED:												              /*socket�������ӽ���״̬*/
			
			IR_ret=getSn_IR(gSocketNum);
			if(IR_ret & Sn_IR_CON)
			{
				if(gSocketNum==0)
					close(1);
				else
					close(0);
				setSn_IR(gSocketNum, Sn_IR_CON);								          /*��������жϱ�־λ*/			
				send(gSocketNum,W5500_Hello,13);
			}
//			if(W5500_Connect_Flag[gSocketNum]==0)
//			{
//				send(gSocketNum,W5500_Hello,13);
//				W5500_Connect_Flag[gSocketNum]=1;
//			}
			len=getSn_RX_RSR(gSocketNum);									            /*����lenΪ�ѽ������ݵĳ���*/
			if(len>0)
			{
				recv(gSocketNum,W5500_Rxbuff,len);								              	/*��������Client������*/
				W5500_Rxbuff[len]=0x00; 											                  /*����ַ���������*/
//				W5500_Debug("%s\r\n",buff);
//				send(gSocketNum,buff,len);									              /*��Client��������*/
				W5500_Command_Process_Callback(W5500_Rxbuff,len);
			}
			break;
		
		case SOCK_CLOSE_WAIT:												                /*socket���ڵȴ��ر�״̬*/
			close(gSocketNum);
			break;
	}
//	if(gSocketNum < 7)
//		gSocketNum++;
//	else
//		gSocketNum = 0;
	if(gSocketNum==0)
		gSocketNum=1;
	else
		gSocketNum=0;
}
uint16 W5500_Sent(uint8 *buff,uint32_t len)
{
	uint16 ret=0;
	uint32 data_len;
	uint8_t *p_buff;
	p_buff=buff;
	data_len=len;
	while(data_len>100)
	{
		ret =send(gSocketNum,p_buff,100);
		p_buff=p_buff+100;
		data_len=data_len-100;
	}
	ret =send(gSocketNum,p_buff,data_len);	
	return ret;
}
__weak void W5500_Command_Process_Callback(uint8 *buff,uint16_t len)
{
	
}
