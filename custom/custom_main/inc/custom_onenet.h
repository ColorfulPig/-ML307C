
#ifndef __CUSTOM_ONENET_H__
#define __CUSTOM_ONENET_H__

#include "cm_mqtt.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cJSON.h"
#include <wolfssl/wolfcrypt/coding.h>
#include <wolfssl/wolfcrypt/hmac.h>
#include "cm_pm.h"
#include "cm_modem.h"
#include "base64.h"

#define	ONENET_MQTT_SERVER_ADDR				"mqtts.heclouds.com"	// 服务器地址
#define	ONENET_MQTT_SERVER_PORT				1883

#define	ONENET_DEV_TOKEN_VERISON			"2018-10-31"			// 目前仅支持"2018-10-31"

#define	ONENET_DEV_TOKEN_METHOD_MD5			"md5"					// 签名方法
#define	ONENET_DEV_TOKEN_METHOD_SHA1		"sha1"
#define	ONENET_DEV_TOKEN_METHOD_SHA256		"sha256"

#define	ONENET_MESSAGE_PAYLOAD_LEN			1024


// 通信主题：https://iot.10086.cn/doc/aiot/fuse/detail/920
// 设备属性/事件：https://iot.10086.cn/doc/aiot/fuse/detail/902

////////////////////////////// OneNet MQTT主题标准格式定义 /////////////////////////////////////////

//【属性上报】上行
//static char *mqtt_dev_attr_report_request_topic  = "$sys/{pid}/{device-name}/thing/property/post";			// 请求Topic
//static char *mqtt_dev_attr_report_response_topic = "$sys/{pid}/{device-name}/thing/property/post/reply";		// 响应Topic
#define	ONENET_MQTT_ATTR_POST_TOPIC				"$sys/%s/%s/thing/property/post"								// 属【发布主题】
#define	ONENET_MQTT_ATTR_POST_REPLY_TOPIC		"$sys/%s/%s/thing/property/post/reply"							// 属【订阅主题】

//【属性设置】下行
//static char *mqtt_dev_attr_set_request_topic  = "$sys/{pid}/{device-name}/thing/property/set";				// 请求Topic
//static char *mqtt_dev_attr_set_response_topic = "$sys/{pid}/{device-name}/thing/property/set_reply";			// 响应Topic
#define	ONENET_MQTT_ATTR_SET_TOPIC				"$sys/%s/%s/thing/property/set"									// 属【订阅主题】
#define	ONENET_MQTT_ATTR_SET_REPLY_TOPIC		"$sys/%s/%s/thing/property/set_reply"							// 属【发布主题】

//【属性获取】下行
//static char *mqtt_dev_attr_get_request_topic  = "$sys/{pid}/{device-name}/thing/property/get";				// 请求Topic
//static char *mqtt_dev_attr_get_response_topic = "$sys/{pid}/{device-name}/thing/property/get_reply";			// 响应Topic
#define	ONENET_MQTT_ATTR_GET_TOPIC				"$sys/%s/%s/thing/property/get"									// 属【订阅主题】
#define	ONENET_MQTT_ATTR_GET_REPLY_TOPIC		"$sys/%s/%s/thing/property/get_reply"							// 属【发布主题】

//【事件上报】上行
//static char *mqtt_dev_event_report_request_topic  = "$sys/{pid}/{device-name}/thing/event/post";				// 请求Topic
//static char *mqtt_dev_event_report_response_topic = "$sys/{pid}/{device-name}/thing/event/post/reply";		// 响应Topic

//【OTA远程升级通信】上行
//static char *mqtt_dev_foat_request_topic  = "$sys/{pid}/{device-name}/ota/inform";							// 订阅：系统OTA通知
//static char *mqtt_dev_foat_response_topic = "$sys/{pid}/{device-name}/ota/inform_reply";						// 发布：设备回复系统OTA通知

//【获取属性期望值】
//static char *mqtt_dev_get_desired_request_topic  = "$sys/{pid}/{device-name}/thing/property/desired/get";				// 请求topic：发布
//static char *mqtt_dev_get_desired_response_topic = "$sys/{pid}/{device-name}/thing/property/desired/get/reply";		// 响应topic：订阅

//【清除属性期望值】
//static char *mqtt_dev_clr_desired_request_topic  = "$sys/{pid}/{device-name}/thing/property/desired/delete";			// 请求topic：发布
//static char *mqtt_dev_clr_desired_response_topic = "$sys/{pid}/{device-name}/thing/property/desired/delete/reply";	// 响应topic：订阅

