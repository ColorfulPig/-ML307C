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
/* Title: Arbel Resource Manager type definitions Header File           */
/*                                                                      */
/* Filename: rm_types.h                                                 */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 11/4/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/


#if !defined _RM_TYPES_H_
    #define _RM_TYPES_H_

    #include "rm_internal.h"

    #if defined EXTERN
        #error EXTERN already defined !!
    #endif // EXTERN

    #if defined _RM_NO_EXTERN_
        #define EXTERN 
    #else
        #define EXTERN extern
    #endif

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined RM_DEBUG_MODE_ENABLED))
	#if 0
//        #define RM_DEBUG_MODE_ENABLED // not automatic - consume to much TCM 
    #endif

    /*----------- Local type definitions -----------------------------------------*/
    // This enumerator describes the multi-user clocks (ones that are used by more
    // than one user).
//ICAT EXPORTED ENUM
typedef enum    {
    MULTI_USER_CLK_DSSP_BUS=0,
    MULTI_USER_CLK_VCTCXO,
    MULTI_USER_CLK_MODEM_78MHZ,
    MULTI_USER_CLK_MODEM_104MHZ,
	MULTI_USER_CLK_DDR_HIGH_FREQ,   //rafiagiv - 21/12
    NUMBER_OF_MULTI_USER_CLOCKS,
    NO_MULTI_USER_CLOCK = -1
}RM_MultiUserClocksE;

// This structure comprises of variables used for debug purposes.
typedef struct  {
    RM_ResourcesLogS resourcesStatusLog;
    BOOL logModeEnabled;
}RM_DebugVariablesS;

// This structure comprises of this package's run time variables.
typedef struct  {
    RM_CurrentResourcesStatusT currentResourcesStatus;
#ifdef PHS_SW_DEMO_TTC_PM
	RM_CurrentResourcesStatusT ext_currentResourcesStatus;
#endif

    INT32                       multiUserCounters[NUMBER_OF_MULTI_USER_CLOCKS];
    void * /*RM_CallbackFuncT*/            d2CbkPrepare     [RM_NUM_OF_CALLBACK_SRVCS];
    void */*RM_D2RecoverCallbackFuncT*/   d2CbkRecover     [RM_NUM_OF_CALLBACK_SRVCS];
    void */*RM_CallbackFuncT */           cbkWakeup        [RM_NUM_OF_CALLBACK_SRVCS];
	RM_CallbackFuncT			LptBeforeIntDis	 [RM_NUM_OF_CALLBACK_SRVCS];
	UINT32		countSSPwakeup;
    #if defined(RM_DEBUG_MODE_ENABLED)
        RM_DebugVariablesS      rmDbgVars;
    #endif
	#if defined (ISPT_OVER_SSP)
	UINT32	bReportISPTclk;
	#endif
}RM_RuntimeVariablesS;



//ICAT EXPORTED STRUCT
typedef struct {
UINT32   								d2CbkPrepare;
UINT32   								d2CbkRecover;
RM_ServiceCallbackE 				cbkevent;
}RM_PrepareRecoverEventsS;

//ICAT EXPORTED STRUCT
typedef struct {
UINT32  								cbkWakeup;
RM_ServiceCallbackE 				cbkevent;
}RM_WakeUpEventsS;

//ICAT EXPORTED STRUCT
typedef struct {
RM_PrepareRecoverEventsS 	RM_PrepareRecoverEventsEArray[RM_NUM_OF_CALLBACK_SRVCS];
}RM_EventsArrayRecPreS;

//ICAT EXPORTED STRUCT
typedef struct {
RM_WakeUpEventsS       		RM_WakeUpEventsArray[RM_NUM_OF_CALLBACK_SRVCS];
}RM_EventsArrayWakeupS;

/*----------- Package global variables --------------------------------------*/
#undef EXTERN

extern RM_RuntimeVariablesS _rmVars; //initialized in phase 1 init 

#endif  /* _RM_TYPES_H_ */
