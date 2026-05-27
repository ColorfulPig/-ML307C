/**
 * @file        statemachine.h
 * @brief       使用状态机的方式连接onenet平台并进行数据收发处理，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangxw
 */


#ifndef __STATEMACHINE_H__
#define __STATEMACHINE_H__

#include <stdlib.h>
#include <stdio.h>


typedef enum{
    CM_ONENET_STATE_CLOSED = 0,     /*!< 已断开*/
    CM_ONENET_STATE_LOGGING,        /*!< 登录中*/
    CM_ONENET_STATE_CONNECTED,      /*!< 已连接*/
    CM_ONENET_STATE_IDLE,           /*!< 空闲*/
    CM_ONENET_STATE_ERR,            /*!< 错误*/
} cm_onenet_state_e;

typedef enum
{
    CM_ONENET_EVT_LOGIN = 0,    /*!< 登录*/
    CM_ONENET_EVT_SUCCESS,      /*!< 成功(登录成功，发送成功等)*/
    CM_ONENET_EVT_NOTIFY,       /*!< 上传数据*/
    CM_ONENET_EVT_UPDATE,       /*!< 更新lifetime*/
    CM_ONENET_EVT_READ,         /*!< 处理平台的read请求*/
    CM_ONENET_EVT_CLOSE,        /*!< 注销*/
} cm_onenet_event_e;

typedef struct
{
    cm_onenet_state_e cur_state;    /*!< 当前状态*/
    cm_onenet_event_e event;        /*!< 触发事件*/
    cm_onenet_state_e next_state;   /*!< 期望状态*/
    int (*action)(void *arg);       /*!< 执行实体*/
} cm_onenet_state_trans_t;

typedef struct
{
    cm_onenet_state_e state;            /*!< 状态机当前状态*/
    int machine_sz;
    const cm_onenet_state_trans_t *machine_tbl; /*!< 状态迁移表*/
} cm_onenet_state_machine_t;


#endif