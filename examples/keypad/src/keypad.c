/**
 * @file        keypad.c
 * @brief       示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by cmiot4594
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "cm_keypad.h"
#include "cm_iomux.h"
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "cm_lcd.h"
#include "cm_gpio.h"
#include "cm_mem.h"
#include "cm_pm.h"
#include "cm_virt_at.h"
#include "keypad.h"
#include "cm_uart.h"
#include "stdarg.h"


/****************************************************************************
 * Private Types
 ****************************************************************************/
static uint8_t cm_keypad_sleep_flag = 0;
static osMessageQueueId_t keypad_queue = NULL;
#define OPENCPU_URAT CM_UART_DEV_0
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/
typedef struct{
    int msg_type;
} uart_event_msg_t;

static cm_pm_cfg_t test_pm_cfg[CM_PM_MAX_NUM];


/* 从测试串口打印字符串 */
void __cm_uart_printf(char *str, ...)
{
    char s[600] = {0}; //This needs to be large enough to store the string TODO Change magic number
    va_list args;
    int len;
    
    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start(args, str);
    len = vsnprintf((char*)s, 600, str, args);
    va_end(args);
    cm_uart_write(OPENCPU_URAT, s, len, 1000);
}

/* 若要修改测试串口可在cm_common.h中修改宏定义 */
void __cm_uart_init(void)
{
    int32_t ret = -1;

    /* 配置参数 */
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8, 
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE, 
        CM_UART_FLOW_CTRL_NONE, 
        CM_UART_BAUDRATE_9600,
        0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
        0,   //环形缓存区大小按照默认配置8k
        0,
        0,
    };

    /* 配置引脚复用 */
    cm_iomux_set_pin_func(OPENCPU_UART0TX_IOMUX);
    cm_iomux_set_pin_func(OPENCPU_UART0RX_IOMUX);

    /* 开启串口 */
    ret = cm_uart_open(OPENCPU_URAT, &config);
    if (ret != 0)
    {
        cm_log_printf(0, "uart init err,ret=%d\n", ret);
        return;
    }

}

static void __cm_pm_init(void)
{
	int i=0;
    
	for(i = 0;i < CM_PM_MAX_NUM;i++)
	{
		test_pm_cfg[i].cb_enter = NULL;
		test_pm_cfg[i].cb_exit = NULL;
	}
}

static void __cm_pm_cb_register(int id,cm_pm_event_cb_t enter,cm_pm_exit_cb_t exit)
{
	if(id >= CM_PM_MAX_NUM)
	{
		cm_log_printf(0,"invalid pm id-%d\r\n",id);
		return;
	}
    
	if(!enter || !exit)
	{
		cm_log_printf(0,"pm func cannot be null,enter(0x%x),exit(0x%x)\r\n",enter,exit);
		return;
	}
    
	if(test_pm_cfg[id].cb_enter || test_pm_cfg[id].cb_exit)
	{
		cm_log_printf(0,"pm id-%d already exist\r\n",id);
		return;
	}
    
	test_pm_cfg[id].cb_exit = exit;
	test_pm_cfg[id].cb_enter = enter;
}

static void __cm_pm_cb_unregister(int id)
{
	if(id >= CM_PM_MAX_NUM)
	{
		cm_log_printf(0,"invalid pm id-%d\r\n",id);
		return;
	}
    
	test_pm_cfg[id].cb_exit = NULL;
	test_pm_cfg[id].cb_exit = NULL;
}

static void __cm_keypad_dell(int key)
{
    switch (key)
    {
    case KEY_00:
        __cm_uart_printf("00\n");
        cm_pm_work_unlock();
        break;
    case KEY_01:
        __cm_uart_printf("01\n");
        cm_pm_work_unlock();
        break;
    case KEY_10:
        __cm_uart_printf("10\n");
        cm_pm_work_lock();
        break;
    case KEY_11:
        __cm_uart_printf("11\n");
        cm_pm_work_lock();
        break;
    default:
        break;
    }
}

