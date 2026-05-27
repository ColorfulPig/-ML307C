# ONEOS 基站定位

## **oneos 基站定位功能介绍**

ONEOS​定位服务可以对使用操作系统的伙伴提供室内外定位功能。该定位功能依据多数智能硬件自身情况而设计，无需增加额外产品成本，就能在当前硬件支持的情况下给出精度最高的定位结果；这里主要介绍只支持网络定位，网路定位是使用设备自身所处的网络环境的特征进行匹配定位的一种定位方式，目前支持的定位方式有：基站定位，即通过模组上报所在网络的信号情况到oneos定位平台，oneos定位平台计算出经纬度返回给模组。

## **实现功能**
本APP将oneos lbs异步接口封装为同步接口

1. 完成oneos lbs初始化和启动请求后，阻塞等待lbs异步回调的返回，将返回的经纬度通过同步接口的参数传出。

## **APP执行流程**

1. 进入oneos lbs测试线程，等待模组pdp激活；

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

2. 每20s调用一次cm_oneos_lbs_demo_func，函数cm_oneos_lbs_demo_func中调用封装的同步接口cm_oneos_lbs_get_sync，并打印结果；

```c
    char longitude[15] = {0},latitude[15] = {0};
    int ret = cm_oneos_lbs_get_sync(longitude,latitude,10);
    if(ret == 0)
    {
        cm_log_printf(0,"get oneos lbs data success,longitude:%s,latitude:%s",longitude,latitude);
    }
    else
    {
        cm_log_printf(0,"get oneos lbs data fail");
    }
```
3. 设置域名解析ipv4优先（底层默认采用ipv6优先，若要测试ipv6，请检查SIM卡是否支持ipv6）；

```c
    cm_async_dns_set_priority(0);
```

4. oneos lbs同步接口cm_oneos_lbs_get_sync：首先cm_lbs_init初始化使用oneos定位，再使用cm_lbs_location注册oneos定位结果回调函数并启动oneos lbs请求；然后阻塞超时等待lbs异步回调函数置位LBS定位标志变量s_oneos_lbs_flag，将定位数据拷贝到参数中传出。

```c
    while(1)
    {
        if(s_oneos_lbs_flag == CM_LBS_LOCATION_OK)  //oneos lbs定位成功
        {
            strcpy(longitude,s_oneos_lbs_longitude);
            strcpy(latitude,s_oneos_lbs_latitude);
            cm_log_printf(0,"[4/4]oneos lbs location ok!");
            break;
        }
        else if(s_oneos_lbs_flag < 0)
        {
            return s_oneos_lbs_flag;
        }
        wait_timeout ++;
        if(wait_timeout * 100 >= (timeout_s + 3) * 1000) //超时后未产生回调
        {
            cm_lbs_deinit();
            return CM_LBS_TIMEOUT;
        }
        osDelay(20);
    }
```

5. lbs异步回调函数__cm_oneos_lbs_cb判断是否定位成功，若定位成功，则将定位的经纬度结果拷贝到经纬度静态全局变量中并去初始化lbs；然后无论是否定位成功，均将定位结果赋值给LBS定位标志变量s_oneos_lbs_flag。

```c
    if(event == CM_LBS_LOCATION_OK)
    {
        strcpy(s_oneos_lbs_longitude,location->longitude);
        strcpy(s_oneos_lbs_latitude,location->latitude);
    }
    cm_lbs_deinit();
    s_oneos_lbs_flag =  event;
```

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：无
- 使用注意事项：  
    1. oneos lbs定位时，pid参数可以为空""，底层将模组内置的PID，与其他模组用户共用每天400W次调用次数，QPS 30；
    2. 因oneos lbs定位接口每天调用次数有限，且每秒查询率QPS也有限制，建议不要频繁调用;
    3. 底层访问oneos lbs平台默认使用ipv6优先解析域名，若要测试iPv6，建议采用支持ipv6的卡测试。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 无

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/4 11:30
- 修改记录：
  1. 初版


--------------------------------------------------------------------------------