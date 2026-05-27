/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/****************************************************************************
*                         PMU_Monitoring.h
*
*   Created:        09/07/2003
*   Authoer:        Maya Shtern
*   Description :   Header file for monitoring the performance of the
*                   PMU registers
*   Last Changed:   02/10/2003
*
****************************************************************************/

#ifndef _PMU_MONITORING_H_
#define _PMU_MONITORING_H_


/*----------------------------------------
 |          Include Files                |
 ----------------------------------------*/
#include <stdio.h>
#include <global_types.h>
#include <nucleus.h>



/*----------------------------------------
 |       events ID definitions           |
 ----------------------------------------*/
#define ICACHE_MISS                     0x0
#define ICACHE_CANNOT_DELIVER           0x1
#define DATA_DEP_STALL                  0x2
#define ITLB_MISS                       0x3
#define DTLB_MISS                       0x4
#define BR_EXEC                         0x5
#define BR_MISPRED                      0x6
#define INSTR_EXEC                      0x7
#define DC_BUF_STALL_DURATION           0x8
#define DC_BUF_STALL_EVENT              0x9
#define DATA_CACHE_ACCESS               0xA
#define DATA_CACHE_MISSE                0xB
#define DATA_CACHE_WB                   0xC
#define SW_PC_CHANGES                   0xD
#define DONT_CARE1                      0xE
#define DONT_CARE2                      0xF
#define BCU_NEW_MEM_REQ                 0x10
#define BCU_REQ_QUEUE_FULL              0x11
#define BCU_REQ_QUEUE_DRAINED           0x12
#define DONT_CARE                       0x13
#define BCU_ECC_ERR_NO_ELOG             0x14
#define BCU_1BIT_ERR                    0x15
#define RMW_ECC                         0x16
#define MCLK                            0x17


/*----------------------------------------
 |              clock types              |
 ----------------------------------------*/
#define CLOCK_104MHZ                    1
#define CLOCK_312MHZ                    2


/*----------------------------------------
 |          timer definitions            |
 ----------------------------------------*/
#define PMU_TIMER_EXPIRED              999


/*----------------------------------------
 |              Structs                  |
 ----------------------------------------*/
//ICAT EXPORTED STRUCT
typedef struct
{
    unsigned long nEvent0;
    unsigned long nEvent1;
    unsigned long nEvent2;
    unsigned long nEvent3;
    unsigned long nTimeoutDurationCCNT;
	unsigned long nTimeoutDurationNuTimer;
    BOOL          bMultipleMeasurements;
}SConfigurationData;

//ICAT EXPORTED STRUCT
typedef struct
{
    unsigned long nEvent0;
    unsigned long nEvent1;
    unsigned long nEvent2;
    unsigned long nEvent3;
    unsigned long nOverflow;
    unsigned long nTimeDuration;
    BOOL          isTimerExpired;
}SResultsData;

/*----------------------------------------
 |       global Variables                |
 ----------------------------------------*/
extern unsigned long nAccumulateCycles;
extern unsigned long nTotalDuration;
extern BOOL          bTestRunning;
extern BOOL          bMultipleMeasurements;
extern unsigned long Event0;
extern unsigned long Event1;
extern unsigned long Event2;
extern unsigned long Event3;


/*----------------------------------------
 |          Functions Prototypes         |
 ----------------------------------------*/
//  mcr & mrc functions
//---------------------
#ifdef __arm
#define READ_CP14_REG(CRn, CRm, vAR)    {                               	    \
                                            register int Rd;                	\
                                            __asm                           	\
                                            {                               	\
                                                MRC p14, 0, Rd, CRn, CRm, 0   	\
                                            }                               	\
                                            vAR = Rd;                       	\
                                        }


#define WRITE_CP14_REG(CRn, CRm, vALUE) {                               	    \
                                            register int Rd = vALUE;        	\
                                            __asm                          		\
                                            {                               	\
                                                MCR p14, 0, Rd, CRn, CRm, 0   	\
                                            }                               	\
                                        }
#else
#if !defined(__INTEL_COMPILER) || (__INTEL_COMPILER == 800) //SDT1.2
#define READ_CP14_REG(CRn, CRm, vAR)    {                               	    \
                                            register int Rd;                	\
                                            /*__asm(" MRC p14, 0, %0, %1, %2, 0" : Rd : CRn,CRm);*/  \
                                            vAR = Rd;                       	               \
                                        }


#define WRITE_CP14_REG(CRn, CRm, vALUE) {                               	                 \
                                            register int Rd = vALUE;           	             \
                                            /*__asm(" MCR p14, 0, %0, %1, %2, 0" ::Rd,CRn,CRm);*/  \
}
#else // SDT 2.0b or above
typedef enum
{
	c0, c1, c2, c3, c4, c5, c6, c7, c8, c9, c10, c11, c12, c13, c14, c15
} CpOpValue;

#define READ_CP14_REG(CRn, CRm, vAR)    {                               	    \
                                            register int Rd;                	\
                                            __asm(" MRC p14, 0, %0, c%a1, c%a2, 0" : "=r"(Rd) : "X"(CRn),"X"(CRm)); \
                                            vAR = Rd;                       	               \
                                        }


#define WRITE_CP14_REG(CRn, CRm, vALUE) {                               	                 \
                                            register int Rd = vALUE;           	             \
                                            __asm(" MCR p14, 0, %0, c%a1, c%a2, 0" :: "r"(Rd),"X"(CRn),"X"(CRm)); \
                                        }
#endif
#endif

// PMU tTest functions
//--------------------
void fPMUStart(void* param);
void fPMUConfigureRegisters(unsigned long nEvent0, unsigned long nEvent1, unsigned long nEvent2, unsigned long nEvent3);
void fPMUStop(UNSIGNED nTimerInd);
void fPMUTimerError(STATUS status);
void fPMUEnterIdleCore(void);
void fPMUExitIdleCore(void);
void fPMUCreateTimer(unsigned long nTimeoutDuration);
BOOL fPMUL1CanSleep(void);
void fPMUGetMultipleResultsSize(void* param);

/***************************************************************************
 * Local Functions
 ***************************************************************************/
extern void Delay(unsigned long  uSec);





#endif  /* _PMU_MONITORING_H_ */

