# 异步socket  

## **异步socket功能介绍**
异步Socket编程的核心思想是在发起网络请求后，调用者不需要等待操作完成即可继续执行其他任务。当操作完成时，系统会通过状态、通知或回调等方式通知调用者。这种机制使得设备可以在等待网络响应的同时执行其他操作，从而提高整体性能和用户体验‌。


## **实现功能**
本APP实现用asocket连接TCP/UDP服务器，并实现数据收发的功能。

1. 测试本示例前请修改宏定义设置连接的服务器地址、端口和类型；
2. 根据服务器地址是否是ip地址判断是否需要域名解析，若需要域名解析则使用解析成功后的ip地址连接服务器；
3. 若连接服务器成功，则每10秒发送一次数据，共发送6次，发送完成后断开连接；若连接服务器失败，则直接断开；
4. 连接成功后的1分钟内，服务器可下发数据（该app每次接收数据的最大长度为128字节，大小用户可自行更改），程序在回调中接收并打印数据。

## **APP执行流程**

1. 进入asocket测试线程，创建asocket状态事件组，然后等待模组pdp激活；

```c
    s_asocket_state_eventflag = osEventFlagsNew(0);     //创建网络状态事件组

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

2.  向asocket eloop发送连接服务器事件，使得在asocket eloop底层线程中执行cm_asocket_start_eloop_cb函数。

```c
    cm_eloop_event_handle_t asocket_start_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_start_eloop_cb, NULL);
    cm_eloop_post_event(asocket_start_event);
```

3. asocket eloop底层线程接收到事件后，执行对应的事件函数（以上是底层自行完成，用户无需关心），即执行cm_asocket_start_eloop_cb。进入eloop事件函数中，首先应该注销事件，否则将周期触发。cm_asocket_start_eloop_cb中注销事件后，首先判断服务器地址类型：使用IP地址转换函数inet_pton，分别使用IPV6和IPV4转换，若转化成功，则是对应协议栈的ip地址，发送事件socket_connect_event到asocket eloop线程中准备连接服务器；若都失败，则是域名，发送事件到dns eloop线程中进行解析。

```c
    cm_eloop_unregister_event(event);

    struct sockaddr_in6 server_addr6;
    struct sockaddr_in server_addr4;
    //判断服务器地址的类型
    if (inet_pton(AF_INET6, ASOCKET_TEST_SERVER, &server_addr6.sin6_addr))  //ipv6地址 
    {
        cm_log_printf(0,"test asocket server is ipv6 address:%s\n",ASOCKET_TEST_SERVER);
        strcpy(s_asocket_test_server_ip, ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t socket_open_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)param);
        cm_eloop_post_event(socket_open_event);
    }
    else if (inet_pton(AF_INET, ASOCKET_TEST_SERVER, &server_addr4.sin_addr))   //ipv4地址
    {
        cm_log_printf(0,"test asocket server is ipv4 address:%s\n",ASOCKET_TEST_SERVER);
        strcpy(s_asocket_test_server_ip, ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t socket_open_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)param);
        cm_eloop_post_event(socket_open_event);
    }
    else    //域名
    {
        cm_log_printf(0,"test asocket server is domin address:%s\n",ASOCKET_TEST_SERVER);
        cm_eloop_event_handle_t dns_event = cm_eloop_register_event(cm_async_dns_eloop(), cm_asocket_dns_eloop_cb, (void *)param);
        cm_eloop_post_event(dns_event);
    }
```

4. cm_asocket_dns_eloop_cb域名解析事件函数中，先注销事件，然后调用域名解析接口cm_async_dns_request进行域名解析（这里优先使用ipv4解析，用户可自行更改），若直接返回0，则表示直接解析成功，不会再通过回调通知，则将解析后的ip地址放入全局变量中，并发送连接事件socket_connect_event到asocket eloop线程中准备连接服务器；若返回>0，则表示将通过异步回调函数通知解析结果；若返回<0，表示解析失败，发送连接断开的标志位到全局的状态事件组s_asocket_state_eventflag中。

```c
    cm_eloop_unregister_event(event);

    cm_async_dns_ip_addr_t ip_addr;
    int ret = cm_async_dns_request(ASOCKET_TEST_SERVER, CM_ASYNC_DNS_ADDRTYPE_IPV4_IPV6, &ip_addr, __cm_asocket_dns_cb, (void *)param);

    if (0 == ret)
    {
        if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr.type)
        {
            inet_ntop(AF_INET, &(ip_addr.u_addr.sin_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
            cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, NULL);
            cm_eloop_post_event(socket_connect_event);
        }
        else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr.type)
        {
            inet_ntop(AF_INET6, &(ip_addr.u_addr.sin6_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
            cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, NULL);
            cm_eloop_post_event(socket_connect_event);
        }
    }
    else if (ret > 0)
    {
        cm_log_printf(0,"dns req(%d) pending...\n", ret);
    }
    else
    {
        cm_log_printf(0,"dns req fail %d\n", ret);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
    }
