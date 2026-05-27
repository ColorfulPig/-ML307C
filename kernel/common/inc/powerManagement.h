/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************//*                                                                      */
/* Title: Power Manager Header File -For CLIENT (ext)                   */
/*                                                                      */
/* Filename: powerManagement.h                                                       */
/*                                                                      */
/* Author:   Yaeli Karni                                                */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Boerne, PM Module  		    	*/
/*												                        */
/* Remarks: -                                                           */
/*    										                        	*/
/* Created: 28/12/2006                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/
#if !defined PowerM_H
#define PowerM_H

#define PM_EXTERN extern

#include "global_types.h"

    // This enum describes the different power states the PM controls.
    // (Both Comm SS, Arbel core, Boerne core and other platforms (hopefully) !)
    typedef enum
    {
		PM_PS_OPERATIONAL_D0=0,
		PM_PS_CORE_LPM_C1,
		PM_PS_D1,
		PM_PS_D2,
		PM_PS_D3,
		PM_PS_D4,

		/* For general WakeUp register that is active from any Dx state to any Dx state */
		PM_PS_ANY_DSTATE,

		/* on exit from LPM - special transition values */
		PM_PSEXIT_C1,
		PM_PSEXIT_D2,
		PM_PSEXIT_D2_interrupted,	// Harbell 
		PM_PSEXIT_D3,

		PM_NUM_OF_POWER_STATES 

    }PM_PowerStatesE;

/*----------- Global type definitions ----------------------------------------*/

// This enumerator describes the various values that may be returned.
//ICAT EXPORTED ENUM
typedef enum
{
    PM_RC_OK = 0,
	PM_RC_FAIL	,			//General Failure
	PM_RC_ALREADY_EXISTS	//Exit function since required target alrteady exists
}PM_ReturnCodeE;

// Reasons of Last Reset -
// TEMPORARY DEFINITION from previous version of module
typedef enum
{
	PM_RR_UNKNOWN = 0,
	PM_RR_POWER_ON_RESET,
	PM_RR_COMM_SUBSYSTEM_RESET,
	PM_RR_WATCHDOG_TIMEOUT_RESET,
	PM_RR_LPM_EXIT_RESET,			
	PM_RR_GPIO_RESET
}PM_LastResetReasonE;

/*----------- Global function prototypes -------------------------------------*/

typedef void (*PM_CallbackFuncDDRstateT)(BOOL b_DDR_ready);

// Package specific APIs.
PM_EXTERN PM_LastResetReasonE PMlastResetReason(void);

PM_EXTERN void PMregisterACIPCDDRstateCB(PM_CallbackFuncDDRstateT notify_cb);

// interface to BSP for init of PM
// defined in pmu.h of hal with return-code: void PMUPhase1Init(void);
void PMUPhase2Init(void);
// must be called at end of init - to release the frequency change logic
void PMUendInit(void);
// disable all PM features - to be used on extrem cases as -  from EE on WDT interrupt?
void PMdisablePMfeautures(void);	// disable all PM features

#undef PM_EXTERN

#endif  /* PowerM_H */
