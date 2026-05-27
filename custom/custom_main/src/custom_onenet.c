
#include "custom_onenet.h"
#include "custom_track.h"
#include "custom_system.h"
#include "custom_common.h"
#include "custom_global.h"
#include "custom_network.h"
#include "custom_led.h"
#include "custom_sps.h"

#define	ONENET_MQTT_WAIT_SYSTEM_INFO_TIME			30

#define	ONENET_MQTT_PDP_ACTIVE_TIME					180				// 等待网络激活时间，秒
#define	ONENET_MQTT_CONNECT_SERVER_TIME				180				// 等待连接服务器时间，秒
#define	ONENET_MQTT_SUBSCRIBE_TOPIC_TIME			60				// 等待订阅时间，秒
#define	ONENET_MQTT_DISCONNECT_SERVER_TIME			60				// 等待断开服务器时间，秒

#define	ONENET_MQTT_TOKEN_ACCESS_EXPIRE_TIME		2524579200		// token访问过期时间(2050/01/01 00:00:00)

// mqtt客户端
cm_mqtt_client_t *onenet_mqtt_client = NULL;

// 平台参数
//static char onenet_config_pid[64] 			= "8kd244ZieB";										// 产品ID-hyc
//static char onenet_config_access_key[128]	= "pY3zv115/dmHCa9nOfP9fZuBP7UHBSsrKysj0+nJCS0=";	// 产品级key(BASE64编码)
//static char onenet_config_pid[64] 			= "6Ew2kB9ToD";										// 产品ID-客户
//static char onenet_config_access_key[128]	= "FQPjA+UVeXUTYUdZAQ2v56Gx0+eRB4XlM4nF+9/JGx0=";	// 产品级key(BASE64编码)
static char onenet_config_pid[64] 			= "3uxq7jn9Nu";										// 产品ID-客户
static char onenet_config_access_key[128]	= "DWev/U2rX82dcZsB7wfTewl1NcORXbXhE/lR1yHv/+M=";	// 产品级key(BASE64编码)
static char onenet_config_device_name[64]	= {0};												// 设备名称(使用IMEI)
static uint8_t onenet_dev_token[256]		= {0};												// 经过key计算的token

// MQTT主题
static char mqtt_pub_attr_post_topic[128] = {0};			// 属性上报，属【发布主题】
static char mqtt_pub_attr_post_reply_topic[128] = {0};		// 属性上报，属【订阅主题】
static char mqtt_pub_attr_set_topic[128] = {0};				// 属性设置，属【订阅主题】
static char mqtt_pub_attr_set_reply_topic[128] = {0};		// 属性设置，属【发布主题】
static char mqtt_pub_attr_get_topic[128] = {0};				// 属性获取，属【订阅主题】
static char mqtt_pub_attr_get_reply_topic[128] = {0};		// 属性获取，属【发布主题】

// 连接参数
static int onenet_config_keepAlive = 120;					// 连接保活时间
static int onenet_config_clean = 1;

// 连接状态
static int onenet_mqtt_conn_flag = 0;						// 连接标志
static int onenet_mqtt_sub_flag = 0;						// 订阅标志

// 组包
static int onenet_message_package_id = 0;					// 打包ID
static int onenet_message_count = 0;						// 消息计数
static char *onenet_message_payload;						// 消息内容

// 属性标识
char *user_attr_id_tab[] = 
{
	ONENET_MQTT_ATTR_POWER_ID,
	ONENET_MQTT_ATTR_DATASTRING_ID,
	ONENET_MQTT_ATTR_DEBUG_ID,
	ONENET_MQTT_ATTR_MEM_ID,
	ONENET_MQTT_ATTR_REPORT_ID,
	ONENET_MQTT_ATTR_TEMP_ID,
	ONENET_MQTT_ATTR_DATA_DTU,
};

enum // 与上数组同步
{
	ATTR_POWER_ID = 0,
	ATTR_DATASTRING_ID,
	ATTR_DEBUG_ID,			// 只读
	ATTR_MEM_ID,			// 只读
	ATTR_REPORT_ID,			// 只读
	ATTR_TEMP_ID,
	ATTR_DATA_DTU,
};

