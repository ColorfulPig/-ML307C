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
/* Title: AAM stress test Header File                                   */
/*                                                                      */
/* Filename: aam_stress_test.h                                          */
/*                                                                      */
/* Author:   Rafi Agiv                                                  */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 10/11/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/




#if !defined (_AAM_STRESS_TEST_H_)
    #define _AAM_STRESS_TEST_H_



	#include "global_types.h"
	#include "aam.h"
	#include "aam_stress_test_def.h"
	

	/*----------- Global defines -------------------------------------------------*/

	/*----------- Global macro definitions ---------------------------------------*/

	/*----------- Global type definitions ----------------------------------------*/

	/*----------- Global function prototypes -------------------------------------*/
	void AAMStressTestInit(void* pData);
	void AAMStressLogDisply(void);











#endif /*_AAM_STRESS_TEST_H_*/

