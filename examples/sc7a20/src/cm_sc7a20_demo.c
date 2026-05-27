/**
 * @file        cm_sc7a20_demo.c
 * @brief       本示例展示SC7A20 I2C通信
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

#include "cm_os.h"
#include "cm_i2c.h"
#include "cm_sys.h"
#include "cm_pm.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define APP_DEMO_GSENSOR_I2C CM_I2C_DEV_0   // GSENSOR使用的I2C外设通路，支持CM_I2C_DEV_0、CM_I2C_DEV_1
#define APP_DEMO_GSENSOR_TIMER_INTERVAL_S 5 // 定时获取GSENSOR加速度值的时间间隔，单位S

#define SC7A20_CHIP_ID 0x11
#define SC7A20_DEVICE_ADDRESS 0x18
#define SC7A20_WHO_AM_I 0x0F
#define SC7A20_CONTROL_REG_1 0x20
#define SC7A20_CONTROL_REG_2 0x21
#define SC7A20_CONTROL_REG_3 0x22
#define SC7A20_OUT_XL 0x28
#define SC7A20_OUT_XH 0x29
#define SC7A20_OUT_YL 0x2A
#define SC7A20_OUT_YH 0x2B
#define SC7A20_OUT_ZL 0x2C
#define SC7A20_OUT_ZH 0x2D
#define SC7A20_ACCEL_LIMIT 2000

static uint8_t cm_gsensor_demo_init_data = 0x27;           // GSNESOR-SC7A20初始化配置参数
static osThreadId_t CM_SC7A20_DEMO_MAIN_TASKHANDLE = NULL; // G-SENSOR测试线程 定时获取一次G-SENSOR数据
static osTimerId_t CM_SC7A20_DEMO_TIMER_HANDLE = NULL;     // 定时器
static osSemaphoreId_t CM_SC7A20_DEMO_SEM_HANDLE = NULL;   // 信号量，用于通知线程获取G-SENSOR
/****************************************************************************
 * Public Types
 ****************************************************************************/
static void cm_sc7a20_demo_timer_callback(void *arg);
static bool cm_sc7a20_demo_init();
static bool cm_sc7a20_demo_read_XYZ(int16_t *x_value, int16_t *y_value, int16_t *z_value);
/**
 *  @brief GSENSOR demo主程序
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details GSENSOR 主线程，在该线程中会等待定时器到达并获取信号量，读取GSENSOR姿态，输出相关数据至log口。
 */
static void cm_sc7a20_demo_main(void *p)
{
    // 创建信号量
    if (CM_SC7A20_DEMO_SEM_HANDLE == NULL)
    {
        CM_SC7A20_DEMO_SEM_HANDLE = osSemaphoreNew(1, 0, NULL);
    }

    // 创建定时器
    if (NULL == CM_SC7A20_DEMO_TIMER_HANDLE)
    {
        CM_SC7A20_DEMO_TIMER_HANDLE = osTimerNew(cm_sc7a20_demo_timer_callback, osTimerPeriodic, NULL, NULL);
    }

    // 初始化GSENSOR
    if (!cm_sc7a20_demo_init())
    {
        cm_log_printf(0, "GSENSOR SC7A20 INIT FAILED ! system reboot...");
        cm_pm_reboot();
        osDelay(2000 / 5);
    }

    // 开始1S定时器
    osTimerStart(CM_SC7A20_DEMO_TIMER_HANDLE, (APP_DEMO_GSENSOR_TIMER_INTERVAL_S * 1000) / 5); // 定时周期单位为tick，此处示例为1s

    bool result = 0;
    int16_t app_demo_x_value = 0;
    int16_t app_demo_y_value = 0;
    int16_t app_demo_z_value = 0;

    while (1)
    {
        // 阻塞等待信号量释放
        osSemaphoreAcquire(CM_SC7A20_DEMO_SEM_HANDLE, osWaitForever);

        result = cm_sc7a20_demo_read_XYZ(&app_demo_x_value, &app_demo_y_value, &app_demo_z_value);

        cm_log_printf(0, "cm_sc7a20_demo_read_XYZ result:%d", result);
        cm_log_printf(0, "cm_sc7a20_demo_read_XYZ X_VALUE:%d", app_demo_x_value);
        cm_log_printf(0, "cm_sc7a20_demo_read_XYZ Y_VALUE:%d", app_demo_y_value);
        cm_log_printf(0, "cm_sc7a20_demo_read_XYZ Z_VALUE:%d", app_demo_z_value);
    }
}
/**
 *  @brief 定时器回调函数
 *
 *  @param [out] argument 标识 与osTimerNew保持一致
 *
 *  @return
 *
 *  @details 回调函数中禁止串口打印等耗时操作，容易导致宕机，建议将事件发送任务中执行
 */