// 打包属性上报json字符串
int custom_onenet_make_attribute_post_onejson_string(int id, char *identification[], char *param_value[], int count, char *onejson_package)
{
	int	ret = -1;

	// 创建cJSON对象
	cJSON* onejson = cJSON_CreateObject();

	// 设置消息id号(String,长度不超过13位)
	char id_buf[16] = {0};
	common_sprintf((uint8_t *)id_buf, "%d", id);
	cJSON_AddStringToObject(onejson, ONENET_MQTT_STANDARD_ID, id_buf);		

	// 设置物模型版本号(String,不填默认为1.0)
	cJSON_AddStringToObject(onejson, ONENET_MQTT_STANDARD_VERSION, ONENET_MQTT_DEFAULT_VERSION);						
	
	//////////////////////////参数内容组包////////////////////////////////
	cJSON* resource = cJSON_CreateObject();
	for(int k = 0; k < count; k++)
	{
		cJSON* params_json = cJSON_CreateObject();
		cJSON_AddItemToObject(params_json, ONENET_MQTT_PARAMS_VALUE, cJSON_CreateString(param_value[k]));
		cJSON_AddItemToObject(resource, identification[k], params_json);		
	}
	//////////////////////////////////////////////////////////////////////

	// 设置请求参数(json格式)
	cJSON_AddItemToObject(onejson, ONENET_MQTT_STANDARD_PARAMS, resource);

	// cJSON对象转字符串(不格式化)
	char* str = NULL;
	if(NULL != (str = cJSON_PrintUnformatted(onejson)))	
	{
		strcpy(onejson_package, str);
		cm_free(str);
		ret = 0;
	}

	// 释放对象
	cJSON_Delete(onejson);

	return ret;
}

// 属性上报
int custom_onenet_send_attribute_post(char *identification[], char *param_value[], int count)
{
	if(onenet_message_payload != NULL)
	{	
		if(custom_onenet_make_attribute_post_onejson_string(++onenet_message_package_id, identification, param_value, count, onenet_message_payload) == 0)
		{
			ONENET_printf("%s: %s", __func__, onenet_message_payload);

			// 异常检测（重启重新连接）
			onenet_message_count++;
			if(onenet_message_count > 3)
			{
				onenet_message_count = 0;
				ONENET_printf("%s: cm_pm_reboot().", __func__);
				osDelay(1);
				cm_pm_reboot();
			}

			// 发布消息
			int ret = cm_mqtt_client_publish(onenet_mqtt_client, mqtt_pub_attr_post_topic, onenet_message_payload, strlen(onenet_message_payload), CM_MQTT_PUBLISH_QOS_1);
			if(ret <= 0)
			{
				ONENET_printf("%s: MQTT publish ERROR!!!, ret = %d", __func__, ret);
				return -1;				
			}
			return 0;
		}
	}
	
	return -2;
}

// 属性上报响应
int custom_onenet_on_attribute_post_reply(char *payload)
{
	// payload格式={"id":"4","code":200,"msg":"success"}
	onenet_message_count = 0;
	return 0;
}

// 属性设置
int custom_onenet_on_attribute_set(char *payload)
{
	int result = -1;

	// payload格式={"id":"11","version":"1.0","params":{"datastring":"12345hrgerg=="}}
	// payload格式={"id":"26","version":"1.0","params":{"Power":"1111111111","datastring":"222222222","temp":33}} 
	/* 应答格式：
	{
	    "id":"123",
	    "code":200,
	    "msg":"xxx"
	}
	*/
	
	cJSON *response = cJSON_CreateObject();			// 应答
	cJSON *resources = cJSON_Parse(payload);		// 文本转对象
	if((resources != NULL) && (response != NULL))
	{
		cJSON *object_id = cJSON_GetObjectItem(resources, ONENET_MQTT_STANDARD_ID);
		cJSON *object_params = cJSON_GetObjectItem(resources, ONENET_MQTT_STANDARD_PARAMS);
		if((object_id != NULL) && (object_params != NULL))
		{
			// 扫描匹配属性
			for(int i = 0; i < cJSON_GetArraySize(object_params); i++)
			{
				cJSON *object_attr = cJSON_GetArrayItem(object_params, i);
				if(object_attr != NULL)
				{
					//ONENET_printf("%s: i=%d,attr=%s", __func__, i, object_attr->string); 

					for(int k = 0; k < _countof(user_attr_id_tab); k++)
					{				
						if(strcmp(object_attr->string, user_attr_id_tab[k]) == 0)					
						{
							//ONENET_printf("%s: k=%d", __func__, k);

							// 处理属性设置标识
							switch(k)
							{
								case ATTR_POWER_ID:			// string
								{
									ONENET_printf("%s=%s", object_attr->string, object_attr->valuestring); 
									//-xxx
									result = 0;
									break;
								}
								case ATTR_DATASTRING_ID:	// base64-string
								{
									ONENET_printf("%s=%s", object_attr->string, object_attr->valuestring); 

									// base64解码
									size_t olen = 0;
									uint8_t *data = cm_malloc(ONENET_MESSAGE_PAYLOAD_LEN);
									if(data != NULL)
									{
										if(mbedtls_base64_decode(data, ONENET_MESSAGE_PAYLOAD_LEN, &olen, (uint8_t *)object_attr->valuestring, strlen(object_attr->valuestring)) == 0)
										{
											result = 0;
											Sps_InCloudScan(data, olen);	//-xxx
										}
									}
									break;
								}
								case ATTR_TEMP_ID:			// float
								{
									ONENET_printf("%s=%f", object_attr->string, object_attr->valuedouble); 
									//-xxx
									result = 0;
									break;
								}
								case ATTR_DATA_DTU:			// string
								{
									ONENET_printf("%s=%s", object_attr->string, object_attr->valuestring); 
									//-xxx
									result = 0;
									break;
								}
								default:
								{
									result = -1;
									break;
								}
							}
							break;
						}
					}
				}
			}

			// 应答
			if(result == 0)
			{				
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_ID, object_id->valuestring);	
				cJSON_AddNumberToObject(response, ONENET_MQTT_STANDARD_CODE_ID, 200);
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_MSG_ID, "success");
			}
			else
			{
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_ID, object_id->valuestring);	
				cJSON_AddNumberToObject(response, ONENET_MQTT_STANDARD_CODE_ID, 404);
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_MSG_ID, "fail");
			}

			// cJSON对象转字符串(不格式化)
			char* str = NULL;
			if(NULL != (str = cJSON_PrintUnformatted(response)))	
			{
				ONENET_printf("%s: %s", __func__, str);

				// 发布消息
				int ret = cm_mqtt_client_publish(onenet_mqtt_client, mqtt_pub_attr_set_reply_topic, str, strlen(str), CM_MQTT_PUBLISH_QOS_1);
				if(ret <= 0)
				{
					ONENET_printf("%s: MQTT publish ERROR!!!, ret = %d", __func__, ret);
				}
				cm_free(str);
			}
		}
		cJSON_Delete(resources);
	}

	return 0;
}

