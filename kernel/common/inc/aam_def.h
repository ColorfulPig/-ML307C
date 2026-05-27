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
/* Title: Aware Applications Manager Definitions Header File            */
/*                                                                      */
/* Filename: aam_def.h                                                  */
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


#ifndef _AAM_DEF_H_

    #define _AAM_DEF_H_

    #include "global_types.h"
    #include "aam.h"

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined AAM_DEBUG_MODE_ENABLED))
	#if 0
//        #define AAM_DEBUG_MODE_ENABLED  // not automatic - consume to much TCM 
    #endif

    // Constant definitions 

    #define AAM_BITS_IN_BYTE          (8)
    #define AAM_APPS_STATUS_LSB       ((UINT32)0x1)
    #define AAM_APPS_STATUS_MSB       (AAM_APPS_STATUS_LSB << (AAM_BITS_IN_BYTE*sizeof(AAM_AppsStatusT)-1))
    #define AAM_APPS_STATUS_VALID_BIT  AAM_APPS_STATUS_MSB
   
    #define AAM_END_OF_STRING         '\0'

    #define AAM_INVALID_VALUE         (0xBABADADA)
    #define AAM_APP_FREE_BIT_MASK     ((UINT32)0x0)
    #define AAM_HANDLE_ZERO
    #define AAM_MAX_32KHZ_TICKS       ((PM_TimeIn32KHzUnitsT )0xFFFFFFFF)

    #define AAM_OS_TICK_SKIP          TRUE
    #define AAM_OS_TICK_DONT_SKIP     FALSE
        
    #if defined AAM_DEBUG_MODE_ENABLED
        #define AAM_UPDATE_LOG              AAMLogUpdate()
    #else
        #define AAM_UPDATE_LOG        
    #endif //AAM_DEBUG_MODE_ENABLED

    #if defined AAM_HALT_FOR_NULLS
        #define AAM_CHECK_NULL_POINTER(pTR)     ASSERT(pTR)
    #else  // AAM_HALT_FOR_NULLS
        #define AAM_CHECK_NULL_POINTER(pTR)     {if (NULL == (pTR)) return AAM_RC_ERR_NULL_POINTER;}
    #endif // AAM_HALT_FOR_NULLS

#endif  /* _AAM_DEF_H_ */
