/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
*                MODULE HEADER FILE
*******************************************************************************
*  COPYRIGHT (C) 2001 Intel Corporation.
*
*  This file and the software in it is furnished under
*  license and may only be used or copied in accordance with the terms of the
*  license. The information in this file is furnished for informational use
*  only, is subject to change without notice, and should not be construed as
*  a commitment by Intel Corporation. Intel Corporation assumes no
*  responsibility or liability for any errors or inaccuracies that may appear
*  in this document or any software that may be provided in association with
*  this document.
*  Except as permitted by such license, no part of this document may be
*  reproduced, stored in a retrieval system, or transmitted in any form or by
*  any means without the express written consent of Intel Corporation.
*
*  Title: time r
*
*  Filename: timer.h
*
*  Target, subsystem: Common Platform, HAL
*
*  Authors:  Yanai Oron, Valach Micha
*
*  Description:  Timer header file.
*
*  Last Modified: <date>
*
*  Notes:
******************************************************************************/

#ifndef _TIMER_H_
#define _TIMER_H_

#include "global_types.h"

#include "hal_cfg.h"

#include "timer_config.h"                /* the definitions were part of API once */

//#include "hal_config.h"
// Move out to hal_config.h
#if defined(_HERMON_) && !defined(_EP_)
#define TIMER_DSYTMR
#endif


/*------------ Global defines -----------------------------------------------*/
/*---------- Global constant declarations -----------------------------------*/
/*------------ Global macro definitions -------------------------------------*/
/*------------ Global type definitions --------------------------------------*/

typedef  enum
{
    TCR_1,                              // non drowsy TCR
    TCR_2,                              // non drowsy TCR
    TCR_3,                              // non drowsy TCR

#ifndef	NEZHA3_1826
    TCR_4, MAX_HW_TCR_NUMBER = TCR_4, TCR_EXT = TCR_4,
    TCR_5,
    TCR_6,
    
    MAX_TCR_NUMBER
#else
	MAX_HW_TCR_NUMBER, MAX_TCR_NUMBER = MAX_HW_TCR_NUMBER,TCR_EXT = MAX_HW_TCR_NUMBER
#endif

}TCR_NUMBER;


typedef enum
{
  MATCH_REG_0,
  MATCH_REG_1,
  MATCH_REG_2,
  MAX_MATCH_NUMBER
} MATCH_NUMBER;

typedef enum
{
  TIMER_RC_OK        =  0,
  TIMER_RC_NOT_VALID = -1,    // the specified timer NUMBER is  not valid
  TIMER_RC_BAD_VALUE = -2     // the specified timer VALUE is not valid
} TIMER_RETURN_CODE;

typedef enum
{
   TCR_CONFIGURE_OK   = 0,
   TCR_CONFIGURE_FAIL = -1,
   TCR_CONFIGURE_TCR_ERR_NUM = -2
}TIMER_TCR_CONFIGURE_RC;

typedef enum
{
   CLK_FAST,
   CLK_32KHZ,
   CLK_1KHZ,                  // accuracy  30% @ 1mSec, 6% @ 30mSec, 2% @ 100mSec
   //CLK_EXT,                 // Not implemented as external pin on Manitoba chip
   CLK_256HZ
} CLOCK_MODES;

typedef enum
{
    C_TCER_COUNT_DISABLED,                       // 0
    C_TCER_COUNT_ENABLED                         // 1
}TCER_MODES;

typedef enum
{
	TCR1_TSR_BIT_ON = 0x01,
	TCR2_TSR_BIT_ON = 0x02,
	TCR3_TSR_BIT_ON = 0x04,
#if !defined (EDEN_1928) && !defined(NEZHA3_1826)
	TCR4_TSR_BIT_ON = 0x08,
	TCR5_TSR_BIT_ON = 0x10,
	TCR6_TSR_BIT_ON = 0x20
#endif
} TEIR_BITS;


typedef enum
{
  C_TCMR_HW_DISABLED_MODE = 0x0,
  C_TCMR_HW_FREE_RUN_MODE = 0x01,
  C_TCMR_HW_PERIODIC_TCMR_MODE = 0x2,   // Accurated AutoLoad is done automatically by HW, using HW Timer Preload Value Register
  C_TCMR_ONE_SHOT_MODE = 0x3,
  C_TCMR_SW_PERIODIC_TCMR_MODE = 0x4   // Autoload is done by SW (= One Shot mode + SW periodic reload);
                                       // TPLCR value should be C_TPLCR_FREE_RUNNING (Used only by SW)
} TCMR_MODES;
//typedef  TCMR_COUNT_MODE TCMR_MODES ;

