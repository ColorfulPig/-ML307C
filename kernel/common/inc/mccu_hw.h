/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

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
/* Title: Modem Clock Control Unit HW Header File                       */
/*                                                                      */
/* Filename: mccu_hw.h                                                  */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: 											                */
/*                                                                      */
/* Created: 15/11/2004                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _MCCU_HW_H_
#define _MCCU_HW_H_

#include "global_types.h"    //  needed for the UINT32 definition.

#define MCCU_BASE_ADDRESS         0xF020400CUL    //  GPB Modem Clock Gator Control register address.


// Defines the Modem CCU HW-Structure 
	/** (See Tavor IAS for reserved bits and other remarks) **/
typedef volatile struct
{
	UINT32      GMCGC;	//GPB Modem Clock Gater            R/W
	UINT32		dummy;	//because there is a space between two registers address.
	UINT32      GMCGC2;	//GPB Modem Clock Gater 2          R/W - exists only in TAVOR PV
} MCCU_HW_Registers;

/************************************************************************/
/*                  Bit related definitions                             */
/************************************************************************/
//Clock enable bit masks.                                   /*Bit #*/
#define GSM_SPI_CLK_EN_MASK				0x2000000UL    //      25        //rafiagiv 27.2
#define WB_CIPHER_CLK_EN_MASK           0x1000000UL    //      24 
#define SCK_FUNC_CLK_EN_MASK            0x0000800UL    //      11 
#define ECIPHER_26MHZ_CLK_EN_MASK       0x0000400UL    //      10 
#define ECIPHER_FUNC_CLK_EN_MASK        0x0000200UL    //       9 
#define VITERBI_FUNC_CLK_EN_MASK        0x0000100UL    //       8
#define EQUALIZER_FUNC_CLK_EN_MASK      0x0000080UL    //       7
#define DSSP3_FUNC_CLK_EN_MASK          0x0000040UL    //       6
#define DSSP2_FUNC_CLK_EN_MASK          0x0000020UL    //       5
#define DSSP1_FUNC_CLK_EN_MASK          0x0000010UL    //       4
#define DSSP0_FUNC_CLK_EN_MASK          0x0000008UL    //       3
#define TTPCOM_FUNC_CLK_EN_MASK         0x0000004UL    //       2
#define HSL_FUNC_CLK_EN_MASK            0x0000002UL    //       1
#define TCU_FUNC_CLK_EN_MASK            0x0000001UL    //       0

//Frequency selection bits.                                     /*Bit #(s)      values*/
#define SCK_CLK_SEL_104MHZ               0x0000000UL    //      23          
#define SCK_CLK_SEL_26MHZ                0x0800000UL    //      23                tbd
#define VITERBI_CLK_SEL_104MHZ           0x0400000UL    //     22:21            
#define VITERBI_CLK_SEL_78MHZ            0x0200000UL    //     22:21            
#define VITERBI_CLK_SEL_52MHZ            0x0000000UL    //     22:21            
#define EQUALIZER_CLK_SEL_104MHZ         0x0100000UL    //     20:19            
#define EQUALIZER_CLK_SEL_78MHZ          0x0080000UL    //     20:19            
#define EQUALIZER_CLK_SEL_52MHZ          0x0000000UL    //     20:19            
#define CIPHER_CLK_SEL_104MHZ            0x0040000UL    //     18:17            
#define CIPHER_CLK_SEL_78MHZ             0x0020000UL    //     18:17            
#define CIPHER_CLK_SEL_52MHZ             0x0000000UL    //     18:17            
#define DSSP0_CLK_SEL_26MHZ              0x0000000UL    //      15          
#define DSSP0_CLK_SEL_13MHZ              0x0008000UL    //      15              
#define DSSP1_CLK_SEL_26MHZ              0x0000000UL    //      14                  
#define DSSP1_CLK_SEL_13MHZ              0x0004000UL    //      14                  
#define DSSP2_CLK_SEL_26MHZ              0x0000000UL    //      13              
#define DSSP2_CLK_SEL_13MHZ              0x0002000UL    //      13                  
#define DSSP3_CLK_SEL_26MHZ              0x0000000UL    //      12              
#define DSSP3_CLK_SEL_13MHZ              0x0001000UL    //      12 
#define FREQ_NOT_AVAILABLE               0x0000000UL                 


//Frequency selection bit masks.                                                           
#define SCK_CLK_SEL_MASK                SCK_CLK_SEL_104MHZ       | SCK_CLK_SEL_26MHZ    
#define VITERBI_CLK_SEL_MASK            VITERBI_CLK_SEL_104MHZ   | VITERBI_CLK_SEL_78MHZ   | VITERBI_CLK_SEL_52MHZ    
#define EQUALIZER_CLK_SEL_MASK          EQUALIZER_CLK_SEL_104MHZ | EQUALIZER_CLK_SEL_78MHZ | EQUALIZER_CLK_SEL_52MHZ    
#define CIPHER_CLK_SEL_MASK             CIPHER_CLK_SEL_104MHZ    | CIPHER_CLK_SEL_78MHZ    | CIPHER_CLK_SEL_52MHZ   
#define DSSP0_CLK_SEL_MASK              DSSP0_CLK_SEL_26MHZ      | DSSP0_CLK_SEL_13MHZ    
#define DSSP1_CLK_SEL_MASK              DSSP1_CLK_SEL_26MHZ      | DSSP1_CLK_SEL_13MHZ    
#define DSSP2_CLK_SEL_MASK              DSSP2_CLK_SEL_26MHZ      | DSSP2_CLK_SEL_13MHZ    
#define DSSP3_CLK_SEL_MASK              DSSP3_CLK_SEL_26MHZ      | DSSP3_CLK_SEL_13MHZ    
#define CLK_SEL_NOT_SUPPORTED           0x0000000UL

//WB selsection bit mask        rafiagiv  27.12
#define WB_SLP_EQ_EN_MASK               0x40000000UL    //      30 
#define WB_WAKE_SEL_MASK                0x80000000UL    //      31 
#define GMCGC2_DSSP_CLOCK_EN_SELECT	(0x00000100)
#define GMCGC2_ECIPHER_26MHZ_FUNC_CLOCK_EN	(0x00000002)
#define GMCGC2_ECIPHER_FUNC_CLOCK_EN	(0x00000001)
#endif /* _MCCU_HW_H_ */
