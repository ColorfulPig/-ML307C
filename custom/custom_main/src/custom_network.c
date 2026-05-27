#include "custom_network.h"
#include "custom_track.h"
#include "custom_system.h"
#include "custom_led.h"

custom_network_data_t	network_state;

static int	sim_check_times = 0;
static int	register_check_times = 0;
static int	active_check_times = 0;

//网络状态通知回调
void custom_network_event_callback(custom_network_event_e state)
{
	switch (state)
	{
		/* 无法识别SIM卡 */
		case NETWORK_EVENT_NO_SIM:
		{
			network_state.SimCard = 0;
			Network_printf("NETWORK_EVENT_NO_SIM");
			
			sim_check_times++;
			if(sim_check_times >= 10)
			{
				Network_printf("sim is not ready,reboot...");
				osDelay(10);
				cm_pm_reboot();
			}
			break;
		}		
		/* 识别到SIM卡 */
		case NETWORK_EVENT_SIM_READY:
		{
			network_state.SimCard = 1;

			sim_check_times = 0;
			Network_printf("NETWORK_EVENT_SIM_READY");
			break;
		}

		/* 网络未注册 */
		case NETWORK_EVENT_NO_REGISTER:
		{
			network_state.Register = 0;
			Network_printf("NETWORK_EVENT_NO_REGISTER");
			
			register_check_times++;
			if(register_check_times >= 10)
			{
				Network_printf("register is not ready,reboot...");
				osDelay(10);
				cm_pm_reboot();
			}
			break;
		}
		/* 网络已注册 */
		case NETWORK_EVENT_REGISTER_READY:
		{
			if(network_state.Register != 1)
			{
				network_state.Register = 1;
				custom_led_setState(LED_STATE_READY);
			}
			
			register_check_times = 0;
			Network_printf("NETWORK_EVENT_REGISTER_READY");
			break;
		}
    
		/* 网络激活失败 */
		case NETWORK_EVENT_PDP_ACTIVE_FAIL:
		{
			network_state.PDPActive = 0;
			Network_printf("NETWORK_EVENT_PDP_ACTIVE_FAIL");

			active_check_times++;
			if(active_check_times >= 3)
			{
				Network_printf("The network active fail,reboot....");
				osDelay(10);
				cm_pm_reboot();
			}
			break;
		}		
		/* 网络激活成功 */
		case NETWORK_EVENT_PDP_ACTIVED:
		{
			network_state.PDPActive = 1;

			active_check_times = 0;
			Network_printf("NETWORK_EVENT_PDP_ACTIVED");
			break;
		}
		
		/* 网络失活，需要重新激活 */
		case NETWORK_EVENT_PDP_DEACTIVED:
		{
			network_state.PDPActive = 0;

			Network_printf("NETWORK_EVENT_PDP_DEACTIVED");			
			Network_printf("The network has become inactive,reboot...");
			osDelay(10);
			cm_pm_reboot();
			break;
		}
		default:
			break;
	}
}

bool custom_network_check_sim_ready(uint16_t timeout_s)
{
	uint16_t retry = 0;
	while (retry < timeout_s)
	{
		if(cm_modem_get_cpin() >= 0)
		{
			return true;
		}
		retry++;
		osDelay(ONE_SECONED);
	}
	
	return false;
}

bool custom_network_check_register(uint16_t timeout_s)
{
	uint16_t retry = 0;
	cm_cereg_state_t cereg_state = {0};
	
	while (retry < timeout_s)
	{
		if (cm_modem_get_cereg_state(&cereg_state) == 0 && (cereg_state.state == 1 || cereg_state.state == 5))
		{
			return true;
		}
		retry++;
		osDelay(ONE_SECONED);
	}
	
	return false;
}

