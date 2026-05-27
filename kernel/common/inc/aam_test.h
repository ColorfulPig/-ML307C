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
/* Title: Aware Applications Manager test Unit Test Header File         */
/*                                                                      */
/* Filename: aam_test.h                                                */
/*                                                                      */
/* Author:   Rafi Agiv                                                  */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 20/9/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/
#ifndef _AAM_TEST_H_

#define _AAM_TEST_H_

#include "timer.h"
#include "aam_test_types.h"


// The amount of 32KHz clock ticks it takes
// to prepare to D2, enter D2 and then exit and recover immediately. (Arbel core runs @ 156MHz)
#define PM_D2_THRESHOLD   (2*TICK_INTERVAL_32KHZUNIT) //(2 OS Tick) todo: measure time



AAMUT_ManageResultE AAMUTManageTest(void);
AAMUT_IsD2AllowedResultE AAMUTiSD2AllowedTimeBBCheck(void);
void AAMUTISD2AllowedTimeWBTest(void* pData);
void AAMUTRegisterTest(void);
void AAMUTLPSPrepareTest(void);
void AAMUTLPSRecoverTest(void);
UINT32 AAMUTRegUnRegPrepareRecoveTest(void);
AAMUT_LogResultE AAMUTSetUnsetLogTest(void);
AAMUT_LogResultE AAMUTLogTest(void);
AAMUT_ExtractResultE AAMExtractStatusTest(void);
AAMUT_ExceptionResultE AAMUTExcepction1Test(void);
AAMUT_ExceptionResultE AAMUTExcepction2Test(void);
//void AAMUTRobustnessTest(void);
void AAMUnitTest(void);
PM_TimeIn32KHzUnitsT AAMUTClosestTimeCalculate(PM_TimeIn32KHzUnitsT arg1,
											   PM_TimeIn32KHzUnitsT arg2,
											   PM_TimeIn32KHzUnitsT currentTime);
//static PM_TimeIn32KHzUnitsT AAMUTRand(void);

#endif  /* _AAM_TEST_H_ */


