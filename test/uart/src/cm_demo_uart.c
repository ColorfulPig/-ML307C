/*********************************************************
 *  @file    cm_demo_uart.c
 *  @brief   OpenCPU UART示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cm_uart.h"
#include "cm_demo_uart.h"
#include "cm_iomux.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdarg.h"
#include "cm_os.h"
#include "cm_mem.h"
#include "cm_common.h"
#include "cm_sys.h"
#include "cm_virt_at.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
 
#define UART_BUF_LEN            1024

/****************************************************************************
 * Private Types
 ****************************************************************************/
 
typedef struct{
    int msg_type;
} uart_event_msg_t;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
void cm_demo_printf(char *str, ...);
static void cm_uart_recv_task(void *param);
static int __cm_cmd_engine(char * prefix,char * uart_buf,int* uart_buf_len,char **cmd_buf,int *cmd_len);

/****************************************************************************
 * Private Data
 ****************************************************************************/

static int rx_rev_len = 0;
static char rx_rev_data[UART_BUF_LEN] = {0};
static osThreadId_t OC_Uart_TaskHandle = NULL; //串口数据接收、解析任务Handle
static void* g_uart_sem = NULL;
static osMessageQueueId_t uart_event_queue = NULL;
static osThreadId_t uart_event_thread = NULL;

cm_uart_cmd_recv_t gstUartCmdRecv = {0}; //串口命令缓冲区

extern osEventFlagsId_t cmd_task_flag;
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/

