

#include "socket.h"
#include "W5500_interface.h"
#include "stdio.h"
#include "w5500.h"




/**
*@brief   This Socket function initialize the channel in perticular mode, 
*					and set the port and wait for W5200 done it.
*@param		s: socket number.
*@param		protocol: The socket to chose.
*@param		port:The port to bind.
*@param		flag: Set some bit of MR,such as **< No Delayed Ack(TCP) flag.
*@return  1 for sucess else 0.
*/
uint8 socket(SOCKET s, uint8 protocol, uint16 port, uint8 flag)
{
  uint8 ret;
	if (
			((protocol&0x0F) == Sn_MR_TCP)    ||
			((protocol&0x0F) == Sn_MR_UDP)    ||
			((protocol&0x0F) == Sn_MR_IPRAW)  ||
			((protocol&0x0F) == Sn_MR_MACRAW) ||
			((protocol&0x0F) == Sn_MR_PPPOE)
		)
	{
		close(s);
		W5500_Socket_WriteByte(Sn_MR(s) ,protocol | flag);
		if (port != 0) {
			 W5500_Socket_WriteByte( Sn_PORT0(s) ,(uint8)((port & 0xff00) >> 8));
			 W5500_Socket_WriteByte( Sn_PORT1(s) ,(uint8)(port & 0x00ff));
		} else {
			 local_port++; // if don't set the source port, set local_port number.
			 W5500_Socket_WriteByte(Sn_PORT0(s) ,(uint8)((local_port & 0xff00) >> 8));
			 W5500_Socket_WriteByte(Sn_PORT1(s) ,(uint8)(local_port & 0x00ff));
		}
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_OPEN ); // run sockinit Sn_CR

		/* wait to process the command... */
		while( W5500_Socket_ReadByte(Sn_CR(s)) )
			 ;
		/* ------- */
		ret = 1;
	}
   else
   {
      ret = 0;
   }
   return ret;
}
void setKPALVTR(SOCKET i,uint8 val)
{
  W5500_Socket_WriteByte(Sn_KPALVTR(i),val);
}
/**
*@brief   This function close the socket and parameter is "s" which represent the socket number
*@param		s: socket number.
*@return  None
*/
void close(SOCKET s)
{
	W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_CLOSE);

	/* wait to process the command... */
	while( W5500_Socket_ReadByte(Sn_CR(s) ) )
		;
	/* ------- */

	W5500_Socket_WriteByte( Sn_IR(s) , 0xFF);	 /* all clear */
}

/**
*@brief   This function established  the connection for the channel in passive (server) mode. 
*					This function waits for the request from the peer.
*@param		s: socket number.
*@return  1 for success else 0.
*/
uint8 listen(SOCKET s)
{
	uint8 ret;
	if (W5500_Socket_ReadByte( Sn_SR(s) ) == SOCK_INIT)
	{
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_LISTEN );
		/* wait to process the command... */
		while( W5500_Socket_ReadByte(Sn_CR(s) ) )
			;
		/* ------- */
		ret = 1;
	}
	else
	{
		ret = 0;
	}
	return ret;
}

/**
*@brief		This function established  the connection for the channel in Active (client) mode.
*					This function waits for the untill the connection is established.
*@param		s: socket number.
*@param		addr: The server IP address to connect
*@param		port: The server IP port to connect
*@return  1 for success else 0.
*/
uint8 connect(SOCKET s, uint8 * addr, uint16 port)
{
	uint8 ret;
	if(
			((addr[0] == 0xFF) && (addr[1] == 0xFF) && (addr[2] == 0xFF) && (addr[3] == 0xFF)) ||
			((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) ||
			(port == 0x00)
		)
	{
		ret = 0;
	}
	else
	{
		ret = 1;
		// set destination IP
		W5500_Socket_WriteByte( Sn_DIPR0(s), addr[0]);
		W5500_Socket_WriteByte( Sn_DIPR1(s), addr[1]);
		W5500_Socket_WriteByte( Sn_DIPR2(s), addr[2]);
		W5500_Socket_WriteByte( Sn_DIPR3(s), addr[3]);
		W5500_Socket_WriteByte( Sn_DPORT0(s), (uint8)((port & 0xff00) >> 8));
		W5500_Socket_WriteByte( Sn_DPORT1(s), (uint8)(port & 0x00ff));
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_CONNECT);
		/* wait for completion */
		while ( W5500_Socket_ReadByte(Sn_CR(s) ) ) ;

		while ( W5500_Socket_ReadByte(Sn_SR(s)) != SOCK_SYNSENT )
		{
			if(W5500_Socket_ReadByte(Sn_SR(s)) == SOCK_ESTABLISHED)
			{
				break;
			}
			if (getSn_IR(s) & Sn_IR_TIMEOUT)
			{
				W5500_Socket_WriteByte(Sn_IR(s), (Sn_IR_TIMEOUT));  // clear TIMEOUT Interrupt
				ret = 0;
				break;
			}
		}
	}
	return ret;
}

