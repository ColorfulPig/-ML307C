# 模组网络状态通过回调函数方式通知  

## **网络状态通知功能介绍**

实现将模组的网络状态异步通知，通知的状态有：未识别SIM卡（CM_NETWORK_EVENT_NO_SIM）、识别到SIM卡（CM_NETWORK_EVENT_SIM_READY）、网络激活成功（CM_NETWORK_EVENT_PDP_ACTIVED）、网络失活（CM_NETWORK_EVENT_PDP_DEACTIVED）、网络激活失败（CM_NETWORK_EVENT_PDP_ACTIVE_FAIL）

## **实现功能**
本APP创建单独的网络维护线程，使用modem接口实现网络状态变化通知

1. 注册网络变化状态的回调函数
2. 创建网络监测线程：查询协议栈状态、查询SIM卡状态、查询基站注册状态、查询网络激活状态，查询后的结果通过回调函数通知
3. 查询SIM卡状态、查询基站注册状态、查询网络激活状态和状态周期检测的间隔时间用户可自定义，修改宏定义即可；
4. 只有状态变化才会触发回调函数；
5. 网络激活失败后建议重启，防止因为卡欠费、机卡绑定等原因导致模组一直无法入网。

## **APP执行流程**

1. 进入网络测试线程，调用网络状态通知注册接口cm_network_init，设置app_network_event_cb为网络状态通知函数，然后网络测试线程周期进入延时阻塞；

```c
    cm_network_init(app_network_event_cb);
    while(1)
    {
        osDelay(2000);
    }
```

2.  cm_network_init函数中，将静态全局网络状态回调函数赋值为参数传入的网络状态通知函数，并创建网络监测线程；

```c
    cm_network_event_callback = network_event_cb;

    /* 创建网络维护线程 */
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "maintain_task";
    app_task_attr.stack_size = 1024 * 4;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_network_maintain_task, 0, &app_task_attr);
```

3. 进入网络监测线程:
(1) 查询协议栈状态，如果未打开，则打开；

```c
    if(cm_modem_get_cfun() != 1)
    {
        cm_modem_set_cfun(1);
    }
```

(2) 查询SIM卡状态，若在设置时间未识别到sim卡，则调用回调函数通知“未识别到SIM卡”；若识别到SIM卡，则调用回调函数通知“识别到SIM卡”；

```c
    if (cm_wait_sim_ready(CM_NETWORK_WAIT_SIM_READY))
	{
        sim_previous_status = 1;
        cm_network_event_callback(CM_NETWORK_EVENT_SIM_READY);
        cm_log_printf(0, "sim is ready");
    }
    else
    {
        sim_previous_status = 0;
        cm_network_event_callback(CM_NETWORK_EVENT_NO_SIM);
    }
```
(3) sim卡为设置apn，通用卡可跳过该步骤；专网卡请使用虚拟AT发送以下指令设置APN: AT+CGDCONT=1,"IPV4V6","SIM卡APN名称" */


(4) 查询和等待网络注册状态，若在设置时间内注册上网络，即判断cereg的状态是1或者5，则表示模组注册上网络；注册上网络或者超过等待的时间均进入下一步；

```c
    if(cm_wait_network_register(CM_NETWORK_WAIT_REGISTER_TIMEOUT))
    {
        cm_log_printf(0,"The network has been registered successfully");
    }
```

(5) 查询和等待网络激活状态，若在设置时间内网络激活，则调用回调函数通知“网络激活成功”，若未激活则调用回调函数通知“网络激活失败”；

```c
    if(cm_wait_network_active(CM_NETWORK_WAIT_ACTIVE_TIMEOUT))
    {
        network_previous_status = 1;    //网络激活
        osDelay(200);
        cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVED);
        cm_log_printf(0,"The network has been actived successfully");
    }
    else
    {
        network_previous_status = 0;    //网络激活失败
        cm_network_event_callback(CM_NETWORK_EVENT_PDP_ACTIVE_FAIL);
        cm_log_printf(0,"The network actived fail");
    }
```

(6) 进入周期监测轮询，若前面步骤网络已经激活，则周期检测，若检测到掉网（网络未注册或者网络未激活），则调用回调函数通知“网络失活”，间隔一个周期后，重复上面的步骤1-5；若前面步骤网络未激活，则再次重复上面的步骤1-5；

(7) 注册的网络状态通知函数app_network_event_cb中，监测和打印网络状态，当出现网络激活失败的时候则重启。

```c
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
```

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：无
- 使用注意事项：  
    1. cm_network_init注册的通知回调函数不能为NULL；
    2. 该功能需要借助modem里面的接口；
    3. cm_modem_get_pdp_state查询到pdp激活后，建议适当延时后再进行网络业务（demo中延时1s后再触发的回调函数）；
    4. 功能模块中等待的超时时间用户可自行定义。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 无

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/10/30 17:40
- 修改记录：
  1. 初版


--------------------------------------------------------------------------------