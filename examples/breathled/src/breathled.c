/**
 * @file        breathled.c
 * @brief       使用pwm驱动led，实现呼吸灯效果，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangxw
 */


#include <string.h>
#include "cm_os.h"
#include "cm_pwm.h"
#include "cm_modem.h"
#include "cm_sys.h"
#include "cm_iomux.h"


#define BREATH_LED_MSG_COUNT      20
#define BREATH_LED_PWM_PERIOD     1000000


typedef struct {
    int msg_id;
    int msg_type;
} breath_led_msg_t;


static osMessageQueueId_t s_breath_msg = NULL;
static osTimerId_t s_breath_timer = NULL;
static bool s_breath_flag = true;


static void __timer_cb(void *arg)
{
    breath_led_msg_t msg = {0};
    osMessageQueuePut(s_breath_msg, &msg, 0, 0);
}


static void __task_loop(void *arg)
{
    while(1)
    {//驻网成功前呼吸灯熄灭，
        if(cm_modem_get_pdp_state(1) == 1)
        {
            break;
        }

        osDelay(200);
    }

    breath_led_msg_t msg = {0};
    static int period_h = 0;
    s_breath_timer = osTimerNew(__timer_cb, osTimerPeriodic, NULL, NULL);
    osTimerStart(s_breath_timer, 20);
    cm_iomux_set_pin_func(CM_IOMUX_PIN_74, CM_IOMUX_FUNC_FUNCTION1); //初始化pwm0功能
    cm_pwm_set_clk(CM_PWM_DEV_0, CM_PWM_CLK_32K); //配置时钟源

    while(1)
    {
        osMessageQueueGet(s_breath_msg, &msg, 0, osWaitForever);
        s_breath_flag = period_h == BREATH_LED_PWM_PERIOD ? false : s_breath_flag; //确认呼吸方向
        s_breath_flag = period_h == 0 ? true : s_breath_flag;

        cm_pwm_open_ns(CM_PWM_DEV_0, BREATH_LED_PWM_PERIOD, period_h);
        cm_log_printf(0, "breath param:flag=%d, period-h=%d", s_breath_flag, period_h);

        if(s_breath_flag) //调节占空比
        {
            period_h += BREATH_LED_PWM_PERIOD / 10;
        }
        else
        {
            period_h -= BREATH_LED_PWM_PERIOD / 10;
        }
    }
}


/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    s_breath_msg = osMessageQueueNew(BREATH_LED_MSG_COUNT, sizeof(breath_led_msg_t), NULL);
    osThreadNew((osThreadFunc_t)__task_loop, 0, &app_task_attr);
    return 0;
}
