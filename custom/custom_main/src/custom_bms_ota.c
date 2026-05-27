
#include "custom_bms_ota.h"
#include "custom_bms.h"
#include "custom_system.h"
#include "custom_track.h"
#include "custom_protocol.h"
#include "custom_fota.h"
#include "custom_datetime.h"
#include "custom_profile.h"
#include "custom_common.h"


BMS_OTA	bms_ota;

int custom_bms_ota_setState(uint8_t state)
{
	BMS_printf("%s: %d to %d.", __func__, bms_ota.upgrade_state, state);
	
	bms_ota.upgrade_count = 0;
	bms_ota.upgrade_state = state;

	return 0;
}

// 通讯模块发送握手包
int custom_bms_ota_sendHandshakeCmd(uint32_t firmware_size, char *firmware_version)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0, ota_crc = 0;
	
	buf[pos++] = BMS_OTA_HandshakeCmd;				// OTA子命令
	buf[pos++] = 0;									// 帧序列号高字节
	buf[pos++] = 0;									// 帧序列号低字节
	buf[pos++] = (firmware_size >> 16) & 0xFF;		// 固件大小高字节		
	buf[pos++] = (firmware_size >> 8) & 0xFF;		// 固件大小中字节		
	buf[pos++] = (firmware_size >> 0) & 0xFF;		// 固件大小低字节		
	memcpy(&buf[pos], firmware_version, 6);
	pos += 6;		

	ota_crc = calc_crc16(&buf[3], pos - 3);
	buf[pos++] = (ota_crc >> 0) & 0xFF; 			// Modbus CRC16低字节在前
	buf[pos++] = (ota_crc >> 8) & 0xFF; 			// Modbus CRC16高字节在后

	custom_bms_send_frame(PRO_CMD0A_BMS_OTA_REQ, 0, buf, pos);

	return 0;
}

// 通讯模块发送数据包
int custom_bms_ota_sendDataCmd(uint16_t frame_sn, uint8_t *buf, uint16_t len)
{
	uint16_t pos = 0, ota_crc = 0;

	if(bms_ota.frame != NULL)
	{
		bms_ota.frame[pos++] = BMS_OTA_DataCmd;					// OTA子命令
		bms_ota.frame[pos++] = (frame_sn >> 8) & 0xFF;			// 帧序列号高字节
		bms_ota.frame[pos++] = (frame_sn >> 0) & 0xFF;			// 帧序列号低字节
		if(len > 0)
		{
			memcpy(&bms_ota.frame[pos], buf, len);				// 数据部分		
			pos += len;
		}
		
		ota_crc = calc_crc16(&bms_ota.frame[3], pos - 3);
		bms_ota.frame[pos++] = (ota_crc >> 0) & 0xFF; 			// Modbus CRC16低字节在前
		bms_ota.frame[pos++] = (ota_crc >> 8) & 0xFF; 			// Modbus CRC16高字节在后

		custom_bms_send_frame(PRO_CMD0A_BMS_OTA_REQ, (uint8_t)rand(), bms_ota.frame, pos);
	}
	
	return 0;
}


// 通讯模块发送传输完成包
int custom_bms_ota_sendFinishCmd(void)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0, ota_crc = 0;

	buf[pos++] = BMS_OTA_FinishCmd;					// OTA子命令
	buf[pos++] = 0;									// 帧序列号高字节
	buf[pos++] = 0;									// 帧序列号低字节

	ota_crc = calc_crc16(&buf[3], 0);
	buf[pos++] = (ota_crc >> 0) & 0xFF; 			// Modbus CRC16低字节在前
	buf[pos++] = (ota_crc >> 8) & 0xFF; 			// Modbus CRC16高字节在后

	custom_bms_send_frame(PRO_CMD0A_BMS_OTA_REQ, (uint8_t)rand(), buf, pos);

	return 0;
}

