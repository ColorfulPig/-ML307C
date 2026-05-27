/**
 * @file        cm_fota_demo.c
 * @brief       本示例使用HTTP实现模组三种FOTA方案升级，通过配置升级方式和URL进行升级
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
#include "cm_mem.h"
#include "cm_http.h"
#include "cm_fota.h"
#include "cm_ota.h"
#include "cm_pm.h"
#include "cm_modem.h"
#include "cm_virt_at.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*FOTA支持三种方案：最小系统升级方案、全系统差分升级、APP整包升级
  最小系统升级方案需传入升级链接，模组自动完成升级
  全系统差分升级/APP整包升级需自行完成升级包下载
  本示例提供两种方案示例：
  最小系统升级方案:传入升级连接->触发升级
  全系统差分升级/APP整包升级:两者逻辑相同，仅生成差分包方式不同，均通过HTTP GET方法下载升级包->写入升级包数据至指定位置->触发升级
  注意在本示例中，采用APP整包升级时，模组升级成功后，会再次下载升级包并反复升级，为正常现象。而最小系统升级和全系统差分升级，升级一次成功后，会校验到升级包为旧升级包，而停止升级。*/
#define APP_DEMO_FOTA_MODE 0                                                                          // 0-最小系统 1-全系统差分升级/APP整包升级
#define APP_DEMO_FOTA_URL "http://8.137.154.246:8080/download/FAE_TEST/zwc/ML307R_APP_DEMO_PATCH.bin" // 升级包下载链接 若要升级成功 需要修改成对应升级包的下载链接
#define APP_FOTA_RETRYTIME_MAX 60                                                                     // FOTA执行超时时间
#define APP_FOTA_RETRYCOUNT_MAX 5                                                                     // FOTA重试次数
#define APP_FOTA_CHECK 1                                                                              // 最小系统是否执行部分校验
#define APP_FOTA_PER_PACKET_LEN (10 * 1024)                                                           // HTTP分包大小，当分包大小大于整个文件大小，将一次性下载
/****************************************************************************
 * Public Types
 ****************************************************************************/
static cm_fota_error_e hal_fota_error = 0;
static osThreadId_t CM_FOTA_DEMO_MAIN_TASKHANDLE = NULL;

#undef APP_DEMO_FOTA_MODE
#define APP_DEMO_FOTA_MODE 1

#undef APP_DEMO_FOTA_URL
#define APP_DEMO_FOTA_URL "http://jxw.qmkd.cn/qdbattery/app/getModuleOtaFile/4"

static void cm_fota_demo_log_to_usb(void)
{
    char operation[64] = {0};
    uint8_t rsp[128] = {0};
    int32_t rsp_len = 0;

    sprintf(operation, "%s\r\n", "AT+MCFG=log2cat,1");
    if (cm_virt_at_send_sync((const uint8_t *)operation, rsp, &rsp_len, 10) == 0)
    {
        cm_log_printf(0, "log2cat rsp=%s rsp_len=%d\n", rsp, rsp_len);
    }
    else
    {
        cm_log_printf(0, "log2cat ret != 0\n");
    }
}

/**
 *  @brief HTTP URL解析
 *
 *  @param [in] http_url HTTP链接
 *  @param [out] http_host HTTP的主机
 *  @param [http_path] datasize HTTP的路径

 *
 *  @return
 *   0 合法
 *  -1 非法，存在格式错误
 *  -2 host_size或path_size过小
 *
 *  @details 传入HTTP链接，解析出主机地址和路径，以便HTTP功能使用
 */
