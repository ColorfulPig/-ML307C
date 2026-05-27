/**
 * @file        sms.c
 * @brief       短信收发，示例仅供参考
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
#include "cm_sms.h"
#include "cm_virt_at.h"

#define SMS_TEST_MODE 1 // 0:PDU模式  1:TEXT模式  (模组开机不使用命令设置短信模式则默认是PDU模式，此宏定义为使用AT+CMGF命令设置将要测试的短信模式)

#define SMS_TASK_MSG_RECV  1     //短信线程队列消息ID

typedef struct 
{
    int msgid;
    int param1;
}cm_sms_task_msg_t;

static osMessageQueueId_t s_sms_task_msg_queue = NULL;


/**
 * @brief  设置短信息模式
 *
 * @param [in] mode 0:pdu 1:text
 *
 * @return  成功/失败
 *
 * @details More details
 */
bool cm_sms_mode_set(int mode)
{
    uint8_t exe_cmd[32] = {0};
    sprintf((char *)exe_cmd, "AT+CMGF=%d\r\n", mode);
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)exe_cmd, rsp, &rsp_len, 32) == 0)
    {
		if(0 == strncmp((char *)rsp, "OK", 2))
        {
			cm_log_printf(0,"set sms mode(%d) success(0:pdu 1:text)", mode);
            return true;
        }
    }
    cm_log_printf(0,"set sms mode(%d) fail(0:pdu 1:text)", mode);
    return false;
}

/**
 * @brief  设置短信通知模式
 *
 * @param  cnmi_mode  0 接受的短消息存储到默认的内存位置（包括class 3）,不通知TE。
    1 接收的短消息储存到默认的内存位置,并且向TE发出通知（包括class 3）。通知的形式为：
     +CMTI: "SM",<index>
    2 对于class 2短消息,储存到SIM卡,并且向TE发出通知；对于其他class,直接将短消息转发到TE：
     +CMT: [<alpha>],<length><CR><LF><pdu>（PDU模式）
     或者＋CMT: <oa>,[<alpha>,]<scts>[,<tooa>,<fo>,<pid>,<dcs>,<sca>,<tosca>,<length>]<CR><LF><data>（text模式）
    3 对于class 3短消息,直接转发到TE,同<mt>＝2；对于其他class,同<mt>＝1。
 *
 * @return  成功/失败
 *
 * @details More details  AT+CNMI=2,1,0,0,0为默认模式，可不设置;注意cnmi_mode设置为2后，urc的关键字将改变，注意修改代码
 */
bool cm_sms_cnmi_set(uint8_t cnmi_mode)
{
    uint8_t exe_cmd[32] = {0};
    sprintf((char *)exe_cmd, "AT+CNMI=2,%d,0,0,0\r\n", cnmi_mode);
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)exe_cmd, rsp, &rsp_len, 32) == 0)
    {
		if(0 == strncmp((char *)rsp, "OK", 2))
        {
			cm_log_printf(0,"set sms cnmi success");
            return true;
        }
    }
    cm_log_printf(0,"set sms cnmi fail");
    return false;
}

/**
 * @brief  获取短信息中心号码
 *
 * @param [out] center_number  短信中心电话号码
 *
 * @return  成功/失败
 *
 * @details More details
 */
bool cm_sms_csca_get(char *center_number)
{
    uint8_t rsp[64] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CSCA?\r\n", rsp, &rsp_len, 32) == 0)
    {
		if(0 == strncmp((char *)rsp, "+CSCA:", 6))
        {
            if (sscanf((const char *)rsp, "+CSCA:%*[^+]+%[^\"]\"", center_number) == 1)
            {
                cm_log_printf(0,"get sms center number \"%s\" success", center_number);
                return true;
            }
        }
    }
    cm_log_printf(0,"get sms center number fail");
    return false;
}

/**
 * @brief  删除短信息
 *
 * @param [in] sms_id  短信编号
 *
 * @return  成功/失败
 *
 * @details More details
 */
bool cm_sms_del(int sms_id)
{
    uint8_t exe_cmd[32] = {0};
    sprintf((char *)exe_cmd, "AT+CMGD=%d\r\n", sms_id);
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)exe_cmd, rsp, &rsp_len, 32) == 0)
    {
		if(0 == strncmp((char *)rsp, "OK", 2))
        {
			cm_log_printf(0,"del sms number(%d) success", sms_id);
            return true;
        }
    }
    cm_log_printf(0,"del sms number(%d) fail", sms_id);
    return false;
}

/**
 * @brief  接收短信息
 *
 * @param [in] sms_id  短信编号
 *
 * @return  成功/失败
 *
 * @details More details
 */
