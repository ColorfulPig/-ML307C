#ifndef _ASR_GPS_LOG_H
#define _ASR_GPS_LOG_H

#include "uart.h"

#define GPS_DEBUG  1
#define NMEA_DEBUG 0
#define SUPL_TEST 0

#if GPS_DEBUG
/*This path is for acat & gnss tool.*/
#if (defined(CRANELS_SUPPORT) || defined(CRANELG_SUPPORT))
#  define  D(f, ...)  UARTLogPrintf_Extend(LOG_MODULE_GNSS, f, ##__VA_ARGS__);    
#else
#  define  D(f, ...)  CPUartLogPrintf("[GNSS]: " f, ##__VA_ARGS__)
#endif /* CRANELS_SUPPORT || CRANELG_SUPPORT */

#else
#  define  D(...)   ((void)0)
#endif /* GPS_DEBUG  */

#endif  /* _ASR_GPS_LOG_H */
