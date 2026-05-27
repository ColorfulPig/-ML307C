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

#ifndef ATNET_SSL_HEADER_INCLUDED
#define  ATNET_SSL_HEADER_INCLUDED

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifdef ATNET_SSL

#include <stddef.h>
#include "diag.h"
//#include "csw_mrv_mem.h"
#include "telatparamdef.h"
#include "cmux_api.h"


typedef struct _Atnet_SSL_Ctx{
	UINT8   ssl_version;    //0---SLL3.0; 1---TLS1.0; 2----TLS1.1; 3----TLS1.2; 4----ALL;
	INT32   ciphersuite[8];
	UINT8   ca_cert_path[MAX_STRING_LEN];
	UINT8   client_cert_path[MAX_STRING_LEN];
	UINT8   client_key_path[MAX_STRING_LEN];
	UINT8   sec_level;      //0: no authentication; 1:manage server authentication;2: manage server and client authentication if requested by the remote server
	UINT8   ignore_local_time;//0: care about tiem check for certification; 1:ignore tiem check for certification
	UINT16  negotiate_time; //indicates max timeout used in SSL negotiate stage,10-300 sec,default :300 sec
	int fd;
}Atnet_SSL_Ctx;


int atnet_ssl_write( void *ssl, const unsigned char *buf, size_t len );
int atnet_ssl_read( void *ssl, const unsigned char *buf, size_t len );

void init_atnet_ssl_context(void);
Atnet_SSL_Ctx* atnet_get_ssl_context(int sslId);
int atnet_get_ssl_ctx_cfg(struct miSSLCfgReq * sslcfgreq);
int atnet_set_ssl_ctx_cfg(struct miSSLCfgReq * sslcfgreq);
Atnet_SSL_Ctx * atnet_get_all_ssl_ctx_cfg(void);
void * atnet_ssl_client_init(int fd,UINT8 ssl_ctx_id);
void atnet_ssl_client_shutdown(int netId);
#endif

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // MONGOOSE_HEADER_INCLUDED

