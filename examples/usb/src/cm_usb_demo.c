/**
 * @file        cm_usb_demo.c
 * @brief       USB功能是将USB例化成串口使用，连接电脑设备管理器会出现2个ASR Modem Device 其中一个为USB例化串口，可进行本demo中的回显测试
                注意该USB存在一特性：需先以某波特率发任意字节给模组，进行波特率适配，模组才能通过USB往外输出数据
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangwencheng
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
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

#include "cm_usb_demo.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
static osThreadId_t CM_USB_DEMO_MAIN_TASKHANDLE = NULL;    // USB DEMO测试线程
static osMessageQueueId_t CM_USB_DEMO_QUEUE_HANDLE = NULL; // USB 队列用于接收USB数据

/****************************************************************************
 * Public Types
 ****************************************************************************/
/**
 *  @brief USB接收数据回调函数
 *
 *  @param [out] data USB接收数据
 *  @param [out] datalen USB接收数据长度
 *
 *  @return
 *
 *  @details 当USB有数据到来时会触发该回调，并将数据推送到USB主线程中进行回显打印
 */
static void cm_usb_demo_recv_callback(void *data, int32_t len)
{
    if (len <= 0) // 如果无数据 则返回
    {
        return;
    }
    cm_usb_demo_data_t *usb_recv_data = cm_malloc(sizeof(cm_usb_demo_data_t));
    if (usb_recv_data == NULL)
    {
        return;
    }
    usb_recv_data->event = CM_USB_DEMO_EVENT_RECV;
    memcpy(usb_recv_data->data, data, len);
    usb_recv_data->datalen = len;
    if (osOK != osMessageQueuePut(CM_USB_DEMO_QUEUE_HANDLE, &usb_recv_data, 0, 0))   //timeout一定要为0  不能在回调中做阻塞操作
    {
        cm_free(usb_recv_data);
        return;
    } 
}

/**
 *  @brief USB插拔事件回调函数
 *
 *  @param [out] event 
 *
 *  @return
 *
 *  @details 当USB有插拔事件到来时会触发该回调，并将事件推送到USB主线程中进行打印
 */
static void cm_usb_demo_event_callback(int32_t event)
{
    cm_usb_demo_data_t *usb_recv_data = cm_malloc(sizeof(cm_usb_demo_data_t));
    if (usb_recv_data == NULL)
    {
        return;
    }
    usb_recv_data->event = event;
    if (osOK != osMessageQueuePut(CM_USB_DEMO_QUEUE_HANDLE, &usb_recv_data, 0, 0))
    {
        cm_free(usb_recv_data);
        return;
    }
    
    return;
}

/**
 *  @brief USB主线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 用于接收来自USB接收回调队列中的数据，并回显相同数据至USB以及用于打印插拔事件
 */
static void cm_usb_demo_main(void)
{
    while (1)
    {
        cm_usb_demo_data_t *usb_data = {0};
        if (osOK == osMessageQueueGet(CM_USB_DEMO_QUEUE_HANDLE, &usb_data, NULL, osWaitForever))
        {
            switch (usb_data->event)
            {
            case CM_USB_DEMO_EVENT_RECV:
                //注意该打印函数只能打印最多108字节 超过字节不打印 而非数据接收有问题
                cm_log_printf(0,"CM_USB_DEMO_EVENT_RECV:%s,%d", usb_data->data, usb_data->datalen);
                cm_usb2com_send_data(usb_data->data,usb_data->datalen);  //实现回显 
                break;
            case CM_USB_DEMO_EVENT_INSERT:
                cm_log_printf(0,"CM_USB_DEMO_EVENT_INSERT");
                break;
            case CM_USB_DEMO_EVENT_REMOVE:
                cm_log_printf(0,"CM_USB_DEMO_EVENT_REMOVE");
                break;

            default:
                break;
            }
            cm_free(usb_data);
            usb_data = NULL;
        }
    }
}

/**
 *  @brief USB接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始USB相关接口测试，初始化USB、创建USB的消息队列，创建USB数据收发线程
 */
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    cm_usb2com_register_recv_cb(cm_usb_demo_recv_callback);    //数据接收回调
    cm_usb2com_register_status_cb(cm_usb_demo_event_callback); //USB插拔检测事件回调

    /*创建USB数据接收队列*/
    if (CM_USB_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_USB_DEMO_QUEUE_HANDLE = osMessageQueueNew(20, sizeof(cm_usb_demo_data_t *), NULL); // 创建消息队列 用于接收USB回调中的数据
        if (CM_USB_DEMO_QUEUE_HANDLE == NULL)
        {
            cm_log_printf(0,"CM_DEMO_USB_QUEUE_HANDLE is NULL");
        }
    }

    /*创建USB数据接收/发送处理线程*/
    osThreadAttr_t usb_demo_main_task_attr = {0};
    usb_demo_main_task_attr.name = "usb_demo_main_task";
    usb_demo_main_task_attr.stack_size = 4 * 1024;
    usb_demo_main_task_attr.priority = osPriorityNormal;

    if (CM_USB_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_USB_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_usb_demo_main, 0, &usb_demo_main_task_attr);
        if (CM_USB_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0,"CM_USB_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    return 0;
}