//【服务调用】
//static char *mqtt_dev_service_request_topic  = "$sys/{pid}/{device-name}/thing/service/{identifier}/invoke";			// 请求topic：订阅
//static char *mqtt_dev_service_response_topic = "$sys/{pid}/{device-name}/thing/service/{identifier}/invoke_reply";	// 响应topic：	发布

//【脚本解析数据上行】
//static char *mqtt_dev_script_up_request_topic  = "$sys/{pid}/{device-name}/custome/up";					// 请求topic：发布
//static char *mqtt_dev_script_up_response_topic = "$sys/{pid}/{device-name}/custome/up_reply";				// 响应topic：订阅

//【脚本解析数据下行】
//static char *mqtt_dev_script_down_request_topic  = "$sys/{pid}/{device-name}/custome/down/{id}";			// 请求topic：订阅
//static char *mqtt_dev_script_down_response_topic = "$sys/{pid}/{devicename}/custome/down_reply/{id}";		// 响应topic：发布

//【批量数据上报】上行
//static char *mqtt_dev_pack_request_topic  = "$sys/{pid}/{device-name}/thing/pack/post";					// 请求Topic：
//static char *mqtt_dev_pack_response_topic = "$sys/{pid}/{device-name}/thing/pack/post/reply";				// 响应Topic：

//【历史数据上报】上行
//static char *mqtt_dev_history_request_topic  = "$sys/{pid}/{device-name}/thing/history/post";				// 请求Topic：
//static char *mqtt_dev_history_response_topic = "$sys/{pid}/{device-name}/thing/history/post/reply";		// 响应Topic：

////////////////////////////// OneNet MQTT标识定义 /////////////////////////////////////////

#define	ONENET_MQTT_STANDARD_ID				"id"
#define	ONENET_MQTT_STANDARD_VERSION		"version"
#define	ONENET_MQTT_STANDARD_PARAMS			"params"
#define	ONENET_MQTT_STANDARD_CODE_ID		"code"
#define	ONENET_MQTT_STANDARD_MSG_ID			"msg"
#define	ONENET_MQTT_STANDARD_DATA_ID		"data"
#define	ONENET_MQTT_PARAMS_VALUE			"value"
#define	ONENET_MQTT_PARAMS_TIME				"time"
#define	ONENET_MQTT_DEFAULT_VERSION			"1.0"

// 属性标识
#define	ONENET_MQTT_ATTR_POWER_ID			"Power"			// string
#define	ONENET_MQTT_ATTR_DATASTRING_ID		"datastring"	// string
#define	ONENET_MQTT_ATTR_DEBUG_ID			"debug"			// string
#define	ONENET_MQTT_ATTR_MEM_ID				"mem"			// string
#define	ONENET_MQTT_ATTR_REPORT_ID			"report"		// string
#define	ONENET_MQTT_ATTR_TEMP_ID			"temp"			// float
#define	ONENET_MQTT_ATTR_DATA_DTU			"data_dtu"		// A1透传专用

typedef enum
{
	CM_MQTT_PUBLISH_DUP = 8u,
	CM_MQTT_PUBLISH_QOS_0 = ((0u << 1) & 0x06),
	CM_MQTT_PUBLISH_QOS_1 = ((1u << 1) & 0x06),
	CM_MQTT_PUBLISH_QOS_2 = ((2u << 1) & 0x06),
	CM_MQTT_PUBLISH_QOS_MASK = ((3u << 1) & 0x06),
	CM_MQTT_PUBLISH_RETAIN = 0x01
} cm_mqtt_publish_flags_e;

enum sig_method_e
{
	SIG_METHOD_MD5,
	SIG_METHOD_SHA1,
	SIG_METHOD_SHA256
};

enum
{
	TOPIC_ATTR_POST = 0,		// 属性上报
	TOPIC_ATTR_SET_REPLY,		// 属性设置响应
	TOPIC_ATTR_GET_REPLY		// 属性获取响应
};

#define	custom_onenet_IsLinkOK()		(cm_mqtt_client_get_state(onenet_mqtt_client) == 1)

extern cm_mqtt_client_t *onenet_mqtt_client;

int custom_onenet_init(void);
int custom_onenet_send_attribute_post(char *identification[], char *param_value[], int count);


#endif

