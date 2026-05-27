
#include "custom_bms.h"
#include "custom_system.h"
#include "custom_sps.h"
#include "custom_track.h"
#include "custom_bluetooth.h"
#include "custom_common.h"
#include "custom_protocol.h"
#include "custom_onenet.h"
#include "custom_global.h"
#include "custom_lbs.h"
#include "custom_gnss.h"
#include "custom_datetime.h"
#include "custom_network.h"
#include "custom_bms_ota.h"

static BMS_SPS bms_sps;


int custom_bms_send_frame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len)
{
	if(bms_sps.sbuf != NULL)
	{
		uint16_t pos = 0;

		bms_sps.sbuf[pos++] = PRO_HEAD;	
		bms_sps.sbuf[pos++] = cid;	
		bms_sps.sbuf[pos++] = tid;	
		bms_sps.sbuf[pos++] = (len >> 8) & 0xFF;	
		bms_sps.sbuf[pos++] = (len >> 0) & 0xFF;

		if(len > 0)
		{
			memcpy(&bms_sps.sbuf[pos], buf, len);
			pos += len;
		}
		
		uint8_t check = calc_xor(&bms_sps.sbuf[1], pos - 1);
		bms_sps.sbuf[pos++] = check;
		bms_sps.sbuf[pos++] = PRO_TAIL;
		
		BMS_printHex("bms send:", bms_sps.sbuf, pos);
		custom_bms_send(bms_sps.sbuf, pos);
		
		return 0;
	}
	else
	{
		return -1;
	}
}

// BMS查询通讯模块基本信息
int custom_bms_send_lte_module_info(uint8_t tid, char *imei, char *iccid, char *bt_mac, char *lte_sdk_version, char *lte_app_version)
{
	uint8_t buf[128] = {0};
	uint16_t pos = 0;

	pos = common_sprintf(buf, "%s,%s,%s,%s,%s", imei, iccid, bt_mac, lte_sdk_version, lte_app_version);
	
	BMS_printRaw("custom_bms_send_lte_module_info:", buf, pos);
	custom_bms_send_frame(PRO_CMD08_MODULE_BASE_ACK, tid, buf, pos);

	return 0;
}

// BMS查询通讯模块状态及定位信息
/*	TIME	ASCII	 格式：2022-09-20-14-08-54
	通讯模块状态	U8 	 模块状态：
Bit0：GPS打开状态（1--已打开）
Bit1：GPS定位状态（1--已定位）
Bit2：LBS打开状态（1--已打开）
Bit3：LBS定位状态（1--已定位）
	通讯模块状态	U8 	Bit0: BLE开启状态（1--开启）
Bit1：BLE链接状态（1--链接）
Bit2:  SIM card状态（1--正常）
Bit3：4G驻网状态（1--联网）
Bit4：pdp attach(附着上LTE网络状态)（1--attach)
Bit5: 物联平台链接状态（1--已链接）
Bit6： 物联设备链接状态（1--已链接）
	LBS经度	ASCII	前后通过逗号分割
	LBS纬度	ASCII	前后通过逗号分割
	GPS经度	ASCII	前后通过逗号分割
	GPS纬度	ASCII	前后通过逗号分割
	GPS可见卫星数	ASCII	前后通过逗号分割 个
	GPS使用卫星数	ASCII	前后通过逗号分割 个
	GPS海拔高度	ASCII	前后通过逗号分割 海拔：米
	GPS 速度	ASCII	前后通过逗号分割 公里/小时*/
int custom_bms_send_lte_module_state(uint8_t tid, 
	cm_tm_t dt, uint8_t gnss_module_state, uint8_t lte_module_state, float lbs_longitude, float lbs_latitude,
	float gnss_longitude, float gnss_latitude, uint8_t visible_satellites, uint8_t use_satellites, float altitude,
	float speed)
{
	uint8_t buf[256] = {0};
	uint16_t pos = 0, len = 0;

	// TIME
	pos = common_sprintf(buf, "%04d-%02d-%02d-%02d-%02d-%02d", dt.tm_year, dt.tm_mon, dt.tm_mday, dt.tm_hour, dt.tm_min, dt.tm_sec);
	// 定位模块状态
	buf[pos++] = gnss_module_state;
	// 通讯模块状态
	buf[pos++] = lte_module_state;
	// LBS经度,LBS纬度,GPS经度,GPS纬度,GPS可见卫星数,GPS使用卫星数,GPS海拔高度,GPS速度
	len = common_sprintf(&buf[pos], ",%f,%f,%f,%f,%d,%d,%f,%f,", lbs_longitude, lbs_latitude,	gnss_longitude, gnss_latitude, visible_satellites, use_satellites, altitude, speed);
	pos += len;
	
	BMS_printRaw("custom_bms_send_lte_module_state:", buf, pos);
	custom_bms_send_frame(PRO_CMD09_MODULE_STATE_ACK, tid, buf, pos);

	return 0;
}