int app_http_url_check(char *http_url, char *http_host, uint32_t host_size, char *http_path, uint32_t path_size)
{
    char http_host_temp[256] = {0}, http_path_temp[256] = {0};
    char *offset = NULL;
    int port = 0;
    if (strncmp(http_url, "http://", strlen("http://")) == 0)
    {
        offset = strchr(http_url + strlen("http://"), '/');
        if (offset == NULL)
        {
            cm_log_printf(0,"http url format is error:%s\n", http_url);
            return -1;
        }
        strcpy(http_path_temp, offset);
        strncpy(http_host_temp, http_url, strlen(http_url) - strlen(offset));
    }
    else
    {
        offset = strchr(http_url, '/');
        if (offset == NULL)
        {
            cm_log_printf(0,"http url format is error:%s\n", http_url);
            return -1;
        }
        strcpy(http_path_temp, offset);
        strcpy(http_host_temp, "http://");
        strncpy(http_host_temp + strlen("http://"), http_url, strlen(http_url) - strlen(offset));
    }
    if (sscanf(http_host_temp, "http://%*[^:]:%d", &port) != 1) // 没有port,添加默认port口80
    {
        strcat(http_host_temp, ":80");
    }

    if (host_size < strlen(http_host_temp) || path_size < strlen(http_path_temp))
    {
        cm_log_printf(0,"http host or path size not enough\n");
        return -2;
    }

    strncpy(http_host, http_host_temp, strlen(http_host_temp));
    strncpy(http_path, http_path_temp, strlen(http_path_temp));
    cm_log_printf(0,"[FOTA]host addr:%s,url:%s\n", http_host, http_path);

    return 0;
}

/**
 *  @brief FOTA 错误码回调
 *
 *  @param [in] error FOTA错误码
 *
 *  @details More details
 */
static void hal_fota_cb(cm_fota_error_e error)
{
    cm_log_printf(0,"[FOTA] error code is %d\n", error);
    hal_fota_error = error;
}


/**
 *  @brief FOTA-最小系统升级方案
 *
 *  @param [in] ota_url 传入升级连接
 *
 *  @return
 *  0  触发FOTA成功
 *  -1 URL错误 \n
 *  -2 HTTP错误
 *  -3 执行升级失败 通过回调查看错误码
 *
 *  @details 直接传入升级链接,若打开APP_FOTA_CHECK会对链接的格式做部分校验，且会先尝试下载10个字节，看升级包是否存在且可下载
 *  若确认每次传入链接是无误的，可无需检查，注意host_addr与file_url大小为256,可根据实际情况调整
 *  若触发升级失败，可查看回调中的错误码
 */
int cm_mini_system_http_fota(char *ota_url)
{
    if (strncmp(ota_url, "https://", strlen("https://")) == 0) // 本示例只支持HTTP 本身模组支持HTTPS FOTA可按需修改
    {
        cm_log_printf(0,"THIS DEMO NOT SUPPORT HTTPS!\n");
        return -1;
    }

    char host_addr[256] = {0}, file_url[256] = {0};
    if (app_http_url_check(ota_url, host_addr, sizeof(host_addr), file_url, sizeof(file_url)) != 0)
    {
        cm_log_printf(0,"ota url error!\n");
        return -1;
    }

#if APP_FOTA_CHECK
    /* 使用http请求验证OTA bin文件是否存在 */
    cm_httpclient_handle_t http_client = NULL;
    cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
    ret = cm_httpclient_create((const uint8_t *)host_addr, NULL, &http_client);
    if (CM_HTTP_RET_CODE_OK != ret || NULL == http_client)
    {
        cm_log_printf(0,"cm_httpclient_create error\n");
        return -2;
    }
    cm_httpclient_cfg_t client_cfg;
    client_cfg.ssl_enable = 0;
    client_cfg.ssl_id = 0;
    client_cfg.cid = 0;
    client_cfg.conn_timeout = 5; // 超时时间5s
    client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
    client_cfg.dns_priority = 1;
    ret = cm_httpclient_set_cfg(http_client, client_cfg);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0,"cm_httpclient_set_cfg error\n");
        cm_httpclient_delete(http_client);
        return -2;
    }
    cm_httpclient_sync_response_t response = {0};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_GET, (const uint8_t*)file_url, 0, NULL};

    char http_header[20] = {0};
    sprintf(http_header, "Range:bytes=0-9\r\n"); // 使用http分包，只下载前10个字节
    cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header));

    ret = cm_httpclient_sync_request(http_client, param, &response);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0,"cm_httpclient_sync_request fail,ota file is not exit\n");
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -2;
    }
    if (response.response_content_len != 10) // 长度是否为10字节
    {
        cm_log_printf(0,"ota  file is not exit\n");
        cm_httpclient_sync_free_data(http_client);
        cm_httpclient_terminate(http_client);
        cm_httpclient_delete(http_client);
        return -2;
    }
    cm_httpclient_sync_free_data(http_client);
    cm_httpclient_terminate(http_client);
    cm_httpclient_delete(http_client);
    cm_log_printf(0,"ota file check normal\n");

    osDelay(100 / 5);

