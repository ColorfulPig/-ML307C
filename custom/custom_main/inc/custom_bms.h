#ifndef __CUSTOM_BMS_H__
#define __CUSTOM_BMS_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"

typedef struct
{
	uint8_t		LinkOK;
	uint8_t		state;		// 状态机
	uint16_t	t_len;		// 临时计数

	uint8_t		cid;		// 命令ID
	uint8_t		tid;		// 通讯序列号
	uint16_t	data_len;	// 消息体长度
	uint8_t		*data_body;
	uint8_t		check;		// 异或

	uint16_t	frame_len;	// 重构帧
	uint8_t		*frame;

	uint8_t		*base64_buffer;	// 加密缓冲

	uint8_t		*sbuf;		// 发送缓冲
}BMS_SPS;

// 115200
#define	BMS_COMM	UART0

#define	custom_bms_send(buf, len)	custom_uart_send(BMS_COMM, buf, len)

#define	custom_bms_IsLinkOK()		(bms_sps.LinkOK == 1)


/* 初始化 BMS 通信缓冲区、串口和任务。 */
int custom_bms_init(void);
/* 按自定义协议封装一帧数据并发送到 BMS。 */
int custom_bms_SendFrame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);
/* 逐字节接收并解析 BMS 串口协议帧。 */
int custom_bms_OnChar(uint8_t ch);	
/* 处理已经解析完成的 BMS 协议帧。 */
int custom_bms_OnFrame(void);	
/* 处理 BMS 帧解析结束事件。 */
int custom_bms_OnFinish(void);	
/* 处理 BMS 串口收到的一段原始数据。 */
int custom_bms_OnBlock(uint8_t *buf,uint32_t len);
/* 按自定义协议封装一帧数据并发送到 BMS。 */
int custom_bms_send_frame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);


#endif


