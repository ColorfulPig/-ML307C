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
/* Title: Power Management configurations Header File                   */
/*                                                                      */
/* Filename: pm_config.h                                                */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: This file contains configurable parameters used by the      */
/*          entire communication power management system                */
/*    													                */
/* Created: 1/1/2006                                                    */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/


#ifndef _PM_CONFIG_H_
    #define _PM_CONFIG_H_

// tick_manager.h must be included for Harvell Plat to compile well with diag_nvm.c (only with this included the FDI_INTEL is defined properly at diag)
    #include "tick_manager.h"

// The amount of 32KHz clock ticks it takes
// to exit and recover from C1. (Arbel core runs @ 156MHz)
#define PM_C1_WAKEUP_TIME 0 //Defined only for completeless and future needs.

// The amount of 32KHz clock ticks it takes
// to exit and recover from D2. (Arbel core runs @ 156MHz)
//#define PM_D2_WAKEUP_TIME (2*TICK_INTERVAL_32KHZUNIT) //(2 OS Tick, hermon is 7ms and we need more...) todo: measure time
//for optimizations changing this value to ~6 msec = 197 scks
#define PM_D2_WAKEUP_TIME 190  //197 (original) + 75 (additional ~2.3 msec for fix)  - 82 (reducing oscc by 2.5 msec)

#define PM_D2_WAKEUP_TIME_WITH_SYNC 135  //197 (original) + 75 (additional ~2.3 msec for fix)  - 82 (reducing oscc by 2.5 msec)
#define MIN_HW_STABN_TIME 50
#endif  /* _PM_CONFIG_H_ */
