/*********************************************************
 *  @file    cm_demo_gpio.c
 *  @brief   OpenCPU GPIO示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2021/08/30
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include "stdlib.h"
#include "cm_demo_keypad.h"
#include "cm_keypad.h"
#include "cm_common.h"
#include "cm_iomux.h"
#include "cm_demo_uart.h"
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "cm_demo_pm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define  WKP_SOURCE_KEYPAD 1<<21

/****************************************************************************
 * Private Types
 ****************************************************************************/
static uint8_t cm_keypad_sleep_flag = 0;

 
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Data
 ****************************************************************************/
 
 
/****************************************************************************
 * Private Functions
 ****************************************************************************/
static void cm_keypad_callback(cm_keypad_map_t key, cm_keypad_event_e event)
{
    if(cm_keypad_sleep_flag == 1)
    {
        cm_keypad_sleep(0);//按键回调函数执行时，按键才退出休眠
        cm_keypad_sleep_flag = 0;
    }
}

static void cm_keypad_pm_enter(void)
{
    cm_keypad_sleep(1);//切换按键中断为休眠中断
    cm_keypad_sleep_flag = 1;
}

static void cm_keypad_pm_exit(uint32_t reason)
{
	if(!(reason & WKP_SOURCE_KEYPAD))
	{
        ;
	}
}
/****************************************************************************
 * Public Functions
****************************************************************************/
void cm_test_keypad(unsigned char **cmd, int len)
{
    cm_keypad_kp_mkin_e col[1] = {0};
    cm_keypad_kp_mkout_e row[1] = {0};
    cm_keypad_cfg_t cm_keypad_cfg = {0};
    
    if( (cmd == NULL) || (len !=2 ))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    cm_keypad_cfg.cm_col = col;
    cm_keypad_cfg.cm_row = row;
    cm_keypad_cfg.cm_col_len = 1;
    cm_keypad_cfg.cm_row_len = 1;
    
    cm_iomux_set_pin_func(OPENCPU_TEST_KEYIN_IOMUX);
    cm_iomux_set_pin_cmd(OPENCPU_TEST_KEYIN_PIN,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);
    cm_iomux_set_pin_func(OPENCPU_TEST_KEYOUT_IOMUX);
    cm_iomux_set_pin_cmd(OPENCPU_TEST_KEYOUT_PIN,CM_IOMUX_PINCMD0_SLEEP,CM_IOMUX_PINCMD0_FUNC1_SLEEP_DIR);
    
    cm_keypad_register(cm_keypad_callback);
    cm_keypad_config(&cm_keypad_cfg);
    cm_test_pm_cb_register(0,cm_keypad_pm_enter,cm_keypad_pm_exit);//需要在初始化cm_test_pm_init函数之后执行，否则会被清除
    
    if(0 == cm_keypad_init())
    {
        cm_demo_printf("cm_keypad_init OK\n");
    }
    else
    {
        cm_demo_printf("cm_keypad_init FAILED\n");
    }
}
