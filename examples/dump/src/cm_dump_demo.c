/**
 * @file        cm_dump_demo.c
 * @brief       本示例是故意制造几种常见的死机场景，可类比用户代码自行排查，请谨慎测试
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

#include <string.h>
#include <strings.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "cm_os.h"
#include "cm_mem.h"
#include "cm_gpio.h"
#include "cm_iomux.h"
#include "cm_sys.h"
#include "cm_pm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/
#define CM_DUMP_DEMO_TYPE 0 // 0-栈过小 1-内存泄漏 2-数组越界 3-变量传递错误 4-软件回调阻塞 5-硬件回调阻塞

static osThreadId_t CM_DUMP_DEMO_MAIN_TASKHANDLE = NULL; // DUMP 测试现场

static osThreadId_t STACK_OVERSIZE_DUMP_TASKHANDLE = NULL; // 栈溢出场景下的测试线程
static uint8_t recursion_count = 5;                        // 递归次数
static osTimerId_t dump_timer_handle = NULL;               // DUMP定时器
static osMessageQueueId_t dump_msgqueue_handle = NULL;     // DUMP队列

/**
 *  @brief 变量传递函数
 *
 *  @param
 *
 *  @return
 *
 *  @details 形参要求传入指针类型
 */
static void app_variable_transmission_func(uint8_t *p)
{
    cm_log_printf(0, "app_variable_transmission_func p1:%d\n", p);
    *p = 10;
    cm_log_printf(0, "app_variable_transmission_func p2:%d\n", p);
}

/**
 *  @brief dump定时器回调
 *
 *  @param
 *
 *  @return
 *
 *  @details 回调中不可执行任何延时、复杂、耗时操作
 */
void cm_dump_timer_callback(void *param)
{
    // cm_log_printf(0, "cm_dump_timer_callback"); // 错误写法 回调中尽量不打印log或少打印log 若log打印函数进行了二次封装，且带有延时，就会造成死机
    uint8_t i = 0;
    osMessageQueuePut(dump_msgqueue_handle, &i, 0, 1000 / 5); // 错误写法 队列推送timeout配置了1s超时时间 导致死机
    // osMessageQueuePut(dump_msgqueue_handle, &i, NULL, 0);                          // 正确写法 timeout应设置为0
}

/**
 *  @brief dump GPIO中断回调
 *
 *  @param
 *
 *  @return
 *
 *  @details 回调中不可执行任何延时、复杂、耗时操作
 */
void cm_dump_gpio_callback(void)
{
    // cm_log_printf(0, "cm_dump_timer_callback"); // 错误写法 回调中尽量不打印log或少打印log 若log打印函数进行了二次封装，且带有延时，就会造成死机
    uint16_t count = 0; // 错误写法 硬件回调中做长时间延时，复杂计算等，造成死机
    while (1)
    {
        osDelay(1000 / 5);
        count++;
        cm_log_printf(0, "cm_dump_gpio_callback:count:%d\n", count);
    }
    // osMessageQueuePut(dump_msgqueue_handle, &i, 0, 0);        // 正确写法 通过消息队列将该事件发送到线程中进行处理，注意timeout应设置为0
}

/**
 *  @brief 用于栈过小 函数递归死机
 *
 *  @param
 *
 *  @return
 *
 *  @details
 */
static uint8_t stack_oversize_dump_func(uint8_t data)
{
    uint8_t a[512] = {0}; // 建立一个大的局部变量 递归5次 理论上至少需要2.5K的栈运行，会死机
    // uint8_t a[128] = {0}; //改为建立一个小的局部变量 递归5次，没有超过栈的大小，不会死机

    memset(a, 0, sizeof(a));
    data--;
    if (data == 0)
    {
        return 0;
    }

    cm_log_printf(0, "app_stack_oversize_func count:%d\n", data);
    return stack_oversize_dump_func(data);
}

/**
 *  @brief DUMP线程
 *
 *  @param
 *
 *  @return
 *
 *  @details 用于实现线程栈过小死机现象，调用函数并递归，逐渐消耗线程的栈空间，最终死机
 */
static void stack_oversize_dump_task(void)
{
    stack_oversize_dump_func(recursion_count);
    while (1)
    {
        osDelay(1000);
    }
}

/**
 *  @brief DUMP 示例主函数
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该接口，可模拟一种死机场景，可参考以上注释及代码，排查是否存在相同/类似情况
 */
