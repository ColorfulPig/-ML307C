#ifndef __CUSTOM_CLOUD_LTE_H__
#define __CUSTOM_CLOUD_LTE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"

enum
{
	CLOUD_LTE_FOTA = 0x01,						// 通讯模块FOTA升级 
	CLOUD_LTE_RESTART = 0x02,					// 通讯模块重启
	CLOUD_DOWNLOAD_BMS_FIRMWARE = 0x03,			// 下载BMS固件指令
	CLOUD_GET_BMS_LTE_INFO = 0x04,				// 查询下载BMS固件信息及4G模块信息 
	CLOUD_SET_BMS_OTA_TIME = 0x05,				// 设置BMS升级时间（没有实现）
	CLOUD_GNSS_REQUEST = 0x06,					// GPS request : 下一个字节内容为para =1 open GPS  para = 0 close GPS
	CLOUD_REPORT_INTERVAL_CTRL = 0x07,			// 监测上报周期设置与查询
												// Para = 1 设置监测数据上报周期 value—value_H=report period高字节value_L=report period低字节(x2秒）须大于等于5(10秒）小于65535 （2字节）
												// Para= 2 恢复出厂设置（1小时上报一次）
};

enum
{
	CLOUD_RESULT_SUCCESS = 0,
	CLOUD_RESULT_FAIL = 1
};

int custom_cloud_lte_init(void);
int custom_cloud_lte_OnInstruction(uint8_t tid, uint8_t *buf, uint16_t len);


#endif