// 属性获取
int custom_onenet_on_attribute_get(char *payload)
{
	int result = -1;

	// payload格式={"id":"10","version":"1.0","params":["Power","datastring","debug","mem","report","temp"]}
	/* 应答格式：
	{
	    "id":"123",
	    "code":200,
	    "msg":"xxxxxx",
	    "data":{
	        "temperature":39.5,
	        "humidity":20
	    }
	}
	*/
	
	cJSON *response = cJSON_CreateObject();			// 应答
	cJSON *resp_data = cJSON_CreateObject();	
	cJSON *resources = cJSON_Parse(payload);		// 文本转对象
	if((resources != NULL) && (response != NULL) && (resp_data != NULL))
	{
		cJSON *object_id = cJSON_GetObjectItem(resources, ONENET_MQTT_STANDARD_ID);
		cJSON *object_params = cJSON_GetObjectItem(resources, ONENET_MQTT_STANDARD_PARAMS);
		if((object_id != NULL) && (object_params != NULL))
		{		
			// 扫描匹配属性
			for(int i = 0; i < cJSON_GetArraySize(object_params); i++)
			{
				cJSON *object_attr = cJSON_GetArrayItem(object_params, i);
				if(object_attr != NULL)
				{
					//ONENET_printf("%s: i=%d,attr=%s", __func__, i, object_attr->valuestring); 

					for(int k = 0; k < _countof(user_attr_id_tab); k++)
					{	
						if(strcmp(object_attr->valuestring, user_attr_id_tab[k]) == 0)
						{
							//ONENET_printf("%s: k=%d", __func__, k);		

							// 处理属性获取标识
							switch(k)
							{
								case ATTR_POWER_ID:					// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_POWER_ID");	
									result = 0;
									break;
								}
								case ATTR_DATASTRING_ID:			// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_DATASTRING_ID");	
									result = 0;
									break;
								}
								case ATTR_DEBUG_ID:					// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_DEBUG_ID");	
									result = 0;
									break;
								}
								case ATTR_MEM_ID:					// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_MEM_ID");	
									result = 0;
									break;
								}
								case ATTR_REPORT_ID:				// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_REPORT_ID");	
									result = 0;
									break;
								}
								case ATTR_TEMP_ID:					// foalt
								{
									//-xxx
									cJSON_AddNumberToObject(resp_data, user_attr_id_tab[k], 36.5);	
									result = 0;
									break;
								}
								case ATTR_DATA_DTU:					// string
								{
									//-xxx
									cJSON_AddStringToObject(resp_data, user_attr_id_tab[k], "ATTR_DATA_DTU");	
									result = 0;
									break;
								}
								default:
									break;
							}

							// 跳出检测
							break;
						}
					}
				}
			}			

			// 应答
			if(result == 0)
			{				
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_ID, object_id->valuestring);	
				cJSON_AddNumberToObject(response, ONENET_MQTT_STANDARD_CODE_ID, 200);
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_MSG_ID, "success");
				cJSON_AddItemToObject(response, ONENET_MQTT_STANDARD_DATA_ID, resp_data);
			}
			else
			{
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_ID, object_id->valuestring);	
				cJSON_AddNumberToObject(response, ONENET_MQTT_STANDARD_CODE_ID, 404);
				cJSON_AddStringToObject(response, ONENET_MQTT_STANDARD_MSG_ID, "fail");
			}

			// cJSON对象转字符串(不格式化)
			char* str = NULL;
			if(NULL != (str = cJSON_PrintUnformatted(response)))	
			{				
				ONENET_printf("%s: %s", __func__, str);

				// 发布消息
				int ret = cm_mqtt_client_publish(onenet_mqtt_client, mqtt_pub_attr_get_reply_topic, str, strlen(str), CM_MQTT_PUBLISH_QOS_1);
				if(ret <= 0)
				{
					ONENET_printf("%s: MQTT publish ERROR!!!, ret = %d", __func__, ret);
				}
				cm_free(str);
			}		
		}
		cJSON_Delete(resources);
	}

	return 0;
}

