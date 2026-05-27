/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title: Watch Dog Timer Driver Header
*
* Filename: watchdog.h
*
* Target, platform: WhiteSail
*
* Authors:	Eilam Ben-Dror
*
* Description: This file is the watchdog driver's header file
*
* Last Updated:
*
* Notes:
*******************************************************************************/



/******************************************************************************
*				OVERVIEW
*******************************************************************************

  The Watchdog Timer (WDT) is a programmable timer, which is used for resetting
the system whenever the SW fails to prevent it.

******************************************************************************/

#ifndef _WATCHDOG_H_
#define _WATCHDOG_H_

#include "global_types.h"


/*----------- Global definitions ---------------------------------------------*/

// Comment this flag definition to avoid parameters checks:
#define _WATCHDOG_CHECK_PARAMETERS_FLAG_


// Maximal double read trials, to avoid metastability of WDT counter value:
#define WATCHDOG_DOUBLE_READ_MAX_COUNT          1000

/*----------- Global type definitions ----------------------------------------*/

// Mode of operation
typedef enum
{
	WATCHDOG_INTERRUPT_MODE = 0,
	WATCHDOG_RESET_MODE

}WATCHDOG_Mode;




// Configuration structure:
typedef struct
{
	UINT32 matchValue;	     // Number of milliseconds from kick until reset/interrupt is generated
    WATCHDOG_Mode mode;      // Reset or Interrupt

}WATCHDOG_Configuration;




// Status structure:
typedef struct
{
	UINT32 timeTillMatch;	// current counter value
	BOOL resetIndication;	// TRUE: WDT reset occurred since last clear

}WATCHDOG_Status;

// Status structure:
typedef enum
{
    WATCHDOG_STOP,
    WATCHDOG_RUNNING,
    WATCHDOG_EXPIRED,
    WATCHDOG_EXPIRED_IN_DEBUG
}WATCHDOG_RunState;

#define WATCHDOG_MIN_TIME ((WATCHDOG_CLOCK_FACTOR+WATCHDOG_CLOCK_RATE-1)/WATCHDOG_CLOCK_RATE) // 4ms ~= 1 tick of 256Hz clock
#define WATCHDOG_MAX_TIME (((UINT32)0xFFFF*WATCHDOG_CLOCK_FACTOR)/WATCHDOG_CLOCK_RATE)        // 255,996ms


// Return Codes:
typedef enum
{
	WATCHDOG_RC_OK = 1,

    WATCHDOG_RC_NO_HANDLER_REGISTERED = -100,
    WATCHDOG_RC_NULL_POINTER,
	WATCHDOG_RC_INTC_ERROR,
	WATCHDOG_RC_BAD_MATCH_VALUE

}WATCHDOG_ReturnCode;




// Watchdog Interrupt Handler type:
typedef void (*WATCHDOG_Handler)(void);



/*----------- Global macro definitions ---------------------------------------*/

/*----------- Global function prototypes -------------------------------------*/

void                watchdogKick             (void);
void                watchdogIndicationClear  (void);
WATCHDOG_ReturnCode watchdogDeactivate       (void);
WATCHDOG_ReturnCode watchdogActivate         (void);
WATCHDOG_ReturnCode watchdogConfigurationGet (WATCHDOG_Configuration * config);
WATCHDOG_ReturnCode watchdogConfigure        (WATCHDOG_Configuration * config);
WATCHDOG_ReturnCode watchdogRegister         (WATCHDOG_Handler handler);
WATCHDOG_ReturnCode watchdogPhase2Init       (void);
WATCHDOG_ReturnCode watchdogPhase1Init       (void);
WATCHDOG_ReturnCode watchdogStatusGet        (WATCHDOG_Status * status);
WATCHDOG_RunState   watchdogRunStateGet      (void);

// Special case: WDT has been set and is expected to expire (used by EE)
BOOL                watchdogProtect          (BOOL newBlockBool); //set new value, returns previous

void                watchdogReset            (void);


#endif	/* _WATCHDOG_H_ */
