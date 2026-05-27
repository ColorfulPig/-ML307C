/**
 * @file        cm_lowpower_demo.c
 * @brief       本示例展示模组的唤醒和休眠功能，并使用一个GPIO的中断功能唤醒和休眠
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zhangwencheng
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cm_os.h"
#include "cm_sys.h"
#include "cm_modem.h"
#include "cm_pm.h"
#include "cm_gpio.h"
#include "cm_iomux.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
#define CM_LOWPOWER_DEMO_GPIO CM_GPIO_NUM_9             // 唤醒的GPIO引脚，范围0-29，本示例中GPIO引脚为PIN9脚，在模组外围，方便做简易测试
#define CM_LOWPOWER_DEMO_WAKEUP_LEVEL CM_GPIO_LEVEL_LOW // 唤醒的触发电平，这里设置的是低电平,即下降沿唤醒，上升沿休眠
#define CM_LOWPOWER_DEMO_WAKEUP_PULL CM_GPIO_PULL_UP    // 默认上下拉，如果唤醒电平是低，则配置为高，如果唤醒是高，则配置为低
#define CM_LOWPOWER_DEMO_DEBOUCE_MS 100                 // GPIO消抖时间

static osThreadId_t CM_LOWPOWER_DEMO_MAIN_TASKHANDLE = NULL;    // 低功耗 DEMO 测试主线程句柄
static osMessageQueueId_t CM_LOWPOWER_DEMO_QUEUE_HANDLE = NULL; // 低功耗 DEMO 消息队列，当模组进入/退出低功耗时，发送相关消息
static bool cm_lowpower_demo_flag = 0;                          // 休眠锁标志位，1-表示锁定 0-表示解锁，避免直接调用cm_pm_lock/unlock 导致多次锁定/多次解锁。

static uint32_t g_cm_lowpower_demo_enter_count = 0; // 全局变量 进入休眠的次数
static uint32_t g_cm_lowpower_demo_exit_count = 0;  // 全局变量 退出休眠的次数

/*GPIO消抖相关*/
static osMessageQueueId_t s_gpio_irq_queue = NULL;
static cm_gpio_level_e s_gpio_last_level = CM_GPIO_LEVEL_LOW;
static osTimerId_t s_gpio_debouce_timer = NULL;

typedef enum
{
    CM_LOWPOWER_DEMO_EVENT_ENTRY = 0,
    CM_LOWPOWER_DEMO_EVENT_EXIT,
} CM_LOWPOWER_DEMO_EVENT;

/* USBT消息内容结构体 */
typedef struct
{
    CM_LOWPOWER_DEMO_EVENT event;
} cm_lowpower_demo_data_t;

/**
 *  @brief 低功耗-进入休眠回调函数
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details
 */
void cm_lowpower_demo_entry_callback(void)
{
    // 进入休眠后该接口会被反复执行，不适用于应用开发，因此不写入任何程序
}

/**
 *  @brief 低功耗-退出休眠回调函数
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details
 */
void cm_lowpower_demo_exit_callback(uint32_t src)
{
    // 进入休眠后该接口会被反复执行，不适用于应用开发，因此不写入任何程序
}

/**
 *  @brief GPIO消抖后定时器回调函数
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 当定时器到达后，若此时电平与触发中断电平时相同，则认为是有效的中断，根据中断后的电平来判断是唤醒还是休眠
 *           
 */
void debouce_gpio_timer_cb(void *param)
{
    cm_gpio_level_e gpio_level = 0;
    cm_gpio_get_level(CM_GPIO_NUM_9, &gpio_level);
    if (gpio_level == s_gpio_last_level)
    {
        s_gpio_last_level = gpio_level;
        cm_lowpower_demo_data_t lowpower_data = {0};
        cm_gpio_level_e gpio_level_temp = 0;
        if (cm_gpio_get_level(CM_LOWPOWER_DEMO_GPIO, &gpio_level_temp) < 0) // 消抖结束，判断电平，根据唤醒电平执行唤醒或休眠操作
        {
            return;
        }
        if (gpio_level_temp == CM_LOWPOWER_DEMO_WAKEUP_LEVEL) // 如果获取到的电平等于唤醒电平 则通知退出休眠，锁定睡眠
        {
            lowpower_data.event = CM_LOWPOWER_DEMO_EVENT_EXIT;
            if (osOK != osMessageQueuePut(CM_LOWPOWER_DEMO_QUEUE_HANDLE, &lowpower_data, 0, 0))
            {
                return;
            }
        }
        else // 如果获取到的电平不等于唤醒电平 则通知进入休眠，解锁睡眠
        {
            lowpower_data.event = CM_LOWPOWER_DEMO_EVENT_ENTRY;
            if (osOK != osMessageQueuePut(CM_LOWPOWER_DEMO_QUEUE_HANDLE, &lowpower_data, 0, 0))
            {
                return;
            }
        }
    }
}

