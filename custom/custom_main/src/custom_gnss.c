#include "custom_gnss.h"
#include "custom_track.h"
#include "custom_system.h"
#include "custom_network.h"

uint8_t Agnss_update = 0;
uint8_t Gnss_working = 0;

gnss_location_info_t gnss_location;

void custom_agnss_update_callback(cm_agnss_update_result_e mode, const char *update_time, uint32_t size)
{
	GNSS_printf("%s: mode=%d,update_time=%s,size=%d", __func__, mode, update_time, size);
}

void custom_gnss_nmea_callback(const char *nmea, uint32_t len)
{	
	char gsv_total[8] = {0}, gsv_sn[8] = {0}, visual_satellite[8] = {0};
	uint8_t gp_satellite = 0,gb_satellite = 0,gl_satellite = 0;
	uint8_t total_satellite = 0;
	char *gsv_nmea;
	
	//GNSS_printf("%s: %s", __func__, (char *)nmea);

	// 计算可视卫星数
	gsv_nmea = strstr(nmea, "$GPGSV");
	if(gsv_nmea != NULL)
	{
		if(sscanf((char *)gsv_nmea, "$GPGSV,%[^,],%[^,],%[^,],", gsv_total, gsv_sn, visual_satellite) == 3)
		{
			if(strlen(visual_satellite) > 0)
			{
				gp_satellite = atoi(visual_satellite);
			}
		}
	}

	gsv_nmea = strstr(nmea, "$GBGSV");
	if(gsv_nmea != NULL)
	{
		if(sscanf((char *)gsv_nmea, "$GBGSV,%[^,],%[^,],%[^,],", gsv_total, gsv_sn, visual_satellite) == 3)
		{
			if(strlen(visual_satellite) > 0)
			{
				gb_satellite = atoi(visual_satellite);
			}
		}
	}

	gsv_nmea = strstr(nmea, "$GLGSV");
	if(gsv_nmea != NULL)
	{
		if(sscanf((char *)gsv_nmea, "$GLGSV,%[^,],%[^,],%[^,],", gsv_total, gsv_sn, visual_satellite) == 3)
		{
			if(strlen(visual_satellite) > 0)
			{
				gl_satellite = atoi(visual_satellite);
			} 
		}
	}
	
	total_satellite = gp_satellite + gb_satellite + gl_satellite;
	
	gnss_location.vsat = total_satellite;

	//GNSS_printf("gp_satellite=%d,gb_satellite=%d,gl_satellite=%d,total_satellite=%d", gp_satellite, gb_satellite, gl_satellite, total_satellite);
	
	
	/*
	$GNRMC,065336.000,A,2308.44865,N,11335.43709,E,0.390,359.654,081125,,,A,U*0F 
	$GNGGA,065336.000,2308.44865,N,11335.43709,E,1,09,7.333,16.8,M,-4.5,M,,*67 
	$GNGLL,2308.44865,N,11335.43709,E,065336.000,A,A*4C 
	$GNGSA,A,3,27,8,199,,,,,,,,,,9.773,7.333,6.459,1*3D 
	$GNGSA,A,3,22,13,38,8,,,,,,,,,9.773,7.333,6.459,4*05 
	$GNGSA,A,3,69,68,,,,,,,,,,,9.773,7.333,6.459,2*33 
	$GPGSV,3,1,9,8,15,197,33,27,40,177,32,32,18,144,22,199,59,148,25,1*53 
	$GPGSV,3,2,9,16,66,290,,3,26,264,,4,34,321,,26,52,16,,1*60 
	$GPGSV,3,3,9,28,30,74,,1*57 
	$GBGSV,5,1,19,56,,,20,22,31,176,30,8,22,202,23,13,28,217,29,1*74 
	$GBGSV,5,2,19,35,24,153,9,38,18,192,30,21,59,249,21,40,,,23,1*7A 
	$GBGSV,5,3,19,1,45,124,,2,48,237,,3,62,187,,4,30,113,,1*7A 
	$GBGSV,5,4,19,5,24,256,,6,53,353,,7,48,183,,9,46,328,,1*7B 
	$GBGSV,5,5,19,10,37,194,,26,38,298,,29,26,87,,1*7B 
	$GLGSV,2,1,8,68,20,167,30,69,67,202,30,81,16,83,8,82,12,130,13,1*45 
	$GLGSV,2,2,8,80,42,315,9,73,11,264,,70,32,323,,79,28,23,,1*45 
	$GNVTG,359.654,T,,M,0.390,N,0.724,K,A*10 
	
	[GNSS]latitude=23.140810 
	[GNSS]longitude=113.590622 
	[GNSS]hdop=7.333000 
	[GNSS]altitude=16.799999 
	[GNSS]fix=3 
	[GNSS]cog=359.653992 
	[GNSS]spkm=0.724000 
	[GNSS]spkn=0.390000 
	[GNSS]nsat=9 
	*/
}

