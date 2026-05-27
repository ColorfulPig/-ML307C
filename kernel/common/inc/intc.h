/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: Interrupt Controller Header File                              */
/*                                                                      */
/* Filename: Intc.h                                                     */
/*                                                                      */
/* Author: Yossi Gabay                                                  */
/*         Eilam Ben-Dror                                               */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/
#ifndef _INTC_H_
#define _INTC_H_

#include "global_types.h"
#ifdef MACRO_FOR_LTG
	#include "nucleus.h"
#endif
#include "powerManagement.h"
// needed to prevent errors in link? diag does not compile well...(include tick_manager.h which
// solves the problem (Jan 2007 - YK)
#include "pm_config.h"

#include "intc_list.h"
#include "intc_list_xirq.h"
#include "xirq_config.h"

#include "gpio.h"


/*-- Type definitions (T type) -----------------------------------------*/
// Return code of INTC package operations
typedef enum
{
    INTC_RC_OK = 1,

    INTC_RC_INVALID_SRC = -100,
	INTC_RC_NOT_AVAILABLE,
	INTC_RC_NO_ISR_BOUND,
    INTC_RC_ISR_ALREADY_BOUND,
    INTC_RC_BAD_ISR_PARAMETER,
    INTC_RC_NULL_POINTER,
    INTC_RC_PRIORITY_TABLE_ERROR,
	INTC_RC_WRONG_PARAMETER		// cover all cases of wrong configuration setting
}INTC_ReturnCode;

typedef enum
{
    INTC_IRQ = 0,
    INTC_FIQ
}INTC_FastNormal;

typedef enum
{
    INTC_RISING_EDGE = 0,
    INTC_FALLING_EDGE,
    INTC_BOTH_EDGE,
    INTC_HIGH_LEVEL,
    INTC_LOW_LEVEL,
    INTC_BOTH_LEVEL
}INTC_ActivationType;

typedef UINT32          INTC_InterruptPriorityTable[MAX_INTERRUPT_CONTROLLER_SOURCES];

/* The parameter that being sent to FISR, with some interrupt info. View API for details */
typedef UINT32          INTC_InterruptInfo;

// Callback function prototype for the interrupt handlers
// source - the interrupt source number that invoke the interrupt
typedef void (*INTC_ISR)(INTC_InterruptInfo interruptInfo);


/*----------------------------------------------------------------------*/


/*-- Constants definitions (C type) ------------------------------------*/

/*----------------------------------------------------------------------*/


/*-- Global variables declarations (Public G type) ---------------------*/

/*----------------------------------------------------------------------*/


/*-- Public Functions Prototypes ---------------------------------------*/
INTC_ReturnCode INTCPhase1Init(void);
void            INTCPhase2Init(void);

// ----------- added by PHS_SW_DEMO_TTC
#if defined(INTC_CONFIGURE_WITH_3_ARGUMENTS)
INTC_ReturnCode INTCConfigure(INTC_InterruptSources isrSource, INTC_FastNormal fastNormal,
                              INTC_ActivationType activationType);
INTC_ReturnCode INTCConfigureFiq(INTC_InterruptSources isrSource, INTC_ActivationType activationType);

INTC_ReturnCode INTCConfigurationGet(INTC_InterruptSources isrSource, INTC_FastNormal *fastNormal,
                                     INTC_ActivationType *activationType);//, GPIO_Debounce *debouce);/*AlexCheck*/
#else
INTC_ReturnCode INTCConfigure(INTC_InterruptSources isrSource, INTC_FastNormal fastNormal,
                              INTC_ActivationType activationType, GPIO_Debounce debouce);
INTC_ReturnCode INTCConfigureFiq(INTC_InterruptSources isrSource, INTC_ActivationType activationType, GPIO_Debounce debouce);

INTC_ReturnCode INTCConfigurationGet(INTC_InterruptSources isrSource, INTC_FastNormal *fastNormal,
                                     INTC_ActivationType *activationType, GPIO_Debounce *debouce);
#endif
//----------------------------------------
INTC_ReturnCode INTCPriorityTableSet(const INTC_InterruptPriorityTable table);

void            INTCPriorityTableGet(INTC_InterruptPriorityTable table);

INTC_ReturnCode INTCBind(INTC_InterruptSources isrSource, INTC_ISR isr);

INTC_ReturnCode INTCUnbind(INTC_InterruptSources isrSource);

INTC_ReturnCode INTCISRGet(INTC_InterruptSources isrSource, INTC_ISR *isr);

INTC_ReturnCode INTCDisable(INTC_InterruptSources isrSource);

INTC_ReturnCode INTCEnable(INTC_InterruptSources isrSource);

INTC_ReturnCode INTCIdleMaskDisable(void);

INTC_ReturnCode INTCIdleMaskEnable(void);

INTC_ReturnCode INTCSourceToGPIOPinConvert(INTC_InterruptSources source, GPIO_PinNumbers *pin);

INTC_ReturnCode INTCSourceFromGPIOPinConvert(GPIO_PinNumbers pin,INTC_InterruptSources *source);

INTC_ReturnCode INTCClear(INTC_InterruptSources source);
#define INTCClrSrc INTCClear

INTC_ReturnCode INTCDisableAllInts(void);


#include "intc_xirq.h"
 #endif  /* _INTC_H_ */