/* 用于测试串口事件，用户可参考 */
static void uart_event_task(void *arg)
{
    uart_event_msg_t msg = {0};

    while (1)
    {
        if (osMessageQueueGet(uart_event_queue, &msg, NULL, osWaitForever) == osOK)
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
static int uart_event_task_create(void)
{
    if (uart_event_queue == NULL)
    {
        uart_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
    }

    if (uart_event_thread == NULL)
    {
        osThreadAttr_t attr1 = {
            .name = "uart_event",
            .priority = UART_TASK_PRIORITY,
            .stack_size = 1024 * 2,
        };
        uart_event_thread = osThreadNew(uart_event_task, NULL, (const osThreadAttr_t*)&attr1);
    }

    return 0;
}

/* 解析指令函数，用于SDK测试，与串口配置无关 */
static int __cm_cmd_engine(char * prefix,char * uart_buf,int* uart_buf_len,char **cmd_buf,int *cmd_len)
{
    char *p1 = NULL;
    char *p2 = NULL;
    char *p3 = NULL;
    char *temp1 = NULL;
    char *temp2 = NULL;
    
    uart_buf[*uart_buf_len] = 0;//结尾
    
    p1 = strstr(uart_buf, prefix);
    
    if (p1 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    
    p2 = strstr(p1, "\r\n");
    
    if (p2 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    
    *cmd_len = 0;
    
    p3 = strchr(p1, ':');
    
    if (p3 == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    
    cmd_buf[(*cmd_len)++] = cm_malloc(p3 - p1 + 1 + 1);
    memset(cmd_buf[*cmd_len - 1], 0, p3 - p1 + 1);
    
    if (cmd_buf[*cmd_len - 1] == 0)
    {
        *uart_buf_len = 0;
        return -1;
    }
    
    memcpy(cmd_buf[*cmd_len - 1], p1, (int)(p3 - p1));
    temp1 = p3;
    
    while (1)
    {
        if (*cmd_len >= 20)
        {
            *uart_buf_len = 0;
            for (int j = 0; j < *cmd_len; j++)
            {
                cm_free(cmd_buf[j]);
            }
            return -1;
        }
        
        temp2 = strchr(temp1 + 1, ':');
        
        if (temp2 == 0)
        {
            break;
        }
        
        if ((temp2 - temp1) < 1)
        {
            for (int j = 0; j < *cmd_len; j++)
            {
                cm_free(cmd_buf[j]);
            }
            
            *uart_buf_len = 0;
            return -1;
        }
        
        cmd_buf[(*cmd_len)++] = cm_malloc(temp2 - temp1 + 1);
        
        if (cmd_buf[*cmd_len -1] == 0)
        {
            for(int j = 0; j < *cmd_len; j++)
            {
                cm_free(cmd_buf[j]);
            }
             
            *uart_buf_len = 0;
            return -1;
        }
        
        memset(cmd_buf[*cmd_len - 1], 0, temp2 - temp1);
        memcpy(cmd_buf[*cmd_len - 1], temp1 + 1, temp2 - temp1 - 1);
        temp1 = temp2;
    }
    
    if ((p2 - temp1 - 1) < 1)
    {
        for (int j = 0; j < *cmd_len; j++)
        {
            cm_free(cmd_buf[j]);
        }
        
        *uart_buf_len = 0;
        return -1;
    }
    
    cmd_buf[(*cmd_len)++] = cm_malloc(p2 - temp1 + 1);
    memset(cmd_buf[*cmd_len - 1], 0, p2 - temp1);
    memcpy(cmd_buf[*cmd_len - 1], temp1 + 1, p2 - 1 - temp1);
    *uart_buf_len = 0;
    return 0;
}

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart_callback(void *param, uint32_t type)
{
    uart_event_msg_t msg = {0};
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 收到接收事件，触发其他线程执行读取数据 */
        osSemaphoreRelease(g_uart_sem);
    }
    
    if ((CM_UART_EVENT_TYPE_RX_OVERFLOW & type) || (CM_UART_EVENT_TYPE_RX_FLOWCTRL & type))
    {
        /* 收到溢出事件或流控事件，触发其他线程处理事件 */
        msg.msg_type = type;
        
        if (uart_event_queue != NULL)//向队列发送数据
        {
            osMessageQueuePut(uart_event_queue, &msg, 0, 0);
        }
    }
}

/* 串口接收示例，平时使用信号量挂起，当收到接收事件后，释放信号量以触发读取任务 */
static void cm_uart_recv_task(void *param)
{
    int temp_len = 0;

    cm_uart_cmd_recv_t* pstUartCmdRecv = &gstUartCmdRecv;
    
    while (1)
    {
        if (g_uart_sem != NULL)
        {
            osSemaphoreAcquire(g_uart_sem, osWaitForever);//阻塞
        }
        
        if (rx_rev_len < UART_BUF_LEN)
        {
            temp_len = cm_uart_read(OPENCPU_MAIN_URAT, (void*)&rx_rev_data[rx_rev_len], UART_BUF_LEN - rx_rev_len, 1000);
            rx_rev_len += temp_len;
        }

        /* 后续用于SDK测试，用户可酌情参考*/
        if (g_uart_sem != NULL && (strstr(rx_rev_data, "\r\n")))
        {
            //处理收到数据事件
            if (gstUartCmdRecv.cmd_execute == 0)
            {
                gstUartCmdRecv.cmd_execute ++;
                
                if (rx_rev_len != 0)
                {
                    if (pstUartCmdRecv->cmd_execute != 0)
                    {             
                        //OC指令参数的格式是否正确
                        char *trai_colon_pz = NULL;

                        trai_colon_pz = strrchr((const char * )rx_rev_data, ':');

                        if ((trai_colon_pz != NULL) && ((*(trai_colon_pz + 1) == '\0') || (*(trai_colon_pz + 1) == '\r') || (*(trai_colon_pz + 1) == '\n')))
                        {
                            cm_demo_printf("formate error\n");
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                            pstUartCmdRecv->cmd_execute = 0;
                            continue;
                        }

                        //解析命令，如果未检测到正确的命令格式，则丢弃本次数据
                        if (__cm_cmd_engine("CM", rx_rev_data, &rx_rev_len, (char **)(pstUartCmdRecv->buf), &pstUartCmdRecv->len) == -1)
                        {
                            cm_demo_printf("CM CMD error\n");
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                            pstUartCmdRecv->cmd_execute = 0;
                        }
                        else
                        {
                            //清空数据缓冲，并提示主任务开始执行命令
                            memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                            rx_rev_len = 0;
                            osEventFlagsSet(cmd_task_flag, 0x00000001U);
                        }
                    }	
                }
            }
            else
            {
                memset((void*)rx_rev_data, 0, sizeof(rx_rev_data));
                rx_rev_len = 0;
                cm_demo_printf("Uart busy\n");
            }
        }
    }
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

/****************************************************************************
 * Public Functions
 ****************************************************************************/
 #define S_MAX		8192
static char s[S_MAX] = {0}; //This needs to be large enough to store the string TODO Change magic number
/* 从测试串口打印字符串 */
void cm_demo_printf(char *str, ...)
{
    
    va_list args;
    int len;
    
    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start(args, str);
    len = vsnprintf((char*)s, S_MAX, str, args);
    va_end(args);
    cm_uart_write(OPENCPU_MAIN_URAT, s, len, 1000);
}

/* 若要修改测试串口可在cm_common.h中修改宏定义 */
void cm_demo_uart(void)
{
    int32_t ret = -1;

    /* 配置参数 */
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

    /* 事件参数 */
    cm_uart_event_t event = 
    {
        CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   //注册需要上报的事件类型
        "uart0",                                                        //用户参数
        cm_serial_uart_callback                                         //上报事件的回调函数
    };

    cm_log_printf(0, "uart NUM = %d demo start... ...", OPENCPU_MAIN_URAT);

    /* 若为UART2，需要先将log打印从debug切换到usb */
    if (OPENCPU_MAIN_URAT == CM_UART_DEV_2)
    {
        cm_virt_at_log2cat();
    }

    /* 配置引脚复用 */
    cm_iomux_set_pin_func(OPENCPU_TEST_UARTTX_IOMUX);
    cm_iomux_set_pin_func(OPENCPU_TEST_UARTRX_IOMUX);

    
    /* 若要开启流控，配置流控引脚复用，仅主串口支持流控 */
    if ((config.flow_ctrl == CM_UART_FLOW_CTRL_HW) && (OPENCPU_MAIN_URAT == CM_UART_DEV_0))
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_22, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_23, CM_IOMUX_FUNC_FUNCTION1);
            
        /* 可注册流控事件上报 */
        event.event_type |= CM_UART_EVENT_TYPE_RX_FLOWCTRL;
    }

    /* 注册事件和回调函数 */
    ret = cm_uart_register_event(OPENCPU_MAIN_URAT, &event);
    
    if (ret != RET_SUCCESS)
    {
        cm_log_printf(0, "uart register event err,ret=%d\n", ret);
        return;
    }

    /* 开启串口 */
    ret = cm_uart_open(OPENCPU_MAIN_URAT, &config);
    
    if (ret != RET_SUCCESS)
    {
        cm_log_printf(0, "uart init err,ret=%d\n", ret);
        return;
    }

    /* 配置串口唤醒 */
    /* 只有UART0具有串口唤醒功能 */
    if (OPENCPU_MAIN_URAT == CM_UART_DEV_0)
    {
        /* 配置uart唤醒功能，使能边沿检测才具备唤醒功能，仅主串口具有唤醒功能，用于唤醒的数据并不能被uart接收，请在唤醒后再进行uart数传 */
        cm_iomux_set_pin_cmd(OPENCPU_UART_WEKEUP_PIN, CM_IOMUX_PINCMD1_LPMEDEG, CM_IOMUX_PINCMD1_FUNC1_LPM_EDGE_RISE);
    }

    cm_log_printf(0, "cm_uart_register_event start... ...\n");

    /* 以下为串口接收示例，不影响串口配置，用户可酌情参考 */
    osThreadAttr_t uart_task_attr = {0};
    uart_task_attr.name = "uart_task";
    uart_task_attr.stack_size = 2048;
    uart_task_attr.priority= UART_TASK_PRIORITY;
    gstUartCmdRecv.cmd_execute = 0;
    
    if (g_uart_sem == NULL)
    {
        g_uart_sem = osSemaphoreNew(1, 0, NULL);
    }

    OC_Uart_TaskHandle = osThreadNew(cm_uart_recv_task, 0, &uart_task_attr);

    uart_event_task_create();
}

/* 关闭串口 */
void cm_test_uart_close(char **cmd, int len)
{
    cm_uart_dev_e dev = atoi(cmd[2]);
    
    if (0 == cm_uart_close(dev))
    {
        cm_demo_printf("uart%d close is ok\n", dev);
    }
    
    else
    {
        cm_demo_printf("uart%d close is error\n", dev);
    }
}
