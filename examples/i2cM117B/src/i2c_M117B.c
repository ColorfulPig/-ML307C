/**
 * @file        i2c_M117B.c
 * @brief       使用i2c对M117B芯片进行操作，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by zengyufeng
 */
 
/****************************************************************************
* Included Files
****************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cm_sys.h"
#include "cm_os.h"
#include "cm_i2c.h"
#include "cm_iomux.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/
#define M177B_I2C_ID (CM_I2C_DEV_1)//以I2C1为例
#define M177B_DEV_ADDR (0x45)//设备地址

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
 
/****************************************************************************
 * Private Data
 ****************************************************************************/

/****************************************************************************
 * Private Functions
 ****************************************************************************/
/**
 * @brief  M117B获取温度值
 *
 * @param [in] void
 *
  * @return  
 *   = 0  - 成功\n
 *   < 0  - 失败.
 *
 * @details More details
 */
static int8_t cm_i2c_m117b_start(void)
{
    cm_i2c_cfg_t config = 
    {
        CM_I2C_ADDR_TYPE_7BIT,
        CM_I2C_MODE_MASTER, //目前仅支持模式
        CM_I2C_CLK_100KHZ
    };//master模式,(100KHZ)
    int32_t ret = 0;
    int32_t write_len = 0;
    int32_t read_len = 0;
    uint8_t send_buf[16] = {0}; 
    char read_data[16] = {0};
    uint8_t crc = 0xff;

    /*开启之前一定要先设置引脚的功能复用，详见资源综述*/
    cm_iomux_set_pin_func(CM_IOMUX_PIN_66, CM_IOMUX_FUNC_FUNCTION1); //IIC1_SDA
    cm_iomux_set_pin_func(CM_IOMUX_PIN_67, CM_IOMUX_FUNC_FUNCTION1); //IIC1_SCL

    /*开启I2C，进行相关配置*/
    ret = cm_i2c_open(M177B_I2C_ID, &config);
    if(ret != 0)
    {
        cm_log_printf(0,"line:%d,i2c init err,ret=%d\n", __LINE__, ret);
        return -1;
    }
    
    cm_log_printf(0,"line:%d,i2c init ok\n", __LINE__);
    
    /*软复位*/
    memset(send_buf, 0, 16);
    send_buf[0] = 0x30;
    send_buf[1] = 0xa2;
    write_len = cm_i2c_write(M177B_I2C_ID, M177B_DEV_ADDR, send_buf, 2);
    if(write_len != 2)
    {
        cm_log_printf(0,"line:%d,cm_i2c_write len=2 error,ret=%d\n", __LINE__, write_len);
        cm_i2c_close(M177B_I2C_ID);
        return -1;
    }
    
    osDelay(200);
    
    /*启动温度转换*/
    memset(send_buf, 0, 16);
    send_buf[0] = 0xcc;
    send_buf[1] = 0x44;
    write_len = cm_i2c_write(M177B_I2C_ID, M177B_DEV_ADDR, send_buf, 2);
    if(write_len != 2)
    {
        cm_log_printf(0,"line:%d,cm_i2c_write len=2 error,ret=%d\n", __LINE__, write_len);
        cm_i2c_close(M177B_I2C_ID);
        return -1;
    }
    
    osDelay(4);
    
    /*等待转换完成后读取（Temp_lsb、Temp_msb、CRC）*/
    memset(read_data, 0, 16);
    read_len = cm_i2c_read(M177B_I2C_ID,M177B_DEV_ADDR,read_data,3);
    if(read_len != 3)
    {
        cm_log_printf(0,"line:%d,cm_i2c_read len=3 error,ret=%d\n", __LINE__, write_len);
        cm_i2c_close(M177B_I2C_ID);
        return -1;
    }
    
    cm_log_printf(0,"line:%d,read data1:%#x\n",__LINE__, read_data[0]);
    cm_log_printf(0,"line:%d,read data2:%#x\n",__LINE__, read_data[1]);
    cm_log_printf(0,"line:%d,read data3:%#x\n",__LINE__, read_data[2]);
    
    /*计算CRC*/
    for(int byte_num = 0; byte_num < 2; byte_num++)
    {
        crc ^= (read_data[byte_num]);
        
        for(int bit = 8; bit > 0; --bit)
        {
            if(crc & 0x80) 
            {
                    crc = (crc <<1) ^ 0x31;
            }
            else
            {
                crc = (crc << 1);
            }
        }
    }
    
    if(read_data[2] != crc)
    {
        cm_log_printf(0,"line:%d,crc=0x%#x error!\n",__LINE__, crc);
        cm_i2c_close(M177B_I2C_ID);
        return -1;
    }

    //计算摄氏度温度
    int16_t temp_int = read_data[0] << 8 | read_data[1];
    cm_log_printf(0,"line:%d,temperature:%f celsius\n",__LINE__, ((float)temp_int/256.0 + 40.0));
    cm_i2c_close(M177B_I2C_ID);
    return 0;
 }


/**
 * @brief  app入口函数
 *
 * @param [in] arg 入参指针
 *
 * @return  void
 *
 * @details More details
 */
static void __task_loop(void *arg)
{
    (void)arg;
    
    while(1)
    {
        cm_log_printf(0,"line:%d,i2c M117B start\n", __LINE__);
        if(0 == cm_i2c_m117b_start())
        {
            cm_log_printf(0,"line:%d,i2c M117B OK\n", __LINE__);
        }
        else
        {
            cm_log_printf(0,"line:%d,i2c M117B FAIL\n", __LINE__);
        }

        osDelay(2000);
    }
}

/****************************************************************************
 * Public Functions
 ****************************************************************************/
/**
 * @brief  opencpu程序主入口
 *
 * @param [in] arg 入参指针
 *
 * @return  
 *   = 0  - 成功\n
 *   < 0  - 失败.
 *
 * @details 禁止任何形式阻塞
 */
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)__task_loop, 0, &app_task_attr);
    return 0;
}
