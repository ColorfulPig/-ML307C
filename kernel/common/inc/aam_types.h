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
/* Title: Aware Applications Manager type definitions Header File       */
/*                                                                      */
/* Filename: aam_types.h                                                */
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


#ifndef _AAM_TYPES_H_
    #define _AAM_TYPES_H_

    #include "aam.h"

    #if defined EXTERN
        #error "EXTERN ALREADY DFINED !!! (aam_types.h)"
    #else
        #if defined _AAM_NO_EXTERN_
            #define EXTERN
        #else
            #define EXTERN extern
        #endif // _AAM_NO_EXTERN_
    #endif //EXTERN

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined AAM_DEBUG_MODE_ENABLED))
	#if 0
//        #define AAM_DEBUG_MODE_ENABLED // not automatic - consume to much TCM 
    #endif


    /*----------- Local type definitions -----------------------------------------*/

    // This structure comprises all the information regarding the AAs
    // (except their current status - for efficiency).
    typedef struct  {
        BOOL                  waitsForBBD2OK;
        BOOL                  appStartRunTimeValid;
        PM_TimeIn32KHzUnitsT  appStartRunTime;
        AAM_CallbackFuncPrepareT* pAACbPrepare; //pD2CbkPrepare;
        AAM_CallbackFuncRecoverT* pAACbRecover; // pD2CbkRecover;
		BOOL                  appRegistered;
        #if defined AAM_DEBUG_MODE_ENABLED
            CHAR              aaName[AAM_MAX_NAME_SIZE + 1];
        #endif //AAM_DEBUG_MDOE_ENABLED
    }AAM_AppsInfoS;

	typedef struct  {
		AAM_CallbackFuncT*    LptBeforeIntDis;
		BOOL                  appRegistered;
	}AAM_AppsBeforeIntDisS;


    // This structure comprises of this package's global run time variables.
    typedef struct  {
        AAM_AppsStatusT        currentAppsStatus;  /* appsBusyStatus */
        AAM_AppsInfoS          appsInfo[AAM_NUM_OF_SUPPORTED_APPS];
		AAM_AppsBeforeIntDisS  appsBeforeIntDis[AAM_NUM_OF_SUPPORTED_APPS];
        PM_TimeIn32KHzUnitsT   nextAppOsStartRunTime;/* nearest System Event - application or system-tick*/
		PM_TimeIn32KHzUnitsT   nextTickTime; /* next time syste event due to know timer requests -
												requires tick-event (not application event) */

		// we rely on the fact that only ONE aware application is registered (the L1-CPA)
		// and with the 2 following settings we wait at least 5 millisec before going to D2 
		// (we should prevent writing PMbits for 5Millsec) after AA-CPA relaease
		// to overcome WB-GSM-sleep module wait to move to 32Khz clock..
		PM_TimeIn32KHzUnitsT	freeAppTimeStamp;	// time stamp of release
		UINT16				   bAppFreeWaitTIme;	// flag to know the AA released ...

        UINT16                 bbD2OkRequiredCounter; // if this counter is zero there is no need to wait for the _bb_d2_ok signal.
        #if defined(AAM_DEBUG_MODE_ENABLED)
            AAM_AppsLogS           appsLog;
            PM_TimeIn32KHzUnitsT   c1EntryExitLog[AAM_C1_LOG_LENGTH][AAM_C1_LOG_WIDTH];
            BOOL                   c1LogEnabled;
            UINT32                 c1LengthLogInd;
        #endif //AAM_DEBUG_MDOE_ENABLED
    }AAM_GlobalsS;


    /*----------- Package global variables --------------------------------------*/

    EXTERN AAM_GlobalsS _aamGlobals; // Initialized in phase 1 init


    #undef EXTERN

#endif  /* _AAM_TYPES_H_ */