// 连接状态回调
static int custom_onenet_mqtt_connack_cb(cm_mqtt_client_t* client, int session, cm_mqtt_conn_state_e conn_res)
{
	ONENET_printf("%s: conn_res=%d.", __func__, conn_res);

	if(conn_res == CM_MQTT_CONN_STATE_SUCCESS)				// 连接成功
	{
		if(onenet_mqtt_conn_flag != 1)
		{
			onenet_mqtt_conn_flag = 1;
			custom_led_setState(LED_STATE_ONLINE);

			onenet_message_count = 0;
		}
	}
	else if(conn_res > CM_MQTT_CONN_STATE_RECONNECTING)		// 连接失败
	{
		if(onenet_mqtt_conn_flag != 0)
		{
			onenet_mqtt_conn_flag = 0;
			custom_led_setState(LED_STATE_READY);
		}
	}
           
    return 0;
}

// 发布消息回调
static int custom_onenet_mqtt_publish_cb(cm_mqtt_client_t* client, unsigned short msgid, char* topic, int total_len, int payload_len,	char* payload)
{
	ONENET_printf("************onenet mqtt recv************");
	ONENET_printf("topic=%s", topic);
	ONENET_printf("payload=%s", payload);

	if(memcmp(mqtt_pub_attr_post_reply_topic, topic, strlen(topic)) == 0)	// 属性上报响应
	{
		ONENET_printf("mqtt_pub_attr_post_reply_topic");
		custom_onenet_on_attribute_post_reply(payload);
	}
	else if(memcmp(mqtt_pub_attr_set_topic, topic, strlen(topic)) == 0)		// 属性设置
	{
		ONENET_printf("mqtt_pub_attr_set_topic");				
		custom_onenet_on_attribute_set(payload);		
	}
	else if(memcmp(mqtt_pub_attr_get_topic, topic, strlen(topic)) == 0)		// 属性获取
	{
		ONENET_printf("mqtt_pub_attr_get_topic");
		custom_onenet_on_attribute_get(payload);
	}
	else
	{
		ONENET_printf("mqtt_pub_undefined_topic");
	}
	
	return 0;
}

// 发布消息ack回调
static int custom_onenet_mqtt_puback_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
	ONENET_printf("%s: msgid=%d,dup=%d.", __func__, msgid, dup);
	// msgid=47383,dup=0
	return 0;
}

// 发布消息recv回调
static int custom_onenet_mqtt_pubrec_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
	ONENET_printf("%s: msgid=%d,dup=%d.", __func__, msgid, dup);

	return 0;
}

// 发布消息rel回调
static int custom_onenet_mqtt_pubrel_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
	ONENET_printf("%s: msgid=%d,dup=%d.", __func__, msgid, dup);

	return 0;
}

// r发布消息comp回调
static int custom_onenet_mqtt_pubcomp_cb(cm_mqtt_client_t* client, unsigned short msgid, char dup)
{
	ONENET_printf("%s: msgid=%d,dup=%d.", __func__, msgid, dup);
	
	return 0;
}

// 订阅ack回调
static int custom_onenet_mqtt_suback_cb(cm_mqtt_client_t* client, unsigned short msgid, int count, int qos[])
{
	ONENET_printf("%s: msgid=%d,count=%d.", __func__, msgid, count);
	
	onenet_mqtt_sub_flag = 1;

	return 0;
}

// 取消订阅ack回调
static int custom_onenet_mqtt_unsuback_cb(cm_mqtt_client_t* client, unsigned short msgid)
{
	ONENET_printf("%s: msgid=%d.", __func__, msgid);

	return 0;
}

