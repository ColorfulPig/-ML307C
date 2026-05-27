/**
 * @file        cm_mqtt_demo.c
 * @brief       本示例使用MQTT完成连接、订阅topic、推送数据、断开操作
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
#include "cm_mqtt.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define CM_MQTT_DEMO_HOST "broker.emqx.io"                                       // MQTT测试服务器地址，支持域名，demo使用的是一个公有MQTT服务器，网址：https://www.emqx.com/zh/mqtt/public-mqtt5-broker
#define CM_MQTT_DEMO_PORT 1883                                                   // MQTT测试服务器端口
#define CM_MQTT_DEMO_CLIENTID "MQTT_DEMO_CLIENTID_123"                           // MQTT测试客户端ID,对于同一服务器，clientid需要唯一，否则会互相挤下线
#define CM_MQTT_DEMO_USERNAME "MQTT_DEMO_USERNAME_456"                           // MQTT测试用户名
#define CM_MQTT_DEMO_PASSWORD "MQTT_DMEO_PASSWORD_789"                           // MQTT测试用户密码
#define CM_MQTT_DEMO_KEEPALIVE 120                                               // MQTT测试KEEPALIVE心跳间隔
#define CM_MQTT_DEMO_CLEANSESSION 0                                              // MQTT测试cleansession是否清除会话
#define CM_MQTT_DEMO_SUB_TOPIC "/dev/demo/mqtt/MQTT_DEMO_CLIENTID_123/sub_topic" // MQTT接收订阅topic demo仅订阅一个  可自行修改代码 订阅多个
#define CM_MQTT_DEMO_PUB_TOPIC "/dev/demo/mqtt/MQTT_DEMO_CLIENTID_123/pub_topic" // MQTT推送topic

#define MQTT_CONNECTTED_BIT 0x00000001U    // MQTT连接标志
#define MQTT_DISCONNECTTED_BIT 0x00000002U // MQTT断开标志

#define CM_MQTT_CLIENT_MAX 5 // MQTT通路通路最大5路，勿更改
/****************************************************************************
 * Public Types
 ****************************************************************************/
static osThreadId_t CM_MQTT_DEMO_MAIN_TASKHANDLE = NULL;

static osEventFlagsId_t s_mqtt_state_eventflag = NULL;                                // MQTT状态事件组
static cm_mqtt_client_t *s_mqtt_client_id[CM_MQTT_CLIENT_MAX] = {0};                 // MQTT 的number,本DEMO创建一路MQTT
static cm_mqtt_conn_state_e s_mqtt_client_state = CM_MQTT_CONN_STATE_CLIENT_SHUTDOWN; // MQTT 的连接状态
static uint8_t s_mqtt_client_sub_flag = 0;                                            // MQTT 的订阅状态
static uint8_t s_mqtt_client_pub_flag = 0;                                            // MQTT 的推送状态
/**
 *  \brief 连接状态回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] session session标志
 *  \param [in] conn_res 连接状态
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_connack_cb(cm_mqtt_client_t *client, int session, cm_mqtt_conn_state_e conn_res)
{

    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    s_mqtt_client_state = conn_res;
    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , CONNECT: %d\n", i, conn_res);

    return 0;
}

/**
 *  \brief server->client发布消息回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \param [in] topic 主题
 *  \param [in] payload 负载
 *  \param [in] payload_len 负载长度
 *  \param [in] total_len 负载总长度
 *  \return 成功返回0，失败返回-1
 *
 *  \details
 */
static int __mqtt_manager_default_publish_cb(cm_mqtt_client_t *client, unsigned short msgid, char *topic, int total_len, int payload_len, char *payload)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , recv: %d,%s,%d,%d\n", i, msgid, topic, total_len, payload_len);

    /* 由于测试示例限制打印长度 */
    int printf_size = payload_len > 600 ? 600 : payload_len;
    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , recv: %.*s\n", i, printf_size, payload);

    return 0;
}

