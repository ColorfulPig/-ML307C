/**
 * @file        cm_api_ntp.c
 * @brief       SNTP协议客户端接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By cmiot3000
 * @date        2021/07/23
 *
 * @defgroup ntp ntp
 * @ingroup DS
 * @{
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cm_os.h"
#include "cm_ntp.h"
#include "cm_asocket.h"
#include "cm_eloop.h"
#include "cm_async_dns.h"
#include "cm_rtc.h"
#include "cm_sys.h"
#include <errno.h>

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
#define CM_TICK_PERIOD_MS                                   (5)         //操作系统1 tick对于的时间（ms），不同平台需适配此宏

#define SERVER_ADDR_LEN_MAX_SIZE                            (128)       //服务器地址最大长度

/* SNTP protocol defines */
#define SNTP_MSG_LEN                48
#define SNTP_LI_NO_WARNING          0x00
#define SNTP_VERSION                3 /* NTP Version */
#define SNTP_MODE_MASK              0x07
#define SNTP_MODE_CLIENT            0x03
#define SNTP_MODE_SERVER            0x04
#define SNTP_MODE_BROADCAST         0x05
#define SNTP_STRATUM_KOD            0x00
/* number of seconds between 1900 and 1970 */
#define DIFF_SEC_1900_1970         (2208988800UL)
/* number of seconds between 1970 and Feb 7, 2036 (6:28:16 UTC) (MSB=0) */
#define DIFF_SEC_1970_2036         (2085978496UL)

#define CM_NTP_MESSAGE_NULL         (0)         /* 无意义 */
#define CM_NTP_MESSAGE_TIMEOUT      (1)         /* 超时 */

struct l_tm {
    int tm_sec; /* 秒 – 取值区间为[0,59] */
    int tm_min; /* 分 - 取值区间为[0,59] */
    int tm_hour; /* 时 - 取值区间为[0,23] */
    int tm_mday; /* 一个月中的日期 - 取值区间为[1,31] */
    int tm_mon; /* 月份（从一月开始，0代表一月） - 取值区间为[0,11] */
    int tm_year; /* 年份，其值等于实际年份减去1900 */
};

typedef struct
{
    uint8_t li_vn_mode;      // Eight bits. li, vn, and mode.
                             // li.	Two bits.	Leap indicator.
                             // vn.	Three bits. Version number of the protocol.
                             // mode. Three bits. Client will pick mode 3 for client.

    uint8_t stratum;         // Eight bits. Stratum level of the local clock.
    uint8_t poll;            // Eight bits. Maximum interval between successive messages.
    uint8_t precision;       // Eight bits. Precision of the local clock.

    uint32_t rootDelay;      // 32 bits. Total round trip delay time.
    uint32_t rootDispersion; // 32 bits. Max error aloud from primary clock source.
    uint32_t refId;          // 32 bits. Reference clock identifier.

    uint32_t refTm_s;        // 32 bits. Reference time-stamp seconds.
    uint32_t refTm_f;        // 32 bits. Reference time-stamp fraction of a second.

    uint32_t origTm_s;       // 32 bits. Originate time-stamp seconds.
    uint32_t origTm_f;       // 32 bits. Originate time-stamp fraction of a second.

    uint32_t rxTm_s;         // 32 bits. Received time-stamp seconds.
    uint32_t rxTm_f;         // 32 bits. Received time-stamp fraction of a second.

    uint32_t txTm_s;         // 32 bits and the most important field the client cares about. Transmit time-stamp seconds.
    uint32_t txTm_f;         // 32 bits. Transmit time-stamp fraction of a second.
} cm_ntp_packet;			   // Total: 384 bits or 48 bytes.

typedef struct _cm_ntp_cfg
{
    int32_t sockfd;
    osTimerId_t timer;
    bool work_flag;
    char addr[SERVER_ADDR_LEN_MAX_SIZE + 1];
    uint16_t port;
    uint32_t dns_priority;
    uint32_t timeout;
    bool set_rtc;
    cm_ntp_event_cb cb;
    void *cb_param;
} cm_ntp_cfg_t;