/**
 *  @brief GPIO消抖前中断函数
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 当GPIO触发中断，会推送消息至队列中，启动定时器开始消抖
 *           
 */
static void cm_lowpower_gpio_interrupt_callback(void)
{
    //cm_log_printf(0,"cm_lowpower_gpio_interrupt_callback");
    int msg = CM_GPIO_NUM_9;
    cm_gpio_level_e gpio_level = 0;
    cm_gpio_get_level(CM_GPIO_NUM_9, &gpio_level);
    s_gpio_last_level = gpio_level;
    osMessageQueuePut(s_gpio_irq_queue, &msg, 0, 0);
}

/**
 *  @brief GPIO消抖线程
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 这里初始化的是GPIO9 注意参考《ML307R_OpenCPU资源综述》手册，对应的是PIN19及复用功能2，若是其他引脚需要相应修改
 *           同时初始化GPIO的上下沿中断功能，并利用定时器实现100ms的消抖
 */
void debouce_gpio_task(void *arg)
{
    int gpio_id = 0;
    cm_gpio_level_e gpio_level = 0;
    while (1)
    {
        if (osMessageQueueGet(s_gpio_irq_queue, &gpio_id, NULL, osWaitForever) == osOK)
        {
            cm_gpio_get_level(CM_GPIO_NUM_9, &gpio_level);
            //cm_log_printf(0,"debouce_gpio_task:%d",gpio_level);
            if (gpio_level == s_gpio_last_level)
            {
                osTimerStart(s_gpio_debouce_timer, CM_LOWPOWER_DEMO_DEBOUCE_MS / 5);
            }
        }
    }
}

/**
 *  @brief 初始化用于休眠唤醒的GPIO
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 这里初始化的是GPIO9 注意参考《ML307R_OpenCPU资源综述》手册，对应的是PIN19及复用功能2，若是其他引脚需要相应修改
 *           同时初始化GPIO的上下沿中断功能，并利用定时器实现100ms的消抖
 */
int cm_gpio_set_debouce_in_init(void)
{
    cm_iomux_func_e gpio_func;
    cm_iomux_set_pin_func(CM_IOMUX_PIN_19, CM_IOMUX_FUNC_FUNCTION2);
    cm_iomux_get_pin_func(CM_IOMUX_PIN_19, &gpio_func);
    if (gpio_func != CM_IOMUX_FUNC_FUNCTION2)
    {
        return -1;
    }
    
    cm_gpio_cfg_t gpio_cfg = {0};
    gpio_cfg.direction = CM_GPIO_DIRECTION_INPUT;
    gpio_cfg.pull = CM_LOWPOWER_DEMO_WAKEUP_PULL;
    int ret = cm_gpio_init(CM_GPIO_NUM_9, &gpio_cfg);
    if (ret < 0)
    {
        return -1;
    }
    
    if (s_gpio_irq_queue == NULL)
    {
        s_gpio_debouce_timer = osTimerNew(debouce_gpio_timer_cb, osTimerOnce, (void *)CM_GPIO_NUM_9, 0);
        s_gpio_irq_queue = osMessageQueueNew(10, sizeof(int), NULL); // 创建消息队列
        osThreadAttr_t attr = {
            .name = "debouce_gpio_task",
            .priority = osPriorityNormal,
            .stack_size = 1024 * 4,
        };
        osThreadNew(debouce_gpio_task, NULL, (const osThreadAttr_t *)&attr);
    }
    
    cm_gpio_interrupt_register(CM_GPIO_NUM_9, cm_lowpower_gpio_interrupt_callback);
    cm_gpio_interrupt_enable(CM_GPIO_NUM_9, CM_GPIO_IT_EDGE_BOTH);

    return 0;
}
/**
 *  @brief 低功耗-锁定休眠
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details
 */
