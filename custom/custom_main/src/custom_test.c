
#include "custom_test.h"
#include "custom_system.h"
#include "custom_track.h"
#include "custom_bluetooth.h"
#include "custom_sps.h"
#include "custom_fota.h"

static void custom_test_convert_download_url(const char *src_url, char *dst_url, uint32_t dst_size)
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

int custom_test_OnBlock(uint8_t *buf,uint32_t len)
{
	if(len < 5)	return -1;
	
	// 透传数据到蓝牙
	if(memcmp(buf, "BT:", 3) == 0)
	{
		custom_bluetooth_send(&buf[3], len - 3);
	}

	// 透传数据到BMS
	if(memcmp(buf, "BMS:", 4) == 0)
	{
//		custom_bms_send_login_req(0);		// // 登录包上报-查询命令(不需要LTE主动查询)
	}

	// 模拟接收到云端数据
	if(memcmp(buf, "CLOUD:", 6) == 0)
	{
		Sps_InCloudScan(&buf[6], len - 6);
	}

	// FOTA(格式：FOTA:LTE,URL=http://121.199.36.16/TUpdate/jsAutoUpdate-0.0.1-SNAPSHOT.jar)
	if((memcmp(buf, "FOTA:LTE,URL=", 13) == 0) && (len > 13))
	{
		char *lte_fota_url = cm_malloc(len);		
		memcpy(lte_fota_url, &buf[13], len-13);
		lte_fota_url[len-13] = 0;
		custom_fota_start(FOTA_MODULE_LTE, lte_fota_url, FOTA_SAVE_MEM, FOTA_UPDATE_YES);
	}
	if((memcmp(buf, "FOTA:BMS,URL=", 13) == 0) && (len > 13))
	{
		char *bms_fota_url = cm_malloc(len);
		char bms_download_url[256] = {0};
		memcpy(bms_fota_url, &buf[13], len-13);
		bms_fota_url[len-13] = 0;
		custom_test_convert_download_url(bms_fota_url, bms_download_url, sizeof(bms_download_url));
		custom_fota_start(FOTA_MODULE_BMS, bms_download_url, FOTA_SAVE_FS, FOTA_UPDATE_YES);
		cm_free(bms_fota_url);
	}
	// FOTA(格式：FOTA:BMS,INFO=59548,7.0.1,URL=http://xxx/ota.bin)
	if((memcmp(buf, "FOTA:BMS,INFO=", 14) == 0) && (len > 14))
	{
		uint32_t bms_firmware_size = 0;
		char bms_firmware_version[64] = {0};
		char bms_fota_url[256] = {0}, bms_download_url[256] = {0};
		char *param = cm_malloc(len - 13);

		if(param != NULL)
		{
			memset(param, 0, len - 13);
			memcpy(param, &buf[14], len - 14);

			if(sscanf(param, "%ld,%[^,],URL=%255s", &bms_firmware_size, bms_firmware_version, bms_fota_url) == 3)
			{
				custom_test_convert_download_url(bms_fota_url, bms_download_url, sizeof(bms_download_url));

				if(cm_fs_exist(BMS_FOTA_INFO_SAVE_DIR) == true)
				{
					cm_fs_delete(BMS_FOTA_INFO_SAVE_DIR);
				}

				int32_t fd = cm_fs_open(BMS_FOTA_INFO_SAVE_DIR, CM_FS_WB);
				if(fd >= 0)
				{
					char info_buf[384] = {0};
					int info_len = snprintf(info_buf, sizeof(info_buf), ",%ld,%s,%s,", bms_firmware_size, bms_firmware_version, bms_fota_url);
					if(info_len > 0)
					{
						cm_fs_write(fd, info_buf, info_len);
					}
					cm_fs_close(fd);
				}

				custom_fota_start(FOTA_MODULE_BMS, bms_download_url, FOTA_SAVE_FS, FOTA_UPDATE_YES);
			}

			cm_free(param);
		}
	}
	
	return 0;
}

void custom_test_task(void *p)
{

	while(1)
	{
		//TEST_printf("test_task:777777");
		
		osDelay(ONE_SECONED);	// 1秒
	}
}

int custom_test_init(void)
{    
	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "test_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_test_task, 0, &app_task_attr);

	return 0;
}

