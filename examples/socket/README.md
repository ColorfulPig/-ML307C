# LWIP SOCKET接口测试 简介 

## LWIP SOCKET介绍**
LWIP socket接口是一种通信协议，用于在网络中实现进程间的通信。它是一种抽象的编程接口，允许应用程序通过网络发送和接收数据。
socket接口允许应用程序通过它发送或接收数据，可对其进行像对文件一样的打开、读写和关闭等操作。它允许应用程序将I/O插入到网络中，并与网络中的其他应用程序进行通信。
应用场景：
socket接口广泛应用于各种网络编程场景，如Web服务器与客户端之间的HTTP通信、文件传输应用程序（如FTP）的文件上传和下载、即时通讯软件的消息传输等。
特点：
LWIP的socket接口兼容BSD socket接口，但只实现了部分功能，因为LWIP设计之初就是为了在嵌入式平台中使用。
在嵌入式平台中，LWIP的socket接口提供了足够的网络通信功能，且具有较高的实时性和稳定性。
综上所述，LWIP的socket接口是一种重要的网络通信接口，它提供了一种标准的通信方式，使得不同的程序能够在网络上进行高效、可靠的数据交换。

## **实现功能**
本APP实现用lwip socket连接TCP/UDP服务器，并实现数据收发的功能。
注意：本示例只支持IPv4

1. 测试本示例前请修改宏定义设置连接的服务器地址、端口和类型；
2. 根据服务器地址是否是ip地址判断是否需要域名解析，若需要域名解析则使用解析成功后的ip地址连接服务器；
3. 若连接服务器成功，则每10秒发送一次数据，共发送6次，发送完成后断开连接；若连接服务器失败，则直接断开；
4. 连接成功后的1分钟内，服务器可下发数据，程序在回调中接收并打印数据。

## **APP执行流程**
1. 进入lwip socket测试线程，创建socket状态事件组，然后等待模组pdp激活；
2. 连接socket，根据宏定义配置的socket类型TCP/UDP、连接地址、端口号进行连接。若是域名则进行DNS解析为IP后连接
3. 创建socket接收线程，在线程中会轮询socket是否有事件发生，用于监控socket是否有数据下发、异常断开等情况
4. 发起socket连接后，会得到连接结果，连接成功与连接失败，连接成功后，会按照10s定时发送一包数据至服务器；连接失败后，会关闭socket
5. 在socket连接成功后，在6*10=60s时间内，从服务器下行数据，会从log中将数据打印。
6. 在发送过程中，如果出现socket断线，则关闭socket；6包数据发送完成，关闭socket。

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：不需要
- 使用注意事项：socket接收中使用了下行数据接收buffer，示例中只开辟了128个字节，测试时勿超过128字节
- APP使用前提：无

## **FAQ**
- 问题：如何实现socket TCP的长连接？
  答复： //在连接成功前打开keepalive功能 
        int keepalive = 1;
        setsockopt(socket_id, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepalive, sizeof(keepalive)) < 0
        //在连接成功后配置
        int32_t keepidle = 90;
        int32_t keepinterval = 60;
        int32_t keepcount = 2;
        int32_t flag = 1;
        // 如果已在套接字上设置套接字选项SO_KEEPALIVE，则连接必须保持空闲状态的时间（以秒为单位），然后TCP才开始发送保持活动的探测。
        setsockopt(socket_id, IPPROTO_TCP, TCP_KEEPIDLE, (void *)&keepidle, sizeof(keepidle));
        // 设置连接上如果没有数据发送的话，多久后发送keepalive探测分组，单位是秒
        setsockopt(socket_id, IPPROTO_TCP, TCP_KEEPINTVL, (void *)&keepinterval, sizeof(keepinterval));
        // 关闭一个非活跃连接之前的最大重试次数
        setsockopt(socket_id, IPPROTO_TCP, TCP_KEEPCNT, (void *)&keepcount, sizeof(keepcount));
        setsockopt(socket_id, IPPROTO_TCP, TCP_NODELAY, (void *)&flag, sizeof(flag));

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/11 18:08
- 修改记录：
  1. 初版

--------------------------------------------------------------------------------