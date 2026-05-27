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
/* Title: Aware Applications Manager Configurations Header File         */
/*                                                                      */
/* Filename: aam_config.h                                               */
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


#ifndef _AAM_CONFIG_H_
    #define _AAM_CONFIG_H_

#include "commpm.h"

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined AAM_DEBUG_MODE_ENABLED))
	#if 0
//        #define AAM_DEBUG_MODE_ENABLED // not automatic - consume to much TCM 
    #endif

    // The cyclic log size
    #define AAM_LOG_SIZE                 32

    // ICAT display related configurable constants
    #define AAM_ADJACENT_BITS             4 // where to put the space... (if changed, change BINARY_STRING_BUFF_SIZE as well)
    #define AAM_BINARY_STRING_BUFF_SIZE   (AAM_NUM_OF_SUPPORTED_APPS + (AAM_NUM_OF_SUPPORTED_APPS/AAM_ADJACENT_BITS) + 1) //1chars (AAs) + 6spaces + 1\n.


    // The number of AAs currently supported by the package. - number of clients defined
	// in system (should not exceed 32!)
    #define AAM_NUM_OF_SUPPORTED_APPS     COMMPM_NUMBER_OF_CLIENTS

    #if defined AAM_DEBUG_MODE_ENABLED
        #define AAM_MAX_NAME_SIZE         20
        #define AAM_C1_LOG_LENGTH         128
        #define AAM_C1_LOG_WIDTH          3   // Entry, exit and suspend time.
        #define AAM_C1_LOG_ENTRY_INDEX    0   // C1 entry time stamp
        #define AAM_C1_LOG_EXIT_INDEX     1   // C1 exit  time stamp
        #define AAM_C1_LOG_LENGTH_INDEX   2   // Requested time to wake
        #define AAM_EVENT_LOG_SIZE        128
    #endif  /* AAM_DEBUG_MDOE_ENABLED */

#endif  /* _AAM_CONFIG_H_ */
