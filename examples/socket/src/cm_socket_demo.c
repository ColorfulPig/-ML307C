/**
 * @file        cm_socket_demo.c
 * @brief       本示例使用LWIP/SOCKET完成TCP/UDP的连接、发送、接收、断开操作
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

#include "cm_os.h"
#include "cm_sys.h"
#include "cm_modem.h"
#include "cm_pm.h"

#include "lwip/lwip/netdb.h"
#include "lwip/lwip/sockets.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LWIP_SOCKET_TEST_TYPE 0    // 0:TCP 1:UDP
#define LWIP_SOCKET_TEST_SERVER "121.199.36.16" // asocket测试服务器地址，ip地址或者域名
#define LWIP_SOCKET_TEST_PORT 8001    // asocket测试服务器端口

#define LWIP_SOCKET_CONNECTTED_BIT 0x00000001U    // lwip_socket连接标志
#define LWIP_SOCKET_DISCONNECTTED_BIT 0x00000002U // lwip_socket断开标志
/****************************************************************************
 * Public Types
 ****************************************************************************/
static osThreadId_t CM_SOCKET_DEMO_MAIN_TASKHANDLE = NULL;
static osThreadId_t CM_SOCKET_DEMO_RECV_TASKHANDLE = NULL;
static char s_lwip_socket_test_server_ip[64] = {0};           // lwip_socket测试服务器ip地址（若测试的服务器为域名，则这里存储解析后的ip地址）
static osEventFlagsId_t s_lwip_socket_state_eventflag = NULL; // lwip_socket状态事件组
static int g_socket_id = -1;

/* lwip_socket DNS示例 */
static int cm_lwip_socket_dns(char *socket_test_server)
{
    char host_ip[64] = {0};

    struct hostent *host = gethostbyname(LWIP_SOCKET_TEST_SERVER); // 请先补充测试域名

    if (host == NULL)
    {
        cm_log_printf(0,"DNS error\n");
        return -1;
    }

    inet_ntop(host->h_addrtype, host->h_addr_list[0], host_ip, sizeof(host_ip));

    cm_log_printf(0,"host ip:%s\n", host_ip);
    strncpy(socket_test_server, host_ip, strlen(host_ip));
    return 0;
}

/* lwip_socket发送示例 */
static void cm_lwip_socket_send(void)
{
    int ret = 0;
    char test_msg[] = "hello world\r\n";

    ret = send(g_socket_id, (char *)test_msg, strlen(test_msg), 0);
    cm_log_printf(0,"lwip socket send ret %d\n", ret);

    if (ret < 0)
    {
        cm_log_printf(0,"lwip send error\n");
        osEventFlagsSet(s_lwip_socket_state_eventflag, LWIP_SOCKET_DISCONNECTTED_BIT);
    }
}

/*lwip socket接收线程，仅供参考 */
static void cm_lwip_socket_recv_task(void)
{
    int ret = 0;
    fd_set sets;
    struct timeval timeout;
    char buf[128] = {0}; //下行数据接收buffer  示例开辟较小，测试时勿超过128字节

    do
    {
        FD_ZERO(&sets);
        FD_SET(g_socket_id, &sets);

        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        ret = select(g_socket_id + 1, &sets, NULL, NULL, &timeout);

        if (ret > 0)
        {
            ret = recv(g_socket_id, buf, sizeof(buf), 0);

            if (ret > 0)
            {
                cm_log_printf(0, "socket_read_test recv %s\n", buf);
                memset(buf, 0, sizeof(buf));
            }
            else if (ret <= 0)
            {
                /* 走到这说明连接已断开，客户端需要执行close */
                cm_log_printf(0, "socket_read_test closed %d\n", errno);
                osEventFlagsSet(s_lwip_socket_state_eventflag, LWIP_SOCKET_DISCONNECTTED_BIT);
            }
        }
        else if (ret == 0)
        {
            /* 走到这是select等待超时 */
            //可不做任何处理
            //cm_log_printf(0, "socket_read_test select timeout %d\n", errno);
        }
        else
        {
            /* 走到这是当前socket已关闭，此时select将一直报错，故而挂起线程，等待重新连接后再次恢复线程 */
            osThreadSuspend(CM_SOCKET_DEMO_RECV_TASKHANDLE);
        }
    } while (1);
}
/**
 *  @brief socket connect函数
 *
 *  @param
 *
 *  @return
 *
 *  @details
 */
