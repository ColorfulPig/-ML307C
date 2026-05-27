/**
 * @file        ctwing.c
 * @brief       使用lwm2m连接ctwing物模型并将gnss数据上报物模型，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by shimingrui
 */

#include <string.h>
#include "stdio.h"
#include "ctwing.h"
#include "cm_lwm2m.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_rtc.h"
#include "cm_os.h"
#include "cJSON.h"
#include "cm_pm.h"
#include "cm_modem.h"


static cm_lwm2m_handle_t lwm2m_dev = NULL;
static uint8_t test_payload[1024]     = {0};
static int32_t conn_flag = 0;
static float latitude = 0;
static float longitude = 0;
static float altitude = 0;

static void __cm_lwm2m_event_cb(int32_t event, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]event:%d\n", param.handle, (char *)param.cb_param, event);
    switch(event)
    {
        case CM_LWM2M_EVENT_REG_SUCCESS: 
        {
            cm_log_printf(0, "lwm2m register ok\n");
            conn_flag = 1;
        }
        break;
        case CM_LWM2M_EVENT_REG_FAILED:
        {
            cm_log_printf(0, "lwm2m register fail\n");
            conn_flag = -1;
        }
        break;
        case CM_LWM2M_EVENT_UNREG_DONE:
        {
            cm_log_printf(0, "lwm2m unregister done\n");
        }
        break;
        default:
        break;
    }
}

static void __cm_lwm2m_notify_cb(int32_t mid, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]mid:%d\n", param.handle, (char *)param.cb_param, mid);
}

static void __cm_lwm2m_read_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]mid:%d objid:%d insid:%d resid:%d\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid);
}

static void __cm_lwm2m_write_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, int32_t type, int32_t is_over, char *data, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]mid:%d objid:%d insid:%d resid:%d type:%d is_over:%d len:%d data:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, type, is_over, len, data);

    int32_t ret = cm_lwm2m_write_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_log_printf(0, "cm_lwm2m_write_rsp() fail, ret is %d\n", ret);
        return;
    }
}

static void __cm_lwm2m_execute_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, char *data, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]mid:%d objid:%d insid:%d resid:%d len:%d len:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, len, data);

    int32_t ret = cm_lwm2m_execute_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_log_printf(0, "cm_lwm2m_execute_rsp() fail, ret is %d\n", ret);
        return;
    }
}

static void __cm_lwm2m_observe_cb(int32_t mid, int32_t observe, int32_t objid, int32_t insid, int32_t resid, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]%d_%d_%d: %d\n", param.handle, (char *)param.cb_param, objid, insid, resid, observe);
}

static void __cm_lwm2m_discover_cb(int32_t mid, int32_t objid, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]discover mid:%d objid:%d \n", param.handle, (char *)param.cb_param, mid, objid);
}

static void __cm_lwm2m_params_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, char *parameter, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%d %s]discover mid:%d objid:%d insid:%d resid:%d len:%d parameter:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, len, parameter);

    int32_t ret = cm_lwm2m_param_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_log_printf(0, "cm_lwm2m_param_rsp() fail, ret is %d\n", ret);
        return;
    }
}

static int32_t __ctwing_tm_prop_geolocation_notify(float latitude, float longitude, float altitude)
{
    void *structure = (void*)cJSON_CreateObject();
    int32_t ret = 0;
    uint64_t time_stamp = cm_rtc_get_current_time();
    uint8_t* str         = NULL;

    cJSON_AddStringToObject((cJSON*)structure, "serviceId", "4");
    cJSON_AddItemToObject((cJSON*)structure, "longitude", cJSON_CreateNumber(longitude));
    cJSON_AddItemToObject((cJSON*)structure, "latitude", cJSON_CreateNumber(latitude));
    cJSON_AddItemToObject((cJSON*)structure, "altitude", cJSON_CreateNumber(altitude));
    cJSON_AddItemToObject((cJSON*)structure, "time", cJSON_CreateNumber(time_stamp));

    memset(test_payload, 0, 1024);

    if (NULL != (str = (uint8_t*)cJSON_PrintUnformatted(structure))) {
        strcat((char *)test_payload, (const char *)str);
        cm_free(str);
    }
    cJSON_Delete(structure);
    cm_log_printf(0, "tm_ctwing payload %s\n", test_payload);

    return ret;
}

