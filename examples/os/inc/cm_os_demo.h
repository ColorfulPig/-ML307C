/**
 * @file        cm_os_demo.h
 * @brief       使用os操作系统接口创建一个线程、一个信号量、一个定时器、一个消息队列
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhaoji
 */

#ifndef __CM_OS_DEMO_H__
#define __CM_OS_DEMO_H__

#define portTICK_PERIOD_MS      (5)     // 每个tick的周期

/**
 * @brief 消息ID列表，根据需求自定义
 * 
 */
typedef enum
{
    OS_MSG_TEST = 0,
    OS_MSG_TIMER_ARRIVED
} os_msgid_e;

/**
 * @brief 消息结构体，根据需求自定义，不能定义超过64的buffer
 * 
 */
typedef struct
{
	int msg_id;
    int param1;
    int param2;
} os_msg_t;

#endif