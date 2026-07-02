#ifndef __CUSTOM_DATETIME_H__
#define __CUSTOM_DATETIME_H__

#include "cm_rtc.h"
#include "cm_os.h"

/* 将秒数时间戳转换为日期时间结构。 */
void custom_seceond_to_datetime(long seconds, cm_tm_t *tTime);
/* 读取当前系统时间并转换为日期时间结构。 */
void custom_get_now_datetime(cm_tm_t *dt);

#endif