```

5. 域名解析回调函数__cm_asocket_dns_cb中，域名解析成功则将解析后的ip地址放入全局变量中，并发送连接事件socket_connect_event到asocket eloop线程中准备连接服务器；解析失败，发送连接断开的标志位到全局的状态事件组s_asocket_state_eventflag中。

```c
    cm_log_printf(0,"__on_async_dns_event(%d, %d, %s)", req_id, event, host_name);
    switch (event)
    {
        case CM_ASYNC_DNS_RESOLVE_OK:
        {
            if (CM_ASYNC_DNS_ADDRTYPE_IPV4 == ip_addr->type)
            {
                inet_ntop(AF_INET, &(ip_addr->u_addr.sin_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
                cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)cb_param);
                cm_eloop_post_event(socket_connect_event);
            }
            else if (CM_ASYNC_DNS_ADDRTYPE_IPV6 == ip_addr->type)
            {
                inet_ntop(AF_INET6, &(ip_addr->u_addr.sin6_addr), s_asocket_test_server_ip, sizeof(s_asocket_test_server_ip));
                cm_eloop_event_handle_t socket_connect_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_connect_eloop_cb, (void *)cb_param);
                cm_eloop_post_event(socket_connect_event);
            }
            break;
        }
        case CM_ASYNC_DNS_RESOLVE_FAIL:
        {
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
            break;
        }
        default:
        {
            break;
        }
    }
```

6. 底层asocket eloop线程接收到连接事件socket_connect_event后，触发cm_asocket_connect_eloop_cb ，首先注销事件，然后根据ip地址的类型使用不同的协议栈（domain变量），根据宏定义中的连接类型设置不同的连接类型（type变量），再创建socket id，并注册socket 异步回调事件；创建socket id成功后，开始调用cm_asocket_connect连接服务器。若接口直接返回0，表示连接成功，将不会再通过socket 异步回调函数通知连接结果，发送连接成功的标志位到全局的状态事件组s_asocket_state_eventflag中；若<0且EINPROGRESS == errno，则表示异步连接中，等待socket 异步回调通知连接结果；其他情况则表示连接失败，则发送连接断开的标志位到全局的状态事件组s_asocket_state_eventflag中。

```c
    /* 注销Event */
    cm_eloop_unregister_event(event);
    int ret = 0,type = SOCK_DGRAM,domain = AF_INET;
    struct sockaddr_in6 server_addr6 = {0};
    struct sockaddr_in server_addr4 = {0};

    if(ASOCKET_TEST_TYPE == 0)  //TCP
    {
        type = SOCK_STREAM;
    }

    //判断服务器地址的类型
    if (inet_pton(AF_INET6, s_asocket_test_server_ip, &server_addr6.sin6_addr))  //ipv6地址 
    {
        server_addr6.sin6_port = htons(ASOCKET_TEST_PORT);
        server_addr6.sin6_family = AF_INET6;
        domain = AF_INET6;
    }
    else if (inet_pton(AF_INET, s_asocket_test_server_ip, &server_addr4.sin_addr))   //ipv4地址
    {
        server_addr4.sin_family = AF_INET;
        //server_addr4.sin_addr.s_addr = inet_addr(s_asocket_test_server_ip);
        server_addr4.sin_port = htons(ASOCKET_TEST_PORT);
    }
    else
    {
        cm_log_printf(0,"asocket ip address(%s) is abnormal,will disconnect\n",s_asocket_test_server_ip);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        return ;
    }

    if (0 == ASOCKET_TEST_TYPE) // TCP
    {
        s_asocket_test_socketid = cm_asocket_open(domain, type, IPPROTO_TCP, __cm_asocket_event_cb, (void *)param);
    }
    else // UDP
    {
        s_asocket_test_socketid = cm_asocket_open(domain, type, IPPROTO_UDP, __cm_asocket_event_cb, (void *)param);
    }
    if (s_asocket_test_socketid < 0)
    {
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        return;
    }

    if(domain == AF_INET6)
    {
        ret = cm_asocket_connect(s_asocket_test_socketid, (const struct sockaddr *)&server_addr6, sizeof(server_addr6));
    }
    else
    {
        ret = cm_asocket_connect(s_asocket_test_socketid, (const struct sockaddr *)&server_addr4, sizeof(server_addr4));
    }

    if (ret < 0)
    {
        if (EINPROGRESS == errno)
        {
            cm_log_printf(0,"asocket socket id(%d) open request success, wait connect...\n", s_asocket_test_socketid);
        }
        else
        {
            cm_log_printf(0,"asocket socket id(%d) connect failed(%d)\n", s_asocket_test_socketid, errno);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
        }
    }
    else if (0 == ret)
    {
        cm_log_printf(0,"asocket socket id(%d) connect_ok\n", s_asocket_test_socketid);
        osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT);
    }
