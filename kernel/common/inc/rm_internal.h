/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************/
/*  COPYRIGHT (C) 2002 Intel Corporation.                               */
/*                                                                      */
/*  This file and the software in it is furnished under                 */
/*  license and may only be used or copied in accordance with the terms */
/*  of the license. The information in this file is furnished for       */
/*  informational use only, is subject to change without notice, and    */
/*  should not be construed as a commitment by Intel Corporation.       */
/*  Intel Corporation assumes no responsibility or liability for any    */
/*  errors or inaccuracies that may appear in this document or any      */
/*  software that may be provided in association with this document.    */
/*  Except as permitted by such license, no part of this document may   */
/*  be reproduced, stored in a retrieval system, or transmitted in any  */
/*  form or by any means without the express written consent of Intel   */
/*  Corporation.                                                        */
/*                                                                      */
/* Title: Arbel Resource Manager Header File                            */
/*                                                                      */
/* Filename: rm_internal.h                                                     */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     			    	*/
/*												                        */
/* Remarks: -                                                           */
/*    										                        	*/
/* Created: 11/4/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/


#if !defined _RM_INTERNAL_H_
#define _RM_INTERNAL_H_

#include "global_types.h"
#include "powerManagement.h"
#include "rm_config.h"
#include "commpm.h"

/*----------- Global defines -------------------------------------------------*/
#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined RM_DEBUG_MODE_ENABLED))
//#define RM_DEBUG_MODE_ENABLED // not automatic - consume to much TCM
#endif


/*----------- Global macro definitions ---------------------------------------*/

/*----------- Global type definitions ----------------------------------------*/

// This enumerator describes the various values that may be returned.
typedef enum
{
	RM_RC_OK = 0,
	RM_RC_RESET_NOT_SUPPORTED,
	RM_RC_ERR_CLOCK = -100,
	RM_RC_ERR_FREQ,
	RM_RC_ERR_NULL_POINTER,
	RM_RC_WAKEUP_NOT_SUPPORTED,
	RM_RC_SERVICE_NOT_SUPPORTED,
	RM_RC_ERR_CPMU
}RM_ReturnCodeE;

#ifdef PHS_SW_DEMO_TTC_PM
// This enumerator describes Arbel's services.
//ICAT EXPORTED ENUM
typedef enum
{
	// APB Application Subsystem 
	RM_SRVC_UART0,
	RM_SRVC_UART1,
	RM_SRVC_GPIO,
	RM_SRVC_PWM0,
	RM_SRVC_PWM1,
	RM_SRVC_PWM2,
	RM_SRVC_PWM3,
	RM_SRVC_SSP0,
	RM_SRVC_SSP1,
	RM_SRVC_ACIPC,
	RM_SRVC_RTC,
	RM_SRVC_TWSI_HS,
	RM_SRVC_KPC,
	RM_SRVC_TIMERS_AS,
	RM_SRVC_TB_ROTARY,
	RM_SRVC_AIB,
	RM_SRVC_SW_JTAG,
	RM_SRVC_TIMERS1_AS,
	RM_SRVC_ONEWIRE,
	RM_SRVC_SSP2,

	// APB Communication Subsystem 	
	RM_SRVC_TCU,
	RM_SRVC_AIRQ,
	RM_SRVC_UART2, // Seagull
	RM_SRVC_USIM1,
	RM_SRVC_USIM2,
	RM_SRVC_TWSI,
	RM_SRVC_ECIPHER,
	RM_SRVC_TIMER_CS,
	RM_SRVC_GSSP,
	RM_SRVC_IPC,

	// Main PMU
	RM_SRVC_WDTPCR,
	
	//Application Subsystem PMU
	RM_SRVC_IRE,
	RM_SRVC_LCD,
	RM_SRVC_CCIC,
	RM_SRVC_SDH0,	
	RM_SRVC_SDH1,
	RM_SRVC_USB,
	RM_SRVC_NF,
	RM_SRVC_DMA,		
	RM_SRVC_GEU,		
	RM_SRVC_DTC,
	RM_SRVC_GC,
	RM_SRVC_SMC,
	RM_SRVC_RTU,	
	
	RM_NUM_OF_SRVCS,
	RM_INVALID_SRVCS = RM_NUM_OF_SRVCS, 
	RM_SRVC_VCTCXO = RM_INVALID_SRVCS,
	RM_SRVC_TIMER0_13M = RM_INVALID_SRVCS,
	RM_SRVC_TIMER2_13M_GB = RM_INVALID_SRVCS,
	RM_SRVC_DDR_HIGH_FREQ = RM_INVALID_SRVCS 
	
}RM_ServiceIdE;

