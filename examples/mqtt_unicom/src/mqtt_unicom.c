/**
 * @file        mqtt.c
 * @brief       使用mqtt连接unicom雁飞平台物模型并将gnss数据上报物模型，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by shimingrui
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "mqtt_unicom.h"
#include "cm_mqtt.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cJSON.h"
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/hmac.h>

#include "cm_pm.h"
#include "cm_modem.h"


#define CM_MQTT_CLIENT_MAX          5

/** An enumeration of the PUBLISH flags. */
typedef enum
{
    CM_MQTT_PUBLISH_DUP = 8u,
    CM_MQTT_PUBLISH_QOS_0 = ((0u << 1) & 0x06),
    CM_MQTT_PUBLISH_QOS_1 = ((1u << 1) & 0x06),
    CM_MQTT_PUBLISH_QOS_2 = ((2u << 1) & 0x06),
    CM_MQTT_PUBLISH_QOS_MASK = ((3u << 1) & 0x06),
    CM_MQTT_PUBLISH_RETAIN = 0x01
} cm_mqtt_publish_flags_e;

/* TODO: 替换为自己设备的信息 */
static char *cmmqtt_glob_subtopic = "$sys/${YourproductKey}/${YourdeviceKey}/property/batch_reply";
static char *cmmqtt_glob_pubtopic = "$sys/${YourproductKey}/${YourdeviceKey}/property/batch";
static char cmqtt_glob_cfgserver[129] = "dmp-mqtt.cuiot.cn";
static int cmqtt_glob_cfgport = 1883;
static const uint8_t cmqtt_glob_device_id[129] = "${YouriotId}";
static const uint8_t cmqtt_glob_device_key[129] = "${YourdeviceKey}";
static const uint8_t cmqtt_glob_product_key[129] = "${YourproductKey}";
static char cmqtt_glob_cfgclient_id[129] = "${YouriotId}|${YourproductKey}|0|0|0";
static int cmqtt_glob_cfgkeepAlive = 120;
static char cmqtt_glob_cfguser[129] = "${YourdeviceKey}|${YourproductKey}";
static const uint8_t cmqtt_glob_device_secret[129] = "${YourdeviceSecret}";
static int cmqtt_glob_cfgclean = 1;


static cm_mqtt_client_t *_mqtt_client[CM_MQTT_CLIENT_MAX] = {0};
static uint8_t dev_token[256] = {0};
static uint8_t test_payload[1024]     = {0};
static int32_t conn_flag = 0;
static int32_t sub_flag = 0;
static float latitude = 0;
static float longitude = 0;
static float altitude = 0;

#define DEV_TOKEN_LEN 256

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
static int __mqtt_manager_default_connack_cb(cm_mqtt_client_t* client, int session, cm_mqtt_conn_state_e conn_res)
{

    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , CONNECT: %d\r\n", i, conn_res);

    if (conn_res == 0)
    {
        conn_flag = 1;
    }
    else if (conn_res > 1)
    {
        conn_flag = -1;
    }
           
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
static int __mqtt_manager_default_publish_cb(cm_mqtt_client_t* client, unsigned short msgid, char* topic, int total_len, int payload_len, char* payload)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }

    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , recv: %d,%s,%d,%d\r\n", i, msgid, topic, total_len, payload_len);

    /* 由于测试示例限制打印长度 */
    int printf_size = payload_len > 600 ? 600 : payload_len;
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , recv: %.*s\r\n", i, printf_size, payload);

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
static int __mqtt_manager_default_puback_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , pub_ack: %d,%d\r\n", i, msgid, dup);
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
static int __mqtt_manager_default_pubrec_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , pub_rec: %d,%d\r\n", i, msgid, dup);
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
static int __mqtt_manager_default_pubcomp_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , pub_comp: %d,%d\r\n", i, msgid, dup);
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
static int __mqtt_manager_default_suback_cb(cm_mqtt_client_t* client, unsigned short msgid, int count, int qos[])
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , sub_ack: %d\r\n", i, msgid);
    sub_flag = 1;
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
static int __mqtt_manager_default_unsuback_cb(cm_mqtt_client_t* client, unsigned short msgid)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , unsub_ack: %d\r\n", i, msgid);
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
static int __mqtt_manager_default_pingresp_cb(cm_mqtt_client_t* client, int ret)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , pingrsp: %d\r\n", i, ret);
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
static int __mqtt_manager_default_timeout_cb(cm_mqtt_client_t* client, unsigned short msgid)
{
    int i = 0;
    
    for (; i < CM_MQTT_CLIENT_MAX; i++)
    {
        if (_mqtt_client[i] == client)
        {
            break;
        }
    }
    
    cm_log_printf(0, "\r\n[MQTT]CM MQTT index[%d] , timeout: %d\r\n", i, msgid);
    return 0;
}