/**
*@brief   This function used for disconnect the socket s
*@param		s: socket number.
*@return  1 for success else 0.
*/
void disconnect(SOCKET s)
{
	W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_DISCON);

	/* wait to process the command... */
	while( W5500_Socket_ReadByte(Sn_CR(s) ) )
		;
	/* ------- */
}

/**
*@brief   This function used to send the data in TCP mode
*@param		s: socket number.
*@param		buf: data buffer to send.
*@param		len: data length.
*@return  1 for success else 0.
*/
uint16 send(SOCKET s, const uint8 * buf, uint16 len)
{
  uint8 status=0;
  uint16 ret=0;
  uint16 freesize=0;

  if (len > getIINCHIP_TxMAX(s)) ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
  else ret = len;

  // if freebuf is available, start.
  do
  {
    freesize = getSn_TX_FSR(s);
    status = W5500_Socket_ReadByte(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT))
    {
      ret = 0;
      break;
    }
  } while (freesize < ret);
  
  // copy data
  send_data_processing(s, (uint8 *)buf, ret);
  W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_SEND);

  /* wait to process the command... */
  while( W5500_Socket_ReadByte(Sn_CR(s) ) );

  while ( (W5500_Socket_ReadByte(Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
  {
    status = W5500_Socket_ReadByte(Sn_SR(s));
    if ((status != SOCK_ESTABLISHED) && (status != SOCK_CLOSE_WAIT) )
    {
      W5500_Debug(" SEND_OK Problem!!\r\n");
      close(s);
      return 0;
    }
  }
  W5500_Socket_WriteByte( Sn_IR(s) , Sn_IR_SEND_OK);

	#ifdef __DEF_IINCHIP_INT__
		putISR(s, getISR(s) & (~Sn_IR_SEND_OK));
	#else
		W5500_Socket_WriteByte( Sn_IR(s) , Sn_IR_SEND_OK);
	#endif
		return ret;
}

/**
*@brief		This function is an application I/F function which is used to receive the data in TCP mode.
*					It continues to wait for data as much as the application wants to receive.
*@param		s: socket number.
*@param		buf: data buffer to receive.
*@param		len: data length.
*@return  received data size for success else 0.
*/
uint16 recv(SOCKET s, uint8 * buf, uint16 len)
{
	uint16 ret=0;
	if ( len > 0 )
	{
		recv_data_processing(s, buf, len);
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_RECV);
		/* wait to process the command... */
		while( W5500_Socket_ReadByte(Sn_CR(s) ));
		/* ------- */
		ret = len;
	}
	return ret;
}

/**
*@brief   This function is an application I/F function which is used to send the data for other then TCP mode.
*					Unlike TCP transmission, The peer's destination address and the port is needed.
*@param		s: socket number.
*@param		buf: data buffer to send.
*@param		len: data length.
*@param		addr: IP address to send.
*@param		port: IP port to send.
*@return  This function return send data size for success else 0.
*/
uint16 sendto(SOCKET s, const uint8 * buf, uint16 len, uint8 * addr, uint16 port)
{
	uint16 ret=0;

	if (len > getIINCHIP_TxMAX(s)) 
	ret = getIINCHIP_TxMAX(s); // check size not to exceed MAX size.
	else ret = len;

	if( ((addr[0] == 0x00) && (addr[1] == 0x00) && (addr[2] == 0x00) && (addr[3] == 0x00)) || ((port == 0x00)) )//||(ret == 0) )
	{
		/* added return value */
		ret = 0;
	}
	else
	{
		W5500_Socket_WriteByte( Sn_DIPR0(s), addr[0]);
		W5500_Socket_WriteByte( Sn_DIPR1(s), addr[1]);
		W5500_Socket_WriteByte( Sn_DIPR2(s), addr[2]);
		W5500_Socket_WriteByte( Sn_DIPR3(s), addr[3]);
		W5500_Socket_WriteByte( Sn_DPORT0(s),(uint8)((port & 0xff00) >> 8));
		W5500_Socket_WriteByte( Sn_DPORT1(s),(uint8)(port & 0x00ff));
		// copy data
		send_data_processing(s, (uint8 *)buf, ret);
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_SEND);
		/* wait to process the command... */
		while( W5500_Socket_ReadByte( Sn_CR(s) ) )
			;
		/* ------- */
		while( (W5500_Socket_ReadByte( Sn_IR(s) ) & Sn_IR_SEND_OK) != Sn_IR_SEND_OK )
		{
			if (W5500_Socket_ReadByte( Sn_IR(s) ) & Sn_IR_TIMEOUT)
			{
				/* clear interrupt */
				W5500_Socket_WriteByte( Sn_IR(s) , (Sn_IR_SEND_OK | Sn_IR_TIMEOUT)); /* clear SEND_OK & TIMEOUT */
				return 0;
			}
		}
		W5500_Socket_WriteByte( Sn_IR(s) , Sn_IR_SEND_OK);
	}
	return ret;
}

