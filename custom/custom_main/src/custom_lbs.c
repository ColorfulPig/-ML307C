
#include "custom_lbs.h"
#include "custom_track.h"

int lbs_location_Update = 0;			// 更新标志
custom_lbs_location_t lbs_location;

void custom_lbs_cb(cm_lbs_callback_event_e event,cm_lbs_location_rsp_t *location,void *cb_arg)
{
	cm_lbs_location_platform_e lbs_platform = 0;
	int ret =0;

	LBS_printf("cm_lbs_callback_event =%d\r\n",event);

	if(event == CM_LBS_LOCATION_OK)
	{	  
		lbs_location.state = location->state;						// 定位结果状态  
		lbs_location.longitude = atof(location->longitude);			// 经度
		lbs_location.latitude = atof(location->latitude); 			// 纬度 
		lbs_location.radius = atof(location->radius);				// 精度半径	
		lbs_location_Update = 1;
		
		lbs_platform = location->platform;
		LBS_printf("location.platform=%d\r\n",location->platform);
		LBS_printf("location.state=%d\r\n",location->state);
		LBS_printf("location.longittude=%s\r\n",location->longitude);
		LBS_printf("location.latitude=%s\r\n",location->latitude);
		LBS_printf("location.precision=%s\r\n",location->location_describe);
		LBS_printf("location.desc=%s\r\n",location->radius);
		LBS_printf("location.country=%s\r\n",location->country);
		LBS_printf("location.province=%s\r\n",location->province);
		LBS_printf("location.city=%s\r\n",location->city);
		LBS_printf("location.citycode=%s\r\n",location->citycode);
		LBS_printf("location.adcode=%s\r\n",location->adcode);
		LBS_printf("location.poi=%s\r\n",location->poi);

		cm_lbs_deinit();

		if((lbs_platform == CM_LBS_PLAT_AMAP10) || (lbs_platform == CM_LBS_PLAT_AMAP20))
		{
			uint8_t aplikey[64] = {0};
			cm_lbs_amap_location_attr_t apap_cfg_acqure = {aplikey,0};
			
			ret = cm_lbs_get_attr(lbs_platform,&apap_cfg_acqure);
			if(ret == 0)
			{
				LBS_printf("apap_cfg_acqure timeout=%d\r\n",apap_cfg_acqure.time_out);
				LBS_printf("apap_cfg_acqure aplikey =%s\r\n",apap_cfg_acqure.api_key);
			}
			else
			{
				LBS_printf("cm_lbs_get_attr ret=%d\r\n",ret);
			}
		}
		else if(lbs_platform == CM_LBS_PLAT_ONEOSPOS)
		{
			char pid[64] = {0};
			cm_lbs_oneospos_attr_t noeospos_cfg_acqure = {pid,0};
			ret = cm_lbs_get_attr(lbs_platform,&noeospos_cfg_acqure);
			if(ret == 0)
			{
				LBS_printf("noeospos_cfg_acqure timeout=%d\r\n",noeospos_cfg_acqure.time_out);
				LBS_printf("noeospos_cfg_acqure pid =%s\r\n",noeospos_cfg_acqure.pid);
			}
			else
			{
				LBS_printf("cm_lbs_get_attr ret=%d\r\n",ret);
			}
		}
	}
}

int custom_lbs_start(cm_lbs_location_platform_e lbs_platform)
{
	int ret = -1;
	
	if((lbs_platform == CM_LBS_PLAT_AMAP10) || (lbs_platform == CM_LBS_PLAT_AMAP20))
	{
		uint8_t aplikey[64] = {0};
		cm_lbs_amap_location_attr_t apap_cfg = {0};
		cm_lbs_amap_location_attr_t apap_cfg_acqure = {aplikey,0};

		//高德1.0平台；CM:LBS:platform:key:time_out:						   nearbts_enable:digital_sign_enable:digital_key
		//高德2.0平台；CM:LBS:platform:key:time_out:show_fields_enable:nearbts_enable:digital_sign_enable:digital_key

		// 配置高德参数
		apap_cfg.api_key = aplikey;    			// apikey
		apap_cfg.time_out = 60;					// 超时时间,(0-60s)
		apap_cfg.nearbts_enable = 0;			// 是否启用邻区
		apap_cfg.digital_sign_enable = 0;		// 是否启动数字签名
		if(apap_cfg.digital_sign_enable == 1)
		{
			apap_cfg.digital_key = 0;			// 数字签名key
		}
		if(lbs_platform == CM_LBS_PLAT_AMAP20)
		{
			apap_cfg.show_fields_enable = 1;	// 高德定位2.0是否请求具体的位置描述 0：不请求 1：请求；高德定位1.0默认开启，无此项配置
		}		
        
		// LBS初始化
		ret = cm_lbs_init(lbs_platform, &apap_cfg);
		LBS_printf("cm_lbs_init ret=%d\r\n",ret);
		
		// 获取平台定位配置信息
		ret = cm_lbs_get_attr(lbs_platform, &apap_cfg_acqure);
		if(ret == 0)
		{
			LBS_printf("apap_cfg_acqure timeout=%d\r\n",apap_cfg_acqure.time_out);
			LBS_printf("apap_cfg_acqure aplikey =%s\r\n",apap_cfg_acqure.api_key);
		}
		else
		{
			LBS_printf("cm_lbs_get_attr ret=%d\r\n",ret);
		}

		// LBS获取位置信息(异步)
		ret = cm_lbs_location(custom_lbs_cb, NULL);
		LBS_printf("cm_lbs_location ret=%d\r\n",ret);
		
		/*ret = cm_lbs_get_attr(lbs_platform, &apap_cfg_acqure);
		if(ret == 0)
		{
			LBS_printf("apap_cfg_acqure timeout=%d\r\n",apap_cfg_acqure.time_out);
			LBS_printf("apap_cfg_acqure aplikey =%s\r\n",apap_cfg_acqure.api_key);
		}
		else
		{
			LBS_printf("cm_lbs_get_attr ret=%d\r\n",ret);
		}*/
	}
	else if(lbs_platform == CM_LBS_PLAT_ONEOSPOS)
	{
		char pid[64] = {0};
		cm_lbs_oneospos_attr_t noeospos_cfg = {0};
		cm_lbs_oneospos_attr_t noeospos_cfg_acqure = {pid,0};

		// OneOS平台	；CM:LBS:platform:设备pid:请求超时时间(0-60s)time_out:是否启用邻区nearbts_enable
		
		// 配置OneOS参数
		noeospos_cfg.pid = pid;				// 设备pid
		noeospos_cfg.time_out = 60;			// 请求超时时间(0-60s)
		noeospos_cfg.nearbts_enable = 0;	// 是否启用邻区

		// LBS初始化
		ret = cm_lbs_init(lbs_platform, &noeospos_cfg);
		LBS_printf("cm_lbs_init ret=%d\r\n",ret);
		
		// 获取平台定位配置信息
		ret = cm_lbs_get_attr(lbs_platform, &noeospos_cfg_acqure);
		if(ret == 0)
		{
			LBS_printf("noeospos_cfg_acqure timeout=%d\r\n",noeospos_cfg_acqure.time_out);
			LBS_printf("noeospos_cfg_acqure pid =%s\r\n",noeospos_cfg_acqure.pid);
		}
		else
		{
			LBS_printf("cm_lbs_get_attr error,ret=%d\r\n",ret);
		}

		// LBS获取位置信息(异步)
		ret = cm_lbs_location(custom_lbs_cb, NULL);
		LBS_printf("cm_lbs_location ret=%d\r\n",ret);
		
		/*ret = cm_lbs_get_attr(lbs_platform, &noeospos_cfg_acqure);
		if(ret == 0)
		{
			LBS_printf("noeospos_cfg_acqure timeout=%d\r\n",noeospos_cfg_acqure.time_out);
			LBS_printf("noeospos_cfg_acqure pid =%s\r\n",noeospos_cfg_acqure.pid);
		}
		else
		{
			LBS_printf("cm_lbs_get_attr ret=%d\r\n",ret);
		}*/
	}

	return ret;
}
 
int custom_lbs_init(void)
{
	memset(&lbs_location, 0, sizeof(lbs_location));
	
	return 0;
}

