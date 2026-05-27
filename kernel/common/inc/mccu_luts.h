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
/* Title: Modem Clock Control Unit LUTs Header File                     */
/*                                                                      */
/* Filename: mccu_luts.h                                                */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: This file contains the Look Up Tables used in the MCCU      */
/*          package, the relevant macros can be found in mccu_def.h     */
/*                                                                      */
/* Important: the look-up-tables in this file depend on the             */
/*            MCCU_ClocksE and the MCCU_ClockFreqE enumerators and the  */
/*            order of the items in them !                              */							        
/*                                                                      */
/* Created: 23/03/2005                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _MCCU_LUTS_H_
#define _MCCU_LUTS_H_

#include "global_types.h"    //  needed for the UINT32 definition.
#include "mccu_def.h"
#include "mccu.h"            //  needed for the enumerators.

/**************** Frequency selection definitions ***********************/
#define FREQ_SELECTIONS_NOT_SUPPORTED   0x00000000UL
#define FREQ_13MHZ_SUPPORTED            0x00000001UL //MCCU_FREQ_13MHZ
#define FREQ_26MHZ_SUPPORTED            0x00000002UL //MCCU_FREQ_26MHZ
#define FREQ_52MHZ_SUPPORTED            0x00000004UL //MCCU_FREQ_52MHZ
#define FREQ_78MHZ_SUPPORTED            0x00000008UL //MCCU_FREQ_78MHZ
#define FREQ_104MHZ_SUPPORTED           0x00000010UL //MCCU_FREQ_104MHZ
/************************************************************************/

typedef const struct 
{   // MCCU clock --> supported frequency selections.
    UINT32 supportedFreqSelections[MCCU_NUM_OF_CLKS];
    
    // MCCU clock --> relevant bits in MCCU register.
    UINT32 freqSelectionBits[MCCU_NUM_OF_CLKS]; 
    
    // MCCU clock and freq --> bits that should be turned on.
    UINT32 freqBits[MCCU_NUM_OF_CLKS][MCCU_NUM_OF_FREQS];

    // freq --> corresponding bit mask (used to enable or'ing).
    UINT32 supportedFreqToBitMask[MCCU_NUM_OF_FREQS];

    // MCCU clock --> functional clock bit mask.
    UINT32 clockToFuncBitMask[MCCU_NUM_OF_CLKS];

	// MCCU WB-GSM control (2 bits added to the reg)
	UINT32 WBContolBitsToBitMask[MCCU_WB_NUM_OF_BITS];
                    
} MCCU_LookUpTablesS;

