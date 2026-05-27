#ifndef _POSIX_LOG_H_
#define _POSIX_LOG_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "UART.h"

#define rt_kprintf  uart_printf

/* DEBUG level */
#define DBG_ERROR           0
#define DBG_WARNING         1
#define DBG_INFO            2
#define DBG_LOG             3


#define POSIX_DBG_LEVEL  DBG_WARNING


#if (POSIX_DBG_LEVEL >= DBG_LOG)
#define POSIX_LOG_D      uart_printf
#else
#define POSIX_LOG_D(...)
#endif

#if (POSIX_DBG_LEVEL >= DBG_INFO)
#define POSIX_LOG_I      uart_printf
#else
#define POSIX_LOG_I(...)
#endif

#if (POSIX_DBG_LEVEL >= DBG_WARNING)
#define POSIX_LOG_W      uart_printf
#else
#define POSIX_LOG_W(...)
#endif

#if (POSIX_DBG_LEVEL >= DBG_ERROR)
#define POSIX_LOG_E      uart_printf
#else
#define POSIX_LOG_E(...)
#endif


extern void utilsAssertFail(const char      *cond,
                            const char      *file,
                            signed short    line,
                            unsigned char   allowDiag);



#define RT_ASSERT(EX)    if(!(EX)) { utilsAssertFail(#EX, __MODULE__, __LINE__, 1);	  }




#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* _POSIX_LOG_H_ */

