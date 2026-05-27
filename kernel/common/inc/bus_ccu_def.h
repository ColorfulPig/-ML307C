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
/* Title: Comm. sub-system Bus Control Unit definitions               */
/*        (macros) Header File                                          */
/*                                                                      */
/* Filename: bus_ccu_def.h                                              */
/*                                                                      */
/* Author:   Idan Bartura                                               */
/*                                                                      */
/* Project, Target, subsystem: Tavor PV2, Seagull, HOP     				*/
/*																		*/
/* Remarks:                             								*/
/*    													                */
/* Created: 11/08/2009                                                  */
/************************************************************************/

#if !defined _BUS_CCU_DEF_H
#define _BUS_CCU_DEF_H

#include "gen_pm_config.h"
#include "global_types.h" // for the bit fields macros
#include "bus_ccu_lut.h"

// Macro definitions


#define MAP_CLOCK_TO_REG_ADDR(clock)    (volatile UINT32 *)(_Bus_CCU_LookUpTables.clockToRegister[(clock)])

#define MAP_CLOCK_TO_BIT_MASK(clock)    (_Bus_CCU_LookUpTables.clockToBitMask[(clock)])

#define BUS_CCU_CHECK_IF_CLOCK_ALWAYS_ON(clock)	(_Bus_CCU_LookUpTables.clockToAlwyasOn[(clock)])

#define CLEAR_BITS(reg, mask)                   ((reg) &= (~(mask)))

#define SET_BITS(reg, value, mask, shift)       {CLEAR_BITS(reg, mask);\
                                                 reg |= (((value)<<(shift))&(mask));}
												 
#define GET_BITS(reg, mask, shift)              (((reg)&(mask))>>(shift))

#define VOTE_ALWAYS                                 1

#define VOTE_IF_CHANGE								0

#define F_BIT_IS_ON                 ((BOOL)GET_BITS( (*(UINT32 *)FCAR), FCAR_F_BIT, FCAR_F_BIT_SHIFT))
#define TURN_F_BIT_ON               if (!F_BIT_IS_ON) 									\
									{ 													\
										(TURN_BIT_ON( (*(UINT32 *)FCAR), FCAR_F_BIT));	\
									}
										
#define INITIATE_SYS_PLL_FREQ_SEL  	    TURN_F_BIT_ON

#endif /* _BUS_CCU_DEF_H */
