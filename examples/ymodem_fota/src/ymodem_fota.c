/**
 * @file        ymodem_fota.c
 * @brief       基于Ymodem传输协议实现app升级, 示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      
 */

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "cm_os.h"
#include "cm_rtc.h"
#include "ymodem_fota.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "cm_modem.h"
#include "cm_fota.h"
#include "cm_fs.h"
#include "cm_ota.h"
#include "cm_uart.h"
#include "cm_virt_at.h"
#include "ymodem.h"
#include "stdarg.h"

#define UART_BUF_LEN            1024

/* 从测试串口打印字符串 */
void cm_demo_printf(char *str, ...)
{
    char s[600] = {0}; //This needs to be large enough to store the string TODO Change magic number
    va_list args;
    int len;
    
    if ((str == NULL) || (strlen(str) == 0))
    {
        return;
    }

    va_start(args, str);
    len = vsnprintf((char*)s, 600, str, args);
    va_end(args);
    cm_uart_write(OPENCPU_MAIN_URAT, s, len, 1000);
}

/**
 *  @brief ymodem协议传输升级包，实现FOTA 升级
 *
 *  @return
 *  0  升级成功
 *  -1 升级初始化失败
 *  -2 升级失败
 *  -3 文件传输失败
 *
 *  @details 
 */
int cm_ymodem_fota_test()
{
    
    char buff[1024] = {0};
    int file_received = 0;

    int ret = cm_ota_init(); //ota升级初始化
    if (ret < 0)
    {
        cm_log_printf(0,"cm_ota_init failed:%d!\n", ret);
        return -1;
    }

    Ymodem_Receive ((uint8_t *)buff, &file_received);   //ymodem协议传输文件
    if (file_received)
    {
      int result = cm_ota_upgrade(); // 升级

      if(result == 0){
         cm_log_printf(0,"upgrade succuss:%d!\n", result);
         return 0;
      }
      else{
        cm_log_printf(0,"upgrade faild:%d!\n", result);
        return -2;
      }

    }
    else
    {
    cm_log_printf(0, "File trans failed.\n");
    cm_ota_firmware_erase();
    return -3;
    }
}

void cm_fota_main(char *param)
{
    int32_t ret = -1;
    char version[10] = "1.0.0";   //测试版本号

    cm_log_printf(0, "Version:%s\n", version);

    /* 配置参数 */
    cm_uart_cfg_t config = 
    {
        CM_UART_BYTE_SIZE_8, 
        CM_UART_PARITY_NONE,
        CM_UART_STOP_BIT_ONE, 
        CM_UART_FLOW_CTRL_NONE, 
        CM_UART_BAUDRATE_9600,
        0,   //配置为普通串口模式，若要配置为低功耗模式可改为1
        0,   //环形缓存区大小按照默认配置8k
        0,
        0,
    };

    cm_log_printf(0, "uart NUM = %d start... ...", OPENCPU_MAIN_URAT);

    /* 开启串口 */
    ret = cm_uart_open(OPENCPU_MAIN_URAT, &config);
    
    if (ret != RET_SUCCESS)
    {
        cm_log_printf(0, "uart init err,ret=%d\n", ret);
        return;
    }

    int result = cm_ymodem_fota_test();

    cm_demo_printf("Ymodem_Fota upgrade Result:%d\n", result);
    
}

int cm_opencpu_entry(void *param)
{
    (void)param;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_fota_main, 0, &app_task_attr);

    return 0;

}
