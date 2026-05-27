#include "cm_os.h"
#include "cm_sys.h"
#include "cm_modem.h"
#include "cm_gpio.h"
#include "cm_lcd.h"
#include "stdlib.h"

/* 支持的APP定时器数量 */
#define APP_TIMER_NUN       (20)

#define black   ((0x00 << 11) | (0x00 << 5) | 0x00)
#define white   ((0x1F << 11) | (0x3F << 5) | 0x1F)

#define x_start   (161 - 32 + 1)
#define x_end     (161)
#define y_start   (2)
#define y_end     (2 + 16 - 1)


typedef void (*app_callback_on_timer)(void *param);
typedef void * APP_TIMER;


osThreadId_t TIMER_APP_TaskHandle = NULL;


typedef struct{
    osTimerId_t timer;                  /**< 定时器句柄 */
    uint32_t interval;                  /**< 首次定时时间，与app_timer_start()接口中interval对应 */
    uint32_t resched_interval;          /**< 循环定时时间，与app_timer_start()接口中resched_interval对应 */
    void *param;                        /**< 回调函数参数，与app_timer_start()接口中param对应 */
    app_callback_on_timer cb;           /**< 回调函数 */
    uint32_t index;                     /**< 定时器序列号 */
}app_timer_ctx_t;

typedef struct{
    int argument;
} timer_event_msg_t;

static app_timer_ctx_t app_timer[APP_TIMER_NUN] = {0};

static osMessageQueueId_t timer_event_queue = NULL;
static osThreadId_t timer_event_thread = NULL;

/* 网络信号显示图标，无网络 */
static uint16_t app_net_no_signal[32 * 16] = {black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, \
                                              black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, \
                                              black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, black, black, black, black, \
                                              black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, \
                                              black, black, black, black, white, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, white, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};

/* 网络信号显示图标，等级1 */
static uint16_t app_net_signal_l1[32 * 16] = {black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};

/* 网络信号显示图标，等级2 */
static uint16_t app_net_signal_l2[32 * 16] = {black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};

/* 网络信号显示图标，等级3 */
static uint16_t app_net_signal_l3[32 * 16] = {black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};

/* 网络信号显示图标，等级4 */
static uint16_t app_net_signal_l4[32 * 16] = {black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, black, black, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};

/* 网络信号显示图标，等级5 */
static uint16_t app_net_signal_l5[32 * 16] = {black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, white, white, white, black, black, black, black, \
                                              black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black, black};


static void __app_timer_callback(void *argument)
{
    int32_t index = (int)argument;

    //发事件至timer_event_task线程，由线程调用回调
    timer_event_msg_t msg = {0};

    msg.argument = index;

    if(timer_event_queue != NULL)//向队列发送数据
    {
        osMessageQueuePut(timer_event_queue, &msg, 0, 0);
    }
}

static void app_timer_event_task(void *arg)
{
    timer_event_msg_t msg = {0};

    while(1)
    {
        if(osMessageQueueGet(timer_event_queue, &msg, NULL, osWaitForever) == osOK)
        {
            cm_log_printf(0, "timer event msg argument = %d\n", msg.argument);
            int index = msg.argument;

            //回调执行
            if (app_timer[index].cb)
            {
                app_timer[index].cb(app_timer[index].param);
            }

            if (0 != app_timer[index].resched_interval)
            {
                if (osOK == osTimerStart(app_timer[index].timer, app_timer[index].resched_interval / 5))
                {
                    //cm_log_printf(0, "ant_timer_start() app_timer[index].timer %x\n", app_timer[index].timer);
                }
            }
            else
            {
                //回调执行后清空a_timer[index]资源
                app_timer[index].timer = NULL;
                app_timer[index].cb = NULL;
                app_timer[index].param = NULL;
                app_timer[index].interval = 0;
                app_timer[index].resched_interval = 0;
                app_timer[index].index = 0;
            }
        }
    }
}

