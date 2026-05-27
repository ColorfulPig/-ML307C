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
/* Title: Aware Applications Manager Unit Test                          */
/*          Configurations Header File                                  */
/*                                                                      */
/* Filename: aam_test_config.h                                          */
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

#ifndef _AAM_TEST_CONFIG_H_

#define _AAM_TEST_CONFIG_H_


#include "diag_API.h" // diag output to ICAT
#include "aam.h"
#include "aam_test_types.h"

extern UINT32 numberOfPrepareExecute;
extern UINT32 numberOfRecoverExecute;


void APP0PrepareFunc(void);
void APP1PrepareFunc(void);
void APP2PrepareFunc(void);
void APP3PrepareFunc(void);
void APP4PrepareFunc(void);

void APP0RecoverFunc(PM_PowerStatesE powerState);
void APP1RecoverFunc(PM_PowerStatesE powerState);
void APP2RecoverFunc(PM_PowerStatesE powerState);
void APP3RecoverFunc(PM_PowerStatesE powerState);
void APP4RecoverFunc(PM_PowerStatesE powerState);






//#define AAM_TCR_FREE_RUNNING_TIMER (TCR_4)
#define AAM_UT_NUMBER_OF_CHANGES (40)
#define AAM_UT_VALIDATION_BIT ((UINT32)0x1<<31)
#define AAM_UT_TIME_LOWER_BOUND (0x3FFFFFFF)
#define AAM_UT_TIME_UPPER_BOUND (0xcFFFFFFF)
#define AAM_UT_MAXIMUM_TIME (0xFFFFFFFF-1)
#define AMM_UT_HALF_MAXIMUM_TIME (0x7FFFFFFF)
#define AAM_UT_13M_TICK_TO_MICRO_SEC(interruptAbsTime) (interruptAbsTime*0.076923)
#define MINIMUM(b,c)(((b) < (c))? (b):(c))

/* look up table for printing the results of the tests and for holding functions pointers */
typedef const struct {
	CHAR*                       manageResults[AAM_UT_NUMBER_OF_MANAGE_RESULTS];
	CHAR*                       isD2AllowedResult[AAM_UT_NUMBER_OF_IS_D2_RESULTS];
	CHAR*                       logResult[AAM_UT_NUMBER_OF_LOG_RESULTS];
	CHAR*                       extractResult[AAM_UT_NUMBER_OF_EXTRACT_RESULTS];
    CHAR*                       exceptionResult[AAM_UT_NUMBER_OF_EXCEPTION_RESULTS];
    CHAR*                       timerResult[AAM_UT_NUMBER_OF_TIMER_RESULTS];
	AAM_CallbackFuncT*          d2PrepareFuncs[AAM_NUM_OF_SUPPORTED_APPS];
	AAM_CallbackFuncRecoverT*          d2RecoverFuncs[AAM_NUM_OF_SUPPORTED_APPS];
} Test_ResultsS;



