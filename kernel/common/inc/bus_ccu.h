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
/* Filename: bus_ccu.h                                                  */
/*                                                                      */
/* Author:   Idan Bartura                                               */
/*                                                                      */
/* Project, Target, subsystem: Tavor PV2, Seagull, HOP     				*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 11/08/2009                                                  */
/*                                                                      */
/*                                                                      */
/************************************************************************/
#ifndef _BUS_CCU_H_
#define _BUS_CCU_H_

#define DIVIDE_BY_1					1
#define DIVIDE_BY_2					2
#define DIVIDE_BY_3					3
#define DIVIDE_BY_4					4

#define SYS_PLL_DIV_2				312
#define SYS_PLL_BRANCH				416
#define COMM_PLL					520


/*-- Type definitions (T type) -----------------------------------------*/
typedef enum {
    BUS_CCU_RC_OK,
    BUS_CCU_RC_CLOCK_ERR = -100, //Error, the input clock's HW does not support frequencY selections.
    BUS_CCU_RC_FREQ_ERR,          //Error, the requested frequency is not legal.
    BUS_CCU_RC_WRONG_PLL,
    BUS_CCU_RC_NULL_POINTER
}BusCCU_ReturnCodeE;

//ICAT EXPORTED ENUM
typedef enum {
    BUS_CCU_TCU_CLK = 0,
    BUS_CCU_AIRQ_CLK,
    BUS_CCU_UART_CLK,
	BUS_CCU_USIM1_CLK,
	BUS_CCU_USIM2_CLK,
	BUS_CCU_I2C_CLK,
	BUS_CCU_ECIPHER_CLK,
	BUS_CCU_TIMER_CLK,   //(possibly change to timer 1,2,3
	BUS_CCU_GSSP_CLK,
	BUS_CCU_COMM_IPC_CLK,
	BUS_CCU_SCK_CLK, //10
	BUS_CCU_MSL_CLK,
	BUS_CCU_RESOURCE_IPC_CLK,
	BUS_CCU_AXI_CFG_CLK,
	BUS_CCU_ETB_CLK,
	BUS_CCU_DTC_CLK,
	BUS_CCU_TCU_CTRL_CLK,
	BUS_CCU_ADMA_CLK,
	BUS_CCU_NUM_OF_CLKS
}BusCCU_ClocksE;

//ICAT EXPORTED ENUM
typedef enum
{
    BUS_CCU_FREQ_13MHZ = 0,
    BUS_CCU_FREQ_26MHZ,
    BUS_CCU_FREQ_52MHZ,
    BUS_CCU_FREQ_78MHZ,
    BUS_CCU_FREQ_104MHZ,
	BUS_CCU_NUM_OF_FREQS,
    BUS_CCU_FREQ_NOT_SUPPORTED=BUS_CCU_NUM_OF_FREQS
}BusCCU_ClockFreqE;


//ICAT EXPORTED ENUM
typedef enum {
	BUS_CCU_SG_PLL_DIV_4 = 0,  	
	BUS_CCU_SG_PLL_DIV_3,
	BUS_CCU_SG_PLL_DIV_2,
	BUS_CCU_SG_PLL_DIV_1,
	BUS_CCU_SG_PLL_NUM_OF_FREQ_DIV
}BusCCU_SG_PLLFreqDivE; //Division from the PLL source which can be [SYS_PLL / 2 = 312MHz] or Comm PLL (416MHz default). [SDIV field]


typedef enum{

	 SG_MG_PP1			= 104, 
	 SG_MG_PP2			= 156, 
	 SG_MG_PP3			= 416,
	 SG_NEVOB0_PP1		= 156, 
	 SG_NEVOB0_PP2		= 312, 
	 SG_NEVOB0_PP3		= 416,
	 SG_NEVOC0_PP1		= 156, 
	 SG_NEVOC0_PP2		= 312, 
	 SG_NEVOC0_PP3		= 520
}SGFreqInMHz;
//ICAT EXPORTED ENUM
typedef enum {
	BUS_CCU_SG_SYS_PLL_DIV_2 	= 0x0,
	BUS_CCU_SG_COMM_PLL_MG 		= 0x1,
	BUS_CCU_SG_COMM_PLL_NEVO 	= 0x3,
	BUS_CCU_SG_SYS_PLL_416		= 0x1,
	BUS_CCU_SG_COMM_PLL			= 0x3,
	BUS_CCU_SG_PLL_NUM_OF_PLL_SRC
}BusCCU_SG_PLLsourceE; //This controls the source of the Seagull core PLL. [SPLL field]

