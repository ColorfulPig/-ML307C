/*********************************************************
 *  @file    cm_demo_cJSON.c
 *  @brief   OpenCPU GcJSON示例
 *  Copyright (c) 2021 China Mobile IOT.
 *  All rights reserved.
 *  created by zyf 2022/11/15
 ********************************************************/

 /****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "cm_demo_uart.h"
#include "cm_iomux.h"
#include "cm_mem.h"
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

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

/****************************************************************************
 * Public Functions
****************************************************************************/
/*通过IOMUX直接可以设置PIN_86的为32k时钟输出，其他引脚无此功能，具体参见资源综述*/
void cm_test_clk_open(unsigned char **cmd, int len)
{
    cm_demo_printf("%d:cm_test_clk start\n",__LINE__);

    if( (cmd == NULL) || (len != 2))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    cm_iomux_set_pin_func(CM_IOMUX_PIN_86, CM_IOMUX_FUNC_FUNCTION2);
}

void cm_test_clk_close(unsigned char **cmd, int len)
{
    cm_demo_printf("%d:cm_test_clk start\n",__LINE__);

    
    if( (cmd == NULL) || (len != 2))
    {
        cm_demo_printf(" cmd = NULL or cmd len = %d error!\n", len);
        return;
    }
    
    cm_iomux_set_pin_func(CM_IOMUX_PIN_86, CM_IOMUX_FUNC_FUNCTION1);
}

