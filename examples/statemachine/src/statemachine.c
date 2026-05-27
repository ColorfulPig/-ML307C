/**
 * @file        statemachine.c
 * @brief       使用状态机的方式连接onenet平台并进行数据收发处理，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangxw
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_modem.h"
#include "cm_lwm2m.h"
#include "statemachine.h"

#define CM_MACHING_MSG_COUNT        20

typedef struct{
    int type;
    int evt;
} cm_machine_msg_t;

static osMessageQueueId_t s_msg_q = NULL;
static osTimerId_t s_timer = NULL;
static cm_lwm2m_handle_t s_onenet_dev = NULL;


static void __onenet_evt_cb(int32_t event, cm_lwm2m_cb_param_t param)
{
    cm_log_printf(0, "[%p]event:%d\n", param.handle, event);
    cm_machine_msg_t msg = {0};

    switch(event)
    {
        case CM_LWM2M_EVENT_REG_SUCCESS: 
        {
            cm_log_printf(0, "lwm2m register ok\n");
            msg.evt = CM_ONENET_EVT_SUCCESS;
            osMessageQueuePut(s_msg_q, &msg, 0, 0);
        }
        break;
        case CM_LWM2M_EVENT_REG_FAILED:
        {
            cm_log_printf(0, "lwm2m register fail\n");
            msg.evt = CM_ONENET_EVT_CLOSE;
            osMessageQueuePut(s_msg_q, &msg, 0, 0);
        }
        break;
        case CM_LWM2M_EVENT_UNREG_DONE:
        {
            cm_log_printf(0, "lwm2m unregister done\n");
        }
        break;
        default://待补充其他处理策略
        break;
    }
}


/** onenet登录，登录动作执行成功后应流转至期望状态，否则需流转到其他错误状态*/
static inline int __onenet_login(void *arg)
{
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    cm_lwm2m_cfg_t cfg = {
        .platform = CM_LWM2M_ONENET,
        .endpoint = {
            .pattern = 2,
            .name = NULL,
        },
        .host = "183.230.40.39",
        .flag = 3,
        .auto_update = 1,
        .cb = {
            .onEvent = __onenet_evt_cb, //ToDo:填充其他回调函数信息
        },
    };
    uint8_t instances_3303[1] = {1};
    int32_t resoures_3303[2] = {5700, 5601};
    int32_t ret = 0;

    ret = cm_lwm2m_create(&cfg, &s_onenet_dev);
    ret = cm_lwm2m_add_obj(s_onenet_dev, 3303, instances_3303, 1, 0, 0);
    ret = cm_lwm2m_discover(s_onenet_dev, 3303, resoures_3303, 2);
    ret = cm_lwm2m_open(s_onenet_dev, 30, 3000);

    if(ret != 0 )
    {
        cm_log_printf(0, "onenet login err:%d", ret);
        return CM_ONENET_STATE_ERR;
    }

    cm_log_printf(0, "onenet login");
	return trans->next_state;
}

static inline int __onenet_close(void *arg)
{
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    cm_lwm2m_del_obj(s_onenet_dev, 3303);
    cm_lwm2m_close(s_onenet_dev);
    cm_lwm2m_delete(s_onenet_dev);
    s_onenet_dev = NULL;
    cm_log_printf(0, "onenet close");
	return trans->next_state;
}

static inline int __onenet_notify(void *arg)
{
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    static int msg_id = 0;
    int ret = 0;
    ret = cm_lwm2m_notify_packing(s_onenet_dev, 3303, 0, 5700, 4, "20.8", strlen("20.8"), 4);
    ret = cm_lwm2m_notify(s_onenet_dev, msg_id++);

    if(ret != 0 )
    {
        cm_log_printf(0, "onenet notify err:%d", ret);
        return CM_ONENET_STATE_ERR;
    }

    cm_log_printf(0, "onenet notify");
	return trans->next_state;
}

static inline int __onenet_update(void *arg)
{
    int ret = 0;
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    ret = cm_lwm2m_update(s_onenet_dev, 3600, NULL);

    if(ret != 0 )
    {
        cm_log_printf(0, "onenet update err:%d", ret);
        return CM_ONENET_STATE_ERR;
    }

    cm_log_printf(0, "onenet update");
	return trans->next_state;
}

static inline int __onenet_read(void *arg)
{
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    /*ToDo:添加用户代码*/
    cm_log_printf(0, "onenet read");
	return trans->next_state;
}

static inline int __onenet_idle(void *arg)
{
    cm_onenet_state_trans_t *trans = (cm_onenet_state_trans_t*)arg;
    cm_log_printf(0, "onenet idle");
	return trans->next_state;
}