// ping回调
static int custom_onenet_mqtt_pingresp_cb(cm_mqtt_client_t* client, int ret)
{
	ONENET_printf("%s: ret=%d.", __func__, ret);

	return 0;
}

// 消息超时回调，包括publish/subscribe/unsubscribe等
static int custom_onenet_mqtt_timeout_cb(cm_mqtt_client_t* client, unsigned short msgid)
{
	ONENET_printf("%s: msgid=%d.", __func__, msgid);

	return 0;
}

// 计算token算法
static int32_t custom_onenet_generate_token(
	uint8_t* token, 				// 输出鉴权Token
	enum sig_method_e method, 		// 签名方法
	uint32_t exp_time, 				// 访问过期时间，unix时间
	const uint8_t* product_id,	 	// 产品ID
	const uint8_t* dev_name, 		// 设备名称
	const uint8_t* access_key)		// key
{
	uint8_t  base64_data[64] = { 0 };
	uint8_t  str_for_sig[64] = { 0 };
	uint8_t  sign_buf[128]   = { 0 };
	uint32_t base64_data_len = sizeof(base64_data);
	uint8_t* sig_method_str  = NULL;
	uint32_t sign_len        = 0;
	uint32_t i               = 0;
	uint8_t* tmp             = NULL;
	Hmac     hmac;

	// 1.参数组版本号(version=2018-10-31)
	common_sprintf(token, "version=%s", ONENET_DEV_TOKEN_VERISON);

	// 2.访问资源(&res=products%2F123123),'%2F'='/'
	if(dev_name)
	{
		/* 设备级别鉴权（一机一密） */
		// 设备级格式为：products/{产品id}/devices/{设备名称}
		common_sprintf(token + strlen((const char *)token), "&res=products%%2F%s%%2Fdevices%%2F%s", product_id, dev_name);
	} 
	else 
	{
		/* 产品级鉴权（一型一密） */
		// 产品级格式为：products/{产品id}
		common_sprintf(token + strlen((const char *)token), "&res=products%%2F%s", product_id);
	}

	// 3.访问过期时间(&et=1537255523)
	// 当一次访问参数中的et时间小于当前时间时，平台会认为访问参数过期从而拒绝该访问
	common_sprintf(token + strlen((const char *)token), "&et=%u", exp_time);

	////////////////////////////////////////////////////////////////

	// key密钥解码Base64
	Base64_Decode(access_key, strlen((const char *)access_key), base64_data, (unsigned int *)&base64_data_len);

	// 签名方法（支持md5、sha1、sha256）
	if(SIG_METHOD_MD5 == method) 
	{
		wc_HmacSetKey(&hmac, MD5, base64_data, base64_data_len);
		sig_method_str = (uint8_t*)ONENET_DEV_TOKEN_METHOD_MD5;
		sign_len       = 16;
	} 
	else if(SIG_METHOD_SHA1 == method)
	{
		wc_HmacSetKey(&hmac, SHA, base64_data, base64_data_len);
		sig_method_str = (uint8_t*)ONENET_DEV_TOKEN_METHOD_SHA1;
		sign_len       = 20;
	}
	else if(SIG_METHOD_SHA256 == method) 
	{
		wc_HmacSetKey(&hmac, SHA256, base64_data, base64_data_len);
		sig_method_str = (uint8_t*)ONENET_DEV_TOKEN_METHOD_SHA256;
		sign_len       = 32;
	}

	// 4.签名方法(&method=sha1)
	common_sprintf(token + strlen((const char *)token), "&method=%s", sig_method_str);

	////////////////////////////////////////////////////////////////

	// 产品
	if(dev_name)
	{
		common_sprintf(str_for_sig, "%u\n%s\nproducts/%s/devices/%s\n%s", exp_time, sig_method_str, product_id, dev_name, ONENET_DEV_TOKEN_VERISON);
	} 
	else 
	{
		common_sprintf(str_for_sig, "%u\n%s\nproducts/%s\n%s", exp_time, sig_method_str, product_id, ONENET_DEV_TOKEN_VERISON);
	}
	wc_HmacUpdate(&hmac, str_for_sig, strlen((const char *)str_for_sig));
	wc_HmacFinal(&hmac, sign_buf);

	memset(base64_data, 0, sizeof(base64_data));
	base64_data_len = sizeof(base64_data);
	Base64_Encode_NoNl(sign_buf, sign_len, base64_data, (unsigned int *)&base64_data_len);

	////////////////////////////////////////////////////////////////

	// 5.签名结果字符串(&sign=ZjA1NzZlMmMxYzIOTg3MjBzNjYTI2MjA4Yw%3D)
	strcat((char *)token, (const char*)"&sign=");
	tmp = token + strlen((const char *)token);

	// 转换编码
	for (i = 0; i < base64_data_len; i++) 
	{
		switch (base64_data[i]) 
		{
			case '+':
				strcat((char *)tmp, (const char*)"%2B");
				tmp += 3;
				break;
			case ' ':
				strcat((char *)tmp, (const char*)"%20");
				tmp += 3;
				break;
			case '/':
				strcat((char *)tmp, (const char*)"%2F");
				tmp += 3;
				break;
			case '?':
				strcat((char *)tmp, (const char*)"%3F");
				tmp += 3;
				break;
			case '%':
				strcat((char *)tmp, (const char*)"%25");
				tmp += 3;
				break;
			case '#':
				strcat((char *)tmp, (const char*)"%23");
				tmp += 3;
				break;
			case '&':
				strcat((char *)tmp, (const char*)"%26");
				tmp += 3;
				break;
			case '=':
				strcat((char *)tmp, (const char*)"%3D");
				tmp += 3;
				break;
			default:
				*tmp = base64_data[i];
				tmp += 1;
				break;
		}
	}
	// version=2018-10-31&res=products%2F0ch7rnzaMM%2Fdevices%2F4G-MQTT&et=1924833600&method=sha1&sign=V817q98BBduPShkD%2BPgNxbEcE%2FQ%3D
	
	return 0;
}

