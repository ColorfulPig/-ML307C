# MQTT接口测试 简介 

## MQTT介绍**
MQTT（Message Queuing Telemetry Transport，消息队列遥测传输协议）是一种基于发布/订阅模式的“轻量级”通讯协议。
发布/订阅模式：MQTT协议采用发布/订阅的消息传输模式。在这种模式中，消息发布者（Publisher）将消息发布到一个特定的主题（Topic）上，而消息订阅者（Subscriber）则订阅该主题以接收消息。当有新的消息发布到该主题时，MQTT代理（Broker，即服务器）会将消息发送给所有订阅了该主题的订阅者。
轻量级：MQTT协议被设计为轻量级，这意味着它使用较少的代码和带宽，同时提供实时可靠的消息服务。这使得MQTT协议非常适合在物联网（IoT）、小型设备、移动应用等受限环境中使用。
基于TCP/IP：MQTT协议构建于TCP/IP协议之上，因此在使用MQTT协议时需要建立TCP连接。这使得MQTT协议能够利用TCP/IP协议的可靠性和稳定性来确保消息的可靠传输。
MQTT协议是一种基于发布/订阅模式的轻量级通讯协议，具有开销小、简单性、可扩展性和可靠性等特点。它被广泛应用于各种物联网场景，为连接远程设备提供实时可靠的消息服务。

## **实现功能**
本示例使用MQTT完成连接、订阅topic、推送数据、断开操作

## **APP执行流程**
1. 进入mqtt测试线程，创建mqtt状态事件组，然后等待模组pdp激活
2. 连接mqtt，初始化MQTT必要的各类回调，根据宏定义配置的mqtt连接地址、端口号、CLIENTID/USER/PASSWORD等参数进行连接
3. 等待__mqtt_manager_default_connack_cb上报，当上报conn_res为CM_MQTT_CONN_STATE_SUCCESS时表示MQTT已连接成功
4. 订阅topic，topic名称由宏定义设置，用户可进行更改，等待__mqtt_manager_default_suback_cb上报
4. MQTT连接成功后，会按照10s定时PUBLISH一包数据至服务器；连接失败后，会关闭MQTT
5. 在MQTT连接成功后，在6*10=60s时间内，从topic推送下行数据，会从log中将数据打印
6. 6包数据发送完成，关闭MQTT

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：不需要
- 使用注意事项：
- APP使用前提：无

## **FAQ**
- 问题：为什么MQTT接口cm_mqtt_client_connect返回0，但是订阅失败？
  答复：因为该接口为异步连接，需等待__mqtt_manager_default_connack_cb上报最终结果后，再进行订阅。

  - 问题：cm_mqtt_connect_options_t conn_options需要注意什么？
  答复：cm_mqtt_client_connect因为该接口为异步连接，因此设置的结构体参数变量conn_options需要为全局变量，若为局部变量，则连接时该内存可能已被回收，导致连接失败。

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/11 18:08
- 修改记录：
  1. 初版

--------------------------------------------------------------------------------