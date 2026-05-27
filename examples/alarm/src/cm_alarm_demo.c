/**
 * @file        cm_alarm_demo.c
 * @brief       本示例使用ALARM完成定时闹钟功能
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

#include "cm_sys.h"
#include "cm_os.h"
#include "cm_rtc.h"
#include "cm_pm.h"
#include "cm_modem.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
/*注意：闹钟暂不支持定时开机功能*/
#define SECOND_OF_DAY (24 * 60 * 60)
static osThreadId_t CM_ALARM_DEMO_MAIN_TASKHANDLE = NULL; // 闹钟测试主线程

#define CM_ALARM_DEMO_WAKEUP_TIME_S 60 // 闹钟定时时间，基于现在的时间加上该定时时间，为闹钟唤醒的时间

/****************************************************************************
 * Public Types
 ****************************************************************************/
static const char DayOfMon[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

/**
 *  @brief 秒转换为日期
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 
 */
static void cm_sec_to_date(long lSec, cm_tm_t *tTime)
{
    unsigned short i, j, iDay;
    unsigned long lDay;

    lDay = lSec / SECOND_OF_DAY;
    lSec = lSec % SECOND_OF_DAY;

    i = 1970;
    while (lDay > 365)
    {
        if (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0))
        {
            lDay -= 366;
        }
        else
        {
            lDay -= 365;
        }
        i++;
    }
    if ((lDay == 365) && !(((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0)))
    {
        lDay -= 365;
        i++;
    }
    tTime->tm_year = i;
    for (j = 0; j < 12; j++)
    {
        if ((j == 1) && (((i % 4 == 0) && (i % 100 != 0)) || (i % 400 == 0)))
        {
            iDay = 29;
        }
        else
        {
            iDay = DayOfMon[j];
        }
        if (lDay >= iDay)
            lDay -= iDay;
        else
            break;
    }
    tTime->tm_mon = j + 1;
    tTime->tm_mday = lDay + 1;
    tTime->tm_hour = ((lSec / 3600)) % 24; // 这里注意，世界时间已经加上北京时间差8，
    tTime->tm_min = (lSec % 3600) / 60;
    tTime->tm_sec = (lSec % 3600) % 60;
}

/**
 *  @brief 闹钟定时到达回调
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 
 */
static void cm_alarm_demo_callback(void)
{
    cm_log_printf(0, "ALARM ARRIVED!\n");
}

/**
 *  @brief ALARM主线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 开启本线程，等待网络同步时间后，定时1分钟等待闹钟唤醒
 */
static void cm_alarm_demo_main(void)
{
    /*打印开机原因*/
    cm_log_printf(0, "reset reason:%d", cm_pm_get_power_on_reason());

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

    /*等待附着网络，自动同步网络时间至模组RTC中，并计算唤醒后的绝对时间*/
    uint64_t base_time = cm_rtc_get_current_time() + cm_rtc_get_timezone() * 60 * 60;
    cm_log_printf(0, "base_time:%llu\n", base_time); // 注意需要用%llu打印uint64_t

    uint64_t wakeup_time = base_time + CM_ALARM_DEMO_WAKEUP_TIME_S;
    cm_log_printf(0, "wakeup_time:%llu\n", wakeup_time); // 注意需要用%llu打印uint64_t

    cm_tm_t wakeup_time_t = {0};
    cm_sec_to_date(wakeup_time, &wakeup_time_t); // 转换成年月日时分秒

    /*设置闹钟*/
    cm_rtc_register_alarm_cb(cm_alarm_demo_callback);

    if (cm_rtc_set_alarm(&wakeup_time_t) < 0)
    {
        cm_log_printf(0, "cm_rtc_set_alarm set failed! system reboot...\n");
        cm_pm_reboot();
        osDelay(1000);
    }

    cm_rtc_enable_alarm(true); // 打开闹钟  当闹钟到达时会触发回调

    cm_log_printf(0, "START ALARM! WAITING ARRIVE...\n");

    while (1)
    {
        osDelay(1000);
    }
}
/**
 *  @brief ALARM接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始ALARM相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*创建MQTT数据接收/发送处理线程*/
    osThreadAttr_t alarm_main_task_attr = {0};
    alarm_main_task_attr.name = "alarm_demo_main_task";
    alarm_main_task_attr.stack_size = 10 * 1024;
    alarm_main_task_attr.priority = osPriorityNormal;

    if (CM_ALARM_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_ALARM_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_alarm_demo_main, 0, &alarm_main_task_attr);
        if (CM_ALARM_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_ALARM_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    return 0;
}
