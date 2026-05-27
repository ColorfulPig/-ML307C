/**
 * @file        lcd.c
 * @brief       使用7735S LCD显示屏驱动，并增加lvgl刷屏展示，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by cmiot4594
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_lcd.h"
#include "cm_gpio.h"
#include "cm_iomux.h"
#include "lv_obj.h"
#include "lv_port_disp.h"
#include "lcd.h"


/****************************************************************************
 * Private Types
 ****************************************************************************/
osThreadId_t MAIN_TASK_HANDLE = NULL;
osThreadId_t LVGL_TASK_HANDLE = NULL;
osThreadId_t LVGL_TICK_TASK_HANDLE = NULL;
osTimerId_t LVGL_TICK_TIMER_HANDLE = NULL;


/* 初始化lcd所需的gpio */
static void __lcd_io_init(cm_gpio_num_e test_gpio)
{
    cm_gpio_cfg_t cfg = {
        .direction = CM_GPIO_DIRECTION_OUTPUT,
        .pull = CM_GPIO_PULL_DOWN
    };
    cm_gpio_init(test_gpio, &cfg);
}

/* lcd背光 */
static void __lcd_backlight_set(cm_gpio_level_e level)
{
    cm_gpio_set_level(LCD_BL_PIN, level);
}

/**
 * st7735s的初始化 初始化后屏会处于花屏状态
 * 根据屏幕型号的不同，自修修改初始化参数
 */
static void __lcd_st7735s_init(void)
{
    __lcd_io_init(LCD_BL_PIN); // 背光以GPIO7为例

    __lcd_io_init(LCD_RST_PIN); // RST脚 以实际为准
    cm_lcd_init();
    /* hw reset */
    cm_gpio_set_level(LCD_RST_PIN, CM_GPIO_LEVEL_HIGH);
    osDelay(20); // 100ms
    cm_gpio_set_level(LCD_RST_PIN, CM_GPIO_LEVEL_LOW);
    osDelay(20); // 100ms
    cm_gpio_set_level(LCD_RST_PIN, CM_GPIO_LEVEL_HIGH);

    osDelay(25); // 120ms
    cm_lcd_write_cmd(0x1);
    osDelay(25); // 120ms
    cm_lcd_write_cmd(0x11); //sleep out
    osDelay(25); // 120ms
    
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
    cm_lcd_write_cmd(0xB4);  //column inversion
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
    cm_lcd_write_cmd(0xC5);  //vcom
    cm_lcd_write_data(0x12);
    cm_lcd_write_cmd(0x36);  
    cm_lcd_write_data(0x60);//cm_lcd_write_data(0x68);////MX,MV,RGB mode
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
    
    cm_lcd_write_cmd(0x3A); //65K mode
    cm_lcd_write_data(0x05);
    cm_lcd_write_cmd(0x35);
    cm_lcd_write_data(0x00);
    
    __lcd_backlight_set(CM_GPIO_LEVEL_HIGH);
    
    cm_lcd_write_cmd(0x29);  //display on
}

/**
 * @brief lvgl tick task
 */
static void __lvgl_tick_task()
{
	while(1) 
    {
        /** lv_tick_inc时间需于系统时钟周期一致，否则会导致刷新屏幕不及时   */
		lv_tick_inc(LVGL_TICK);
		osDelay(LVGL_TICK / 5);
	}
}

/**
 * @brief lvgl task
 */
static void __lvgl_task()
{
	while(1) 
    {
		lv_task_handler();
		osDelay(LVGL_TICK / 5);
	}
}

static void __lvgl_task_create(void)
{
    /** LVGL需要定期执行操作系统任务，因此需要创建两个线程，
     * 一个用于执行lv_tick_inc同步时钟，一个用于执行lv_task_handler刷新屏幕。
     * lv_tick_inc同步时钟可以用定时器实现，此处直接用线程delay调度实现  */

    /** __lvgl_task优先级高于__lvgl_tick_task，保证刷新屏幕及时 */
#if 1
    osThreadAttr_t lvgl_tick_task_attrs = {0};
    lvgl_tick_task_attrs.name  = "__lvgl_tick_task";
    lvgl_tick_task_attrs.stack_size = 1024;
    lvgl_tick_task_attrs.priority = osPriorityAboveNormal;
    LVGL_TICK_TASK_HANDLE = osThreadNew((osThreadFunc_t)__lvgl_tick_task,0,&lvgl_tick_task_attrs);
#else
    LVGL_TICK_TIMER_HANDLE = osTimerNew((osTimerFunc_t)lv_tick_inc, osTimerPeriodic, (void*)LVGL_TICK, NULL);
    osTimerStart(LVGL_TICK_TIMER_HANDLE, LVGL_TICK/5);
#endif

    osThreadAttr_t lvgl_task_attrs = {0};
    lvgl_task_attrs.name  = "__lvgl_task";
    lvgl_task_attrs.stack_size = 4096;
    lvgl_task_attrs.priority = osPriorityNormal;
    LVGL_TASK_HANDLE = osThreadNew((osThreadFunc_t)__lvgl_task,0,&lvgl_task_attrs);


}


void lcd_lvgl_show(void)
{
    /*Create a chart*/
    lv_obj_t * chart;
    chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, 160, 128);
    lv_obj_center(chart);
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/

    /*Add two data series*/
    lv_chart_series_t * ser1 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_YELLOW), LV_CHART_AXIS_SECONDARY_Y);
    lv_chart_series_t * ser2 = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_GREEN), LV_CHART_AXIS_PRIMARY_Y);

    /*Set the next points on 'ser1'*/
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 10);
    lv_chart_set_next_value(chart, ser1, 30);
    lv_chart_set_next_value(chart, ser1, 50);
    lv_chart_set_next_value(chart, ser1, 30);
    lv_chart_set_next_value(chart, ser1, 70);
    lv_chart_set_next_value(chart, ser1, 20);
    lv_chart_set_next_value(chart, ser1, 30);
    lv_chart_set_next_value(chart, ser1, 70);
    lv_chart_set_next_value(chart, ser1, 90);

    int times = 0;
    while (1)
    {
        /*Directly set points on 'ser2'*/
        ser2->y_points[0] = 10 * LV_ABS(10 - times);
        ser2->y_points[1] = 10 * LV_ABS(9 - times);
        ser2->y_points[2] = 10 * LV_ABS(8 - times);
        ser2->y_points[3] = 10 * LV_ABS(7 - times);
        ser2->y_points[4] = 10 * LV_ABS(6 - times);
        ser2->y_points[5] = 10 * LV_ABS(5 - times);
        ser2->y_points[6] = 10 * LV_ABS(4 - times);
        ser2->y_points[7] = 10 * LV_ABS(3 - times);
        ser2->y_points[8] = 10 * LV_ABS(2 - times);
        ser2->y_points[9] = 10 * LV_ABS(1 - times);

        lv_chart_refresh(chart); /*Required after direct set*/
        times++;
        if(times >= 10)
        {times = 0;}
        osDelay(10);
    }
}

/** app线程入口*/
static void __task_loop(void *arg)
{
    (void)arg;

    __lcd_st7735s_init();       // lcd初始化
    lv_init();                  // lvgl初始化，如果这个没有初始化，那么下面的初始化会崩溃
    lv_port_disp_init();        // lvgl显示屏初始化
    __lvgl_task_create();       // lvgl任务创建

    while (1)
    {
        lcd_lvgl_show();         // lvgl显示屏刷新
        osDelay(200);
    }
    
}

/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    MAIN_TASK_HANDLE = osThreadNew((osThreadFunc_t)__task_loop, 0, &app_task_attr);
    return 0;
}