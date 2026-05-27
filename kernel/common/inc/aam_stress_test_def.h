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
/* Title: aam_stress_test_def Definitions Header File                   */
/*                                                                      */
/* Filename: aam_stress_test_def.h                                      */
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


#include "aam_stress_config.h"
#include "nucleus.h"

#if !defined (_AMM_STRESS_TEST_DEF_H)
    #define _AMM_STRESS_TEST_DEF_H


	#define AAM_STRESS_MANAGER_TASK_STACK_SIZE      (0x800)  // 1K
	                                                    
	#define AAM_STRESS_AAP_TASK_STACK_SIZE          (0x800)  // 1K

	#define AAM_STRESS_IDLE_TASK_STACK_SIZE         (0x800)  // 1K

	#define AAM_STRESS_ALL_TASK_FINISHED            (0x0)

	#define AAM_STRESS_MSG_SIZE			            (sizeof(AAMStress_InfoS ))

	#define AAM_STRESS_MAX_NUM_OF_ELEMENTS		    (20)

	#define AAM_STRESS_MSG_QUEUE_SIZE               (AAM_STRESS_MSG_SIZE*AAM_STRESS_MAX_NUM_OF_ELEMENTS) 
	 
	#define AAM_STRESS_NUMBER_OF_ITERATIONS         (30) 

	#define AAM_STRESS_IDLE_TASK_ID                 (AAM_STRESS_NUMBER_OF_TASKS_SUPPORTED_BY_TEST+1)

	#define AAM_STRESS_MANAGER_TASK_ID              (0) 

	#define AAM_STRESS_OS_TIME_TICK_IN_32KHZ_TICKS  (5*32768/1000) // (5 mili-second (5/1000) multiply by clock rate 32K

	#define AAM_STRESS_ALL_TASK_INITIALIZED         ((0x1 << AAM_STRESS_NUMBER_OF_TASKS_SUPPORTED_BY_TEST)-1)// turn on all the bit who represent the
                                                                                                             // supported tasks
	#define AAM_STRESS_ARE_ALL_TASKS_FINISHED       ((_aamStressGlobalVars.currnetTaskStatus==AAM_STRESS_ALL_TASK_FINISHED)? YES:NO)

    #define AAM_STRESS_AVERAGE_NUMBER_OF_TICKS_TO_SLEEP ((AAM_STRESS_MINIMUM_NUMBER_OF_TICKS_TO_SLEEP+AAM_STRESS_MAXIMUM_NUMBER_OF_TICKS_TO_SLEEP)/2)

    #define AAM_STRESS_MAX_PRIORITY                 (240)

	typedef void (AAMStress_DeleteFuncT)(UINT8 taskIndex);
	typedef void (AAMStressTaskEntry)(UINT32 argc,void* argv);

	typedef UINT32 AAMStress_AppsStatusT; 
	typedef UINT32 AAMStress_TaskStatusT; 

	//this enumerator describes the status of the handle in the log
	typedef enum{
		AAM_STRESS_ALLOC=0,
		AAM_STRESS_FREE,
		AAM_STRESS_NOT_VALID
	}AAMstress_StatusE;

    //this enumerator describes the options of the messages
	typedef enum{
		AAM_STRESS_HANDLE_FREE=0,
		AAM_STRESS_HANDLE_ALLOC,
		AAM_STRESS_D2_REPORT,
		AAM_STRESS_TASK_FINISHED
	}AAMstress_MessageE;




	typedef struct{
		AAM_HandleT             handle;
		UINT8                   handleStatus;
		PM_TimeIn32KHzUnitsT   WakeUpTime;
		AAM_AppsStatusT         currentAAMStatus;
		AAMStress_AppsStatusT   currentAATestStatus;
		BOOL                    d2Allowed;	
		UINT8                   ticksToSleep;
	}AAMStress_LogS;




	typedef struct{
		UINT32    loggingCounter;       //counts the number of loggings
		UINT32    idleTaskCounter;      //counts how many times we entered the idle task
		BOOL      d2AlreadyAsked;       //indicates weather or not we just entered the idle task so we will get into a loop
		UINT8     taskPriorityConfig;   //indicates the priority configurations of the tasks in the test
		UINT8     taskPriorityReference;//according to this priority all the tasks get their priority 
		UINT8     d2EntryCounter;       //counts the number of times d2 was allowed
		UINT8     d2CheckPassed;        //indicates if the entering to d2 or rejecting d2 was correct according to the AAM status
		BOOL      managerTaskRunning;   //indicates if the manager task still exist
	}AAMStress_TestParametersS;




	typedef struct{
		AAMStress_AppsStatusT            currentAATestStatus;
		AAMStress_TaskStatusT            currnetTaskStatus;
		UINT32                           callbackPrepareExecution[AAM_STRESS_NUMBER_OF_TASKS_SUPPORTED_BY_TEST];
		UINT32                           callbackRecoverExecution[AAM_STRESS_NUMBER_OF_TASKS_SUPPORTED_BY_TEST];
		AAMStress_TestParametersS        testParameters;
		AAMStress_LogS                   log[AAM_STRESS_SIZE_OF_LOG];
		UINT32                           nextEntry;
	}AAMStress_GlobalVarsS;



	typedef struct{
		UINT8                 task;
		AAM_HandleT           handle;
		AAMstress_MessageE    message;
		PM_TimeIn32KHzUnitsT WakeUpTimeFromSleep;// must be check if it is still a relevant typedef
		BOOL                  d2Allowed;	
		UINT8                 ticksToSleep;
	}AAMStress_InfoS;


	

	typedef struct{
#ifdef PLAT_TEST	
		OSATaskRef			 taskPointer;
#else
		NU_TASK*            taskPointer;
#endif
		CHAR*               taskName[8];
		void*               taskStack;
	}AAMStress_TaskVarsS;

    
	
	CHAR** _aamStressAllocFree[2]={"ALLOC","FREE"};
	CHAR** _aamStressYesNo[2]={"NO","YES"};
	


#endif /*_AMM_STRESS_TEST_DEF_H*/







