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
/* Title: Grayback Clock Control Unit definitions (macros) Header File  */
/*                                                                      */
/* Filename: gb_ccu_def.h                                               */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: Note that this code controls all of Grayback's registers	*/
/*			eventhough it runs from Arbel !!!							*/
/*			The purpose is to provide with a TEMPORARY tool that will   */
/*			turn all the clocks on.										*/
/*    													                */
/* Created: 4/11/2004                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _GB_CCU_DEF_H
#define _GB_CCU_DEF_H

#include "global_types.h" // for the bit fields macros

#define GB_CCU_Registers (* (GB_CCU_HW_Registers *) GB_CCU_BASE_ADDRESS)

// The folowing masks definitions are to be used in this file only.

// CCCR bit masks
#define PS_MASK 			 0x00000001UL
#define	SPFS_312MHZ_ON_BITS	 0x00000008UL
#define	SPFS_312MHZ_OFF_BITS 0x00000006UL
#define	MCE0_MASK		 	 0x00000020UL
#define	MCE1_MASK			 0x00000040UL
#define	MCE2_MASK			 0x00000080UL
#define	TCUE_MASK			 0x00000100UL

// CCSR bit masks
#define	PS_S_MASK 			 0x00000001UL
#define	SPFS_S_MASK 		 0x0000000EUL // 100 — Grayback : 312 MHz, Arbel : 156MHz for 2:1
                                          //       clock ratio 208MHz for 3:2 clock ratio
#define	SPLCK_MASK 			 0x10000000UL
#define	GPLCK_MASK 		 	 0x20000000UL

// GCCCR bit masks
#define F_MASK				 0x00000001UL

// GCKEN bit masks
#define	I2C_CLK_MASK		 0x00000001UL	// CKEN[0]  I2C Unit Clock Enable for both functional and bus clock
#define	SSP_CLK_MASK 		 0x00000002UL	// CKEN[1]  SSP Unit Clock Enable for both functional and bus clock
#define	AUDIO_SSP_CLK_MASK	 0x00000004UL	// CKEN[2]  Audio SSP Unit Clock Enable for both functional and bus clock
// 3 was the GB USB
#define	MSL_CLK_MASK		 0x00000010UL	// CKEN[4]  MSL Unit Clock Enable for both functional and bus clock
#define	USIM_CLK_MASK 		 0x00000020UL	// CKEN[5]  USIM Unit Clock Enable for both functional and bus clock
#define	GPB_MODEM_0_CLK_MASK 0x00000040UL // CKEN[6]  Grayback peripheral bus clock for modem [0] Enable
#define	GPB_MODEM_1_CLK_MASK 0x00000080UL // CKEN[7]  Grayback peripheral bus clock for modem [1] Enable
#define	GPB_MODEM_2_CLK_MASK 0x00000100UL // CKEN[8]  Grayback peripheral bus clock for modem [2] Enable
#define	GPB_MODEM_3_CLK_MASK 0x00000200UL // CKEN[9]  Grayback peripheral bus clock for modem [3] Enable
#define	GPB_MODEM_4_CLK_MASK 0x00000400UL // CKEN[10] Grayback peripheral bus clock for modem [4] Enable
#define	GPB_MODEM_5_CLK_MASK 0x00000800UL // CKEN[11] Grayback peripheral bus clock for modem [5] Enable
#define	GPB_MODEM_6_CLK_MASK 0x00001000UL // CKEN[12] Grayback peripheral bus clock for modem [6] Enable
#define	GPB_MODEM_7_CLK_MASK 0x00002000UL // CKEN[13] Grayback peripheral bus clock for modem [7] Enable

/// Assert macro for fatal errors.
#define CCU_ASSERT           while (TRUE);

/*
* We willingly chose readability over small code here. (per bit macro)
*/

// CCCR related macros

#define SET_ARBEL_TCU_CLK_ON		(TURN_BIT_ON( pCommCCU_Registers->CCCR, TCUE_MASK))
#define SET_ARBEL_TCU_CLK_OFF		(TURN_BIT_OFF(pCommCCU_Registers->CCCR, TCUE_MASK))

