/**
 * @file        cm_wifiscan_demo.c
 * @brief       本示例展示WIFISCAN功能，并将WIFISCAN扫描到的热点结果上传至高德，并获取经纬度及位置信息
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

#include "cm_wifiscan.h"
#include "cm_http.h"
#include "cm_ssl.h"
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_mem.h"
#include "cm_modem.h"
#include "cm_pm.h"
#include "cJSON.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define APP_SIZE_MAC_ADDRESS 13 // MAC地址原本为6个字节，转为字符串后为12字节，多加一个字节存放'\0'
/** 扫描到的单元数据 */
typedef struct
{
    uint8_t bssid[APP_SIZE_MAC_ADDRESS]; /*!< Basic Service SetIDentifier，一般是AP MAC地址 */
    int32_t rssi;                        /*!< 信号值 单位：dbm */
    uint32_t channel_number;             /*!< 通道号 */
} cm_wifiscan_demo_cell_info_t;

/** 扫描到的全部数据 */
typedef struct
{
    uint8_t bssid_number;                                                /*!< 扫描到的单元数量 */
    cm_wifiscan_demo_cell_info_t channel_cell_list[MAX_BSSID_NUM_TOTAL]; /*!< 单元数据 */
} cm_wifiscan_demo_info_t;
/****************************************************************************
 * Public Types
 ****************************************************************************/
#define CM_WIFISCAN_DEMO_AMAP_KEY "********************************" // 高德密钥，使用智能硬件定位服务需要企业账户申请KEY，请自行申请密钥
#define CM_WIFISCAN_DEMO_RETRY_COUNTS 5                              // 获取WIFISCAN最大重试次数
#define CM_WIFISCAN_DEMO_RETRY_TIMES 5                               // 获取WIFISCAN重试时间,单位S

#define CM_WIFISCAN_DEMO_HOST "http://apilocate.amap.com/" // 高德地图智能硬件定位 API HOST
#define CM_WIFISCAN_DEMO_URL "/position?"                  // 高德地图智能硬件定位 API URL
cm_wifiscan_demo_info_t g_cm_wifiscan_data = {0};
bool g_cm_wifiscan_finish_flag = false;
uint8_t g_cm_http_url[1024] = {0};

static osThreadId_t CM_WIFISCAN_DEMO_MAIN_TASKHANDLE = NULL;

/**
 *  @brief 源数据转十六进制字符串
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 将源数据二进制或者十六进制 转换成十六进制字符串，字节长度翻倍
 */
static void __NumToHex(char *inData, int inLen, char *outData)
{
    int i = 0;
    char tmpBuf[10];

    memset(tmpBuf, 0, sizeof(tmpBuf));

    if (inLen > 0)
    {
        for (i = 0; i < inLen; i++)
        {
            sprintf(tmpBuf, "%02X", inData[i]);
            strcat(outData, tmpBuf);
        }
    }
}

/**
 *  @brief WIFISCAN 扫描回调
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details WIFISCAN扫描到数据后，会通过该回调将数据拷贝到g_cm_wifiscan_data中
 *
 */
static void cm_wifiscan_demo_callback(cm_wifiscan_demo_info_t *param, void *user_param)
{
    char macBuf[24];
    g_cm_wifiscan_data.bssid_number = param->bssid_number;
    cm_log_printf(0, "cm_wifiscan_demo_callback number:%d\n", g_cm_wifiscan_data.bssid_number);

    for (int i = 0; i < param->bssid_number; i++)
    {
        memset(macBuf, 0, sizeof(macBuf));
        __NumToHex((char *)param->channel_cell_list[i].bssid, sizeof(param->channel_cell_list[i].bssid), macBuf);
        memcpy(g_cm_wifiscan_data.channel_cell_list[i].bssid, macBuf, strlen(macBuf));
        g_cm_wifiscan_data.channel_cell_list[i].rssi = param->channel_cell_list[i].rssi;
        g_cm_wifiscan_data.channel_cell_list[i].channel_number = param->channel_cell_list[i].channel_number;
    }

    g_cm_wifiscan_finish_flag = true;
}

/**
 *  @brief 获取WIFISCAN 扫描数据
 *
 *  @param [in]
 *
 *  @return 0-扫描成功，数据有效 -1-初始化失败 -2-开启扫描失败 -3-扫描超时失败 -4-数量过小失败，扫描到的数量小于2，不足以支持定位
 *
 *  @details
 *
 */