static void cm_sc7a20_demo_timer_callback(void *arg)
{
    osSemaphoreRelease(CM_SC7A20_DEMO_SEM_HANDLE); // 释放信号量
}

/**
 *  @brief SC7A20 GSENSOR读I2C接口
 *
 *  @param [in] dev_addr  设备地址
 *  @param [in] reg_addr  寄存器地址
 *  @param [in] data      读出数据后预存放的buffer
 *  @param [in] len       预读取字节
 *
 *  @return
 *  >0 读取成功 实际读出字节数
 *  <0 读取失败 对应错误码
 *
 *  @details 该接口是包含了先写后读的过程，因此reg_addr是预读取的寄存器地址，写入成功后，外设再返回寄存器上的数据，并存放于data中
 */
int32_t cm_sc7a20_demo_read(uint16_t dev_addr, uint8_t reg_addr, void *data, uint32_t len)
{
    int32_t ret = 0;
    ret = cm_i2c_write(APP_DEMO_GSENSOR_I2C, dev_addr, &reg_addr, 1);
    if (ret < 0)
    {
        cm_log_printf(0, "app i2c read/write error:%d", ret);
        return ret;
    }

    ret = cm_i2c_read(APP_DEMO_GSENSOR_I2C, dev_addr, data, len);
    if (ret != len)
    {
        cm_log_printf(0, "app i2c read error:%d", ret);
        return ret;
    }

    return ret;
}

/**
 *  @brief SC7A20 GSENSOR写I2C接口
 *
 *  @param [in] dev_addr  设备地址
 *  @param [in] reg_addr  寄存器地址
 *  @param [in] data      预写入的数据
 *  @param [in] len       预写入的字节数
 *
 *  @return
 *  >0 写入成功 实际写入的字节数
 *  <0 写入失败 对应错误码
 *
 *  @details 该接口是包含了先写后读的过程，因此reg_addr是预读取的寄存器地址，写入成功后，外设再返回寄存器上的数据，并存放于data中
 */
int32_t cm_sc7a20_demo_write(uint16_t dev_addr, uint8_t reg_addr, const void *data, uint32_t len)
{
    int32_t ret = 0;
    uint8_t buf[256] = {0};
    memset(buf, 0, sizeof(buf));
    buf[0] = reg_addr;
    memcpy(buf + 1, data, len);
    ret = cm_i2c_write(APP_DEMO_GSENSOR_I2C, dev_addr, buf, len + 1);
    if (ret != len + 1)
    {
        cm_log_printf(0, "app i2c write error:%d", ret);
        return ret;
    }

    return len;
}

/**
 *  @brief SC7A20 GSENSOR初始化接口
 *
 *  @param [in] dev_addr  设备地址
 *
 *  @return
 *  true 初始化成功
 *  false 初始化失败
 *
 *  @details 该接口用于初始化I2C，并向SC7A20读取WHO_AM_I寄存器，若答复成功说明与外设通信正常，并进一步进行配置。
 */
