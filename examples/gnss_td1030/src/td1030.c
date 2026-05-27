/**
 * @copyright Copyright © 2024 China Mobile IOT. All rights reserved.
 * 
 * @file td1030.c
 * @brief TD1030 GPS数据解析
 * @author zhaoji
 * @version 1.0
 * @date 2024-11-18
 */

#include "stdio.h"
#include "stdint.h"
#include "stdlib.h"
#include "stdbool.h"
#include "string.h"
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_uart.h"
#include "cm_gpio.h"
#include "cm_iomux.h"
#include "cm_mem.h"
#include "td1030.h"

static osSemaphoreId_t s_uart_semaphore = NULL;     //串口数据通知信号量
static osMutexId_t s_gnss_mutex = NULL;
static uint8_t s_gnss_recv_buf[TD1030_DEMO_RX_SIZE] = {0};
static uint16_t s_gnss_recv_len = 0;
static uint8_t s_gnss_recv_temp_buf[TD1030_DEMO_RX_SIZE] = {0};


/**
 * @brief 字符串提取（起始、结束关键字中间内容）
 * 
 * @param buf 获取内容缓冲区
 * @param bufsize 获取内容缓冲区大小
 * @param source_str 待提取原始字符串
 * @param start_str 起始关键字
 * @param end_str 结束关键字
 * @return 提取内容长度
 */
int app_demo_str_extract(uint8_t *buf, size_t bufsize, uint8_t *source_str, uint8_t *start_str, uint8_t *end_str)
{
    char *l_startstr = NULL, *l_endstr = NULL;
    int len = 0, num = 0, offset = 0;

    if(buf == NULL || bufsize <= 0 || source_str == NULL || start_str == NULL || end_str == NULL)
    {
        return 0;
    }

    if (strstr((const char *)source_str, (const char *)start_str) == NULL || strstr((const char *)source_str, (const char *)end_str) == NULL)
    {
        return 0;
    }

    l_startstr = strstr((const char *)source_str, (const char *)start_str);
    l_endstr = strstr((const char *)l_startstr, (const char *)end_str);

    if (l_endstr == NULL)
    {
        return 0;
    }

    offset = strlen((const char *)start_str);
    len = strlen((const char *)l_startstr) - strlen((const char *)l_endstr)- offset;
    if(bufsize < len)  //缓冲区内存不足
    {
        return 0;
    }
    
    for (num = 0; num < len; num++)
    {
        *(buf + num) = *(l_startstr + offset + num);
    }

    *(buf + num) = '\0';

    return num;
}


/**
 * @brief 将经纬度的度分格式转换为度格式(保留6位小数)
 * 
 * @param r_longitude 度格式经度字符串
 * @param r_latitude 度格式纬度字符串
 * @param longitude 度分格式经度字符串
 * @param latitude 度分格式纬度字符串
 */
void td1030_demo_location_info_transform(uint8_t *r_longitude, uint8_t *r_latitude, uint8_t *longitude, uint8_t *latitude)
{
    if (longitude == NULL || latitude == NULL || strlen((const char *)longitude) == 0 || strlen((const char *)latitude) == 0)
    {
        return;
    }
    float longitude_value = 0, latitude_value = 0;
    longitude_value = atof((const char *)longitude);
    latitude_value = atof((const char *)latitude);
    /* 经度:11414.44280->114+14.44280/60=114.247013 */
    longitude_value = (uint16_t)(longitude_value / 100) + (longitude_value - (uint16_t)(longitude_value / 100) * 100) / 60;
    sprintf((char *)r_longitude, "%.6f", longitude_value);
    /* 纬度：3037.65695->30+37.65695/60=30.627615 */
    latitude_value = (uint16_t)(latitude_value / 100) + (latitude_value - (uint16_t)(latitude_value / 100) * 100) / 60;
    sprintf((char *)r_latitude, "%.6f", latitude_value);
}


/**
 * @brief 从NMEA数据中获取指定GNSS数据
 * 
 * @param data 获取数据缓存区
 * @param nmea_data NMEA数据
 * @param keyword NMEA关键字
 * @param num 关键字对应行中的第几个数据
 * @return int 
 */
