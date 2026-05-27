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
/* Title: Aware Applications Manager Header File                        */
/*                                                                      */
/* Filename: aam.h                                                       */
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


#ifndef _AAM_H_
    #define _AAM_H_

    #include "global_types.h"
    #include "pm_dbg_types.h"
    #include "aam_config.h"

    /*----------- Global defines -------------------------------------------------*/
    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined AAM_DEBUG_MODE_ENABLED))
	#if 0
//        #define AAM_DEBUG_MODE_ENABLED // not automatic - consume to much TCM
    #endif

    /*----------- Global macro definitions ---------------------------------------*/

    /*----------- Global type definitions ----------------------------------------*/

    // This enumerator describes the various values that may be returned.
    typedef enum
    {
        AAM_RC_OK = 0,
        AAM_RC_ERR_NULL_POINTER = -100
    }AAM_ReturnCodeE;

    // This enumerator describes the requests made by AAs.
    typedef enum
    {
        AAM_APP_BUSY = 0,
        AAM_APP_FREE
    }AAM_AppStatusE;

    // This enumerator describes the request types to the logger
    typedef enum
    {
        AAM_LOGGING_START = 0,
        AAM_LOGGING_STOP
    }AAM_LogReqE;

    // Bit vector describing the current AAs status / handle registration status / etc...
    // if changing the size of this type make sure to change AAM_GET_BIT_MASK_BY_HANDLE as well.
    typedef UINT32 AAM_AppsStatusT;

    // 32 bit type to describe handle returned by the AAM.
    typedef UINT32 AAM_HandleT;

    // This type definition describes the AAM callback functions type.
    typedef void (AAM_CallbackFuncT)(void);

    // This type definition describes the AAM D2 prepare Recover callback functions type.
	typedef void (AAM_CallbackFuncPrepareT)( PM_PowerStatesE statetoprepare);
	typedef void (AAM_CallbackFuncRecoverT)( PM_PowerStatesE stateexited, BOOL b_DDR_ready, BOOL b_RegsRetainedState);

    #if defined AAM_DEBUG_MODE_ENABLED
        // These type definitions describes the AAs log.
        typedef AAM_AppsStatusT AAM_AppsArrT[AAM_LOG_SIZE];

        typedef struct
        {
            AAM_AppsArrT  appsStatusArr;
            UINT32        nextEntry;
            BOOL          logEnabled;
        }AAM_AppsLogS;
    #endif //AAM_DEBUG_MDOE_ENABLED

    /*----------- Extern definition ----------------------------------------------*/

    /*----------- Global variable declarations -----------------------------------*/

    /*----------- Global constant definitions ------------------------------------*/

    /*----------- Global function prototypes -------------------------------------*/
    // Init functions.

    void                  AAMPhase1Init       (void);
    void                  AAMPhase2Init       (void);

    // Package specific APIs.
    void                  AAMManage           (AAM_HandleT                 appHandle,
                                               AAM_AppStatusE              busyFree,
                                               BOOL                        appStartRunTimeValid,
                                               PM_TimeIn32KHzUnitsT        appStartRunTime);
    BOOL                  AAMIsD2Allowed      (void);

    void                  AAMUnregister       (AAM_HandleT                 appHandle);

    AAM_HandleT           AAMRegister         (CommPM_ClientsE client,
    										   AAM_CallbackFuncPrepareT*   pd2CbkPrepare,
                                               AAM_CallbackFuncRecoverT*   pd2CbkRecover,
                                               BOOL                        waitsForBBD2OK,
                                               AAM_AppStatusE              initStatus);

	void				  AAMControl	      (AAM_HandleT				   appHandle,
											   BOOL						   waitsForBBD2OK);
    void                  AAMLPSPrepare       (PM_PowerStatesE             newLPS);
    void                  AAMLPSRecover       (PM_PowerStatesE             psType);
   void 			AAMBeforeIntDisRegister (CommPM_ClientsE client,
   												AAM_CallbackFuncT*          cbkBeforeIntDis);

   void AAMDoBeforeIntDis(void);

    // Debug functions.
    #if defined(AAM_DEBUG_MODE_ENABLED)
        AAM_AppsStatusT        AAMCurrentAppsStatusGet    (void);
        void                   AAMAppsStatusLog           (AAM_LogReqE     logReq);
        AAM_AppsLogS*          AAMAppsStatusLogGet        (void);
        AAM_AppStatusE         AAMAppStatusExtract        (AAM_HandleT     appHandle,
                                                           AAM_AppsStatusT currentAppsStatus);
        void                   AAMAppsStatusLogICATDisplay(void);
    #endif //AAM_DEBUG_MODE_ENABLED

    // Test functions
    void                  AAMTest(void);
    void                  mmiAAMInit(void);

    UINT32                mmiAAMRequest(UINT32 handle, BOOL request_type);

#endif  /* _AAM_H_ */
