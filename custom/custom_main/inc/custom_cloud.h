#ifndef __CUSTOM_CLOUD_H__
#define __CUSTOM_CLOUD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"

typedef struct
{
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
}CLOUD_SPS;


/* 初始化 Cloud 协议缓冲区并创建后台任务。 */
int custom_cloud_init(void);
/* 封装本地 Cloud 协议帧并交给 OneNET 上报。 */
int custom_cloud_sendFrame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);
/* 逐字节接收并解析 Cloud 协议帧。 */
int custom_cloud_OnChar(uint8_t ch);	
/* 处理已经解析完成的 Cloud 协议帧并分发到对应业务。 */
int custom_cloud_OnFrame(void);	
/* 处理 Cloud 帧解析结束事件。 */
int custom_cloud_OnFinish(void);	

#endif