#endif
    cm_fota_res_callback_register((cm_fota_result_callback)hal_fota_cb);

    char l_ota_url[256] = {0};
    strcat(l_ota_url,host_addr);
    strcat(l_ota_url,file_url);
    if (cm_fota_set_url(l_ota_url) < 0)
    {
        cm_log_printf(0,"cm_fota_set_url fail\n");
        return -3;
    }
    cm_log_printf(0,"cm_fota_set_url success\n");

    int result = cm_fota_exec_upgrade();
    cm_log_printf(0,"cm_mini_system_http_fota result:%d\n", result);
    hal_fota_error = 0;
    uint8_t count = 0;
    uint8_t retry_count = 0;
    for (count = 0; count < APP_FOTA_RETRYTIME_MAX; count++)
    {
        if (hal_fota_error != 0)
        {
            hal_fota_error = 0;
            retry_count++;
            result = cm_fota_exec_upgrade();
            cm_log_printf(0,"cm_mini_system_http_fota retry result:%d,retry count:%d\n", result, retry_count);
            count = 0;
        }
        if (retry_count == APP_FOTA_RETRYCOUNT_MAX)
        {
            cm_log_printf(0,"cm_mini_system_http_fota RETRY COUNT MAX! OTA FAILED!\n");
            break;
        }
        osDelay(1000 / 5);
    }

    if (count == APP_FOTA_RETRYTIME_MAX || retry_count == APP_FOTA_RETRYCOUNT_MAX)
    {
        cm_log_printf(0,"cm_mini_system_http_fota cm_fota_exec_upgrade failed!\n");
        return -3;
    }

    return 0;
}

/*以上为最小系统升级方案demo程序，以下为全系统差分升级/APP整包升级*/

/**
 *  @brief HTTP GET文件下载
 *
 *  @param [in] http_url 传入升级连接
 *  @param [out] data 存放下载数据的buffer
 *  @param [in] datasize 存放下载数据的buffer大小
 *  @param [in] isfull 是否全量下载 0-非全量下载 1-全量下载
 *  @param [in] range_start 下载部分文件,从n字节开始，包括0
 *  @param [in] range_end 下载部分文件,至n字节结束
 *  @param [out] filelen 整个文件的大小
 *  @param [out] datalen 下载成功的字节数
 *
 *  @return
 *   0 下载成功
 *  -1 URL错误 \n
 *  -2 HTTP请求错误
 *  -3 datasize过小
 *  -4 range_start range_end参数错误
 *
 *  @details 本示例使用HTTP GET方法下载文件，需服务器支持Range分包下载，即断点续传功能，可控制下载文件的始末
 *  三种下载方式：
 *  （1）一次性下载完成，设置isfull为1，range_start和range_end的值会被忽略
 *  （2）获取文件包大小，设置isfull为0, range_start和range_end为0
 *  （3）部分下载，设置isfull为0，同时设置range_start和range_end
 *  使用一次性下载需注意datasize是否满足下载内容大小
 */
