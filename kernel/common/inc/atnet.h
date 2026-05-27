// Copyright (c) 2004-2010 Sergey Lyubka
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#ifndef ATNET_HEADER_INCLUDED
#define  ATNET_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus



#include <stddef.h>
#include "diag.h"
//#include "csw_mrv_mem.h"
#include "telatparamdef.h"
#include "mi_api.h"


#define TEL_ATNET_AT_CMD_ATP TEL_AT_CMD_ATP_12

#ifndef NO_EXTEND_MY_Q_AT


typedef enum {
	AT_NET_TCP_SERVER_SERVICE = MI_PRIM_DEFINE_END,
	AT_NET_TCP_CLIENT_SERVICE,
	AT_NET_UDP_SERVICE,
	AT_NET_TCP_SERVER_CREATE,
	AT_NET_TCP_SERVER_CREATE_RES,
	AT_NET_TCP_SERVER_LISTEN,
	AT_NET_TCP_SERVER_LISTEN_RES,
	AT_NET_TCP_SERVER_ACCEPT,
	AT_NET_TCP_SERVER_ACCEPT_RES,
	AT_NET_TCP_SERVER_ACCEPT_ERROR,
	AT_NET_TCP_SERVER_INCOMING_CLIENT,
	AT_NET_TCP_SERVER_INCOMING_CLIENT_RECV,
	AT_NET_TCP_CLIENT_CREATE,
	AT_NET_TCP_CLIENT_CREATE_RES,
	AT_NET_TCP_CLIENT_CONNECT,
	AT_NET_TCP_CLIENT_CONNECT_RES,
	AT_NET_UDP_CREATE,
	AT_NET_UDP_CREATE_RES,
	AT_NET_UDP_CONNECT,
	AT_NET_RECV_ERROR,
	AT_NET_ASR_OPEN,
} _AtNetMessage;

typedef enum{
	AT_NET0=0,
	AT_NET1,
#ifndef LTEONLY_THIN_SINGLE_SIM_2MFLASH
	AT_NET2,
	AT_NET3,
	AT_NET4,
	AT_NET5,
#endif
	AT_NET_MAX
} _AtNetId;


struct netUdpDataBackup
{
	UINT16 len;
	char * buf;
};

struct netTimerCon
{
    int netId;
    struct netUdpDataBackup *udpdata;
};


struct netSetSocketOpenReq_t
{
	INT32 result;
	UINT8 socketID;
};

typedef struct _Atnet_SocketId_Map{
	int fd;
	UINT8 SocketId;		/*0-5*/
}Atnet_SocketId_Map;


typedef struct _Atnet_Socket{
	UINT16 local_port;
	UINT16 ip_port;		/*dest port*/
	UINT32 ip_addr;/*dest ip address*/
}Atnet_Socket;

typedef struct _Atnet_Recv_Buf{
	UINT16      	rota_flag;
	char 		* begin;
	char 		* end;
	char 		* buffer;
}Atnet_Recv_Buf;


typedef struct _AtNet_Context{
	UINT8 channel;		/*0-5*/
	UINT8 netType;		/*0: TCP client ; 1: TCP server; 2: UDP; 3:UDP SERVICE*/
	UINT8 viewMode;		/*0:HEXT ; 1: TEXT*/
	UINT8 accept_transportMode;/*TCP Server, 0:none transport ; 1: transport; **/
	UINT8 transportMode;/*net create, 0:none transport ; 1: transport; 2: push mode*/
	UINT32 unAck;
	UINT8 action;		/*TCP Server, 0: accept ; 1: don't accept*/
	UINT8 listen;		/*TCP Server, 0: do't listen ; 1: listen*/
	UINT8 type;			/*0: disconnect; 1: connected*/
	Atnet_SocketId_Map socket_map[3];
	char service_addr[MAX_STRING_LEN];
	UINT16 service_port;
	Atnet_Socket net_socket;
	Atnet_Recv_Buf recv;
	UINT32 Total_send;
	UINT32 Total_read;
	UINT16 remote_port;
	UINT32 remote_ip;
	INT32 error_code;
	UINT8 ssl_enable;
	UINT8 ssl_ctx_id;
	void* sslclient;
}AtNet_Context;

void AtNetMutexLock(int netId);
void AtNetMutexUnlock(int netId);
void init_atnet(void);
void e20_set_timer_flag(void);

AtNet_Context* atnet_get_context_by_netId(int netId);
int atnet_context_is_transport_channel(int channel);
int cmuxGetSvcIdByConnectId(UINT8   ConnectID);
int atnet_context_is_transport_channel(int channel);

#endif
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED

