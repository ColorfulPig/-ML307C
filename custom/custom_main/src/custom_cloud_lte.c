#include "custom_cloud_lte.h"
#include "custom_cloud.h"
#include "custom_system.h"
#include "custom_track.h"
#include "custom_common.h"
#include "custom_protocol.h"
#include "custom_bluetooth.h"
#include "custom_global.h"
#include "custom_bms.h"
#include "custom_bms_ota.h"
#include "custom_gnss.h"
#include "custom_fota.h"
#include "custom_profile.h"

/* 解析 LTE FOTA 参数:
 * ,软件大小,软件版本,硬件版本,固件URL,厂家信息
 */
static int custom_cloud_lte_parse_lte_fota_param(const char *param,
	uint32_t *software_size,
	char *software_version,
	char *hardware_version,
	char *firmware_url,
	char *manufacturer)
{
	if((param == NULL) || (software_size == NULL) || (software_version == NULL) ||
		(hardware_version == NULL) || (firmware_url == NULL) || (manufacturer == NULL))
	{
		return -1;
	}

	if(sscanf(param, ",%ld,%63[^,],%63[^,],%255[^,],%63[^,]",
		software_size, software_version, hardware_version, firmware_url, manufacturer) != 5)
	{
		return -2;
	}

	if(*software_size == 0)
	{
		return -3;
	}

	if((strlen(software_version) == 0) || (strlen(hardware_version) == 0) ||
		(strlen(firmware_url) == 0) || (strlen(manufacturer) == 0))
	{
		return -4;
	}

	if(strcmp(manufacturer, "ZY") != 0)
	{
		return -5;
	}

	return 0;
}

// 将 https 地址转成 http
static void custom_cloud_lte_convert_download_url(const char *src_url, char *dst_url, uint32_t dst_size)
{
	if((src_url == NULL) || (dst_url == NULL) || (dst_size == 0))
	{
		return;
	}

	memset(dst_url, 0, dst_size);

	if(strncmp(src_url, "https://", 8) == 0)
	{
		snprintf(dst_url, dst_size, "http://%s", src_url + 8);
	}
	else
	{
		snprintf(dst_url, dst_size, "%s", src_url);
	}
}

// 发送执行结果
int custom_cloud_lte_sendResult(uint8_t tid, uint8_t cmd, uint8_t result)
{
	uint8_t buf[2];
	
	buf[0] = cmd;
	buf[1] = result;
	custom_cloud_sendFrame(PRO_CMD50_MODULE, tid, buf, 2);
	
	return 0;
}

// 发送 BMS 固件信息和 4G 模块信息
int custom_cloud_lte_sendBmsLteInfo(uint8_t tid,
	uint32_t bms_size, char *bms_version, char *imei, char *iccid, char *btmac, char *lte_sdk_version, char *lte_app_version)
{
	uint8_t buf[256]={0};
	uint16_t pos = 0, len = 0;
	
	buf[pos++] = CLOUD_GET_BMS_LTE_INFO;

	len = common_sprintf(&buf[pos], ",%ld,%s,%s,%s,%s,%s,%s,", bms_size, bms_version, imei, iccid, btmac, lte_sdk_version, lte_app_version);
	pos += len;
	
	Cloud_printRaw("custom_cloud_lte_sendBmsLteInfo:", buf, pos);
	custom_cloud_sendFrame(PRO_CMD50_MODULE, tid, buf, pos);
	
	return 0;
}