static int32_t timer_event_task_create(void)
{
    if(timer_event_queue == NULL)
    {
        timer_event_queue = osMessageQueueNew(10, sizeof(timer_event_msg_t), NULL);
    }

    if(timer_event_thread == NULL)
    {
        osThreadAttr_t attr1 = {
            .name = "timer_event",
            .priority = osPriorityNormal - 2,
            .stack_size = 1024 * 4,
        };
        timer_event_thread = osThreadNew(app_timer_event_task, NULL, (const osThreadAttr_t*)&attr1);
    }

    return 0;
}

/**
 * 创建定时器
 * @param  name  定时器名称
 *
 * @return 成功: 返回定时器对象
 *         失败: 返回NULL
 */
static APP_TIMER app_timer_create(const char *name)
{
    int32_t index = 0;

    timer_event_task_create();

    for (index = 0; index < APP_TIMER_NUN; index++)
    {
        if (NULL == app_timer[index].timer)
        {
            app_timer[index].index = index;

            osTimerAttr_t attr = {0};
            attr.name = name;

            osTimerId_t timer = osTimerNew(__app_timer_callback, osTimerOnce, (void *)app_timer[index].index, &attr);

            if (NULL == timer)
            {
                return NULL;
            }
            else
            {
                app_timer[index].timer = timer;
                cm_log_printf(0, "app_timer_create() app_timer[index].timer %x\n", app_timer[index].timer);
                return app_timer[index].timer;
            }
        }
    }

    cm_log_printf(0, "app_timer_create() error\n");
    return NULL;
}

/**
 * 启动定时器
 *
 * @param timer 定时器对象
 * @param callback 时间到达时的回调函数
 * @param param 回调函数参数
 * @param interval 定时时间
 * @param resched_interval 定时事件发生时，自动重新开启定时的时间，如果传入0，则定时器自动停止
 *
 * @return 0   成功
 *         <0  失败
 */
static int32_t app_timer_start(APP_TIMER timer, app_callback_on_timer callback, void *param, int32_t interval, int32_t resched_interval)
{
    int32_t index = 0;

    for (index = 0; index < APP_TIMER_NUN; index++)
    {
        if (app_timer[index].timer == timer)
        {
            app_timer[index].cb = callback;
            app_timer[index].param = param;
            app_timer[index].interval = interval;
            app_timer[index].resched_interval = resched_interval;
            cm_log_printf(0, "app_timer_start() interval %d resched_interval %d\n", interval, resched_interval);
            break;
        }
    }

    if (osOK == osTimerStart(timer, interval / 5))
    {
        cm_log_printf(0, "app_timer_start() app_timer[index].timer %p\n", timer);
        return 0;
    }
    else
    {
        cm_log_printf(0, "app_timer_start() error app_timer[index].timer %p\n", timer);
        return -1;
    }
}

/**
 * 停止定时器对象
 *
 * @param timer 定时器对象
 *
 * @return 0   成功
 *         <0  失败
 */        
static int32_t app_timer_stop(APP_TIMER timer)
{
    if (osOK == osTimerStop(timer))
    {
        cm_log_printf(0, "app_timer_stop() a_timer[index].timer %p\n", timer);
        return 0;
    }
    else
    {
        cm_log_printf(0, "app_timer_stop() error a_timer[index].timer %p\n", timer);
        return -1;
    }
}

/**
 * 销毁定时器
 *
 * @param timer 定时器对象
 *
 * @return 0   成功
 *         <0  失败
 */
static int32_t app_timer_destroy(APP_TIMER timer)
{
    int32_t index = 0;

    for (index = 0; index < APP_TIMER_NUN; index++)
    {
        if (app_timer[index].timer == timer)
        {
            osTimerStop(app_timer[index].timer);
            osTimerDelete(app_timer[index].timer);
            app_timer[index].timer = NULL;
            app_timer[index].cb = NULL;
            app_timer[index].param = NULL;
            app_timer[index].interval = 0;
            app_timer[index].resched_interval = 0;
            app_timer[index].index = 0;
            cm_log_printf(0, "app_timer_destroy()__%d__ app_timer[index].timer %p\n", __LINE__, timer);
            return 0;
        }
    }

    cm_log_printf(0, "app_timer_destroy() error__%d__ app_timer[index].timer %p\n", __LINE__, timer);
    return -1;	
}