void cm_ctwing_lwm2m_tm_test(void)
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
    
    int32_t ret = -1;
    uint32_t pup_count = 0;
    
    if (NULL == lwm2m_dev)
    {
        cm_lwm2m_cfg_t cfg = {0};
        cfg.platform = CM_LWM2M_CTWING;
        cfg.endpoint.pattern = 1;
        cfg.endpoint.name = NULL;
        cfg.host = "221.229.214.202";
        cfg.flag = 0;
        cfg.auth_code = NULL;
        cfg.psk = NULL;
        cfg.pskid = NULL;
        cfg.auto_update = 1;
        cfg.cb.onRead = __cm_lwm2m_read_cb;
        cfg.cb.onWrite = __cm_lwm2m_write_cb;
        cfg.cb.onExec = __cm_lwm2m_execute_cb;
        cfg.cb.onObserve = __cm_lwm2m_observe_cb;
        cfg.cb.onParams = __cm_lwm2m_params_cb;
        cfg.cb.onEvent = __cm_lwm2m_event_cb;
        cfg.cb.onNotify = __cm_lwm2m_notify_cb;
        cfg.cb.onDiscover = __cm_lwm2m_discover_cb;
        cfg.cb_param = "CTWing";

        ret = cm_lwm2m_create(&cfg, &lwm2m_dev);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_create() fail, ret is %d\n", ret);
            return;
        }

        uint8_t instances_19[1] = {1};
        ret = cm_lwm2m_add_obj(lwm2m_dev, 19, instances_19, 1, 0, 0);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_add_obj() obj 19 fail, ret is %d\n", ret);
            return;
        }

        int32_t resoures_19[1] = {0};
        ret = cm_lwm2m_discover(lwm2m_dev, 19, resoures_19, 1);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_discover() obj 19 fail, ret is %d\n", ret);
            return;
        }

        conn_flag = 0;
        ret = cm_lwm2m_open(lwm2m_dev, 30, 86400);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_open() fail, ret is %d\n", ret);
            return;
        }

        /* 等待ctwing连接成功 */
        while (!conn_flag)
        {
            osDelay(1);
        }
        if (conn_flag != 1)
        {
            cm_log_printf(0, "[CTWing] conn err\n");
            return;
        }

        cm_log_printf(0, "[CTWing] onenet_dev[%d] CHECKIN SUCC\n", lwm2m_dev);
        
        while (pup_count < 10)
        {
            /* 获取gnss定位数据 此处使用模拟获取的定位数据，可以使用带定位功能的模组或者外接定位模块来获取真实的定位数据并在此处替换 */
            latitude = 30.34;
            longitude = 30.34;
            altitude = 30.34;

            /* 对定位数据进行JSON组包 */
            __ctwing_tm_prop_geolocation_notify(latitude, longitude, altitude);

            /* lwm2m组包 */
            ret = cm_lwm2m_notify_packing(lwm2m_dev, 19, 0, 0, 1, (char *)test_payload, strlen((const char *)test_payload), 1);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_log_printf(0, "[CTWing] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }
            static int mid = 1;

            /* 上报数据给物模型 */
            ret = cm_lwm2m_notify(lwm2m_dev, mid++);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_log_printf(0, "[CTWing] cm_lwm2m_notify() fail, ret is %d\n", ret);
                return;
            }

            pup_count++;

            osDelay(1000);
        }

        /* 关闭连接并释放资源 */
        ret = cm_lwm2m_del_obj(lwm2m_dev, 19);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_del_obj() 19 fail, ret is %d\n", ret);
            return;
        }

        ret = cm_lwm2m_close(lwm2m_dev);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_close() fail, ret is %d\n", ret);
            return;
        }

        ret = cm_lwm2m_delete(lwm2m_dev);

        if (CM_LWM2M_SUCCESS != ret)
        {
            cm_log_printf(0, "[CTWing] cm_lwm2m_delete() fail, ret is %d\n", ret);
            return;
        }

        lwm2m_dev = NULL;
    }
}

/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_ctwing_lwm2m_tm_test, 0, &app_task_attr);
    return 0;
}