static void __cm_keypad_callback(cm_keypad_map_t key, cm_keypad_event_e event)
{
    cm_log_printf(0, "keypad key:%x\n", key);
    cm_log_printf(0, "keypad event:%d\n", event);
    if(cm_keypad_sleep_flag == 1)
    {
        cm_keypad_sleep(0);//按键回调函数执行时，按键才退出休眠
        cm_keypad_sleep_flag = 0;
    }
    if(event == CM_KEY_EVENT_PRESS)
    {
        osMessageQueuePut(keypad_queue, &key, 0, 0);
    }
}

static void __cm_keypad_pm_enter(void)
{
    cm_keypad_sleep(1);//切换按键中断为休眠中断
    cm_keypad_sleep_flag = 1;
}

static void __cm_keypad_pm_exit(uint32_t reason)
{
	if(!(reason & WKP_SOURCE_KEYPAD))
	{
        ;
	}
}

void __cm_pm_enty_callback(void)
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

void __cm_pm_exit_callback(uint32_t src)
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

/****************************************************************************
 * Public Functions
****************************************************************************/


static void __cm_keypad_fuc_init(void)
{
    cm_keypad_kp_mkin_e col[2] = {0,1};
    cm_keypad_kp_mkout_e row[2] = {0,1};
    cm_keypad_cfg_t cm_keypad_cfg = {0};
    
    cm_keypad_cfg.cm_col = col;
    cm_keypad_cfg.cm_row = row;
    cm_keypad_cfg.cm_col_len = 2;
    cm_keypad_cfg.cm_row_len = 2;

    cm_iomux_set_pin_func(OPENCPU_KEYIN_MUX0);
    cm_iomux_set_pin_cmd(OPENCPU_KEYIN_PIN0,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

    cm_iomux_set_pin_func(OPENCPU_KEYOUT_MUX0);
    cm_iomux_set_pin_cmd(OPENCPU_KEYOUT_PIN0,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

    cm_iomux_set_pin_func(OPENCPU_KEYIN_MUX1);
    cm_iomux_set_pin_cmd(OPENCPU_KEYIN_PIN1,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

    cm_iomux_set_pin_func(OPENCPU_KEYOUT_MUX1);
    cm_iomux_set_pin_cmd(OPENCPU_KEYOUT_PIN1,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);

    cm_keypad_register(__cm_keypad_callback);
    cm_keypad_config(&cm_keypad_cfg);
    __cm_pm_cb_register(0,__cm_keypad_pm_enter,__cm_keypad_pm_exit);//需要在初始化cm_pm_init函数之后执行，否则会被清除

}


/** app线程入口*/
static void __task_loop(void *arg)
{
    (void)arg;
    __cm_uart_init();
    cm_pm_cfg_t pm_cfg = {__cm_pm_enty_callback,__cm_pm_exit_callback};
    cm_pm_init(pm_cfg); //初始化pm模块
    __cm_pm_init();     //初始化自定义pm模块回调函数，可在此注册自定义回调函数，或者直接修改cm_pm_cfg_t pm_cfg = {__cm_pm_enty_callback,__cm_pm_exit_callback};中的回调函数
    __cm_keypad_fuc_init();

    if(0 == cm_keypad_init())
    {
        cm_log_printf(0, "cm_keypad_init OK\n");
    }
    else
    {
        cm_log_printf(0, "cm_keypad_init FAILED\n");
    }
    cm_pm_work_unlock();

    keypad_queue = osMessageQueueNew(10, sizeof(cm_keypad_map_t), NULL);
    int keypad_event = 0;
	while(1) 
    {
		if(osMessageQueueGet(keypad_queue, &keypad_event, NULL, osWaitForever) == osOK)
        {
            cm_log_printf(0, "keypad_event:%x\n", keypad_event);
            __cm_keypad_dell(keypad_event);
        }
		osDelay(5);
	}
    
}

/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;

    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)__task_loop, 0, &app_task_attr);
    return 0;
}