#define SET_312MHZ_MODEM_CLK_ON		(TURN_BIT_ON( pCommCCU_Registers->CCCR, MCE2_MASK))
#define SET_312MHZ_MODEM_CLK_OFF	(TURN_BIT_OFF(pCommCCU_Registers->CCCR, MCE2_MASK))

#define SET_208MHZ_MODEM_CLK_ON		(TURN_BIT_ON( pCommCCU_Registers->CCCR, 0x00010UL))
#define SET_208MHZ_MODEM_CLK_OFF	(TURN_BIT_OFF(pCommCCU_Registers->CCCR, 0x00010UL))

#define SET_104MHZ_MODEM_CLK_ON		(TURN_BIT_ON( pCommCCU_Registers->CCCR, MCE1_MASK))
#define SET_104MHZ_MODEM_CLK_OFF	(TURN_BIT_OFF(pCommCCU_Registers->CCCR, MCE1_MASK))

#define SET_78MHZ_MODEM_CLK_ON		(TURN_BIT_ON( pCommCCU_Registers->CCCR, MCE0_MASK))
#define SET_78MHZ_MODEM_CLK_OFF		(TURN_BIT_OFF(pCommCCU_Registers->CCCR, MCE0_MASK))

// CCSR related macros

// read macros (ZBB - will be done at a later stage when the system needs will be cleared)

// GCCCR related macros

//redefinition //#define CLK_CHANGE_NOT_DONE			(pCommCCU_Registers->GCCCR & F_MASK)
//#define F_BIT_IS_ON                 (pCommCCU_Registers->GCCCR & F_MASK)
// Note that we check the F bit status just as a precaution.
//#define INITIATE_CORE_CLK_CHANGE	 if (!F_BIT_IS_ON) (TURN_BIT_ON( pCommCCU_Registers->GCCCR, F_MASK))


// GCKEN related macros

#define SET_I2C_CLK_ON				(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, I2C_CLK_MASK))
#define SET_I2C_CLK_OFF				(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, I2C_CLK_MASK))

#define SET_SSP_CLK_ON				(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, SSP_CLK_MASK))
#define SET_SSP_CLK_OFF				(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, SSP_CLK_MASK))

#define SET_AUDIO_SSP_CLK_ON 		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, AUDIO_SSP_CLK_MASK))
#define SET_AUDIO_SSP_CLK_OFF 		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, AUDIO_SSP_CLK_MASK))

#define SET_MSL_CLK_ON				(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, MSL_CLK_MASK))
#define SET_MSL_CLK_OFF				(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, MSL_CLK_MASK))

#define SET_USIM_CLK_ON				(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, USIM_CLK_MASK))
#define SET_USIM_CLK_OFF			(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, USIM_CLK_MASK))

#define SET_GPB_MODEM_0_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_0_CLK_MASK))
#define SET_GPB_MODEM_0_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_0_CLK_MASK))

#define SET_GPB_MODEM_1_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_1_CLK_MASK))
#define SET_GPB_MODEM_1_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_1_CLK_MASK))

#define SET_GPB_MODEM_2_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_2_CLK_MASK))
#define SET_GPB_MODEM_2_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_2_CLK_MASK))

#define SET_GPB_MODEM_3_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_3_CLK_MASK))
#define SET_GPB_MODEM_3_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_3_CLK_MASK))

#define SET_GPB_MODEM_4_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_4_CLK_MASK))
#define SET_GPB_MODEM_4_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_4_CLK_MASK))

#define SET_GPB_MODEM_5_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_5_CLK_MASK))
#define SET_GPB_MODEM_5_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_5_CLK_MASK))

#define SET_GPB_MODEM_6_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_6_CLK_MASK))
#define SET_GPB_MODEM_6_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_6_CLK_MASK))

#define SET_GPB_MODEM_7_CLK_ON		(TOGGLE_REG_SET_BITS_ON( pCommCCU_Registers->GCKEN, GPB_MODEM_7_CLK_MASK))
#define SET_GPB_MODEM_7_CLK_OFF		(TOGGLE_REG_SET_BITS_OFF( pCommCCU_Registers->GCKEN, GPB_MODEM_7_CLK_MASK))

#endif /* _GB_CCU_DEF_H */
