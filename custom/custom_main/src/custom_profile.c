
#include "custom_profile.h"

static char *profile_buf = NULL;
static cJSON *profile_cjson_root = NULL;

// 保存配置对象到配置文件（0：不释放对象，程序中参数更改时会使用）
int custom_object_to_profile(uint8_t delect)
{
	int ret = 0;

	if(profile_cjson_root == NULL)				
	{
		cJSON_printf("%s: error. profile_cjson_root=NULL.", __func__);
		return -1;
	}
	
	// 对象转文本
	char *out = cJSON_Print(profile_cjson_root);
	if(out == NULL)
	{
		cJSON_printf("%s:cJSON_Print() error.", __func__);
		return -2;
	}	

	// 删除对象
	if(delect != 0)										
	{
		cJSON_Delete(profile_cjson_root);
		profile_cjson_root = NULL;
	}

	// 写入文件
	int32_t fd = cm_fs_open(PROFILE_NAME, CM_FS_WB);					// 只写方式打开文件
	if(fd < 0)
	{
		ret = -3;
		cJSON_printf("%s:cm_fs_open() error,fd=%d",__func__, fd);
	}
	else
	{
		int32_t f_olen = strlen(out);
		int32_t f_wlen = cm_fs_write(fd, out, f_olen);					// 写入文件
		if(f_wlen != f_olen)											// 写入失败
		{			
			ret = -4;
			cJSON_printf("%s:cm_fs_write() error,f_wlen=%d,f_olen=%d",__func__, f_wlen, f_olen);
		}	
	    cm_fs_close(fd);
	}
	
	// 释放内存
	cm_free(out);
	
	return ret;
}

// 配置文件转对象
int custom_profile_to_object(void)
{
	// 获取文件大小
	int32_t filesize = cm_fs_filesize(PROFILE_NAME);					
	if(filesize > PROFILE_BUF_SIZE)										// 内存不足
	{
		cJSON_printf("%s: profile size is too big. filesize=%d.", __func__, filesize);
		return -1;
	}
	
	// 申请内存	
	profile_buf = cm_malloc(PROFILE_BUF_SIZE);
	if(profile_buf == NULL)
	{
		cJSON_printf("%s: cm_malloc() error.", __func__);
		return -2;
	}

	// 打开文件
    int32_t fd = cm_fs_open(PROFILE_NAME, CM_FS_RB);
	if(fd < 0)
	{
		cJSON_printf("%s: cm_fs_open(%s) error.fd=%d", __func__, PROFILE_NAME, fd);
		cm_free(profile_buf);
		return -3;
	}

	// 读取文件
    int32_t f_len = cm_fs_read(fd, profile_buf, filesize);				
	if(f_len != filesize)
	{
		cJSON_printf("%s: cm_fs_read() error. f_len=%d,filesize=%d", __func__, f_len, filesize);
		cm_free(profile_buf);
		return -4;
	}
	cJSON_printf("%s: read %s success. f_len=%d", __func__, PROFILE_NAME, f_len);
	//cJSON_printf("profile_buf=%s", profile_buf);

	// 关闭文件
    cm_fs_close(fd);													

	// 解析文件为对象
	profile_cjson_root = cJSON_Parse(profile_buf);

	// 释放内存
	cm_free(profile_buf);												
	
	if(profile_cjson_root == NULL)
	{
		cJSON_printf("%s: cJSON_Parse(%s) error.[%s]", __func__, PROFILE_NAME, cJSON_GetErrorPtr());
		return -5;
	}

	return 0;
}

// 创建配置对象(使用APP固定的默认参数)
int custom_object_create(void)								// 参数版本改变时，添加参数初始值。
{
	cJSON_printf("%s: use default config!!!", __func__);

	// 创建对象
	profile_cjson_root = cJSON_CreateObject();

	// 添加参数
	if(profile_cjson_root != NULL)
	{
		custom_profile_addNumber(CONFIG_ITEM_MAGICAL, CONFIG_DATA_MAGICAL);	
		custom_profile_addString(CONFIG_ITEM_DEVICE_ID, CONFIG_DATA_DEVICE_ID);
		custom_profile_addNumber(CONFIG_ITEM_BMS_TIMING_TASK, CONFIG_DATA_BMS_TIMING_TASK);
		custom_profile_addNumber(CONFIG_ITEM_BMS_TIMING_TIME, CONFIG_DATA_BMS_TIMING_TIME);
		custom_profile_addString(CONFIG_ITEM_BT_MAC, CONFIG_DATA_BT_MAC);
	}
	
	return 0;
}

// 配置文件更新（重要参数不变）
int custom_profile_update(void)								// 参数版本改变时，需要参数保持不变，在此处理
{
	cJSON_printf("%s: custom_profile_update()!!!", __func__);

	if(profile_cjson_root != NULL)										
	{
		////////// 提取重要参数 ////////
		char remain_device_id[32]={0};
		custom_profile_getString(CONFIG_ITEM_DEVICE_ID, remain_device_id);
		cJSON_printf("remain_device_id:%s", remain_device_id);

		int remain_bms_ota_timing_upgrade=0;
		custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TASK, &remain_bms_ota_timing_upgrade);
		cJSON_printf("remain_bms_ota_timing_upgrade:%d", remain_bms_ota_timing_upgrade);

		int remain_bms_ota_timing_hour=0;
		custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TIME, &remain_bms_ota_timing_hour);
		cJSON_printf("remain_bms_ota_timing_hour:%d", remain_bms_ota_timing_hour);
		
		////////////////////////////////////////////////////////////////

		cJSON_Delete(profile_cjson_root);			// 删除旧对象
		profile_cjson_root = NULL;
			
		custom_object_create(); 					// 新创配置对象

		////////// 恢复重要参数 ////////

		custom_profile_setString(CONFIG_ITEM_DEVICE_ID, remain_device_id);
		custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TASK, remain_bms_ota_timing_upgrade);
		custom_profile_setNumber(CONFIG_ITEM_BMS_TIMING_TIME, remain_bms_ota_timing_hour);

		////////////////////////////////////////////////////////////////
				
		custom_object_to_profile(0); 				// 保存配置对象到配置文件(不删除对象)
		
	}

	return 0;
}