typedef enum
{
	TCMR_DISABLE_MODE       = 0 ,
	TCMR_EN_FREERUN_MODE    = 0x01,   /* Enable , free running (wraps at max value) */
	TCMR_EN_PERIOD_MODE     = 0x02,   /* Enable , periodic timer mode               */
	TCMR_EN_ONESHOT_MODE    = 0x03 ,  /* Enable , one-shot timer mode               */
	TCMR_EN_SW_PERIOD_MODE  = 0x04    /* Used only by S/W */
}
TCMR_COUNT_MODE;



typedef enum
{
  C_NON_DROWSY,
  C_DROWSY
}DROWSY_MODES;

typedef enum
{
  C_TPLCR_FREE_RUNNING,
  C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_O  // while using pre-load, the other two match registers are obsolete!!
  //C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_1,
  //C_TPLCR_ENABLE_PRELOAD_WITH_MATCH_2
}TPLCR_MODES;

/* The following values can be OR-combined to set the desired options in the interrupt_required field below */
typedef enum
{
  NONE = 0,                  /* "NONE" causes enum collision. It is OBSOLET, use TMR_INTERRUPT_NONE instead.*/
  TMR_INTERRUPT_NONE = 0,    /* used if no interrupt is required*/
  TO_INTERRUPT_REQUIRED = 1, /* for compatibility with old definition of the field - BOOL */
  TO_LISR_NOTIFICATION  = 2  /* if set, the notification callback will be called in ISR rather than in HISR */
}TimerOptions;

typedef void (*TIMER_CALLBACK_FUNCTION)(UINT8) ; //pointer to a callback function to be called upon expiration

typedef struct   /* This is structure of the TCR Configuration  */
{
   CLOCK_MODES  clock_mode;      // TCCR, clock modes for selected TCR:fast = vtcxo/4, 32Khz, 1Khz, 256 Hz and Ext.
   TCMR_MODES   tcmr_mode;       // TCMR, timer modes: One-Shot or HW Periodic or SW Periodic
   TPLCR_MODES  tplcr_mode;      // TPLCR pre-load control mode: Free-Running or Pre-Load with Match#0 (other two match not relevant).
   UINT32       tplvr_value;     // Timer Pre-Load value register.
   UINT32       microSecTimeInterval;   // [Usec] interval
   TIMER_CALLBACK_FUNCTION timerNotificationPtr;  // callback function
   UINT8        cookie;
   UINT8        interrupt_required;    // interrupt when expired?, notification in ISR or in HISR?
}TCR_CONFIGURATION;



/*---------------------------------------------------------------------------*/

/*---------- Extern definition ----------------------------------------------*/
#ifndef _TIMER_NO_EXTERN_
  #define EXTERN extern
#else
  #define EXTERN
#endif /*_TIMER_NO_EXTERN_*/

/*---------- Global variable declarations -----------------------------------*/

/*---------- Global constant declarations -----------------------------------*/



/*---------- Exported function prototypes -------------------------------------*/

//EXTERN void timerAPBClock(TCR_NUMBER tcr_number/*, PMUOnOff onOff*/); /* Used by the Watchdog timer only! */

EXTERN TIMER_TCR_CONFIGURE_RC timerTCRconfigure(TCR_NUMBER tcr_number, TCR_CONFIGURATION *tcr_configuration);

EXTERN TIMER_RETURN_CODE timerActivate (TCR_NUMBER tcr_number);

EXTERN TIMER_RETURN_CODE timerDeActivate (TCR_NUMBER tcr_number);

EXTERN UINT32 timerCountRead (TCR_NUMBER tcr_number);

EXTERN UINT32 timerClockRateGet(TCR_NUMBER tcr_number);

EXTERN UINT32 timerElapsedTimeGet (void);

EXTERN void timerPhase1Init(void);

EXTERN void timerPhase2Init(void);

EXTERN SwVersion timerVersionGet(void);

EXTERN	UINT32 timerDelayLite(UINT32 Usec);

EXTERN TIMER_TCR_CONFIGURE_RC timerTCRReConfigure(TCR_NUMBER tcr_number, TCR_CONFIGURATION *tcr_configuration);
extern void timerDelay(UINT32 USec);    // this is blocked service in [MicroSec]

#ifdef TIMER_TEST_MODE_ENABLED
TCMR_MODES timerDBModeGet(TCR_NUMBER tcr_number);
#endif

#define MAX_READING_ALLOWED_FOR_32K 8

#if defined TIMER_TEST_DVT_ENABLE
	UINT32 timerGetMatch (TCR_NUMBER tcr_number);
  	UINT32 timerTCMRGet(TCR_NUMBER tcr_number); //we add to timer.h so we can use it in timer_test_dvt.c test (change ret val to /uint32)
	UINT32 timerTCCRGet(TCR_NUMBER tcr_number); //we add to timer.h so we can use it in timer_test_dvt.c test
#endif

#undef EXTERN



/*
 *************** Config Macro ********************
 * 1. SUPPORT_ACC_TIMER
 * 2. ADD_ACC_TIMER_TEST_CASE
 *************************************************
 */
	
