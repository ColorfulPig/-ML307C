
#ifndef __CUSTOM_USB_H__
#define __CUSTOM_USB_H__

#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cm_usb.h"
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_mem.h"

#define CUSTOM_USB_RECV_LEN			1024

typedef enum
{
	CUSTOM_USB_EVENTREMOVE = 0,		// USB移除事件
	CUSTOM_USB_EVENTINSERT,			// USB插入事件
	CUSTOM_USB_EVENTRECV,			// USB接收事件
}CUSTOM_USB_EVENT;

typedef struct
{
	CUSTOM_USB_EVENT event;
	uint16_t datalen;
	uint8_t data[CUSTOM_USB_RECV_LEN];
} custom_usb_data_t; 

/* 初始化 USB 通道、队列和后台任务。 */
int custom_usb_init(void);
/* 通过 USB 通道发送数据。 */
int custom_usb_send(uint8_t *data, int32_t len);


#endif