void custom_gnss_rsp_callback(const char *data, uint32_t len)
{
	GNSS_printf("%s: %s", __func__, (char *)data);
}

void custom_gnss_getlocateinfo(void)
{
	cm_gnss_location_info_t location;

	memset(&location, 0, sizeof(location));
	
	if(0 == cm_gnss_get_location_info(&location))
	{
		gnss_location.latitude = location.latitude;
		gnss_location.longitude = location.longitude;
		gnss_location.hdop = location.hdop;
		gnss_location.altitude = location.altitude;
		gnss_location.fix = location.fix;
		gnss_location.cog = location.cog;
		gnss_location.spkm = location.spkm;
		gnss_location.spkn = location.spkn;
		gnss_location.nsat = location.nsat;
	}
}

int	custom_gnss_enable(uint8_t enable)
{
	uint32_t nema_mask = 0xFFFF;
	//uint32_t nema_cycle = 10;

	if(enable == 1)
	{
		if(Gnss_working != 1)
		{
			Gnss_working = 1;
			Agnss_update = 0;
			
			GNSS_printf("%s: 1", __func__);

			// 配置GNSS
			cm_gnss_config(CM_GNSS_CONFIG_TYPE_NMEA_MASK, &nema_mask);			// NMEA输出：所有语句
			//cm_gnss_config(CM_GNSS_CONFIG_TYPE_NMEA_CYCLE, &nema_cycle);		// NMEA输出周期: 10秒

			// 打开GNSS
			if(custom_network_IsPDPActive())
			{
				cm_gnss_open(CM_GNSS_TYPE_GPS|CM_GNSS_TYPE_BDS|CM_GNSS_TYPE_QZSS|CM_GNSS_TYPE_GLO, CM_AGNSS_ENABLE);
			}
			else
			{
				cm_gnss_open(CM_GNSS_TYPE_GPS|CM_GNSS_TYPE_BDS|CM_GNSS_TYPE_QZSS|CM_GNSS_TYPE_GLO, CM_AGNSS_DISABLE);
			}
			
			// 注册NMEA数据上报
			cm_gnss_nmea_all_callback_regist(custom_gnss_nmea_callback);

			// 注册RAW数据上报
			//cm_gnss_rawdata_callback_regist(custom_gnss_rsp_callback);
			
			GNSS_printf("%s: init finish!", __func__);
		}
	}
	else
	{
		if(Gnss_working != 0)
		{
			Gnss_working = 0;

			GNSS_printf("%s: 0", __func__);

			// 关闭GNSS
			cm_gnss_close();
		}
	}
	
	return 0;
}

void custom_gnss_task(void *p)
{
	uint32_t count = 0;

	custom_gnss_enable(1);

	while(1)
	{
		count++;
		osDelay(ONE_SECONED);  //每1s检查一下

		if(Gnss_working == 1)
		{
			// 获取定位数据
			custom_gnss_getlocateinfo();	
		
			// 开启AGNSS
			if(custom_network_IsPDPActive())
			{
				if(Agnss_update == 0)
				{
					Agnss_update = 1;
					cm_agnss_data_start_update(custom_agnss_update_callback);
				}
			}	

			if((count % 10) == 0)
			{
				GNSS_printf("latitude=%lf",gnss_location.latitude);				// 纬度,float,度,默认值为0
				GNSS_printf("longitude=%lf",gnss_location.longitude);			// 经度,float,度,默认值为0
				GNSS_printf("hdop=%lf",gnss_location.hdop);						// 水平精度因子,float,度,保留1位小数,默认值为0
				GNSS_printf("altitude=%lf",gnss_location.altitude);				// 海拔高度,float,米,保留1位小数,默认值为0
				GNSS_printf("fix=%d",gnss_location.fix);						// 定位类型
				GNSS_printf("spkm=%lf",gnss_location.spkm);						// 水平运动速度,float,单位Km/h,默认值为0
				GNSS_printf("nsat=%d",gnss_location.nsat);						// 参与定位的卫星数,uint8_t,默认值为0
			}		
		}
	}
}

int custom_gnss_init(void)
{
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "gnss_task";
	app_task_attr.stack_size = 1024 * 4;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_gnss_task, 0, &app_task_attr);

	return 0;
}