int cm_httpfile_download(char *http_url, uint8_t *data, uint32_t datasize, uint32_t range_start, uint32_t range_end, bool isfull, uint32_t *filelen, uint32_t *datalen)
{
    if (strncmp(http_url, "https://", strlen("https://")) == 0) // 本示例只支持HTTP 本身模组支持HTTPS FOTA可按需修改
    {
        cm_log_printf(0,"THIS DEMO NOT SUPPORT HTTPS!\n");
        return -1;
    }

    char host_addr[256] = {0}, file_url[256] = {0};
    if (app_http_url_check(http_url, host_addr, sizeof(host_addr), file_url, sizeof(file_url)) != 0)
    {
        cm_log_printf(0,"ota url error!\n");
        return -1;
    }

    cm_httpclient_handle_t http_client = NULL;
    cm_httpclient_ret_code_e ret = CM_HTTP_RET_CODE_UNKNOWN_ERROR;
    ret = cm_httpclient_create((const uint8_t*)host_addr, NULL, &http_client);
    if (CM_HTTP_RET_CODE_OK != ret || NULL == http_client)
    {
        cm_log_printf(0,"cm_httpclient_create error\n");
        return -2;
    }
    cm_httpclient_cfg_t client_cfg;
    client_cfg.ssl_enable = 0;
    client_cfg.ssl_id = 0;
    client_cfg.cid = 0;
    client_cfg.conn_timeout = HTTPCLIENT_CONNECT_TIMEOUT_DEFAULT;
    client_cfg.rsp_timeout = HTTPCLIENT_WAITRSP_TIMEOUT_DEFAULT;
    client_cfg.dns_priority = 1;
    ret = cm_httpclient_set_cfg(http_client, client_cfg);
    if (CM_HTTP_RET_CODE_OK != ret)
    {
        cm_log_printf(0,"cm_httpclient_set_cfg error\n");
        cm_httpclient_delete(http_client);
        return -2;
    }
    cm_httpclient_sync_response_t response = {0};
    cm_httpclient_sync_param_t param = {HTTPCLIENT_REQUEST_GET, (const uint8_t*)file_url, 0, NULL};
    int file_size = 0, http_ret = 0;
    char http_header[64] = {0};
    char *file_size_offset = NULL;

    /*isfull = 1 全量下载*/
    if (isfull == 1)
    {
        memset(http_header, 0, sizeof(http_header));
        cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); // 若有报头设置 可使用该接口设置报头
        ret = cm_httpclient_sync_request(http_client, param, &response);
        if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
        {
            osDelay(1000 / 5);
            ret = cm_httpclient_sync_request(http_client, param, &response);                                    // 重新下载
            if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
            {
                http_ret = -2;
                cm_log_printf(0,"[**HTTP**]httpfile download full mode error!\n");
                goto EXIT;
            }
        }

        if (datasize < response.response_content_len)
        {
            http_ret = -3;
            cm_log_printf(0,"[**HTTP**]httpfile download full mode datasize not enough! at least:%d\n", response.response_content_len);
            goto EXIT;
        }

        memcpy(data, response.response_content, response.response_content_len); // 拷贝数据
        *datalen = response.response_content_len;                               // 拷贝数据长度
        *filelen = response.response_content_len;                               // 拷贝数据长度
        http_ret = 0;
        cm_log_printf(0,"[**HTTP**]httpfile download full mode success!\n");
        cm_log_printf(0,"[**HTTP**]httpfile download full mode response_code:%d,response_content_len:%d,reponse_header_len:%d\n",
                     response.response_code, response.response_content_len, response.response_header_len);
        goto EXIT;
    }

    /*获取文件包大小*/
    else if (isfull == 0 && range_start == 0 && range_end == 0)
    {
        memset(http_header, 0, sizeof(http_header));
        strcpy(http_header, "Range:bytes=0-9\r\n");     // 使用Range尝试下载10个字节 以获取文件大小
        cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); // 若有报头设置 可使用该接口设置报头
        ret = cm_httpclient_sync_request(http_client, param, &response);
        if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
        {
            osDelay(1000 / 5);
            ret = cm_httpclient_sync_request(http_client, param, &response);                                    // 重新下载
            if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
            {
                http_ret = -2;
                cm_log_printf(0,"[**HTTP**]httpfile download full mode error!\n");
                goto EXIT;
            }
        }
        /*取出报头中的文件包大小*/
        file_size_offset = strstr((char *)response.response_header, (char *)"Content-Range");
        if (file_size_offset == NULL)
        {
            cm_log_printf(0,"[**HTTP**]http header error:%d,%s\n", response.response_header_len, response.response_header);
            http_ret = -2;
            goto EXIT;
        }
        if (sscanf(file_size_offset, "%*[^/]/%d", &file_size) != 1 || file_size <= 0)
        {
            cm_log_printf(0,"[**HTTP**]http header error:%d,%s\n", response.response_header_len, response.response_header);
            http_ret = -2;
            goto EXIT;
        }
        *filelen = file_size; // 拷贝数据长度
        http_ret = 0;
        cm_log_printf(0,"[**HTTP**]httpfile download get filesize:%d\n", *filelen);
        goto EXIT;
    }

    /*按照range_start和range_end 部分下载*/
    else if (isfull == 0 && (range_end - range_start > 0))
    {
        memset(http_header, 0, sizeof(http_header));
        sprintf(http_header, "Range:bytes=%d-%d\r\n", (int)range_start, (int)range_end);          // 使用Range配置下载文件始末
        cm_httpclient_custom_header_set(http_client, (uint8_t *)http_header, strlen(http_header)); // 若有报头设置 可使用该接口设置报头
        ret = cm_httpclient_sync_request(http_client, param, &response);
        if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
        {
            osDelay(1000 / 5);
            ret = cm_httpclient_sync_request(http_client, param, &response);                                    // 重新下载
            if (CM_HTTP_RET_CODE_OK != ret || (response.response_code != 200 && response.response_code != 206)) // ret不等于OK 或者code不等于200也不等于206
            {
                http_ret = -2;
                cm_log_printf(0,"[**HTTP**]httpfile download full mode error!\n");
                goto EXIT;
            }
        }

        /*取出报头中的文件包大小*/
        file_size_offset = strstr((char *)response.response_header, (char *)"Content-Range");
        if (file_size_offset == NULL)
        {
            cm_log_printf(0,"[**HTTP**]http header error:%d,%s\n", response.response_header_len, response.response_header);
            http_ret = -2;
            goto EXIT;
        }
        if (sscanf(file_size_offset, "%*[^/]/%d", &file_size) != 1 || file_size <= 0)
        {
            cm_log_printf(0,"[**HTTP**]http header error:%d,%s\n", response.response_header_len, response.response_header);
            http_ret = -2;
            goto EXIT;
        }

        memcpy(data, response.response_content, response.response_content_len); // 拷贝数据
        *datalen = response.response_content_len;                               // 拷贝数据长度
        *filelen = file_size;                                                   // 拷贝数据长度
        http_ret = 0;
        cm_log_printf(0,"[**HTTP**]httpfile download partial mode success!\n");
        cm_log_printf(0,"[**HTTP**]httpfile download partial mode response_code:%d,response_content_len:%d,reponse_header_len:%d\n",
                     response.response_code, response.response_content_len, response.response_header_len);
    }

    else
    {
        cm_log_printf(0,"[**HTTP**]httpfile download param error!\n");
        http_ret = -4;
        goto EXIT;
    }

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
 *  @brief FOTA-全系统差分升级/APP整包升级
 *
 *  @param [in] ota_url 传入升级连接
 *
 *  @return
 *   0 下载成功 触发升级成功
 *  -1 OTA初始化失败
 *  -2 获取升级包大小失败 \n
 *  -3 动态内存不足 申请内存失败 \n
 *  -4 下载失败 并清除已写入升级空间的数据 \n
 *  -5 写入数据失败                      \n
 *  -6 触发升级失败，可能升级包有误       \n
 *
 *  @details 直接传入升级链接,若打开APP_FOTA_CHECK会对链接的格式做部分校验，且会先尝试下载10个字节，看升级包是否存在且可下载
 *  若确认每次传入链接是无误的，可无需检查，注意host_addr与file_url大小为256,可根据实际情况调整
 */
