/**
 *  @file    cm_cpuinfo.h
 *  @brief   cpu信息统计，禁止跨线程使用
 *  @copyright copyright © 2023 China Mobile IOT. All rights reserved.
 *  @author by ZXW
 *  @date 2023/6/29
 */


#ifndef __CM_CPUINFO_H__
#define __CM_CPUINFO_H__


#include <stdint.h>
#include "cm_os.h"


/** 线程信息*/
typedef struct{
    osThreadId_t handle;        /*!< 线程句柄*/
    uint32_t scheduled_cnt;     /*!< 系统调度次数*/
    uint32_t tick_cnt;          /*!< 累计占用tick计数，32KHZ*/
    uint32_t last_tick;         /*!< 线程切入时系统tick计数，32KHZ*/
} cm_thread_runtime_t;


/**
 *  @brief 开始统计CPU运行信息
 *  
 *  @param [in] mode 统计模式，0：活跃线程；1：全部线程
 *
 *  @return 0：成功 \n
 *       其他：失败
 *  
 *  @details 开启记录后线程调度可能受影响
 */
int32_t cm_cpuinfo_record_start(uint32_t mode);

/**
 *  @brief 读取统计信息
 *  
 *  @param [out] info 存储线程信息
 *  @param [out] cnt 线程数量
 *  @param [out] total_tick 从记录开始至读取时CPU的tick计数
 *  
 *  @return 0：成功 \n
 *       其他：失败
 *  
 *  @details cpu占用率 = cm_thread_runtime_t->tick_cnt / total_tick * 100%
 */
int32_t cm_cpuinfo_record_read(cm_thread_runtime_t info[], uint32_t *cnt, uint32_t *total_tick);

/**
 *  @brief 结束运行信息统计
 *  
 *  @return 0：成功 \n
 *       其他：失败
 *  
 *  @details More details
 */
int32_t cm_cpuinfo_record_stop(void);

/**
 *  @brief 获取cpu唯一标识
 *  
 *  @return 非空字符串：成功 \n
 *          NULL：失败
 *  
 *  @details CPU序列号
 */
const char *cm_cpuinfo_get_cpuid(void);

#endif
