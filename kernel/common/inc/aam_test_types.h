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
/* Title: Aware Applications Manager Test Unit                          */
/*        type definitions Header File                                  */
/*                                                                      */
/* Filename: aam_test_types.h                                           */
/*                                                                      */
/* Author:   Rafi Agiv                                                  */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 11/4/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _AAM_TEST_TYPES_H_

#define _AAM_TEST_TYPES_H_




typedef enum
{
	AAM_UT_MANAGE_PASSED = 0,
	AAM_UT_MANAGE_TURNING_ON_FAILED,
	AAM_UT_MANAGE_TURNING_OFF_FAILED,
	AAM_UT_NUMBER_OF_MANAGE_RESULTS
}AAMUT_ManageResultE;







typedef enum
{
	AAM_UT_ISD2ALLOWED_RIGHT_ANSWER = 0, 
	AAM_UT_ISD2ALLOWED_WRONG_ANSWER,
	AAM_UT_GOT_WORNG_TIME,
	AAM_UT_NUMBER_OF_IS_D2_RESULTS
}AAMUT_IsD2AllowedResultE;






typedef enum
{
	AAM_UT_LOG_PASSED = 0,
	AAM_UT_ENABLED_LOGGING_FAILED,
	AAM_UT_DISABLED_LOGGING_FAILED,
	AAM_UT_LOGGING_WHEN_NOT_NEDDED,
	AAM_UT_LOGGING_IS_NOT_CORRECT,
	AAM_UT_LOGGING_IS_NOT_CYCLIC,
	AAM_UT_NUMBER_OF_LOG_RESULTS
}AAMUT_LogResultE;




typedef enum
{
	AAM_UT_EXTRACT_PASSED = 0,
	AAM_UT_HANDLE_SHOULD_BE_BUSY,
	AAM_UT_HANDLE_SHOULD_BE_FREE,
	AAM_UT_NUMBER_OF_EXTRACT_RESULTS
}AAMUT_ExtractResultE;



typedef enum
{
	AAM_UT_EXCEPTION1_PASSED = 0,
	AAM_UT_EXCEPTION2_PASSED,
	AAM_UT_STATUS_CHANGED,
	AAM_UT_TIME_WAS_NOT_UPDATED,
	AAM_UT_NUMBER_OF_EXCEPTION_RESULTS
}AAMUT_ExceptionResultE;



typedef enum
{
	AAM_UT_TIMER_RECONFIGURED_TO_13M = 0,
	AAM_UT_TIMER_RECONFIGURED_TO_32K,
	AAM_UT_TIMER_ACTIVATED_FAILED,
    AAM_UT_TIMER_RECONFIGURED_FAILED,
	AAM_UT_TIMER_DEACTIVATED_FAILED,
    AAM_UT_NUMBER_OF_TIMER_RESULTS
}AAMUT_TimerResultE;

#endif /* _AAM_TEST_TYPES_H_ */