// BMS OTA
int custom_bms_send_bms_ota(uint8_t tid)
{
	//custom_bms_send_frame(PRO_CMD0A_BMS_OTA_REQ, tid, buf, pos);
	return 0;
}

// BMS上报调试/故障信息
int custom_bms_send_fault_report_ack(uint8_t tid, uint8_t result)
{
	custom_bms_send_frame(PRO_CMD0C_FAULT_ACK, tid, &result, 1);
	return 0;
}

int custom_bms_OnChar(uint8_t ch)
{
	switch(bms_sps.state)
	{
		case PRO_S_HEAD:
		{
			if(ch == PRO_HEAD)
			{
				bms_sps.frame_len = 0;
				bms_sps.frame[bms_sps.frame_len++] = ch;
				bms_sps.state = PRO_S_COMMAND;
			}
			break;
		}
		case PRO_S_COMMAND:
		{
			bms_sps.cid = ch;
			bms_sps.check = ch;
			bms_sps.frame[bms_sps.frame_len++] = ch;
			bms_sps.state = PRO_S_SERIAL;
			break;
		}	
		case PRO_S_SERIAL:
		{
			bms_sps.tid = ch;
			bms_sps.check ^= ch;
			bms_sps.frame[bms_sps.frame_len++] = ch;
			bms_sps.state = PRO_S_MSGLEN1;
			break;
		}
		case PRO_S_MSGLEN1:
		{
			bms_sps.data_len = ch;
			bms_sps.check ^= ch;
			bms_sps.frame[bms_sps.frame_len++] = ch;
			bms_sps.state = PRO_S_MSGLEN2;
			break;
		}
		case PRO_S_MSGLEN2:
		{
			bms_sps.data_len <<= 8;
			bms_sps.data_len |= ch;
			bms_sps.check ^= ch;
			bms_sps.t_len = 0;
			bms_sps.frame[bms_sps.frame_len++] = ch;

			if(bms_sps.data_len > PRO_BUFFER_LENGTH)
			{
				BMS_printf("%s: drop frame, data_len overflow. cid=%02X tid=%d len=%d", __func__, bms_sps.cid, bms_sps.tid, bms_sps.data_len);
				return TPTC_R_FALSE;
			}
			
			if((bms_sps.data_len > 0) && (bms_sps.data_body != NULL))
			{				
				bms_sps.state = PRO_S_MSGDATA;
			}
			else
			{
				bms_sps.state = PRO_S_CHECK;
			}
			break;
		}
		case PRO_S_MSGDATA:
		{
			bms_sps.data_body[bms_sps.t_len++] = ch;			
			bms_sps.check ^= ch;
			bms_sps.frame[bms_sps.frame_len++] = ch;

			if(bms_sps.t_len >= bms_sps.data_len)
			{
				bms_sps.state = PRO_S_CHECK;
			}			
			break;
		}
		case PRO_S_CHECK:
		{
			if(bms_sps.check == ch)
			{
				bms_sps.frame[bms_sps.frame_len++] = ch;
				bms_sps.state = PRO_S_TAIL;
			}
			else
			{
				BMS_printf("%s: drop frame, xor mismatch. cid=%02X tid=%d len=%d calc=%02X recv=%02X", __func__, bms_sps.cid, bms_sps.tid, bms_sps.data_len, bms_sps.check, ch);
				return TPTC_R_FALSE;
			}
			break;
		}
		case PRO_S_TAIL:
		{
			if(ch == PRO_TAIL)
			{
				bms_sps.frame[bms_sps.frame_len++] = ch;
				bms_sps.LinkOK = 1;
				return TPTC_R_FRAME;
			}
			else
			{
				BMS_printf("%s: drop frame, tail mismatch. cid=%02X tid=%d len=%d recv=%02X", __func__, bms_sps.cid, bms_sps.tid, bms_sps.data_len, ch);
				return TPTC_R_FALSE;
			}
		}
		default:
			break;
	}
	
	return TPTC_R_CONTINUE;
}

