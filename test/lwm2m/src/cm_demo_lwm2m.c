/**
 * @file         cm_demo_lwm2m.c
 * @brief        lwm2m接口
 * @copyright    Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author       cmiot3000
 * @date         2024/03/22
 *
 */

#include "stdio.h"
#include "cm_lwm2m.h"
#include "cm_demo_uart.h"

static cm_lwm2m_handle_t onenet_dev = NULL;         //OneNET设备

static void __cm_lwm2m_event_cb(int32_t event, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]event:%d\n", param.handle, (char *)param.cb_param, event);
    switch(event)
    {
        case CM_LWM2M_EVENT_REG_SUCCESS: 
        {
            cm_demo_printf("lwm2m register ok\n");
        }
        break;
        case CM_LWM2M_EVENT_REG_FAILED:
        {
            cm_demo_printf("lwm2m register fail\n");
        }
        break;
        case CM_LWM2M_EVENT_UNREG_DONE:
        {
            cm_demo_printf("lwm2m unregister done\n");
        }
        break;
        default:
        break;
    }
}

static void __cm_lwm2m_notify_cb(int32_t mid, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]mid:%d\n", param.handle, (char *)param.cb_param, mid);
}

static void __cm_lwm2m_read_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]mid:%d objid:%d insid:%d resid:%d\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid);
}

static void __cm_lwm2m_write_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, int32_t type, int32_t is_over, char *data, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]mid:%d objid:%d insid:%d resid:%d type:%d is_over:%d len:%d data:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, type, is_over, len, data);

    int32_t ret = cm_lwm2m_write_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_demo_printf("cm_lwm2m_write_rsp() fail, ret is %d\n", ret);
        return;
    }
}

static void __cm_lwm2m_execute_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, char *data, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]mid:%d objid:%d insid:%d resid:%d len:%d len:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, len, data);

    int32_t ret = cm_lwm2m_execute_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_demo_printf("cm_lwm2m_execute_rsp() fail, ret is %d\n", ret);
        return;
    }
}

static void __cm_lwm2m_observe_cb(int32_t mid, int32_t observe, int32_t objid, int32_t insid, int32_t resid, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]%d_%d_%d: %d\n", param.handle, (char *)param.cb_param, objid, insid, resid, observe);
}

static void __cm_lwm2m_discover_cb(int32_t mid, int32_t objid, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]discover mid:%d objid:%d \n", param.handle, (char *)param.cb_param, mid, objid);
}

static void __cm_lwm2m_params_cb(int32_t mid, int32_t objid, int32_t insid, int32_t resid, char *parameter, int32_t len, cm_lwm2m_cb_param_t param)
{
    cm_demo_printf("[%d %s]discover mid:%d objid:%d insid:%d resid:%d len:%d parameter:%s\n", param.handle, (char *)param.cb_param, mid, objid, insid, resid, len, parameter);

    int32_t ret = cm_lwm2m_param_rsp(param.handle, mid, CM_LWM2M_RESULT_204_CHANGED);

    if (CM_LWM2M_SUCCESS != ret)
    {
        cm_demo_printf("cm_lwm2m_param_rsp() fail, ret is %d\n", ret);
        return;
    }
}

/**
 *  UART口OneNET功能功能调试使用示例
 */