/**
*@brief   This function is an application I/F function which is used to receive the data in other then
*					TCP mode. This function is used to receive UDP, IP_RAW and MAC_RAW mode, and handle the header as well.
*@param		s: socket number.
*@param		buf: data buffer to receive.
*@param		len: data length.
*@param		addr: IP address to receive.
*@param		port: IP port to receive.
*@return	This function return received data size for success else 0.
*/
uint16 recvfrom(SOCKET s, uint8 * buf, uint16 len, uint8 * addr, uint16 *port)
{
	uint8 head[8];
	uint16 data_len=0;
	uint16 ptr=0;
	uint32 addrbsb =0;
	if ( len > 0 )
	{
		ptr     = W5500_Socket_ReadByte(Sn_RX_RD0(s) );
		ptr     = ((ptr & 0x00ff) << 8) + W5500_Socket_ReadByte(Sn_RX_RD1(s));
		addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;

		switch (W5500_Socket_ReadByte(Sn_MR(s) ) & 0x07)
		{
			case Sn_MR_UDP :
			W5500_SPI_Read(addrbsb, head, 0x08);        
			ptr += 8;
			// read peer's IP address, port number.
			addr[0]  = head[0];
			addr[1]  = head[1];
			addr[2]  = head[2];
			addr[3]  = head[3];
			*port    = head[4];
			*port    = (*port << 8) + head[5];
			data_len = head[6];
			data_len = (data_len << 8) + head[7];

			addrbsb = (uint32)(ptr<<8) +  (s<<5) + 0x18;
			W5500_SPI_Read(addrbsb, buf, data_len);                
			ptr += data_len;

			W5500_Socket_WriteByte( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
			W5500_Socket_WriteByte( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
			break;

			case Sn_MR_IPRAW :
			//	   	W5500_Debug(" \r\n Sn_MR_IPRAW \r\n");
			W5500_SPI_Read(addrbsb, head, 0x06);

			ptr += 6;
			addr[0]  = head[0];
			addr[1]  = head[1];
			addr[2]  = head[2];
			addr[3]  = head[3];
			data_len = head[4];
			data_len = (data_len << 8) + head[5];

			addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;

			//		W5500_Debug(" data��%d \r\n",data_len);
			W5500_SPI_Read(addrbsb, buf, data_len);

			ptr += data_len;

			W5500_Socket_WriteByte( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
			W5500_Socket_WriteByte( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));

			break;

			case Sn_MR_MACRAW :
			//	 W5500_Debug(" \r\n Sn_MR_MCRAW \r\n");
			W5500_SPI_Read(addrbsb, head, 0x02);
			ptr+=2;
			data_len = head[0];
			data_len = (data_len<<8) + head[1] - 2;
			if(data_len > 1514)
			{
				W5500_Debug(" data_len over 1514\r\n");
				while(1)
					;
			}

			addrbsb  = (uint32)(ptr<<8) +  (s<<5) + 0x18;
			W5500_SPI_Read(addrbsb, buf, data_len);
			ptr += data_len;

			W5500_Socket_WriteByte( Sn_RX_RD0(s), (uint8)((ptr & 0xff00) >> 8));
			W5500_Socket_WriteByte( Sn_RX_RD1(s), (uint8)(ptr & 0x00ff));
			break;

			default :
			break;
		}
		W5500_Socket_WriteByte( Sn_CR(s) ,Sn_CR_RECV);

		/* wait to process the command... */
		while( W5500_Socket_ReadByte( Sn_CR(s)) )
			;
		/* ------- */
	}
	return data_len;
}

