/**
 * @file        i2c_eeprom.c
 * @brief       使用i2c对eeprom is24c256芯片进行操作，示例仅供参考
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
#define EEPROM_DEV_ADDR   	(0x50)//I2C地址
#define EEPROM_I2C_ID	    CM_I2C_DEV_1//以I2C1为例

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
 * @brief eeprom写1字节数据
 *
 * @param [in] addr:地址，范围0-0x7fff
 * @param [in] data:待写入数据
 *
 * @return  
 *   = 0  - 成功\n
 *   < 0  - 失败, 返回值为错误码.
 *
 * @details More details
 */
static int32_t is24c256_write_byte(uint16_t addr, int8_t data)
{
	int8_t tmp[3] = {0};
	int32_t ret = -1;

	tmp[0] = (addr >> 8) & 0xff;
	tmp[1] = addr & 0xff;
	tmp[2] = data;

	ret = cm_i2c_write(EEPROM_I2C_ID , EEPROM_DEV_ADDR, tmp, 3);
	if(ret < 0)
	{
		cm_log_printf(0,"i2c write e2prom err:0x%x\r\n", ret);
		return -1;
	}
		
	return 0;
}


/**
 * @brief eeprom读1字节数据
 *
 * @param [in] addr:地址，范围0-0x7fff
 * @param [out] data:待读取数据指针
 *
 * @return  
 *   = 0  - 成功\n
 *   < 0  - 失败.
 *
 * @details More details
 */
static int32_t is24c256_read_byte(uint16_t addr, int8_t* data)
{
	int8_t tmp[2] = {0};
	int32_t ret;

	if(data == NULL)
	{
		cm_log_printf(0,"is24c256_read_byte data ptr err\r\n");
		return -1;
	}

	tmp[0] = (addr >> 8) & 0xff;
	tmp[1] = addr & 0xff;

	ret = cm_i2c_write(EEPROM_I2C_ID, EEPROM_DEV_ADDR, tmp, 2);
	if(ret < 0)
	{
		cm_log_printf(0,"i2c read addr err(w):%08x\r\n", ret);
		return ret;
	}

	ret = cm_i2c_read(EEPROM_I2C_ID, EEPROM_DEV_ADDR, data, 1);
	if(ret < 0)
	{
		cm_log_printf(0,"i2c read addr err(r):%08x\r\n", ret);
		return ret;
	}

	return 0;	
}

/**
 * @brief  eeprom利用i2c接口读写数据测试开始
 *
 * @param [in] void
 *
 * @return  
 *   = 0  - 成功\n
 *   < 0  - 失败.
 *
 * @details More details
 */
static int cm_i2c_is24c256_start(void)
{

	cm_i2c_cfg_t config = 
	{
		CM_I2C_ADDR_TYPE_7BIT,
		CM_I2C_MODE_MASTER, //目前仅支持模式
		CM_I2C_CLK_100KHZ
	};//master模式,(100KHZ)

	uint16_t eprom_addr = 0x100; //选取所支持的任意E2PROM地址进行测试,可修改
	int8_t w_data = 'B',r_data = 0;
	int32_t ret = 0;

    //开启之前一定要先设置引脚的功能复用，详见资源综述
	cm_iomux_set_pin_func(CM_IOMUX_PIN_66, CM_IOMUX_FUNC_FUNCTION1); //IIC1_SDA
	cm_iomux_set_pin_func(CM_IOMUX_PIN_67, CM_IOMUX_FUNC_FUNCTION1); //IIC1_SCL

    //开启I2C，进行相关配置
	ret = cm_i2c_open(EEPROM_I2C_ID, &config);
	if(ret != 0)
	{
		cm_log_printf(0,"i2c init err,ret=%d\n", ret);
		return -1;
	}
	cm_log_printf(0,"i2c init ok\n");
	
	//写入测试数据
	ret = is24c256_write_byte(eprom_addr, w_data);
	if(ret != 0)
	{
		cm_log_printf(0,"i2c write e2prom err %d\n", (uint32_t)ret);
		cm_i2c_close(EEPROM_I2C_ID);//关闭相应的I2C
		return -1;
	}
	cm_log_printf(0,"i2c write e2prom ok\n");
	osDelay(10);//延时等待写入完成

	//读取相应的地址数据
	ret = is24c256_read_byte(eprom_addr, &r_data);
	if(ret != 0)
	{
		cm_log_printf(0,"i2c read e2prom err:%d\n", ret);
		cm_i2c_close(EEPROM_I2C_ID);//关闭相应的I2C
		return -1;
	}
    
	cm_log_printf(0,"i2c read e2prom,%c\n",r_data);
    cm_i2c_close(EEPROM_I2C_ID);//关闭相应的I2C
	cm_log_printf(0,"i2c test end!\n");

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
        if(0 == cm_i2c_is24c256_start())
        {
            cm_log_printf(0,"i2c is24c256 OK\n");
        }
        else
        {
            cm_log_printf(0,"i2c is24c256 FAIL\n");
        }

        osDelay(10000);
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
