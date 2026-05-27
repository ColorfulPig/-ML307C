# TD1030 NMEA数据解析测试 简介 

## **NMEA介绍**
NMEA是National Marine Electronics Association的缩写，是美国国家海洋电子协会的简称，现在是GPS导航设备统一的RTCM标准协议。NMEA-0183协议是目前GPS接收机上使用最广泛的协议，大多数常见的GPS接收机、GPS数据处理软件、导航软件都遵守或者至少兼容这个协议。

```
NMEA示例数据：

$GNRMC,025956.00,A,2937.18783,N,10629.67849,E,0.118,,041224,,,A,V*12
$GNGGA,025956.00,2937.18783,N,10629.67849,E,1,04,2.31,425.3,M,-27.3,M,,*62
$GNGSA,A,3,32,10,31,26,,,,,,,,,6.48,2.31,6.06,1*0D
$GNGSA,A,3,,,,,,,,,,,,,6.48,2.31,6.06,4*0E
$GPGSV,2,1,08,02,09,291,18,10,55,167,35,12,10,039,25,23,14,154,,0*6A
$GPGSV,2,2,08,26,29,194,27,31,46,262,41,32,61,033,36,50,49,143,36,0*60
$BDGSV,1,1,00,0*74

其中的$GNRMC、$GNGGA为NMEA关键字，本示例代码通过关键字进行数据检索，本示例中关键字定义如下：

#define NMEA_KEYWORD_RMC        "GNRMC"
#define NMEA_KEYWORD_GSA        "GNGSA"
#define NMEA_KEYWORD_GSV        "GPGSV"

当使用不同卫星定位芯片或不同定位系统时，对应的NMEA关键字存在差异，需根据实现情况进行修改。
```

## **实现功能**
实现通过串口接收GNSS器件（如TD1030）的NMEA数据并解析其中的关键信息，如：经纬度、速度、卫星数量等
1. 实现UART接收NMEA数据，默认使用UART0
2. 实现NMEA数据的解析，并将解析结果打印到DEBUG串口LOG

## **APP执行流程**
1. 创建DEMO测试任务及UART接收任务

```c
    osThreadAttr_t gnss_task_attr = {0};
    gnss_task_attr.name = "td1030_demo_task";  // 任务名称
    gnss_task_attr.stack_size = 1024 * 10; // 任务栈空间
    gnss_task_attr.priority= osPriorityNormal;    // 任务优先级
    osThreadNew((osThreadFunc_t)td1030_demo_task, 0, &gnss_task_attr);  // 创建并运行任务

    osThreadNew((osThreadFunc_t)td1030_uart_recv_task, 0, &gnss_task_attr);  // 创建并运行任务
```

2. 初始化UART串口及互斥锁、信号量等系统资源
3. UART事件回调中释放信号量

```c
    void _td1030_demo_gnss_uart_callback(void *param, uint32_t type)
    {
        if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
        {
            /* 收到接收事件，触发线程执行读取数据 */
            osSemaphoreRelease(s_uart_semaphore);
        }
    }
```

4. UART接收任务中获取信号量并读取串口内容，通过100ms内无下一包数据判断当次NMEA数据接收完毕

```c
    while (1)
    {
        single_recv_buflen = cm_uart_read(TD1030_DEMO_UART, (buf + recv_buflen), TD1030_DEMO_RX_SIZE - recv_buflen, 100);
        if (single_recv_buflen <= 0)   // 第一次检查无串口数据或出现异常
        {
            if (recv_buflen == 0)     // 无串口数据
            {
                break;
            }
            osDelay(100 / 5);
            single_recv_buflen = cm_uart_read(TD1030_DEMO_UART, (buf + recv_buflen), TD1030_DEMO_RX_SIZE - recv_buflen, 100);
            if (single_recv_buflen <= 0 && recv_buflen > 0)    // 无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
            {
                cm_uart_write(TD1030_DEMO_UART, buf, recv_buflen, 0);
                osMutexAcquire(s_gnss_mutex, osWaitForever);
                memset(s_gnss_recv_buf, 0, TD1030_DEMO_RX_SIZE);
                memcpy(s_gnss_recv_buf, buf, recv_buflen);
                s_gnss_recv_len = recv_buflen;
                osMutexRelease(s_gnss_mutex);
                break;
            }
        }

        recv_buflen += single_recv_buflen;

        /* 到达buf空间后，重新接收 */
        if (recv_buflen >= TD1030_DEMO_RX_SIZE)
        {
            recv_buflen = 0;
            break;
        }
    }
```

