/**
 * @file        cm_keypad.h
 * @brief       cm_keypad接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By cmiot3000
 * @date        2021/5/14
 *
 * @defgroup keypad keypad
 * @ingroup PI
 * @{
 */
 
/****************************************************************************************************************
 ***************************************KEYPAD使用注意事项***********************************************
 * 1、设备上电之后首先需要将各个引脚功能进行IOMUX配置，特别时keypad引脚;
 * 2、不使用的keypad引脚需要IOMUX配置为其他功能，多个引脚不能同时配置为同一功能；
 * 3、搭建稳定的硬件环境进行测试开发；
 * 4、中断函数不能处理耗时和重进入函数，例如打印；
 * 5、如果需要keypad唤醒功能，需要在进入低功耗回调函数中设置keypad休眠使能;
 * 6、如果需要keypad唤醒功能，需要在矩阵键盘回调函数中设置keypad休眠失能;
 * 7、不能将无关引脚接入矩阵键盘，特别是将可以配置为键盘的引脚接入，这将引入不确定电平干扰寄存器判断;
 *****************************************************************************************************************/
#ifndef __CM_KEYPAD_H__
#define __CM_KEYPAD_H__

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

/****************************************************************************/

/**
 *  @brief key map
 *  
 */
typedef uint32_t cm_keypad_map_t;

/**
 *  @brief key事件类型
 *  
 */
typedef enum
{
    CM_KEY_EVENT_RELEASE = 0,         /*!<按键被释放*/
    CM_KEY_EVENT_PRESS = 1,           /*!<按键被按下*/
} cm_keypad_event_e;
    
/*具体配置参照资源综述*/
typedef enum cm_keypad_kp_mkin{
	CM_KP_MKI0 = 0,
	CM_KP_MKI1,
	CM_KP_MKI2,
	CM_KP_MKI3,
	CM_KP_MKI4,
	CM_KP_MKI5,
	CM_KP_MKI6,
	CM_KP_MKI7,
	CM_KP_MKI_NO_VALID
}cm_keypad_kp_mkin_e;
    
/*具体配置参照资源综述*/
typedef enum cm_keypad_kp_mkout{
	CM_KP_MKO0 = 0,
	CM_KP_MKO1,
	CM_KP_MKO2,
	CM_KP_MKO3,
	CM_KP_MKO4,
	CM_KP_MKO5,
	CM_KP_MKO6,
	CM_KP_MKO7,
	CM_KP_MKO_NO_VALID
}cm_keypad_kp_mkout_e;

/** 配置 */
typedef struct{
    cm_keypad_kp_mkin_e *cm_col;
    cm_keypad_kp_mkout_e *cm_row;
    uint8_t cm_col_len;//*cm_col长度
    uint8_t cm_row_len;//*cm_cow长度
} cm_keypad_cfg_t;

/**
 *  @brief keypad事件回调函数
 *
 *  @param [in] key         键值
 *  @param [in] event       按键事件
 *
 */
typedef void (*cm_key_event_callback_t)(cm_keypad_map_t key, cm_keypad_event_e event);

/**
 *  @brief keypad配置
 *
 *  @param [in] cm_keypad_cfg  keypad 配置参数指针
 *
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details 需在cm_keypad_init函数之前使用
 */
 
int32_t cm_keypad_config(cm_keypad_cfg_t *cm_keypad_cfg);


/**
 *  @brief keypad初始化
 * 
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 */
int32_t cm_keypad_init(void);

/**
 *  @brief keypad事件回调注册
 *
 *  @param [in] cb  keypad回调函数
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details  最大允许注册的回调函数个数为10
 */
int32_t cm_keypad_register(cm_key_event_callback_t cb);

/**
 *  @brief keypad事件回调注销
 *
 *  @param [in] cb   keypad回调函数
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details  最大允许注册的回调函数个数为10
 */
int32_t cm_keypad_unregister(cm_key_event_callback_t cb);

/**
 *  @brief keypad去初始化
 *  
 *  @details  由于任务删除不能释放已经分配的内存，因此重复初始化keypad功能会造成内存泄露
 */
void cm_keypad_deinit(void);

/**
 *  @brief keypad休眠使能
 *
 *  @param [in] enable   1,切换为休眠唤醒中断函数;0 ,切换为唤醒状态下中断函数
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 *  @details  在进入休眠时需要将按键休眠使能，将按键回调函数指向唤醒中断；
 *            休眠状态下，按按键唤醒，唤醒之后需立即调用cm_keypad_sleep(0),将按键休眠去使能，且持有休眠锁，参照demo回调函数使用方式；
 *            在休眠进入的回调函数中调用cm_keypad_sleep(1)，将按键休眠唤醒使能。
 */
int32_t cm_keypad_sleep(int enable);



#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif /* __CM_KEYPAD_H__ */

/** @}*/