static int cm_wifiscan_get_data(cm_wifiscan_demo_info_t *wifiscan_data)
{
    uint8_t round = 3;                         // 扫描轮次
    uint8_t max = 10;                          // 期望上报数量
    uint8_t time_out = 30;                     // 总的扫描时间
    uint8_t priority = CM_WIFI_SCAN_WIFI_HIGH; // LTE/WIFI优先级，为了稳定扫描到WIFISCAN，建议设置AT+CFUN=5后进行扫描

    if (cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_ROUND, &round))
    {
        cm_log_printf(0, "wifiscan cfg round err\n");
        return -1;
    }

    if (cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_MAX_COUNT, &max))
    {
        cm_log_printf(0, "wifiscan cfg max err\n");
        return -1;
    }

    if (cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_TIMEOUT, &time_out))
    {
        cm_log_printf(0, "wifiscan cfg time_out err\n");
        return -1;
    }

    if (cm_wifiscan_cfg(CM_WIFI_SCAN_CFG_PRIORITY, &priority))
    {
        cm_log_printf(0, "wifiscan cfg priority err\n");
        return -1;
    }

    g_cm_wifiscan_finish_flag = false;                          // 扫描前，先置为false
    memset(&g_cm_wifiscan_data, 0, sizeof(g_cm_wifiscan_data)); // 扫描前，先清空
    if (cm_wifiscan_start((cm_wifiscan_callback_t)cm_wifiscan_demo_callback, "app_demo_wifiscan"))
    {
        cm_log_printf(0, "wifiscan start err\n");
        return -2;
    }

    cm_log_printf(0, "cm_wifiscan_start!\n");

    while (time_out--) // 只等待总扫描时间
    {
        if (g_cm_wifiscan_finish_flag) // 如果扫描到了则跳出等待
        {
            break;
        }
        osDelay(1000 / 5); // 1s 轮询一次
    }

    if (g_cm_wifiscan_finish_flag == false)
    {
        cm_log_printf(0, "wifiscan timeout\n");
        return -3;
    }

    /*高德要求WIFISCAN数量必须大于等于2，因此小于2不能定位*/
    if (g_cm_wifiscan_data.bssid_number < 2)
    {
        cm_log_printf(0, "wifiscan bssid number < 2 ,not enouth!\n");
        return -4;
    }

    memcpy(wifiscan_data, &g_cm_wifiscan_data, sizeof(g_cm_wifiscan_data)); // 把所扫描的数据拷贝，并返回给形参

    cm_log_printf(0, "cm_wifiscan_get_data success!\n");

    /*打印数据*/
    for (int i = 0; i < wifiscan_data->bssid_number; i++)
    {
        // cm_log_printf(0, "%d, %s, %d\n", wifiscan_data->channel_cell_list[i].channel_number, wifiscan_data->channel_cell_list[i].bssid, wifiscan_data->channel_cell_list[i].rssi);
    }

    return 0;
}

/**
 *  @brief WIFISCAN HTTP封装
 *
 *  @param [in] wifiscan_data 扫描到的WIFI数据
 *  @param [out] url_buffer   封装好后的存放URL的buffer
 *  @param [out] url_bufferlen 封装好后的存放URL的bufferlen 需要注意长度,
 *
 *  @return true-封装成功 false-封装失败,传入的bufferlen过小
 *
 *  @details 调用该接口，可将扫描的WIFI数据封装成符合高德地图智能硬件定位所需的报文，此API请求方式为GET
 *  因此按照平台要求将必要信息封装进URL即可
 */