static MCCU_LookUpTablesS _MCCU_LookUpTables = 
{
        {   //UINT32 supportedFreqSelections[MCCU_NUM_OF_CLKS]
            //MCCU_CLK_DSSP0
            FREQ_13MHZ_SUPPORTED | FREQ_26MHZ_SUPPORTED,
            //MCCU_CLK_DSSP1
            FREQ_13MHZ_SUPPORTED | FREQ_26MHZ_SUPPORTED,
            //MCCU_CLK_DSSP2
            FREQ_13MHZ_SUPPORTED | FREQ_26MHZ_SUPPORTED,
            //MCCU_CLK_DSSP3
            FREQ_13MHZ_SUPPORTED | FREQ_26MHZ_SUPPORTED,
            //MCCU_CLK_WB_CIPHER
            FREQ_SELECTIONS_NOT_SUPPORTED,
            //MCCU_CLK_SCK
            FREQ_104MHZ_SUPPORTED | FREQ_26MHZ_SUPPORTED,
            //MCCU_CLK_E_CIPHER
            FREQ_104MHZ_SUPPORTED |FREQ_78MHZ_SUPPORTED|FREQ_52MHZ_SUPPORTED,
            //MCCU_CLK_E_CIPHER_26MHZ
            FREQ_SELECTIONS_NOT_SUPPORTED,
            //MCCU_CLK_VITERBI
            FREQ_104MHZ_SUPPORTED |FREQ_78MHZ_SUPPORTED|FREQ_52MHZ_SUPPORTED,
            //MCCU_CLK_EQUALIZER
            FREQ_104MHZ_SUPPORTED|FREQ_78MHZ_SUPPORTED|FREQ_52MHZ_SUPPORTED,
            //MCCU_CLK_TTPCOM
            FREQ_SELECTIONS_NOT_SUPPORTED,
            //MCCU_CLK_HSL
            FREQ_SELECTIONS_NOT_SUPPORTED,
            //MCCU_CLK_TCU
            FREQ_SELECTIONS_NOT_SUPPORTED       
    },   
    {       //UINT32 freqSelectionBits[MCCU_NUM_OF_CLKS]
            //MCCU_CLK_DSSP0
            DSSP0_CLK_SEL_MASK,
            //MCCU_CLK_DSSP1                   
            DSSP1_CLK_SEL_MASK,
            //MCCU_CLK_DSSP2
            DSSP2_CLK_SEL_MASK,
            //MCCU_CLK_DSSP3
            DSSP3_CLK_SEL_MASK,
            //MCCU_CLK_WB_CIPHER
            CLK_SEL_NOT_SUPPORTED,
            //MCCU_CLK_SCK
            SCK_CLK_SEL_MASK,
            //MCCU_CLK_E_CIPHER
            CIPHER_CLK_SEL_MASK,
            //MCCU_CLK_E_CIPHER_26MHZ
            CLK_SEL_NOT_SUPPORTED,
            //MCCU_CLK_VITERBI
            VITERBI_CLK_SEL_MASK,
            //MCCU_CLK_EQUALIZER
            EQUALIZER_CLK_SEL_MASK,
            //MCCU_CLK_TTPCOM
            CLK_SEL_NOT_SUPPORTED,
            //MCCU_CLK_HSL
            CLK_SEL_NOT_SUPPORTED,
            //MCCU_CLK_TCU
            CLK_SEL_NOT_SUPPORTED
    },
    {       //UINT32 freqBits[MCCU_NUM_OF_CLKS][MCCU_NUM_OF_FREQS]
            //MCCU_CLK_DSSP0
            {
                DSSP0_CLK_SEL_13MHZ,
                DSSP0_CLK_SEL_26MHZ,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE
            },
            //MCCU_CLK_DSSP1                   
            {
                DSSP1_CLK_SEL_13MHZ, 
                DSSP1_CLK_SEL_26MHZ,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE
            },
            //MCCU_CLK_DSSP2
            {
                DSSP2_CLK_SEL_13MHZ,
                DSSP2_CLK_SEL_26MHZ,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE
            },
            //MCCU_CLK_DSSP3
            {
                DSSP3_CLK_SEL_13MHZ,
                DSSP3_CLK_SEL_26MHZ,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE
            },
            //MCCU_CLK_WB_CIPHER
            {CLK_SEL_NOT_SUPPORTED},
            //MCCU_CLK_SCK
            {
                FREQ_NOT_AVAILABLE,
                SCK_CLK_SEL_26MHZ,
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                SCK_CLK_SEL_104MHZ
            },
            //MCCU_CLK_E_CIPHER
            {
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                CIPHER_CLK_SEL_52MHZ,
                CIPHER_CLK_SEL_78MHZ,
                CIPHER_CLK_SEL_104MHZ
            },
            //MCCU_CLK_E_CIPHER_26MHZ
            {CLK_SEL_NOT_SUPPORTED},
            //MCCU_CLK_VITERBI
            {
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                VITERBI_CLK_SEL_52MHZ,
                VITERBI_CLK_SEL_78MHZ,
                VITERBI_CLK_SEL_104MHZ
            },
            //MCCU_CLK_EQUALIZER
            {
                FREQ_NOT_AVAILABLE,
                FREQ_NOT_AVAILABLE,
                EQUALIZER_CLK_SEL_52MHZ,
                EQUALIZER_CLK_SEL_78MHZ,
                EQUALIZER_CLK_SEL_104MHZ
            },
            //MCCU_CLK_TTPCOM
            {CLK_SEL_NOT_SUPPORTED},
            //MCCU_CLK_HSL
            {CLK_SEL_NOT_SUPPORTED},
            //MCCU_CLK_TCU
            {CLK_SEL_NOT_SUPPORTED} 
    },
    {       //UINT32 supportedFreqToBitMask[MCCU_NUM_OF_FREQS]
            //MCCU_FREQ_13MHZ
            FREQ_13MHZ_SUPPORTED,
            //MCCU_FREQ_26MHZ
            FREQ_26MHZ_SUPPORTED,
            //MCCU_FREQ_52MHZ
            FREQ_52MHZ_SUPPORTED,
            //MCCU_FREQ_78MHZ
            FREQ_78MHZ_SUPPORTED,            
            //MCCU_FREQ_104MHZ
            FREQ_104MHZ_SUPPORTED               
    },
    {       //UINT32 clockToFuncBitMask[MCCU_NUM_OF_CLKS]
            //MCCU_CLK_DSSP0
            DSSP0_FUNC_CLK_EN_MASK,
            //MCCU_CLK_DSSP1                   
            DSSP1_FUNC_CLK_EN_MASK,
            //MCCU_CLK_DSSP2
            DSSP2_FUNC_CLK_EN_MASK,
            //MCCU_CLK_DSSP3
            DSSP3_FUNC_CLK_EN_MASK,
            //MCCU_CLK_WB_CIPHER
            WB_CIPHER_CLK_EN_MASK,
            //MCCU_CLK_SCK
            SCK_FUNC_CLK_EN_MASK,
            //MCCU_CLK_E_CIPHER
            ECIPHER_FUNC_CLK_EN_MASK,
            //MCCU_CLK_E_CIPHER_26MHZ
            ECIPHER_26MHZ_CLK_EN_MASK,
            //MCCU_CLK_VITERBI
            VITERBI_FUNC_CLK_EN_MASK,
            //MCCU_CLK_EQUALIZER
            EQUALIZER_FUNC_CLK_EN_MASK,
            //MCCU_CLK_TTPCOM
            TTPCOM_FUNC_CLK_EN_MASK,
            //MCCU_CLK_HSL
            HSL_FUNC_CLK_EN_MASK,
            //MCCU_CLK_TCU
            TCU_FUNC_CLK_EN_MASK           
    },
	{       //UINT32 WBContolBitsToBitMask[MCCU_WB_NUM_OF_BITS];    rafiagiv 27.12
			//MCCU_WB_SLP_EQ_EN
			WB_SLP_EQ_EN_MASK,
			//MCCU_WB_WAKE_SEL
			WB_WAKE_SEL_MASK
	}
};


#endif /*_MCCU_LUTS_H_*/