int cm_full_system_http_fota(char *ota_url)
{
    int ret = -1;
    ret = cm_ota_init(); // 初始化OTA
    if (ret < 0)
    {
        cm_log_printf(0,"cm_full_system_http_fota cm_ota_init failed:%d!\n", ret);
        return -1;
    }

    ret = cm_ota_firmware_erase(); // 先擦除固件区
    if (ret < 0)
    {
        cm_log_printf(0,"cm_full_system_http_fota cm_ota_firmware_erase failed:%d!\n", ret);
        return -1;
    }

    uint32_t filelen = 0, datalen = 0;                                         
    if (0 != cm_httpfile_download(ota_url, NULL, 0, 0, 0, 0, &filelen, &datalen)) // 先获取升级包大小
    {
        cm_log_printf(0,"cm_full_system_http_fota get filelen error!\n");
        return -2;
    }

    cm_log_printf(0,"cm_full_system_http_fota filelen:%d\n", filelen);

    cm_ota_set_otasize(filelen);

    cm_log_printf(0,"cm_ota_set_otasize:%d\n", filelen);

    /*开始下载*/
    uint8_t *data = cm_malloc(APP_FOTA_PER_PACKET_LEN + 1024); // 多申请1k  以免溢出
    if (data == NULL)
    {
        cm_log_printf(0,"cm_malloc data failed!\n");
        return -3;
    }
    uint32_t write_offset = 0, download_len = 0;
    while (1) // 按照间隔分包多次下载,且下载一包，写入升级区域一包，一旦下载失败 清除已写入的数据
    {
        /*先清空*/
        memset(data, 0, APP_FOTA_PER_PACKET_LEN + 1024);
        datalen = 0;

        download_len = (APP_FOTA_PER_PACKET_LEN <= (filelen - write_offset)) ? APP_FOTA_PER_PACKET_LEN : (filelen - write_offset); // 确认下载范围
        ret = cm_httpfile_download(ota_url, data, APP_FOTA_PER_PACKET_LEN + 1024, write_offset, write_offset + download_len - 1, 0, &filelen, &datalen);
        if (ret != 0)
        {
            if (cm_ota_firmware_erase() < 0)
            {
                cm_log_printf(0,"cm_full_system_http_fota cm_ota_firmware_erase failed! try again!\n");
                cm_free(data);
                cm_ota_firmware_erase();
                return -4;
            }
            cm_log_printf(0,"cm_full_system_http_fota cm_httpfile_download failed! erase data!\n");
            cm_free(data);
            return -4;
        }
        /*下载成功,写入升级区域*/
        ret = cm_ota_firmware_write((char *)data, datalen); // 该接口是累加写，不是覆盖写，写入数据会在原数据尾部增加
        if (ret != 0)
        {
            cm_log_printf(0,"cm_full_system_http_fota cm_ota_firmware_write failed! try again!\n");
            ret = cm_ota_firmware_write((char *)data, datalen); // 如果写失败了，再写一遍
            if (ret != 0)                               // 如果再次失败，退出流程
            {
                cm_log_printf(0,"cm_full_system_http_fota cm_ota_firmware_write failed! erase data!\n");
                cm_free(data);
                cm_ota_firmware_erase();
                return -5;
            }
        }
        cm_log_printf(0,"cm_ota_firmware_write %d bytes success\n", datalen);

        write_offset += datalen; // 累加已下载成功 并写入的数据

        if (cm_ota_get_written_size() == filelen) // 当已写入完成
        {
            cm_log_printf(0,"cm_full_system_http_fota download and write ota area finish!\n ");
            break;
        }
        osDelay(10 / 5);
    }

    ret = cm_ota_upgrade();

    if (ret < 0)
    {
        cm_free(data);
        cm_log_printf(0,"cm_full_system_http_fota cm_ota_upgrade failed ret:%d\n", ret);
        return -6;
    }

    cm_free(data);
    cm_log_printf(0,"cm_full_system_http_fota cm_ota_upgrade success! waiting reboot...\n");
    osDelay(2000 / 5);

    return 0;
}

