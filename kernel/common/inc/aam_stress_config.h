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
/* Title: AAM stress test Configurations Header File                    */
/*                                                                      */
/* Filename: aam_stress_test_config.h                                   */
/*                                                                      */
/* Author:   Rafi Agiv                                                  */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 10/11/2005                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#if !defined (_AMM_STRESS_TEST_CONFIG_H) 
    #define _AMM_STRESS_TEST_CONFIG_H 


	/* range of ticks to send the tasks to sleep */ 
	#define AAM_STRESS_MAXIMUM_NUMBER_OF_TICKS_TO_SLEEP        (15) 
	#define AAM_STRESS_MINIMUM_NUMBER_OF_TICKS_TO_SLEEP        (10)

	//The number of tasks supported by the AAM Stress tester
	#define AAM_STRESS_NUMBER_OF_TASKS_SUPPORTED_BY_TEST       (5)

	#define AAM_STRESS_DEFAULT_REFERENCE_TASK_PRIORITY         (5)
	
	#define AAM_STRESS_SIZE_OF_LOG                             (250)

	

	#define AAM_STRESS_NUM_OF_CONFIGS (3)


	

#endif /*_AMM_STRESS_TEST_CONFIG_H*/