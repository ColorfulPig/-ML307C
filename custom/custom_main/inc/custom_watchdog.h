#ifndef __CUSTOM_WATCHDOG_H__
#define __CUSTOM_WATCHDOG_H__

#include "cm_os.h"
#include "cm_gpio.h"
#include "cm_iomux.h"

/* 初始化看门狗 GPIO 并创建喂狗任务。 */
int custom_watchdog_init(void);

#endif


