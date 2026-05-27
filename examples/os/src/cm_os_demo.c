/**
 * @file        cm_os_demo.c
 * @brief       使用os操作系统接口创建一个线程、一个信号量、一个定时器、一个消息队列
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhaoji
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_os_demo.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
static osThreadId_t os_task_handle = NULL;
static osTimerId_t os_timer_handle = NULL;
static osSemaphoreId_t os_sem_handle = NULL;
static osMessageQueueId_t os_msgqueue_handle = NULL;
static int os_timer_cb_count = 0;

/****************************************************************************
 * Public Types
 ****************************************************************************/

/**
 *  @brief 定时器回调函数
 *
 *  @param [out] argument 标识 与osTimerNew保持一致
 *
 *  @return
 *
 *  @details 回调函数中禁止串口打印等耗时操作，容易导致宕机，建议将事件发送任务中执行
 */
static void cm_os_demo_timer_callback(void *arg)
{
    os_timer_cb_count ++;

    // 释放信号量
    osSemaphoreRelease(os_sem_handle);

    // 往消息队列发送消息
    os_msg_t msg = { OS_MSG_TIMER_ARRIVED, 0, 0 };
    osMessageQueuePut(os_msgqueue_handle, &msg, 0, 0);  // 当在中断回调推送消息时，timeout参数必须为0，否则容易导致宕机
}


/**
 *  @brief 入口函数
 *
 *  @param [in] 
 *
 *  @return
 *
 *  @details 
 */
static void cm_os_demo_task(void)
{
    cm_log_printf(0,"[os demo] os task start success");

    // 创建信号量
    if (NULL == os_sem_handle)
    {
        os_sem_handle = osSemaphoreNew(1, 0, NULL);
    }

    // 创建定时器
    if (NULL == os_timer_handle)
    {
        os_timer_handle = osTimerNew(cm_os_demo_timer_callback, osTimerPeriodic, NULL, NULL);
        osTimerStart(os_timer_handle, (30 * 1000) / portTICK_PERIOD_MS );  // 定时周期单位为tick，此处示例为30s
    }

    // 创建消息队列
    if (NULL == os_msgqueue_handle)
    {
        os_msgqueue_handle = osMessageQueueNew(10, sizeof(os_msg_t), NULL); // 消息队列深度为10个，单个元素为结构体大小
    }
    
    // 任务函数一定不能自动运行结束，一般是循环处理消息、事件等
    while (1)
    {
        // 阻塞等待信号量释放
        osSemaphoreAcquire(os_sem_handle, osWaitForever);
        cm_log_printf(0,"[os demo] os sem acquire success");
        cm_log_printf(0,"[os demo] os timer count: %d", os_timer_cb_count);

        // 阻塞2s等待消息，当接收到消息时立即返回
        os_msg_t msg = {0};
        if (osMessageQueueGet(os_msgqueue_handle, &msg, NULL, 2000 / portTICK_PERIOD_MS) == osOK)
        {
            cm_log_printf(0,"[os demo] os get messageid: %d", msg.msg_id);
        }
        else
        {
            cm_log_printf(0,"[os demo] os get message timeout");
        }
    }
}

/**
 *  @brief OS系统接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始操作系统相关接口测试，会开启一个信号量、一个定时器、一个消息队列
 */
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t os_task_attr = {0};
    os_task_attr.name = "os_task";  // 任务名称
    os_task_attr.stack_size = 4096; // 任务栈空间
    os_task_attr.priority= osPriorityNormal;    // 任务优先级
    
    os_task_handle = osThreadNew((osThreadFunc_t)cm_os_demo_task, 0, &os_task_attr);  // 创建并运行任务
    if (NULL == os_task_handle)
    {
        cm_log_printf(0,"[os demo] os task create fail...");
    }
    else
    {
        cm_log_printf(0,"[os demo] os task create success");
    }

    return 0;
}
