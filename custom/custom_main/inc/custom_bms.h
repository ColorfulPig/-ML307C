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


int custom_bms_init(void);
int custom_bms_SendFrame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);
int custom_bms_OnChar(uint8_t ch);	
int custom_bms_OnFrame(void);	
int custom_bms_OnFinish(void);	
int custom_bms_OnBlock(uint8_t *buf,uint32_t len);
int custom_bms_send_frame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len);


#endif


