
#include "custom_uart.h"
#include "custom_system.h"
#include "custom_sps.h"
#include "custom_usb.h"

typedef struct{
    int msg_type;
} uart_event_msg_t;

#ifdef	UART0

#define	UART0_RX_BUF_SIZE	(2*1024)					//串口线程接收缓冲区大小
#define UART0_PAKCET_TIME	100							//串口按照时间分包：连续多久没收到串口数据，就认为是一包数据。单位是ms

static osSemaphoreId_t s_uart0_rx_semaphore = NULL;		//串口数据接收信号量
static osMessageQueueId_t s_uart0_event_queue = NULL;	//串口事件队列
static osThreadId_t s_uart0_event_thread = NULL;		//串口事件线程

/* 串口事件任务 */
static void cm_uart0_event_task(void *arg)
{
	uart_event_msg_t msg = {0};

	while (1)
	{
		if (osMessageQueueGet(s_uart0_event_queue, &msg, NULL, osWaitForever) == osOK)
		{
			cm_log_printf(0, "uart0 event msg type = %d\n", msg.msg_type);
			
			if (CM_UART_EVENT_TYPE_RX_OVERFLOW & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART0_EVENT_TYPE_RX_OVERFLOW... ...");
			}

			if (CM_UART_EVENT_TYPE_RX_FLOWCTRL & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART0_EVENT_TYPE_RX_FLOWCTRL... ...");
			}
		}
	}
}

/* 串口事件创建 */
static int cm_uart0_event_task_create(void)
{
    if (s_uart0_event_queue == NULL)
    {
        s_uart0_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
    }

    if (s_uart0_event_thread == NULL)
    {
        osThreadAttr_t attr = {
            .name = "uart0_event",
            .priority = osPriorityNormal,
            .stack_size = 1024 * 2,
        };
        s_uart0_event_thread = osThreadNew(cm_uart0_event_task, NULL, (const osThreadAttr_t*)&attr);
    }

    return 0;
}

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart0_callback(void *param, uint32_t type)
{
	uart_event_msg_t msg = {0};

	// 接收事件
	if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
	{
		/* 触发线程执行读取数据 */
		osSemaphoreRelease(s_uart0_rx_semaphore);
	}

	// 溢出事件或流控事件
	if ((CM_UART_EVENT_TYPE_RX_OVERFLOW & type) || (CM_UART_EVENT_TYPE_RX_FLOWCTRL & type))
	{
		/* 触发其他线程处理事件 */
		msg.msg_type = type;
		
		if (s_uart0_event_queue != NULL)
		{
			osMessageQueuePut(s_uart0_event_queue, &msg, 0, 0);
		}
	}
}

/* uart0线程 */
void custom_uart0_task(void *p)
{
	/* 1. 设置GPIO复用 */
	cm_iomux_set_pin_func(CM_IOMUX_PIN_17, CM_IOMUX_FUNC_FUNCTION1);
	cm_iomux_set_pin_func(CM_IOMUX_PIN_18, CM_IOMUX_FUNC_FUNCTION1);

	/* 2. 创建串口通知信号量和串口事件接收线程 */
	s_uart0_rx_semaphore = osSemaphoreNew(1, 0, NULL);
	cm_uart0_event_task_create();

	/* 3. 注册串口事件参数 */
	cm_uart_event_t event = 
	{
		CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   //注册需要上报的事件类型
		"uart0",                                                        //用户参数
		cm_serial_uart0_callback                                         //上报事件的回调函数
	};
	cm_uart_register_event(UART0, &event);

	/* 4. 初始化串口配置参数 */
	cm_uart_cfg_t config = 
	{
		CM_UART_BYTE_SIZE_8, 
		CM_UART_PARITY_NONE,
		CM_UART_STOP_BIT_ONE, 
		CM_UART_FLOW_CTRL_NONE, 
		CM_UART_BAUDRATE_115200,    
		0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
		0,   //环形缓存区大小按照默认配置8k
		0,
		0,
	};
	cm_uart_open(UART0, &config);

	uint8_t *buf = cm_malloc(UART0_RX_BUF_SIZE);
	uint16_t recv_buflen = 0, single_recv_buflen = 0;
	while (1)
	{
		memset(buf, 0, UART0_RX_BUF_SIZE);
		recv_buflen = 0;
		osSemaphoreAcquire(s_uart0_rx_semaphore, osWaitForever); // 阻塞
		while (1)
		{
			single_recv_buflen = cm_uart_read(UART0, (buf + recv_buflen), UART0_RX_BUF_SIZE - recv_buflen, 100);
			if (single_recv_buflen <= 0)   //第一次检查无串口数据或出现异常
			{
				if (recv_buflen == 0)     //无串口数据
				{
					break;
				}

				osDelay(UART0_PAKCET_TIME / 5);  //延时后再次读取底层接收串口缓冲队列中的是否有数据
				single_recv_buflen = cm_uart_read(UART0, (buf + recv_buflen), UART0_RX_BUF_SIZE - recv_buflen, 100);
				if (single_recv_buflen <= 0 && recv_buflen > 0)    //无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
				{
					/* 5. 串口接收数据 */
					Sps_InUart0Scan(buf,recv_buflen);
					break;
				}
			}

			recv_buflen += single_recv_buflen;

			/* 到达buf空间后，也认为是一包数据 */
			if (recv_buflen >= UART0_RX_BUF_SIZE)
			{
				/* 6. 串口接收数据 */
				Sps_InUart0Scan(buf,recv_buflen);
				recv_buflen = 0; // 重新接收				
				break;
			}
        }
    }
}

