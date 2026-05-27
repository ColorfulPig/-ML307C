/**
 * @file        asokcet.c
 * @brief       使用，示例仅供参考
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

#include "lwip/sockets.h"
#include "lwip/arch.h"

#include "cm_pm.h"
#include "cm_eloop.h"
#include "cm_async_dns.h"
#include "cm_asocket.h"
#include "cm_modem.h"

#define ASOCKET_TEST_TYPE 0    // 0:TCP 1:UDP
#define ASOCKET_TEST_SERVER "" // asocket测试服务器地址，ip地址或者域名（若是域名，本app优先解析为ipv4（若需要ipv6优先，请修改cm_async_dns_request函数的参数））
#define ASOCKET_TEST_PORT 0    // asocket测试服务器端口

#define ASOCKET_CONNECTTED_BIT 0x00000001U    // asocket连接标志
#define ASOCKET_DISCONNECTTED_BIT 0x00000002U // asocket断开标志

static char s_asocket_test_server_ip[64] = {0}; // asocket测试服务器ip地址（若测试的服务器为域名，则这里存储解析后的ip地址）
static int s_asocket_test_socketid = -1;        // asocket创建的socketid

static osEventFlagsId_t s_asocket_state_eventflag = NULL; // asocket状态事件组

void cm_asocket_start_eloop_cb(cm_eloop_event_handle_t event, void *param);
void cm_asocket_dns_eloop_cb(cm_eloop_event_handle_t event, void *param);
void cm_asocket_connect_eloop_cb(cm_eloop_event_handle_t event, void *param);
void cm_asocket_send_eloop_cb(cm_eloop_event_handle_t event, void *param);
void cm_asocket_close_eloop_cb(cm_eloop_event_handle_t event, void *param);


/**
 * @brief  asocket dns 回调
 *
 * @param [out] req_id 请求ID
 * @param [out] event 事件类型
 * @param [out] cb_param 用户参数(创建socket时指定)
 * @param [out] host_name 域名
 * @param [out] ip_addr IP地址(DNS解析结果)
 *
 * @return  void
 *
 * @details More details
 */
static void __cm_asocket_dns_cb(int req_id, cm_async_dns_event_e event, void *cb_param,
                const char *host_name, const cm_async_dns_ip_addr_t *ip_addr)
{
    cm_log_printf(0,"__on_async_dns_event(%d, %d, %s)", req_id, event, host_name);
    switch (event)
    {
        case CM_ASYNC_DNS_RESOLVE_OK:
        {
            if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr->type)
            {
                inet_ntop(AF_INET, &(ip_addr->u_addr.sin_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
                cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)cb_param);
                cm_eloop_post_event(socket_connect_event);
            }
            else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr->type)
            {
                inet_ntop(AF_INET6, &(ip_addr->u_addr.sin6_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
                cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)cb_param);
                cm_eloop_post_event(socket_connect_event);
            }
            break;
        }
        case CM_ASYNC_DNS_RESOLVE_FAIL:
        {
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * @brief  asocket 回调
 *
 * @param [out] sock socket id
 * @param [out] event 事件
 * @param [out] user_param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
static void __cm_asocket_event_cb(int sock, cm_asocket_event_e event, void *user_param)
{
    cm_log_printf(0,"asocket socket id(%d) event :%d\n", sock,event);
    switch (event)
    {
        // responses
        case CM_ASOCKET_EV_CONNECT_OK: 
        {
            cm_log_printf(0,"asocket socket id(%d) connect_ok\n", sock);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT);
            break;
        }
        case CM_ASOCKET_EV_CONNECT_FAIL: 
        {
            cm_log_printf(0,"sock(%d) connect_fail\n", sock);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
            break;
        }
        // indications
        case CM_ASOCKET_EV_RECV_IND: 
        {
            /* 取得获取接收缓存中可读的数据长度 */
            int recv_avail = 0;
            cm_asocket_ioctl(sock, FIONREAD, &recv_avail);

            /* 接收数据，每次接收数据的最大长度为128字节，大小用户可自行更改 */
            uint8_t recv_buf[128] = {0};
            int ret = cm_asocket_recv(sock, recv_buf, sizeof(recv_buf), 0);
            
            if (ret > 0)
            {
                cm_log_printf(0,"sock(%d) recv_ind: recv_avail=%d, recv_len=%d, data=%.*s\n", sock, recv_avail, ret, ret, recv_buf);
            }
            else
            {
                cm_log_printf(0,"sock(%d) recv_ind error(%d)\n", sock, errno);

                if (ENOTCONN == errno)
                {
                    /* Connection closed */
                    cm_log_printf(0,"sock(%d) recv_ind: Connection closed\n", sock);
                    osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
                }
            }
            
            break;
        }
        case CM_ASOCKET_EV_SEND_IND:
        {
            cm_log_printf(0,"sock(%d) send_ind\n", sock);
            break;
        }    
        case CM_ASOCKET_EV_ACCEPT_IND:
        {
            cm_log_printf(0,"sock(%d) accept_ind\n", sock);
            break;
        }
        case CM_ASOCKET_EV_ERROR_IND: 
        {
            /* 获取socket错误码 */
            int sock_error = 0;
            socklen_t opt_len = sizeof(sock_error);
            
            cm_asocket_getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_error, &opt_len);
            cm_log_printf(0,"sock(%d) error_ind: sock_error(%d)\n", sock, sock_error);
            
            if (ECONNABORTED == sock_error)
            {
                /* Connection aborted */
                cm_log_printf(0,"sock(%d) error_ind: Connection aborted\n", sock);
            }
            
            else if (ECONNRESET == sock_error)
            {
                /* Connection reset */
                cm_log_printf(0,"sock(%d) error_ind: Connection reset\n", sock);
            }
            
            else if (ENOTCONN == sock_error)
            {
                /* Connection closed */
                cm_log_printf(0,"sock(%d) error_ind: Connection closed\n", sock);
            }

            break;
        }
        default:
        {
            break;
        }
    }
}

