#ifndef __CUSTOM_FOAT_H__
#define __CUSTOM_FOAT_H__

#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cm_os.h"
#include "cm_sys.h"
#include "cm_mem.h"
#include "cm_http.h"
#include "cm_fota.h"
#include "cm_ota.h"
#include "cm_pm.h"
#include "cm_modem.h"

/*本FOTA采用：全系统差分升级、APP整包升级
  全系统差分升级/APP整包升级:两者逻辑相同，仅生成差分包方式不同，均通过HTTP GET方法下载升级包->写入升级包数据至指定位置->触发升级
  注意:
  在本示例中，采用APP整包升级时，模组升级成功后，会再次下载升级包并反复升级，为正常现象。  而全系统差分升级，升级一次成功后，
  会校验到升级包为旧升级包，而停止升级。
*/

#define LTE_DEFAULT_FOTA_URL "http://jxw.qmkd.cn/qdbattery/app/getModuleOtaFile/4" 	// 默认LTE升级链接

#define	LTE_FOTA_FILE_SAVE_DIR		"lte_fota_file.bin"									// 默认LTE保存路径
#define	BMS_FOTA_FILE_SAVE_DIR		"bms_fota_file.bin"									// 默认BMS保存路径
#define	BMS_FOTA_INFO_SAVE_DIR		"bms_info_file.txt"									// BMS信息

#define	FOTA_RESERVED_SPACE_SIZE	(10 * 1024)											// 预留空间
#define	FOTA_PARTIAL_PACKET_LEN		(10 * 1024)											// HTTP分包大小

enum
{
	FOTA_STATE_IDLE = 0,		// 空闲
	FOTA_STATE_UPGRADE,			// 升级
};

enum
{
	FOTA_MODULE_LTE = 0,		// 通讯模块
	FOTA_MODULE_BMS,			// BMS模块
	
	FOTA_MODULE_MAX,
};

enum
{
	FOTA_SAVE_NONE = 0,			// 无法保存
	FOTA_SAVE_MEM,				// 升级文件下载到内存
	FOTA_SAVE_FS,				// 升级文件下载到文件系统
};

// 是否立即升级
enum
{
	FOTA_UPDATE_NO = 0,
	FOTA_UPDATE_YES
};


typedef struct
{
	uint8_t		state;			// OTA 状态
	uint8_t		modules;		// OTA 模块
	uint8_t		save_mode;		// OTA 存储模式
	uint8_t		is_update;		// 是否马上升级
	char		*url;			// OTA URL
	char		*file;			// OTA 文件(fs)或数据(mem)
	uint32_t	filesize;		// OTA 文件大小
}custom_fota_t;

int custom_fota_init(void);
int custom_fota_start(uint8_t fota_module, char *fota_url, uint8_t save_mode, uint8_t is_update);
int custom_fota_finish(void);

#endif