//ICAT EXPORTED ENUM
typedef enum {
	BUS_CCU_GB_PLL_DIV_4 = 0,  
	BUS_CCU_GB_PLL_DIV_3,
	BUS_CCU_GB_PLL_DIV_2,
	BUS_CCU_GB_PLL_DIV_1,
	BUS_CCU_GB_PLL_NUM_OF_FREQ_DIV
}BusCCU_GB_PLLFreqDivE; //Division from the PLL source which can be [SYS_PLL / 2 = 312MHz] or Comm PLL (416MHz default).[GDIV field]

//ICAT EXPORTED ENUM
typedef enum {
	BUS_CCU_GB_SYS_PLL_DIV_2	= 0x0,
	BUS_CCU_GB_SYS_PLL_416		= 0x1,
	BUS_CCU_GB_COMM_PLL_MG		= 0x1,
	BUS_CCU_GB_COMM_PLL_NEVO	= 0x3,
	BUS_CCU_GB_PLL_NUM_OF_PLL_SRC
}BusCCU_GB_PLLsourceE; //This controls the source of the GrayBack core PLL. [GPLL field]

//ICAT EXPORTED ENUM
typedef enum {
    BUS_CCU_CLK_DISABLE = 0,
    BUS_CCU_CLK_ENABLE
}BusCCU_ClockStateE;


//ICAT EXPORTED ENUM
typedef enum {
    BUS_CCU_CLK_CONTROLLED = 0,
	BUS_CCU_CLK_ALWAYS_ON
}BusCCU_ClockAlwaysOnE;

typedef enum
{
    BUS_CCU_BIT_DISABLE = 0,
    BUS_CCU_BIT_ENABLE,
}BusCCU_Bit_StatusE;

typedef enum
{
    BUS_CCU_COMM_PLL_DISABLE = 0,
    BUS_CCU_COMM_PLL_ENABLE
}BusCCU_Comm_PLL_StatusE;


//ICAT EXPORTED ENUM
typedef enum {
    APB_BUS = 0,
	AXI_BUS,
	GPB_BUS,
	NUM_OF_BUSES,
	UNKNOWN_BUS
}BusCCU_BusTypeE;

//ICAT EXPORTED STRUCT
typedef struct
{
	 BusCCU_SG_PLLFreqDivE  SeagullFreqDiv;
	 BusCCU_SG_PLLsourceE   SeagullPLLSelect;
	 BusCCU_GB_PLLFreqDivE  GraybackFreqDiv;
	 BusCCU_GB_PLLsourceE   GraybackPLLSelect;

	 SGFreqInMHz			SGCoreFreqInMHZ;

}SG_GBFreqSelectS;

// for defaults and always on clocks
typedef struct {
	UINT32 DefaultOnClks;
	UINT32 AlwaysOnClks;
} BusCCU_def_on_clkS;
/*-- -----Public Functions Prototypes ----------------------------------*/
SG_GBFreqSelectS 	BusCCUSystemPLLFreqGet(void);
BusCCU_ClockStateE      BusCCUClockStateGet (BusCCU_ClocksE clock);
BusCCU_ReturnCodeE      BusCCUClockFreqSelect(BusCCU_ClocksE ClkSelected , BusCCU_ClockFreqE ClkFreq);
BusCCU_ClockFreqE       BusCCUClockFreqGet(BusCCU_ClocksE ClkSelected);

/*-- -----Public Functions Prototypes ----------------------------------*/
UINT32 				 BusCCUFreqToOffset(SG_GBFreqSelectS freq);
void                 BusCCUClockStateSet(BusCCU_ClocksE clock, BusCCU_ClockStateE enableDisable);
void                 BusCCUPhase1Init(void);
void                 BusCCUPhase2Init(void);
BusCCU_ClockStateE  BusCCUClockStateGet (BusCCU_ClocksE clock);
void BusCCUEnableD1Uartdependency(BOOL enable);
void BusCCUSetAlwaysOnClk(void);
void BusCCUAPBShutDownControl(UINT32 OnOff);
void 				 SetSeagullVoting(UINT32 ForceHWwrite);
void BusCCUSystemPLLFreqSelect(SG_GBFreqSelectS SelectPLLfreq, BOOL CommPLLisNeeded);
void 				 BusCCUSystemPLLVoltageSelect(SG_GBFreqSelectS Freq);
void  BusCCUI2CHighFreqSelect(BusCCU_ClockStateE enableDisable);
void SetSagullVoteBusShutdown(BusCCU_BusTypeE bus_type, UINT32 OnOff);

#endif /*_BUS_CCU_H_*/