static cm_ntp_cfg_t ntp_cfg = {-1, NULL, 0, "cn.ntp.org.cn", 123, 0, 3000, 1, NULL, NULL};
static char sntp_ip[SERVER_ADDR_LEN_MAX_SIZE] = "";
static const char days[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
static osThreadId_t NtpTaskHandle;                          /* NTP管理句柄 */
static osMessageQueueId_t NtpMessageQueue;                  /* NTP信消息队列 */

/* 时间戳转日期时间*/
static void __cm_rtc_timestamp_to_date(long time,struct l_tm *t)
{
    unsigned int Pass4year;
    int hours_per_year;
    
    if(time < 0)
    {
        time = 0;
    }
    t->tm_sec = (int)(time % 60);
    time /= 60;
    t->tm_min =(int)(time % 60);
    time /= 60;
    Pass4year =((unsigned int)time / (1461L * 24L));
    t->tm_year =(Pass4year << 2) + 1970;
    time %= 1461L * 24L;
    for (;;)
    {
        hours_per_year = 365 * 24;
        if ((t->tm_year & 3) == 0)
            hours_per_year += 24;
        if (time < hours_per_year)
            break;
        t->tm_year++;
        time -= hours_per_year;
    }
    t->tm_hour = (int)(time % 24);
    time /= 24;
    time++;
    if((t->tm_year & 3) == 0)
    {
        if (time > 60)
            time--;
        else
            if (time == 60)
            {
                t->tm_mon = 1;
                t->tm_mday = 29;
                return ;
            }
    }
    for (t->tm_mon = 0; days[t->tm_mon] < time;t->tm_mon++)
        time -= days[t->tm_mon];
    t->tm_mday = (int)(time);
}

/* 执行环境为eloop线程(和异步socket在同一个线程,可以安全地处理socket创建和数据收发等操作) */
static void __on_eloop_ntp_close_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    int sock = (int)cb_param;

    cm_log_printf(0, "__on_eloop_ntp_close_recv_event sock=%d\n", sock);

    /* 处理命令 */
    int ret = cm_asocket_close(sock);
    if (0 == ret)
    {
        cm_log_printf(0, "sock(%d) close success\n", sock);
    }
    else
    {
        cm_log_printf(0, "sock(%d) close error(%d)\n", sock, errno);
    }
    ntp_cfg.work_flag = 0;
    ntp_cfg.sockfd = -1;
}

static void __cm_api_ntp_notify(cm_ntp_event_e event, char* data)
{
    cm_log_printf(0, "[NTP]__cm_api_ntp_notify() event[%d] sockfd[%d]\n", event, ntp_cfg.sockfd);
    if (ntp_cfg.timer)
    {
        osTimerStop(ntp_cfg.timer);
        osTimerDelete(ntp_cfg.timer);
        ntp_cfg.timer = NULL;
    }

    if (-1 != ntp_cfg.sockfd)
    {
        /* 注册Event(将命令封装成Event发送到eloop执行) */
        cm_eloop_event_handle_t ntp_close_recv_event = cm_eloop_register_event(cm_asocket_eloop(), __on_eloop_ntp_close_recv_event, (void *)(ntp_cfg.sockfd));
        /* 发送到eloop执行 */
        cm_eloop_post_event(ntp_close_recv_event);
    }
    else
    {
        ntp_cfg.work_flag = 0;
    }

    if (ntp_cfg.cb)
    {
        ntp_cfg.cb(event, data, ntp_cfg.cb_param);
    }
}

static void __cm_api_ntp_timer_Handler(void * arg)
{
    int32_t msg = CM_NTP_MESSAGE_TIMEOUT;

    //cm_log_printf(0, "[NTP]timerTimeNtpHandler() timeout\n");

    if (NtpMessageQueue != NULL)//向队列发送数据
    {
        osMessageQueuePut(NtpMessageQueue, &msg, 0, 0);
    }
}

static void __cm_ntp_task(void *param)
{
    int32_t msg = CM_NTP_MESSAGE_NULL;

    while (1)
    {
        if (osMessageQueueGet(NtpMessageQueue, &msg, NULL, osWaitForever) == osOK)
        {
            if (CM_NTP_MESSAGE_TIMEOUT == msg)
            {
                cm_log_printf(0, "[NTP]CM_NTP_MESSAGE_TIMEOUT\n");
                __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
            }
        }
    }
}