static bool cm_sc7a20_demo_init(void)
{
    uint8_t chip_id = 0;

    int ret = 0;
    cm_i2c_cfg_t config =
        {
            CM_I2C_ADDR_TYPE_7BIT,
            CM_I2C_MODE_MASTER, // 目前仅支持模式
            CM_I2C_CLK_100KHZ}; // master模式,(100KHZ)
    ret = cm_i2c_open(APP_DEMO_GSENSOR_I2C, &config);
    if (ret < 0)
    {
        cm_log_printf(0, "cm_sc7a20_demo_init I2C open error! ret:%d", ret);
        return false;
    }

    cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_WHO_AM_I, &chip_id, 1);
    if (chip_id != SC7A20_CHIP_ID)
    {
        cm_log_printf(0, "cm_sc7a20_demo_init gsensor read chip id failed!");
        return false;
    }

    if (1 != cm_sc7a20_demo_write(SC7A20_DEVICE_ADDRESS, SC7A20_CONTROL_REG_1, &cm_gsensor_demo_init_data, 1))
    {
        cm_log_printf(0, "cm_sc7a20_demo_init gsensor config reg1 failed!");
        return false;
    }

    return true;
}

/**
 *  @brief SC7A20 补位
 *
 *  @param [in] msb  高4位
 *  @param [in] lsb  低4位
 *
 *  @return
 *
 *  @details
 */
static int16_t cm_sc7a20_demo_12bit_complement(uint8_t msb, uint8_t lsb)
{
    int16_t temp;
    temp = msb << 8 | lsb;
    temp = temp >> 4; // 只有高12位有效
    temp = temp & 0x0fff;
    if (temp & 0x0800) // 负数 补码==>原码
    {
        temp = temp & 0x07ff; // 屏弊最高位
        temp = ~temp;
        temp = temp + 1;
        temp = temp & 0x07ff;
        temp = -temp; // 还原最高位
    }
    return temp;
}

/**
 *  @brief SC7A20 读取XYZ轴的加速度值
 *
 *  @param [out] x_value X轴的加速度值
 *  @param [out] y_value Y轴的加速度值
 *  @param [out] z_value Z轴的加速度值
 *
 *  @return
 *  true 读取成功
 *  false 读取失败
 *
 *  @details
 */
bool cm_sc7a20_demo_read_XYZ(int16_t *x_value, int16_t *y_value, int16_t *z_value)
{
    uint8_t x_msb, x_lsb, y_msb, y_lsb, z_msb, z_lsb;
    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_XH, &x_msb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_XH error!");
        return false;
    }
    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_XL, &x_lsb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_XL error!");
        return false;
    }
    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_YH, &y_msb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_YH error!");
        return false;
    }
    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_YL, &y_lsb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_YL error!");
        return false;
    }
    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_ZH, &z_msb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_ZH error!");
        return false;
    }

    if (1 != cm_sc7a20_demo_read(SC7A20_DEVICE_ADDRESS, SC7A20_OUT_ZL, &z_lsb, 1))
    {
        cm_log_printf(0, "gsensor i2c read SC7A20_OUT_ZL error!");
        return false;
    }

    *x_value = cm_sc7a20_demo_12bit_complement(x_msb, x_lsb);
    *y_value = cm_sc7a20_demo_12bit_complement(y_msb, y_lsb);
    *z_value = cm_sc7a20_demo_12bit_complement(z_msb, z_lsb);

    // cm_log_printf(0,"x_value:%d,y_value:%d,z_value:%d", *x_value, *y_value, *z_value);

    return true;
}

/**
 *  @brief MQTT接口测试启动
 *
 *  @param [in]
 *
 *  @return
 *
 *  @details 调用该函数，会开始MQTT相关接口测试
 */
int cm_opencpu_entry(void *arg)
{
    /*创建SC7A20测试主线程*/
    osThreadAttr_t sc7a20_main_task_attr = {0};
    sc7a20_main_task_attr.name = "sc7a20_demo_main_task";
    sc7a20_main_task_attr.stack_size = 10 * 1024;
    sc7a20_main_task_attr.priority = osPriorityNormal;

    if (CM_SC7A20_DEMO_MAIN_TASKHANDLE == NULL)
    {
        CM_SC7A20_DEMO_MAIN_TASKHANDLE = osThreadNew((osThreadFunc_t)cm_sc7a20_demo_main, 0, &sc7a20_main_task_attr);
        if (CM_SC7A20_DEMO_MAIN_TASKHANDLE == NULL)
        {
            cm_log_printf(0, "CM_SC7A20_DEMO_MAIN_TASKHANDLE is NULL");
        }
    }

    return 0;
}