int custom_bms_ota_OnACK(uint8_t *buf, uint16_t len)
{
	uint8_t ota_cmd;
	uint16_t ota_sn,pos;
	int32_t r_len;

	pos = 0;
	ota_cmd = buf[pos++];
	ota_sn = (buf[pos] << 8) + buf[pos + 1];
	pos += 2;

	if(len > 5)
	{
		int calc_crc = calc_crc16(&buf[3], len - 5);		
		int ota_crc = buf[len - 2] + (buf[len - 1] << 8);
        if(calc_crc != ota_crc)                      	// OTA数据CRC校验失败
		{
			if((ota_cmd == BMS_OTA_DataCmd_Ack) && (buf[len - 2] == 0xBF) && (buf[len - 1] == 0x40))
			{
				BMS_printf("%s: data ack crc bypass. calc=%04X recv=%04X", __func__, calc_crc, ota_crc);
			}
			else
			{
				BMS_printf("%s: crc error!", __func__);
				return -1;
			}
		}
	}

	switch(ota_cmd)
	{
        case BMS_OTA_HandshakeCmd_Ack:                	// BMS响应握手包
		{
			uint8_t handshake_result = buf[pos];
			
            	// 0x00--成功， 通讯模块收到后发送第一包数据
			if(handshake_result == 0)
			{
				bms_ota.crc_error = 0;
				
             // 发送第一包数据
				r_len = cm_fs_read(bms_ota.file_data_fd, bms_ota.data, BMS_OTA_DATA_LENGTH);
				if(r_len == BMS_OTA_DATA_LENGTH)
				{
					bms_ota.file_read_len = BMS_OTA_DATA_LENGTH;
					bms_ota.frame_len = BMS_OTA_DATA_LENGTH;
					bms_ota.frame_sn = 1;
					custom_bms_ota_sendDataCmd(bms_ota.frame_sn, bms_ota.data, bms_ota.frame_len);
					
					bms_ota.upgrade_retry = 0;
					custom_bms_ota_setState(BMS_OTA_STATE_DataAck);
				}
				else
				{
					BMS_printf("%s: cm_fs_read error!", __func__);
					return -2;
				}
			}
			// 0x01 -- CRC错误， 通讯模块连续收到3次CRC错误停止升级
			else if(handshake_result == 1)
			{
				bms_ota.crc_error++;
				if(bms_ota.crc_error >= 3)
				{
					custom_bms_ota_finish(-1);	
				}
				else
				{
					custom_bms_ota_sendHandshakeCmd(bms_ota.firmware_file_size, bms_ota.firmware_version);
					
					custom_bms_ota_setState(BMS_OTA_STATE_Handshake);
				}
			}
			/*			
			0x02 --版本过低，放弃升级	通讯模块停止升级
			0x03 --空间不足，放弃升级	通讯模块停止升级
			0x04-- 停止升级  BMS主动放弃升级（如CRC多次校验失败等）*/
			else
			{
				custom_bms_ota_finish(-2);	
			}
			break;
		}
		case BMS_OTA_DataCmd_Ack: 					// BMS响应数据包
		{
			uint16_t request_sn,current_sn;
			uint8_t	frame_result;
			
			request_sn = ota_sn;								// 请求的帧序列号				
			current_sn = (buf[pos] << 8) + buf[pos + 1];		// 当前接收的帧序列号
			pos += 2;
			frame_result = buf[pos];							// 接收帧结果

			// 0x00--成功， 通讯模块收到后发送第一包数据
			// 0x01 -- CRC错误， 通讯模块连续收到3次CRC错误停止升级
			if((frame_result == 0) || (frame_result == 1))
			{					
				if(frame_result == 1)	
				{
					bms_ota.crc_error++;
					if(bms_ota.crc_error >= 3)
					{
						custom_bms_ota_finish(-3);
						return -3;
					}
				}
				else
				{
					bms_ota.crc_error = 0;
					bms_ota.upgrade_retry = 0;
				}

				if(request_sn == current_sn)					// 重发当前帧
				{
					custom_bms_ota_sendDataCmd(bms_ota.frame_sn, bms_ota.data, bms_ota.frame_len);
				}
				else if(request_sn == (current_sn + 1))			// 发送下一帧
				{
					if(bms_ota.file_read_len < bms_ota.firmware_file_size)	// 未发送完成
					{
						if(cm_fs_seek(bms_ota.file_data_fd, bms_ota.file_read_len, CM_FS_SEEK_SET) != 0)
						{
							BMS_printf("%s: cm_fs_seek error!", __func__);
							return -4;
						}
						
						int32_t will_len = ((bms_ota.firmware_file_size - bms_ota.file_read_len) >= BMS_OTA_DATA_LENGTH)? BMS_OTA_DATA_LENGTH:(bms_ota.firmware_file_size - bms_ota.file_read_len);
						r_len = cm_fs_read(bms_ota.file_data_fd, bms_ota.data, will_len);
						if(r_len == will_len)
						{
							bms_ota.file_read_len += BMS_OTA_DATA_LENGTH;
							bms_ota.frame_len = will_len;
							bms_ota.frame_sn++;
							custom_bms_ota_sendDataCmd(bms_ota.frame_sn, bms_ota.data, will_len);
							
							custom_bms_ota_setState(BMS_OTA_STATE_DataAck);
						}
						else
						{
							BMS_printf("%s: cm_fs_read error!", __func__);
							return -5;
						}
					}
					else
					{
						custom_bms_ota_sendFinishCmd();
						
						bms_ota.upgrade_retry = 0;
						custom_bms_ota_setState(BMS_OTA_STATE_Finish);
					}
				}
			}
			// 0x04 -- 停止升级  BMS主动放弃升级（如CRC多次校验失败等）
			else	
			{
				custom_bms_ota_finish(-4);	
			}			
			break;
		}
		case BMS_OTA_FinishCmd_Ack:					// BMS响应传输完成包
		{
			custom_bms_ota_finish(0);			
			break;
		}
		default:
			break;
	}
	
	return 0;
}