static int32_t __tm_sprintf(uint8_t* str, const uint8_t* format, ...)
{
    va_list ap;
    int     ret = 0;

    va_start(ap, format);
    ret = vsprintf((char*)str, (char*)format, ap);
    va_end(ap);

    return ret;
}

/* 计算token */
static int32_t __unicom_dev_token_generate(uint8_t* token, const uint8_t* deviceId, const uint8_t* deviceKey, const uint8_t* productKey, const uint8_t* deviceSecret)
{
    uint8_t sign_buf[128] = {0};
    Hmac    hmac;
    uint8_t tmp[256] = {0};
    uint8_t token_tmp[256] = {0};
    
    __tm_sprintf(tmp, (const uint8_t*)"%s%s%s", deviceId, deviceKey, productKey);

    wc_HmacSetKey(&hmac, SHA256, deviceSecret, strlen((const char *)deviceSecret));

    wc_HmacUpdate(&hmac, tmp, strlen((const char *)tmp));
    wc_HmacFinal(&hmac, sign_buf);
    
    for (int i = 0; i < 32; i++)
    {
        if ((sign_buf[i]>>4) <= 9) 
        {
            token_tmp[i*2] = (sign_buf[i]>>4) + '0';
        }
        else
        {
            token_tmp[i*2] = (sign_buf[i]>>4) + 'a' - 0x0A;
        }

        if ((sign_buf[i]&0x0f) <= 9)
        {
            token_tmp[i*2 + 1] = (sign_buf[i]&0x0f) + '0';
        }
        else
        {
            token_tmp[i*2 + 1] = (sign_buf[i]&0x0f) + 'a' - 0x0A;
        }
        
    }

    memcpy(token, token_tmp, 64);
    return 0;
}

/* JSON组包 */
int32_t __unicom_tm_prop_geolocation_notify(float latitude, float longitude, float altitude)
{
    void *resource = NULL;
    void *structure = (void*)cJSON_CreateObject();
    int32_t ret = 0;

    cJSON* sub    = NULL;

    resource = (void*)cJSON_CreateObject();

    cJSON_AddItemToObject((cJSON*)structure, "altitude", cJSON_CreateNumber(altitude));
    cJSON_AddItemToObject((cJSON*)structure, "latitude", cJSON_CreateNumber(latitude));
    cJSON_AddItemToObject((cJSON*)structure, "longitude", cJSON_CreateNumber(longitude));

    sub = cJSON_CreateObject();
    cJSON_AddItemToObject(sub, "value", structure);
    cJSON_AddStringToObject(sub, "key", "geoLocation");
    cJSON_AddItemToObject(resource, "data", sub);

    memset(test_payload, 0, 1024);
    static int32_t  post_id     = 1;
    
    cJSON*   request     = cJSON_CreateObject();
    uint8_t* str         = NULL;
    uint8_t  temp_id[16] = { 0 };

    __tm_sprintf(temp_id, (const uint8_t*)"%d", post_id++);

    cJSON_AddStringToObject(request, "messageId", (const char* const)temp_id);

    cJSON_AddItemToObject(request, "params", (cJSON*)resource);

    if (NULL != (str = (uint8_t*)cJSON_PrintUnformatted(request))) {
        strcat((char *)test_payload, (const char *)str);
        cm_free(str);
    }
    cJSON_Delete(request);
    cm_log_printf(0, "tm_onejson payload %s\n", test_payload);

    return ret;
}

static int __mqtt_client_init(void)
{
    if(_mqtt_client[0])
    {
        cm_log_printf(0, "\r\n[MQTT]CM MQTT CLIENT IS RUN!!!\r\n");
        return -1;
    }
    
    _mqtt_client[0] = cm_mqtt_client_create();  //client初始化，最大支持五个实例
    
    if (NULL == _mqtt_client[0])
    {
        cm_log_printf(0, "\r\n[MQTT]CM MQTT CREATE CLIENT ERROR!!!\r\n");
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
    int version = 4;//版本3.1.1
    int pkt_timeout = 10;//发送超时10秒
    int reconn_times = 3;//重连三次
    int reconn_cycle = 20;//重连间隔20秒
    int reconn_mode = 0;//以固定间隔尝试重连
    int retry_times = 3;//重传三次
    int ping_cycle = 60;//ping周期60秒
    int dns_priority = 1;//MQTT dns解析ipv4优先

    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_EVENT, (void*)&callback);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_VERSION, (void*)&version);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_PKT_TIMEOUT, (void*)&pkt_timeout);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_RETRY_TIMES, (void*)&retry_times);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_RECONN_MODE, (void*)&reconn_mode);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_RECONN_TIMES, (void*)&reconn_times);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_RECONN_CYCLE, (void*)&reconn_cycle);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_PING_CYCLE, (void*)&ping_cycle);
    cm_mqtt_client_set_opt(_mqtt_client[0], CM_MQTT_OPT_DNS_PRIORITY, (void*)&dns_priority);
    return 0;
}