#else
// This enumerator describes Arbel's services.
//ICAT EXPORTED ENUM
typedef enum
{
	RM_SRVC_DSSP3 = 0,
	RM_SRVC_GSSP2,
	RM_SRVC_I2C,
    RM_SRVC_MSL,
    RM_SRVC_USIM,
    RM_SRVC_TIMER0_13M,
    RM_SRVC_TIMER1_13M,
    RM_SRVC_DDR_HIGH_FREQ, //RM_SRVC_IPC,
	RM_SRVC_CGPIO,
    RM_SRVC_DMA,
    RM_SRVC_TCU,
    RM_SRVC_SCK,
    RM_SRVC_WB_SLEEP_MODULE,
    RM_SRVC_VCTCXO,
    RM_SRVC_GSSP1_GB,
    RM_SRVC_WB_CIPHER_GB,
    RM_SRVC_CPA_DDR_HPerf,      //for L1 to control high freq DDR
    RM_SRVC_FREE_3,             //RM_SRVC_VITERBI_GB, rafiagiv 28.12.06
    RM_SRVC_FREE_4,             //RM_SRVC_EQUALIZER_GB, rafiagiv 28.12.06
    RM_SRVC_DSSP2_GB,
    RM_SRVC_DSSP1_GB,
    RM_SRVC_DSSP0_GB,
    RM_SRVC_TTPCOM_GB,
    RM_SRVC_TIMER2_13M_GB,
    RM_SRVC_TIMER3_13M_GB,
	RM_SRVC_UART,
	RM_SRVC_DVFM,
	RM_SRVC_ECIPHER,                // PRM_SRVC_ECIPHER   for PV only - defined because chip identification done in run time
	RM_SRVC_USIM2,                  // PRM_SRVC_USIM2   for PV only - defined because chip identification done in run time
	RM_SRVC_SDIO,                 	// PRM_SRVC_SDIO   for PV only  - defined because chip identification done in run time
	RM_NUM_OF_SRVCS,
	RM_INVALID_SRVCS = RM_NUM_OF_SRVCS
}RM_ServiceIdE;
#endif

// This enumerator describes the services that are allowed to register
// callback functions for D2 entry/exit. We need a different enumerator
// for this as some of the above services, like the SSPs, are logically
// sub-services of one service.
// The order of enum determines calls for D2-prepare and D2-recovery.
// There might be dependencies with in the APIs (especially with INTC)
// Thus, we try to set it on the order this id done on init!

//ICAT EXPORTED ENUM
typedef enum
{
	RM_CALLBACK_SRVC_INTC = 0,		/* MUST BE FIRST - it saves data while request are
									done, thus if request to enable/disable interrupt
									will come from onother source (on there exit routine)
									the data to restore interrup controler will be ruined */
									RM_CALLBACK_SRVC_TIMER,            /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_SSP,              /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_DMA,
									RM_CALLBACK_SRVC_I2C,              /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_WDT,              /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_IPC,
									RM_CALLBACK_SRVC_USIM,             /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_PMIC,             /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_MSL,              /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_SCK,              /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_WB_SLEEP_MODULE,  /** This service may register for wakeup **/
									RM_CALLBACK_SRVC_LTE_SLEEP_MODULE,
									RM_CALLBACK_SRVC_WB_LTE_SLEEP_MODULE,
									RM_CALLBACK_SRVC_TCU,
									RM_CALLBACK_SRVC_UART,
									RM_CALLBACK_SRVC_HSI,
									RM_CALLBACK_SRVC_AC_IPC,			/* Apps-Comm IPC service wakeup */ // YKYK TODO
									RM_CALLBACK_SRVC_CGPIO,				/** This service may register for wakeup **/
									RM_NUM_OF_CALLBACK_SRVCS,
									RM_INVALID_CALLBACK = RM_NUM_OF_CALLBACK_SRVCS

}RM_ServiceCallbackE;

// This enumerator describes the available services' clock frequencies.
typedef enum
{
	RM_FREQ_13MHZ = 0,
	RM_FREQ_26MHZ,
	RM_FREQ_52MHZ,
	RM_FREQ_78MHZ,
	RM_FREQ_104MHZ,
	RM_NUM_OF_FREQS,
	RM_INVALID_FREQ=RM_NUM_OF_FREQS
}RM_ServiceFreqE;

