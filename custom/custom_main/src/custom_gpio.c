
#include "custom_gpio.h"
#include "custom_system.h"
#include "custom_track.h"

#define	WAKEUP_IOMUX_PIN	CM_IOMUX_PIN_49
#define	WAKEUP_GPIO			CM_GPIO_NUM_20

#define	SET_WAKEUP_HIGH	cm_gpio_set_level(WAKEUP_GPIO, CM_GPIO_LEVEL_HIGH)
#define	SET_WAKEUP_LOW	cm_gpio_set_level(WAKEUP_GPIO, CM_GPIO_LEVEL_LOW)

void custom_gpio_task(void *p)
{	
	while(1)
	{
		SET_WAKEUP_HIGH;
		
		osDelay(ONE_SECONED);	// 1秒
	}
}

int custom_gpio_init(void)
{    
	cm_gpio_cfg_t gpio_cfg = {0};

	// 引脚复用
	cm_iomux_set_pin_func(WAKEUP_IOMUX_PIN, CM_IOMUX_FUNC_FUNCTION1);

	// 引脚配置
	gpio_cfg.direction = CM_GPIO_DIRECTION_OUTPUT;
	gpio_cfg.pull = CM_GPIO_PULL_UP;
	int ret = cm_gpio_init(WAKEUP_GPIO, &gpio_cfg);
	if (ret < 0)
	{
		return -1;
	}

	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "gpio_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_gpio_task, 0, &app_task_attr);

	return 0;
}

