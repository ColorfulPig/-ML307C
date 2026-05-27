/**
 * @file        cm_demo_ntp.h
 * @brief       ntp示例
 * @copyright   Copyright © 2022 China Mobile IOT. All rights reserved.
 */

#ifndef _CM_DEMO_NTP_H_
#define _CM_DEMO_NTP_H_


/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include "cm_uart.h"


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/
#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif


/**
 * @brief  
 *
 * @return  void
 *
 * @details More details
 */
void cm_test_ntp(unsigned char **cmd, int len);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* _CM_DEMO_UART_H_ */