#endif

#ifdef	UART1

#define	UART1_RX_BUF_SIZE	(2*1024)					//串口线程接收缓冲区大小
#define UART1_PAKCET_TIME	100							//串口按照时间分包：连续多久没收到串口数据，就认为是一包数据。单位是ms

static osSemaphoreId_t s_uart1_rx_semaphore = NULL;		//串口数据接收信号量
static osMessageQueueId_t s_uart1_event_queue = NULL;	//串口事件队列
static osThreadId_t s_uart1_event_thread = NULL;		//串口事件线程

/* 串口事件任务 */
static void cm_uart1_event_task(void *arg)
{
	uart_event_msg_t msg = {0};

	while (1)
	{
		if (osMessageQueueGet(s_uart1_event_queue, &msg, NULL, osWaitForever) == osOK)
		{
			cm_log_printf(0, "uart1 event msg type = %d\n", msg.msg_type);

			if (CM_UART_EVENT_TYPE_RX_OVERFLOW & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART1_EVENT_TYPE_RX_OVERFLOW... ...");
			}

			if (CM_UART_EVENT_TYPE_RX_FLOWCTRL & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART1_EVENT_TYPE_RX_FLOWCTRL... ...");
			}
		}
	}
}

/* 串口事件创建 */
static int cm_uart1_event_task_create(void)
{
	if (s_uart1_event_queue == NULL)
	{
		s_uart1_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
	}

	if (s_uart1_event_thread == NULL)
	{
		osThreadAttr_t attr = {
			.name = "uart1_event",
			.priority = osPriorityNormal,
			.stack_size = 1024 * 2,
		};
		s_uart1_event_thread = osThreadNew(cm_uart1_event_task, NULL, (const osThreadAttr_t*)&attr);
	}

	return 0;
}

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart1_callback(void *param, uint32_t type)
{
	uart_event_msg_t msg = {0};

	// 接收事件
	if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
	{
		/* 触发线程执行读取数据 */
		osSemaphoreRelease(s_uart1_rx_semaphore);
	}

	// 溢出事件或流控事件
	if ((CM_UART_EVENT_TYPE_RX_OVERFLOW & type) || (CM_UART_EVENT_TYPE_RX_FLOWCTRL & type))
	{
		/* 触发其他线程处理事件 */
		msg.msg_type = type;
		
		if (s_uart1_event_queue != NULL)
		{
			osMessageQueuePut(s_uart1_event_queue, &msg, 0, 0);
		}
	}
}