// 等待网络激活（失败重启）
static int custom_onenet_mqtt_wait_network_ready(void)
{
	uint16_t wait_network_time;

	wait_network_time = 0;
	while(!custom_network_IsPDPActive())
	{
		osDelay(ONE_SECONED);

		wait_network_time++;
		// ONENET_printf("%s: wait_network_time=%d", __func__, wait_network_time);
		
		if(wait_network_time >= ONENET_MQTT_PDP_ACTIVE_TIME)
		{
			ONENET_printf("%s: cm_pm_reboot().", __func__);
			osDelay(1);
			cm_pm_reboot();
		}
	}

	return 0;
}

// mqtt初始化
static int custom_onenet_mqtt_wait_system_info_ready(void)
{
	uint16_t wait_system_info_time = 0;

	while((g_ReadyOK == 0) || (strlen(g_IMEI) == 0))
	{
		osDelay(ONE_SECONED);
		wait_system_info_time++;

		if(wait_system_info_time >= ONENET_MQTT_WAIT_SYSTEM_INFO_TIME)
		{
			ONENET_printf("%s: wait system info timeout, g_ReadyOK=%d, IMEI=%s", __func__, g_ReadyOK, g_IMEI);
			return -1;
		}
	}

	return 0;
}

static int custom_onenet_mqtt_client_init(void)
{
	// 初始赋值
	strcpy(onenet_config_device_name, g_IMEI);					// 设备名称用IMEI
	if(onenet_message_payload == NULL)
	{
		onenet_message_payload = cm_malloc(ONENET_MESSAGE_PAYLOAD_LEN);
	}
	if(onenet_message_payload == NULL)
	{
		ONENET_printf("%s: cm_malloc() error.", __func__);
		return -2;
	}
	
	// 【主题】
	common_sprintf((uint8_t *)mqtt_pub_attr_post_topic, ONENET_MQTT_ATTR_POST_TOPIC, onenet_config_pid, onenet_config_device_name);
	common_sprintf((uint8_t *)mqtt_pub_attr_post_reply_topic, ONENET_MQTT_ATTR_POST_REPLY_TOPIC, onenet_config_pid, onenet_config_device_name); 
	common_sprintf((uint8_t *)mqtt_pub_attr_set_topic, ONENET_MQTT_ATTR_SET_TOPIC, onenet_config_pid, onenet_config_device_name);
	common_sprintf((uint8_t *)mqtt_pub_attr_set_reply_topic, ONENET_MQTT_ATTR_SET_REPLY_TOPIC, onenet_config_pid, onenet_config_device_name); 
	common_sprintf((uint8_t *)mqtt_pub_attr_get_topic, ONENET_MQTT_ATTR_GET_TOPIC, onenet_config_pid, onenet_config_device_name);
	common_sprintf((uint8_t *)mqtt_pub_attr_get_reply_topic, ONENET_MQTT_ATTR_GET_REPLY_TOPIC, onenet_config_pid, onenet_config_device_name); 

	// 创建MQTT客户端
	onenet_mqtt_client = cm_mqtt_client_create();
	if(onenet_mqtt_client == NULL)
	{
		ONENET_printf("%s: cm_mqtt_client_create() error.", __func__);
		return -1;
	}

	// 设置client参数
	int version = 4;			// 版本3.1.1
	int pkt_timeout = 10;		// 发送超时10秒
	int reconn_times = 3;		// 重连三次
	int reconn_cycle = 20;		// 重连间隔20秒
	int reconn_mode = 0;		// 以固定间隔尝试重连
	int retry_times = 3;		// 重传三次
	int ping_cycle = 60;		// ping周期60秒
	int dns_priority = 1;		// dns解析ipv4优先

	// 设置回调函数
	cm_mqtt_client_cb_t callback = {0};
	callback.connack_cb = custom_onenet_mqtt_connack_cb;			// 连接状态回调
	callback.publish_cb = custom_onenet_mqtt_publish_cb;			// 发布消息回调
	callback.puback_cb = custom_onenet_mqtt_puback_cb;				// 发布消息ack回调
	callback.pubrec_cb = custom_onenet_mqtt_pubrec_cb;				// 发布消息recv回调
	callback.pubrel_cb = custom_onenet_mqtt_pubrel_cb;				// 发布消息rel回调
	callback.pubcomp_cb = custom_onenet_mqtt_pubcomp_cb;			// 发布消息comp回调
	callback.suback_cb = custom_onenet_mqtt_suback_cb;				// 订阅ack回调
	callback.unsuback_cb = custom_onenet_mqtt_unsuback_cb;			// 取消订阅ack回调
	callback.pingresp_cb = custom_onenet_mqtt_pingresp_cb;			// ping回调
	callback.timeout_cb = custom_onenet_mqtt_timeout_cb;			// 消息超时回调

	// 设置选项
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_EVENT, (void*)&callback);				// 回调函数集合
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_VERSION, (void*)&version);
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_PKT_TIMEOUT, (void*)&pkt_timeout);		// 发送超时，单位:s
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_RETRY_TIMES, (void*)&retry_times);		// 重传次数
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_RECONN_MODE, (void*)&reconn_mode);		// 重连模式，0:以固定间隔尝试重连，1:以上次重连间隔的2倍尝试重连
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_RECONN_TIMES, (void*)&reconn_times); // 重连次数
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_RECONN_CYCLE, (void*)&reconn_cycle); // 重连间隔，单位:s
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_PING_CYCLE, (void*)&ping_cycle);
	cm_mqtt_client_set_opt(onenet_mqtt_client, CM_MQTT_OPT_DNS_PRIORITY, (void*)&dns_priority); // MQTT dns解析优先级, 0(默认):使用全局优先级, 1:v4优先, 2:v6优先 			
	//CM_MQTT_OPT_SSL_ENABLE,										// SSL开启，1：MQTTS，0：MQTT
	//CM_MQTT_OPT_SSL_ID,											// SSL通道
	//CM_MQTT_OPT_PING_CYCLE,										// 发送ping包的周期，单位:s

	// 计算token
	memset(onenet_dev_token, 0, sizeof(onenet_dev_token));
	custom_onenet_generate_token(
		onenet_dev_token, 										// 输出鉴权Token
		SIG_METHOD_SHA1, 										// 签名方法
		ONENET_MQTT_TOKEN_ACCESS_EXPIRE_TIME, 					// Token访问过期时间
		(const uint8_t*)onenet_config_pid, 						// 产品ID
		(const uint8_t*)onenet_config_device_name, 				// 设备名称
		(const uint8_t*)onenet_config_access_key);				// 产品access_key	

	ONENET_printf("%s: token=%s", __func__, onenet_dev_token);

	return 0;
}