int td1030_demo_get_data_from_nmea(uint8_t *data, uint8_t *nmea_data, uint8_t *keyword, int num)
{
    if(nmea_data == NULL || data == NULL || num == 0)
    {
        return 0;
    }

    uint8_t keyword_data[512] = {0};
    /* 获取关键字数据(nmea数据的每一行都是以\r\n结尾) */
    int keyword_len = app_demo_str_extract(keyword_data, sizeof(keyword_data), nmea_data, keyword, (uint8_t *)"\r\n");
    if(keyword_len == 0)
    {
        cm_log_printf(0, "get keyword fail.");
        return 0;
    }

    int i = 0,j = 0;
    int comma_id[128] = {0};    /* 记录逗号和星号的位置 */
    for ( i = 0, j = 0; i < keyword_len; i++ )
    {
        if ( keyword_data[i] == ',' || keyword_data[i] == '*')
        {
            comma_id[j] = i;    /* 记录‘,’与‘*’的位置 */
            j++;
        }
    }
    for(i = comma_id[num - 1] + 1,j = 0;i < comma_id[num];i ++,j++)
    {
        *(data + j) = keyword_data[i];  /* 将逗号之间的数据截取出来 */
    }
    *(data + j) = '\0';
    cm_log_printf(0, "get keyword[\'%s\'] val: %s", keyword, data);
    return j;
}


/**
 * @brief 解析NMEA数据
 * 
 * @param nmea_data NMEA数据
 * @param gnss_info GNSS定位参数
 */
void td1030_demo_gnss_info_get(uint8_t *nmea_data, gnss_info_t *gnss_info)
{
    // 获取定位状态
    uint8_t state[5] = {0};
    td1030_demo_get_data_from_nmea(state, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 2);
    if (state[0] == 'A')
    {
        gnss_info->state = 1;
    }
    else
    {
        memset(gnss_info, 0, sizeof(gnss_info_t));
        gnss_info->state = 0;
        return;
    }

    // 获取经纬度
    uint8_t longitude_str[10] = {0}, latitude_str[10] = {0}, r_longitude_str[10] = {0}, r_latitude_str[10] = {0};
    uint8_t e_w_str[5] = {0}, n_s_str[5] = {0};
    td1030_demo_get_data_from_nmea(latitude_str, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 3);  // 获取纬度
    td1030_demo_get_data_from_nmea(n_s_str, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 4);       // 获取南北纬
    td1030_demo_get_data_from_nmea(longitude_str, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 5); // 获取经度
    td1030_demo_get_data_from_nmea(e_w_str, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 6);       // 获取东西经
    td1030_demo_location_info_transform(r_longitude_str, r_latitude_str, longitude_str, latitude_str); // 度分格式转换为度格式
    gnss_info->N_S = n_s_str[0];
    gnss_info->latitude = atof((char *)r_latitude_str);
    gnss_info->E_W = e_w_str[0];
    gnss_info->longitude = atof((char *)r_longitude_str);

    // 速度
    uint8_t speed_str[10] = {0};
    td1030_demo_get_data_from_nmea(speed_str, nmea_data, (uint8_t *)NMEA_KEYWORD_RMC, 7);
    gnss_info->speed = atof((char *)speed_str) * 1.852;

    // 定位精度，PDOP位置精度因子
    uint8_t accuracy_str[5] = {0};
    td1030_demo_get_data_from_nmea(accuracy_str, nmea_data, (uint8_t *)NMEA_KEYWORD_GSA, 15);
    gnss_info->accuracy = atof((char *)accuracy_str);

    // 可见卫星数
    uint8_t num_str[5] = {0};
    td1030_demo_get_data_from_nmea(num_str, nmea_data, (uint8_t *)NMEA_KEYWORD_GSV, 3);
    gnss_info->sate_num = atoi((const char *)num_str);
}


/**
 * @brief GNSS串口接收回调
 * 
 * @param data NMEA数据缓存区
 * @param datalen NMEA数据长度
 */
void _td1030_demo_gnss_uart_callback(void *param, uint32_t type)
{
    if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
    {
        /* 收到接收事件，触发线程执行读取数据 */
        osSemaphoreRelease(s_uart_semaphore);
    }
}


/**
 * @brief UART接收任务
 * 
 */
void td1030_uart_recv_task()
{
    uint8_t *buf = cm_malloc(TD1030_DEMO_RX_SIZE);
    uint16_t recv_buflen = 0, single_recv_buflen = 0;
    while (1)
    {
        memset(buf, 0, TD1030_DEMO_RX_SIZE);
        recv_buflen = 0;
        osSemaphoreAcquire(s_uart_semaphore, osWaitForever); // 阻塞
        while (1)
        {
            single_recv_buflen = cm_uart_read(TD1030_DEMO_UART, (buf + recv_buflen), TD1030_DEMO_RX_SIZE - recv_buflen, 100);
            if (single_recv_buflen <= 0)   // 第一次检查无串口数据或出现异常
            {
                if (recv_buflen == 0)     // 无串口数据
                {
                    break;
                }
                osDelay(100 / 5);
                single_recv_buflen = cm_uart_read(TD1030_DEMO_UART, (buf + recv_buflen), TD1030_DEMO_RX_SIZE - recv_buflen, 100);
                if (single_recv_buflen <= 0 && recv_buflen > 0)    // 无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
                {
                    // cm_uart_write(TD1030_DEMO_UART, buf, recv_buflen, 0);
                    osMutexAcquire(s_gnss_mutex, osWaitForever);
                    memset(s_gnss_recv_buf, 0, TD1030_DEMO_RX_SIZE);
                    memcpy(s_gnss_recv_buf, buf, recv_buflen);
                    s_gnss_recv_len = recv_buflen;
                    osMutexRelease(s_gnss_mutex);
                    break;
                }
            }

            recv_buflen += single_recv_buflen;

            /* 到达buf空间后，重新接收 */
            if (recv_buflen >= TD1030_DEMO_RX_SIZE)
            {
                recv_buflen = 0;
                break;
            }
        }
    }
}


