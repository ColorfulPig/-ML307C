#ifndef __CUSTOM_NETWORK_H__
#define __CUSTOM_NETWORK_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stdarg.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_pm.h"
#include "cm_modem.h"

typedef enum
{
    NETWORK_EVENT_NO_SIM,
    NETWORK_EVENT_SIM_READY,
    NETWORK_EVENT_NO_REGISTER,
    NETWORK_EVENT_REGISTER_READY, 
    NETWORK_EVENT_PDP_ACTIVED,
    NETWORK_EVENT_PDP_ACTIVE_FAIL,   
    NETWORK_EVENT_PDP_DEACTIVED,			// 失活
    NETWORK_EVENT_MAX
} custom_network_event_e;

#define NETWORK_INIT_WAIT_SIM_READY			10			// 初始化阶段-等待SIM卡识别的超时时间（秒）
#define NETWORK_INIT_WAIT_REGISTER_TIMEOUT	300			// 初始化阶段-等待网络注册的超时时间（秒）
#define NETWORK_INIT_WAIT_ACTIVE_TIMEOUT	60    		// 初始化阶段-等待网络激活的超时时间（秒）

#define NETWORK_CHECK_WAIT_SIM_READY		3			// 检测阶段-等待SIM卡识别的超时时间（秒）
#define NETWORK_CHECK_WAIT_REGISTER_TIMEOUT	3			// 检测阶段-等待网络注册的超时时间（秒）
#define NETWORK_CHECK_WAIT_ACTIVE_TIMEOUT	3    		// 检测阶段-等待网络激活的超时时间（秒）

typedef struct
{
	uint8_t SimCard;
	uint8_t	Register;
	uint8_t	PDPActive;
	uint8_t	CSQ;
} custom_network_data_t; 

#define	custom_network_IsSimCardReady()			(network_state.SimCard == 1)
#define	custom_network_IsRegister()				(network_state.Register == 1)
#define	custom_network_IsPDPActive()			(network_state.PDPActive == 1)
#define	custom_network_getCSQ()					(network_state.CSQ)

extern custom_network_data_t network_state;

int custom_network_init(void);
	

#endif


