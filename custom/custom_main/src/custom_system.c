
#define	CUSTOM_GLOBAL
#include "custom_system.h"
#include "custom_track.h"
#include "custom_global.h"
#include "custom_datetime.h"
#include "custom_common.h"
#include "custom_profile.h"

#define CUSTOM_SYSTEM_ICCID_RETRY_TIMES		5
#define CUSTOM_SYSTEM_ICCID_RETRY_DELAY_MS	1000

static int custom_system_read_iccid_with_retry(char *iccid, int retry_times, int retry_delay_ms)
{
	int ret = -1;

	for(int i = 0; i < retry_times; i++)
	{
		memset(iccid, 0, ICCID_BUF_SIZE);
		ret = cm_sim_get_iccid(iccid);
		SYSTEM_printf("ICCID read attempt %d/%d, ret=%d, iccid=%s", i + 1, retry_times, ret, iccid);

		if((ret == 0) && (strlen(iccid) > 0))
		{
			return 0;
		}

		if(i < (retry_times - 1))
		{
			osDelay(retry_delay_ms);
		}
	}

	return ret;
}

void custom_system_info(void)
{
	// 获取模组SN
	memset(g_SN, 0, SN_BUF_SIZE);
	cm_sys_get_sn(g_SN); 
	
	// 获取模组IMEI
	memset(g_IMEI, 0, IMEI_BUF_SIZE);
	cm_sys_get_imei(g_IMEI); 

	// 获取IMSI
	memset(g_IMSI, 0, IMSI_BUF_SIZE);
	cm_sim_get_imsi(g_IMSI);

	// 获取ICCID
	custom_system_read_iccid_with_retry(g_ICCID, CUSTOM_SYSTEM_ICCID_RETRY_TIMES, CUSTOM_SYSTEM_ICCID_RETRY_DELAY_MS);

	// 获取SDK版本号
	memset(g_SDKVER, 0, SDKVER_BUF_SIZE);
	cm_sys_get_cm_ver(g_SDKVER, SDKVER_BUF_SIZE);

	// 获取APP版本号
	common_sprintf((uint8_t *)g_APPVER, "%d.%d.%d", APP_VERSION_HIGH, APP_VERSION_MID, APP_VERSION_LOW);
	g_ReadyOK = 1;
	
	// 获取文件系统大小、剩余空间
	cm_fs_getinfo(&fs_system_info);

	// 获取目前系统heap状态
	cm_mem_get_heap_stats(&heap_stats);

	/* 静态内存256K */
	
	SYSTEM_printf("SN: %s", g_SN);
	SYSTEM_printf("IMEI: %s", g_IMEI);
	SYSTEM_printf("IMSI: %s", g_IMSI);
	SYSTEM_printf("ICCID: %s", g_ICCID);
	SYSTEM_printf("SDKVER: %s", g_SDKVER);
	SYSTEM_printf("APPVER: %s", g_APPVER);
	SYSTEM_printf("file system total:%.2fKB,remain:%.2fKB", fs_system_info.total_size/1024.0, fs_system_info.free_size/1024.0);
	SYSTEM_printf("heap stats total:%.2fKB,remain:%.2fKB", heap_stats.total_size/1024.0, heap_stats.free/1024.0);

	// 参数更新
	char device_id[32]={0};
	if(custom_profile_getString(CONFIG_ITEM_DEVICE_ID, device_id)==0)
	{
		if(strlen(device_id)==0)
		{
			custom_profile_setString(CONFIG_ITEM_DEVICE_ID, g_IMEI);
			custom_profile_save();
		}
	}	
}

/* 切换打印log到usb，掉电不保存配置 */
void custom_system_virt_at_usb(void)
{
	char operation[64] = {0};
	sprintf(operation, "%s\r\n", "AT+MCFG=log2cat,1");
	uint8_t rsp[128] = {0};
	int32_t rsp_len = 0;

	if(cm_virt_at_send_sync((const uint8_t *)operation, rsp, &rsp_len, 10) == 0)
	{
		cm_log_printf(0, "rsp=%s rsp_len=%d\n", rsp, rsp_len);
	}
	else
	{
		cm_log_printf(0, "ret != 0\n");
	}
}

void custom_system_task(void *p)
{
	cm_tm_t dt;
		
	custom_system_info();

	// 随机数产生器
	srand((unsigned int)cm_rtc_get_current_time());
	//SYSTEM_printf("rand: %d", rand());
  
	while(1)
	{
		// 打印时间
		custom_get_now_datetime(&dt);
		SYSTEM_printf("%04d-%02d-%02d %02d:%02d:%02d", dt.tm_year, dt.tm_mon, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);

		// 防止启动时未检测出SIM卡
		if(strlen(g_ICCID) == 0)
		{
			int ret = custom_system_read_iccid_with_retry(g_ICCID, CUSTOM_SYSTEM_ICCID_RETRY_TIMES, CUSTOM_SYSTEM_ICCID_RETRY_DELAY_MS);
			SYSTEM_printf("ICCID refresh ret=%d, ICCID: %s", ret, g_ICCID);
		}
		
		osDelay(ONE_SECONED * 10);	// 10秒
	}
}

int custom_system_init(void)
{    
	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "system_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_system_task, 0, &app_task_attr);

	return 0;
}