static bool cm_wifiscan_httpurl_packet(cm_wifiscan_demo_info_t wifiscan_data, uint8_t *url_buffer, uint16_t url_bufferlen)
{
    uint8_t access_type[] = "accesstype=1"; // 0-移动网络接入 1-WIFI接入 这里因为是使用WIFISCAN定位所以选择1
    char imei_temp[15] = {0};
    char imei[21] = "imei=";
    cm_sys_get_imei(imei_temp);
    strncat(imei, imei_temp, strlen(imei_temp));

    char key[128] = "key=";
    strcat(key, CM_WIFISCAN_DEMO_AMAP_KEY);

    // 将搜索到的WIFISCAN逐一放入到MACS中：①以逗号分隔6个字节 ②加入信号 ③加入ssid,其中ssid可填可不填.每个MAC信息以|分隔
    // 模版：5A:48:49:12:26:e6,-84,ML307A|58:48:49:02:26:e6,-85,ML307A|8e:4d:95:1D:48:f3,-85,ML307A
    uint8_t macs[1024] = "macs=";
    uint8_t mac_temp[20] = {0};  // 存放单个添加了逗号的MAC地址
    uint8_t rssi_temp[64] = {0}; // 存放单个信号
    uint8_t rssi_templen = {0};  // 存放单个信号
    uint8_t macs_temp[64] = {0}; // 存放单个添加了MAC_TEMP、信号、SSID的数据
    uint8_t macs_templen = 0;
    uint16_t offset2 = strlen((char *)macs);
    for (uint8_t i = 0; i < wifiscan_data.bssid_number; i++)
    {
        uint8_t offset1 = 0;
        for (uint8_t j = 0; j < 12; j = j + 2)
        {
            memcpy(mac_temp + offset1, &wifiscan_data.channel_cell_list[i].bssid[j], 2);
            offset1 = offset1 + 2;
            if (j != 10) // 最后一个字节不加:
            {
                memcpy(mac_temp + offset1, ":", strlen(":"));
                offset1 = offset1 + 1;
            }
        }
        memcpy(macs_temp, mac_temp, offset1);
        memcpy(macs_temp + offset1, ",", strlen(","));
        offset1 = offset1 + 1;

        rssi_templen = snprintf((char *)rssi_temp, sizeof(rssi_temp), "%ld,ML307A", wifiscan_data.channel_cell_list[i].rssi);

        memcpy(macs_temp + offset1, rssi_temp, rssi_templen);
        macs_templen = offset1 + rssi_templen;

        memcpy(macs + offset2, macs_temp, macs_templen);
        offset2 = offset2 + macs_templen;
        // 最后一个字节不加:
        if (i != wifiscan_data.bssid_number - 1)
        {

            memcpy(macs + offset2, "|", strlen("|"));
            offset2 = offset2 + 1;
        }

        memset(mac_temp, 0, sizeof(mac_temp));
        memset(rssi_temp, 0, sizeof(rssi_temp));
        memset(macs_temp, 0, sizeof(macs_temp));
    }

    strcat((char *)url_buffer, CM_WIFISCAN_DEMO_URL);
    strcat((char *)url_buffer, (char *)access_type);
    strcat((char *)url_buffer, "&");
    strcat((char *)url_buffer, imei);
    strcat((char *)url_buffer, "&");
    strcat((char *)url_buffer, (char *)macs);
    strcat((char *)url_buffer, "&");
    strcat((char *)url_buffer, key);

    if (url_bufferlen < strlen((char *)url_buffer))
    {
        cm_log_printf(0, "cm_wifiscan_httpurl_packet len not enough!\n");
        return false;
    }

    cm_log_printf(0, "url_buffer:%s\n", url_buffer);
    return true;
}

/**
 *  @brief WIFISCAN HTTP GET获取定位信息
 *
 *  @param [in] host_addr 传入高德平台API HOST
 *  @param [in] url   传入高德平台API URL
 *  @param [out] longitude 定位成功后得到的经度
 *  @param [out] latitude 定位成功后得到的纬度
 *  @param [out] data 定位成功后得到平台返回的完整报文（包含中文的街道信息）
 *  @param [in] datasize 存放报文的大小，如果过小会返回错误
 *  @param [out] datalen 实际返回的报文大小
 *
 *  @return 0-获取成功 -1-HTTP失败 -2-定位失败，解析不到有效数据 -3-存放报文大小过小
 *
 *  @details 调用该接口，可将扫描的WIFI数据封装成符合高德地图智能硬件定位所需的报文，此API请求方式为GET
 *  因此按照平台要求将必要信息封装进URL即可
 */
