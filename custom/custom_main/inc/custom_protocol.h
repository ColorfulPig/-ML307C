#ifndef __CUSTOM_PROTOCOL_H__
#define __CUSTOM_PROTOCOL_H__

#define	PRO_HEAD			0x7B				// 帧头
#define	PRO_TAIL			0x7D				// 帧尾

#define	PRO_BUFFER_LENGTH	1024				// 帧缓冲长度

enum
{
	PRO_S_HEAD = 0,			// 帧头
	PRO_S_COMMAND,			// 命令
	PRO_S_SERIAL,			// 通信序列号
	PRO_S_MSGLEN1,			// 消息长度-高字节
	PRO_S_MSGLEN2,			// 消息长度-低字节
	PRO_S_MSGDATA,			// 消息内容
	PRO_S_CHECK,			// 帧校验
	PRO_S_TAIL,				// 帧尾
};

///////////////////////////// 命令定义 ///////////////////////////////////

// 登录包上报
#define	PRO_CMD03_LOGIN_REQ				0x03		// 查询命令(云端->LTE->BMS)
#define	PRO_CMD83_LOGIN_ACK				0x83		// 登录或查询上报(BMS->LTE->云端)
#define	PRO_CMD93_LOGIN_ACK				0x93		// 登录或查询上报2.0(BMS->LTE->云端)

// 电池信息上报
#define	PRO_CMD04_REPORT_REQ			0x04		// 主动查询(云端->LTE->BMS)
#define	PRO_CMD84_REPORT_ACK			0x84		// 主动上报(BMS->LTE->云端)
#define	PRO_CMD94_REPORT_ACK			0x94		// 主动上报2.0(BMS->LTE->云端)

// 激活命令
#define	PRO_CMD05_ACTIVE_REQ			0x05		// 云端->LTE->BMS
#define	PRO_CMD85_ACTIVE_ACK			0x85		// BMS->LTE->云端

// 保护参数设置或读取
#define	PRO_CMD06_PROTECT_PARA_REQ		0x06		// 云端->LTE->BMS
#define	PRO_CMD86_PROTECT_PARA_ACK		0x86		// BMS->LTE->云端

// 特殊参数设置或读取
#define	PRO_CMD07_SPECIAL_PARA_REQ		0x07		// 云端->LTE->BMS
#define	PRO_CMD87_SPECIAL_PARA_ACK		0x87		// BMS->LTE->云端

// BMS查询通讯模块基本信息
#define	PRO_CMD08_MODULE_BASE_ACK		0x08		// LTE->BMS
#define	PRO_CMD88_MODULE_BASE_REQ		0x88		// BMS->LTE

// BMS查询通讯模块状态及定位信息
#define	PRO_CMD09_MODULE_STATE_ACK		0x09		// LTE->BMS
#define	PRO_CMD89_MODULE_STATE_REQ		0x89		// BMS->LTE

// BMS OTA
#define	PRO_CMD0A_BMS_OTA_REQ			0x0A		// LTE->BMS
#define	PRO_CMD8A_BMS_OTA_ACK			0x8A		// BMS->LTE

// 加热指令
#define	PRO_CMD0B_HEAT_REQ				0x0B		// 云端->LTE->BMS
#define	PRO_CMD8B_HEAT_ACK				0x8B		// BMS->LTE->云端

// BMS上报调试/故障信息
#define	PRO_CMD0C_FAULT_ACK				0x0C		// LTE->BMS
#define	PRO_CMD8C_FAULT_REPORT			0x8C		// BMS->LTE

// BMS数据透传
#define	PRO_CMD11_DATA_TRANSFER_DOWN	0x11		// 云端->LTE->BMS
#define	PRO_CMDA1_DATA_TRANSFER_UP		0xA1		// BMS->LTE->云端

// LTE特有指令
#define	PRO_CMD50_MODULE				0x50		// 云端<->LTE


#endif

