#include "custom_usb.h"
#include "custom_system.h"
#include "custom_sps.h"
#include "custom_track.h"

/**
USB功能是将USB例化成串口使用，连接电脑设备管理器会出现2个ASR Modem Device。其中一个为USB例化串口。
注意：该USB存在一特性：需先以某波特率发任意字节给模组，进行波特率适配，模组才能通过USB往外输出数据。
 */ 
static osThreadId_t USB_TASK_HANDLE = NULL;			// USB线程
static osMessageQueueId_t USB_QUEUE_HANDLE = NULL;	// USB队列，用于接收USB数据

/**
 *  @brief USB接收数据回调函数
 *  @details 当USB有数据到来时会触发该回调，并将数据推送到USB主线程中进行回显打印
 */
static void custom_usb_recv_callback(void *data, int32_t len)						// 实测USB单次接收512字节
{
	if (len > 0)
	{
		custom_usb_data_t *usb_recv_data = cm_malloc(sizeof(custom_usb_data_t));
		if (usb_recv_data != NULL)
		{
			memcpy(usb_recv_data->data, data, len);
			usb_recv_data->datalen = len;
			usb_recv_data->event = CUSTOM_USB_EVENTRECV;
			
			if (osOK != osMessageQueuePut(USB_QUEUE_HANDLE, &usb_recv_data, 0, 0))   //timeout一定要为0  不能在回调中做阻塞操作
			{
				cm_free(usb_recv_data);	// 失败释放
			}
		}
	}
}

/**
 *  @brief USB插拔事件回调函数
 *  @details 当USB有插拔事件到来时会触发该回调，并将事件推送到USB主线程中进行打印
 */
static void custom_usb_event_callback(int32_t event)
{
	custom_usb_data_t *usb_recv_data = cm_malloc(sizeof(custom_usb_data_t));
	if (usb_recv_data != NULL)
	{
		usb_recv_data->event = event;
		
		if (osOK != osMessageQueuePut(USB_QUEUE_HANDLE, &usb_recv_data, 0, 0))
		{
			cm_free(usb_recv_data);	// 失败释放
		}
	}
}

/**
 *  @brief USB主线程
 *  @details 用于接收来自USB接收回调队列中的数据，并回显相同数据至USB以及用于打印插拔事件
 */
static void custom_usb_task(void)
{
	while (1)
	{
		custom_usb_data_t *usb_data = {0};

		if (osOK == osMessageQueueGet(USB_QUEUE_HANDLE, &usb_data, NULL, osWaitForever))
		{
			switch (usb_data->event)
			{					
				case CUSTOM_USB_EVENTINSERT:	// USB插入事件
				{
					SYSTEM_printf("CUSTOM_USB_EVENTINSERT");	
					break;
				}
				case CUSTOM_USB_EVENTREMOVE:	// USB移除事件
				{
					SYSTEM_printf("CUSTOM_USB_EVENTREMOVE");
					break;
				}
				case CUSTOM_USB_EVENTRECV:		// USB接收事件
				{
					SYSTEM_printf("CUSTOM_USB_EVENTRECV:%d", usb_data->datalen);	
					Sps_InUsbScan(usb_data->data,usb_data->datalen);
					break;
				}
				default:
					break;
			}
			
			cm_free(usb_data);
			usb_data = NULL;
		}
	}
}

#define USB_SEND_MAX	108

int custom_usb_send(uint8_t *data, int32_t len)
{
	int k,m,n;
	
	if((cm_usb2com_get_status() != 0) && (len > 0))
	{
		m = len / USB_SEND_MAX;
		n = len % USB_SEND_MAX;
		
		if(m > 0)
		{
			for(k=0; k < m; k++)
			{
				cm_usb2com_send_data(data, USB_SEND_MAX);		// 该函数最多只能打印108字节，超过字节不打印。
				data += USB_SEND_MAX;
			}
		}
		
		if(n > 0)
		{
			cm_usb2com_send_data(data, n);
		}		
	}

	return 0;
}

int custom_usb_init(void)
{
#ifndef	UART2													// 因为UART2中已经调用该函数
	/* 切换DBG打印到USB */	
	custom_system_virt_at_usb();
#endif
	cm_usb2com_register_recv_cb(custom_usb_recv_callback);		// 数据接收回调
	cm_usb2com_register_status_cb(custom_usb_event_callback);	// USB插拔检测事件回调

	/*创建USB数据接收队列*/
	if (USB_TASK_HANDLE == NULL)
	{
		USB_QUEUE_HANDLE = osMessageQueueNew(20, sizeof(custom_usb_data_t *), NULL); // 创建消息队列 用于接收USB回调中的数据
	}

	/*创建USB数据接收/发送处理线程*/
	osThreadAttr_t usb_demo_main_task_attr = {0};
	usb_demo_main_task_attr.name = "usb_task";
	usb_demo_main_task_attr.stack_size = 4 * 1024;
	usb_demo_main_task_attr.priority = osPriorityNormal;
	if (USB_TASK_HANDLE == NULL)
	{
		USB_TASK_HANDLE = osThreadNew((osThreadFunc_t)custom_usb_task, 0, &usb_demo_main_task_attr);
	}

	return 0;
}
