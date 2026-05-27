#ifndef __HTTP_CLIENT_ARCH_H__
#define __HTTP_CLIENT_ARCH_H__

#include "UART.h"
#include "diag_API.h"


#if (defined(LTEONLY_THIN) || defined(CRANEL_FP_8MRAM) || defined(LTEGSM_THIN))
#define http_debug(cat3, fmt, args...)

#else
#define http_debug(cat3, fmt, args...)  \
	do{									\
		DIAG_FILTER(MIFI, HTTPCLI, cat3, DIAG_INFORMATION)	\
		diagPrintf(fmt, ##args);			\
	}while(0)

#endif
#define http_error(cat3, fmt, args...) \
	do{									\
		DIAG_FILTER(MIFI, HTTPCLI, cat3, DIAG_INFORMATION)	\
		diagPrintf(fmt, ##args);			\
	}while(0)


extern char * __http_client_strdup(const char *s);

extern void * __http_client_malloc(int size);

extern void __http_client_free(void * p);

#define HTTP_CLIENT_MALLOC __http_client_malloc
#define HTTP_CLIENT_FREE(p)   do{__http_client_free(p);p=NULL;}while(0)

#undef strdup
#define strdup __http_client_strdup

#endif