// 连接服务器（失败重启）
int custom_onenet_mqtt_connect_server(void)
{
	uint16_t wait_connect_time;

	// 连接选项
	cm_mqtt_connect_options_t conn_options = {
		.hostport = (unsigned short)ONENET_MQTT_SERVER_PORT,	// 服务器端口号
		.hostname = (const char*)ONENET_MQTT_SERVER_ADDR,		// 服务器域名或IP
		.clientid = (const char*)onenet_config_device_name, 	// clientid（设备名称）
		.username = (const char*)onenet_config_pid, 			// 用户名（平台分配的产品ID）
		.password = (const char*)onenet_dev_token,				// 密码（经过key计算的token）
		.keepalive = (unsigned short)onenet_config_keepAlive,	// 保活时间
		.will_topic = NULL, 									// 标题
		.will_message = NULL,									// 信息
		.will_message_len = 0,									// 信息长度
		.will_flag = 0, 										// 遗嘱标志。若要使用遗嘱机制请置1，并补充相关遗嘱信息
		.clean_session = (char)onenet_config_clean,
																//will_qos; 		// qos
																//will_retain;		// retain
																//clean_session;	// 清除标志
																//conn_flags;		// 连接标志
	};	
	
	onenet_mqtt_conn_flag = 0;
	
	// mqtt连接
	cm_mqtt_client_connect(onenet_mqtt_client, &conn_options);

	// 等待mqtt连接结果
	wait_connect_time = 0;
	while (!onenet_mqtt_conn_flag)
	{
		osDelay(ONE_SECONED);

		wait_connect_time++;
		// ONENET_printf("%s: wait_connect_time=%d", __func__, wait_connect_time);
				
		if(wait_connect_time >= ONENET_MQTT_CONNECT_SERVER_TIME)
		{
			ONENET_printf("%s: cm_pm_reboot().", __func__);
			osDelay(1);
			cm_pm_reboot();
		}
	}

	return 0;
}

