#ifndef __CM_DEMO_LCD_H__
#define __CM_DEMO_LCD_H__

#include <stdint.h>
#include <stddef.h>

void cm_test_lcd(unsigned char **cmd, int len);

#define LCD_RST_PIN CM_GPIO_NUM_1
#define LCD_BL_PIN CM_GPIO_NUM_7

#endif