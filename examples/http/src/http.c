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
#include "cm_modem.h"
#include "cm_http.h"

#include "cJSON.h"
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/hmac.h>

#define HTTP_TEST_TYPE 1 // 0:http 1:https

#define HTTP_ONENET_SERVER "http://iot-api.heclouds.com"   // onenet物联网开放平台http测试地址
#define HTTPS_ONENET_SERVER "https://iot-api.heclouds.com" // onenet物联网开放平台https测试地址

#define ONENET_POST_URL "/device/create" // http(s) post示例url测试参数（新增设备）
#define ONENET_GET_URL "/device/detail"  // http(s) get示例url测试参数（查询设备详细信息）

#define ONENET_ET 1924833600        // 到期时间2030-12-30 12:00:00
#define ONENET_PRODUCT_ID ""        // onenet物联网开放平台产品id
#define ONENET_PRODUCT_ACCESSKEY "" // onenet物联网开放平台产品接入密钥

enum sig_method_e
{
    SIG_METHOD_MD5,
    SIG_METHOD_SHA1,
    SIG_METHOD_SHA256
};

#define DEV_TOKEN_LEN 256
#define DEV_TOKEN_VERISON_STR "2022-05-01"

#define DEV_TOKEN_SIG_METHOD_MD5 "md5"
#define DEV_TOKEN_SIG_METHOD_SHA1 "sha1"
#define DEV_TOKEN_SIG_METHOD_SHA256 "sha256"


/* 计算鉴权 */
static int32_t cm_onenet_auth_generate(char* auth, enum sig_method_e method, uint32_t exp_time, const char* product_id, const char* access_key)
{

    uint8_t  base64_data[64] = { 0 };
    uint8_t  str_for_sig[64] = { 0 };
    uint8_t  sign_buf[128]   = { 0 };
    uint32_t base64_data_len = sizeof(base64_data);
    uint8_t* sig_method_str  = NULL;
    uint32_t sign_len        = 0;
    uint32_t i               = 0;
    uint8_t* tmp             = NULL;
    Hmac     hmac;

    sprintf((char *)auth, "version=%s", DEV_TOKEN_VERISON_STR);

    sprintf((char *)(auth + strlen((const char *)auth)), "&res=products%%2F%s", product_id);

    sprintf((char *)(auth + strlen((const char *)auth)), "&et=%lu", exp_time);

    Base64_Decode((const byte *)(access_key), strlen((const char *)access_key), base64_data, (unsigned int *)&base64_data_len);

    if (SIG_METHOD_MD5 == method) {
        wc_HmacSetKey(&hmac, MD5, base64_data, base64_data_len);
        sig_method_str = (uint8_t*)DEV_TOKEN_SIG_METHOD_MD5;
        sign_len       = 16;
    } else if (SIG_METHOD_SHA1 == method) {
        wc_HmacSetKey(&hmac, SHA, base64_data, base64_data_len);
        sig_method_str = (uint8_t*)DEV_TOKEN_SIG_METHOD_SHA1;
        sign_len       = 20;
    } else if (SIG_METHOD_SHA256 == method) {
        wc_HmacSetKey(&hmac, SHA256, base64_data, base64_data_len);
        sig_method_str = (uint8_t*)DEV_TOKEN_SIG_METHOD_SHA256;
        sign_len       = 32;
    }

    sprintf((char *)(auth + strlen((const char *)auth)), "&method=%s", sig_method_str);
    
    sprintf((char *)str_for_sig, "%lu\n%s\nproducts/%s\n%s", exp_time, sig_method_str, product_id, DEV_TOKEN_VERISON_STR);
    
    wc_HmacUpdate(&hmac, str_for_sig, strlen((const char *)str_for_sig));
    wc_HmacFinal(&hmac, sign_buf);

    memset(base64_data, 0, sizeof(base64_data));
    base64_data_len = sizeof(base64_data);
    Base64_Encode_NoNl(sign_buf, sign_len, base64_data, (unsigned int *)&base64_data_len);

    strcat((char *)auth, (const char*)"&sign=");
    tmp = (uint8_t *)(auth + strlen((const char *)auth));

    for (i = 0; i < base64_data_len; i++) {
        switch (base64_data[i]) {
            case '+':
                strcat((char *)tmp, (const char*)"%2B");
                tmp += 3;
                break;
            case ' ':
                strcat((char *)tmp, (const char*)"%20");
                tmp += 3;
                break;
            case '/':
                strcat((char *)tmp, (const char*)"%2F");
                tmp += 3;
                break;
            case '?':
                strcat((char *)tmp, (const char*)"%3F");
                tmp += 3;
                break;
            case '%':
                strcat((char *)tmp, (const char*)"%25");
                tmp += 3;
                break;
            case '#':
                strcat((char *)tmp, (const char*)"%23");
                tmp += 3;
                break;
            case '&':
                strcat((char *)tmp, (const char*)"%26");
                tmp += 3;
                break;
            case '=':
                strcat((char *)tmp, (const char*)"%3D");
                tmp += 3;
                break;
            default:
                *tmp = base64_data[i];
                tmp += 1;
                break;
        }
    }

    return 0;
}