/* uart1线程 */
void custom_uart1_task(void *p)
{
	/* 1. 设置GPIO复用 */
	cm_iomux_set_pin_func(CM_IOMUX_PIN_28, CM_IOMUX_FUNC_FUNCTION1);
	cm_iomux_set_pin_func(CM_IOMUX_PIN_29, CM_IOMUX_FUNC_FUNCTION1);

	/* 2. 创建串口通知信号量和串口事件接收线程 */
	s_uart1_rx_semaphore = osSemaphoreNew(1, 0, NULL);
	cm_uart1_event_task_create();

	/* 3. 注册串口事件参数 */
	cm_uart_event_t event = 
	{
		CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   //注册需要上报的事件类型
		"uart1",                                                        //用户参数
		cm_serial_uart1_callback                                         //上报事件的回调函数
	};
	cm_uart_register_event(UART1, &event);

	/* 4. 初始化串口配置参数 */
	cm_uart_cfg_t config = 
    {
		CM_UART_BYTE_SIZE_8, 
		CM_UART_PARITY_NONE,
		CM_UART_STOP_BIT_ONE, 
		CM_UART_FLOW_CTRL_NONE, 
		CM_UART_BAUDRATE_115200,    
		0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
		0,   //环形缓存区大小按照默认配置8k
		0,
		0,
	};
	cm_uart_open(UART1, &config);

	uint8_t *buf = cm_malloc(UART1_RX_BUF_SIZE);
	uint16_t recv_buflen = 0, single_recv_buflen = 0;
	while (1)
	{
		memset(buf, 0, UART1_RX_BUF_SIZE);
		recv_buflen = 0;
		osSemaphoreAcquire(s_uart1_rx_semaphore, osWaitForever); // 阻塞
		while (1)
		{
			single_recv_buflen = cm_uart_read(UART1, (buf + recv_buflen), UART1_RX_BUF_SIZE - recv_buflen, 100);
			if (single_recv_buflen <= 0)   //第一次检查无串口数据或出现异常
			{
				if (recv_buflen == 0)     //无串口数据
				{
					break;
				}
				osDelay(UART1_PAKCET_TIME / 5);  //延时后再次读取底层接收串口缓冲队列中的是否有数据
				single_recv_buflen = cm_uart_read(UART1, (buf + recv_buflen), UART1_RX_BUF_SIZE - recv_buflen, 100);
				if (single_recv_buflen <= 0 && recv_buflen > 0)    //无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
				{
					/* 5. 串口接收数据 */					
					Sps_InUart1Scan(buf,recv_buflen);
					break;
				}
			}

			recv_buflen += single_recv_buflen;

			/* 到达buf空间后，也认为是一包数据 */
			if (recv_buflen >= UART1_RX_BUF_SIZE)
			{
				/* 6. 串口接收数据 */				
				Sps_InUart1Scan(buf,recv_buflen);
				recv_buflen = 0; // 重新接收
				break;
			}
		}
	}
}

#endif

#ifdef	UART2

#define	UART2_RX_BUF_SIZE		(2*1024)				//串口线程接收缓冲区大小
#define UART2_PAKCET_TIME		100						//串口按照时间分包：连续多久没收到串口数据，就认为是一包数据。单位是ms

static osSemaphoreId_t s_uart2_rx_semaphore = NULL;	//串口数据接收信号量
static osMessageQueueId_t s_uart2_event_queue = NULL;	//串口事件队列
static osThreadId_t s_uart2_event_thread = NULL;		//串口事件线程

/* 串口事件任务 */
static void cm_uart2_event_task(void *arg)
{
	uart_event_msg_t msg = {0};

	while (1)
	{
		if (osMessageQueueGet(s_uart2_event_queue, &msg, NULL, osWaitForever) == osOK)
		{
			cm_log_printf(0, "uart2 event msg type = %d\n", msg.msg_type);

			if (CM_UART_EVENT_TYPE_RX_OVERFLOW & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART2_EVENT_TYPE_RX_OVERFLOW... ...");
			}

			if (CM_UART_EVENT_TYPE_RX_FLOWCTRL & msg.msg_type)
			{
				cm_log_printf(0, "CM_UART2_EVENT_TYPE_RX_FLOWCTRL... ...");
			}
		}
	}
}

/* 串口事件创建 */
static int cm_uart2_event_task_create(void)
{
	if (s_uart2_event_queue == NULL)
	{
		s_uart2_event_queue = osMessageQueueNew(10, sizeof(uart_event_msg_t), NULL);
	}

	if (s_uart2_event_thread == NULL)
	{
		osThreadAttr_t attr = {
			.name = "uart2_event",
			.priority = osPriorityNormal,
			.stack_size = 1024 * 2,
		};
		s_uart2_event_thread = osThreadNew(cm_uart2_event_task, NULL, (const osThreadAttr_t*)&attr);
	}

	return 0;
}

/* 回调函数中不可输出LOG、串口打印、执行复杂任务或消耗过多资源，建议以信号量或消息队列形式控制其他线程执行任务 */
static void cm_serial_uart2_callback(void *param, uint32_t type)
{
	uart_event_msg_t msg = {0};

	// 接收事件
	if (CM_UART_EVENT_TYPE_RX_ARRIVED & type)
	{
		/* 触发线程执行读取数据 */
		osSemaphoreRelease(s_uart2_rx_semaphore);
	}

	// 溢出事件或流控事件
	if ((CM_UART_EVENT_TYPE_RX_OVERFLOW & type) || (CM_UART_EVENT_TYPE_RX_FLOWCTRL & type))
	{
		/* 触发其他线程处理事件 */
		msg.msg_type = type;
		
		if (s_uart2_event_queue != NULL)
		{
			osMessageQueuePut(s_uart2_event_queue, &msg, 0, 0);
		}
	}
}

