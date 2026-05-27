
#ifndef __UART_COMMON_TYPE_H__
#define __UART_COMMON_TYPE_H__

#include "UART.h"

typedef unsigned char       BOOL;

typedef int                 int32;
typedef unsigned int        uint32;
typedef unsigned int        uint32_t;

typedef unsigned char       UINT8;
typedef unsigned char       u_int8;
typedef unsigned char       uint8;
typedef unsigned char       uint8_t;


typedef unsigned short int  uint16_t;
typedef unsigned short int  u_int16;
typedef unsigned short int  uint16;
typedef short int           int16;

typedef unsigned char       __u8;
typedef unsigned short      __u16;
typedef unsigned long       __u32;

typedef unsigned short      UINT16;
typedef unsigned long       UINT32;
typedef unsigned long long  UINT64;

#define write32(addr, val)  (*(volatile unsigned int *)(addr) = (val))
#define read32(addr)        (*(volatile unsigned int *)(addr))
#define REG32(x)            (*((volatile uint32 *)(x)))

#define TRUE 1
#define FALSE 0

//if use CPUartLogPrintf, log will output from catstudio
//if use uart_printf, log will output from uart
#ifdef LWIP_IPNETBUF_SUPPORT
//#define raw_uart_log      uart_printf
#define raw_uart_log(fmt, args...) do { CPUartLogPrintf("[BTUART]"fmt, ##args); } while(0)
#else
#define raw_uart_log      uart_printf
//#define raw_uart_log(fmt, args...) do { CPUartLogPrintf("[BT]"fmt, ##args); } while(0)
#endif




#endif