/** OneNET状态转移表定义，根据业务详情扩展*/
static const cm_onenet_state_trans_t s_onenet_machine_tbl[] = {
    {CM_ONENET_STATE_CLOSED,    CM_ONENET_EVT_LOGIN,    CM_ONENET_STATE_LOGGING,       __onenet_login},
    {CM_ONENET_STATE_CONNECTED, CM_ONENET_EVT_CLOSE,    CM_ONENET_STATE_CLOSED,        __onenet_close},
    {CM_ONENET_STATE_LOGGING,   CM_ONENET_EVT_SUCCESS,  CM_ONENET_STATE_CONNECTED,     __onenet_idle},
    {CM_ONENET_STATE_LOGGING,   CM_ONENET_EVT_CLOSE,    CM_ONENET_STATE_CLOSED,        __onenet_close},
    {CM_ONENET_STATE_ERR,       CM_ONENET_EVT_CLOSE,    CM_ONENET_STATE_CLOSED,        __onenet_close},
    {CM_ONENET_STATE_IDLE,      CM_ONENET_EVT_CLOSE,    CM_ONENET_STATE_CLOSED,        __onenet_close},
    {CM_ONENET_STATE_IDLE,      CM_ONENET_EVT_NOTIFY,   CM_ONENET_STATE_IDLE,          __onenet_notify},
    {CM_ONENET_STATE_IDLE,      CM_ONENET_EVT_UPDATE,   CM_ONENET_STATE_IDLE,          __onenet_update},
    {CM_ONENET_STATE_IDLE,      CM_ONENET_EVT_READ,     CM_ONENET_STATE_IDLE,          __onenet_read},
};

static cm_onenet_state_machine_t s_onenet_machine = {
    .state = CM_ONENET_STATE_CLOSED,
    .machine_sz = sizeof(s_onenet_machine_tbl) / sizeof(s_onenet_machine_tbl[0]),
    .machine_tbl = s_onenet_machine_tbl,
};


static int __onenet_machine_get_state(void)
{
    return s_onenet_machine.state;
}


/**
 *  @brief 状态机运转
 *  
 *  @param [in] machine 状态机
 *  @param [in] evt 事件
 *
 *  @return 0:流转完成；其他:流转失败
 *  
 *  @details
 */
static inline int __onenet_machine_run(cm_onenet_state_machine_t *machine, cm_onenet_event_e evt)
{
    int i = 0;
    cm_onenet_state_trans_t *trans = NULL;

    for(i = 0; i < machine->machine_sz; i++)
    {
        if(machine->state == machine->machine_tbl[i].cur_state
          && evt == machine->machine_tbl[i].event)  //检索当前状态节点，并匹配迁移路线
        {
            trans = (cm_onenet_state_trans_t*)&machine->machine_tbl[i];
        }
    }

    if(trans == NULL)
    {
        cm_log_printf(0, "state machine step err,cur_state:%d,evt:%d", machine->state, evt);
        return -1;
    }

    int pre_state = machine->state;
    machine->state = trans->action(trans);

    if(machine->state == CM_ONENET_STATE_CONNECTED) //连接之后直接切换为IDLE状态
    {
        machine->state = CM_ONENET_STATE_IDLE;
    }

    cm_log_printf(0, "state machine step %d -> %d", pre_state, machine->state);
	return 0;
}


/** 使用定时器模拟定时上报数据，驱动状态机运转*/
static void __timer_cb(void *arg)
{
    static int count = 0;
    cm_machine_msg_t msg = {0};

    if(__onenet_machine_get_state() == CM_ONENET_STATE_IDLE)
    {
        if(count % 2) //模拟上传数据和更新事件
        {
            msg.evt = CM_ONENET_EVT_NOTIFY;
        }
        else
        {
            msg.evt = CM_ONENET_EVT_UPDATE;
        }
    }
    else if (__onenet_machine_get_state() == CM_ONENET_STATE_ERR)
    {
        msg.evt = CM_ONENET_EVT_CLOSE;
    }    
    else
    {
        msg.evt = CM_ONENET_EVT_LOGIN;
    }

    count++;
    osMessageQueuePut(s_msg_q, &msg, 0, 0);
}


/** app线程入口*/
static void __task_loop(void *arg)
{
    (void)arg;
    cm_machine_msg_t msg = {0};
    s_timer = osTimerNew(__timer_cb, osTimerPeriodic, NULL, NULL);

    while(1)
    {
        if(cm_modem_get_pdp_state(1) == 1)
        {
            break;
        }

        osDelay(200);
    }

    cm_log_printf(0, "network ready");
    osTimerStart(s_timer, 2000);

    while(1)
    {
        osMessageQueueGet(s_msg_q, &msg, 0, osWaitForever);
        cm_log_printf(0, "got machine evt %d", msg.evt);
        __onenet_machine_run(&s_onenet_machine, msg.evt);
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

    s_msg_q = osMessageQueueNew(CM_MACHING_MSG_COUNT, sizeof(cm_machine_msg_t), NULL);
    osThreadNew((osThreadFunc_t)__task_loop, 0, &app_task_attr);
    return 0;
}