// 订阅主题（失败重启）
int custom_onenet_mqtt_subscribe_topic(void)
{
	uint16_t wait_subscribe_time;
	char *topic_tmp[3] = {0};
	char qos_tmp[3] = {0};	
	
	// 主题列表与QOS列表
	topic_tmp[0] = mqtt_pub_attr_post_reply_topic;
	topic_tmp[1] = mqtt_pub_attr_set_topic;
	topic_tmp[2] = mqtt_pub_attr_get_topic;
	qos_tmp[0] = 0;	
	qos_tmp[1] = 0;	
	qos_tmp[2] = 0;
	
	onenet_mqtt_sub_flag = 0;

	// 订阅mqtt topic
	int ret = cm_mqtt_client_subscribe(onenet_mqtt_client, (const char**)topic_tmp, qos_tmp, 3);
	if(ret != CM_MQTT_RET_OK)
	{
		ONENET_printf("MQTT subscribe ERROR!!!, ret = %d", ret);
	}

	// 等待mqtt订阅成功
	wait_subscribe_time = 0;
	while (!onenet_mqtt_sub_flag)
	{
		osDelay(ONE_SECONED);

		wait_subscribe_time++;
		// ONENET_printf("%s: wait_subscribe_time=%d", __func__, wait_subscribe_time);
				
		if(wait_subscribe_time >= ONENET_MQTT_SUBSCRIBE_TOPIC_TIME)
		{
			ONENET_printf("%s: cm_pm_reboot().", __func__);
			osDelay(1);
			cm_pm_reboot();
		}
	}
	
	return 0;
}

// 断开服务器（失败重启）
int custom_onenet_mqtt_disconnect_server(void)
{
	uint16_t wait_disconnect_time;
	
	// 断开连接
	if(onenet_mqtt_client != NULL)
	{
		cm_mqtt_client_disconnect(onenet_mqtt_client);
		
		// 等待mqtt断开结果
		wait_disconnect_time = 0;
		while (onenet_mqtt_conn_flag != 0)
		{
			osDelay(ONE_SECONED);

			wait_disconnect_time++;
			ONENET_printf("%s: wait_disconnect_time=%d", __func__, wait_disconnect_time);
					
			if(wait_disconnect_time >= ONENET_MQTT_DISCONNECT_SERVER_TIME)
			{
				ONENET_printf("%s: cm_pm_reboot().", __func__);
				osDelay(1);
				cm_pm_reboot();
			}
		}	

		// 释放资源 
		cm_mqtt_client_destroy(onenet_mqtt_client);
		onenet_mqtt_client = NULL;
		onenet_mqtt_conn_flag = 0;
		onenet_mqtt_sub_flag = 0;
		if(onenet_message_payload != NULL)
		{
			cm_free(onenet_message_payload);
			onenet_message_payload = NULL;
		}
	}
	
	return 0;
}

static void custom_onenet_task(void)
{
	uint32_t onenet_task_count;
	
	while(1)
	{		
		// 等待网络激活（失败重启）
		if(custom_onenet_mqtt_wait_system_info_ready() != 0)
		{
			continue;
		}

		custom_onenet_mqtt_wait_network_ready();
	    
		// mqtt初始化
		custom_onenet_mqtt_client_init();
		
		// 连接服务器（失败重启）
		custom_onenet_mqtt_connect_server();

		// 订阅主题（失败重启）
		custom_onenet_mqtt_subscribe_topic();
		
		// 在线等待
		onenet_task_count = 0;
		while(onenet_mqtt_conn_flag == 1)
		{			
			osDelay(ONE_SECONED);
			onenet_task_count++;
			
			// 定时任务
			if((onenet_task_count % 10) == 0)
			{
				// ONENET_printf("%s: %d.", __func__, onenet_task_count);
				
			}
		}

		// 断开连接服务器（失败重启）
		custom_onenet_mqtt_disconnect_server();
	}
}

int custom_onenet_init(void)
{
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "onenet_task";
	app_task_attr.stack_size = 4096 * 8;
	app_task_attr.priority = osPriorityNormal;

	osThreadNew((osThreadFunc_t)custom_onenet_task, 0, &app_task_attr);

	return 0;
}