// 加载配置(更新参数)
int custom_profile_load(void)
{
	int ret = 0;

	if(profile_cjson_root != NULL)					
	{
		cJSON_printf("======%s======", PROFILE_NAME);

		int magical;
		ret = custom_profile_getNumber(CONFIG_ITEM_MAGICAL, &magical);
		cJSON_printf("magical:%d", magical);

		// 检查魔幻数
		if((ret == 0) && (magical != CONFIG_DATA_MAGICAL))
		{
			cJSON_printf("magical diff:%d->%d", magical, CONFIG_DATA_MAGICAL);

			// 配置参数版本有更新
			custom_profile_update();
		}

		////////////////////////////////// 获取参数 ////////////////////////////////
		char device_id[32]={0};
		custom_profile_getString(CONFIG_ITEM_DEVICE_ID, device_id);
		cJSON_printf("device_id:%s", device_id);
		
		int bms_ota_timing_upgrade;
		custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TASK, &bms_ota_timing_upgrade);
		cJSON_printf("bms_ota_timing_upgrade:%d", bms_ota_timing_upgrade);
		
		int bms_ota_timing_hour;
		custom_profile_getNumber(CONFIG_ITEM_BMS_TIMING_TIME, &bms_ota_timing_hour);
		cJSON_printf("bms_ota_timing_hour:%d", bms_ota_timing_hour);
				
		char bluetooth_mac[32]={0};
		custom_profile_getString(CONFIG_ITEM_BT_MAC, bluetooth_mac);
		cJSON_printf("bluetooth_mac: %s", bluetooth_mac);
		
		cJSON_printf("========================");
	}

	return 0;
}

int custom_profile_init(void)
{
	int ret, retry;

	// 检测配置文件，并确保存在
	retry = 0;
	do
	{
		retry++;		
		if(cm_fs_exist(PROFILE_NAME) == false)	// 不存在，则创建。
		{
			cJSON_printf("%s: %s is not exist. create it!!! retry=%d", __func__, PROFILE_NAME, retry);
		
			custom_object_create(); 			// 创建配置对象
			
			custom_object_to_profile(1); 		// 保存配置对象到配置文件(删除对象)

			if(retry >= 3)
			{
				cJSON_printf("%s: error. retry=%d", __func__, retry);
				return -1;
			}
		}
		else
		{
			cJSON_printf("%s: %s is ready!", __func__, PROFILE_NAME);
			break;
		}
	}
	while(retry < 3);

	// 读取配置文件，并转换为JSON对象
	ret = custom_profile_to_object();			
	if(ret != 0)	
	{
		cJSON_printf("%s: custom_profile_to_object() error. ret=%d", __func__, ret);
		return -2;
	}

	// 装载配置对象中的参数
	retry = 0;
	do
	{
		retry++;	
		ret = custom_profile_load();
		if(ret != 0)						// 参数有变化
		{
			cJSON_printf("%s: custom_profile_load() error!!! ret=%d,retry=%d", __func__, ret, retry);
			
			if(retry >= 3)
			{
				cJSON_printf("%s: error. retry=%d", __func__, retry);
				return -3;
			}
		}
		else
		{
			break;
		}
	}
	while(retry < 3);

	return 0;
}

// 设置数值参数（item需存在）
int custom_profile_setNumber(char *item, int value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON_ReplaceItemInObject(profile_cjson_root, item, cJSON_CreateNumber(value));
	}
	return 0;
}

int custom_profile_addNumber(char *item, int value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON_AddNumberToObject(profile_cjson_root, item, value);
	}
	return 0;
}

int custom_profile_getNumber(char *item, int *value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON *object = cJSON_GetObjectItem(profile_cjson_root, item);
		if(object != NULL)
		{
			*value = object->valueint;
			return 0;
		}
	}
	
	return -1;
}

// 设置字符串参数（item需存在）
int custom_profile_setString(char *item, char *value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON_ReplaceItemInObject(profile_cjson_root, item, cJSON_CreateString(value));
	}
	return 0;
}

int custom_profile_addString(char *item, char *value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON_AddStringToObject(profile_cjson_root, item, value);
	}
	return 0;
}

int custom_profile_getString(char *item, char *value)
{
	if(profile_cjson_root != NULL)
	{
		cJSON *object = cJSON_GetObjectItem(profile_cjson_root, item);
		if(object != NULL)
		{
			strcpy(value, object->valuestring);
			return 0;
		}
		else
		{
			return -2;
		}
	}
	
	return -1;
}

// 修改对象的参数后保存到配置文件中
int custom_profile_save(void)
{
	custom_object_to_profile(0);		// 保存配置对象到配置文件(不删除对象)
	return 0;
}

// 删除配置文件（恢复出厂设置使用，配合重启）
int custom_profile_delect(void)
{
	cm_fs_delete(PROFILE_NAME);

	if(profile_cjson_root != NULL)
	{
		cJSON_Delete(profile_cjson_root);
		profile_cjson_root = NULL;
	}

	return 0;
}

// TEST
//custom_profile_addString("test7", "777777777777");
//custom_profile_addNumber("t158", 158);
//custom_profile_save();


