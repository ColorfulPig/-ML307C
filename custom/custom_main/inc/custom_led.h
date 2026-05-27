#ifndef __CUSTOM_LED_H__
#define __CUSTOM_LED_H__

#include "cm_os.h"
#include "cm_gpio.h"
#include "cm_iomux.h"

enum
{
	LED_STATE_IDLE = 0,
	LED_STATE_READY,
	LED_STATE_ONLINE,
};

int custom_led_init(void);
int custom_led_setState(uint8_t state);

#endif