5. 将接收到的NMEA数据拷贝到另一个缓冲区，并进行解析
6. 截取字符串中的关键字内容

```c
    int app_demo_str_extract(uint8_t *buf, size_t bufsize, uint8_t *source_str, uint8_t *start_str, uint8_t *end_str)
```

7. 获取指定NMEA关键字的内容

```c
    int td1030_demo_get_data_from_nmea(uint8_t *data, uint8_t *nmea_data, uint8_t *keyword, int num)
```

8. 获取定位信息，包含定位是否成功，经纬度、速度、卫星数量等信息

```c
    typedef struct
    {
        int state;        // 定位状态 0 表示定位失败， 1 表示 GPS 定位成功， 2 表示 GPS 经纬度信息非实时，而是最近保存的经纬度信息
        char E_W;         // 东经、西经
        double longitude; // 经度
        char N_S;         // 南纬、北纬
        double latitude;  // 纬度
        double speed;     // 速度(km/h)
        double accuracy;  // 定位精确度(m)
        int sate_num;     // 可用卫星数量
    } gnss_info_t;

    void td1030_demo_gnss_info_get(uint8_t *nmea_data, gnss_info_t *gnss_info)
```

9.循环打印定位信息

```c
    while (1)
    {
        osDelay(1000 / 5);

        osMutexAcquire(s_gnss_mutex, osWaitForever);
        memcpy(s_gnss_recv_temp_buf, s_gnss_recv_buf, s_gnss_recv_len);
        osMutexRelease(s_gnss_mutex);

        gnss_info_t gnss_info = {0};
        td1030_demo_gnss_info_get(s_gnss_recv_temp_buf, &gnss_info);
        if (gnss_info.state)
        {
            cm_log_printf(0, "---------------- gnss Location succ ----------------");
            cm_log_printf(0, "---- lng: %.6lf(%C)", gnss_info.longitude, gnss_info.E_W);
            cm_log_printf(0, "---- lng: %.6lf(%C)", gnss_info.latitude, gnss_info.N_S);
            cm_log_printf(0, "---- accuracy: %lf", gnss_info.accuracy);
            cm_log_printf(0, "---- speed: %lf", gnss_info.speed);
            cm_log_printf(0, "---- sate_num: %02d", gnss_info.sate_num);
            cm_log_printf(0, "---------------- gnss Location end -----------------");
        }
        else
        {
            cm_log_printf(0, "gnss location fail.");
        }
    }
```

## **使用说明**
- 支持的模组（子）型号：ML307R-DC
- 支持的SDK版本：ML307R OpenCPU SDK 2.0.0版本及其后续版本
- 是否需要外设支撑：需要
- 使用注意事项：
（1）该DEMO仅做为数据解析的示例，若中途将GNSS器件连接断开后，串口在未获取到数据的情况下，仍会以上一次接收到的NMEA数据进行解析并打印结果；
（2）该DEMO未对AGNSS及LBS辅助定位等功能进行实现；
（3）当使用不同卫星定位系统时，对应的NMEA关键字存在差异，需根据实现情况进行修改。
- APP使用前提：无

## **FAQ**
- 无

## **版本更新说明**

### **1.0.0版本**
- 发布时间：2024/11/18 17:30
- 修改记录：
  1. 初版

--------------------------------------------------------------------------------