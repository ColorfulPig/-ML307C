
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

// 把https地址转成http
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

// 发送下载BMS固件及4G模块信息  (逗号前后分割，如：,BMS大小,BMS版本,4G模块imei,4G模块iccid,4G模块btmac,4G模块core版本,4G模块app版本,)
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

// LTE特有指令
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
    char lte_fota_url[256] = {0};
    char param[256] = {0};
    uint16_t param_len = 0;

    if(len > pos)
    {
        param_len = len - pos;
        if(param_len >= sizeof(param))
        {
            param_len = sizeof(param) - 1;
        }

        memcpy(param, &buf[pos], param_len);

        // 平台下发格式：,http://xxx/ota.bin,
        if(sscanf(param, ",%255[^,],", lte_fota_url) == 1)
        {
            Cloud_printf("CLOUD_LTE_FOTA: url=%s", lte_fota_url);

            ret = custom_fota_start(FOTA_MODULE_LTE, lte_fota_url, FOTA_SAVE_MEM, FOTA_UPDATE_YES);
        }
    }

    custom_cloud_lte_sendResult(tid, lte_cmd, (ret == 0) ? CLOUD_RESULT_SUCCESS : CLOUD_RESULT_FAIL);
    break;
		}
		case CLOUD_LTE_RESTART:						// 通讯模块重启
		{
			// 应答
			custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			
			osDelay(ONE_SECONED * 3);			
			cm_pm_reboot();
			break;
		}
		case CLOUD_DOWNLOAD_BMS_FIRMWARE:			// 下载BMS固件指令
		{
			uint32_t bms_firmware_size = 0;
			char bms_firmware_version[64] = {0}, bms_firmware_url[256] = {0}, bms_download_url[256] = {0};	// 注意溢出
			
			// URL前后通过逗号分割（格式：,固件大小,固件版本,固件URL,)
			if(sscanf((char *)&buf[pos], ",%ld,%[^,],%[^,],", &bms_firmware_size, bms_firmware_version, bms_firmware_url) == 3)
			{
				Cloud_printf("CLOUD_DOWNLOAD_BMS_FIRMWARE: bms_firmware_size=%d, bms_firmware_version=%s, bms_firmware_url=%s", 
					bms_firmware_size, bms_firmware_version, bms_firmware_url);

				custom_cloud_lte_convert_download_url(bms_firmware_url, bms_download_url, sizeof(bms_download_url));//把 https:// 转成 http://

				// 先写入bms版本信息
				if(cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR) == true)								// 存在，则删除。
				{
					cm_fs_delete(BMS_FOTA_INFO_SAVE_DIR);
				}
				
				int32_t fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_WB);					// 只写方式打开文件
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
				
				// 启动BMS升级
				ret = custom_fota_start(FOTA_MODULE_BMS, bms_download_url, FOTA_SAVE_FS, FOTA_UPDATE_YES);		
			}

			// 应答
			if(ret == 0)							// 执行成功
			{
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			}
			else
			{
				custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_FAIL);
			}
			break;
		}
		case CLOUD_GET_BMS_LTE_INFO:				// 查询下载BMS固件信息及4G模块信息 
		{			
			uint32_t bms_firmware_size = 0;
			char bms_firmware_version[64] = {0}, bms_firmware_url[256] = {0};	// 注意溢出
			
			if((cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR)==true) && (cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR)==true))		// 存在
			{
				int bms_file_size = cm_fs_filesize(BMS_FOTA_FILE_SAVE_DIR);
				int bms_info_size = cm_fs_filesize(BMS_FOTA_INFO_SAVE_DIR);

				if((bms_file_size > 0) && (bms_info_size > 0))
				{
					char *bms_info_data = cm_malloc(bms_info_size);
					if(bms_info_data != NULL)
					{
						int32_t fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_RB);					// 只读方式打开文件
						if(fd >= 0)
						{
							int32_t r_len = cm_fs_read(fd, bms_info_data, bms_info_size);
							if(r_len == bms_info_size)
							{
								// URL前后通过逗号分割（格式：,固件大小,固件版本,固件URL,)
								if(sscanf(bms_info_data, ",%ld,%[^,],%[^,],", &bms_firmware_size, bms_firmware_version, bms_firmware_url) == 3)
								{
									Cloud_printf("CLOUD_GET_BMS_LTE_INFO: bms_firmware_size=%d, bms_firmware_version=%s, bms_firmware_url=%s", 
										bms_firmware_size, bms_firmware_version, bms_firmware_url);

									// 使用真实文件大小
									bms_firmware_size = bms_file_size;
								}
							}
							cm_fs_close(fd);							
						}
						cm_free(bms_info_data);
					}
				}
			}
			
			// 应答
			custom_cloud_lte_sendBmsLteInfo(0, bms_firmware_size, bms_firmware_version, g_IMEI, g_ICCID, bluetooth_MAC, g_SDKVER, g_APPVER);
			break;
		}
		case CLOUD_SET_BMS_OTA_TIME:				// 设置BMS升级时间（没有实现）
		{
			uint8_t ota_hour_time;

			ota_hour_time = buf[pos];						// 升级时间,24小时制的整点时刻

			if(cm_fs_exist(BMS_FOTA_FILE_SAVE_DIR) == true)	// 固件存在
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
		case CLOUD_GNSS_REQUEST:					// GPS request
		{
			uint8_t ctrl;
		
			ctrl = buf[pos];
			if(ctrl == 1)							// para =1 open GPS  para = 0 close GPS
			{
				Cloud_printf("GPS request: open");
				custom_gnss_enable(1);
			}
			else
			{
				Cloud_printf("GPS request: close");
				custom_gnss_enable(0);			
			}
			
			// 应答--xxx
			custom_cloud_lte_sendResult(tid, lte_cmd, CLOUD_RESULT_SUCCESS);
			break;
		}
		case CLOUD_REPORT_INTERVAL_CTRL:			// 监测上报周期设置与查询
		{			
			// --xxx
			// Para = 1 设置监测数据上报周期 
			//			value_H=report period高字节
			//			value_L=report period低字节(x2秒）须大于等于5(10秒）小于65535 （2字节）
			// Para= 2 恢复出厂设置（1小时上报一次）
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
		
		osDelay(ONE_SECONED);	// 1秒
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

