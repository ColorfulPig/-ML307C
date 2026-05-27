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
/* Title: Modem Clock Control Unit definitions (macros) Header File     */
/*                                                                      */
/* Filename: mccu_def.h                                                 */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: 1. Note that this code controls all modem registers	        */
/*			eventhough it runs from Arbel !!!							*/
/*			The purpose is to provide with a TEMPORARY tool that will   */
/*			turn all the clocks on.                                     */
/*          2. Note that when we set a clock to a certain frequency     */
/*             it also turns it on !!                                   */
/*    													                */
/* Created: 15/11/2004                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _OLD_MCCU_DEF_H
#define _OLD_MCCU_DEF_H

#include "global_types.h" // for the bit fields macros
#include "mccu_hw.h"

//#define MCCU_Register (* (MCCU_HW_Register *) MCCU_BASE_ADDRESS)

// The following masks definitions are to be used in this file only.
#define OLD_WB_CIPHER_CLK_EN_MASK           0x1000000UL
#define OLD_SCK_CLK_SEL_MASK                0x0800000UL
#define VITERBI_CLK_SEL_MASK_BIT_1          0x0400000UL
#define VITERBI_CLK_SEL_MASK_BIT_0          0x0200000UL
#define EQUALIZER_CLK_SEL_MASK_BIT_1        0x0100000UL
#define EQUALIZER_CLK_SEL_MASK_BIT_0        0x0080000UL
#define CIPHER_CLK_SEL_MASK_BIT_1           0x0040000UL
#define CIPHER_CLK_SEL_MASK_BIT_0           0x0020000UL
#define OLD_DSSP0_CLK_SEL_MASK              0x0008000UL // Bit #16 is reserved.
#define OLD_DSSP1_CLK_SEL_MASK              0x0004000UL
#define OLD_DSSP2_CLK_SEL_MASK              0x0002000UL
#define OLD_DSSP3_CLK_SEL_MASK              0x0001000UL
#define OLD_SCK_FUNC_CLK_EN_MASK            0x0000800UL
#define OLD_ECIPHER_26MHZ_CLK_EN_MASK       0x0000400UL
#define OLD_ECIPHER_FUNC_CLK_EN_MASK        0x0000200UL //  This is the Cipher clock enable.
#define OLD_VITERBI_FUNC_CLK_EN_MASK        0x0000100UL
#define OLD_EQUALIZER_FUNC_CLK_EN_MASK      0x0000080UL
#define OLD_DSSP3_FUNC_CLK_EN_MASK          0x0000040UL
#define OLD_DSSP2_FUNC_CLK_EN_MASK          0x0000020UL
#define OLD_DSSP1_FUNC_CLK_EN_MASK          0x0000010UL
#define OLD_DSSP0_FUNC_CLK_EN_MASK          0x0000008UL
#define OLD_TTPCOM_FUNC_CLK_EN_MASK         0x0000004UL
#define OLD_HSL_FUNC_CLK_EN_MASK            0x0000002UL
#define OLD_TCU_FUNC_CLK_EN_MASK            0x0000001UL


/*
 * We willingly chose readability over small code here. (per bit macro)
 */

// CCCR related macros

#define SET_WB_CIPHER_CLK_ON		(TURN_BIT_ON( (pMCCU_Register->GMCGC),OLD_WB_CIPHER_CLK_EN_MASK))
#define SET_WB_CIPHER_CLK_OFF		(TURN_BIT_OFF((pMCCU_Register->GMCGC),OLD_WB_CIPHER_CLK_EN_MASK))

#define SET_SCK_CLK_104MHZ          SET_SCK_FUNC_CLK_OFF;                              \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_SCK_CLK_SEL_MASK));  \
                                    SET_SCK_FUNC_CLK_ON

#define SET_SCK_CLK_26MHZ			SET_SCK_FUNC_CLK_OFF;                              \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_SCK_CLK_SEL_MASK));   \
                                    SET_SCK_FUNC_CLK_ON

#define SET_VITERBI_CLK_104MHZ		SET_VITERBI_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  VITERBI_CLK_SEL_MASK_BIT_1));        \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   VITERBI_CLK_SEL_MASK_BIT_0));       \
                                    SET_VITERBI_FUNC_CLK_ON


#define SET_VITERBI_CLK_78MHZ		SET_VITERBI_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   VITERBI_CLK_SEL_MASK_BIT_1));       \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  VITERBI_CLK_SEL_MASK_BIT_0));        \
                                    SET_VITERBI_FUNC_CLK_ON

#define SET_VITERBI_CLK_52MHZ	    SET_VITERBI_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   VITERBI_CLK_SEL_MASK_BIT_1));       \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   VITERBI_CLK_SEL_MASK_BIT_0));       \
                                    SET_VITERBI_FUNC_CLK_ON

#define SET_EQUALIZER_CLK_104MHZ	SET_EQUALIZER_FUNC_CLK_OFF;                        \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  EQUALIZER_CLK_SEL_MASK_BIT_1));      \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_0));     \
                                    SET_EQUALIZER_FUNC_CLK_ON

#define SET_EQUALIZER_CLK_78MHZ		SET_EQUALIZER_FUNC_CLK_OFF;                        \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_1));     \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  EQUALIZER_CLK_SEL_MASK_BIT_0));      \
                                    SET_EQUALIZER_FUNC_CLK_ON

#define SET_EQUALIZER_CLK_52MHZ     SET_EQUALIZER_FUNC_CLK_OFF;                        \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   CIPHER_CLK_SEL_MASK_BIT_1));        \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   CIPHER_CLK_SEL_MASK_BIT_0));        \
                                    SET_EQUALIZER_FUNC_CLK_ON

