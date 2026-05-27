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
/* Title: Comm. sub-system Clock Control Unit HW Header File            */
/*                                                                      */
/* Filename: commccu_hw.h                                               */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: Note that this code controls all of Grayback's registers	*/
/*			even though its logic relation is to Arbel resources only.	*/
/*			The purpose is to provide with a fallback tool that will    */
/*			allow control to GB's registers as well.					*/
/*    													                */
/* Created: 4/11/2004                                                   */
/*                                                                      */
/* Modified: 2/4/2005                                                   */
/*                                                                      */
/************************************************************************/

#ifndef _COMMCCU_HW_H_
#define _COMMCCU_HW_H_

#include "global_types.h"    //  needed for the UINT32 definition.

#define COMMCCU_BASE_ADDRESS         0xF00E0000UL

//address of GPBMCSR_REG for control of uart clock.
#define GPBMCSR_REG_ADDRESS			 0xF024000CUL

/* Define the timeout properties according to the GB hardware characteristics.
 * The info was given by GB team (Dai, Xiuhong)
 *
 * CCU core clock frequency change:
 * 25 old frequency cycles + 3 new frequency cycles for frequency changes within system PLL.
 *
 * Thus we set the length of the loop  to be 28 (no doubt it will take more than 28 cycles now...)
 *
 * While in D0, Grayback PLL is active (and obviously locked..) as it clocks the DDR - Thus
 * PLL lock time was not taken into consideration.
 *
 */

#define CLOCK_CHANGE_TIMEOUT_LOOP_LENGTH 500  // Was 28. (Changed to make it work better.)


// Defines the CommCCU HW-Structure
	/** (See GB IAS for special limitaions regarding reserved bits and other remarks) **/
typedef volatile struct
{
    UINT32      CCCR;   //	Communication subsystem Clock Configuration Register    R/W
    UINT32		CCSR;	//	Communication subsystem Clock Status Register			 R
    UINT32		GCCCR;  //  Grayback Core Clock Control Register					R/W
    UINT32		GCKEN;  //  Grayback Clock Enable Register							R/W
} CommCCU_HW_RegistersS;

// masks definitions
// The following masks definitions are to be used in this package only.

// GCKEN masks
#define	GCKEN_I2C_CLK		         0x00000001UL   // CKEN[0]  I2C Unit Clock Enable for both functional and bus clock
#define	GCKEN_GSSP1_CLK 		     0x00000002UL   // CKEN[1]  GSSP1 Unit Clock Enable for both functional and bus clock
#define	GCKEN_GSSP2_CLK	             0x00000004UL   // CKEN[2]  GSSP2 Unit Clock Enable for both functional and bus clock
#define	GCKEN_MSL_CLK		         0x00000010UL   // CKEN[4]  MSL Unit Clock Enable for both functional and bus clock
#define	GCKEN_USIM_CLK 		         0x00000020UL   // CKEN[5]  USIM Unit Clock Enable for both functional and bus clock
#define	GCKEN_DSSP_BUS_CLK           0x00000040UL   // CKEN[6]  Grayback peripheral bus clock for all DSSPs Enable

#define	GCKEN_SDIO_CLK               0x00000008UL   // CKEN[3]  SDIO Unit Clock Enable for both functional and bus clock (PV ONLY) not in use
#define	GCKEN_USIM2_CLK              0x00000200UL   // CKEN[9]  USIM2 Unit Clock Enable for both functional and bus clock (PV ONLY) not in use

#define GPBMCSR_UART_CLK			 0x00000002UL   // GPBMCSR[1] uart peripheral clock

// CCCR masks
#define	CCCR_MODEM_CLK_78MHZ         0x00000020UL   // Modem 78MHz clock enable
#define	CCCR_MODEM_CLK_104MHZ        0x00000040UL   // Modem 104MHz clock enable
#define	CCCR_MODEM_CLK_312MHZ        0x00000080UL   // Modem 312MHz clock enable (GB)
#define	CCCR_MODEM_CLK_208MHZ        0x00000010UL   // Modem 208MHz clock enable (GB)
#define CCCR_PLL_SELECT              0x00000001UL   // 0 for SYS, 1 for GB
#define CCCR_D1_UART_DEPENDENCY      0x00000100UL	//d1 uart dependency

// System PLL Frequency Select (SPFS) masks
#define SPFS_SYS_PLL_FREQ_MASK       0x0000000EUL
#define SPFS_SYS_PLL_FREQ_312MHZ     0x00000008UL
#define SPFS_SYS_PLL_FREQ_208MHZ     0x00000006UL
#define SPFS_SYS_PLL_FREQ_156MHZ     0x00000004UL
#define SPFS_SYS_PLL_FREQ_104MHZ     0x00000002UL
#define SPFS_SYS_PLL_FREQ_52MHZ      0x00000000UL

// CCSR bit masks
#define	CCSR_PLL_SEL_STATUS     	 0x00000001UL
#define	CCSR_SYS_PLL_FREQ_SEL_STATUS 0x0000000EUL
#define	CCSR_SYS_PLL_LOCKED 		 0x10000000UL
#define	CCSR_GB_PLL_LOCKED 		 	 0x20000000UL

#define GCCCR_F_BIT              	 0x00000001UL






#endif /* _GB_CCU_HW_H_ */
