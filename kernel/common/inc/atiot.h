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

#ifndef ATNBIOT_HEADER_INCLUDED
#define  ATNBIOT_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef NEUL_COMMAND

/*
#include "sockets.h"


typedef struct _AtNBIOT_Data{
	char *buf;
	int len;
	struct sockaddr_in cliaddr;
	void *next;
}AtNBIOT_Data;
*/

typedef struct _AtNBIOT_Context{
	int fd;
	UINT8 SocketId;
	UINT8 recv_control;
	UINT8 hex_notSupport;
	UINT8 reserve;
	UINT32 received_length;
	UINT32 reqHandle;
	void *next;
}AtNBIOT_Context;

void* get_nbiot_msgQ();

#endif
#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED

