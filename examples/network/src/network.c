/**
 * @file        network.c
 * @brief       网络状态异步回调通知，示例仅供参考
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
#include "network.h"
#include "cm_pm.h"
#include "cm_modem.h"

#define CM_NETWORK_WAIT_SIM_READY 10         // 等待SIM卡识别的超时时间（秒）
#define CM_NETWORK_WAIT_REGISTER_TIMEOUT 300 // 等待网络注册的超时时间（秒）
#define CM_NETWORK_WAIT_ACTIVE_TIMEOUT 60    // 等待网络激活的超时时间（秒）
#define CM_NETWORK_CHECK_PERIOD 10           // 网络检查的周期（秒）


static cm_network_event_cb cm_network_event_callback = NULL;

/**
 * @brief  等待SIM卡识别
 *
 * @param [in] timeout_s 超时时间(秒)
 *
 * @return  void
 *
 * @details More details
 */
bool cm_network_wait_sim_ready(uint16_t timeout_s)
{
    uint16_t retry = 0;
    while (retry < timeout_s)
    {
        if(cm_modem_get_cpin() >= 0)
        {
            return true;
        }
        retry++;
        osDelay(200);
    }
    return false;
}

/**
 * @brief  等待网络注册
 *
 * @param [in] timeout_s 超时时间(秒)
 *
 * @return  void
 *
 * @details More details
 */
bool cm_network_wait_register(uint16_t timeout_s)
{
    uint16_t retry = 0;
    cm_cereg_state_t cereg_state = {0};
    while (retry < timeout_s)
    {
        if (cm_modem_get_cereg_state(&cereg_state) == 0 && (cereg_state.state == 1 || cereg_state.state == 5))
        {
            return true;
        }
        retry++;
        osDelay(200);
    }
    return false;
}


/**
 * @brief  等待网络激活
 *
 * @param [in] timeout_s 超时时间(秒)
 *
 * @return  void
 *
 * @details More details
 */
bool cm_network_wait_active(uint16_t timeout_s)
{
    uint16_t retry = 0;
    while (retry < timeout_s)
    {
        if (cm_modem_get_pdp_state(1) == 1)
        {
            return true;
        }
        retry++;
        osDelay(200);
    }
    return false;
}


/**
 * @brief  网络维护线程
 *
 * @param [in] p 线程参数
 *
 * @return  void
 *
 * @details More details
 */
void cm_network_maintain_task(void *p)
{
    int sim_previous_status = -1;    //SIM卡前期状态标志位
    int network_previous_status = 0;    //网络前期状态标志位

    /* 1.检查CFUN是否打开(不打开cfun，sim卡无法读到) */
    if(cm_modem_get_cfun() != 1)
    {
        cm_modem_set_cfun(1);
    }
    cm_log_printf(0,"[1]cfun turn on");

    /*  2.检查SIM卡 */
    if (cm_network_wait_sim_ready(CM_NETWORK_WAIT_SIM_READY))
    {
        sim_previous_status = 1;
        cm_network_event_callback(CM_NETWORK_EVENT_SIM_READY);
        cm_log_printf(0, "[2]sim is ready");
    }
    else
    {
        sim_previous_status = 0;
        cm_network_event_callback(CM_NETWORK_EVENT_NO_SIM);
    }

    /* 3.设置apn，通用卡可跳过该步骤；
    专网卡请使用虚拟AT发送以下指令设置APN: AT+CGDCONT=1,"IPV4V6","SIM卡APN名称" */


    /* 4.检查基站的注册状态 */
    if(cm_network_wait_register(CM_NETWORK_WAIT_REGISTER_TIMEOUT))
    {
        cm_log_printf(0,"[3]The network has been registered successfully");
    }

    /* 5.检查网络的激活状态 */
    if(cm_network_wait_active(CM_NETWORK_WAIT_ACTIVE_TIMEOUT))
    {
        network_previous_status = 1;    //网络激活
        osDelay(200);
        cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVED);
        cm_log_printf(0,"[4]The network has been actived successfully");
    }
    else
    {
        network_previous_status = 0;    //网络激活失败
        cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVE_FAIL);
        cm_log_printf(0,"[4]The network actived fail");
    }

    /* 6.开始网络状态监测 */
    cm_cereg_state_t cereg_state = {0};
    while(1)
    {
        if (cm_modem_get_cereg_state(&cereg_state) == 0 && (cereg_state.state == 1 || cereg_state.state == 5) && cm_modem_get_pdp_state(1) == 1)
        {
            if(network_previous_status == 0)
            {
                network_previous_status = 1;    //网络激活
                osDelay(200);
                cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVED);
                cm_log_printf(0,"The network has been actived successfully");
            }
        }
        else
        {
            if(network_previous_status == 1)
            {
                network_previous_status = 0;    //网络失活
                cm_network_event_callback(CM_NETWORK_EVENT_PDP_DEACTIVED);
                cm_log_printf(0,"The network deactived");
            }
            else
            {
                /* 1.检查CFUN是否打开(不打开cfun，sim卡无法读到) */
                if (cm_modem_get_cfun() != 1)
                {
                    cm_modem_set_cfun(1);
                }

                /*  2.检查SIM卡 */
                if (cm_network_wait_sim_ready(CM_NETWORK_WAIT_SIM_READY))
                {
                    if(sim_previous_status != 1)
                    {
                        sim_previous_status = 1;
                        cm_network_event_callback(CM_NETWORK_EVENT_SIM_READY);
                    }
                }   
                else
                {
                    if(sim_previous_status != 0)
                    {
                        sim_previous_status = 0;
                        cm_network_event_callback(CM_NETWORK_EVENT_NO_SIM);
                    }
                }

                /* 4.检查基站的注册状态 */
                cm_network_wait_register(CM_NETWORK_WAIT_REGISTER_TIMEOUT);

                /* 5.检查网络的激活状态 */
                if (cm_network_wait_active(CM_NETWORK_WAIT_ACTIVE_TIMEOUT))
                {
                    network_previous_status = 1; // 网络激活
                    osDelay(200);
                    cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVED);
                }
                else
                {
                    network_previous_status = 0; // 网络激活失败
                    cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVE_FAIL);
                }
            }
        }

        osDelay(CM_NETWORK_CHECK_PERIOD * 200);  //每10s检查一下网络状态，用户可自行更改
    }
}


/**
 * @brief  网络维护初始化
 *
 * @param [int] network_event_cb 注册网络状态通知回调函数
 *
 * @return  void
 *
 * @details More details
 */  
void cm_network_init(cm_network_event_cb network_event_cb)
{
    cm_network_event_callback = network_event_cb;

    /* 创建网络维护线程 */
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "maintain_task";
    app_task_attr.stack_size = 1024 * 4;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_network_maintain_task, 0, &app_task_attr);
}


/**
 * @brief  网络状态通知回调
 *
 * @param [out] state 网络状态
 *
 * @return  void
 *
 * @details More details
 */  
void app_network_event_cb(cm_network_event_e state)
{
    switch (state)
    {
        /* 无法识别SIM卡 */
        case CM_NETWORK_EVENT_NO_SIM:
        {
            cm_log_printf(0,"CM_NETWORK_EVENT_NO_SIM");
            //SIM未识别，也可尝试重启
            // cm_log_printf(0,"sim is not ready,wiil reboot....");
            // osDelay(10);
            // cm_pm_reboot();
            break;
        }
        /* 识别到SIM卡 */
        case CM_NETWORK_EVENT_SIM_READY:
        {
            cm_log_printf(0,"CM_NETWORK_EVENT_SIM_READY");
            break;
        }
        /* 网络激活成功 */
        case CM_NETWORK_EVENT_PDP_ACTIVED:
        {
            cm_log_printf(0,"CM_NETWORK_EVENT_PDP_ACTIVED");
            break;
        }
        /* 网络失活，需要等待重新激活 */
        case CM_NETWORK_EVENT_PDP_DEACTIVED:
        {
            cm_log_printf(0,"CM_NETWORK_EVENT_PDP_DEACTIVED");
            break;
        }
        /* 网络激活失败 */
        case CM_NETWORK_EVENT_PDP_ACTIVE_FAIL:
        {
            cm_log_printf(0,"CM_NETWORK_EVENT_PDP_ACTIVE_FAIL");
            //激活失败可尝试重启（ 因SIM卡欠费导致模组正常业务流程中断时，充值完成后需复位模组；或执行AT+CFUN=0、AT+CFUN=1命令重新驻网。模组不会自动重新搜网）
            cm_log_printf(0,"network active fail,wiil reboot....");
            osDelay(10);
            cm_pm_reboot();
            break;
        }
        default:
            break;
    }
}

/* 网络测试线程 */
void cm_network_task_test(void *p)
{
    cm_network_init(app_network_event_cb);
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
    app_task_attr.name  = "test_task";
    app_task_attr.stack_size = 1024 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_network_task_test, 0, &app_task_attr);
    return 0;
}