```

7. socket异步回调函数__cm_asocket_event_cb中，若event为CM_ASOCKET_EV_CONNECT_OK，表示连接成功，发送连接成功的标志位到全局的状态事件组s_asocket_state_eventflag中；若event为CM_ASOCKET_EV_CONNECT_FAIL，表示连接失败，发送连接断开的标志位到全局的状态事件组s_asocket_state_eventflag中；若event为CM_ASOCKET_EV_RECV_IND，表示收到了服务器的数据，接收并打印数据；若event为CM_ASOCKET_EV_ERROR_IND，表示连接出现了异常，打印异常。

```c
    cm_log_printf(0,"asocket socket id(%d) event :%d\n", sock,event);
    switch (event)
    {
        // responses
        case CM_ASOCKET_EV_CONNECT_OK: 
        {
            cm_log_printf(0,"asocket socket id(%d) connect_ok\n", sock);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT);
            break;
        }
        case CM_ASOCKET_EV_CONNECT_FAIL: 
        {
            cm_log_printf(0,"sock(%d) connect_fail\n", sock);
            osEventFlagsSet(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT);
            break;
        }
        // indications
        case CM_ASOCKET_EV_RECV_IND: 
        {
            /* 取得获取接收缓存中可读的数据长度 */
            int recv_avail = 0;
            cm_asocket_ioctl(sock, FIONREAD, &recv_avail);

            /* 接收数据，每次接收数据的最大长度为128字节，大小用户可自行更改 */
            uint8_t recv_buf[128] = {0};
            int ret = cm_asocket_recv(sock, recv_buf, sizeof(recv_buf), 0);
            
            if (ret > 0)
            {
                cm_log_printf(0,"sock(%d) recv_ind: recv_avail=%d, recv_len=%d, data=%.*s\n", sock, recv_avail, ret, ret, recv_buf);
            }
            else
            {
                cm_log_printf(0,"sock(%d) recv_ind error(%d)\n", sock, errno);

                if (ENOTCONN == errno)
                {
                    /* Connection closed */
                    cm_log_printf(0,"sock(%d) recv_ind: Connection closed\n", sock);
                }
            }
            
            break;
        }
        case CM_ASOCKET_EV_SEND_IND:
        {
            cm_log_printf(0,"sock(%d) send_ind\n", sock);
            break;
        }    
        case CM_ASOCKET_EV_ACCEPT_IND:
        {
            cm_log_printf(0,"sock(%d) accept_ind\n", sock);
            break;
        }
        case CM_ASOCKET_EV_ERROR_IND: 
        {
            /* 获取socket错误码 */
            int sock_error = 0;
            socklen_t opt_len = sizeof(sock_error);
            
            cm_asocket_getsockopt(sock, SOL_SOCKET, SO_ERROR, &sock_error, &opt_len);
            cm_log_printf(0,"sock(%d) error_ind: sock_error(%d)\n", sock, sock_error);
            
            if (ECONNABORTED == sock_error)
            {
                /* Connection aborted */
                cm_log_printf(0,"sock(%d) error_ind: Connection aborted\n", sock);
            }
            
            else if (ECONNRESET == sock_error)
            {
                /* Connection reset */
                cm_log_printf(0,"sock(%d) error_ind: Connection reset\n", sock);
            }
            
            else if (ENOTCONN == sock_error)
            {
                /* Connection closed */
                cm_log_printf(0,"sock(%d) error_ind: Connection closed\n", sock);
            }
            
            break;
        }
        default:
        {
            break;
        }
    }
