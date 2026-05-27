/**
 * @file        call.c
 * @brief       语音通话，示例仅供参考
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
#include "cm_virt_at.h"


#define CALL_TEST_NUMBER "" //请填写测试号码

enum
{
    CALL_RING,
    CALL_NUMBER,
    CALL_CONNECT,
    CALL_DISCONNECT,
    CALL_HANGUP,
};

typedef struct 
{
    int msgid;
    char param[64];
}cm_call_task_msg_t;

static osMessageQueueId_t s_call_task_msg_queue = NULL;     //语音通话测试线程消息队列
static osMessageQueueId_t s_call_vir_at_queue = NULL;       //虚拟AT回调函数结果通知消息队列
static osTimerId_t s_call_hangup_timer = NULL;              //挂断电话的定时器
/**
 * @brief  虚拟AT回调
 *
 * @param [in] param  结果
 *
 * @return 
 *
 * @details More details
 */
static void __cm_vir_at_cb(void * param)
{
    if(s_call_vir_at_queue != NULL)
    {
        int mqtt_param = atoi((char*)param);
        osMessageQueuePut(s_call_vir_at_queue, &mqtt_param, 0, 1000/5);  
    }
}

/**
 * @brief  打电话
 *
 * @param [in] phone_number  电话号码
 *
 * @return  0:成功  <0:失败
 *
 * @details More details
 */