void cm_lowpower_demo_lock(void)
{
    if (cm_lowpower_demo_flag == 0)
    {
        cm_pm_work_lock();
        cm_lowpower_demo_flag = 1;
    }
}

/**
 *  @brief 低功耗-解锁休眠
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 注意调用该接口后，模组不是立即进入休眠，而是当模组满足休眠条件时才会进入
 *           往往调用该接口后，网络可能还在工作，处于ACTIVE态，因此建议所有数据业务做完后，解锁休眠，模组自动会进入休眠
 */
void cm_lowpower_demo_unlock(void)
{
    if (cm_lowpower_demo_flag == 1)
    {
        cm_pm_work_unlock();
        cm_lowpower_demo_flag = 0;
    }
}

/**
 *  @brief 低功耗 demo主程序
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 接收回调收到的事件，并打印进入休眠和退出休眠次数
 */
static void cm_lowpower_demo_main(void *p)
{
    cm_lowpower_demo_unlock(); //初始先解锁休眠
    while (1)
    {
        cm_lowpower_demo_data_t lowpower_data = {0};
        memset(&lowpower_data, 0, sizeof(lowpower_data));
        if (osOK == osMessageQueueGet(CM_LOWPOWER_DEMO_QUEUE_HANDLE, &lowpower_data, NULL, osWaitForever))
        {
            switch (lowpower_data.event)
            {
            case CM_LOWPOWER_DEMO_EVENT_ENTRY:
                g_cm_lowpower_demo_enter_count++;
                cm_log_printf(0,"CM_LOWPOWER_DEMO_EVENT_ENTRY:%d", g_cm_lowpower_demo_enter_count); // 打印进入次数
                cm_lowpower_demo_unlock(); // 解锁睡眠后，就不要做任何操作了,使系统进入osWaitForever或者osDelay等阻塞态，让模组空闲便会自动进入休眠
                break;
            case CM_LOWPOWER_DEMO_EVENT_EXIT:
                g_cm_lowpower_demo_exit_count++;
                cm_log_printf(0,"CM_LOWPOWER_DEMO_EVENT_EXIT:%d", g_cm_lowpower_demo_exit_count); // 打印退出次数
                cm_lowpower_demo_lock(); // 锁定睡眠后,就可以开始应用层业务,如发送数据等
                break;

            default:
                break;
            }
        }
    }
}
/**
 *  @brief LOWPOWER接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始MQTT相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*初始化用于测试休眠唤醒的GPIO口 这里使用的GPIO9*/
    if(cm_gpio_set_debouce_in_init() < 0)
    {
        cm_log_printf(0,"cm_gpio_set_debouce_in_init failed!");
    }
    
    /*初始化PM低功耗功能，设置回调，注意这里的回调中不写入任何应用*/
    cm_pm_cfg_t pm_cfg = {cm_lowpower_demo_entry_callback, cm_lowpower_demo_exit_callback};
    cm_pm_init(pm_cfg);

    /*创建测试主函数线程*/
    if (CM_LOWPOWER_DEMO_QUEUE_HANDLE == NULL)
    {
        CM_LOWPOWER_DEMO_QUEUE_HANDLE = osMessageQueueNew(20, sizeof(cm_lowpower_demo_data_t), NULL); // 创建消息队列
    }

    /*创建socket数据接收/发送处理线程*/
    osThreadAttr_t lowpower_main_task_attr = {0};
    lowpower_main_task_attr.name = "lowpower_demo_main_task";
    lowpower_main_task_attr.stack_size = 10 * 1024;
    lowpower_main_task_attr.priority = osPriorityNormal;

    if (CM_LOWPOWER_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_LOWPOWER_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_lowpower_demo_main, 0, &lowpower_main_task_attr);
        if (CM_LOWPOWER_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_LOWPOWER_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    cm_log_printf(0,"lowpower init success!\n");

    return 0;
}