static Test_ResultsS TestResults = 
{  /* mannageResults */
	{   "AAMMannage PASSED",
		"TURNING ON FAILED - ",
		"TURNING OFF FAILED - "
	},
	{ /* isD2AllowedResult */
		"isD2Allowed PASSED",
		"FAILED - GETTING WRONG ANSWER",
		"FAILED - CALCULATING WRONG TIME"
	},
	{  /* logResult */
		"LOG PASSED",
		"FAILED TO ENABLE THE LOG",
		"FAILED TO DISBLE THE LOG",
		"FAILED - LOGGED WHEN SHOULDN'T",
		"FAILED - LOG IS NOT CORRECT",
		"FAILED - LOG IS NOT CYCLIC"
	},
	{  /* extractResult */
		"AAMStatusExtract PASSED",
		"FAILED - INDICATES HANDLE IS FREE WHEN HANDLE IS BUSY - ",
		"FAILED - INDICATES HANDLE IS BUSY WHEN HANDLE IS FREE - ",
	},
	{  /* exceptionResult */
		"PASSED - SETTING A HANDLE TWICE TO BE BUSY",
		"PASSED - SETTING A HANDLE TWICE TO BE FREE",
		"FAILED - STATUS CHANGED",
		"FAILED - WAKE UP TIME WAS NOT UPDATED"
	},
		{   "FREE RUNNING TIME IS RECONFIGURED TO 13MHZ",
		   "FREE RUNNING TIME IS RECONFIGURED TO 32KHZ",
			"ACTIVATION OF THE NEW CONFIGURATION FAILED",
			"CONFIGURATION FAILED",
			"DEACTIVATION FAILED"
	},
	{  /* d2PrepareFuncs */ 
        APP0PrepareFunc,
		APP1PrepareFunc,
		APP2PrepareFunc,
		APP3PrepareFunc,
		APP4PrepareFunc
	},
	{  /* d2RecoverFuncs */
		APP0RecoverFunc,
		APP1RecoverFunc,
		APP2RecoverFunc,
		APP3RecoverFunc,
		APP4RecoverFunc
	}

};   
/* d2PrepareFuncs */
void APP0PrepareFunc(void)
{
			++numberOfPrepareExecute;
	      //  DIAG_FILTER(PM,AAM,AAMD2PrepareTest1,DIAG_INFORMATION);
         //   diagPrintf("%s","aa0 prepare function executed");
}
void APP1PrepareFunc(void)
{
		    ++numberOfPrepareExecute;
	     //   DIAG_FILTER(PM,AAM,AAMD2PrepareTest2,DIAG_INFORMATION);
         //   diagPrintf("%s","aa1 prepare function executed");
}
void APP2PrepareFunc(void)
{
		    ++numberOfPrepareExecute;
	     //   DIAG_FILTER(PM,AAM,AAMD2PrepareTest3,DIAG_INFORMATION);
         //   diagPrintf("%s","aa2 prepare function executed");
}
void APP3PrepareFunc(void)
{
			++numberOfPrepareExecute;
	      //  DIAG_FILTER(PM,AAM,AAMD2PrepareTest4,DIAG_INFORMATION);
         //   diagPrintf("%s","aa3 prepare function executed");
}
void APP4PrepareFunc(void)
{
			++numberOfPrepareExecute;
	     //   DIAG_FILTER(PM,AAM,AAMD2PrepareTest5,DIAG_INFORMATION);
         //   diagPrintf("%s","aa4 prepare function executed");
}



/* d2RecoverFuncs */
void APP0RecoverFunc(PM_PowerStatesE powerState)
{
			++numberOfRecoverExecute;
	     //   DIAG_FILTER(PM,AAM,AAMD2RecoverTest1,DIAG_INFORMATION);
         //   diagPrintf("%s","aa0 recover function executed");
}
void APP1RecoverFunc(PM_PowerStatesE powerState)
{
			++numberOfRecoverExecute;
	    //    DIAG_FILTER(PM,AAM,AAMD2RecoverTest2,DIAG_INFORMATION);
        //    diagPrintf("%s","aa1 recover function executed");
}
void APP2RecoverFunc(PM_PowerStatesE powerState)
{
			++numberOfRecoverExecute;
	    //    DIAG_FILTER(PM,AAM,AAMD2RecoverTest3,DIAG_INFORMATION);
        //    diagPrintf("%s","aa2 recover function executed");
}
void APP3RecoverFunc(PM_PowerStatesE powerState)
{
			++numberOfRecoverExecute;
	    //    DIAG_FILTER(PM,AAM,AAMD2RecoverTest4,DIAG_INFORMATION);
         //   diagPrintf("%s","aa3 recover function executed");
}
void APP4RecoverFunc(PM_PowerStatesE powerState)
{
			++numberOfRecoverExecute;
	    //    DIAG_FILTER(PM,AAM,AAMD2RecoverTest5,DIAG_INFORMATION);
        //    diagPrintf("%s","aa4 recover function executed");
}


#endif /* _AAM_TEST_CONFIG_H_ */
