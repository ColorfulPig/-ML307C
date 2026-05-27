/**
 * @file        lcd.h
 * @brief       使用7735S LCD显示屏驱动，并增加lvgl刷屏展示，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by cmiot4594
 */


#ifndef __LCD_H__
#define __LCD_H__

#include <stdlib.h>
#include <stdio.h>


/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define LVGL_TICK 30

#define SUPPORT_APPDEMO_ML307C

#ifdef SUPPORT_APPDEMO_ML307C

    #define LCD_RST_PIN CM_GPIO_NUM_1
    #define LCD_BL_PIN CM_GPIO_NUM_7

#endif

#ifdef SUPPORT_APPDEMO_ML307R

    #define LCD_RST_PIN CM_GPIO_NUM_2
    #define LCD_BL_PIN CM_GPIO_NUM_7

#endif

#endif