/**
 *  \brief client->server发布消息ack回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \param [in] dup dup标志
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_puback_cb(cm_mqtt_client_t *client, unsigned short msgid, char dup)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }
    
    s_mqtt_client_pub_flag = 1;
    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , pub_ack: %d,%d\n", i, msgid, dup);
    return 0;
}

/**
 *  \brief client->server发布消息recv回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \param [in] dup dup标志
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_pubrec_cb(cm_mqtt_client_t *client, unsigned short msgid, char dup)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , pub_rec: %d,%d\n", i, msgid, dup);
    return 0;
}

/**
 *  \brief client->server发布消息comp回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \param [in] dup dup标志
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_pubcomp_cb(cm_mqtt_client_t *client, unsigned short msgid, char dup)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , pub_comp: %d,%d\n", i, msgid, dup);
    return 0;
}

/**
 *  \brief 订阅ack回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_suback_cb(cm_mqtt_client_t *client, unsigned short msgid, int count, int qos[])
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    s_mqtt_client_sub_flag = 1;
    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , sub_ack: %d\n", i, msgid);
    return 0;
}

/**
 *  \brief 取消订阅ack回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_unsuback_cb(cm_mqtt_client_t *client, unsigned short msgid)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , unsub_ack: %d\n", i, msgid);
    return 0;
}

/**
 *  \brief ping回调
 *
 *  \param [in] client mqtt客户端
 *  \param [in] ret 消息状态，0：ping成功，1：ping超时
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_pingresp_cb(cm_mqtt_client_t *client, int ret)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , pingrsp: %d\n", i, ret);
    return 0;
}

/**
 *  \brief 消息超时回调，包括publish/subscribe/unsubscribe等
 *
 *  \param [in] client mqtt客户端
 *  \param [in] msgid 消息ID
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int __mqtt_manager_default_timeout_cb(cm_mqtt_client_t *client, unsigned short msgid)
{
    int i = 0;

    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (s_mqtt_client_id[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "[MQTT]CM MQTT index[%d] , timeout: %d\n", i, msgid);
    return 0;
}

/**
 *  \brief MQTT去初始化
 *
 *  \param
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int cm_mqtt_demo_deinit(void)
{
    if(cm_mqtt_client_disconnect(s_mqtt_client_id[0]) < 0 )
    {
        cm_log_printf(0,"[MQTT]CM MQTT DISCONNECT CLIENT FAILED!!!\n");
        return -1;
    }
    
    uint16_t timeout = 60 * 100; // 连接超时时间60s,100ms检查一次是否连接成功
    for (uint16_t i = 0; i < timeout; i++)
    {
        if (s_mqtt_client_state == CM_MQTT_CONN_STATE_CLIENT_SHUTDOWN)
        {
            break;
        }
        osDelay(100 / 5);
    }
    if (s_mqtt_client_state != CM_MQTT_CONN_STATE_CLIENT_SHUTDOWN)
    {
        cm_log_printf(0, "[MQTT]CM MQTT DISCONNECT CLIENT FAILED!!!\n");
        return -1;
    }

    cm_log_printf(0, "[MQTT]CM MQTT DISCONNECT CLIENT SUCCESS!!!\n");

    cm_mqtt_client_destroy(s_mqtt_client_id[0]);
    if(cm_mqtt_client_destroy(s_mqtt_client_id[0]) < 0 )
    {
        cm_log_printf(0,"[MQTT]CM MQTT DESTROY CLIENT FAILED!!!\n");
        return -1;
    }
    
    cm_log_printf(0,"[MQTT]CM MQTT DECLIENT SUCCESS!!!\n");

    return 0;
}
/**
 *  \brief MQTT发布topic
 *
 *  \param
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int cm_mqtt_demo_publish_topic(void)
{
    uint8_t publish_flags = 0;
    publish_flags = CM_MQTT_QOS_1;
    char test_msg[] = "hello world\r\n";
    int ret = cm_mqtt_client_publish(s_mqtt_client_id[0], CM_MQTT_DEMO_PUB_TOPIC, test_msg, strlen(test_msg), publish_flags);

    if (ret < 0)
    {
        cm_log_printf(0, "[MQTT]CM MQTT PUBLISH ERROR!!!, ret = %d\n", ret);
        return -1;
    }
    
    s_mqtt_client_pub_flag = 0;
    uint16_t timeout = 60 * 100; // 订阅超时时间60s,100ms检查一次是否连接成功
    for (uint16_t i = 0; i < timeout; i++)
    {
        if (s_mqtt_client_pub_flag)
        {
            break;
        }
        osDelay(100 / 5);
    }
    if (!s_mqtt_client_pub_flag)
    {
        cm_log_printf(0, "[MQTT]CM MQTT CLIENT PUBLISH FAILED!!!\n");
        return -1;
    }

    cm_log_printf(0, "[MQTT]CM MQTT CLIENT PUBLISH SUCCESS!!!\n");

    return 0;
}
/**
 *  \brief MQTT订阅topic
 *
 *  \param
 *  \return 成功返回0，失败返回-1
 *
 *  \details More details
 */
