#ifndef __CUSTOM_UART_H__
#define __CUSTOM_UART_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_pm.h"
#include "cm_iomux.h"
#include "cm_virt_at.h"
#include "cm_uart.h"

// 定义需要的编译的串口
#define UART0		CM_UART_DEV_0		// MAIN_UART
//#define UART1		CM_UART_DEV_1		// GNSS_UART
#define UART2		CM_UART_DEV_2		// DBG_UART
#define UART_USB	0xFF				// USB_UART

int custom_uart_init(void);
int custom_uart_send(int dev, uint8_t *data, uint16_t len);

#endif