static int __cm_api_ntp_process_time(cm_ntp_packet *packet, struct timeval *time)
{
    uint32_t t0_s, t1_s, t2_s, t3_s, real_time_s;
    uint32_t t0_us, t1_us, t2_us, t3_us, real_time_us;
    int deta1, deta2, transmit_time;

    if (((packet->li_vn_mode & SNTP_MODE_MASK) == SNTP_MODE_SERVER) ||
        ((packet->li_vn_mode & SNTP_MODE_MASK) == SNTP_MODE_BROADCAST)) {

        t0_s = ntohl(packet->origTm_s);
        t0_us = ntohl(packet->origTm_f);

        t3_s = cm_rtc_get_current_time();

        /* ASR平台无微秒级时间获取接口，故NTP不校正微秒级 */
        t3_us = 0;

        t1_s =	ntohl(packet->rxTm_s);
        t1_us = ntohl(packet->rxTm_f) / 4295;

        t2_s =	ntohl(packet->txTm_s);
        t2_us = ntohl(packet->txTm_f) / 4295;


        deta1 = (t3_s - t0_s) * 1000000 + (t3_us - t0_us); /* us */
        deta2 = (t2_s - t1_s) * 1000000 + (t2_us - t1_us); /* us */

        transmit_time = (deta1 - deta2) / 2; /* us */

        real_time_s = t2_s + (t2_us + transmit_time) / 1000000;
        real_time_us = (t2_us + transmit_time) % 1000000;

        int is_1900_based = ((real_time_s & 0x80000000) != 0);
        real_time_s = is_1900_based ? (real_time_s - DIFF_SEC_1900_1970) : (real_time_s + DIFF_SEC_1970_2036);

        time->tv_sec = real_time_s;
        time->tv_usec = real_time_us;
    } else {
        cm_log_printf(0, "[NTP]sntp_request: not response frame code");
        return -1;
    }
    return 0;
}
/* 异步socket消息回调(执行环境为eloop线程) */
static void __on_asocket_event(int sock, cm_asocket_event_e event, void *user_param)
{
    switch (event)
    {
    // responses
    case CM_ASOCKET_EV_CONNECT_OK: {
        cm_log_printf(0, "[NTP]sock(%d) connect_ok\n", sock);
        break;
    }
    case CM_ASOCKET_EV_CONNECT_FAIL: {
        cm_log_printf(0, "[NTP]sock(%d) connect_fail\n", sock);
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
        break;
    }

    // indications
    case CM_ASOCKET_EV_RECV_IND: {
        /* 取得获取接收缓存中可读的数据长度 */
        int recv_avail = 0;
        cm_asocket_ioctl(sock, FIONREAD, &recv_avail);

        /* 接收数据 */
        cm_ntp_packet packet;

        //int ret = cm_asocket_recvfrom(sock, &packet, sizeof(packet), 0, NULL, NULL);
        int ret = cm_asocket_recv(sock, &packet, sizeof(packet), 0);
        if (ret > 0)
        {
            cm_log_printf(0, "[NTP]sock(%d) recv_ind: recv_avail=%d, recv_len=%d\n", sock, recv_avail, ret, ret);

            struct timeval ntp_time;

            if (0 != __cm_api_ntp_process_time(&packet, &ntp_time))
            {
                cm_log_printf(0, "[NTP]__cm_lib_ntp_process_time() error\n");
                __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
            }
            else
            {
                char buffer[64] = "";
                struct l_tm t;
                int32_t timezone = cm_rtc_get_timezone();
                __cm_rtc_timestamp_to_date((long)(ntp_time.tv_sec), &t);
                sprintf(buffer, "%04d-%02d-%02dT%02d:%02d:%02d.%03d+%02ld:00", t.tm_year, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec, ntp_time.tv_usec / 1000, timezone);

                if (ntp_cfg.set_rtc)
                {
                    if (-1 == cm_rtc_set_current_time(ntp_time.tv_sec))
                    {
                        cm_log_printf(0, "[NTP]cm_plat_rtc_set_time() error\n");
                        __cm_api_ntp_notify(CM_NTP_EVENT_SETTIME_FAIL, buffer);
                    }
                    else
                    {
                        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_OK, buffer);
                    }
                }
                else
                {
                    __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_OK, buffer);
                }
            }
        }
        else
        {
            cm_log_printf(0, "[NTP]sock(%d) recv_ind ret(%d) error(%d)\n", sock, ret, errno);

            if (ENOTCONN == errno)
            {
                /* Connection closed */
                cm_log_printf(0, "[NTP]sock(%d) recv_ind: Connection closed\n", sock);
            }
            __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
        }
        break;
    }
    case CM_ASOCKET_EV_SEND_IND:
        cm_log_printf(0, "[NTP]sock(%d) send_ind\n", sock);
        break;
    case CM_ASOCKET_EV_ACCEPT_IND:
        cm_log_printf(0, "[NTP]sock(%d) accept_ind\n", sock);
        break;
    case CM_ASOCKET_EV_ERROR_IND: {
        /* 获取socket错误码 */
        int sock_error = 0;
        socklen_t opt_len = sizeof(sock_error);
        cm_asocket_getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_error, &opt_len);
        cm_log_printf(0, "[NTP]sock(%d) error_ind: sock_error(%d)\n", sock, sock_error);
        if (ECONNABORTED == sock_error)
        {
            /* Connection aborted */
            cm_log_printf(0, "[NTP]sock(%d) error_ind: Connection aborted\n", sock);
        }
        else if (ECONNRESET == sock_error)
        {
            /* Connection reset */
            cm_log_printf(0, "[NTP]sock(%d) error_ind: Connection reset\n", sock);
        }
        else if (ENOTCONN == sock_error)
        {
            /* Connection closed */
            cm_log_printf(0, "[NTP]sock(%d) error_ind: Connection closed\n", sock);
        }
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
        break;
    }
    default:
        break;
    }
}
static void __cm_api_ntp_request_packet(cm_ntp_packet *packet)
{
    memset(packet, 0, sizeof(cm_ntp_packet));
    packet->li_vn_mode = SNTP_LI_NO_WARNING | SNTP_VERSION << 3 | SNTP_MODE_CLIENT;

    packet->txTm_s = htonl(cm_rtc_get_current_time());
    packet->txTm_f = htonl(0);
}

