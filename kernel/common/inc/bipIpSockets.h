/*------------------------------------------------------------
(C) Copyright [2006-2016] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _MTIL_IPSOCKETS_LIB_H_INC_
#define _MTIL_IPSOCKETS_LIB_H_INC_

#include "sockets.h"

#define OUT
#define IN

typedef int SOCKET_DESC;

#ifdef  __cplusplus
extern "C" {
#endif				/* __cplusplus */
	BOOL MTIL_InitTcp(IN OUT SOCKET_DESC * sock, IN unsigned short port);
	BOOL MTIL_Accept(IN SOCKET_DESC mainSock, OUT SOCKET_DESC * sessionSock);
	BOOL MTIL_Listen(IN SOCKET_DESC sock, IN int backlog);
	BOOL MTIL_Connect(IN SOCKET_DESC sock, struct sockaddr *addrTo);
	BOOL MTIL_SendSock(IN SOCKET_DESC sock, IN char *sndBuf, IN int sndSize, IN struct sockaddr *to);
	BOOL MTIL_RecvSock(IN SOCKET_DESC sock, IN OUT char *rcvBuf, IN OUT int *bufSize, IN long waitSec,
			   IN OUT struct sockaddr *from, int from_len);
	int MTIL_SelectReadMult(IN SOCKET_DESC * sockArray, IN int numSocks, IN long waitSec);
	BOOL MTIL_RcvMtilTcpMessage(IN SOCKET_DESC sock, IN OUT char *buffer, IN OUT int *buf_sz, int minHdrLen);
	void MTIL_CloseSocket(IN OUT SOCKET_DESC * sock, char *keep_path);
	int MTIL_GetLastSocketError(void);
	BOOL MTIL_SetSocketKeepAliveOpt(SOCKET_DESC sock);
#ifdef  __cplusplus
}
#endif				/* __cplusplus */
#endif				/* _MTIL_IPSOCKETS_LIB_H_INC_ */