//在onenet物联网平台产品下新增设备（使用模组imei作为设备名称）
int cm_http_post_onenet_reg_dev()
{
    cm_httpclient_handle_t http_client = NULL;
    cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
    cm_httpclient_cfg_t client_cfg = {0};

    char http_header[256] = {0},http_body[128] = {0};

    /* 1. 计算onenet物联网平台创建设备的鉴权信息 */
    char authorization[150] = {0};
    cm_onenet_auth_generate(authorization,SIG_METHOD_MD5,ONENET_ET,ONENET_PRODUCT_ID,ONENET_PRODUCT_ACCESSKEY);
    cm_log_printf(0,"onenet auth:%s",authorization);

    /* 2. 根据http类型创建http实例 */
    if(HTTP_TEST_TYPE == 0)
    {
        ret = cm_httpclient_create((const uint8_t *)HTTP_ONENET_SERVER, NULL, &http_client);
        if (CM_HTTP_RET_CODE_OK != ret)
        {
            cm_log_printf(0,"create http client fail");
            return -1;
        }
        client_cfg.ssl_enable = 0;
        client_cfg.ssl_id = 0;
    }
    else
    {   
        ret = cm_httpclient_create((const uint8_t *)HTTPS_ONENET_SERVER, NULL, &http_client);
        if (CM_HTTP_RET_CODE_OK != ret)
        {
            cm_log_printf(0,"create http client fail");
            return -1;
        }
        client_cfg.ssl_enable = 1;
        client_cfg.ssl_id = 1;
    }

    client_cfg.cid = 0;
    client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
    client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
    client_cfg.dns_priority = 1;

    /* 3. 配置http参数 */
    ret = cm_httpclient_set_cfg(http_client, client_cfg);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_httpclient_delete(http_client);
        cm_log_printf(0,"http client set config fail...");
        return -2;
    }
    
    /* 4. 设置http header */
    int http_headerlen = snprintf(http_header,sizeof(http_header),"Content-type: application/json\r\n");
    http_headerlen += snprintf(http_header + http_headerlen,sizeof(http_header) - http_headerlen,
                                "authorization: %s\r\n",authorization);
    cm_httpclient_custom_header_set(http_client,(uint8_t *)http_header,(uint16_t)http_headerlen);

    /* 5. 封装http body，使用imei作为设备名称 */
    char imei[16] = {0};
    cm_sys_get_imei(imei);
    int http_bodylen = snprintf(http_body,sizeof(http_body),"{\"product_id\": \"%s\",\"device_name\": \"%s\"}",ONENET_PRODUCT_ID,imei);

    /* 6. 发起http同步请求 */
    cm_httpclient_sync_response_t response = {0};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_POST, (const uint8_t *)ONENET_POST_URL,(uint32_t)http_bodylen,(uint8_t *)http_body};
    ret = cm_httpclient_sync_request(http_client, param, &response);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0,"cm_httpclient_sync_request() error! ret is %d\r\n", ret);
        cm_httpclient_custom_header_free(http_client);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -3;
    }

    /* 7. 打印获取到的数据（因cm_log_printf打印长度有限，数据可能打印不完整）  */
    cm_log_printf(0,"http response code is %d\r\n", response.response_code);
    cm_log_printf(0,"http header is %d,%s\r\n", response.response_header_len,response.response_header);
    cm_log_printf(0,"response_content_len is %d,%s\r\n", response.response_content_len,response.response_content);

    /* 8. 解析获取到的数据 */
    cJSON *root = NULL;
    root = cJSON_Parse((const char *)(response.response_content));
    if(root == NULL)
    {
        cm_httpclient_custom_header_free(http_client);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -4;
    }

    cJSON *code = cJSON_GetObjectItem(root, "code");
    if(code == NULL)
    {
        cJSON_Delete(root);
        cm_httpclient_custom_header_free(http_client);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -4;
    }

    if(code->valueint != 0 && code->valueint != 10406)  //0表示注册成功 10406表示设备已存在
    {
        cm_log_printf(0,"http register onenet device(%s) fail,result code:%d",imei,code->valueint);
        cJSON_Delete(root);
        cm_httpclient_custom_header_free(http_client);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -5;
    }
    
    cm_log_printf(0,"http register onenet device(%s) success,result code:%d",imei,code->valueint);
    cJSON_Delete(root);
    cm_httpclient_custom_header_free(http_client);
    cm_httpclient_sync_free_data(http_client);
    cm_httpclient_terminate(http_client);
    cm_httpclient_delete(http_client);
    return 0;
}