int custom_bms_OnFrame(void)
{
	int w_len=0;
	int onenet_forward = 0;		// 转发标志
	int bluetooth_forward = 0;	// 转发标志

	BMS_printf("%s: cid=%02X,tid=%d,len=%d.", __func__, bms_sps.cid, bms_sps.tid, bms_sps.data_len);

	// 指令解析处理
	switch(bms_sps.cid)
	{
		// 登录包上报
		case PRO_CMD83_LOGIN_ACK:						// 登录或查询上报(BMS->LTE->云端)
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;
			break;
		}
		case PRO_CMD93_LOGIN_ACK:						// 登录或查询上报2.0(BMS->LTE->云端)
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;		
			break;
		}
		// 电池信息上报
		case PRO_CMD84_REPORT_ACK:						// 主动上报(BMS->LTE->云端)
		{		
			onenet_forward = 1;
			bluetooth_forward = 1;

			//-xxx			// 解析指令获取bms相关的参数
			break;
		}
		case PRO_CMD94_REPORT_ACK:						// 主动上报2.0(BMS->LTE->云端)
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;		
			break;
		}
		// 激活命令
		case PRO_CMD85_ACTIVE_ACK:						// BMS->LTE->云端
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;		
			break;
		}
		// 保护参数设置或读取
		case PRO_CMD86_PROTECT_PARA_ACK:				// BMS->LTE->云端
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;		
			break;
		}
		// 特殊参数设置或读取
		case PRO_CMD87_SPECIAL_PARA_ACK:				// BMS->LTE->云端
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;			
			break;
		}
		// BMS查询通讯模块基本信息
		case PRO_CMD88_MODULE_BASE_REQ:					// BMS->LTE
		{
			// 应答
			custom_bms_send_lte_module_info(0, g_IMEI, g_ICCID, bluetooth_MAC, g_SDKVER, g_APPVER);
			break;
		}
		// BMS查询通讯模块状态及定位信息
		case PRO_CMD89_MODULE_STATE_REQ:				// BMS->LTE
		{
			cm_tm_t datetime;
			uint8_t gnss_module_state = 0, lte_module_state = 0;

			/*模块状态：
			Bit0：GPS打开状态（1--已打开）
			Bit1：GPS定位状态（1--已定位）
			Bit2：LBS打开状态（1--已打开）
			Bit3：LBS定位状态（1--已定位）*/
			gnss_module_state |= 0x01;
			if(gnss_location.fix != GNSS_FIX_INVALID)
			{
				gnss_module_state |= 0x02; 
			}

			/*模块状态：
			Bit0: BLE开启状态（1--开启）
			Bit1：BLE链接状态（1--链接）
			Bit2:  SIM card状态（1--正常）
			Bit3：4G驻网状态（1--联网）
			Bit4：pdp attach(附着上LTE网络状态)（1--attach)
			Bit5: 物联平台链接状态（1--已链接）
			Bit6： 物联设备链接状态（1--已链接）*/
			lte_module_state |= 0x01;
			if(custom_bluetooth_IsLinkOK())
			{
				lte_module_state |= 0x02; 
			}
			if(custom_network_IsSimCardReady())
			{
				lte_module_state |= 0x04; 
			}
			if(custom_network_IsRegister())
			{
				lte_module_state |= 0x08; 
			}
			if(custom_network_IsPDPActive())
			{
				lte_module_state |= 0x10; 
			}
			if(custom_onenet_IsLinkOK())
			{
				lte_module_state |= 0x20; 
			}
			if(custom_bms_IsLinkOK())
			{
				lte_module_state |= 0x40; 
			}
			
			// 应答
			custom_get_now_datetime(&datetime);
			custom_bms_send_lte_module_state(0, datetime, 
				gnss_module_state, lte_module_state, 
				lbs_location.longitude, lbs_location.latitude, 
				gnss_location.longitude, gnss_location.latitude, gnss_location.vsat, gnss_location.nsat, gnss_location.altitude, gnss_location.spkm);
			break;
		}
		// BMS OTA
		case PRO_CMD8A_BMS_OTA_ACK:						// BMS->LTE
		{
			custom_bms_ota_OnACK(bms_sps.data_body, bms_sps.data_len);
			break;
		}
		// 加热指令
		case PRO_CMD8B_HEAT_ACK:						// BMS->LTE->云端
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;			
			break;
		}
		// BMS上报调试/故障信息
		case PRO_CMD8C_FAULT_REPORT:					// BMS->LTE
		{
			// 应答
			custom_bms_send_fault_report_ack(bms_sps.tid, 1);
			break;
		}
		// BMS数据透传
		case PRO_CMDA1_DATA_TRANSFER_UP:				// BMS->LTE->云端
		{
			onenet_forward = 1;
//小程序不支持			bluetooth_forward = 1;			
			break;
		}
		default:		
			break;
	}
	
	// 转发到蓝牙(不加密)
	if(bluetooth_forward == 1)
	{
		if(custom_bluetooth_IsLinkOK())
		{
			/*w_len = make_base64(1, bms_sps.frame, bms_sps.frame_len, bms_sps.base64_buffer, PRO_BUFFER_LENGTH);
			bms_sps.base64_buffer[w_len] = 0;
			//BMS_printf("bluetooth-makebase64:%s", bms_sps.base64_buffer);
			
			if(w_len > 0)
			{				
				custom_bluetooth_send(bms_sps.base64_buffer, w_len);
			}*/

			custom_bluetooth_send(bms_sps.frame, bms_sps.frame_len);
		}
	}
	
	// 转发到OneNet(加密)
	if(onenet_forward == 1)
	{
		if(custom_onenet_IsLinkOK() && custom_bluetooth_IsCommandIdle())	// 非蓝牙窗口
		{
			w_len = make_base64(0, bms_sps.frame, bms_sps.frame_len, bms_sps.base64_buffer, PRO_BUFFER_LENGTH);
			bms_sps.base64_buffer[w_len] = 0;			
			//BMS_printf("onenet-makebase64:%s", bms_sps.base64_buffer);
				
			if(w_len > 0)
			{
				char *identification[2];
				char *param_value[2];

				// 属性上报-report
				identification[0] = ONENET_MQTT_ATTR_REPORT_ID;
				param_value[0] = (char *)bms_sps.base64_buffer;
				custom_onenet_send_attribute_post(identification, param_value, 1);
			}
		}

	}
	
	return 0;
}