int cm_call_callup(char *phone_number)
{
    unsigned char at_data[64] = {0};
	int cm_ret = -1;
	int modem_flag = -1; 

    uint8_t exe_cmd[32] = {0};
    snprintf((char *)exe_cmd, sizeof(exe_cmd),"ATD%s;\r\n", phone_number);

    if(0 != cm_virt_at_init(__cm_vir_at_cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
	s_call_vir_at_queue = osMessageQueueNew(1, sizeof(int), NULL);
	if(cm_virt_at_send((uint8_t *)exe_cmd, strlen((const char*)exe_cmd)) < 0)
    {
        goto _exit;
    }
	if(0 == osMessageQueueGet(s_call_vir_at_queue, &modem_flag, 0, 10000 / 5))
	{
		cm_virt_at_get(at_data, sizeof(at_data));
		if(0 == modem_flag)
        {
			if (0 == strncmp((char *)at_data, "OK", 2))
            {
				cm_log_printf(0,"call number(%s) success", phone_number);
                cm_ret = 0;
                goto _exit;
            }
		}
	}
    cm_log_printf(0,"call number(%s) fail", phone_number);
_exit:
	osMessageQueueDelete(s_call_vir_at_queue);
    s_call_vir_at_queue = NULL;
    cm_virt_at_deinit(); 
	return cm_ret;
}

/**
 * @brief  接听电话
 *
 * @param 
 *
 * @return  0:成功  <0:失败
 *
 * @details More details
 */
int cm_call_answer()
{
    unsigned char at_data[64] = {0};
	int cm_ret = -1;
	int modem_flag = -1; 

    uint8_t exe_cmd[32] = {0};
    snprintf((char *)exe_cmd, sizeof(exe_cmd),"ATA\r\n");

    if(0 != cm_virt_at_init(__cm_vir_at_cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
	s_call_vir_at_queue = osMessageQueueNew(1, sizeof(int), NULL);
	if(cm_virt_at_send((uint8_t *)exe_cmd, strlen((const char*)exe_cmd)) < 0)
    {
        goto _exit;
    }
	if(0 == osMessageQueueGet(s_call_vir_at_queue, &modem_flag, 0, 5000 / 5))
	{
		cm_virt_at_get(at_data, sizeof(at_data));
		if(0 == modem_flag)
        {
			if (strstr((char *)at_data, "OK") != NULL || strstr((char *)at_data, "+CLCC") != NULL
            || strstr((char *)at_data, "CONNECT") != NULL)
            {
				cm_log_printf(0,"answer the phone success");
                cm_ret = 0;
                goto _exit;
            }
		}
	}
    cm_log_printf(0,"answer the phone fail");
_exit:
	osMessageQueueDelete(s_call_vir_at_queue);
    s_call_vir_at_queue = NULL;
    cm_virt_at_deinit(); 
	return cm_ret;
}


/**
 * @brief  挂电话
 *
 * @param 
 *
 * @return  0:成功  <0:失败
 *
 * @details More details
 */
int cm_call_hangup()
{
    unsigned char at_data[64] = {0};
	int cm_ret = -1;
	int modem_flag = -1; 

    uint8_t exe_cmd[32] = {0};
    snprintf((char *)exe_cmd, sizeof(exe_cmd),"ATH\r\n");

    if(0 != cm_virt_at_init(__cm_vir_at_cb))
    {
        cm_virt_at_deinit();
        return -1;
    }
	s_call_vir_at_queue = osMessageQueueNew(1, sizeof(int), NULL);
	if(cm_virt_at_send((uint8_t *)exe_cmd, strlen((const char*)exe_cmd)) < 0)
    {
        goto _exit;
    }
	if(0 == osMessageQueueGet(s_call_vir_at_queue, &modem_flag, 0, 5000 / 5))
	{
		cm_virt_at_get(at_data, sizeof(at_data));
		if(0 == modem_flag)
        {
			if (strstr((char *)at_data, "NO CARRIER") != NULL || strstr((char *)at_data, "OK") != NULL)
            {
				cm_log_printf(0,"hang up the phone success");
                cm_ret = 0;
                goto _exit;
            }
		}
	}
    cm_log_printf(0,"hang up the phone fail");
_exit:
	osMessageQueueDelete(s_call_vir_at_queue);
    s_call_vir_at_queue = NULL;
    cm_virt_at_deinit(); 
	return cm_ret;
}


/**
 * @brief  发送消息到语音线程消息队列中
 *
 * @param msg 消息id
 * @param number 电话号码
 *
 * @return 
 *
 * @details More details
 */
void cm_send_call_msg(int msg,char *number)
{
    cm_call_task_msg_t call_msg = {0};
    call_msg.msgid = msg;
    strcpy(call_msg.param,number);
    osMessageQueuePut(s_call_task_msg_queue,&call_msg,0,0);
}

/**
 * @brief  urc上报回调函数（禁止在回调函数内容调用虚拟AT接口函数）
 *
 * @param urc urc上报的内容
 *
 * @return 
 *
 * @details More details
 */
void cm_call_urc_callback(char *urc)
{
    //cm_log_printf(0,"[***URC***]:%s",urc);
    if (strstr(urc, "RING") != NULL)    //语音来电
    {
        cm_send_call_msg(CALL_RING,NULL);
    }
    else if (strstr(urc, "+CLCC:") != NULL) //电话号码显示，拨打电话、语音来电都会上报该URC，可通过内容获取对方的通话号码
    {
        char call_number[64] = {0};
        char *offset = NULL;
        offset = strstr(urc, "+CLCC:");
        memset(call_number,0,sizeof(call_number));
        if (sscanf(offset, "+CLCC:%*[^,],%*[^,],%*[^,],%*[^,],%*[^,],\"%[^\"]\"", call_number) == 1)
        {
            cm_send_call_msg(CALL_NUMBER,call_number);
        }
    }
    else if (strstr(urc, "NO CARRIER") != NULL) //语音通话结束：如对方挂断将上报该URC
    {
        cm_send_call_msg(CALL_DISCONNECT,NULL);
    }
    else if(strstr(urc, "CONNECT") != NULL) //语音通话连接建立成功：对方接听或者模组主动接听都会上报该URC
    {
        cm_send_call_msg(CALL_CONNECT,NULL);
    }
}

/**
 * @brief  挂断电话的定时器回调
 *
 * @param p 定时器参数
 *
 * @return 
 *
 * @details More details
 */
void __cm_call_hangup_timer_cb(void *p)
{
    cm_send_call_msg(CALL_HANGUP,NULL);
}

/* 语音通话测试线程 */
void cm_call_task_test(void *p)
{
    s_call_hangup_timer = osTimerNew(__cm_call_hangup_timer_cb,osTimerOnce, (void*)0 ,0);
    s_call_task_msg_queue = osMessageQueueNew(10, sizeof(cm_call_task_msg_t), NULL); //创建短信消息队列
    
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

    /* 1. 注册urc回调，通过urc上报的内容判断语音通话的状态 */
    cm_virt_at_urc_reg(cm_call_urc_callback);

    /* 2. 使用虚拟AT指令ATD打电话，注意号码后面要加分号，如"ATD10086;" */
    cm_call_callup(CALL_TEST_NUMBER);
    cm_log_printf(0,"[CALL]call number:%s",CALL_TEST_NUMBER);

    /* 3. 启动1分钟定时器就挂断电话：对方可能接听，也可能不接听或者直接挂断 */
    osTimerStart(s_call_hangup_timer,60 * 1000 / 5);

    /* 4. 等待语音通话状态消息 */
    cm_call_task_msg_t call_msg = {0};
    while(1)
    {
        if (osMessageQueueGet(s_call_task_msg_queue, &call_msg, NULL, osWaitForever) == osOK)
        {
            switch (call_msg.msgid)
            {
                /* 语音来电：自动接听 */
                case CALL_RING:
                {
                    cm_log_printf(0,"[CALL MSG]CALL_RING,will answer the phone");
                    cm_call_answer();
                    break;
                }   
                /* 通话号码提示 */
                case CALL_NUMBER:
                {
                    cm_log_printf(0,"[CALL MSG]CALL_NUMBER:%s",call_msg.param);
                    break;
                }  
                /* 语音通话建立成功 */
                case CALL_CONNECT:
                {
                    cm_log_printf(0,"[CALL MSG]CALL_CONNECT");
                    break;
                }  
                /* 语音通话断开 */
                case CALL_DISCONNECT:
                {
                    cm_log_printf(0,"[CALL MSG]CALL_DISCONNECT");
                    break;
                }  
                /* 挂断语音通话 */
                case CALL_HANGUP:
                {
                    cm_log_printf(0,"[CALL MSG]CALL_HANGUP,will hang up the phone");
                    cm_call_hangup();
                    break;
                }
                default:
                    break;
            }
        }
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

    osThreadNew((osThreadFunc_t)cm_call_task_test, 0, &app_task_attr);
    return 0;
}