static void __app_lcd_no_signal_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_no_signal, 32 * 16 * 2);
}

static void __app_lcd_signal_l1_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_signal_l1, 32 * 16 * 2);
}

static void __app_lcd_signal_l2_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_signal_l2, 32 * 16 * 2);
}

static void __app_lcd_signal_l3_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_signal_l3, 32 * 16 * 2);
}

static void __app_lcd_signal_l4_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_signal_l4, 32 * 16 * 2);
}

static void __app_lcd_signal_l5_display(void)
{
    cm_lcd_write_cmd(0x2a);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(x_end);
    cm_lcd_write_cmd(0x2b);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_start);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(y_end);

    cm_lcd_write_cmd(0x2c);
    cm_lcd_write_buf((uint8_t *)app_net_signal_l5, 32 * 16 * 2);
}

/**
 * 初始化LCD
 *
 */
static void app_lcd_init(void)
{
    cm_gpio_cfg_t cfg = {
        .direction = CM_GPIO_DIRECTION_OUTPUT,
        .pull = CM_GPIO_PULL_DOWN
    };

    /* 背光控制角，以实际为准，示例中未使用 */
    cm_gpio_init(CM_GPIO_NUM_7, &cfg);

    /* RST脚，以实际为准 */
    cm_gpio_init(CM_GPIO_NUM_2, &cfg);

    cm_lcd_init();

    /* hw reset */
    cm_gpio_set_level(CM_GPIO_NUM_2, CM_GPIO_LEVEL_HIGH);
    osDelay(20);
    cm_gpio_set_level(CM_GPIO_NUM_2, CM_GPIO_LEVEL_LOW);
    osDelay(20);
    cm_gpio_set_level(CM_GPIO_NUM_2, CM_GPIO_LEVEL_HIGH);

    osDelay(30);
    cm_lcd_write_cmd(0x1);
    osDelay(30);
    cm_lcd_write_cmd(0x11);
    osDelay(30);
    
    cm_lcd_write_cmd(0xB1);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB2);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB3);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x05);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_cmd(0xB4);
    cm_lcd_write_data(0x03);
    cm_lcd_write_cmd(0xC0);
    cm_lcd_write_data(0x62);
    cm_lcd_write_data(0x02);
    cm_lcd_write_data(0x04);
    cm_lcd_write_cmd(0xC1);
    cm_lcd_write_data(0xC0);
    cm_lcd_write_cmd(0xC2);
    cm_lcd_write_data(0x0D);
    cm_lcd_write_data(0x00);
    cm_lcd_write_cmd(0xC3);
    cm_lcd_write_data(0x8D);
    cm_lcd_write_data(0x6A);
    cm_lcd_write_cmd(0xC4);
    cm_lcd_write_data(0x8D);
    cm_lcd_write_data(0xEE);
    cm_lcd_write_cmd(0xC5);
    cm_lcd_write_data(0x12);
    cm_lcd_write_cmd(0x36);  
    cm_lcd_write_data(0x60);
    osDelay(5);
    cm_lcd_write_cmd(0xE0);
    cm_lcd_write_data(0x03);
    cm_lcd_write_data(0x1B);
    cm_lcd_write_data(0x12);
    cm_lcd_write_data(0x11);
    cm_lcd_write_data(0x3F);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x32);
    cm_lcd_write_data(0x34);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2B);
    cm_lcd_write_data(0x30);
    cm_lcd_write_data(0x3A);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x01);
    cm_lcd_write_data(0x02);
    cm_lcd_write_data(0x05);
    
    cm_lcd_write_cmd(0xE1);
    cm_lcd_write_data(0x03);
    cm_lcd_write_data(0x1B);
    cm_lcd_write_data(0x12);
    cm_lcd_write_data(0x11);
    cm_lcd_write_data(0x32);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2A);
    cm_lcd_write_data(0x2F);
    cm_lcd_write_data(0x2E);
    cm_lcd_write_data(0x2C);
    cm_lcd_write_data(0x35);
    cm_lcd_write_data(0x3F);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x00);
    cm_lcd_write_data(0x01);
    cm_lcd_write_data(0x05);
    
    cm_lcd_write_cmd(0x3A);
    cm_lcd_write_data(0x05);
    cm_lcd_write_cmd(0x35);
    cm_lcd_write_data(0x00);

    __app_lcd_no_signal_display();

    cm_lcd_write_cmd(0x29);
}