/**
 *  @brief FOTA主线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 用于进行一次FOTA升级，根据宏定义选项进行最小系统FOTA升级、全系统差分升级/APP整包升级
 */
void cm_fota_demo_main(void)
{
    cm_log_printf(0,"version 1\n"); //为验证FOTA 可以加入版本的标识
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

    int result = 0;
    if (APP_DEMO_FOTA_MODE == 0)
    {
        result = cm_mini_system_http_fota(APP_DEMO_FOTA_URL);
    }
    else
    {
        result = cm_full_system_http_fota(APP_DEMO_FOTA_URL);
    }
    cm_log_printf(0,"app_http_fota_demo_func fota result FOTA MODE:%d,result:%d\n", APP_DEMO_FOTA_MODE, result);

    while(1)
    {
        osDelay(1000);
    }
}
/**
 *  @brief FOTA接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始FOTA相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    cm_fota_demo_log_to_usb();

    /*创建FOTA测试线程*/
    osThreadAttr_t fota_main_task_attr = {0};
    fota_main_task_attr.name = "fota_demo_main_task";
    fota_main_task_attr.stack_size = 10 * 1024;
    fota_main_task_attr.priority = osPriorityNormal;

    if (CM_FOTA_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_FOTA_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_fota_demo_main, 0, &fota_main_task_attr);
        if (CM_FOTA_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_FOTA_DEMO_MAIN_TASKHANDLE is NULL\n");
        }
    }

    return 0;
}