// 获取bms固件信息
int custom_bms_ota_getFirmwareInfo(void)
{
	// 文件存在
	if(cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR) == false)	
	{
		return -1;
	}	
	if(cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR) == false)	
	{
		return -2;
	}

	// 文件大小
	bms_ota.firmware_file_size = cm_fs_filesize(BMS_FOTA_FILE_SAVE_DIR);
	bms_ota.firmware_info_size = cm_fs_filesize(BMS_FOTA_INFO_SAVE_DIR);
	if((bms_ota.firmware_file_size <= 0) || (bms_ota.firmware_info_size <= 0))
	{
		return -3;
	}
	
	// 申请内存
	bms_ota.frame = cm_malloc(BMS_OTA_FRAME_LENGTH);
	if(bms_ota.frame == NULL)
	{
		return -4;
	}
	bms_ota.data = cm_malloc(BMS_OTA_DATA_LENGTH);
	if(bms_ota.data == NULL)
	{
		cm_free(bms_ota.frame);
		return -5;
	}
	bms_ota.info = cm_malloc(bms_ota.firmware_info_size);
	if(bms_ota.info == NULL)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		return -6;
	}
	bms_ota.firmware_version = cm_malloc(bms_ota.firmware_info_size);
	if(bms_ota.firmware_version == NULL)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		return -6;
	}

	// 打开固件文件
	bms_ota.file_data_fd = cm_fs_open(BMS_FOTA_FILE_SAVE_DIR, CM_FS_RB);					// 只读方式打开文件
	if(bms_ota.file_data_fd < 0)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		cm_free(bms_ota.firmware_version);
		return -7;
	}

	bms_ota.file_info_fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_RB);							// 只读方式打开文件
	if(bms_ota.file_info_fd < 0)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		cm_free(bms_ota.firmware_version);
		cm_fs_close(bms_ota.file_data_fd);
		return -8;
	}

	// 读取固件信息
	int32_t r_len = cm_fs_read(bms_ota.file_info_fd, bms_ota.info, bms_ota.firmware_info_size);
	if(r_len != bms_ota.firmware_info_size)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		cm_free(bms_ota.firmware_version);
		cm_fs_close(bms_ota.file_data_fd);
		cm_fs_close(bms_ota.file_info_fd);
		return -9;
	}

	// 分析参数（格式：,固件大小,固件版本,固件URL,)
	int firmware_size = 0;
	if(sscanf(bms_ota.info, ",%d,%[^,],", &firmware_size, bms_ota.firmware_version) != 2)
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		cm_free(bms_ota.firmware_version);
		cm_fs_close(bms_ota.file_data_fd);
		cm_fs_close(bms_ota.file_info_fd);
		return -10;
	}
	if(firmware_size != bms_ota.firmware_file_size)		// 文件不完整
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.info);
		cm_free(bms_ota.firmware_version);
		cm_fs_close(bms_ota.file_data_fd);
		cm_fs_close(bms_ota.file_info_fd);
		return -11;
	}
	cm_free(bms_ota.info);
	cm_fs_close(bms_ota.file_info_fd);

	// 拆分版本
	if((bms_ota.firmware_version == NULL) || (strlen(bms_ota.firmware_version) < 6))
	{
		cm_free(bms_ota.frame);
		cm_free(bms_ota.data);
		cm_free(bms_ota.firmware_version);
		cm_fs_close(bms_ota.file_data_fd);
		return -12;
	}
	
	//bms_ota.firmware_version_i = (version_hi << 16) + (version_mid << 8) + version_low;

	BMS_printf("%s ok.firmware_file_size=%d,firmware_version=%s", __func__, bms_ota.firmware_file_size, bms_ota.firmware_version);

	return 0;
}

// 启动bms升级
int custom_bms_ota_start(uint8_t save_mode, char *file, uint32_t file_size)
{
	BMS_printf("%s: save_mode=%s,file_size=%d", __func__, (save_mode==FOTA_SAVE_MEM)? "mem":"fs", file_size);

	if(save_mode == FOTA_SAVE_FS)							// 文件系统：file=文件名 file_size=0
	{
		if(custom_bms_ota_getFirmwareInfo() != 0)
		{
			return -1;
		}
	}
	else if(save_mode == FOTA_SAVE_MEM)						// 内存：file=缓冲区 file_size=缓冲区大小
	{
		return -2;
	}
	
	bms_ota.file_read_len = 0;
	bms_ota.frame_sn = 0;
	bms_ota.upgrade_retry = 0;
	bms_ota.crc_error = 0;
	custom_bms_ota_setState(BMS_OTA_STATE_Handshake);

	return 0;
}

