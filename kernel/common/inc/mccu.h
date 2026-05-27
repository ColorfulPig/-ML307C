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
/* Title: Modem Clock Control Unit Header File                          */
/*                                                                      */
/* Filename: mccu.h                                                     */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created:  15/11/2004                                                 */
/*                                                                      */
/* Modified: 18/03/2005 - Added enum typedefs for non-temp package.     */
/************************************************************************/
#ifndef _MCCU_H_
#define _MCCU_H_

/*-- Type definitions (T type) -----------------------------------------*/

//This enum describes the various values that may be returned when using this package API:
typedef enum
{	
    MCCU_RC_OK,               //The operation was performed successfully.
    MCCU_RC_CLOCK_ERR = -100, //Error, the input clock's HW does not support frequencY selections.
    MCCU_RC_FREQ_ERR          //Error, the requested frequency is not legal.
}MCCU_ReturnCodeE;

//This enum describes the various clocks on which the MCCU controls:
typedef enum	
{
    MCCU_CLK_DSSP0 = 0,
    MCCU_CLK_DSSP1,
    MCCU_CLK_DSSP2,
    MCCU_CLK_DSSP3,
    MCCU_CLK_WB_CIPHER,
    MCCU_CLK_SCK,
    MCCU_CLK_E_CIPHER,
    MCCU_CLK_E_CIPHER_26MHZ,
    MCCU_CLK_VITERBI,
    MCCU_CLK_EQUALIZER,
    MCCU_CLK_TTPCOM, //TTPComm modules: MIRQ, ASPECT
    MCCU_CLK_HSL,
    MCCU_CLK_TCU,
    MCCU_NUM_OF_CLKS
}MCCU_ClocksE;

//rafiagiv 27.02
typedef enum	
{
    MCCU_BIT_DISABLE = 0,
    MCCU_BIT_ENABLE,  
}MCCU_Bit_StatusE;

//rafiagiv 27.2 - 2 bits which control L1 GSM/WB setting (not clocks nor freq)
typedef enum	
{
    MCCU_WB_SLP_EQ_EN = 0,
	MCCU_WB_WAKE_SEL,
	MCCU_WB_NUM_OF_BITS
}MCCU_WB_GSM_Control_BitsE;


// This enum describes the requested clock status that may be asked or returned:
typedef enum
{
    MCCU_CLK_ENABLE, //To enable a clock / clock is enabled.
    MCCU_CLK_DISABLE //To disable a clock / clock is disabled.
}MCCU_ClockStateE;

// This enum describes the available clock frequencies for all MCCU controlled peripherals:
typedef enum
{	
    MCCU_FREQ_13MHZ = 0,
    MCCU_FREQ_26MHZ,
    MCCU_FREQ_52MHZ,
    MCCU_FREQ_78MHZ,
    MCCU_FREQ_104MHZ,
	MCCU_NUM_OF_FREQS,
    MCCU_FREQ_NOT_SUPPORTED=MCCU_NUM_OF_FREQS
}MCCU_ClockFreqE;

// for defaults and always on clocks (GCGEN/ CCCR)
typedef struct {
	UINT32 MCCUDefaultOnClks;	// default-om includes also the always on Set on run-time - as this MCCU clocks are set by default on after D2 exit
	UINT32 MCCUAlwaysOnClks;
} MCCU_def_on_clkS;

//defined in hop/rm/src/rm_memRetain.c - as it needs to be in memory retain - and mccu does not have code/data in DTCM
extern MCCU_def_on_clkS MCCU_s;
/*----------------------------------------------------------------------*/
/*-- Constants definitions (C type) ------------------------------------*/
/*----------------------------------------------------------------------*/
/*-- Global variables declarations (Public G type) ---------------------*/
/*----------------------------------------------------------------------*/
/*-- -----Public Functions Prototypes ----------------------------------*/
void             MCCUClockStateSet  (MCCU_ClocksE,MCCU_ClockStateE);
void             MCCUWbGsmBitSet (MCCU_WB_GSM_Control_BitsE controlBit ,MCCU_Bit_StatusE enableDisable); //rafiagiv 27.12
MCCU_Bit_StatusE MCCUWbGsmBitGet (MCCU_WB_GSM_Control_BitsE controlBit);
void             MCCUPhase1Init     (void);
void             MCCUPhase2Init     (void);
MCCU_ReturnCodeE MCCUClockFreqSelect(MCCU_ClocksE,MCCU_ClockFreqE);
MCCU_ClockFreqE MCCUClockFreqGet(MCCU_ClocksE clk);
void			MCCUSetAlwaysOnClk(MCCU_ClocksE clk, UINT32 bOnState);	//Update for default/always on clock - exception cases
/*----------------------------------------------------------------------*/
/*-------Validation Functions-------------------------------------------*/
MCCU_ClockStateE MCCUClockStateGet(MCCU_ClocksE);
/*----------------------------------------------------------------------*/

#endif  /* _MCCU_H_ */