/**
 * @brief GNSS初始化
 * 
 * @details 此处仅包含串口初始化，若需GPIO使能GNSS电源则自行实现
 */
void td1030_demo_init()
{
    int32_t ret = -1;

    /* 初始化互斥锁 */
    osMutexAttr_t mutex_attr = {"mutex_attr", osMutexRecursive | osMutexPrioInherit, 0, 0U};
    s_gnss_mutex = osMutexNew(&mutex_attr);
    if (s_gnss_mutex == NULL)
    {
        cm_log_printf(0, "gnss mutex create fail.");
    }

    /* 初始化信号量 */
    s_uart_semaphore = osSemaphoreNew(1, 0, NULL);

    /* 串口配置参数 */
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8,
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE,
        CM_UART_FLOW_CTRL_NONE,
        CM_UART_BAUDRATE_9600,
        0,   // 配置为普通串口模式，若要配置为低功耗模式可改为1
        0,   // 环形缓存区大小按照默认配置8k
        0,
        0,
    };

    /* 事件参数 */
    cm_uart_event_t event = 
    {
        CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   //注册需要上报的事件类型
        "gnss_uart",                                                        //用户参数
        _td1030_demo_gnss_uart_callback                                 //上报事件的回调函数
    };

    /* 配置引脚复用 */
    if (TD1030_DEMO_UART == CM_UART_DEV_0)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1);
    }
    else if (TD1030_DEMO_UART == CM_UART_DEV_1)
    {
        cm_iomux_set_pin_func(CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1);
        cm_iomux_set_pin_func(CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1);
    }
    else
    {
        cm_log_printf(0, "invalid gnss uart num: %d", TD1030_DEMO_UART);
        return;
    }

    /* 注册事件和回调函数 */
    ret = cm_uart_register_event(TD1030_DEMO_UART, &event);
    if (ret != 0)
    {
        cm_log_printf(0, "uart register event err, ret: %d", ret);
        return;
    }
    
    /* 开启串口 */
    ret = cm_uart_open(TD1030_DEMO_UART, &config);
    if (ret != 0)
    {
        cm_log_printf(0, "gnss uart init err, ret: %d", ret);
        return;
    }
}


/**
 * @brief GNSS测试任务
 * 
 */
void td1030_demo_task()
{
    td1030_demo_init();

    while (1)
    {
        osDelay(1000 / 5);

        osMutexAcquire(s_gnss_mutex, osWaitForever);
        memcpy(s_gnss_recv_temp_buf, s_gnss_recv_buf, s_gnss_recv_len);
        osMutexRelease(s_gnss_mutex);

        gnss_info_t gnss_info = {0};
        td1030_demo_gnss_info_get(s_gnss_recv_temp_buf, &gnss_info);
        if (gnss_info.state)
        {
            cm_log_printf(0, "---------------- gnss Location succ ----------------");
            cm_log_printf(0, "---- lng: %.6lf(%C)", gnss_info.longitude, gnss_info.E_W);
            cm_log_printf(0, "---- lng: %.6lf(%C)", gnss_info.latitude, gnss_info.N_S);
            cm_log_printf(0, "---- accuracy: %lf", gnss_info.accuracy);
            cm_log_printf(0, "---- speed: %lf", gnss_info.speed);
            cm_log_printf(0, "---- sate_num: %02d", gnss_info.sate_num);
            cm_log_printf(0, "---------------- gnss Location end -----------------");
        }
        else
        {
            cm_log_printf(0, "gnss location fail.");
        }
    }
}


/**
 * @brief OC入口函数
 * 
 * @param arg 
 * @return int 
 */
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t gnss_task_attr = {0};
    gnss_task_attr.name = "td1030_demo_task";  // 任务名称
    gnss_task_attr.stack_size = 1024 * 10; // 任务栈空间
    gnss_task_attr.priority= osPriorityNormal;    // 任务优先级
    osThreadNew((osThreadFunc_t)td1030_demo_task, 0, &gnss_task_attr);  // 创建并运行任务

    osThreadNew((osThreadFunc_t)td1030_uart_recv_task, 0, &gnss_task_attr);  // 创建并运行任务
    return 0;
}


// end of app_demo_gnss.c