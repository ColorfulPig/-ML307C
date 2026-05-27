
#include "custom_bluetooth.h"
#include "custom_system.h"
#include "custom_track.h"
#include "custom_bms.h"
#include "custom_common.h"
#include "custom_profile.h"

// 蓝牙通讯：BMS发送给4G模块的内容检测包头包尾符合后就转发给BLE, BLE返回的内容则不做任何检查返回给BMS。

char bluetooth_Name[32] = {0};
char bluetooth_MAC[20] = {0};
uint8_t bluetooth_LinkState = 0;				// 连接状态：0-未连接,1-已连接
uint8_t bluetooth_LinkState_Update = 0;			// 更新标志
uint8_t bluetooth_Name_Update = 0;				// 更新标志
uint8_t bluetooth_MAC_Update = 0;				// 更新标志
uint8_t bluetooth_CommandAck_Wait = 0;			// 指令等待延时
uint32_t bluetooth_Link_NoDataCount = 0;		// 连接无数据超时
uint32_t bluetooth_Recv_NoDataCount = 0;		// 接收长时间超时

#define	BLUETOOTH_LINK_NODATA_WAIT		60		// 秒
#define	BLUETOOTH_RECV_NODATA_WAIT		300		// 秒
#define	BLUETOOTH_COMMAND_ACK_WAIT		5		// 秒

int custom_bluetooth_OnBlock(uint8_t *buf,uint32_t len)
{
	uint8_t head[3],tail[2];
	char *pstart,*pend;
	int dlen;
	
	if(len > 5)
	{
		memcpy(head, buf, 3);
		memcpy(tail, &buf[len-2], 2);

		// 蓝牙指令
		if((head[0] == '\r') && (head[1] == '\n') && (head[2] == '+') && (tail[0] == '\r') && (tail[1] == '\n'))
		{
			// 蓝牙名称："\r\n+NAME:xxxxxx\r\n"
			pstart = strstr((char *)buf, "\r\n+NAME:");
			if(pstart != NULL)
			{
				pstart += 8;				
				pend = strstr(pstart, "\r\n");
				if(pend != NULL)
				{
					dlen = pend - pstart;
					if(dlen > 0)
					{
						if(dlen > 28)	dlen = 28;	// 蓝牙名称显示最多28字节
						memset(bluetooth_Name, 0, 32);
						memcpy(bluetooth_Name, pstart, dlen);
						bluetooth_Name_Update = 1;
						BlueTooth_printf("BlueTooth Name:%s", bluetooth_Name);
					}
				}
			}
			
			// 蓝牙MAC地址："\r\n+MAC:xxxxxx\r\n"
			pstart = strstr((char *)buf, "\r\n+MAC:");
			if(pstart != NULL)
			{
				pstart += 7;				
				pend = strstr(pstart, "\r\n");
				if(pend != NULL)
				{
					dlen = pend - pstart;
					if((dlen > 0) && (dlen < 20))
					{
						if((strlen(bluetooth_MAC) != dlen) || (memcmp(bluetooth_MAC, pstart, dlen) != 0))
						{							
							memset(bluetooth_MAC, 0, 20);
							memcpy(bluetooth_MAC, pstart, dlen);
							custom_profile_setString(CONFIG_ITEM_BT_MAC, bluetooth_MAC);
							custom_profile_save();
							BlueTooth_printf("BlueTooth set mac!!!");
						}
						bluetooth_MAC_Update = 1;	
						BlueTooth_printf("BlueTooth MAC: %s", bluetooth_MAC);	
					}
				}
			}
			
			// 蓝牙连接成功："\r\n+LINK:CONNECTED\r\n"
			pstart = strstr((char *)buf, "\r\n+LINK:CONNECTED\r\n");
			if(pstart != NULL)
			{
				bluetooth_LinkState = 1;
				bluetooth_LinkState_Update = 1;
				bluetooth_Link_NoDataCount = BLUETOOTH_LINK_NODATA_WAIT;
				BlueTooth_printf("BlueTooth Link Connected!");
			}

			// 蓝牙连接断开："\r\n+LINK:DISCONNECTED\r\n"
			pstart = strstr((char *)buf, "\r\n+LINK:DISCONNECTED\r\n");
			if(pstart != NULL)
			{
				bluetooth_LinkState = 0;
				bluetooth_LinkState_Update = 1;
				BlueTooth_printf("BlueTooth Link Disconnected!");
			}
		}
		else	// 蓝牙数据
		{
			bluetooth_Link_NoDataCount = 0;
			bluetooth_Recv_NoDataCount = BLUETOOTH_RECV_NODATA_WAIT;
			bluetooth_CommandAck_Wait = BLUETOOTH_COMMAND_ACK_WAIT;
			custom_bms_send(buf, len);			// 转发给bms
			BlueTooth_printHex("BlueTooth to BMS:", buf, len);
		}
	}
	
	return 0;
}