static int cm_mqtt_demo_subscribe_topic(void)
{
    char *topic_tmp[1] = {0};
    topic_tmp[0] = CM_MQTT_DEMO_SUB_TOPIC;
    char qos_tmp[1] = {0};
    qos_tmp[0] = 0;
    int ret = cm_mqtt_client_subscribe(s_mqtt_client_id[0], (const char **)topic_tmp, qos_tmp, 1);

    if (ret < 0)
    {
        cm_log_printf(0, "[MQTT]CM MQTT SUBSCRIBE ERROR!!!, ret = %d\n", ret);
        return -1;
    }
    
    s_mqtt_client_sub_flag = 0;
    uint16_t timeout = 60 * 100; // 订阅超时时间60s,100ms检查一次是否连接成功
    for (uint16_t i = 0; i < timeout; i++)
    {
        if (s_mqtt_client_sub_flag)
        {
            break;
        }
        osDelay(100 / 5);
    }
    if (!s_mqtt_client_sub_flag)
    {
        cm_log_printf(0, "[MQTT]CM MQTT CLIENT SUBSCRIBE FAILED!!!\n");
        return -1;
    }

    cm_log_printf(0, "[MQTT]CM MQTT CLIENT SUBSCRIBE SUCCESS!!!\n");

    return 0;
}

/**
 *  \brief MQTT初始化
 *
 *  \param
 *  \return 成功返回0，失败返回-1
 *
 *  \details 包含注册MQTT必要的各类回调，配置MQTT的基本参数及登录参数，并等待MQTT返回结果
 */
static int cm_mqtt_demo_client_init(void)
{
    if (s_mqtt_client_id[0])
    {
        cm_log_printf(0, "[MQTT]CM MQTT CLIENT IS RUN!!!\n");
        return -1;
    }

    s_mqtt_client_id[0] = cm_mqtt_client_create(); // client初始化，最大支持五个实例

    if (NULL == s_mqtt_client_id[0])
    {
        cm_log_printf(0, "[MQTT]CM MQTT CREATE CLIENT ERROR!!!\n");
        return -1;
    }

    /* 设置回调函数，连接、订阅、发布等接口均为异步接口，结果请根据回调函数返回进行判断 */
    cm_mqtt_client_cb_t callback = {0};
    callback.connack_cb = __mqtt_manager_default_connack_cb;
    callback.publish_cb = __mqtt_manager_default_publish_cb;
    callback.puback_cb = __mqtt_manager_default_puback_cb;
    callback.pubrec_cb = __mqtt_manager_default_pubrec_cb;
    callback.pubcomp_cb = __mqtt_manager_default_pubcomp_cb;
    callback.suback_cb = __mqtt_manager_default_suback_cb;
    callback.unsuback_cb = __mqtt_manager_default_unsuback_cb;
    callback.pingresp_cb = __mqtt_manager_default_pingresp_cb;
    callback.timeout_cb = __mqtt_manager_default_timeout_cb;

    /* 设置client参数 */
    int version = 4;       // 版本3.1.1
    int pkt_timeout = 10;  // 发送超时10秒
    int reconn_times = 3;  // 重连三次
    int reconn_cycle = 20; // 重连间隔20秒
    int reconn_mode = 0;   // 以固定间隔尝试重连
    int retry_times = 3;   // 重传三次
    int ping_cycle = 60;   // ping周期60秒
    int dns_priority = 2;  // MQTT dns解析ipv6优先

    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_EVENT, (void *)&callback);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_VERSION, (void *)&version);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_PKT_TIMEOUT, (void *)&pkt_timeout);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_RETRY_TIMES, (void *)&retry_times);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_RECONN_MODE, (void *)&reconn_mode);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_RECONN_TIMES, (void *)&reconn_times);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_RECONN_CYCLE, (void *)&reconn_cycle);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_PING_CYCLE, (void *)&ping_cycle);
    cm_mqtt_client_set_opt(s_mqtt_client_id[0], CM_MQTT_OPT_DNS_PRIORITY, (void *)&dns_priority);

    cm_mqtt_connect_options_t conn_options = {
        .hostport = (unsigned short)CM_MQTT_DEMO_PORT,
        .hostname = (const char *)CM_MQTT_DEMO_HOST,
        .clientid = (const char *)CM_MQTT_DEMO_CLIENTID,
        .username = (const char *)CM_MQTT_DEMO_USERNAME,
        .password = (const char *)CM_MQTT_DEMO_PASSWORD,
        .keepalive = (unsigned short)CM_MQTT_DEMO_KEEPALIVE,
        .will_topic = NULL,
        .will_message = NULL,
        .will_message_len = 0,
        .will_flag = 0, // 若要使用遗嘱机制请置1，并补充相关遗嘱信息
        .clean_session = (char)CM_MQTT_DEMO_CLEANSESSION,
    };

    if (cm_mqtt_client_connect(s_mqtt_client_id[0], &conn_options) < 0) // 连接
    {
        cm_log_printf(0, "[MQTT]CM MQTT CONNECT CLIENT ERROR!!!\n");
        return -1;
    }

    uint16_t timeout = 60 * 100; // 连接超时时间60s,100ms检查一次是否连接成功
    for (uint16_t i = 0; i < timeout; i++)
    {
        if (s_mqtt_client_state == CM_MQTT_CONN_STATE_SUCCESS)
        {
            break;
        }
        osDelay(100 / 5);
    }
    if (s_mqtt_client_state != CM_MQTT_CONN_STATE_SUCCESS)
    {
        cm_log_printf(0, "[MQTT]CM MQTT CONNECT CLIENT FAILED!!!\n");
        return -1;
    }

    cm_log_printf(0, "[MQTT]CM MQTT CONNECT CLIENT SUCCESS!!!\n");

    return 0;
}
/**
 *  @brief MQTT主线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 用于初始化连接MQTT，并在1分钟内间隔10s发送6包数据至服务器，并在1分钟内接收数据，最后关闭MQTT
 */
