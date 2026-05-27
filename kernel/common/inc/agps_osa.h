/*
 * agps_osa.h
 *
 *  Created on: 2021Äê1ÔÂ15ÈÕ
 *      Author: huizhang
 */

#ifndef AGPS_OSA_H_
#define AGPS_OSA_H_
#ifdef OS_RTOS
#include <global_types.h>
//#include <pmic_rtc.h>
#include "errno.h"
#define  EPERM            1  /* Operation not permitted */
typedef unsigned int time_t;
#include "utlTypes.h"
#else
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
typedef unsigned char BOOL;
typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT;
typedef unsigned int UINT32;
typedef signed char  INT8;
typedef signed short INT16;
typedef unsigned int OSA_STATUS;
#endif
typedef UINT8   CiBoolean;
typedef unsigned int UINT;
#define FALSE 0
#define TRUE 1


#endif /* AGPS_TIME_H_ */
