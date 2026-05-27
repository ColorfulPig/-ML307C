#ifndef __CUSTOM_SYSTEM_H__
#define __CUSTOM_SYSTEM_H__

#include "cm_os.h"
#include "cm_sys.h"
#include "cm_mem.h"
#include "cm_sim.h"
#include "cm_fs.h"
#include "cm_virt_at.h"
#include "cm_rtc.h"


#define	ONE_SECONED			(1000 / 5)	// 1000ms
#define	ONE_SECONED_1_4		(50)		// 250ms
#define	ONE_SECONED_3_4		(150)		// 750ms
#define	ONE_SECONED_1_2		(100)		// 500ms
#define	ONE_SECONED_5_100	(10)		// 50ms
#define	ONE_SECONED_195_100	(390)		// 1950ms

int custom_system_init(void);
void custom_system_virt_at_usb(void);


#endif