// This enumerator describes the requests made by services.
typedef enum
{
	RM_RSRC_ALLOC = 0,
	RM_RSRC_FREE
}RM_AllocFreeE;

// Bit vector describing the current resources status.
typedef UINT32 RM_CurrentResourcesStatusT;

// This enumerator describes the request types to the logger
typedef enum
{
	RM_LOGGING_START = 0,
	RM_LOGGING_STOP
}RM_LogReqE;

// This type definitions describes the resources log.

typedef RM_CurrentResourcesStatusT RM_ResourcesArrT[RM_LOG_SIZE];

typedef struct
{
	RM_ResourcesArrT  resourcesArr;
	INT32             nextEntry;
}RM_ResourcesLogS;

// This type definition describes the RM callback functions type.
typedef void (*RM_CallbackFuncT)(void);

// This type definition describes the RM D2 Recover callback functions type.
typedef void (*RM_D2RecoverCallbackFuncT)(PM_PowerStatesE psType);

// This enumerator describes the service's status.
typedef enum
{
	RM_SRVC_BUSY = 0, // resources are allocated for this service.
	RM_SRVC_FREE      // no resources are allocated for this service.
}RM_ServiceStatusE;

/*----------- Extern definition ----------------------------------------------*/

/*----------- Global variable declarations -----------------------------------*/

/*----------- Global constant definitions ------------------------------------*/

/*----------- Global function prototypes -------------------------------------*/
// Init functions.
void                  RMPhase1Init        (void);
void                  RMPhase2Init        (void);

// Package specific APIs.
void                  RMManage            (RM_ServiceIdE serviceId,
										   RM_AllocFreeE allocFree);
void				  RMLPMexcpetion	  (RM_ServiceIdE serviceId, RM_AllocFreeE allocFree);
RM_ReturnCodeE        RMServiceFreqSelect (RM_ServiceIdE   serviceId,
										   RM_ServiceFreqE newServiceFreq);
RM_ServiceFreqE RMServiceFreqGet (RM_ServiceIdE      serviceId);
RM_ReturnCodeE        RMSWReset           (RM_ServiceIdE serviceId);
BOOL				  RMIsResourceFree		(RM_ServiceIdE serviceId);
BOOL				  RMIsResourceBusy		(RM_ServiceIdE serviceId);
BOOL				  RMIsResourceMulti		(RM_ServiceIdE serviceId);

// D2 related functions.
BOOL                  RMIsD2Allowed       (void);
void                  RMD2Unregister      (RM_ServiceCallbackE service);
void                  RMD2Prepare         (void);
void				  RMD2PrepareWithFastClk(void);
void                  RMD2Recover         (PM_PowerStatesE     psType);
void                  RMD2RecoverWithFastClk(PM_PowerStatesE psType);
void				  RMD2RecoverINTC(PM_PowerStatesE psType);
RM_ReturnCodeE        RMWakeup            (RM_ServiceCallbackE serviceId);
BOOL				RMwuEnabled(RM_ServiceCallbackE serviceId, CommPM_WakeupSourcesE * commPMsw);

void 				RMBeforeIntDisRegister (RM_ServiceCallbackE cbkServiceId,
											RM_CallbackFuncT    cbkBeforeIntDis);

// Internal APIs for move toward 'PRM' interface (not 'RM')
void RMD2RegisterINT (RM_ServiceCallbackE cbkServiceId,
					  void *    cbkPrepare,
					  void * cbkRecover);

void RMD2WakeupControlINT(RM_ServiceCallbackE cbkServiceId,
						  void *   cbkWakeup);
void RMDoBeforeIntDis(void);
// Debug functions.
#if defined(RM_DEBUG_MODE_ENABLED)
RM_ReturnCodeE    RMCurrentResourcesStatusGet(RM_CurrentResourcesStatusT* pCurrentResourcesStatus );
void              RMResourcesStatusLog(RM_LogReqE logReq);
RM_ReturnCodeE    RMResourcesStatusLogGet(RM_ResourcesLogS**  ppResourcesLog);
void              RMResourcesStatusLogICATDisplay(void);
RM_ServiceStatusE RMServiceStatusExtract (RM_ServiceIdE serviceId,
										  RM_CurrentResourcesStatusT resourcesStatus);
// Test functions
void                  RMTest(void);
#endif

#endif  /* _RM_INTERNAL_H_ */
