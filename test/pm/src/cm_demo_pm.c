/*********************************************************
 *  @file    cm_demo_pm.c
 *  @brief   OpenCPU PM示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/
#include "stdlib.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "cm_demo_pm.h"
#include "cm_os.h"
#include "cm_demo_uart.h"
#include "cm_keypad.h"

#define CM_PM_MAX_NUM 10


static cm_pm_cfg_t test_pm_cfg[CM_PM_MAX_NUM];

void cm_test_pm_init(void)
{
	int i=0;
    
	for(i = 0;i < CM_PM_MAX_NUM;i++)
	{
		test_pm_cfg[i].cb_enter = NULL;
		test_pm_cfg[i].cb_exit = NULL;
	}
}

void cm_test_pm_cb_register(int id,cm_pm_event_cb_t enter,cm_pm_exit_cb_t exit)
{
	if(id >= CM_PM_MAX_NUM)
	{
		cm_demo_printf("invalid pm id-%d\r\n",id);
		return;
	}
    
	if(!enter || !exit)
	{
		cm_demo_printf("pm func cannot be null,enter(0x%x),exit(0x%x)\r\n",enter,exit);
		return;
	}
    
	if(test_pm_cfg[id].cb_enter || test_pm_cfg[id].cb_exit)
	{
		cm_demo_printf("pm id-%d already exist\r\n",id);
		return;
	}
    
	test_pm_cfg[id].cb_exit = exit;
	test_pm_cfg[id].cb_enter = enter;
}

void cm_test_pm_cb_unregister(int id)
{
	if(id >= CM_PM_MAX_NUM)
	{
		cm_demo_printf("invalid pm id-%d\r\n",id);
		return;
	}
    
	test_pm_cfg[id].cb_exit = NULL;
	test_pm_cfg[id].cb_exit = NULL;
}

void cm_test_pm_reboot(unsigned char **cmd,int len)
{
    if( (cmd == NULL) || (len !=2 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    cm_demo_printf("[REBOOT]rebooting...\n");
    cm_pm_reboot();
}

void cm_test_pm_poweroff(unsigned char **cmd,int len)
{
    if( (cmd == NULL) || (len !=2 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    cm_demo_printf("[POWEROFF]poweroff...\n");
    cm_pm_poweroff();
}

void cm_test_pm_power_on_reason(unsigned char **cmd,int len)
{
    
    if( (cmd == NULL) || (len !=2 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    cm_demo_printf("[PM]power on reason:%d\n", cm_pm_get_power_on_reason());
}

void cm_pm_enty_callback(void)
{
	int i;
    
	for(i = 0;i < CM_PM_MAX_NUM;i++)
	{
		if(test_pm_cfg[i].cb_enter != NULL)
		{
            test_pm_cfg[i].cb_enter();
		}
	}
}

void cm_pm_exit_callback(uint32_t src)
{
	int i;
    
	for(i = 0;i < CM_PM_MAX_NUM;i++)
	{
		if(test_pm_cfg[i].cb_exit != NULL)
		{
            test_pm_cfg[i].cb_exit(src);
		}
	}
}
void cm_test_sleep_or_wakeup_enable(unsigned char **cmd,int len)
{
    uint8_t mode = 0;
    cm_pm_cfg_t pm_cfg = {cm_pm_enty_callback,cm_pm_exit_callback};

    if( (cmd == NULL) || (len !=3 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    mode = atoi((char *)cmd[2]);
    cm_pm_init(pm_cfg);
    cm_test_pm_init();
    
    if(mode == 1)
    {
        cm_pm_work_unlock();
    }
    else if(mode == 2)
    {
        cm_pm_work_lock();
    }
    else
    {
        cm_demo_printf("[PM]dormant mode(%d) is error...\n",mode);
    }
}

void cm_pm_powerkey_callback(cm_powerkey_event_e event)
{
    if(CM_POWERKEY_EVENT_RELEASE == event)
    {
        cm_demo_printf("[PM]powerkey CM_POWERKEY_EVENT_RELEASE\n");
    }
    else
    {
        cm_demo_printf("[PM]powerkey CM_POWERKEY_EVENT_PRESS\n");
    }
}

void cm_test_pm_powerkey(unsigned char **cmd,int len)
{
    uint8_t mode = 0;
    
    if( (cmd == NULL) || (len !=3 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    mode = atoi((char *)cmd[2]);
    
    if(mode == 0)
    {
        cm_pm_powerkey_regist_callback(NULL);
    }
    else
    {
        cm_pm_powerkey_regist_callback(cm_pm_powerkey_callback);
    }
}



