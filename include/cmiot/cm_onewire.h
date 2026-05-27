/**
 * @file        cm_onewire.h
 * @brief       onewire接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By zyf
 * @date        2024/08/07
 *
 * @defgroup onewire
 * @ingroup PI
 * @{
 */

#ifndef __CM_ONEWIRE_H__
#define __CM_ONEWIRE_H__

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
* @brief oenwire 初始化
*  
* @param void
*  
* @return void
*  
* @details
*/
void cm_onewire_init(void);

/**
* @brief oenwire reset
*  
* @param void
*  
* @return 
*   = 1 - 复位成功 \n
*   = 0 - 复位失败 \n
*   = -1 - 复位超时
* @details
*/
int32_t cm_onewire_setup_reset(void);
 
/**
* @brief oenwire 单字节发送
*  
* @param [in] data 
*  
* @return 
*   = 0 - 发送成功 \n
*   < 0 - 发送失败
* @details
*/
int32_t cm_onewire_write_byte(uint32_t data );

/**
* @brief oenwire 单字节读取
*  
* @param [out] data 指针
*  
* @return 
*   = 0 - 读取成功 \n
*   < 0 - 读取失败
* @details
*/
int32_t cm_onewire_read_byte(uint32_t *data);

/**
* @brief oenwire 多字节发送
*  
* @param [in] data 指针
* @param [in] len 长度
*
* @return 
*   = 0 - 发送成功 \n
*   < 0 - 发送失败
* @details
*/
int32_t cm_onewire_write_data(uint32_t *data ,int32_t len);

/**
* @brief oenwire 多字节读取
*  
* @param [out] data 指针
* @param [out] len 长度

* @return 
*   = 0 - 读取成功 \n
*   < 0 - 读取失败
* @details
*/
int32_t cm_onewire_read_data(uint32_t *data ,int32_t len);

#undef EXTERN
#ifdef __cplusplus
}
#endif


#endif 

/** @}*/