/* uart2线程 */
void custom_uart2_task(void *p)
{
	/* 0. 切换DBG打印到USB */	
	custom_system_virt_at_usb();

	/* 1. 设置GPIO复用 */
	cm_iomux_set_pin_func(CM_IOMUX_PIN_38, CM_IOMUX_FUNC_FUNCTION2);
	cm_iomux_set_pin_func(CM_IOMUX_PIN_39, CM_IOMUX_FUNC_FUNCTION2);

	/* 2. 创建串口通知信号量和串口事件接收线程 */
	s_uart2_rx_semaphore = osSemaphoreNew(1, 0, NULL);
	cm_uart2_event_task_create();

	/* 3. 注册串口事件参数 */
	cm_uart_event_t event = 
	{
		CM_UART_EVENT_TYPE_RX_ARRIVED|CM_UART_EVENT_TYPE_RX_OVERFLOW,   	//注册需要上报的事件类型
		"uart2",                                                        	//用户参数
		cm_serial_uart2_callback                                         	//上报事件的回调函数
	};
	cm_uart_register_event(UART2, &event);

	/* 4. 初始化串口配置参数 */
	cm_uart_cfg_t config = 
	{
		CM_UART_BYTE_SIZE_8, 
		CM_UART_PARITY_NONE,
		CM_UART_STOP_BIT_ONE, 
		CM_UART_FLOW_CTRL_NONE, 
		CM_UART_BAUDRATE_115200,    
		0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
		0,   //环形缓存区大小按照默认配置8k
		0,
		0,
	};
	cm_uart_open(UART2, &config);

	uint8_t *buf = cm_malloc(UART2_RX_BUF_SIZE);
	uint16_t recv_buflen = 0, single_recv_buflen = 0;
	while (1)
	{
		memset(buf, 0, UART2_RX_BUF_SIZE);
		recv_buflen = 0;
		osSemaphoreAcquire(s_uart2_rx_semaphore, osWaitForever); // 阻塞
		while (1)
		{
			single_recv_buflen = cm_uart_read(UART2, (buf + recv_buflen), UART2_RX_BUF_SIZE - recv_buflen, 100);
			if (single_recv_buflen <= 0)   //第一次检查无串口数据或出现异常
			{
				if (recv_buflen == 0)     //无串口数据
				{
					break;
				}
				osDelay(UART2_PAKCET_TIME / 5);  //延时后再次读取底层接收串口缓冲队列中的是否有数据
				single_recv_buflen = cm_uart_read(UART2, (buf + recv_buflen), UART2_RX_BUF_SIZE - recv_buflen, 100);
				if (single_recv_buflen <= 0 && recv_buflen > 0)    //无串口数据（或接收异常）并且有接收的数据，就认为是一包数据
				{
					/* 5. 串口接收数据 */					
					Sps_InUart2Scan(buf,recv_buflen);
					break;
				}
			}

			recv_buflen += single_recv_buflen;

			/* 到达buf空间后，也认为是一包数据 */
			if (recv_buflen >= UART2_RX_BUF_SIZE)
			{
				/* 6. 串口接收数据 */
				Sps_InUart2Scan(buf,recv_buflen);
				recv_buflen = 0; // 重新接收
				break;
			}
		}
	}
}
#endif

/* 串口发送数据 */
int custom_uart_send(int dev, uint8_t *data, uint16_t len)
{
	if(dev == UART_USB)
		custom_usb_send(data, len);
	else
		cm_uart_write(dev, data, len, 100);

	return 0;
}

int custom_uart_init(void)
{    
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.stack_size = 1024 * 4;
    app_task_attr.priority = osPriorityNormal;
	
#ifdef	UART0
    app_task_attr.name  = "uart0_task";
    osThreadNew((osThreadFunc_t)custom_uart0_task, 0, &app_task_attr);
#endif

#ifdef	UART1
    app_task_attr.name  = "uart1_task";	
    osThreadNew((osThreadFunc_t)custom_uart1_task, 0, &app_task_attr);
#endif
	
#ifdef	UART2
	app_task_attr.name  = "uart2_task";	
    osThreadNew((osThreadFunc_t)custom_uart2_task, 0, &app_task_attr);
#endif

    return 0;
}