static void __cm_api_ntp_setserverip(const char *server_ip)
{
    int32_t len = (strlen(server_ip) > SERVER_ADDR_LEN_MAX_SIZE) ? SERVER_ADDR_LEN_MAX_SIZE : strlen(server_ip);
    strncpy((char *)sntp_ip, server_ip, len);
    sntp_ip[len] = 0;
}

static int32_t __cm_api_ntp_sync(const cm_async_dns_ip_addr_t *ip_addr)
{
    int type = SOCK_DGRAM;
    int domain = AF_INET;
    int protocol = 17;
    struct sockaddr_in	sock_address;
    struct sockaddr_in6 sock_address_6;

    if(ip_addr->type == CM_ASYNC_DNS_ADDRTYPE_IPV6)
    {
        sock_address_6.sin6_port = htons(ntp_cfg.port);
        sock_address_6.sin6_family = AF_INET6;
        memcpy((uint8_t *)&sock_address_6.sin6_addr, (uint8_t *)&ip_addr->u_addr.sin6_addr, sizeof(struct in6_addr));
        domain = sock_address_6.sin6_family;
    }
    else
    {
        sock_address.sin_port = htons(ntp_cfg.port);
        sock_address.sin_family = AF_INET;
        sock_address.sin_addr.s_addr = ip_addr->u_addr.sin_addr.s_addr;
        domain = sock_address.sin_family;
    }

    /* 处理命令 */
    ntp_cfg.sockfd = cm_asocket_open(domain, type, protocol, __on_asocket_event, NULL);
    if (-1 == ntp_cfg.sockfd)
    {
        cm_log_printf(0, "[NTP]open failed\n");
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
        return -1;
    }
    cm_log_printf(0, "[NTP]sock(%d) open dns_addrtype %d\n", ntp_cfg.sockfd, ip_addr->type);
    cm_log_printf(0, "[NTP]ip %s port %d\n", sntp_ip, ntp_cfg.port);

    cm_ntp_packet packet;
    __cm_api_ntp_request_packet(&packet);

    int32_t ret = -1;

    if(ip_addr->type == CM_ASYNC_DNS_ADDRTYPE_IPV6)
    {
        ret = cm_asocket_sendto(ntp_cfg.sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&sock_address_6,sizeof(sock_address_6));
    }
    else
    {
        ret = cm_asocket_sendto(ntp_cfg.sockfd, &packet, sizeof(packet), 0, (struct sockaddr *)&sock_address,sizeof(sock_address));
    }

    if (ret <= 0) {
        cm_log_printf(0, "[NTP]send err,ret=%d,errno:%d", ret, errno);
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
        return -1;
    }

    return 0;
}

