#ifndef __CUSTOM_PROFILE_H__
#define __CUSTOM_PROFILE_H__

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "cJSON.h"
#include "cm_mem.h"
#include "cm_fs.h"
#include "custom_track.h"

// 配置参数文件
#define	PROFILE_NAME					"profile.json"
#define	PROFILE_BUF_SIZE				4096

// 字段统一用小写
#define	CONFIG_ITEM_MAGICAL				"magical"
#define	CONFIG_DATA_MAGICAL				1759647490					// 用于区分不用的参数版本，自定义

#define	CONFIG_ITEM_DEVICE_ID			"device_id"					// IMEI
#define	CONFIG_DATA_DEVICE_ID			""

#define	CONFIG_ITEM_BMS_TIMING_TASK		"bms_ota_timing_upgrade"	// BMS定时升级任务
#define	CONFIG_DATA_BMS_TIMING_TASK		0

#define	CONFIG_ITEM_BMS_TIMING_TIME		"bms_ota_timing_hour"		// BMS定时时间
#define	CONFIG_DATA_BMS_TIMING_TIME		0

#define	CONFIG_ITEM_BT_MAC				"bluetooth_mac"
#define	CONFIG_DATA_BT_MAC				""

// 注意：参数改变可能要更新custom_object_create(),custom_profile_update(),custom_profile_load()这三个函数

int custom_profile_init(void);
int custom_profile_setNumber(char *item, int value);
int custom_profile_addNumber(char *item, int value);
int custom_profile_getNumber(char *item, int *value);
int custom_profile_setString(char *item, char *value);
int custom_profile_addString(char *item, char *value);
int custom_profile_getString(char *item, char *value);
int custom_profile_save(void);
int custom_profile_delect(void);


#endif