void cm_dump_demo_main(void)
{
    if (CM_DUMP_DEMO_TYPE == 0)
    {
        /*创建DUMP线程*/
        osThreadAttr_t stack_oversize_dump_task_attr = {0};
        stack_oversize_dump_task_attr.name = "dump_demo_task";
        stack_oversize_dump_task_attr.stack_size = 512; // 将栈空间开小，只有512
        stack_oversize_dump_task_attr.priority = osPriorityNormal;

        if (STACK_OVERSIZE_DUMP_TASKHANDLE == NULL)
        {
            STACK_OVERSIZE_DUMP_TASKHANDLE = osThreadNew((osThreadFunc_t)stack_oversize_dump_task, 0, &stack_oversize_dump_task_attr);
            if (STACK_OVERSIZE_DUMP_TASKHANDLE == NULL)
            {
                cm_log_printf(0, "STACK_OVERSIZE_DUMP_TASKHANDLE is NULL\n");
                osDelay(200);
                cm_pm_reboot();
            }
        }
    }
    else if (CM_DUMP_DEMO_TYPE == 1)
    {
        while (1)
        {
            /*每1s申请一次动态内存，模组大约还剩可用空间为1178K左右，一次申请100K，因此大约10S后 就会内存耗尽死机*/
            cm_heap_stats_t stats = {0};
            if (0 == cm_mem_get_heap_stats(&stats))
            {
                cm_log_printf(0, "heap allocated: %d,heap free: %d\n", stats.allocated, stats.free); // 打印模组已使用和未使用的内存
            }
            uint8_t *p = cm_malloc(100 * 1024); // 动态申请内存
            p[0] = 0;
            // cm_free(p);      // 正确写法 申请完内存 使用完内存后，及时释放内存
            osDelay(1000 / 5); // 每1秒申请一次动态内存，不释放，最终内存耗尽死机
        }
    }
    else if (CM_DUMP_DEMO_TYPE == 2)
    {
        /*在编程中涉及到memcpy、memset等直接操作内存的接口应该谨慎使用*/
        uint8_t array_a[10] = "123456";
        uint8_t array_b[1024] = "123456789012345678901234567890";
        cm_log_printf(0, "memcpy1 b to a\n");
        // memcpy(array_a, array_b, sizeof(array_b)); // 错误写法 将b的10240字节拷贝给a,内存越界死机
        memcpy(array_a, array_b, sizeof(array_a)); // 正确写法 善用sizeof来限定字节数大小 这里采用正确写法，只是为了消除编译报警，测试时应该启用错误写法
        cm_log_printf(0, "memcpy2 b to a\n");
        /*这里实际测试未死机，内存越界是存在死机风险，仅当越界的内存是不能被修改或者被底层使用时才会死机，但在编程过程中也需要防止出现该类情况*/
    }
    else if (CM_DUMP_DEMO_TYPE == 3)
    {
        /*传参错误*/
        uint8_t a = 0;
        // app_variable_transmission_func(a); //   错误写法
        app_variable_transmission_func(&a); // 正确写法   这里采用正确写法，只是为了消除编译报警，测试时应该启用错误写法
    }
    else if (CM_DUMP_DEMO_TYPE == 4)
    {
        // 创建定时器
        if (NULL == dump_timer_handle)
        {
            dump_timer_handle = osTimerNew(cm_dump_timer_callback, osTimerOnce, NULL, NULL);
            osTimerStart(dump_timer_handle, (5 * 1000) / 5); // 计时5s进入回调死机
            cm_log_printf(0, "dump_timer start!\n");
        }

        // 创建消息队列
        if (NULL == dump_msgqueue_handle)
        {
            dump_msgqueue_handle = osMessageQueueNew(10, 10, NULL);
        }
    }
    else if (CM_DUMP_DEMO_TYPE == 5)
    {
        /*使用GPIO9作为测试脚，触发GPIO9的下降沿中断 触发中断回调，因为中断函数中存在阻塞/延时死机*/
        cm_gpio_cfg_t cfg = {0};
        cm_gpio_num_e gpio_num = CM_GPIO_NUM_9;
        cm_iomux_pin_e pin = CM_IOMUX_PIN_19;
        cm_iomux_func_e fun = CM_IOMUX_FUNC_FUNCTION2;
        cfg.direction = CM_GPIO_DIRECTION_INPUT;
        cfg.pull = CM_GPIO_PULL_UP;

        cm_iomux_set_pin_func(pin, fun);

        cm_gpio_init(gpio_num, &cfg); // 初始化GPIO9
        cm_gpio_interrupt_register(gpio_num, cm_dump_gpio_callback);
        cm_gpio_interrupt_enable(gpio_num, CM_GPIO_IT_EDGE_FALLING);
        cm_log_printf(0, "dump_gpio inited!\n");
    }

    while (1)
    {
        osDelay(1000);
    }
}

int cm_opencpu_entry(void *arg)
{
    /*创建DUMP处理线程*/
    osThreadAttr_t dump_main_task_attr = {0};
    dump_main_task_attr.name = "dump_demo_main_task";
    dump_main_task_attr.stack_size = 10 * 1024;
    dump_main_task_attr.priority = osPriorityNormal;

    if (CM_DUMP_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_DUMP_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_dump_demo_main, 0, &dump_main_task_attr);
        if (CM_DUMP_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_DUMP_DEMO_MAIN_TASKHANDLE is NULL\n");
        }
    }

    return 0;
}