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


#if !defined _RM_GEAGULL_INTERNAL_H_
#define _RM_GEAGULL_INTERNAL_H_

#include "global_types.h"
#include "powerManagement.h"
#include "rm_config.h"
#include "commpm.h"

/*----------- Global defines -------------------------------------------------*/
//#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined RM_DEBUG_MODE_ENABLED))
#if 0
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

// This enumerator describes Arbel's services.
//ICAT EXPORTED ENUM
typedef enum
{
    RM_SRVC_TCU_CLK = 0,
    RM_SRVC_AIRQ_CLK,
    RM_SRVC_UART_CLK,
	RM_SRVC_USIM1_CLK,
	RM_SRVC_USIM2_CLK,
	RM_SRVC_I2C_CLK,
	RM_SRVC_ECIPHER_CLK,
	RM_SRVC_TIMER_CLK,   //(possibly change to timer 1,2,3
	RM_SRVC_GSSP_CLK,
	RM_SRVC_COMM_IPC_CLK,
	RM_SRVC_SCK_CLK, //10
	RM_SRVC_MSL_CLK,
	RM_SRVC_RESOURCE_IPC_CLK,
	RM_SRVC_AXI_CFG_CLK,
	RM_SRVC_ETB_CLK,
	RM_SRVC_DTC_CLK,
	RM_SRVC_TCU_CTRL,
	RM_SRVC_ADMA_CLK,

	//*********** Not Direct Clocks **********
	RM_SRVC_CGPIO_CLK,
    RM_SRVC_CPA_DDR_HPerf,
	RM_SRVC_DDR_HIGH_FREQ,
    RM_SRVC_VCTCXO,
    RM_SRVC_DVFM,
    RM_SRVC_APB_BUS,
    RM_SRVC_AXI_BUS,

	//********** Till Here **************

	RM_NUM_OF_SRVCS,
	RM_INVALID_SRVCS = RM_NUM_OF_SRVCS
}RM_ServiceIdE;

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
									RM_CALLBACK_SRVC_TCU,
									RM_CALLBACK_SRVC_UART,
									RM_CALLBACK_SRVC_AC_IPC,			/* Apps-Comm IPC service wakeup */ // YKYK TODO
									RM_CALLBACK_SRVC_CGPIO,				/** This service may register for wakeup **/
									RM_CALLBACK_SRVC_DTC,
									RM_CALLBACK_SRVC_PMNC,

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
//ICAT EXPORTED ENUM
typedef enum
{
	RM_RSRC_FREE  = 0,
	RM_RSRC_ALLOC

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

#endif  /* _RM_GEAGULL_INTERNAL_H_ */
