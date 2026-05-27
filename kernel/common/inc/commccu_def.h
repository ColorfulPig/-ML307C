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
/* Title: Comm. sub-system Clock Control Unit definitions               */
/*        (macros) Header File                                          */
/*                                                                      */
/* Filename: commccu_def.h                                              */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks:                             								*/
/*    													                */
/* Created: 04/11/2004                                                  */
/*                                                                      */
/* Modified: 01/04/2005                                                 */
/************************************************************************/

#if !defined _COMMCCU_DEF_H
#define _COMMCCU_DEF_H

#include "global_types.h" // for the bit fields macros
#include "commccu_lut.h"

// Macro definitions
    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined COMMCCU_DEBUG_MODE_ENABLED))
	#if 0
//        #define COMMCCU_DEBUG_MODE_ENABLED  // not automatic - consume to much TCM 
    #endif

#define F_BIT_IS_ON                 ((BOOL)pCommCCU_Registers->GCCCR & GCCCR_F_BIT)

// Note that we check the F bit status just as a precaution.
#define TURN_F_BIT_ON                   {if (!F_BIT_IS_ON) (TURN_BIT_ON( pCommCCU_Registers->GCCCR, GCCCR_F_BIT));}
#define INITIATE_CORE_CLK_CHANGE	    TURN_F_BIT_ON
#define INITIATE_SYS_PLL_FREQ_SEL  	    TURN_F_BIT_ON

// if the timeout expires and the boolean expression is not FALSE yet, we halt.
#define WAIT_FOR_FALSE_WITH_TIMEOUT(boolExpr,timeoutVal)                                \
                                      {                                                 \
                                        int timeout = (int)(timeoutVal);                \
                                        while ((boolExpr) && ((BOOL)timeout))           \
                                        {                                               \
                                        timeout--;                                      \
                                        }                                               \
                                        ASSERT((!(boolExpr)));                          \
                                      } 

#define IS_CLK_IN_CCCR(clock)           ((clock) < COMMCCU_CLK_I2C)
#define MAP_CLOCK_TO_REG_ADDR(clock)    (IS_CLK_IN_CCCR(clock) ?                        \
                            (&(pCommCCU_Registers->CCCR)) : (&(pCommCCU_Registers->GCKEN)))
#define COMMCCU_CCCR_REG_ADDR           (&(pCommCCU_Registers->CCCR))

#define MAP_SYS_PLL_FREQ_TO_MASK(freq)  (((UINT32) (freq))<<0x1)

#if defined COMMCCU_DEBUG_MODE_ENABLED
    #define CHECK_ARRAY_INDEX(index,size) ASSERT(((index)<(size))&&((index)>=0))
#else
    #define CHECK_ARRAY_INDEX(index,size)
#endif

#define MAP_CLOCK_TO_BIT_MASK(clock)    (_CommCCU_LookUpTables.clockToBitMask[(clock)])   

// PLL status getters
#define GET_USED_PLL                    ((CommCCU_PLLsE) pCommCCU_Registers->CCSR & CCSR_PLL_SEL_STATUS);
#define GET_SYS_PLL_FREQ                ((CommCCU_SystemPLLFreqE) (((UINT32) (pCommCCU_Registers->CCSR & CCSR_SYS_PLL_FREQ_SEL_STATUS)) >> 1));
#define GET_SYS_PLL_LOCK_STATUS         ((BOOL) (pCommCCU_Registers->CCSR & CCSR_SYS_PLL_LOCKED));
#define GET_GB_PLL_LOCK_STATUS          ((BOOL) (pCommCCU_Registers->CCSR & CCSR_GB_PLL_LOCKED));

#endif /* _GB_CCU_DEF_H */