#define SET_CIPHER_CLK_104MHZ		SET_ECIPHER_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_1));     \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_0));     \
                                    SET_ECIPHER_FUNC_CLK_ON

#define SET_CIPHER_CLK_78MHZ		SET_ECIPHER_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_1));     \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  EQUALIZER_CLK_SEL_MASK_BIT_0));      \
                                    SET_ECIPHER_FUNC_CLK_ON

#define SET_CIPHER_CLK_52MHZ		SET_ECIPHER_FUNC_CLK_OFF;                          \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   EQUALIZER_CLK_SEL_MASK_BIT_1));     \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  EQUALIZER_CLK_SEL_MASK_BIT_0));      \
                                    SET_ECIPHER_FUNC_CLK_ON

#define SET_DSSP0_CLK_26MHZ			SET_DSSP0_CLK_OFF;                                 \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_DSSP0_CLK_SEL_MASK));\
                                    SET_DSSP0_CLK_ON

#define SET_DSSP0_CLK_13MHZ			SET_DSSP0_CLK_OFF;                                 \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_DSSP0_CLK_SEL_MASK)); \
                                    SET_DSSP0_CLK_ON

#define SET_DSSP1_CLK_26MHZ			SET_DSSP1_CLK_OFF;                                 \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_DSSP1_CLK_SEL_MASK));\
                                    SET_DSSP1_CLK_ON

#define SET_DSSP1_CLK_13MHZ			SET_DSSP1_CLK_OFF;                                 \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_DSSP1_CLK_SEL_MASK)); \
                                    SET_DSSP1_CLK_ON

#define SET_DSSP2_CLK_26MHZ			SET_DSSP2_CLK_OFF;                                 \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_DSSP2_CLK_SEL_MASK));\
                                    SET_DSSP2_CLK_ON

#define SET_DSSP2_CLK_13MHZ			SET_DSSP2_CLK_OFF;                                 \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_DSSP2_CLK_SEL_MASK));\
                                    SET_DSSP2_CLK_ON

#define SET_DSSP3_CLK_26MHZ			SET_DSSP3_CLK_OFF;                                 \
                                    (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_DSSP3_CLK_SEL_MASK));\
                                    SET_DSSP3_CLK_ON

#define SET_DSSP3_CLK_13MHZ			SET_DSSP3_CLK_OFF;                                 \
                                    (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_DSSP3_CLK_SEL_MASK)); \
                                    SET_DSSP3_CLK_ON

#define SET_SCK_FUNC_CLK_ON         (TURN_BIT_ON( (pMCCU_Register->GMCGC), OLD_SCK_FUNC_CLK_EN_MASK))
#define SET_SCK_FUNC_CLK_OFF        (TURN_BIT_OFF( (pMCCU_Register->GMCGC), OLD_SCK_FUNC_CLK_EN_MASK))

#define SET_ECIPHER_26MHZ_CLK_ON    (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_ECIPHER_26MHZ_CLK_EN_MASK))
#define SET_ECIPHER_26MHZ_CLK_OFF   (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_ECIPHER_26MHZ_CLK_EN_MASK))

#define SET_ECIPHER_FUNC_CLK_ON     (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_ECIPHER_FUNC_CLK_EN_MASK))
#define SET_ECIPHER_FUNC_CLK_OFF    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_ECIPHER_FUNC_CLK_EN_MASK))

#define SET_VITERBI_FUNC_CLK_ON     (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_VITERBI_FUNC_CLK_EN_MASK))
#define SET_VITERBI_FUNC_CLK_OFF    (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_VITERBI_FUNC_CLK_EN_MASK))

#define SET_EQUALIZER_FUNC_CLK_ON   (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_EQUALIZER_FUNC_CLK_EN_MASK))
#define SET_EQUALIZER_FUNC_CLK_OFF  (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_EQUALIZER_FUNC_CLK_EN_MASK))

#define SET_DSSP0_CLK_ON			(TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_DSSP0_FUNC_CLK_EN_MASK))
#define SET_DSSP0_CLK_OFF			(TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  OLD_DSSP0_FUNC_CLK_EN_MASK))

#define SET_DSSP1_CLK_ON			(TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_DSSP1_FUNC_CLK_EN_MASK))
#define SET_DSSP1_CLK_OFF			(TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  OLD_DSSP1_FUNC_CLK_EN_MASK))

#define SET_DSSP2_CLK_ON			(TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_DSSP2_FUNC_CLK_EN_MASK))
#define SET_DSSP2_CLK_OFF			(TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  OLD_DSSP2_FUNC_CLK_EN_MASK))

#define SET_DSSP3_CLK_ON			(TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_DSSP3_FUNC_CLK_EN_MASK))
#define SET_DSSP3_CLK_OFF			(TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  OLD_DSSP3_FUNC_CLK_EN_MASK))

#define SET_TTPCOM_FUNC_CLK_ON      (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_TTPCOM_FUNC_CLK_EN_MASK))
#define SET_TTPCOM_FUNC_CLK_OFF     (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                  OLD_TTPCOM_FUNC_CLK_EN_MASK))

#define SET_HSL_FUNC_CLK_ON         (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_HSL_FUNC_CLK_EN_MASK))
#define SET_HSL_FUNC_CLK_OFF        (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_HSL_FUNC_CLK_EN_MASK))

#define SET_TCU_FUNC_CLK_ON         (TURN_BIT_OFF( (pMCCU_Register->GMCGC),                      \
                                                   OLD_TCU_FUNC_CLK_EN_MASK))
#define SET_TCU_FUNC_CLK_OFF        (TURN_BIT_ON( (pMCCU_Register->GMCGC),                       \
                                                  OLD_TCU_FUNC_CLK_EN_MASK))

#endif /* _OLD_MCCU_DEF_H */