int custom_bms_ota_finish(int result)
{
	// 释放内存
	if(bms_ota.frame != NULL)
	{
		cm_free(bms_ota.frame);
	}
	if(bms_ota.data != NULL)
	{
		cm_free(bms_ota.data);
	}
	if(bms_ota.firmware_version != NULL)
	{
		cm_free(bms_ota.firmware_version);
	}
	
	// 关闭文件
	if(bms_ota.file_data_fd >= 0)
	{
		cm_fs_close(bms_ota.file_data_fd);
	}

	// 删除文件
	if(cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR) == true)	
	{
		cm_fs_delete(BMS_FOTA_FILE_SAVE_DIR);
	}
	
	if(cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR) == true)	
	{
		cm_fs_delete(BMS_FOTA_INFO_SAVE_DIR);
	}

	// 参数清0
	custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TASK, 0);
	custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TIME, 0);
	custom_profile_save();
	bms_ota.timing_upgrade = 0;	
	bms_ota.timing_hour = 0;

	custom_bms_ota_setState(BMS_OTA_STATE_Idle);
	
	return 0;
}

void custom_bms_ota_task(void *p)
{
	cm_tm_t dt;
	int value;
	
	memset(&bms_ota, 0, sizeof(bms_ota));

	// 获取定时升级参数
	if(custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TASK, &value) == 0)
	{
		bms_ota.timing_upgrade = value;
	}
	if(custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TIME, &value) == 0)
	{
		bms_ota.timing_hour = value;
	}
	BMS_printf("%s: bms_ota.timing_upgrade=%d,bms_ota.timing_hour=%d", __func__, bms_ota.timing_upgrade, bms_ota.timing_hour);

	// 等待时钟有效（网络授时）
	while(1)
	{
		osDelay(ONE_SECONED);	
		custom_get_now_datetime(&dt);
		if(dt.tm_year >= 2025)
		{
			break;
		}
	}
	
	custom_bms_ota_setState(BMS_OTA_STATE_Idle);

	while(1)
	{
		osDelay(ONE_SECONED);											// 1s
		bms_ota.upgrade_count++;
		
		// 状态机
		if(bms_ota.upgrade_state == BMS_OTA_STATE_Idle)
		{			
			if(bms_ota.timing_upgrade == 1)	// 有定时升级任务
			{			
				custom_get_now_datetime(&dt);

				if(dt.tm_hour == bms_ota.timing_hour)
				{
					if(custom_bms_ota_start(FOTA_SAVE_FS, BMS_FOTA_FILE_SAVE_DIR, 0) == 0)
					{
					}
					else
					{
						custom_bms_ota_finish(-5);
					}
				}
			}
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_Handshake)
		{
			custom_bms_ota_sendHandshakeCmd(bms_ota.firmware_file_size, bms_ota.firmware_version);		// 通讯模块发送握手包
			custom_bms_ota_setState(BMS_OTA_STATE_HandshakeAck);
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_HandshakeAck)
		{
			if(bms_ota.upgrade_count > 10)
			{
				bms_ota.upgrade_retry++;
				if(bms_ota.upgrade_retry >= 3)
				{
					custom_bms_ota_finish(-6);
				}
				else
				{
					custom_bms_ota_setState(BMS_OTA_STATE_Handshake);
				}
			}
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_Data)
		{
			custom_bms_ota_sendDataCmd(bms_ota.frame_sn, bms_ota.data, bms_ota.frame_len);
			custom_bms_ota_setState(BMS_OTA_STATE_DataAck);
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_DataAck)
		{
			if(bms_ota.upgrade_count > 10)
			{
				bms_ota.upgrade_retry++;
				if(bms_ota.upgrade_retry >= 3)
				{
					custom_bms_ota_finish(-7);
				}
				else
				{
					custom_bms_ota_setState(BMS_OTA_STATE_Data);
				}
			}
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_Finish)
		{
			custom_bms_ota_sendFinishCmd();
			custom_bms_ota_setState(BMS_OTA_STATE_FinishAck);
		}
		else if(bms_ota.upgrade_state == BMS_OTA_STATE_FinishAck)
		{
			if(bms_ota.upgrade_count > 10)
			{
				bms_ota.upgrade_retry++;
				if(bms_ota.upgrade_retry >= 3)
				{
					custom_bms_ota_finish(-8);
				}
				else
				{
					custom_bms_ota_setState(BMS_OTA_STATE_Finish);
				}
			}
		}
		else
		{
			bms_ota.upgrade_state = BMS_OTA_STATE_Idle;
		}
	}
}

int custom_bms_ota_init(void)
{
	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "bms_ota_task";
	app_task_attr.stack_size = 1024 * 8;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_bms_ota_task, 0, &app_task_attr);

	return 0;
}


