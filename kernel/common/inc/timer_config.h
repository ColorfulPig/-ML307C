/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _TIMER_CONFIG_H
#define _TIMER_CONFIG_H






#define OS_TIMER_ID     TCR_1
#define TS_TIMER_ID     TCR_2
#define GB_FIRST_TIMER  TCR_3
#ifndef PHS_SW_DEMO_TTC
#define GB_SECOND_TIMER TCR_4
#endif
/* will be use by L1 code to configure timer for fast calibration needs*/
#if defined (EDEN_1928) || defined (NEZHA3_1826)
#define FAST_CAL_TIMER GB_FIRST_TIMER
#else
#define FAST_CAL_TIMER  TCR_4 //GB_FIRST_TIMER
#endif

//#define MSA_TIMERS_TWO_BIT_CLEAR_MASK  ~( (0x3<<(GB_FIRST_TIMER *2)) | (0x3<<(GB_SECOND_TIMER *2)) )
//#define MSA_TIMERS_ONE_BIT_CLEAR_MASK  ~( (0x1<<(GB_FIRST_TIMER )) | (0x1<<(GB_SECOND_TIMER )) )

/* Harbell will use TCR_3 for fast calibration only*/
#define MSA_TIMERS_TWO_BIT_CLEAR_MASK  ~( 0x3<<(GB_SECOND_TIMER *2))
#define MSA_TIMERS_ONE_BIT_CLEAR_MASK  ~( 0x1<<(GB_SECOND_TIMER )) 
// OS tick interval (in mSec)
#define OS_TICK_INTERVAL_IN_MILISEC   5 // also for FDI use


#define TIMER_HISR_PRIORITY 1

#endif
