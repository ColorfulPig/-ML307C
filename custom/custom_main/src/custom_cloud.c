
#include "custom_cloud.h"
#include "custom_system.h"
#include "custom_sps.h"
#include "custom_track.h"
#include "custom_bluetooth.h"
#include "custom_common.h"
#include "custom_protocol.h"
#include "custom_bms.h"
#include "custom_cloud_lte.h"
#include "custom_onenet.h"

//因为平台侧 长度上传有误 bmsota 指令 0x03时，接收的实际长度为0x0061，但是平台传的时0x6202.
#define CLOUD_CMD50_FORCE_LEN_0062_EN    1
#define CLOUD_CMD50_FORCE_RAW_LEN        0x6202
#define CLOUD_CMD50_FORCE_FIXED_LEN      0x0061


static CLOUD_SPS cloud_sps;

int custom_cloud_send_onenet_attribute_post(uint8_t *buf, uint16_t len)	//-xxx
{
	int w_len = make_base64(0, buf, len, cloud_sps.base64_buffer, PRO_BUFFER_LENGTH);
	cloud_sps.base64_buffer[w_len] = 0;
		
	if(w_len > 0)
	{
		char *identification[2];
		char *param_value[2];

		// 属性上报-report
		identification[0] = ONENET_MQTT_ATTR_REPORT_ID;	//-xxx
		param_value[0] = (char *)cloud_sps.base64_buffer;
		custom_onenet_send_attribute_post(identification, param_value, 1);
		return 0;
	}	
	return -1;
}

int custom_cloud_sendFrame(uint8_t cid, uint8_t tid, uint8_t *buf, uint16_t len)
{
	if(cloud_sps.sbuf != NULL)
	{
		uint16_t pos = 0;

		cloud_sps.sbuf[pos++] = PRO_HEAD;	
		cloud_sps.sbuf[pos++] = cid;	
		cloud_sps.sbuf[pos++] = tid;	
		cloud_sps.sbuf[pos++] = (len >> 8) & 0xFF;	
		cloud_sps.sbuf[pos++] = (len >> 0) & 0xFF;

		if(len > 0)
		{
			memcpy(&cloud_sps.sbuf[pos], buf, len);
			pos += len;
		}
		
		uint8_t check = calc_xor(&cloud_sps.sbuf[1], pos - 1);
		cloud_sps.sbuf[pos++] = check;
		cloud_sps.sbuf[pos++] = PRO_TAIL;

		custom_cloud_send_onenet_attribute_post(cloud_sps.sbuf, pos);
		Cloud_printHex("custom_cloud_send_onenet_attribute_post():", cloud_sps.sbuf, pos);
		
		return 0;
	}
	else
	{
		return -1;
	}
}

int custom_cloud_OnChar(uint8_t ch)
{
	switch(cloud_sps.state)
	{
		case PRO_S_HEAD:
		{
			if(ch == PRO_HEAD)
			{
				cloud_sps.frame_len = 0;
				cloud_sps.frame[cloud_sps.frame_len++] = ch;
				cloud_sps.state = PRO_S_COMMAND;
			}
			break;
		}
		case PRO_S_COMMAND:
		{
			cloud_sps.cid = ch;
			cloud_sps.check = ch;
			cloud_sps.frame[cloud_sps.frame_len++] = ch;
			cloud_sps.state = PRO_S_SERIAL;
			break;
		}	
		case PRO_S_SERIAL:
		{
			cloud_sps.tid = ch;
			cloud_sps.check ^= ch;
			cloud_sps.frame[cloud_sps.frame_len++] = ch;
			cloud_sps.state = PRO_S_MSGLEN1;
			break;
		}
		case PRO_S_MSGLEN1:
		{
			cloud_sps.data_len = ch;
			cloud_sps.check ^= ch;
			cloud_sps.frame[cloud_sps.frame_len++] = ch;
			cloud_sps.state = PRO_S_MSGLEN2;
			break;
		}
		case PRO_S_MSGLEN2:
		{
			cloud_sps.data_len <<= 8;
			cloud_sps.data_len |= ch;
			cloud_sps.check ^= ch;
			cloud_sps.t_len = 0;
			cloud_sps.frame[cloud_sps.frame_len++] = ch;

			#if CLOUD_CMD50_FORCE_LEN_0062_EN
				if((cloud_sps.cid == PRO_CMD50_MODULE) && (cloud_sps.data_len == CLOUD_CMD50_FORCE_RAW_LEN))
				{
					Cloud_printf("%s: force cmd50 len %04X -> %d.", __func__, CLOUD_CMD50_FORCE_RAW_LEN, CLOUD_CMD50_FORCE_FIXED_LEN);
					cloud_sps.data_len = CLOUD_CMD50_FORCE_FIXED_LEN;
				}
			#endif

			if(cloud_sps.data_len > PRO_BUFFER_LENGTH)
			{
				return TPTC_R_FALSE;
			}
			
			if((cloud_sps.data_len > 0) && (cloud_sps.data_body != NULL))
			{				
				cloud_sps.state = PRO_S_MSGDATA;
			}
			else
			{
				cloud_sps.state = PRO_S_CHECK;
			}
			break;
		}
		case PRO_S_MSGDATA:
		{
			cloud_sps.data_body[cloud_sps.t_len++] = ch;			
			cloud_sps.check ^= ch;
			cloud_sps.frame[cloud_sps.frame_len++] = ch;

			if(cloud_sps.t_len >= cloud_sps.data_len)
			{
				cloud_sps.state = PRO_S_CHECK;
			}			
			break;
		}
		case PRO_S_CHECK:
		{
			if(cloud_sps.check == ch)
			{
				cloud_sps.frame[cloud_sps.frame_len++] = ch;
				cloud_sps.state = PRO_S_TAIL;
			}
			else
			{
				return TPTC_R_FALSE;
			}
			break;
		}
		case PRO_S_TAIL:
		{
			if(ch == PRO_TAIL)
			{
				cloud_sps.frame[cloud_sps.frame_len++] = ch;
				return TPTC_R_FRAME;
			}
			else
			{
				return TPTC_R_FALSE;
			}
		}
		default:
			break;
	}
	
	return TPTC_R_CONTINUE;
}