/* 异步DNS事件回调函数(执行环境为eloop线程) */
static void __on_ntp_async_dns_event(int req_id, cm_async_dns_event_e event, void *cb_param,
								 const char *host_name, const cm_async_dns_ip_addr_t *ip_addr)
{
    cm_log_printf(0, "[NTP]__on_ntp_async_dns_event(%d, %d, %s)\n", req_id, event, host_name);

    switch (event)
    {
    case CM_ASYNC_DNS_RESOLVE_OK: {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr->type)
        {
            inet_ntop(AF_INET, &(ip_addr->u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr->type)
        {
            inet_ntop(AF_INET6, &(ip_addr->u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }

        cm_log_printf(0, "[NTP]dns req(%d) ok: %s->%s dns_addrtype %d\n", req_id, host_name, ip_str, ip_addr->type);

        /* 添加NTP异步功能接口 */
        __cm_api_ntp_setserverip(ip_str);
        __cm_api_ntp_sync(ip_addr);

        break;
    }
    case CM_ASYNC_DNS_RESOLVE_FAIL:{
        cm_log_printf(0, "[NTP]dns req(%d) fail: %s\n", req_id, host_name);

        /* 添加NTP回调接口，返回错误码 */
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");

        break;
    }
    default:
        break;
    }
}

static void __on_eloop_ntp_DNS_recv_event(cm_eloop_event_handle_t event, void *cb_param)
{
    /* 注销Event */
    cm_eloop_unregister_event(event);

    /* 取得参数 */
    char *host_name = (char *)cb_param;

    cm_log_printf(0, "[NTP]__on_eloop_ntp_DNS_recv_event host_name=%s\n", host_name);

    /* 处理命令 */
    cm_async_dns_ip_addr_t ip_addr;

    /* 获取DNS设置 */
    cm_async_dns_addrtype_e dns_addrtype = CM_ASYNC_DNS_ADDRTYPE_IPV4_IPV6;
    if (1 == ntp_cfg.dns_priority)
    {
        dns_addrtype = CM_ASYNC_DNS_ADDRTYPE_IPV4_IPV6;
    }
    else if (2 == ntp_cfg.dns_priority)
    {
        dns_addrtype = CM_ASYNC_DNS_ADDRTYPE_IPV6_IPV4;
    }
    else
    {
        cm_async_dns_addrtype_e cm_async_dns_get_type_by_priority(void);
        dns_addrtype = cm_async_dns_get_type_by_priority();
    }

    cm_log_printf(0, "%s: dns_addrtype %d\n", __func__, dns_addrtype);

    int ret = cm_async_dns_request(host_name, dns_addrtype, &ip_addr, __on_ntp_async_dns_event, NULL);
    if (0 == ret)
    {
        char ip_str[128] = "";
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr.type)
        {
            inet_ntop(AF_INET, &(ip_addr.u_addr.sin_addr), ip_str, sizeof(ip_str));
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr.type)
        {
            inet_ntop(AF_INET6, &(ip_addr.u_addr.sin6_addr), ip_str, sizeof(ip_str));
        }

        cm_log_printf(0, "[NTP]dns req ok: %s->%s dns_addrtype %d\n", host_name, ip_str, ip_addr.type);

        /* 添加NTP异步功能接口 */
        __cm_api_ntp_setserverip(ip_str);
        __cm_api_ntp_sync(&ip_addr);
    }
    else if (ret > 0)
    {
        cm_log_printf(0, "[NTP]dns req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0, "[NTP]dns req fail\n");

        /* 添加NTP回调接口，返回错误码 */
        __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
    }
}

/**
 * @brief NTP服务设置参数
 *
 * @param [in] type  设置参数类型
 * @param [in] value 设置参数数值
 *
 * @return
 *	 =	0 - 成功 \n
 *	 = -1 - 失败
 *
 * @details NTP服务进行中不可设置
 */
int32_t cm_ntp_set_cfg(cm_ntp_cfg_type_e type, void *value)
{
    int32_t ret = -1;

    if (ntp_cfg.work_flag)
    {
        cm_log_printf(0, "[NTP]NTP is working\n");
        return ret;
    }

    if (NULL == value)
    {
        cm_log_printf(0, "[NTP]value is NULL\n");
        return ret;
    }

    if (CM_NTP_CFG_SERVER == type)
    {
        int32_t len = strlen((char *)value);

        if (SERVER_ADDR_LEN_MAX_SIZE < len || 0 == len)
        {
            cm_log_printf(0, "[NTP]server len is error\n");
        }
        else
        {
            strncpy(ntp_cfg.addr, (char *)value, len);
            ntp_cfg.addr[len] = 0;
            cm_log_printf(0, "[NTP]cm_ntp_set_cfg() server %s\n", ntp_cfg.addr);
            ret = 0;
        }
    }
    else if (CM_NTP_CFG_PORT == type)
    {
        ntp_cfg.port = *(uint16_t *)value;
        cm_log_printf(0, "[NTP]cm_ntp_set_cfg() port %d\n", ntp_cfg.port);
        ret = 0;
    }
    else if (CM_NTP_CFG_TIMEOUT == type)
    {
        if (1000 > *(uint32_t *)value || 10000 < *(uint32_t *)value)
        {
            cm_log_printf(0, "[NTP]TIMEOUT is error %d\n", *(uint32_t *)value);
        }
        else
        {
            ntp_cfg.timeout = *(uint32_t *)value;
            cm_log_printf(0, "[NTP]cm_ntp_set_cfg() timeout %d\n", ntp_cfg.timeout);
            ret = 0;
        }
    }
    else if (CM_NTP_CFG_DNS == type)
    {
        if (2 < *(uint32_t *)value)
        {
            cm_log_printf(0, "[NTP]dns_priority is error %d\n", *(uint32_t *)value);
        }
        else
        {
            ntp_cfg.dns_priority = *(uint32_t *)value;
            cm_log_printf(0, "[NTP]cm_ntp_set_cfg() dns_priority %d\n", ntp_cfg.dns_priority);
            ret = 0;
        }
    }
    else if (CM_NTP_CFG_SET_RTC == type)
    {
        if (0 == *(bool *)value)
        {
            ntp_cfg.set_rtc = 0;
        }
        else
        {
            ntp_cfg.set_rtc = 1;
        }
        cm_log_printf(0, "[NTP]cm_ntp_set_cfg() set_rtc %d\n", ntp_cfg.set_rtc);
        ret = 0;
    }
    else if (CM_NTP_CFG_CB == type)
    {
        ntp_cfg.cb = (cm_ntp_event_cb)value;
        ret = 0;
    }
    else if (CM_NTP_CFG_CB_PARAM == type)
    {
        ntp_cfg.cb_param = value;
        ret = 0;
    }
    else
    {
        cm_log_printf(0, "[NTP]type is error %d\n", type);
    }

    return ret;
}

/**
 * @brief 执行NTP时间同步
 *
 * @return 
 *	 = 0  - 成功 \n
 *	 < 0  - 错误码
 *
 * @details NTP模块底层依赖asocket、async_dns和eloop模块，使用NTP功能时应先初始化前述模块
 */
int32_t cm_ntp_sync(void)
{
    if (ntp_cfg.work_flag)
    {
        cm_log_printf(0, "[NTP]NTP is working\n");
        return -1;
    }

    cm_log_printf(0, "[NTP]cm_ntp_sync host_name=%s\n", ntp_cfg.addr);	
    ntp_cfg.work_flag = 1;

    /* 注册Event(将命令封装成Event发送到eloop执行) */
    cm_eloop_event_handle_t ntp_DNS_recv_event = cm_eloop_register_event(cm_asocket_eloop(), __on_eloop_ntp_DNS_recv_event, (void *)ntp_cfg.addr);
    /* 发送到eloop执行 */
    cm_eloop_post_event(ntp_DNS_recv_event);

    if (NULL == NtpMessageQueue)
    {
        NtpMessageQueue = osMessageQueueNew(10, sizeof(int32_t), NULL);

        if (NULL == NtpMessageQueue)
        {
            cm_log_printf(0, "%s() __%d__ osMessageQueueNew() error", __func__, __LINE__);
            __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
            return -1;
        }
    }

    /* 创建线程资源 */
    if (NULL == NtpTaskHandle)
    {
        osThreadAttr_t ntp_task_attr = {0};
        ntp_task_attr.name = "ntp_task";
        ntp_task_attr.stack_size = 1024;
        ntp_task_attr.priority= osPriorityNormal;

        NtpTaskHandle = osThreadNew(__cm_ntp_task, 0, &ntp_task_attr);

        if (NULL == NtpTaskHandle)
        {
            cm_log_printf(0, "%s() __%d__ osThreadNew() error", __func__, __LINE__);
            __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
            return -1;
        }
    }

    if (NULL == ntp_cfg.timer)
    {
        ntp_cfg.timer = osTimerNew(__cm_api_ntp_timer_Handler, osTimerOnce, 0 ,0);

        if (NULL == ntp_cfg.timer)
        {
            cm_log_printf(0, "[NTP]Timer create fail\n");
            __cm_api_ntp_notify(CM_NTP_EVENT_SYNC_FAIL, "");
            return -1;
        }
    }

    osTimerStart(ntp_cfg.timer, ntp_cfg.timeout / CM_TICK_PERIOD_MS);

    return 0;
}
