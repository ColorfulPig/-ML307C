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
/* Title: Comm. sub-system Clock Control Unit Header File               */
/*                                                                      */
/* Filename: commccu.h                                                  */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 4/11/2004                                                   */
/*                                                                      */
/* Modified: 01/04/2005                                                 */
/************************************************************************/
#ifndef _COMMCCU_H_
#define _COMMCCU_H_

/*-- Type definitions (T type) -----------------------------------------*/
typedef enum {
    COMMCCU_RC_OK,
    COMMCCU_RC_WRONG_PLL = -100,
    COMMCCU_RC_NULL_POINTER
}CommCCU_ReturnCodeE;

typedef enum {
    COMMCCU_MODEM_CLK_312MHZ_GB = 0,
    COMMCCU_MODEM_CLK_208MHZ_GB,
    COMMCCU_MODEM_CLK_104MHZ,
	COMMCCU_MODEM_CLK_78MHZ,
	COMMCCU_CLK_I2C,
	COMMCCU_CLK_GSSP1,
	COMMCCU_CLK_GSSP2,
	COMMCCU_CLK_MSL,
	COMMCCU_CLK_USIM,
	COMMCCU_CLK_DSSP_BUS,
	COMMCCU_CLK_UART,
	COMMCCU_NUM_OF_CLKS
}CommCCU_ClocksE;

typedef enum {
    COMMCCU_CLK_DISABLE = 0,
    COMMCCU_CLK_ENABLE
}CommCCU_ClockStateE;

//ICAT EXPORTED ENUM
typedef enum {
	COMMCCU_SYS_PLL_FREQ_52MHZ = 0,
	COMMCCU_SYS_PLL_FREQ_104MHZ,
	COMMCCU_SYS_PLL_FREQ_156MHZ,
	COMMCCU_SYS_PLL_FREQ_208MHZ,
	COMMCCU_SYS_PLL_FREQ_312MHZ,
	COMMCCU_SYS_PLL_NUM_OF_FREQS
}CommCCU_SystemPLLFreqE;

typedef enum {
    COMMCCU_PLL_SYS = 0,
    COMMCCU_PLL_GB
}CommCCU_PLLsE;

typedef struct {
    CommCCU_PLLsE          usedPll;
    CommCCU_SystemPLLFreqE systemPllFreq;
    BOOL                   isSystemPllLocked;
    BOOL                   isGbPllLocked;
}CommCCU_PLLsStatusS;

// for defaults and always on clocks (GCGEN/ CCCR)
typedef struct {
	UINT32 CCCRDefaultOnClks;
	UINT32 CCCRAlwaysOnClks;
	UINT32 GCKENDefaultOnClks;
	UINT32 GCKENAlwaysOnClks;
} CommCCU_def_on_clkS;

extern CommCCU_def_on_clkS commCCU_s;
/*----------------------------------------------------------------------*/
/*-- Constants definitions (C type) ------------------------------------*/

/*----------------------------------------------------------------------*/
/*-- Global variables declarations (Public G type) ---------------------*/
/*----------------------------------------------------------------------*/
/*-- -----Public Functions Prototypes ----------------------------------*/
void                 CommCCUPLLSelect(CommCCU_PLLsE);
void                 CommCCUClockStateSet(CommCCU_ClocksE,CommCCU_ClockStateE);
void                 CommCCUSystemPLLFreqSelect(CommCCU_SystemPLLFreqE);
void                 CommCCUPhase1Init(void);
void                 CommCCUPhase2Init(void);
CommCCU_ClockStateE  CommCCUClockStateGet (CommCCU_ClocksE);
CommCCU_ReturnCodeE  CommCCUPLLsStatusGet (CommCCU_PLLsStatusS*);
CommCCU_SystemPLLFreqE CommCCUSystemPLLFreqGet(void);
void CommCCUSystemPLLVoltageSelect(CommCCU_SystemPLLFreqE Freq);
void CommCCUEnableD1Uartdependency(BOOL enable);
void CommCCUSetAlwaysOnClk(CommCCU_ClocksE commccuClock, UINT32 bOnState);
void CommCCUToggleUSIM2(void);
/*----------------------------------------------------------------------*/
/*-------Test Function--------------------------------------------------*/
/*----------------------------------------------------------------------*/




/*
 *	OLD FUNCIONS THAT WILL BE REMOVED GRADUALLY (ASAP)
 */

void CCUDefaultConfigSet(void);
void CCUTurnClocksOff(void);
void CCUSysPLL312MHzSet(void);


 #endif  /* _COMMCCU_H_ */