static void cm_mqtt_demo_main(void)
{
    s_mqtt_state_eventflag = osEventFlagsNew(0); // 创建MQTT状态事件组

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

    if (cm_mqtt_demo_client_init() == 0) // 连接mqtt成功
    {
        osEventFlagsSet(s_mqtt_state_eventflag, MQTT_CONNECTTED_BIT);
    }
    else // 连接socket失败
    {
        osEventFlagsSet(s_mqtt_state_eventflag, MQTT_DISCONNECTTED_BIT);
    }

    // osEventFlagsWait获取成功后默认会清除标志位
    uint32_t event_flag = osEventFlagsWait(s_mqtt_state_eventflag, MQTT_CONNECTTED_BIT | MQTT_DISCONNECTTED_BIT, osFlagsWaitAny, osWaitForever);
    if (event_flag & MQTT_CONNECTTED_BIT)
    {
        cm_log_printf(0, "check mqtt has been connected,will send data\n");
        cm_mqtt_demo_subscribe_topic();
        uint8_t retry = 6;
        while (retry--)
        {
            cm_mqtt_demo_publish_topic();
            uint32_t event_flag2 = osEventFlagsWait(s_mqtt_state_eventflag, MQTT_DISCONNECTTED_BIT, osFlagsWaitAny, 10 * 200);
            if (!(event_flag2 & 0x80000000) && (event_flag2 & MQTT_DISCONNECTTED_BIT)) // 若超时则会返回0xffffffff,可判断最高位不能为1
            {
                cm_log_printf(0, "check mqtt has been disconnected,will close mqtt\n");
                break;
            }
            if (retry == 0)
            {
                cm_log_printf(0, "6th send data has been completed, connection will be closed\n");
            }
        }
        cm_mqtt_demo_deinit();
    }
    if (event_flag & MQTT_DISCONNECTTED_BIT)
    {
        cm_log_printf(0, "check mqtt has been disconnected,will close mqtt\n");
        cm_mqtt_demo_deinit();
    }

    while (1)
    {
        osDelay(2000);
    }
}
/**
 *  @brief MQTT接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始MQTT相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*创建MQTT数据接收/发送处理线程*/
    osThreadAttr_t mqtt_main_task_attr = {0};
    mqtt_main_task_attr.name = "mqtt_demo_main_task";
    mqtt_main_task_attr.stack_size = 10 * 1024;
    mqtt_main_task_attr.priority = osPriorityNormal;

    if (CM_MQTT_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_MQTT_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_mqtt_demo_main, 0, &mqtt_main_task_attr);
        if (CM_MQTT_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_MQTT_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    return 0;
}