int custom_bms_OnFinish(void)
{
	bms_sps.state = PRO_S_HEAD;

	return 0;
}

// modbus协议返回1，否则返回0
int custom_bms_OnBlock(uint8_t *buf,uint32_t len)
{
	if((buf[0] == 0xFE) || (buf[0] == 0x01))		// Modbus协议
	{
		if(custom_bluetooth_IsLinkOK())
		{
			custom_bluetooth_send(buf, len);		// 直接发送
		}

		return 1;
	}
	
	return 0;
}

void custom_bms_task(void *p)
{
	//int tid = 0;
	//int mbedtls_base64_decode( unsigned char *dst, size_t dlen, size_t *olen, const unsigned char *src, size_t slen );
	
	while(1)
	{
		//BMS_printf("custom_bms_task is running...");
		//custom_bms_SendFrame(1, tid++, NULL, 0);
		
		osDelay(ONE_SECONED);	// 1秒
	}
}

int custom_bms_init(void)
{
	memset(&bms_sps, 0, sizeof(bms_sps));
	
	bms_sps.data_body = cm_malloc(PRO_BUFFER_LENGTH);
	bms_sps.frame = cm_malloc(PRO_BUFFER_LENGTH);
	bms_sps.base64_buffer = cm_malloc(PRO_BUFFER_LENGTH);
	bms_sps.sbuf = cm_malloc(PRO_BUFFER_LENGTH);
	bms_sps.state = PRO_S_HEAD;

	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "bms_task";
	app_task_attr.stack_size = 1024 * 4;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_bms_task, 0, &app_task_attr);
	
	return 0;
}