int cm_wifiscan_http_location(uint8_t *host_addr, uint8_t *url, double *longitude, double *latitude, uint8_t *data, uint16_t datasize, uint16_t *datalen)
{
    cm_httpclient_handle_t http_client = NULL;
    cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
    ret = cm_httpclient_create(host_addr, NULL, &http_client);
    cm_log_printf(0, "cm_wifiscan_http_location host_addr:%s\n", host_addr);
    cm_log_printf(0, "cm_wifiscan_http_location url:%s\n", url);
    if (CM_HTTP_RET_CODE_OK != ret || NULL == http_client)
    {
        cm_log_printf(0, "cm_httpclient_create error");
        return -1;
    }
    cm_httpclient_cfg_t client_cfg;
    client_cfg.ssl_enable = false;
    client_cfg.ssl_id = 0;
    client_cfg.cid = 0;
    client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
    client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
    client_cfg.dns_priority = 1;
    ret = cm_httpclient_set_cfg(http_client, client_cfg);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0, "cm_httpclient_set_cfg error");
        cm_httpclient_delete(http_client);
        return -1;
    }
    cm_httpclient_sync_response_t response = {0};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_GET, url, 0, NULL};
    int http_ret = 0;
    uint8_t http_header[64] = {0};

    memset(http_header, 0, sizeof(http_header));
    cm_httpclient_custom_header_set(http_client, http_header, strlen((char *)http_header)); // 若有报头设置 可使用该接口设置报头

    int tmp = 0;
    int negotime = 60;
    cm_ssl_setopt(0, CM_SSL_PARAM_VERIFY, &tmp); // 设置SSL验证方式
    cm_ssl_setopt(0, CM_SSL_PARAM_NEGOTIME, &negotime);
    ret = cm_httpclient_sync_request(http_client, param, &response);
    if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
    {
        osDelay(1000 / 5);
        ret = cm_httpclient_sync_request(http_client, param, &response);                                    // 重新下载
        if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
        {
            http_ret = -1;
            cm_log_printf(0, "[**HTTP**]httpfile download full mode error!\n");
            goto EXIT;
        }
    }

    cm_log_printf(0, "cm_wifiscan_http_location http get success! response_code:%d,response_content_len:%d,reponse_header_len:%d\n",
                  response.response_code, response.response_content_len, response.response_header_len);

    /*解析经纬度*/
    cJSON *root = cJSON_Parse((char *)response.response_content);
    if (root == NULL)
    {
        cJSON_Delete(root);
        http_ret = -2;
        cm_log_printf(0, "root is null !\n");
        goto EXIT;
    }
    cJSON *cjson_status = NULL;
    cJSON *cjson_result = NULL;
    cJSON *cjson_location = NULL;

    cjson_status = cJSON_GetObjectItem(root, "status");

    if (strncmp(cjson_status->valuestring, "1", strlen("1"))) // 如果不是为1   说明没有定位成功
    {
        cJSON_Delete(root);
        http_ret = -2;
        cm_log_printf(0, "cm_wifiscan_http_location content status error:%s\n !", cjson_status->string);
        goto EXIT;
    }

    cjson_result = cJSON_GetObjectItem(root, "result");
    if (cjson_result == NULL)
    {
        cJSON_Delete(root);
        http_ret = -2;
        cm_log_printf(0, "cjson_result is null !\n");
        goto EXIT;
    }

    cjson_location = cJSON_GetObjectItem(cjson_result, "location");
    if (cjson_location == NULL)
    {
        cJSON_Delete(root);
        http_ret = -2;
        cm_log_printf(0, "cjson_location is null !\n");
        goto EXIT;
    }

    cm_log_printf(0, "cm_wifiscan_http_location:%s\n", cjson_location->valuestring);

    if (sscanf(cjson_location->valuestring, "%lf,%lf", longitude, latitude) != 2)
    {
        cJSON_Delete(root);
        http_ret = -2;
        cm_log_printf(0, "cjson_location is null !\n");
        goto EXIT;
    }

    if (datasize < response.response_content_len)
    {
        cJSON_Delete(root);
        http_ret = -3;
        cm_log_printf(0, "[**HTTP**]httpfile download full mode datasize not enough! at least:%d\n", response.response_content_len);
        goto EXIT;
    }

    http_ret = 0;
    memcpy(data, response.response_content, response.response_content_len); // 拷贝数据
    *datalen = response.response_content_len;                               // 拷贝数据长度

EXIT:
    cm_httpclient_custom_header_free(http_client);
    cm_httpclient_sync_free_data(http_client);
    cm_httpclient_terminate(http_client);
    cm_httpclient_delete(http_client);

    if (http_ret < 0)
    {
        memset(data, 0, datasize);
        *datalen = 0;
    }

    return http_ret;
}

/**
 *  @brief WIFISCAN 示例
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该接口，可获取一次WIFISCAN扫描数据，并从高德返回相关经纬度
 *  为保障WIFISCAN可进行有效扫描，扫描期间会关闭模组的射频功能，模组会掉网；扫描完成后，会重新开启射频，访问服务器。
 */
