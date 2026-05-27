
#include "custom_led.h"
#include "custom_system.h"
#include "custom_track.h"

#define	LED_LINK_IOMUX_PIN	CM_IOMUX_PIN_16
#define	LED_LINK_GPIO		CM_GPIO_NUM_4

#define	SET_LED_LINK_ON		cm_gpio_set_level(LED_LINK_GPIO, CM_GPIO_LEVEL_HIGH)
#define	SET_LED_LINK_OFF	cm_gpio_set_level(LED_LINK_GPIO, CM_GPIO_LEVEL_LOW)

uint8_t	custom_led_state = 0;


int custom_led_setState(uint8_t state)
{
	custom_led_state = state;
	return 0;
}

void custom_led_task(void *p)
{
	custom_led_setState(LED_STATE_IDLE);
	
	while(1)
	{
		if(custom_led_state == LED_STATE_IDLE)
		{
			SET_LED_LINK_ON;
			osDelay(ONE_SECONED_1_4);
			SET_LED_LINK_OFF;
			osDelay(ONE_SECONED_3_4);

		}
		else if(custom_led_state == LED_STATE_READY)
		{
			SET_LED_LINK_ON;
			osDelay(ONE_SECONED * 2);
			SET_LED_LINK_OFF;
			osDelay(ONE_SECONED);

		}
		else if(custom_led_state == LED_STATE_ONLINE)
		{
			SET_LED_LINK_ON;
			osDelay(ONE_SECONED_5_100);
			SET_LED_LINK_OFF;
			osDelay(ONE_SECONED_195_100);
		}
		else
		{
			SET_LED_LINK_ON;
			osDelay(ONE_SECONED);
			SET_LED_LINK_OFF;
			osDelay(ONE_SECONED);
		}
	}
}

int custom_led_init(void)
{    
	cm_gpio_cfg_t gpio_cfg = {0};

	// 引脚复用
	cm_iomux_set_pin_func(LED_LINK_IOMUX_PIN, CM_IOMUX_FUNC_FUNCTION1);

	// 引脚配置
	gpio_cfg.direction = CM_GPIO_DIRECTION_OUTPUT;
	gpio_cfg.pull = CM_GPIO_PULL_UP;
	int ret = cm_gpio_init(LED_LINK_GPIO, &gpio_cfg);
	if (ret < 0)
	{
		return -1;
	}

	// 创建任务
	osThreadAttr_t app_task_attr = {0};
	app_task_attr.name  = "led_task";
	app_task_attr.stack_size = 1024 * 2;
	app_task_attr.priority = osPriorityNormal;
	osThreadNew((osThreadFunc_t)custom_led_task, 0, &app_task_attr);

	return 0;
}