bool cm_sms_recv(uint8_t *content,int *contentlen,int sms_id,int sms_mode)
{
    uint8_t at_cmd[20] = {0};
    unsigned char at_rsp_data[512] = {0};
	int32_t at_rsp_data_len = 0;

    sprintf((char *)at_cmd,"AT+CMGR=%d\r\n",sms_id);

    if (cm_virt_at_send_sync((const uint8_t *)at_cmd, at_rsp_data, &at_rsp_data_len, 2) < 0)
    {
        return false;
    }

    char *content_offset = strstr((const char *)at_rsp_data, "+CMGR:");
    if(content_offset != NULL)
    {
        if(sms_mode == 0)   //pdu格式
        {
            //AT+CMGR=35 //读取编号为35的短信
            // +CMGR: 1,,22
            // 0881683108200305F0040D91683189737221F400001280319044602302331B //短信内容
            content_offset = strstr((const char *)at_rsp_data, "\r\n");
            if(content_offset != NULL)
            {
                *contentlen = strlen(content_offset) - 2;
                if(*contentlen > 0)
                {
                    memcpy(content,content_offset + 2,*contentlen);
                    return true;
                }
            }
        }
        else
        {
            // AT+CMGR=40 //读取编号为40的短信
            // +CMGR: "REC READ","+861398372xxxx",,"21/08/13,10:03:53+32" //编号40短信，来自1398372xxxx。
            // test2 //短信内容
            char send_number[20] = {0};
            if(sscanf((const char *)at_rsp_data, "+CMGR:%*[^,],\"%[^\"]\"", send_number) == 1)
            {
                cm_log_printf(0,"sms send from %s",send_number);
            }
            content_offset = strstr((const char*)at_rsp_data, "\r\n");
            if(content_offset != NULL)
            {
                *contentlen = strlen(content_offset) - 2;
                if(*contentlen > 0)
                {
                    memcpy(content,content_offset + 2,*contentlen);
                    return true;
                }
            }
        }
    }
    return false;
}

/**
 * @brief  短信urc监测回调
 *
 * @param [out] urc URC内容
 * @param [out] urc_len URC内容长度
 *
 * @return  成功/失败
 *
 * @details More details
 */
void __cm_sms_virat_msg_cb(uint8_t *urc, int32_t urc_len)
{
    //+CMTI: "ME",1
    int sms_id = 0;
    cm_log_printf(0,"__cm_sms_virat_msg_cb urc:%s urc_len:%d\n", urc, urc_len);
    if(sscanf((const char*)urc,"%*[^,],%d",&sms_id) == 1)
    {
        cm_sms_task_msg_t sms_msg = {0};
        sms_msg.msgid = SMS_TASK_MSG_RECV;
        sms_msg.param1 = sms_id;
        osMessageQueuePut(s_sms_task_msg_queue, &sms_msg, 0, 0); 
    }
}


/* 短信测试线程 */
void cm_sms_task_test(void *p)
{
    s_sms_task_msg_queue = osMessageQueueNew(10, sizeof(cm_sms_task_msg_t), NULL); //创建短信消息队列

    cm_sms_mode_set(SMS_TEST_MODE);
    cm_virt_at_urc_cb_reg((const uint8_t *)"+CMTI:",(cm_virt_at_urc_cb)__cm_sms_virat_msg_cb);
    
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

    if(SMS_TEST_MODE == 0)
    {
        //PDU格式数据可采用网上的pdu工具生成，由于SIM卡不同，短信中心号码可能不同，建议使用AT+CSCA?指令查询短信的中心号码，本示例中心号码：+8613800230500
        //PDU模式向18883993627发送“欢迎使用中移物联网Cat.1模组”
        cm_sms_send_pdu("0891683108200305F011000D91688188933926F70008B0206B228FCE4F7F75284E2D79FB726980547F51004300610074002E00316A217EC4", "47", CM_MSG_MODE_GSM_7, CM_SIM_ID_0);//SIM0卡发送PDU格式GSM7编码短信
    }
    else
    {
        //text模式向18883993627发送“zhongyimozu”
        cm_sms_send_txt("zhongyimozu", "18883993627", CM_MSG_MODE_GSM_7, CM_SIM_ID_0);//SIM0卡发送txt格式GSM7编码短信
    }

    cm_sms_task_msg_t sms_msg = {0};
    while(1)
    {
        if (osMessageQueueGet(s_sms_task_msg_queue, &sms_msg, NULL, osWaitForever) == osOK)
        {
            switch (sms_msg.msgid)
            {
                case SMS_TASK_MSG_RECV:
                {
                    uint8_t sms_content[512] = {0};
                    int sms_contentlen = 0;
                    if(cm_sms_recv(sms_content,&sms_contentlen,sms_msg.param1,SMS_TEST_MODE))
                    {
                        cm_log_printf(0,"recv sms data:%d,%s",sms_contentlen,sms_content);
                    }
                    cm_sms_del(sms_msg.param1);
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

    osThreadNew((osThreadFunc_t)cm_sms_task_test, 0, &app_task_attr);
    return 0;
}