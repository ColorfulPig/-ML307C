#ifndef __CUSTOM_DATETIME_H__
#define __CUSTOM_DATETIME_H__

#include "cm_rtc.h"
#include "cm_os.h"

void custom_seceond_to_datetime(long seconds, cm_tm_t *tTime);
void custom_get_now_datetime(cm_tm_t *dt);

#endif

