# aliyun APP 简介

## **阿里云功能介绍**

阿里云物联网平台是一个集成了大数据、云计算等技术的综合解决方案，旨在将物理世界与数字世界相结合，为企业提供一站式的物联网服务。
该平台支持百万级别的设备接入，提供实时数据应对、数据存储和分析等服务，助力企业挖掘数据价值，实现数字化转型。
阿里云支持mqtt、AMGP等多种协议接入。

## **APP实现功能**

本APP来源于aliyun sdk，适配模组的RTOS。
本APP基于阿里云开源sdk，实现阿里云平台的接入和数据传输功能，包括以下子功能：

1. 创建mqtt_recv线程，处理mqtt收到的消息
2. 创建mqtt process线程，处理mqtt事务。

## **APP执行流程**

1、open入口处创建mqtt_recv/mqtt_process现成，分别处理mqtt收发事务。
2、mqtt_process线程在驻网成功后初始化mqtt配置并尝试连接服务器，连接成功后轮询处理mqtt消息。
while (g_mqtt_process_thread_running) {
        res = aiot_mqtt_process(args);
        if (res == STATE_USER_INPUT_EXEC_DISABLED) {
            break;
        }
        osDelay(1);
    }
3、mqtt_recv线程轮询接收并处理来自阿里云服务器的消息。
while (g_mqtt_recv_thread_running) {
        res = aiot_mqtt_recv(args);
        if (res < STATE_SUCCESS) {
            if (res == STATE_USER_INPUT_EXEC_DISABLED) {
                break;
            }
            osDelay(1);
        }
    }
3、aiot_mqtt_process自动发送心跳保活，自动重发QoS1的报文等。

## **APP使用说明**

- 支持的模组（子）型号：ML307A-DSLN/ML307A-DCLN/ML307R-DC/ML307C-DC-CN
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0/ML307C OpenCPU SDK 1.0.0版本及其后续版本；
- 是否需要外设支撑：
- 使用注意事项：
1、示例中的product_key、device_name等设备信息，需要用户自己申请并填充。

## **FAQ**

- 无

## **版本更新说明**

### **1.0.1版本**
- 发布时间：2024/12/24 10:26
- 修改记录：
  1. 新增支持的模组（子）型号以及支持的SDK版本
  2. 修复模组接收到服务器下发数据但未上报缺陷

### **1.0.0版本**

- 发布时间：2024/10/23 18:20
- 修改记录：
  1. 初版

---
