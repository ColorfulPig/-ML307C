# 短信  

## **短信功能介绍**

‌短信（Short Message Service，SMS）‌是一种通过手机或其他电信终端发送和接收的文字或数字信息服务。短信功能最早在1992年由英国沃达丰在GSM网络上实现，至今仍然广泛使用。短信的基本功能包括发送和接收文本信息，每次发送的信息长度限制为140字节，大约可以包含160个英文或数字字符，或者70个中文字符‌。
目前，发送短消息常用Text和PDU(Protocol Data Unit，协议数据单元)模式。使用Text模式收发短信代码简单，实现起来十分容易，但最大的缺点是不能收发中文短信；而PDU模式不仅支持中文短信，也能发送英文短信。PDU模式收发短信可以使用3种编码：7-bit、8-bit和UCS2编码。7-bit编码用于发送普通的ASCII字符，8-bit编码通常用于发送数据消息，UCS2编码用于发送Unicode字符。一般的PDU编码由A B C D E F G H I J K L M十三项组成。

## **实现功能**
本APP实现对PDU模式短信和Text模式短信发送和接收

1. 修改SMS_TEST_MODE宏定义可测试PDU模式短信或者Text模式短信；
2. 实现使用虚拟URC监测短信，并使用虚拟AT指令读取短信内容（PDU模式短信内容为PDU编码，需用户自行解析）；
3. 读取短信后，删除短信，防止短信存储满了，造成无法读取短信。
4. 封装设置短信模式、短信上报格式、获取短信中心号码接口、删除短信和读取短信接口。


## **APP执行流程**

1. 进入短信测试线程，首先创建短信线程消息队列，用于通知线程接收短信；
2. 使用虚拟AT命令“AT+CMGF=<mode>”设置短信的模式，用户可修改SMS_TEST_MODE宏定义可测试PDU模式短信或者Text模式短信。

```c
cm_sms_mode_set(SMS_TEST_MODE);
```

```c
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
```

3. 注册虚拟AT关键字回调函数，短信默认上报模式(+CNMI: 2,1,0,0,0)的关键字为“+CMTI:”，即模组接收到短信后，就会触发注册的回调函数__cm_sms_virat_msg_cb，回调函数打印上报的urc内容和长度，内容格式一般是“+CMTI: "ME",1”，"1"是短信编号。从urc内容解析出短信的编码后，发送带有接收到短信的消息ID和短信编号的队列消息到s_sms_task_msg_queue队列中。

```c
cm_virt_at_urc_cb_reg((const uint8_t *)"+CMTI:",(cm_virt_at_urc_cb)__cm_sms_virat_msg_cb);
```

```c
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

```

4. 等待模组pdp激活，因为发送短信也需要模组入网。

```c
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
```

5. 根据宏定义SMS_TEST_MODE的短信模式，发送不同模式的短信。
PDU模式调用cm_sms_send_pdu发送，第一个参数是PDU编码，编码中包含的短信内容是“欢迎使用中移物联网Cat.1模组”，短信中心号码是8613800230500（用户将SIM卡插在模组上后使用AT+CSCA?命令查询），收短信方8618883993627，整个编码使用网上的pdu工具生成；cm_sms_send_pdu第二个参数为pdu内容数据实际长度的10进制字符串；
Text模式（只能发英文）调用cm_sms_send_txt发送数据，第一个参数是内容“zhongyimozu”，直接也是短信的内容，第二个参数为接收方号码。

```c
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
```

6. 短信发送后，一直等待s_sms_task_msg_queue队列中接收消息；

```c
    cm_sms_task_msg_t sms_msg = {0};
    while(1)
    {
        if (osMessageQueueGet(s_sms_task_msg_queue, &sms_msg, NULL, osWaitForever) == osOK)
        {
            
        }
    }
```

7. 接收到步骤3中的队列消息后，判断是否是接收到短信的消息ID，若是，则根据短信编号使用虚拟AT指令：“AT+CMGR=(id)” 读取短信内容，pdu模式短信格式一般为：

```c
    +CMGR: 1,,22
    0881683108200305F0040D91683189737221F400001280319044602302331B //短信内容
```

Text模式短信格式一般为：

```c
    +CMGR: "REC READ","+861398372xxxx",,"21/08/13,10:03:53+32" //来自1398372xxxx。
    test2 //短信内容
```

```c
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
```

```c

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

```

8. 解析和打印出短信内容后，使用虚拟AT指令：“AT+CMGD=(id)” 删除短信。

```c
    cm_sms_del(sms_msg.param1);
```

```c
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
```

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：无
- 使用注意事项：  
    1. ML307R DC不支持IMS短信，即只支持移动卡或者部分联通卡，模组短信建议使用移动卡测试；
    2. 使用模组默认的上报格式（CNMI:2,1,0,0,0），短信触发的关键字为"+CMTI:"，短信只上报短信编号；若CNMI第二个参数设置为2，则短信触发的关键字为"+CMT:"，短信会携带内容一起上报；
    3. cm_virt_at_urc_cb_reg注册的回调函数中禁止使用虚拟AT或者申请大内存，建议发消息到线程中处理；
    4. 短信读取后，建议立即删除，防止短信存储满了；
    5. ML307R不支持SIM1发送短信；
    6. 发送短信的接口底层实际也是调用虚拟AT，共享长度512个字节空间，而短信末尾需包含结束符，因此发送短
信长度请勿超过511字节。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 问题：是否有短信常用AT命令使用方法？
    答复：AT具体指令见《AT_Commands_Reference_Guide_4G_Series》，内容为英文，也可参考《ML307R_通信流程示例》手册中的短信章节；短信指令为3GPP标准指令，用户也可参考网上的解释，如https://blog.csdn.net/weixin_34021089/article/details/86103766

- 问题：是否有PDU编码参考代码？
    答复：无。PDU编码为标准格式，网上有很多PDU解析示例，如https://blog.csdn.net/liht_1634/article/details/124179486，https://blog.csdn.net/u013087068/article/details/45147019。

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/5 15:05
- 修改记录：
  1. 初版


--------------------------------------------------------------------------------