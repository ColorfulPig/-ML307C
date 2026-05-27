/**
 * @file        cm_usb_demo.h
 * @brief       USB功能是将USB例化成串口使用，连接电脑设备管理器会出现2个ASR Modem Device 其中一个为USB例化串口，可进行本demo中的回显测试
                注意该USB存在一特性：需先以某波特率发任意字节给模组，进行波特率适配，模组才能通过USB往外输出数据
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangwencheng
 */
#ifndef __CM_USB_DEMO_H__
#define __CM_USB_DEMO_H__

#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cm_usb.h"
#include "cm_os.h"

#define CM_DEMO_USB_RECV_DATALEN_MAX 1024

typedef enum
{
    CM_USB_DEMO_EVENT_REMOVE = 0,
    CM_USB_DEMO_EVENT_INSERT,
    CM_USB_DEMO_EVENT_RECV,
}CM_USB_DEMO_EVENT;

/* USBT消息内容结构体 */
typedef struct
{
    CM_USB_DEMO_EVENT event;
    uint16_t datalen;
	uint8_t data[CM_DEMO_USB_RECV_DATALEN_MAX];
} cm_usb_demo_data_t; 

#endif
