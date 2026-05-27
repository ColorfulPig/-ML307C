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


int custom_cloud_init(void);
int custom_cloud_sendFrame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);
int custom_cloud_OnChar(uint8_t ch);	
int custom_cloud_OnFrame(void);	
int custom_cloud_OnFinish(void);	

#endif


