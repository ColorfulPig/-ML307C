/**
 * @file        http.c
 * @brief       http示例，该示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by yangqiang
 */

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

#define UART_TEST CM_UART_DEV_0         //测试串口
#define UART_RX_BUF_SIZE (2 * 1024)     //串口线程接收缓冲区大小，用户可自行更改
#define UART_PAKCET_TIME 100            //串口按照时间分包：连续多久没收到串口数据，就认为是一包数据。用户可自行更改，单位是ms

typedef struct{
    int msg_type;
} uart_event_msg_t;

static osSemaphoreId_t s_uart_semaphore = NULL;     //串口数据通知信号量
static osMessageQueueId_t s_uart_event_queue = NULL;    //串口事件消息队列
static osThreadId_t s_uart_event_thread = NULL;         //串口事件线程

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart_callback(void *param, uint32_t type)
{
    uart_event_msg_t msg = {0};
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 收到接收事件，触发线程执行读取数据 */
        osSemaphoreRelease(s_uart_semaphore);
    }
    
    if ((CM_UART_EVENT_TYPE_RX_OVERFLOW & type) || (CM_UART_EVENT_TYPE_RX_FLOWCTRL & type))
    {
        /* 收到溢出事件或流控事件，触发其他线程处理事件 */
        msg.msg_type = type;
        if (s_uart_event_queue != NULL)//向队列发送数据
        {
            osMessageQueuePut(s_uart_event_queue, &msg, 0, 0);
        }
    }
}
/* 用于测试串口事件，用户可参考 */
static void uart_event_task(void *arg)
{
    uart_event_msg_t msg = {0};

    while (1)
    {
        if (osMessageQueueGet(s_uart_event_queue, &msg, NULL, osWaitForever) == osOK)
        {
            //cm_log_printf(0, "uart event msg type = %d\n", msg.msg_type);
            if (CM_UART_EVENT_TYPE_RX_OVERFLOW & msg.msg_type)
            {
                cm_log_printf(0, "CM_UART_EVENT_TYPE_RX_OVERFLOW... ...");
            }

            if (CM_UART_EVENT_TYPE_RX_FLOWCTRL & msg.msg_type)
            {
                cm_log_printf(0, "CM_UART_EVENT_TYPE_RX_FLOWCTRL... ...");
            }
        }
    }
}

/* 用于测试串口事件，用户可参考 */
static int cm_uart_event_task_create(void)
{
    if (s_uart_event_queue == NULL)
    {
        s_uart_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
    }

    if (s_uart_event_thread == NULL)
    {
        osThreadAttr_t attr = {
            .name = "uart_event",
            .priority = osPriorityNormal,
            .stack_size = 1024 * 2,
        };
        s_uart_event_thread = osThreadNew(uart_event_task, NULL, (const osThreadAttr_t*)&attr);
    }

    return 0;
}

/* 切换debug引脚打印的log到usb打印，掉电不保存配置 */
static void cm_virt_at_log2cat(void)
{
    char operation[64] = {0};
    sprintf(operation, "%s\r\n", "AT+MCFG=log2cat,1");
    uint8_t rsp[128] = {0};
    int32_t rsp_len = 0;

    if(cm_virt_at_send_sync((const uint8_t *)operation, rsp, &rsp_len, 10) == 0)
    {
        cm_log_printf(0, "rsp=%s rsp_len=%d\n", rsp, rsp_len);
    }
    else
    {
        cm_log_printf(0, "ret != 0\n");
    }
}

/* uart测试线程 */
void cm_uart_task_test(void *p)
{
    /* 1. 设置GPIO复用（可作为串口的GPIO引脚以及对应的复用功能见《资源综述手册》） */
    if(UART_TEST == CM_UART_DEV_0)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1);
    }
    else if(UART_TEST == CM_UART_DEV_1)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1);
    }
    else if(UART_TEST == CM_UART_DEV_2)
    {
        cm_virt_at_log2cat();
        cm_iomux_set_pin_func(CM_IOMUX_PIN_38, CM_IOMUX_FUNC_FUNCTION2);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_39, CM_IOMUX_FUNC_FUNCTION2);
    }
    
    /* 2. 创建串口通知信号量和串口事件接收线程 */
    s_uart_semaphore = osSemaphoreNew(1, 0, NULL);
    cm_uart_event_task_create();

    /* 3. 注册串口事件参数 */
    cm_uart_event_t event = 
    {
        CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   //注册需要上报的事件类型
        "uart",                                                        //用户参数
        cm_serial_uart_callback                                         //上报事件的回调函数
    };
    cm_uart_register_event(UART_TEST, &event);

    /* 4. 初始化串口配置参数 */
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8, 
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE, 
        CM_UART_FLOW_CTRL_NONE, 
        CM_UART_BAUDRATE_115200,    
        0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
        0,   //环形缓存区大小按照默认配置8k
        0,
        0,
    };
    cm_uart_open(UART_TEST, &config);

    /* 5. 串口输出初始化完成的日志 */
    cm_uart_write(UART_TEST,"Uart init completely\r\n",strlen("Uart init completely\r\n"),100);

    uint8_t *buf = cm_malloc(UART_RX_BUF_SIZE);
    uint16_t recv_buflen = 0, single_recv_buflen = 0;
    while (1)
    {
        memset(buf, 0, UART_RX_BUF_SIZE);
        recv_buflen = 0;
        osSemaphoreAcquire(s_uart_semaphore, osWaitForever); // 阻塞
        while (1)
        {
            single_recv_buflen = cm_uart_read(UART_TEST, (buf + recv_buflen), UART_RX_BUF_SIZE - recv_buflen, 100);
            if (single_recv_buflen <= 0)   //第一次检查无串口数据或出现异常
            {
                if (recv_buflen == 0)     //无串口数据
                {
                    break;
                }
                osDelay(UART_PAKCET_TIME / 5);  //延时后再次读取底层接收串口缓冲队列中的是否有数据
                single_recv_buflen = cm_uart_read(UART_TEST, (buf + recv_buflen), UART_RX_BUF_SIZE - recv_buflen, 100);
                if (single_recv_buflen <= 0 && recv_buflen > 0)    //无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
                {
                    /* 6. 串口接收到什么数据，就输出什么数据 */
                    cm_uart_write(UART_TEST,"[Recv]",strlen("[Recv]"),100);
                    cm_uart_write(UART_TEST,buf,recv_buflen,100);
                    cm_uart_write(UART_TEST,"\r\n",strlen("\r\n"),100);
                    break;
                }
            }

            recv_buflen += single_recv_buflen;

            /* 到达buf空间后，也认为是一包数据 */
            if (recv_buflen >= UART_RX_BUF_SIZE)
            {
                /* 6. 串口接收到什么数据，就输出什么数据 */
                cm_uart_write(UART_TEST, "[Recv]", strlen("[Recv]"), 100);
                cm_uart_write(UART_TEST, buf, recv_buflen, 100);
                cm_uart_write(UART_TEST, "\r\n", strlen("\r\n"), 100);
                recv_buflen = 0; // 重新接收
                break;
            }
        }
    }
}


/* opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "test_task";
    app_task_attr.stack_size = 1024 * 4;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_uart_task_test, 0, &app_task_attr);
    return 0;
}