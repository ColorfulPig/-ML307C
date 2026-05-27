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
/* Title: Arbel Resource Manager Look Up Tables Header File             */
/*                                                                      */
/* Filename: rm_lut.h                                                   */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 11/4/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _RM_LUT_H_
#define _RM_LUT_H_

#include "rm_internal.h"
#include "commccu.h"
#include "mccu.h"
#include "commpm.h"
#include "cpmu.h"

#include "rm_db.h"
#include "rm_def.h"
#include "rm_types.h"


//Used for the look-up-tables
#define NO_MCCU_CLK                 MCCU_NUM_OF_CLKS
#define NO_COMMCCU_CLK              COMMCCU_NUM_OF_CLKS
#define CPMU_RESET_NOT_SUPPORTED    (0xFFFFFFFFUL)

typedef const struct {
    RM_MultiUserClocksE         serviceToMultiUserClocks[RM_NUM_OF_SRVCS][RM_MAX_MUC_PER_SRVC];
    CommCCU_ClocksE             serviceToCommCCUClock[RM_NUM_OF_SRVCS];
    MCCU_ClocksE                serviceToMCCUClock[RM_NUM_OF_SRVCS];
    RM_CurrentResourcesStatusT  serviceToMainResourceBitVector[RM_NUM_OF_SRVCS];
    MCCU_ClockFreqE             rmFreqtoMCCUFreq[RM_NUM_OF_FREQS];
    RM_CurrentResourcesStatusT  multiUserClkToResourceBitVector[NUMBER_OF_MULTI_USER_CLOCKS];
    CommCCU_ClocksE             multiUserToCommCCU[NUMBER_OF_MULTI_USER_CLOCKS];
    CommPM_WakeupSourcesE       cbkServiceToCommPMWakeup[RM_NUM_OF_CALLBACK_SRVCS];
    UINT32                      rmServiceToCPMUPeripheral[RM_NUM_OF_SRVCS];
	// for ISPT
	#if defined (ISPT_OVER_SSP)
	UINT32						serviceToISPTclockVal[RM_NUM_OF_SRVCS];
	UINT32						multiUserClockToISPTclockVal[NUMBER_OF_MULTI_USER_CLOCKS];
	#endif
} RM_LookUpTablesS;