void cm_test_onenet(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);
    int32_t ret = -1;

    if (0 == strcmp((const char *)operation, "CHECKIN"))
    {
        if (NULL == onenet_dev)
        {
            cm_lwm2m_cfg_t cfg = {0};
            cfg.platform = CM_LWM2M_ONENET;
            cfg.endpoint.pattern = 2;
            cfg.endpoint.name = NULL;
            cfg.host = "183.230.40.39";
            cfg.flag = 3;       //bootstrap服务器，OneNET不支持moniter功能
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
            cfg.cb_param = "OneNET";

            ret = cm_lwm2m_create(&cfg, &onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_create() fail, ret is %d\n", ret);
                return;
            }

            uint8_t instances_3303[1] = {1};
            ret = cm_lwm2m_add_obj(onenet_dev, 3303, instances_3303, 1, 0, 0);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_add_obj() obj 3303 fail, ret is %d\n", ret);
                return;
            }

            uint8_t instances_3306[1] = {1};
            ret = cm_lwm2m_add_obj(onenet_dev, 3306, instances_3306, 1, 0, 0);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_add_obj() obj 3306 fail, ret is %d\n", ret);
                return;
            }

            int32_t resoures_3303[2] = {5700, 5601};
            ret = cm_lwm2m_discover(onenet_dev, 3303, resoures_3303, 2);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_discover() obj 3303 fail, ret is %d\n", ret);
                return;
            }

            int32_t resoures_3306[2] = {5852, 5853};
            ret = cm_lwm2m_discover(onenet_dev, 3306, resoures_3306, 2);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_discover() obj 3306 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_open(onenet_dev, 30, 3000);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_open() fail, ret is %d\n", ret);
                return;
            }

            cm_demo_printf("[OneNET] onenet_dev[%d] CHECKIN SUCC\n", onenet_dev);
        }
    }
    else if (0 == strcmp((const char *)operation, "NOTIFY"))
    {
        if (NULL != onenet_dev)
        {
            /* 服务器支持的编码格式（即content_type入参）支持情况请与平台确认，本用例开发时OneNET平台支持的格式为TLV */
            ret = cm_lwm2m_notify_packing(onenet_dev, 3303, 0, 5700, 4, "20.8", strlen("20.8"), 4);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_notify(onenet_dev, 1);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify() fail, ret is %d\n", ret);
                return;
            }

            /* 服务器支持的编码格式（即content_type入参）支持情况请与平台确认，本用例开发时OneNET平台支持的格式为TLV */
            ret = cm_lwm2m_notify_packing(onenet_dev, 3306, 0, 5853, 1, "test", strlen("test"), 4);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_notify(onenet_dev, 2);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify() fail, ret is %d\n", ret);
                return;
            }
        }
    }
    else if (0 == strcmp((const char *)operation, "NOTIFY_TLV"))
    {
        if (NULL != onenet_dev)
        {
            /* 服务器支持的编码格式（即content_type入参）支持情况请与平台确认，本用例开发时OneNET平台支持的格式为TLV */
            ret = cm_lwm2m_notify_packing(onenet_dev, 3303, 0, 5700, 4, "20.8", strlen("20.8"), 4);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }

            /* 服务器支持的编码格式（即content_type入参）支持情况请与平台确认，本用例开发时OneNET平台支持的格式为TLV */
            ret = cm_lwm2m_notify_packing(onenet_dev, 3303, 0, 5601, 4, "20.7", strlen("20.7"), 4);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_notify(onenet_dev, 1);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_notify() fail, ret is %d\n", ret);
                return;
            }
        }
    }
    else if (0 == strcmp((const char *)operation, "CHECKOUT"))
    {
        if (NULL != onenet_dev)
        {
            ret = cm_lwm2m_del_obj(onenet_dev, 3303);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_del_obj() 3303 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_del_obj(onenet_dev, 3306);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_del_obj() 3306 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_close(onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_close() 3306 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_delete(onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[OneNET] cm_lwm2m_delete() 3306 fail, ret is %d\n", ret);
                return;
            }

            onenet_dev = NULL;
        }
    }
    else
    {
        cm_demo_printf("[OneNET] Illegal operation\n"); 
    }
}

/**
 *  UART口CTWing功能功能调试使用示例
 */
void cm_test_ctwing(unsigned char **cmd,int len)
{
    unsigned char operation[20] = {0};
    sprintf((char *)operation, "%s", cmd[2]);
    int32_t ret = -1;

    if (0 == strcmp((const char *)operation, "CHECKIN"))
    {
        if (NULL == onenet_dev)
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

            ret = cm_lwm2m_create(&cfg, &onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_create() fail, ret is %d\n", ret);
                return;
            }

            uint8_t instances_19[1] = {1};
            ret = cm_lwm2m_add_obj(onenet_dev, 19, instances_19, 1, 0, 0);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_add_obj() obj 19 fail, ret is %d\n", ret);
                return;
            }

            int32_t resoures_19[1] = {0};
            ret = cm_lwm2m_discover(onenet_dev, 19, resoures_19, 1);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_discover() obj 19 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_open(onenet_dev, 30, 3000);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_open() fail, ret is %d\n", ret);
                return;
            }

            cm_demo_printf("[CTWing] onenet_dev[%d] CHECKIN SUCC\n", onenet_dev);
        }
    }
    else if (0 == strcmp((const char *)operation, "NOTIFY"))
    {
        if (NULL != onenet_dev)
        {
            ret = cm_lwm2m_notify_packing(onenet_dev, 19, 0, 0, 1, "test", strlen("test"), 1);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_notify_packing() fail, ret is %d\n", ret);
                return;
            }
            static int mid = 1;
            ret = cm_lwm2m_notify(onenet_dev, mid++);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_notify() fail, ret is %d\n", ret);
                return;
            }
        }
    }
    else if (0 == strcmp((const char *)operation, "CHECKOUT"))
    {
        if (NULL != onenet_dev)
        {
            ret = cm_lwm2m_del_obj(onenet_dev, 19);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_del_obj() 19 fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_close(onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_close() fail, ret is %d\n", ret);
                return;
            }

            ret = cm_lwm2m_delete(onenet_dev);

            if (CM_LWM2M_SUCCESS != ret)
            {
                cm_demo_printf("[CTWing] cm_lwm2m_delete() fail, ret is %d\n", ret);
                return;
            }

            onenet_dev = NULL;
        }
    }
    else
    {
        cm_demo_printf("[CTWing] Illegal operation\n"); 
    }
}

