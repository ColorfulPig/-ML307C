#ifndef __CUSTOM_BMS_OTA_H__
#define __CUSTOM_BMS_OTA_H__

#include "cm_os.h"
#include "cm_gpio.h"
#include "cm_iomux.h"


#define	BMS_OTA_FRAME_LENGTH			2048		// 帧打包长度

#define	BMS_OTA_DATA_LENGTH				256			// 数据长度


enum
{
	BMS_OTA_STATE_Idle = 0,
	BMS_OTA_STATE_Handshake,
	BMS_OTA_STATE_HandshakeAck,
	BMS_OTA_STATE_Data,
	BMS_OTA_STATE_DataAck,
	BMS_OTA_STATE_Finish,
	BMS_OTA_STATE_FinishAck,
};

enum
{
	BMS_OTA_HandshakeCmd = 0x01,		// 通讯模块发起握手包
	BMS_OTA_DataCmd = 0x02,				// 通讯模块发送数据包
	BMS_OTA_FinishCmd = 0x03,			// 通讯模块发送传输完成包

	BMS_OTA_HandshakeCmd_Ack = 0x81,	// BMS响应握手包
	BMS_OTA_DataCmd_Ack = 0x82, 		// BMS响应数据包 
	BMS_OTA_FinishCmd_Ack = 0x83,		// BMS响应传输完成包
};

typedef struct
{
	int		file_data_fd;				// bms固件文件句柄
	int		file_info_fd;				// bms固件信息句柄
	int		firmware_file_size;			// bms固件文件大小
	int		firmware_info_size;			// bms固件信息大小	
	uint32_t firmware_version_i;		// bms鍥轰欢鐗堟湰
	uint32_t frame_sn;					
	uint32_t file_read_len;				
	uint32_t frame_len;
	uint32_t upgrade_count;				
	uint32_t upgrade_retry;		
	uint8_t	 crc_error;	

	uint8_t *frame;						// bms打包长度
	uint8_t *data;						// bms数据长度
	char	 *info;						// bms固件信息
	char 	*firmware_version;			// bms固件版本

	uint8_t	upgrade_state;				// bms升级状态	
	uint8_t	timing_upgrade;				// bms定时升级标志
	uint8_t	timing_hour;				// bms定时升级时间		
}BMS_OTA;

extern BMS_OTA	bms_ota;

int custom_bms_ota_init(void);
int custom_bms_ota_sendHandshakeCmd(uint32_t firmware_size, char *firmware_version);
int custom_bms_ota_sendDataCmd(uint16_t frame_sn, uint8_t *buf, uint16_t len);
int custom_bms_ota_sendFinishCmd(void);
int custom_bms_ota_OnACK(uint8_t *buf, uint16_t len);
int custom_bms_ota_start(uint8_t save_mode, char *file, uint32_t file_size);
int custom_bms_ota_finish(int result);


#endif