//查询onenet物联网平台设备状态（使用模组imei作为设备名称查询）
int cm_http_post_onenet_inquire_dev()
{
     cm_httpclient_handle_t http_client = NULL;
    cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
    cm_httpclient_cfg_t client_cfg = {0};

    char http_header[256] = {0},http_param[256] = {0};

    /* 1. 计算onenet物联网平台创建设备的鉴权信息 */
    char authorization[256] = {0};
    cm_onenet_auth_generate(authorization,SIG_METHOD_MD5,ONENET_ET,ONENET_PRODUCT_ID,ONENET_PRODUCT_ACCESSKEY);
    cm_log_printf(0,"onenet auth is :%s",authorization);

    /* 2. 根据http类型创建http实例 */
    if(HTTP_TEST_TYPE == 0)
    {
        ret = cm_httpclient_create((const uint8_t *)HTTP_ONENET_SERVER, NULL, &http_client);
        if (CM_HTTP_RET_CODE_OK != ret)
        {
            cm_log_printf(0,"create http client fail");
            return -1;
        }
        client_cfg.ssl_enable = 0;
        client_cfg.ssl_id = 0;
    }
    else
    {   
        ret = cm_httpclient_create((const uint8_t *)HTTPS_ONENET_SERVER, NULL, &http_client);
        if (CM_HTTP_RET_CODE_OK != ret)
        {
            cm_log_printf(0,"create http client fail");
            return -1;
        }
        client_cfg.ssl_enable = 1;
        client_cfg.ssl_id = 1;
    }

    client_cfg.cid = 0;
    client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
    client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
    client_cfg.dns_priority = 1;

    /* 3. 配置http参数 */
    ret = cm_httpclient_set_cfg(http_client, client_cfg);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_httpclient_delete(http_client);
        cm_log_printf(0,"http client set config fail...");
        return -2;
    }
    
    /* 4. 设置http header */
    int http_headerlen = snprintf(http_header,sizeof(http_header),"authorization: %s\r\n",authorization);
    cm_httpclient_custom_header_set(http_client,(uint8_t *)http_header,(uint16_t)http_headerlen);

    /* 5. 封装http url中的参数信息，使用imei作为设备名称 */
    char imei[16] = {0};
    cm_sys_get_imei(imei);
    sprintf(http_param,"%s?product_id=%s&device_name=%s",ONENET_GET_URL,ONENET_PRODUCT_ID,imei);

    /* 6. 发起http同步请求 */
    cm_httpclient_sync_response_t response = {0};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_GET, (const uint8_t *)http_param,0,NULL};
    ret = cm_httpclient_sync_request(http_client, param, &response);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0,"cm_httpclient_sync_request() error! ret is %d\r\n", ret);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -3;
    }

    /* 7. 打印获取到的数据（因cm_log_printf打印长度有限，数据可能打印不完整）  */
    cm_log_printf(0,"http response code is %d\r\n", response.response_code);
    cm_log_printf(0,"http header is %d,%s\r\n", response.response_header_len,response.response_header);
    cm_log_printf(0,"response_content_len is %d,%s\r\n", response.response_content_len,response.response_content);

    /* 8. 解析获取到的数据 */
    cJSON *root = NULL;
    root = cJSON_Parse((const char *)(response.response_content));
    if(root == NULL)
    {
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -4;
    }

    cJSON *code = cJSON_GetObjectItem(root, "code");
    cJSON *data = cJSON_GetObjectItem(root, "data");
    if(code == NULL || data == NULL)
    {
        cJSON_Delete(root);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -4;
    }

    cJSON *status = cJSON_GetObjectItem(data, "status");
    
    if(code->valueint != 0)  //0表示查询成功
    {
        cm_log_printf(0,"http inquire onenet device(%s) fail,result code:%d",imei,code->valueint);
        cJSON_Delete(root);
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -5;
    }
    
    cm_log_printf(0,"http inquire onenet device(%s) success,device status:%d,result code:%d",imei,status->valueint,code->valueint);
    cJSON_Delete(root);
    cm_httpclient_sync_free_data(http_client);
    cm_httpclient_terminate(http_client);
    cm_httpclient_delete(http_client);
    return 0;
}

/* http测试线程 */
void cm_http_task_test(void *p)
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

    /* 1.使用http(s) post方式在onenet 物联网平台新增设备 */
    cm_http_post_onenet_reg_dev();

    osDelay(400);
    
    /* 2.使用http(s) get方式查询onenet 物联网平台的设备 */
    cm_http_post_onenet_inquire_dev();

    while(1)
    {
        osDelay(1000);
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

    osThreadNew((osThreadFunc_t)cm_http_task_test, 0, &app_task_attr);
    return 0;
}