int custom_bluetooth_send(uint8_t *buf, uint16_t len)
{
	custom_uart_send(BLUETOOTH_COMM, buf, len);
	BlueTooth_printRaw("send to BlueTooth:", buf, len);
	return 0;
}

void custom_bluetooth_task(void *p)
{	
	int count = 0;
	
	custom_profile_getString(CONFIG_ITEM_BT_MAC, bluetooth_MAC);
	//BlueTooth_printf("bluetooth_MAC: %s", bluetooth_MAC);
		
	while(1)
	{
		osDelay(ONE_SECONED);	// 1秒
		count++;

		// 首次更新蓝牙信息
		if((count % 3) == 0)
		{
			if(bluetooth_MAC_Update == 0)
			{
				custom_bluetooth_send((uint8_t *)"AT+MAC?\r\n", 9);
			}
			else if(bluetooth_Name_Update == 0)
			{
				custom_bluetooth_send((uint8_t *)"AT+NAME?\r\n", 10);
			}
			else if(bluetooth_LinkState_Update == 0)
			{
				custom_bluetooth_send((uint8_t *)"AT+LINK?\r\n", 10);
			}
		}

		// 蓝牙断开检测
		if(bluetooth_LinkState == 1)
		{
			// 蓝牙连接后无数据-断开连接
			if(bluetooth_Link_NoDataCount > 0)
			{
				bluetooth_Link_NoDataCount--;
				if(bluetooth_Link_NoDataCount == 0)
				{
					custom_bluetooth_send((uint8_t *)"AT+DISCONN\r\n", 12);		// 断开连接
					BlueTooth_printf("BlueTooth Link NoData Disconnect!");
				}
			}

			// 蓝牙长时间无数据-断开连接
			if(bluetooth_Recv_NoDataCount > 0)
			{
				bluetooth_Recv_NoDataCount--;
				if(bluetooth_Recv_NoDataCount == 0)
				{
					custom_bluetooth_send((uint8_t *)"AT+DISCONN\r\n", 12);		// 断开连接
					BlueTooth_printf("BlueTooth Recv NoData Disconnect!");
				}
			}

			// 蓝牙指令等待应答
			if(bluetooth_CommandAck_Wait > 0)
			{
				bluetooth_CommandAck_Wait--;
			}
		}
		else
		{
			bluetooth_CommandAck_Wait = 0;
		}
	}
}

int custom_bluetooth_init(void)
{    
	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "bluetooth_task";
	app_task_attr.stack_size = 1024 * 4;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_bluetooth_task, 0, &app_task_attr);

	return 0;
}

/*
蓝牙测试格式：BT:蓝牙数据

AT+NAME?\r\n
\r\n+NAME:LS-BLE\r\n

AT+MAC?\r\n
\r\n+MAC:EBD9054F52DE\r\n

AT+LINK?\r\n
\r\n+LINK:CONNECTED\r\n
\r\n+LINK:DISCONNECTED\r\n

AT+DISCONN\r\n
\r\n+DISCONN:OK\r\n
\r\n+LINK:DISCONNECTED\r\n

*/