static RM_LookUpTablesS rmLookUpTables =
{
    {   // serviceToMultiUserClocks

        // RM_SRVC_DSSP3
        {MULTI_USER_CLK_DSSP_BUS , MULTI_USER_CLK_MODEM_104MHZ},

        // RM_SRVC_GSSP2
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_I2C
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_MSL
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_USIM
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_TIMER0_13M
        {MULTI_USER_CLK_VCTCXO , NO_MULTI_USER_CLOCK},

        // RM_SRVC_TIMER1_13M
        {MULTI_USER_CLK_VCTCXO , NO_MULTI_USER_CLOCK},

        // RM_SRVC_DDR_HIGH_FREQ    --   RM_SRVC_IPC //rafiagiv   21.12.06
        {MULTI_USER_CLK_DDR_HIGH_FREQ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_CGPIO
		{NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

		// RM_SRVC_DMA
        {MULTI_USER_CLK_DSSP_BUS , NO_MULTI_USER_CLOCK},

        // RM_SRVC_TCU
        {MULTI_USER_CLK_MODEM_104MHZ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_SCK
        {MULTI_USER_CLK_MODEM_104MHZ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_WB_SLEEP_MODULE
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_VCTCXO
        {MULTI_USER_CLK_VCTCXO , NO_MULTI_USER_CLOCK},

        // RM_SRVC_GSSP1_GB
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_WB_CIPHER_GB
        {MULTI_USER_CLK_MODEM_104MHZ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_CPA_DDR_HPerf
        {MULTI_USER_CLK_DDR_HIGH_FREQ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_FREE_3             //RM_SRVC_VITERBI_GB, rafiagiv 28.12.06
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_FREE_4,             //RM_SRVC_EQUALIZER_GB, rafiagiv 28.12.06
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

        // RM_SRVC_DSSP2_GB
        {MULTI_USER_CLK_DSSP_BUS , MULTI_USER_CLK_MODEM_104MHZ},

        // RM_SRVC_DSSP1_GB
        {MULTI_USER_CLK_DSSP_BUS , MULTI_USER_CLK_MODEM_104MHZ},

        // RM_SRVC_DSSP0_GB
        {MULTI_USER_CLK_DSSP_BUS , MULTI_USER_CLK_MODEM_104MHZ},

        // RM_SRVC_TTPCOM_GB
        {MULTI_USER_CLK_MODEM_104MHZ , NO_MULTI_USER_CLOCK},

        // RM_SRVC_TIMER2_13M_GB
        {MULTI_USER_CLK_VCTCXO , NO_MULTI_USER_CLOCK},

        // RM_SRVC_TIMER3_13M_GB
        {MULTI_USER_CLK_VCTCXO , NO_MULTI_USER_CLOCK},

		// RM_SRVC_UART
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

		 // RM_SRVC_DVFM
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

		 // RM_SRVC_ECIPHER - not supported at the moment
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

		 //	RM_SRVC_USIM2 - not supported at the moment
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK},

		 //	RM_SRVC_SDIO - not supported at the moment
        {NO_MULTI_USER_CLOCK , NO_MULTI_USER_CLOCK}
    },
    {   // serviceToCommCCUClock

        //RM_SRVC_DSSP3
        NO_COMMCCU_CLK,

        //RM_SRVC_GSSP2
        COMMCCU_CLK_GSSP2,

        //RM_SRVC_I2C
        COMMCCU_CLK_I2C,

        //RM_SRVC_MSL
        COMMCCU_CLK_MSL,

        //RM_SRVC_USIM
        COMMCCU_CLK_USIM,

        //RM_SRVC_TIMER0_13M
        NO_COMMCCU_CLK,

        //RM_SRVC_TIMER1_13M
        NO_COMMCCU_CLK,

        //RM_SRVC_DDR_HIGH_FREQ
        NO_COMMCCU_CLK,

        //RM_SRVC_CGPIO
        NO_COMMCCU_CLK,

        //RM_SRVC_DMA
        NO_COMMCCU_CLK,

        //RM_SRVC_TCU
        NO_COMMCCU_CLK,

        //RM_SRVC_SCK
        NO_COMMCCU_CLK,

        //RM_SRVC_WB_SLEEP_MODULE
        NO_COMMCCU_CLK,

        //RM_SRVC_VCTCXO
        NO_COMMCCU_CLK,

        //RM_SRVC_GSSP1_GB
        COMMCCU_CLK_GSSP1,

        //RM_SRVC_WB_CIPHER_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_CPA_DDR_HPerf
        NO_COMMCCU_CLK,

        //RM_SRVC_FREE_3
        NO_COMMCCU_CLK,

        //RM_SRVC_FREE_4
        NO_COMMCCU_CLK,

        //RM_SRVC_DSSP2_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_DSSP1_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_DSSP0_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_TTPCOM_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_TIMER2_13M_GB
        NO_COMMCCU_CLK,

        //RM_SRVC_TIMER3_13M_GB
        NO_COMMCCU_CLK,

		//RM_SRVC_UART
        COMMCCU_CLK_UART,

		//RM_SRVC_DVFM
        NO_COMMCCU_CLK,

		//RM_SRVC_ECIPHER - not supported at the moment
		NO_COMMCCU_CLK,

		// RM_SRVC_USIM2 - not supported at the moment
		NO_COMMCCU_CLK,

		//	RM_SRVC_SDIO - not supported at the moment
		NO_COMMCCU_CLK
    },
    {   // serviceToMCCUClock

        //RM_SRVC_DSSP3
        MCCU_CLK_DSSP3,

        //RM_SRVC_GSSP2
        NO_MCCU_CLK,

        //RM_SRVC_I2C
        NO_MCCU_CLK,

        //RM_SRVC_MSL
        NO_MCCU_CLK,

        //RM_SRVC_USIM
        NO_MCCU_CLK,

        //RM_SRVC_TIMER0_13M
        NO_MCCU_CLK,

        //RM_SRVC_TIMER1_13M
        NO_MCCU_CLK,

        //RM_SRVC_DDR_HIGH_FREQ
        NO_MCCU_CLK,

        //RM_SRVC_CGPIO
        NO_MCCU_CLK,

		//RM_SRVC_DMA
        NO_MCCU_CLK,

        //RM_SRVC_TCU
        MCCU_CLK_TCU,

        //RM_SRVC_SCK
        MCCU_CLK_SCK,

        //RM_SRVC_WB_SLEEP_MODULE
        NO_MCCU_CLK,

        //RM_SRVC_VCTCXO
        NO_MCCU_CLK,

        //RM_SRVC_GSSP1_GB
        NO_MCCU_CLK,

        //RM_SRVC_WB_CIPHER_GB
        MCCU_CLK_WB_CIPHER,

        // RM_SRVC_CPA_DDR_HPerf
        NO_MCCU_CLK,

        //RM_SRVC_FREE_3             //RM_SRVC_VITERBI_GB, rafiagiv 28.12.06
        NO_MCCU_CLK,

        //RM_SRVC_FREE_4             //RM_SRVC_EQUALIZER_GB, rafiagiv 28.12.06
        NO_MCCU_CLK,

        //RM_SRVC_DSSP2_GB
        MCCU_CLK_DSSP2,

        //RM_SRVC_DSSP1_GB
        MCCU_CLK_DSSP1,

        //RM_SRVC_DSSP0_GB
        MCCU_CLK_DSSP0,

        //RM_SRVC_TTPCOM_GB
        MCCU_CLK_TTPCOM,

        //RM_SRVC_TIMER2_13M_GB
        NO_MCCU_CLK,

        //RM_SRVC_TIMER3_13M_GB
        NO_MCCU_CLK,

		//RM_SRVC_UART
		NO_MCCU_CLK,

		//RM_SRVC_DVFM
		NO_MCCU_CLK,

		//RM_SRVC_ECIPHER- not supported at the moment
		NO_MCCU_CLK,

		// RM_SRVC_USIM2 - not supported at the moment
		NO_MCCU_CLK,

		//	RM_SRVC_SDIO - not supported at the moment
		NO_MCCU_CLK
    },
    {   // serviceToMainResourceBitVector

        //RM_SRVC_DSSP3
        RSRC_DSSP3, // Uses also: RSRC_MODEM_104MHZ, RSRC_DSSP_BUS

        //RM_SRVC_GSSP2
        RSRC_GSSP2,

        //RM_SRVC_I2C
        RSRC_I2C,

        //RM_SRVC_MSL
        RSRC_MSL,

        //RM_SRVC_USIM
        RSRC_USIM,

        //RM_SRVC_TIMER0_13M
        RSRC_TIMER0_13M,// Uses also: RSRC_VCTCXO

        //RM_SRVC_TIMER_13M
        RSRC_TIMER1_13M,// Uses also: RSRC_VCTCXO

        //RM_SRVC_DDR_HIGH_FREQ   --  RM_SRVC_IPC //rafiagiv  25.12.06
        RSRC_DDR_HIGH_FREQ,              //RSRC_IPC,

        //RM_SRVC_CGPIO
        RSRC_CGPIO,

        //RM_SRVC_DMA
        RSRC_DMA,

        //RM_SRVC_TCU
        RSRC_TCU,// Uses also: RSRC_MODEM_104MHZ

        //RM_SRVC_SCK
        RSRC_SCK_FUNC, // Uses also: RSRC_MODEM_104MHZ

        //RM_SRVC_WB_SLEEP_MODULE
        RSRC_WB_SLEEP_MODULE,

        //RM_SRVC_VCTCXO
        RSRC_VCTCXO,

        //RM_SRVC_GSSP1_GB
        RSRC_GSSP1_GB,

        //RM_SRVC_WB_CIPHER_GB
        RSRC_WB_CIPHER_GB,			  // Uses also: RSRC_MODEM_104MHZ

        // RM_SRVC_CPA_DDR_HPerf
        RSRC_CPA_DDR_HPerf,				  // Uses also : RM_SRVC_DDR_HIGH_FREQ

        //RM_SRVC_FREE_3              // RM_SRVC_VITERBI_GB, rafiagiv 28.12.06
        RSRC_FREE_3,				  // Uses also: RSRC_MODEM_78MHZ, RSRC_MODEM_104MHZ

        //RM_SRVC_FREE_4              //RM_SRVC_EQUALIZER_GB, rafiagiv 28.12.06
        RSRC_FREE_4,				  // Uses also: RSRC_MODEM_78MHZ, RSRC_MODEM_104MHZ

        //RM_SRVC_DSSP2_GB
        RSRC_DSSP2_GB, // Uses also: RSRC_MODEM_104MHZ,RSRC_DSSP_BUS

        //RM_SRVC_DSSP1_GB
        RSRC_DSSP1_GB, // Uses also: RSRC_MODEM_104MHZ,RSRC_DSSP_BUS

        //RM_SRVC_DSSP0_GB
        RSRC_DSSP0_GB, // Uses also: RSRC_MODEM_104MHZ,RSRC_DSSP_BUS

        //RM_SRVC_TTPCOM_GB
        RSRC_TTPCOMM_GB, // Uses also: RSRC_MODEM_104MHZ

        //RM_SRVC_TIMER2_13M_GB
        RSRC_TIMER2_13M_GB, // Uses also: RSRC_VCTCXO

        //RM_SRVC_TIMER3_13M_GB
        RSRC_TIMER3_13M_GB, // Uses also: RSRC_VCTCXO

		//RM_SRVC_UART
        (UINT32)RSRC_UART,

		//RM_SRVC_DVFM
		RSRC_DVFM,

		//RM_SRVC_ECIPHER - not supported at the moment
		NO_ALLOCATED_RESOURCES,

		//RM_SRVC_USIM2 - not supported at the moment
		NO_ALLOCATED_RESOURCES,

		//RM_SRVC_SDIO - not supported at the moment
		NO_ALLOCATED_RESOURCES
    },
    {   // rmFreqtoMCCUFreq

        //RM_FREQ_13MHZ
        MCCU_FREQ_13MHZ,

        //RM_FREQ_26MHZ
        MCCU_FREQ_26MHZ,

        //RM_FREQ_52MHZ
        MCCU_FREQ_52MHZ,

        //RM_FREQ_78MHZ
        MCCU_FREQ_78MHZ,

        //RM_FREQ_104MHZ
        MCCU_FREQ_104MHZ
    },
    {   // multiUserClkToResourceBitVector

        //MULTI_USER_CLK_DSSP_BUS
        RSRC_DSSP_BUS,

        //MULTI_USER_CLK_VCTCXO
        RSRC_VCTCXO,

        //MULTI_USER_CLK_MODEM_78MHZ
        RSRC_MODEM_78MHZ,

        //MULTI_USER_CLK_MODEM_104MHZ
        RSRC_MODEM_104MHZ,

		//MULTI_USER_CLK_DDR_HIGH_FREQ   rafiagiv - 21/12
		RSRC_DDR_HIGH_FREQ
    },
    {   // multiUserToCommCCU

        //MULTI_USER_CLK_DSSP_BUS
        COMMCCU_CLK_DSSP_BUS,

        //MULTI_USER_CLK_VCTCXO
        NO_COMMCCU_CLK, /*ERROR - this entry will never be referenced*/

        //MULTI_USER_CLK_MODEM_78MHZ
        COMMCCU_MODEM_CLK_78MHZ,

        //MULTI_USER_CLK_MODEM_104MHZ
        COMMCCU_MODEM_CLK_104MHZ,

		//MULTI_USER_CLK_DDR_HIGH_FREQ   rafiagiv - 21/12
		NO_COMMCCU_CLK
    },
    { //cbkServiceToCommPMWakeup

        //RM_CALLBACK_SRVC_INTC,
        COMMPM_NUM_OF_WAKEUP_SOURCES, //Wakeup not supported

        //RM_CALLBACK_SRVC_TIMER
        COMMPM_WAKEUP_SOURCE_TIMER,

        //RM_CALLBACK_SRVC_SSP
        COMMPM_WAKEUP_SOURCE_SSP,

        //RM_CALLBACK_SRVC_DMA,
        COMMPM_NUM_OF_WAKEUP_SOURCES, //Wakeup not supported

        //RM_CALLBACK_SRVC_I2C
        COMMPM_WAKEUP_SOURCE_I2C,

        //RM_CALLBACK_SRVC_WDT
        COMMPM_WAKEUP_SOURCE_WDT,

        //RM_CALLBACK_SRVC_IPC,
        COMMPM_NUM_OF_WAKEUP_SOURCES, //Wakeup not supported

        //RM_CALLBACK_SRVC_USIM
        COMMPM_WAKEUP_SOURCE_USIM,

        //RM_CALLBACK_SRVC_PMIC
        COMMPM_WAKEUP_SOURCE_PMIC,

        //RM_CALLBACK_SRVC_MSL
        COMMPM_WAKEUP_SOURCE_MSL,

        //RM_CALLBACK_SRVC_SCK
        COMMPM_WAKEUP_SOURCE_SCK,

        //RM_CALLBACK_SRVC_WB_SLEEP_MODULE
        COMMPM_WAKEUP_SOURCE_WB_SLEEP_MODULE,

        //RM_CALLBACK_SRVC_TCU,
        COMMPM_WAKEUP_SOURCE_TCU,

		//RM_CALLBACK_SRVC_UART
        COMMPM_WAKEUP_SOURCE_UART,

		//RM_CALLBACK_SRVC_AC_IPC
		COMMPM_WAKEUP_SOURCE_AC_IPC,

		//RM_CALLBACK_SRVC_CGPIO
		COMMPM_WAKEUP_SOURCE_CGPIO
    },
    {   //rmServiceToCPMUPeripheral
        //RM_SRVC_DSSP3
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_GSSP2
        CPMU_PERIPHERAL_GSSP2,

        //RM_SRVC_I2C
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_MSL
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_USIM
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TIMER0_13M
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TIMER1_13M
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_DDR_HIGH_FREQ
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_CGPIO
		CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_DMA
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TCU
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_SCK
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_WB_SEARCH
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_VCTCXO
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_GSSP1_GB
        CPMU_PERIPHERAL_GSSP1,

        //RM_SRVC_WB_CIPHER_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_CPA_DDR_HPerf
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_FREE_3
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_FREE_4
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_DSSP2_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_DSSP1_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_DSSP0_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TTPCOM_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TIMER2_13M_GB
        CPMU_RESET_NOT_SUPPORTED,

        //RM_SRVC_TIMER3_13M_GB
        CPMU_RESET_NOT_SUPPORTED,

		//RM_SRVC_UART
        CPMU_RESET_NOT_SUPPORTED,

		//RM_SRVC_DVFM
        CPMU_RESET_NOT_SUPPORTED,

		//RM_SRVC_ECIPHER - - not supported at the moment
		CPMU_RESET_NOT_SUPPORTED,

		//RM_SRVC_USIM2 - not supported at the moment
		CPMU_RESET_NOT_SUPPORTED,

		//RM_SRVC_SDIO - not supported at the moment
		CPMU_RESET_NOT_SUPPORTED
    }
	#if defined (ISPT_OVER_SSP)
	,
	{  //UINT32	serviceToISPTclockVal[RM_NUM_OF_SRVCS];
		//RM_SRVC_DSSP3 = 0,
		19,
		//RM_SRVC_GSSP2,
		6,
		//RM_SRVC_I2C,
		4,
	    //RM_SRVC_MSL,
		8,
	    //RM_SRVC_USIM,
		9,
	    //RM_SRVC_TIMER0_13M,
		0xFF,			// not assign
	    //RM_SRVC_TIMER1_13M,
		0xFF,			// not assign
	    //RM_SRVC_DDR_HIGH_FREQ,
		0xFF,			// not assign
	    //RM_SRVC_CGPIO,
		0xFF,			// not assign
	    //RM_SRVC_DMA,
		0xFF,			// not assign
	    //RM_SRVC_TCU,
		13,
	    //RM_SRVC_SCK,
		24,
	    //RM_SRVC_WB_SLEEP_MODULE,
		0xFF,				//  not assign
	    //RM_SRVC_VCTCXO,
		0xFF,				// not assign
	    //RM_SRVC_GSSP1_GB,
		5,
	    //RM_SRVC_WB_CIPHER_GB,
		25,
	    //RM_SRVC_CPA_DDR_HPerf,      //for L1 to control high freq DDR
	    0xFF,				// not assign
	    //RM_SRVC_FREE_3,             //RM_SRVC_VITERBI_GB, rafiagiv 28.12.06
		0xFF,				// not assign
	    //RM_SRVC_FREE_4,             //RM_SRVC_EQUALIZER_GB, rafiagiv 28.12.06
		0xFF,				// not assign
	    //RM_SRVC_DSSP2_GB,
		18,
	    //RM_SRVC_DSSP1_GB,
		17,
	    //RM_SRVC_DSSP0_GB,
		16,
	    //RM_SRVC_TTPCOM_GB,
		0xFF,				// not asign
	    //RM_SRVC_TIMER2_13M_GB,
		0xFF,				// not asign
	    //RM_SRVC_TIMER3_13M_GB,
		0xFF,				// not asign
		//RM_SRVC_UART,
		27,
		//RM_SRVC_DVFM,
		0xFF,				// not asign
		//RM_SRVC_ECIPHER - not supported at the moment
		0xFF,				// not asign
		//RM_SRVC_USIM2 - not supported at the moment
		0xFF,				// not asign
		//RM_SRVC_SDIO - not supported at the moment
		0xFF				// not asign
	},
	{  //UINT32	multiUserClockToISPTclockVal[NUMBER_OF_MULTI_USER_CLOCKS];
	    //MULTI_USER_CLK_DSSP_BUS=0,
		0xFF,				// not asign
	    //MULTI_USER_CLK_VCTCXO,
		0xFF,				//  not asign
	    //MULTI_USER_CLK_MODEM_78MHZ,
		1,
	    //MULTI_USER_CLK_MODEM_104MHZ,
		2,
		//MULTI_USER_CLK_DDR_HIGH_FREQ,   //rafiagiv - 21/12
		0xFF				// not asign
	}
	#endif

};

#endif  /* _RM_LUT_H_ */
