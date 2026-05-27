/**
 * @file        keypad.h
 * @brief       示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by cmiot4594
 */


#ifndef __KEYPAD_H__
#define __KEYPAD_H__

#include <stdlib.h>
#include <stdio.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CM_PM_MAX_NUM 10
#define WKP_SOURCE_KEYPAD 1<<21
#define LVGL_TICK 5

#define  OPENCPU_KEYIN_PIN0     CM_IOMUX_PIN_81
#define  OPENCPU_KEYOUT_PIN0    CM_IOMUX_PIN_29
#define  OPENCPU_KEYIN_MUX0     CM_IOMUX_PIN_81, CM_IOMUX_FUNC_FUNCTION2
#define  OPENCPU_KEYOUT_MUX0    CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION3

#define  OPENCPU_KEYIN_PIN1     CM_IOMUX_PIN_28
#define  OPENCPU_KEYOUT_PIN1    CM_IOMUX_PIN_49
#define  OPENCPU_KEYIN_MUX1     CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION3
#define  OPENCPU_KEYOUT_MUX1    CM_IOMUX_PIN_49, CM_IOMUX_FUNC_FUNCTION3

#define UART_BUF_LEN            1024
#define OPENCPU_UART0TX_IOMUX CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1
#define OPENCPU_UART0RX_IOMUX CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1

enum
{
    KEY_00 = 0x00,
    KEY_01 = 0x01,
    KEY_10 = 0x10,
    KEY_11 = 0x11,
}KEYPAD_EVENT;

typedef struct 
{
    int cmd_execute;  //命令执行标志，1未被执行，0执行完成
    unsigned char * buf[20]; //最多接受20个参数
    int len;
}cm_uart_cmd_recv_t;

#endif