#define SUPPORT_ACC_TIMER
//#define ADD_ACC_TIMER_TEST_CASE


#ifdef SUPPORT_ACC_TIMER
#define ACC_TIMER_NODE_MAX 32		/* maximum number of nodes */
#define ACC_TIMER_NO_MEMORY 0             /* memory is not enough*/
#define ACC_TIMER_ERR_PARAMS (-1)   /* incorrect parameter*/
#define ACC_TIMER_ERR_CALL_CONTEXT	(-2)
#define ACC_TIMER_TIMER_ID_MAX 32
#define ACC_TIMER_TIMER_ID_MIN 1

#define HALF_RANGE_32_BIT 0x7FFFFFFF
#define RANGE_32_BIT 0x100000000	

#define FREQ 32768              //the timer's frequency
#define S_US 1000000            //unit converted from us to tick
#define TS_NODE_INTERVAL 8      //judege the node's match during TS_handler 
#define STOP_NODE_INTERVAL 8    //judege the node's match during stop 
#define MIN_PERIOD 5           //the minum period            

typedef void(*ACC_TIMER_CALLBACK)(UINT32);	/* Function pointer */

struct timer
{
	unsigned int start_time;				/* time to configure match */
	unsigned int end_time;					/* interrupt trigger time */
	unsigned int flag;						/* flag: determine whether the timer executes once or periodically */
	unsigned int period;					/* delta_time unit is us */
	unsigned int period_tick;               /* delta_time unit is tick */
	unsigned int magic;						/* node status: create, active, stop, empty */
	ACC_TIMER_CALLBACK timerCallbackFunc;	/* callback function pointer */
	unsigned int timerParams;				/* function parameter */
	unsigned int timer_id;                  /* timer id*/       
};

typedef struct timer Item;

struct _ACCTimerConfig					/* timer configuration */
{
	unsigned int flag;
	unsigned int period;
	ACC_TIMER_CALLBACK timerCallbackFunc;
	unsigned int timerParams;
};

struct _ACC_TIMER_NODE				/* node structure */
{
	struct _ACC_TIMER_NODE *previous;	/* front pointer */
	struct _ACC_TIMER_NODE *next;		/* back pointer */
	Item item;
};

typedef enum   /* The meaning of the API flag*/         
{
	ACC_TIMER_PERIODIC = 0x1,		/* periodic execution */
	ACC_TIMER_AUTO_DELETE = 0x2	    /* one execution */
}ACC_TIMER_FLAG;

/* enum is int type. if ACC_TIMER_ACTIVE = 0xAAAAAAAA, 
       will warning because enumeration is out of 'int' range. (enum is started from 0)
    */    
typedef enum 	/* The value of enum member's value is sorted by value from small to large  */
{
	ACC_TIMER_EMPTY = 0x0,				/* the status of empty node */
	ACC_TIMER_ACTIVE = 0xAAAA,		    /* the status of active node */
	ACC_TIMER_CREATE = 0xCCCC,		    /* the status of active node */
	ACC_TIMER_STOP = 0xDDDD		        /* the status of stop node */	
}ACC_TIMER_MAGIC;

typedef enum    /* determine whether the timer is started or not*/
{
    ACC_ACTIVE,                      /* the node has been added in the active_list and callback function has not been executed */
    ACC_INACTIVE,                    /* the node is not in the active_list*/
    ACC_TIMER_ID_NOT_EXIST           /* timer_id does not exist */                 
}ACC_TIMER_STATUS;

typedef int TIMER_STATUS;       /* timer's function execution status*/
typedef int TIMER_ID;
typedef struct _ACC_TIMER_NODE ACC_TIMER_NODE;
typedef struct _ACCTimerConfig ACCTimerConfig;
void AccTimerInitilize(void);
TIMER_STATUS AccTimerCreate(ACCTimerConfig *pTimerCfg);
TIMER_STATUS AccTimerDelete(TIMER_ID acc_timer_id);
TIMER_STATUS AccTimerStart(TIMER_ID acc_timer_id,ACCTimerConfig *pTimerCfg);
TIMER_STATUS AccTimerStop(TIMER_ID acc_timer_id);
TIMER_STATUS AccTimerStartEx(
								unsigned int flag,
								unsigned int period,
								ACC_TIMER_CALLBACK timerCallbackFunc,
								unsigned int timerParams
							       );
ACC_TIMER_STATUS GetTimerStatus(TIMER_ID acc_timer_id);
void TShandler(void);

#ifdef ADD_ACC_TIMER_TEST_CASE
void test_AccTimer(void);
#endif

#endif /* SUPPORT_ACC_TIMER */

UINT32  timerGetCount(TCR_NUMBER tcr_number);

#endif  /* _TIMER_H_ */