// LTE 特有指令处理
int custom_cloud_lte_OnInstruction(uint8_t tid, uint8_t *buf, uint16_t len)
{
	uint8_t lte_cmd;
	uint16_t pos;
	int ret = -1;
	
	pos = 0;
	lte_cmd = buf[pos++];

	switch(lte_cmd)
	{
		case CLOUD_LTE_FOTA:
		{
			uint32_t lte_software_size = 0;
			char lte_software_version[64] = {0};
			char lte_hardware_version[64] = {0};
			char lte_fota_url[256] = {0};
			char lte_download_url[256] = {0};
			char lte_manufacturer[64] = {0};
			char param[512] = {0};
			uint16_t param_len = 0;

			if(len > pos)
			{
				param_len = len - pos;
				if(param_len >= sizeof(param))
				{
					param_len = sizeof(param) - 1;
				}

				memcpy(param, &buf[pos], param_len);

				if(custom_cloud_lte_parse_lte_fota_param(param,
					&lte_software_size,
					lte_software_version,
					lte_hardware_version,
					lte_fota_url,
					lte_manufacturer) == 0)
				{
					custom_cloud_lte_convert_download_url(lte_fota_url, lte_download_url, sizeof(lte_download_url));

					Cloud_printf("CLOUD_LTE_FOTA: size=%d, sw=%s, hw=%s, url=%s, manufacturer=%s",
						lte_software_size, lte_software_version, lte_hardware_version, lte_download_url, lte_manufacturer);

					ret = custom_fota_start(FOTA_MODULE_LTE, lte_download_url, FOTA_SAVE_MEM, FOTA_UPDATE_YES);
				}
			}

			custom_cloud_lte_sendResult(tid, lte_cmd, (ret == 0) ? CLOUD_RESULT_SUCCESS : CLOUD_RESULT_FAIL);
			break;
		}
		case CLOUD_LTE_RESTART:
		{
			custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			osDelay(ONE_SECONED * 3);
			cm_pm_reboot();
			break;
		}
		case CLOUD_DOWNLOAD_BMS_FIRMWARE:
		{
			uint32_t bms_firmware_size = 0;
			char bms_firmware_version[64] = {0}, bms_firmware_url[256] = {0}, bms_download_url[256] = {0};
			
			if(sscanf((char *)&buf[pos], ",%ld,%[^,],%[^,],", &bms_firmware_size, bms_firmware_version, bms_firmware_url) == 3)
			{
				Cloud_printf("CLOUD_DOWNLOAD_BMS_FIRMWARE: bms_firmware_size=%d, bms_firmware_version=%s, bms_firmware_url=%s",
					bms_firmware_size, bms_firmware_version, bms_firmware_url);

				custom_cloud_lte_convert_download_url(bms_firmware_url, bms_download_url, sizeof(bms_download_url));

				if(cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR) == true)
				{
					cm_fs_delete(BMS_FOTA_INFO_SAVE_DIR);
				}
				
				int32_t fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_WB);
				if(fd >= 0)
				{
					char info_buf[384] = {0};
					int info_len = snprintf(info_buf, sizeof(info_buf), ",%ld,%s,%s,",
						bms_firmware_size, bms_firmware_version, bms_firmware_url);
					if(info_len > 0)
					{
						cm_fs_write(fd, info_buf, info_len);
					}
					cm_fs_close(fd);
				}
				
				ret = custom_fota_start(FOTA_MODULE_BMS, bms_download_url, FOTA_SAVE_FS, FOTA_UPDATE_YES);
			}

			if(ret == 0)
			{
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			}
			else
			{
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_FAIL);
			}
			break;
		}
		case CLOUD_GET_BMS_LTE_INFO:
		{
			uint32_t bms_firmware_size = 0;
			char bms_firmware_version[64] = {0}, bms_firmware_url[256] = {0};
			
			if((cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR)==true) && (cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR)==true))
			{
				int bms_file_size = cm_fs_filesize(BMS_FOTA_FILE_SAVE_DIR);
				int bms_info_size = cm_fs_filesize(BMS_FOTA_INFO_SAVE_DIR);

				if((bms_file_size > 0) && (bms_info_size > 0))
				{
					char *bms_info_data = cm_malloc(bms_info_size);
					if(bms_info_data != NULL)
					{
						int32_t fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_RB);
						if(fd >= 0)
						{
							int32_t r_len = cm_fs_read(fd, bms_info_data, bms_info_size);
							if(r_len == bms_info_size)
							{
								if(sscanf(bms_info_data, ",%ld,%[^,],%[^,],", &bms_firmware_size, bms_firmware_version, bms_firmware_url) == 3)
								{
									Cloud_printf("CLOUD_GET_BMS_LTE_INFO: bms_firmware_size=%d, bms_firmware_version=%s, bms_firmware_url=%s",
										bms_firmware_size, bms_firmware_version, bms_firmware_url);
									bms_firmware_size = bms_file_size;
								}
							}
							cm_fs_close(fd);
						}
						cm_free(bms_info_data);
					}
				}
			}
			
			custom_cloud_lte_sendBmsLteInfo(0, bms_firmware_size, bms_firmware_version, g_IMEI, g_ICCID, bluetooth_MAC, g_SDKVER, g_APPVER);
			break;
		}
		case CLOUD_SET_BMS_OTA_TIME:
		{
			uint8_t ota_hour_time;

			ota_hour_time = buf[pos];

			if(cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR) == true)
			{
				bms_ota.timing_hour = ota_hour_time;
				bms_ota.timing_upgrade = 1;
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			}
			else
			{
				bms_ota.timing_hour = 0;
				bms_ota.timing_upgrade = 0;
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_FAIL);
			}
			
			custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TASK, bms_ota.timing_upgrade);
			custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TIME, bms_ota.timing_hour);
			custom_profile_save();
			break;
		}
		case CLOUD_GNSS_REQUEST:
		{
			uint8_t ctrl;
		
			ctrl = buf[pos];
			if(ctrl == 1)
			{
				Cloud_printf("GPS request: open");
				custom_gnss_enable(1);
			}
			else
			{
				Cloud_printf("GPS request: close");
				custom_gnss_enable(0);
			}
			
			custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			break;
		}
		case CLOUD_REPORT_INTERVAL_CTRL:
		{
			break;
		}
		default:
		{
			custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_FAIL);
			break;
		}
	}
	
	return 0;
}

void custom_cloud_lte_task(void *p)
{
	while(1)
	{
		osDelay(ONE_SECONED);
	}
}

int custom_cloud_lte_init(void)
{
	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "cloud_lte_task";
	app_task_attr.stack_size = 1024 * 4;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_cloud_lte_task, 0, &app_task_attr);

	return 0;
}
