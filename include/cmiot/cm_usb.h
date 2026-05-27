/**
 *  @file    cm_usb.h
 *  @brief   usb虚拟串口功能
 *  @copyright copyright © 2023 China Mobile IOT. All rights reserved.
 *  @author by ZXW
 *  @date 2023/8/4
 *  
 */


#ifndef __CM_USB_H__
#define __CM_USB_H__


#include <stddef.h>


#define CM_USB_EVT_INSERT   1
#define CM_USB_EVT_REMOVE   0


/** 定义USB接收回调*/
typedef void (*cm_usb2com_recv_cb)(void *data, int32_t len);

/** 定义USB插拔事件回调*/
typedef void (*cm_usb2com_status_cb)(int32_t evt);


/**
 *  @brief 注册USB2COM接收回调函数
 *  
 *  @param [in] recv_cb 接收回调函数
 *
 *  @return None
 *  
 *  @details 禁止跨多线程使用
 */
void cm_usb2com_register_recv_cb(cm_usb2com_recv_cb recv_cb);

/**
 *  @brief USB2COM发送数据
 *  
 *  @param [in] data 待发送数据
 *  @param [in] len 数据长度
 *
 *  @return 成功返回0，失败返回负值
 *  
 *  @details 禁止跨多线程使用
 */
int32_t cm_usb2com_send_data(void *data, int32_t len);

/**
 *  @brief 注册USB2COM插拔事件回调函数
 *  
 *  @param [in] status_cb 插拔事件回调函数
 *
 *  @return None
 *  
 *  @details 禁止跨多线程使用
 */
void cm_usb2com_register_status_cb(cm_usb2com_status_cb status_cb);

/**
 *  @brief 检测USB插拔状态
 *  
 *
 *  @return 0：USB拔出 \n
 *          1：USB插入
 *  
 *  @details 禁止跨多线程使用
 */
int32_t cm_usb2com_get_status(void);

#endif