static int cm_lwip_socket_conn(void)
{
    int ret = 0;

    if (LWIP_SOCKET_TEST_TYPE == 0)
    {
        g_socket_id = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    }
    else
    {
        g_socket_id = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    }

    if (g_socket_id == -1)
    {
        cm_log_printf(0, "socket id create error\n");
        return -1;
    }

    cm_log_printf(0, "socket id:%d\n", g_socket_id);

    struct in_addr remote_addr;
    if (inet_aton(LWIP_SOCKET_TEST_SERVER, &remote_addr) > 0) /*判断是IP地址*/
    {
        cm_log_printf(0, "test lwip socket server is ip address\n"); // 直接用IP地址连接
        strcpy(s_lwip_socket_test_server_ip, LWIP_SOCKET_TEST_SERVER);
    }
    else /* 判断是域名，需dns解析*/
    {
        cm_log_printf(0, "test lwip socket server is domain name\n");
        if (cm_lwip_socket_dns(s_lwip_socket_test_server_ip) < 0)
        {
            cm_log_printf(0, "test lwip socket dns failed!");
            return -1;
        }
    }

    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_len = sizeof(server_addr);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(LWIP_SOCKET_TEST_PORT);                   // 请先补充测试端口
    server_addr.sin_addr.s_addr = inet_addr(s_lwip_socket_test_server_ip); // 请先补充测试地址

    ret = connect(g_socket_id, (const struct sockaddr *)&server_addr, sizeof(server_addr));

    if (ret == -1)
    {
        cm_log_printf(0, "socket connect error\n");
        close(g_socket_id);
        return -1;
    }

    cm_log_printf(0, "socket connect succ\n");

    /*创建socket数据接收/发送处理线程*/
    osThreadAttr_t socket_recv_task_attr = {0};
    socket_recv_task_attr.name = "socket_recv_task";
    socket_recv_task_attr.stack_size = 10 * 1024;
    socket_recv_task_attr.priority = osPriorityNormal;

    if (CM_SOCKET_DEMO_RECV_TASKHANDLE == NULL)
    {
        CM_SOCKET_DEMO_RECV_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_lwip_socket_recv_task, 0, &socket_recv_task_attr);
        if (CM_SOCKET_DEMO_RECV_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_SOCKET_DEMO_RECV_TASKHANDLE is NULL");
        }
    }
    else // 再次调用是恢复接收线程
    {
        osThreadResume(CM_SOCKET_DEMO_RECV_TASKHANDLE);
    }

    return 0;
}
/**
 *  @brief socket主线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 用于初始化连接socket，并在1分钟内间隔10s发送6包数据至服务器，并在1分钟内接收数据，最后关闭socket
 */
static void cm_socket_demo_main(void)
{
    s_lwip_socket_state_eventflag = osEventFlagsNew(0); // 创建SOCKET状态事件组

    /* 等待模组PDP激活 */
    int32_t pdp_time_out = 0;
    while (1)
    {
        if (pdp_time_out > 20)
        {
            cm_log_printf(0, "network timeout\n");
            cm_pm_reboot();
        }
        if (cm_modem_get_pdp_state(1) == 1)
        {
            cm_log_printf(0, "network ready\n");
            break;
        }
        osDelay(200);
        pdp_time_out++;
    }

    /* 适当等待（模组PDP激活到能进行数据通信需要适当等待，或者通过PING操作确定模组数据通信是否可用） */
    osDelay(1000 / 5);

    if (cm_lwip_socket_conn() == 0) // 连接socket成功
    {
        osEventFlagsSet(s_lwip_socket_state_eventflag, LWIP_SOCKET_CONNECTTED_BIT);
    }
    else // 连接socket失败
    {
        osEventFlagsSet(s_lwip_socket_state_eventflag, LWIP_SOCKET_DISCONNECTTED_BIT);
    }

    // osEventFlagsWait获取成功后默认会清除标志位
    uint32_t event_flag = osEventFlagsWait(s_lwip_socket_state_eventflag, LWIP_SOCKET_CONNECTTED_BIT | LWIP_SOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, osWaitForever);
    if (event_flag & LWIP_SOCKET_CONNECTTED_BIT)
    {
        cm_log_printf(0, "check lwip socket has been connected,will send data\n");
        uint8_t retry = 6;
        while (retry--)
        {
            cm_lwip_socket_send();
            uint32_t event_flag2 = osEventFlagsWait(s_lwip_socket_state_eventflag, LWIP_SOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, 10 * 200);
            if (!(event_flag2 & 0x80000000) && (event_flag2 & LWIP_SOCKET_DISCONNECTTED_BIT)) // 若超时则会返回0xffffffff,可判断最高位不能为1
            {
                cm_log_printf(0, "check lwip socket has been disconnected,will close socket\n");
                break;
            }
            if (retry == 0)
            {
                cm_log_printf(0, "6th send data has been completed, connection will be closed\n");
            }
        }
        close(g_socket_id);
    }
    if (event_flag & LWIP_SOCKET_DISCONNECTTED_BIT)
    {
        cm_log_printf(0, "check lwip socket has been disconnected,will close socket\n");
        close(g_socket_id);
    }

    while (1)
    {
        osDelay(2000);
    }
}
/**
 *  @brief socket接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始socket相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*创建socket数据接收/发送处理线程*/
    osThreadAttr_t socket_main_task_attr = {0};
    socket_main_task_attr.name = "socket_demo_main_task";
    socket_main_task_attr.stack_size = 10 * 1024;
    socket_main_task_attr.priority = osPriorityNormal;

    if (CM_SOCKET_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_SOCKET_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_socket_demo_main, 0, &socket_main_task_attr);
        if (CM_SOCKET_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_SOCKET_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    return 0;
}