void cm_wifiscan_demo_main(void)
{
    /*为了WIFISCAN可以更快更稳定搜索，设置CFUN为5，该状态下模组会掉网，关闭4G射频*/
    osDelay(2000 / 5); // 刚开机立即关闭CFUN可能会失败，延时一下再关闭。
    int ret = 0;
    ret = cm_modem_set_cfun(5);
    if (ret < 0)
    {
        cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 5 failed! try again!\n");
        ret = cm_modem_set_cfun(5);
        if (ret < 0)
        {
            cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 5 failed! system reboot!\n");
            cm_pm_reboot();
            osDelay(1000);
        }
    }

    cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 5 success!\n");
    /*开启WIFI扫描*/
    cm_wifiscan_demo_info_t app_wifi_scan_data = {0};

    for (uint8_t i = 0; i < CM_WIFISCAN_DEMO_RETRY_COUNTS; i++)
    {
        ret = cm_wifiscan_get_data(&app_wifi_scan_data);
        if (ret == 0)
        {
            cm_log_printf(0, "cm_wifiscan_get_data success! recover network!\n");
            break;
        }
        cm_log_printf(0, "cm_wifiscan_get_data result:%d,fail try again:%d\n", ret, i);

        osDelay(CM_WIFISCAN_DEMO_RETRY_TIMES * 1000 / 5); // 延时一段时间再获取wifiscan
    }
    if (ret < 0)
    {
        cm_log_printf(0, "cm_wifiscan_get_data fail! system reboot\n");
        cm_pm_reboot();
        osDelay(1000);
    }

    /*恢复网络*/
    ret = cm_modem_set_cfun(1);
    if (ret < 0)
    {
        cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 1 failed! try again!\n");
        ret = cm_modem_set_cfun(1);
        if (ret < 0)
        {
            cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 1 failed! system reboot!\n");
            cm_pm_reboot();
            osDelay(1000);
        }
    }

    cm_log_printf(0, "cm_wifiscan_demo_main set cfun to 1 success!\n");

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

    /*将获取到的WIFI数据进行封装，得到访问高德平台API的URL*/
    ret = cm_wifiscan_httpurl_packet(app_wifi_scan_data, g_cm_http_url, sizeof(g_cm_http_url));
    if (ret == 0)
    {
        cm_log_printf(0, "cm_wifiscan_httpurl_packet fail!\n");
        cm_pm_reboot();
        osDelay(1000);
    }

    /*访问API，并获取位置信息*/
    double app_longitude = 0;
    double app_latitude = 0;
    uint8_t data[1024] = {0};
    uint16_t datalen = 0;
    ret = cm_wifiscan_http_location((uint8_t *)CM_WIFISCAN_DEMO_HOST, g_cm_http_url, &app_longitude, &app_latitude, data, sizeof(data), &datalen);
    if (ret < 0)
    {
        cm_log_printf(0, "cm_wifiscan_http_location fail!\n");
        cm_pm_reboot();
        osDelay(1000);
    }

    cm_log_printf(0, "cm_wifiscan_http_location app_longitude:%lf\n", app_longitude);
    cm_log_printf(0, "cm_wifiscan_http_location app_latitude:%lf\n", app_latitude);
    // cm_log_printf(0, "cm_wifiscan_http_location data:%s\n", data);  /*因为数据太长超过180字节，无法打印，实际有数据产生*/
    cm_log_printf(0, "cm_wifiscan_http_location datalen:%d\n", datalen);

    while (1)
    {
        osDelay(1000);
    }
}

/**
 *  @brief WIFISCAN接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始MQTT相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*创建WIFISCAN数据接收/发送处理线程*/
    osThreadAttr_t wifiscan_main_task_attr = {0};
    wifiscan_main_task_attr.name = "wifiscan_demo_main_task";
    wifiscan_main_task_attr.stack_size = 20 * 1024;
    wifiscan_main_task_attr.priority = osPriorityNormal;

    if (CM_WIFISCAN_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_WIFISCAN_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_wifiscan_demo_main, 0, &wifiscan_main_task_attr);
        if (CM_WIFISCAN_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_WIFISCAN_DEMO_MAIN_TASKHANDLE is NULL\n");
        }
    }

    return 0;
}