```

8. asocket测试线程中，进行步骤2后，就开始执行定义asocket eloop发送数据事件和关闭连接事件，然后等待s_asocket_state_eventflag事件组连接标志位或者断开标志位。

```c
    char asocket_data[] = "hello world\r\n";    
    cm_eloop_event_handle_t asocket_send_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_send_eloop_cb, asocket_data);
    cm_eloop_event_handle_t asocket_close_event = cm_eloop_register_event(cm_asocket_eloop(), cm_asocket_close_eloop_cb, NULL);

    uint32_t event_flag = osEventFlagsWait(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT | ASOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, osWaitForever);
```

9. 若s_asocket_state_eventflag事件组若接收到连接标志位，则发送发送数据asocket_send_event事件到asocket eloop中，每10s发送一次，共发送6次；中途若发现连接断开或者到达最大发送次数，则发送断开连接asocket_close_event事件到asocket eloop中。若s_asocket_state_eventflag事件组若接收到连接断开标志位，则发送断开连接asocket_close_event事件到asocket eloop中；

```c
    uint32_t event_flag = osEventFlagsWait(s_asocket_state_eventflag, ASOCKET_CONNECTTED_BIT | ASOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, osWaitForever);
    if(event_flag & ASOCKET_CONNECTTED_BIT)
    {
        cm_log_printf(0, "check asocket has been connected,will send data\n");
        uint8_t retry = 6;
        while(retry --)
        {
            cm_eloop_post_event(asocket_send_event);
            uint32_t event_flag2 = osEventFlagsWait(s_asocket_state_eventflag, ASOCKET_DISCONNECTTED_BIT, osFlagsWaitAny, 10 * 200);
            if(!(event_flag2 & 0x80000000)  &&  (event_flag2 & ASOCKET_DISCONNECTTED_BIT))  //若超时则会返回0xffffffff,可判断最高位不能为1
            {
                cm_log_printf(0, "check asocket has been disconnected,will close socket\n");
                break;
            }
            if(retry == 0)
            {
                cm_log_printf(0, "6th send data has been completed, connection will be closed\n");
            }
        }
        cm_eloop_post_event(asocket_close_event);
    }
    if(event_flag & ASOCKET_DISCONNECTTED_BIT)
    {
        cm_log_printf(0, "check asocket has been disconnected,will close socket\n");
        cm_eloop_post_event(asocket_close_event);
    }
```

10. 底层asocket eloop线程接收到连接事件socket_connect_event后，注销事件，调用发送接口cm_asocket_send发送参数数据（参数指针为发送事件时传入的参数指针数据）；

```c
    /* 注销Event */
    cm_eloop_unregister_event(event);

    char *asocket_data = (char *)param;
    if(s_asocket_test_socketid >= 0)
    {
        cm_asocket_send(s_asocket_test_socketid,asocket_data,strlen(asocket_data),0);
    }
```

11. 底层asocket eloop线程接收到连接事件asocket_close_event后，注销事件，调用cm_asocket_close断开socket。

```c
   /* 注销Event */
    cm_eloop_unregister_event(event);

    if(s_asocket_test_socketid >= 0)
    {
        cm_asocket_close(s_asocket_test_socketid);
        s_asocket_test_socketid = -1;
    } 
```

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：无
- 使用注意事项：  
    1. asocket相关接口尽可能在asocket eloop事件回调使用，防止多线程使用时出现异常（使用asocket eloop则会在底层的asocket eloop线程中挨个调用）；
    2. socket异步回调中尽可能不要申请大数组或者进行复杂操作（如http等），因该回调函数在asocket eloop线程调用，且该线程较小，容易造成内存溢出导致死机；
    3. 测试ipv6时，建议客户先确定sim卡是否打开了ipv6的业务（可使用手机卡测试，一般手机卡开通了ipv6），可通过usb AT口使用AT指令（AT+MIPCALL?）查询是否有获取到ipv6的地址判断是否可测试ipv6业务。

## **FAQ（非必要，视客户/FAE咨询情况增列）**

- 无

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/1 17:44
- 修改记录：
  1. 初版


--------------------------------------------------------------------------------