static void cm_onenet_mqtt_tm_test(void)
{
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
    
    uint32_t pup_count = 0;
    
    /* mqtt 初始化 */
    __mqtt_client_init();

    /* 计算token */
    memset(dev_token, 0, sizeof(dev_token));
    cm_log_printf(0, "dev_token_generate ret %d\n", __unicom_dev_token_generate(dev_token, cmqtt_glob_device_id, cmqtt_glob_device_key, cmqtt_glob_product_key, cmqtt_glob_device_secret));
    cm_log_printf(0, "dev_token_generate token %s\n", dev_token);

    cm_mqtt_connect_options_t conn_options = {
        .hostport = (unsigned short)cmqtt_glob_cfgport,
        .hostname = (const char*)cmqtt_glob_cfgserver,
        .clientid = (const char*)cmqtt_glob_cfgclient_id,
        .username = (const char*)cmqtt_glob_cfguser,
        .password = (const char*)dev_token,
        .keepalive = (unsigned short)cmqtt_glob_cfgkeepAlive,
        .will_topic = NULL,
        .will_message = NULL,
        .will_message_len = 0,
        .will_flag = 0, //若要使用遗嘱机制请置1，并补充相关遗嘱信息
        .clean_session = (char)cmqtt_glob_cfgclean,
        };

    conn_flag = 0;
    /* mqtt连接 */
    cm_mqtt_client_connect(_mqtt_client[0], &conn_options);//连接

    /* 等待mqtt连接成功 */
    while (!conn_flag)
    {
        osDelay(1);
    }
    if (conn_flag != 1)
    {
        cm_log_printf(0, "\r\n[MQTT]CM MQTT conn err\n");
        return;
    }

    char *topic_tmp[1] = {0};
    topic_tmp[0] = cmmqtt_glob_subtopic;
    char qos_tmp[1] = {0};
    qos_tmp[0] = 0;

    sub_flag = 0;
    
    /* 订阅mqtt topic   */
    int ret = cm_mqtt_client_subscribe(_mqtt_client[0], (const char**)topic_tmp, qos_tmp, 1);
    
    if (0 > ret)
    {
        cm_log_printf(0, "\r\n[MQTT]CM MQTT subscribe ERROR!!!, ret = %d\r\n", ret);
    }

    /* 等待mqtt订阅成功 */
    while (!sub_flag)
    {
        osDelay(1);
    }

    while (pup_count < 10)
    {
        /* 获取gnss定位数据 此处使用模拟获取的定位数据，可以使用带定位功能的模组或者外接定位模块来获取真实的定位数据并在此处替换 */
        latitude = 30.34;
        longitude = 30.34;
        altitude = 30.34;
        
        /* 对定位数据进行JSON组包 */
        __unicom_tm_prop_geolocation_notify(latitude, longitude, altitude);

        uint8_t publish_flags = 0;
        publish_flags |= CM_MQTT_PUBLISH_QOS_1;
        char *pubmessage = (char *)test_payload;

        /* 上报数据给物模型 */
        int ret = cm_mqtt_client_publish(_mqtt_client[0], cmmqtt_glob_pubtopic, pubmessage, strlen(pubmessage), publish_flags);
        
        if (0 >= ret)
        {
            cm_log_printf(0, "\r\n[MQTT]CM MQTT publish ERROR!!!, ret = %d\r\n", ret);
        }

        pup_count++;

        osDelay(1000);
    }

    /* 断开连接 */
    cm_mqtt_client_disconnect(_mqtt_client[0]);
    while (conn_flag != -1)
    {
        osDelay(1);
    }

    /* 释放资源 */
    cm_mqtt_client_destroy(_mqtt_client[0]);
    
    _mqtt_client[0] = NULL;
    conn_flag = 0;
    sub_flag = 0;

    cm_log_printf(0, "\r\n[MQTT]CM MQTT test end\n");
}

/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_onenet_mqtt_tm_test, 0, &app_task_attr);
    return 0;
}