/**
 * @brief  asocket eloop开始事件回调
 *
 * @param [out] event 事件
 * @param [out] param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_asocket_start_eloop_cb(cm_eloop_event_handle_t event, void *param)
{
    cm_eloop_unregister_event(event);

    struct sockaddr_in6 server_addr6;
    struct sockaddr_in server_addr4;
    //判断服务器地址的类型
    if (inet_pton(AF_INET6, ASOCKET_TEST_SERVER, &server_addr6.sin6_addr))  //ipv6地址 
    {
        cm_log_printf(0,"test asocket server is ipv6 address:%s\n",ASOCKET_TEST_SERVER);
        strcpy(s_asocket_test_server_ip, ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t socket_open_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)param);
        cm_eloop_post_event(socket_open_event);
    }
    else if (inet_pton(AF_INET, ASOCKET_TEST_SERVER, &server_addr4.sin_addr))   //ipv4地址
    {
        cm_log_printf(0,"test asocket server is ipv4 address:%s\n",ASOCKET_TEST_SERVER);
        strcpy(s_asocket_test_server_ip, ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t socket_open_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)param);
        cm_eloop_post_event(socket_open_event);
    }
    else    //域名
    {
        cm_log_printf(0,"test asocket server is domin address:%s\n",ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t dns_event = cm_eloop_register_event(cm_async_dns_eloop(), cm_asocket_dns_eloop_cb, (void *)param);
        cm_eloop_post_event(dns_event);
    }
}

/**
 * @brief  asocket dns eloop事件回调
 *
 * @param [out] event 事件
 * @param [out] param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_asocket_dns_eloop_cb(cm_eloop_event_handle_t event, void *param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    cm_async_dns_ip_addr_t ip_addr;
    int ret = cm_async_dns_request(ASOCKET_TEST_SERVER, CM_ASYNC_DNS_ADDRTYPE_IPV4_IPV6, &ip_addr, __cm_asocket_dns_cb, (void *)param);

    if (0 == ret)
    {
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr.type)
        {
            inet_ntop(AF_INET, &(ip_addr.u_addr.sin_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
            cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, NULL);
            cm_eloop_post_event(socket_connect_event);
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr.type)
        {
            inet_ntop(AF_INET6, &(ip_addr.u_addr.sin6_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
            cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, NULL);
            cm_eloop_post_event(socket_connect_event);
        }
    }
    else if (ret > 0)
    {
        cm_log_printf(0,"dns req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0,"dns req fail %d\n", ret);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
    }
}

/**
 * @brief  asocket eloop连接事件回调
 *
 * @param [out] event 事件
 * @param [out] param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_asocket_connect_eloop_cb(cm_eloop_event_handle_t event, void *param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);
    int ret = 0,type = SOCK_DGRAM,domain = AF_INET;
    struct sockaddr_in6 server_addr6 = {0};
    struct sockaddr_in server_addr4 = {0};

    if(ASOCKET_TEST_TYPE == 0)  //TCP
    {
        type = SOCK_STREAM;
    }

    //判断服务器地址的类型
    if (inet_pton(AF_INET6, s_asocket_test_server_ip, &server_addr6.sin6_addr))  //ipv6地址 
    {
        server_addr6.sin6_port = htons(ASOCKET_TEST_PORT);
        server_addr6.sin6_family = AF_INET6;
        domain = AF_INET6;
    }
    else if (inet_pton(AF_INET, s_asocket_test_server_ip, &server_addr4.sin_addr))   //ipv4地址
    {
        server_addr4.sin_family = AF_INET;
        //server_addr4.sin_addr.s_addr = inet_addr(s_asocket_test_server_ip);
        server_addr4.sin_port = htons(ASOCKET_TEST_PORT);
    }
    else
    {
        cm_log_printf(0,"asocket ip address(%s) is abnormal,will disconnect\n",s_asocket_test_server_ip);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        return ;
    }

    if (0 == ASOCKET_TEST_TYPE) // TCP
    {
        s_asocket_test_socketid = cm_asocket_open(domain, type, IPPROTO_TCP, __cm_asocket_event_cb, (void *)param);
    }
    else // UDP
    {
        s_asocket_test_socketid = cm_asocket_open(domain, type, IPPROTO_UDP, __cm_asocket_event_cb, (void *)param);
    }
    if (s_asocket_test_socketid < 0)
    {
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        return;
    }

    if(domain == AF_INET6)
    {
        ret = cm_asocket_connect(s_asocket_test_socketid, (const struct sockaddr *)&server_addr6, sizeof(server_addr6));
    }
    else
    {
        ret = cm_asocket_connect(s_asocket_test_socketid, (const struct sockaddr *)&server_addr4, sizeof(server_addr4));
    }

    if (ret < 0)
    {
        if (EINPROGRESS == errno)
        {
            cm_log_printf(0,"asocket socket id(%d) open request success, wait connect...\n", s_asocket_test_socketid);
        }
        else
        {
            cm_log_printf(0,"asocket socket id(%d) connect failed(%d)\n", s_asocket_test_socketid, errno);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        }
    }
    else if (0 == ret)
    {
        cm_log_printf(0,"asocket socket id(%d) connect_ok\n", s_asocket_test_socketid);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT);
    }
}

/**
 * @brief  asocket eloop发送数据事件回调
 *
 * @param [out] event 事件
 * @param [out] param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_asocket_send_eloop_cb(cm_eloop_event_handle_t event, void *param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    char *asocket_data = (char *)param;
    if(s_asocket_test_socketid >= 0)
    {
        cm_asocket_send(s_asocket_test_socketid,asocket_data,strlen(asocket_data),0);
    }
}

/**
 * @brief  asocket eloop关闭事件回调
 *
 * @param [out] event 事件
 * @param [out] param 事件参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_asocket_close_eloop_cb(cm_eloop_event_handle_t event, void *param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    if(s_asocket_test_socketid >= 0)
    {
        cm_asocket_close(s_asocket_test_socketid);
        s_asocket_test_socketid = -1;
    }
}


/* asocket测试线程 */
void cm_asocket_task_test(void *p)
{
    s_asocket_state_eventflag = osEventFlagsNew(0);     //创建网络状态事件组

    /* 等待模组PDP激活 */
    int32_t pdp_time_out = 0;
    while(1)
    {
        if(pdp_time_out > 20)
        {
            cm_log_printf(0, "network timeout\n");
            cm_pm_reboot();
        }
        if(cm_modem_get_pdp_state(1) == 1)
        {
            cm_log_printf(0, "network ready\n");
            break;
        }
        osDelay(200);
        pdp_time_out++;
    }

    /* 适当等待（模组PDP激活到能进行数据通信需要适当等待，或者通过PING操作确定模组数据通信是否可用） */
    osDelay(1000);

    /* 向asocket eloop发送连接服务器事件 */
    cm_eloop_event_handle_t asocket_start_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_start_eloop_cb, NULL);
    cm_eloop_post_event(asocket_start_event);


    //发送的数据若通过参数传递，建议使用全局变量，防止内存被释放；
    //这里使用局部变量因在线程函数中，函数不会结束，则内存不会被释放
    char asocket_data[] = "hello world\r\n";    
    cm_eloop_event_handle_t asocket_send_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_send_eloop_cb, asocket_data);
    cm_eloop_event_handle_t asocket_close_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_close_eloop_cb, NULL);

    // osEventFlagsWait获取成功后默认会清除标志位
    uint32_t event_flag = osEventFlagsWait(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT | ASOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, osWaitForever);   
    if(event_flag & ASOCKET_CONNECTTED_BIT)
    {
        cm_log_printf(0, "check asocket has been connected,will send data\n");
        uint8_t retry = 6;
        while(retry --)
        {
            cm_eloop_post_event(asocket_send_event);
            uint32_t event_flag2 = osEventFlagsWait(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, 10 * 200);
            if(!(event_flag2 & 0x80000000)  &&  (event_flag2 & ASOCKET_DISCONNECTTED_BIT))  //若超时则会返回0xffffffff,可判断最高位不能为1
            {
                cm_log_printf(0, "check asocket has been disconnected,will close socket\n");
                break;
            }
            if(retry == 0)
            {
                cm_log_printf(0, "6th send data has been completed, connection will be closed\n");
            }
        }
        cm_eloop_post_event(asocket_close_event);
    }
    if(event_flag & ASOCKET_DISCONNECTTED_BIT)
    {
        cm_log_printf(0, "check asocket has been disconnected,will close socket\n");
        cm_eloop_post_event(asocket_close_event);
    }

    while(1)
    {
        osDelay(2000);
    }
}

/* opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "asocket_test_task";
    app_task_attr.stack_size = 1024 * 6;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_asocket_task_test, 0, &app_task_attr);
    return 0;
}