/**
 * LCD自检
 *
 */
static void app_lcd_self_test(void)
{
    osDelay(200);
    __app_lcd_signal_l1_display();

    osDelay(200);
    __app_lcd_signal_l2_display();

    osDelay(200);
    __app_lcd_signal_l3_display();

    osDelay(200);
    __app_lcd_signal_l4_display();

    osDelay(200);
    __app_lcd_signal_l5_display();
}

static void _net_timer_expired(void *param)
{
    /* 获取cid 1路PDP上下文激活状态 */
    int32_t ret = cm_modem_get_pdp_state(1);

    if(1 == ret)
    {
        char s_rssi[16] = {0};
        char s_ber[16] = {0};
        int32_t rssi = 0;

        /* 获取CSQ信息 */
        if (0 == cm_modem_get_csq(s_rssi, s_ber))
        {
            rssi = atoi(s_rssi);
            cm_log_printf(0, "[network] Activated cid 1 is activated, rssi[%s, %d] ber[%d]\n", s_rssi, rssi, atoi(s_ber));

            /* 根据获取的rssi，在LCD屏幕右上角显示网络信号图标 */
            if (9 >= rssi)
            {
                __app_lcd_signal_l1_display();
            }
            else if ((9 < rssi) && (15 >= rssi))
            {
                __app_lcd_signal_l2_display();
            }
            else if ((15 < rssi) && (20 >= rssi))
            {
                __app_lcd_signal_l3_display();
            }
            else if ((20 < rssi) && (25 >= rssi))
            {
                __app_lcd_signal_l4_display();
            }
            else
            {
                __app_lcd_signal_l5_display();
            }
        }
        else
        {
            cm_log_printf(0, "[network] Activated cid 1 is not activated, rssi[%s] ber[%s]\n", s_rssi, s_ber);

            /* 在LCD屏幕右上角显示无网络图标 */
            __app_lcd_no_signal_display();
        }
    }
    else
    {
        cm_log_printf(0, "[network] Activated cid 1 is not activated (ret is %d), get CSQ fail\n", ret);

        /* 在LCD屏幕右上角显示无网络图标 */
        __app_lcd_no_signal_display();
    }
}

/**
 * 定时器定期检查网络线程
 *
 */
void timer_demo_appimg_enter(char *param)
{
    /* 初始化LCD */
    app_lcd_init();

    /* LCD自检 */
    app_lcd_self_test();

    /* 创建定时器 */
    APP_TIMER net_timer = app_timer_create("net_timer");

    if (NULL != net_timer)
    {
        int32_t ret = app_timer_start(net_timer, _net_timer_expired, "nettimer", 5000, 5000);

        if (0 > ret)
        {
            cm_log_printf(0, "app_timer_start() fail\n");
            app_timer_destroy(net_timer);
        }
    }

    /* 用户程序主业务，此处示例采用空实现 */
    while(1)
    {
        osDelay(1000);
    }

    /* 删除定时器 */
    app_timer_stop(net_timer);
    app_timer_destroy(net_timer);
}

int cm_opencpu_entry(char * param)
{
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "timer_demo_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    TIMER_APP_TaskHandle = osThreadNew((osThreadFunc_t)timer_demo_appimg_enter,0,&app_task_attr);

    return 0;
}