int custom_cloud_OnFrame(void)
{
	Cloud_printf("%s: cid=%02X,tid=%d,len=%d.", __func__, cloud_sps.cid, cloud_sps.tid, cloud_sps.data_len);

	// 指令解析处理
	switch(cloud_sps.cid)
	{
		// 登录包上报
		case PRO_CMD03_LOGIN_REQ:					// 查询命令(云端->LTE->BMS)
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);			
			break;
		}
		// 电池信息上报
		case PRO_CMD04_REPORT_REQ:					// 主动查询(云端->LTE->BMS)
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);		
			break;
		}
		// 激活命令
		case PRO_CMD05_ACTIVE_REQ:					// 云端->BMS
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);
			break;
		}
		// 保护参数设置或读取
		case PRO_CMD06_PROTECT_PARA_REQ:			// 云端->LTE->BMS
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);		
			break;
		}
		// 特殊参数设置或读取
		case PRO_CMD07_SPECIAL_PARA_REQ:			// 云端->LTE->BMS
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);			
			break;
		}
		// 加热指令
		case PRO_CMD0B_HEAT_REQ:					// 云端->LTE->BMS
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);			
			break;
		}		
		// BMS数据透传
		case PRO_CMD11_DATA_TRANSFER_DOWN:			// 云端->LTE->BMS
		{
			custom_bms_send(cloud_sps.frame, cloud_sps.frame_len);			
			break;
		}
		// 模块指令
		case PRO_CMD50_MODULE:						// 云端<->LTE
		{
			// 应答
			custom_cloud_lte_OnInstruction(cloud_sps.tid, cloud_sps.data_body, cloud_sps.data_len);
			break;
		}
		default:
			break;
	}
	
	return 0;
}

int custom_cloud_OnFinish(void)
{
	cloud_sps.state = PRO_S_HEAD;

	return 0;
}

void custom_cloud_task(void *p)
{
	//int mbedtls_base64_decode( unsigned char *dst, size_t dlen, size_t *olen, const unsigned char *src, size_t slen );
	
	while(1)
	{
		
		osDelay(ONE_SECONED);	// 1秒
	}
}

int custom_cloud_init(void)
{
	memset(&cloud_sps, 0, sizeof(cloud_sps));
	
	cloud_sps.data_body = cm_malloc(PRO_BUFFER_LENGTH);
	cloud_sps.frame = cm_malloc(PRO_BUFFER_LENGTH);
	cloud_sps.base64_buffer = cm_malloc(PRO_BUFFER_LENGTH);
	cloud_sps.sbuf = cm_malloc(PRO_BUFFER_LENGTH);
	cloud_sps.state = PRO_S_HEAD;

	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "cloud_task";
	app_task_attr.stack_size = 1024 * 4;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_cloud_task, 0, &app_task_attr);
	
	return 0;
}