bool custom_network_check_active(uint16_t timeout_s)
{
	uint16_t retry = 0;
	
	while (retry < timeout_s)
	{
		if (cm_modem_get_pdp_state(1) == 1)
		{
			return true;
		}
		retry++;
		osDelay(ONE_SECONED);
	}
	
	return false;
}

// 网络维护线程
void custom_network_task(void *p)
{
	memset(&network_state, 0, sizeof(network_state));

	// 1.检查CFUN是否打开(不打开cfun，sim卡无法读到) 
	if(cm_modem_get_cfun() != 1)
	{
		cm_modem_set_cfun(1);
	}

	// 2.检查SIM卡 
	while(!custom_network_check_sim_ready(NETWORK_INIT_WAIT_SIM_READY))
	{
		custom_network_event_callback(NETWORK_EVENT_NO_SIM);
	}
	custom_network_event_callback(NETWORK_EVENT_SIM_READY);

	// 3.设置apn，通用卡可跳过该步骤
	/* 专网卡请使用虚拟AT发送以下指令设置APN: AT+CGDCONT=1,"IPV4V6","SIM卡APN名称" */

	// 4.检查基站的注册状态
	while(!custom_network_check_register(NETWORK_INIT_WAIT_REGISTER_TIMEOUT))
	{
		custom_network_event_callback(NETWORK_EVENT_NO_REGISTER);
	}
	custom_network_event_callback(NETWORK_EVENT_REGISTER_READY);
	Network_printf("The network has been registered successfully");

	// 5.检查网络的激活状态
	while(!custom_network_check_active(NETWORK_INIT_WAIT_ACTIVE_TIMEOUT))
	{
		custom_network_event_callback(NETWORK_EVENT_PDP_ACTIVE_FAIL);
	}
	custom_network_event_callback(NETWORK_EVENT_PDP_ACTIVED);
	Network_printf("The network has been actived successfully");

	// 6.开始网络状态监测	
	while(1)
	{		
		osDelay(ONE_SECONED * 60); 					// 每60s检查一下网络状态，用户可自行更改
		
		// 检查SIM卡 
		if(custom_network_check_sim_ready(NETWORK_CHECK_WAIT_SIM_READY))
		{
			custom_network_event_callback(NETWORK_EVENT_SIM_READY);
		}
		else
		{
			custom_network_event_callback(NETWORK_EVENT_NO_SIM);
		}
		
		// 检查基站的注册状态
		if(custom_network_check_register(NETWORK_CHECK_WAIT_REGISTER_TIMEOUT))
		{
			custom_network_event_callback(NETWORK_EVENT_REGISTER_READY);
		}
		else
		{
			custom_network_event_callback(NETWORK_EVENT_NO_REGISTER);
		}

		// 检查网络的激活状态
		if(custom_network_check_active(NETWORK_CHECK_WAIT_ACTIVE_TIMEOUT))
		{
			custom_network_event_callback(NETWORK_EVENT_PDP_ACTIVED);
		}
		else
		{
			custom_network_event_callback(NETWORK_EVENT_PDP_DEACTIVED);		// 失活
		}
		
		// 检测信号质量(GC)
		char s_rssi[8],s_ber[8];
		if(cm_modem_get_csq(s_rssi, s_ber) == 0)
		{
			uint8_t rssi = atoi(s_rssi);
			uint8_t ber = atoi(s_ber);
			
			Network_printf("CSQ: %d,%d", rssi, ber);
			network_state.CSQ = rssi;
			if(network_state.CSQ == 99)	network_state.CSQ = 0;
		}	
		/*
		// 检测信号质量(DC)
		uint8_t rssi,ber;
		if(cm_modem_get_csq(&rssi, &ber) == 0)
		{
			Network_printf("CSQ: %d,%d", rssi, ber);
			network_state.CSQ = rssi;
			if(network_state.CSQ == 99)	network_state.CSQ = 0;
		}*/
	}
}

int custom_network_init(void)
{
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "network_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_network_task, 0, &app_task_attr);

	return 0;
}

