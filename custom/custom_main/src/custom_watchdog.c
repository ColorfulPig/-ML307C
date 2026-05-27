
#include "custom_watchdog.h"
#include "custom_track.h"
#include "custom_system.h"

#define	WATCHDOG_IOMUX_PIN	CM_IOMUX_PIN_25
#define	WATCHDOG_GPIO		CM_GPIO_NUM_5

#define	SET_WATCHDOG_HIGH	cm_gpio_set_level(WATCHDOG_GPIO, CM_GPIO_LEVEL_HIGH)
#define	SET_WATCHDOG_LOW	cm_gpio_set_level(WATCHDOG_GPIO, CM_GPIO_LEVEL_LOW)

void custom_watchdog_task(void *p)
{
	uint8_t level_flag = 0;
	
	while(1)
	{
		if(level_flag == 0)
		{
			level_flag = 1;
			SET_WATCHDOG_HIGH;
		}
		else
		{
			level_flag = 0;
			SET_WATCHDOG_LOW;
		}
		
		osDelay(ONE_SECONED);	// 1秒
	}
}

int custom_watchdog_init(void)
{    
	cm_gpio_cfg_t gpio_cfg = {0};

	// 引脚复用
	cm_iomux_set_pin_func(WATCHDOG_IOMUX_PIN, CM_IOMUX_FUNC_FUNCTION1);

	// 引脚配置
	gpio_cfg.direction = CM_GPIO_DIRECTION_OUTPUT;
	gpio_cfg.pull = CM_GPIO_PULL_UP;
	int ret = cm_gpio_init(WATCHDOG_GPIO, &gpio_cfg);
	if (ret < 0)
	{
		return -1;
	}

	// 创建喂狗任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "watchdog_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_watchdog_task, 0, &app_task_attr);

	return 0;
}

