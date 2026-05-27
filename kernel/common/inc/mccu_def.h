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
/* Title: Modem Clock Control Unit definitions Header File              */
/*                                                                      */
/* Filename: mccu_def.h                                                 */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks:                                                             */
/*    													                */
/* Created: 15/11/2004                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _MCCU_DEF_H
#define _MCCU_DEF_H

#include "global_types.h" // for the bit fields macros
#include "mccu_hw.h"
#include "mccu_luts.h"

/************************************************************************/
/*                   Macros for inter-package use                       */
/************************************************************************/

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined MCCU_DEBUG_MODE_ENABLED))
	#if 0
//        #define MCCU_DEBUG_MODE_ENABLED	 // not automatic - consume to much TCM 
    #endif

#ifdef MCCU_DEBUG_MODE_ENABLED
    #define HANDLE_MCCU_CLOCK_ERR ASSERT(0)
    #define HANDLE_MCCU_FREQ_ERR  ASSERT(0)
#else
    #define HANDLE_MCCU_CLOCK_ERR return MCCU_RC_CLOCK_ERR
    #define HANDLE_MCCU_FREQ_ERR  return MCCU_RC_FREQ_ERR
#endif

#define GET_FREQ_SELECTION_BITS(clk)                                        \
    (_MCCU_LookUpTables.freqSelectionBits[clk])

#define GET_FREQ_BITS(clk, freq)                                             \
    (_MCCU_LookUpTables.freqBits[clk][freq])

#define MAP_SUPPORTED_FREQ_TO_BIT_MASK(freq)                                \
    (_MCCU_LookUpTables.supportedFreqToBitMask[freq])

#define MAP_FUNC_CLOCK_TO_BIT_MASK(clk)                                     \
    (_MCCU_LookUpTables.clockToFuncBitMask[clk])   

#define GET_SUPPORTED_FREQS(clk)                                            \
    (_MCCU_LookUpTables.supportedFreqSelections[clk])   

#define FREQ_CHANGE_NOT_ALLOWED(clk)                                        \
    (GET_SUPPORTED_FREQS(clk) == FREQ_SELECTIONS_NOT_SUPPORTED)

#define FREQ_NOT_SUPPORTED(clk, freq)                                       \
    (!(GET_SUPPORTED_FREQS(clk) & MAP_SUPPORTED_FREQ_TO_BIT_MASK(freq)))

#define MCCU_CLOCK_IS_ON(clk)                                                    \
    ((BOOL)((pMCCU_Register->GMCGC) & MAP_FUNC_CLOCK_TO_BIT_MASK(clk)))
#define MAP_WB_CONTROL_BIT_TO_BIT_MASK(controlBit)                                     \
    (_MCCU_LookUpTables.WBContolBitsToBitMask[controlBit])   

#endif /* _MCCU_DEF_H */