// 登录包上报-查询命令(不需要LTE主动查询)
/*int custom_bms_send_login_req(uint8_t tid)
{
	custom_bms_send_frame(PRO_CMD03_LOGIN_REQ, tid, NULL, 0);		
	return 0;
}*/

// 电池信息上报-主动查询(不需要LTE主动查询)
/*int custom_bms_send_report_req(uint8_t tid)
{
	custom_bms_send_frame(PRO_CMD04_REPORT_REQ, tid, NULL, 0);		
	return 0;
}*/

// 激活命令(不需要LTE主动查询)
/*int custom_bms_send_active_req(uint8_t tid)
{
	custom_bms_send_frame(PRO_CMD05_ACTIVE_REQ, tid, NULL, 0);		
	return 0;
}*/

// 保护参数读取(不需要LTE主动查询)
/*int custom_bms_send_protect_param_read_req(uint8_t tid)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 1;							// 读写标志: 0x01-读取
	custom_bms_send_frame(PRO_CMD06_PROTECT_PARA_REQ, tid, buf, pos);		

	return 0;
}*/

// 保护参数设置(参数ID,保护值,恢复值,保护时间ms)(不需要LTE主动查询)
/*int custom_bms_send_protect_param_set_req(uint8_t tid, uint8_t protect_id, uint16_t protect_value, uint16_t restore_value, uint16_t protect_time)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 2;							// 读写标志: 0x02-设置
	buf[pos++] = protect_id;				// 参数ID
	CopyWord(&buf[pos], protect_value);		// 保护值
	pos += 2;
	CopyWord(&buf[pos], restore_value);		// 恢复值
	pos += 2;
	CopyWord(&buf[pos], protect_time);		// 保护时间
	pos += 2;
	custom_bms_send_frame(PRO_CMD06_PROTECT_PARA_REQ, tid, buf, pos);

	return 0;
}*/

// 特殊参数读取(不需要LTE主动查询)
/*int custom_bms_send_special_param_read_req(uint8_t tid)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 0x01;						// 读写标志: 0x01-读取
	custom_bms_send_frame(PRO_CMD07_SPECIAL_PARA_REQ, tid, buf, pos);

	return 0;
}*/

// 特殊参数设置(不需要LTE主动查询)
/*int custom_bms_send_special_param_set_req(uint8_t tid, uint8_t param_id, uint8_t *param_value, uint16_t param_len)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 0x02;						// 读写标志: 0x02-写入
	buf[pos++] = param_id;					// 参数ID
	if(param_len > 0)
	{
		memcpy(&buf[pos], param_value, param_len);
		pos += param_len;
	}
	custom_bms_send_frame(PRO_CMD07_SPECIAL_PARA_REQ, tid, buf, pos);

	return 0;
}*/

// 预约加热指令(不需要LTE主动查询)
/*int custom_bms_send_heat(uint8_t tid, uint8_t utc_time)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 0x01;					// 0x01--预约加热
	CopyDword(&buf[pos], utc_time);
	pos += 5;
	custom_bms_send_frame(PRO_CMD0B_HEAT_REQ, tid, buf, pos);

	return 0;
}*/

// 预约加热指令(不需要LTE主动查询)
/*int custom_bms_send_cancel_heat(uint8_t tid)
{
	uint8_t buf[64] = {0};
	uint16_t pos = 0;

	buf[pos++] = 0x02;					// 0x02--取消预约加热
	custom_bms_send_frame(PRO_CMD0B_HEAT_REQ, tid, buf, pos);

	return 0;
}*/

// BMS数据透传(不需要LTE主动查询)
/*int custom_bms_send_data_transfer_down(uint8_t tid, uint8_t *buf, uint16_t len)
{
	custom_bms_send_frame(PRO_CMD11_DATA_TRANSFER_DOWN, tid, buf, len);
	return 0;
}*/

/*int custom_bms_send_data_transfer_up(uint8_t tid, uint8_t *buf, uint16_t len)
{
	custom_bms_send_frame(PRO_CMDA1_DATA_TRANSFER_UP, tid, buf, len);
	return 0;
}*/




