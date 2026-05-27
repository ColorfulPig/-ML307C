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
/* Title: Communication (SS) Clock Control Unit LUTs Header File        */
/*                                                                      */
/* Filename: commccu_luts.h                                             */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: This file contains the Look Up Tables used in the CommCCU   */
/*          package, the relevant macros can be found in commccu_def.h  */
/*          - LUT stands for Look-Up-Table                              */
/*                                                                      */
/* Important: the LUT in this file depend on the CommCCU_ClocksE        */
/*            enumerator and the order of the items in them !           */							        
/*                                                                      */
/* Created: 05/04/2005                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _COMCCU_LUTS_H_
#define _COMCCU_LUTS_H_

#include "global_types.h"    //  needed for the UINT32 definition.
#include "commccu_hw.h"
#include "commccu.h"            //  needed for the enumerators.
#include "cpmu_hw.h"


typedef struct
{
	UINT32 mask;
	UINT32 offset;
}
BitMaskAndOffset ;


typedef const struct
{   // CommCCU clock --> relevant enable/disable bit masks.
    UINT32 clockToBitMask[COMMCCU_NUM_OF_CLKS];
	BitMaskAndOffset pllFreqToBitMask[COMMCCU_SYS_PLL_NUM_OF_FREQS];
} CommCCU_LookUpTablesS;

static CommCCU_LookUpTablesS _CommCCU_LookUpTables = 
{//UINT32 clockToBitMask[COMMCCU_NUM_OF_CLKS]
  {  
      //CCCR_MODEM_CLK_312MHZ_GB - COMMCCU_MODEM_CLK_312MHZ_GB
      CCCR_MODEM_CLK_312MHZ,
      
      //CCCR_MODEM_CLK_208MHZ_GB - COMMCCU_MODEM_CLK_208MHZ_GB
      CCCR_MODEM_CLK_208MHZ,
      
      //CCCR_MODEM_CLK_104MHZ - COMMCCU_MODEM_CLK_104MHZ
      CCCR_MODEM_CLK_104MHZ,
      
      //CCCR_MODEM_CLK_78MHZ - COMMCCU_MODEM_CLK_78MHZ
      CCCR_MODEM_CLK_78MHZ,
      
      //GCKEN_I2C_CLK - COMMCCU_CLK_I2C
      GCKEN_I2C_CLK,
      
      //GCKEN_CLK_GSSP1, - COMMCCU_CLK_GSSP1
      GCKEN_GSSP1_CLK,
      
      //GCKEN_CLK_GSSP2	 - COMMCCU_CLK_GSSP2
      GCKEN_GSSP2_CLK,
      
      //GCKEN_CLK_MSL - COMMCCU_CLK_MSL
      GCKEN_MSL_CLK,
      
      //GCKEN_CLK_USIM - COMMCCU_CLK_USIM
      GCKEN_USIM_CLK,
      
      //GCKEN_CLK_DSSP_BUS - COMMCCU_CLK_DSSP_BUS
      GCKEN_DSSP_BUS_CLK,

	  //GPBMCSR_UART_CLK
	  GPBMCSR_UART_CLK
  },
   {
	{CPCR_FREQ_52MHZ_MASK,  CPCR_FREQ_52MHZ_OFFSET},           /* the reason this lut is in commccu and cpmu  */
															  /* is that core freq values are related to the */
	{CPCR_FREQ_104MHZ_MASK,  CPCR_FREQ_104MHZ_OFFSET},         /* commccu module							     */

	{CPCR_FREQ_156MHZ_MASK,  CPCR_FREQ_156MHZ_OFFSET},

	{CPCR_FREQ_208MHZ_MASK,  CPCR_FREQ_208MHZ_OFFSET},

	{CPCR_FREQ_312MHZ_MASK,   CPCR_FREQ_312MHZ_OFFSET}
  }
};
    
#endif /*_COMCCU_LUTS_H_*/

