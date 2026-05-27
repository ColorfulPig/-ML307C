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
/* Filename: bus_ccu_lut.h                                             	*/
/*                                                                      */
/* Author:   Idan Bartura                                               */
/*                                                                      */
/* Project, Target, subsystem: Tavor PV2, Seagull, HOP     				*/
/*																		*/
/* Remarks: This file contains the Look Up Tables used in the BUS CCU   */
/*          package, the relevant macros can be found in commccu_def.h  */
/*          - LUT stands for Look-Up-Table                              */
/*                                                                      */
/* Important: the LUT in this file depend on the BUS_CCU_ClocksE        */
/*            enumerator and the order of the items in them !           */
/*                                                                      */
/* Created: 11/08/2009                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

#ifndef _BUS_CCU_LUTS_H_
#define _BUS_CCU_LUTS_H_

#include "global_types.h"    //  needed for the UINT32 definition.
#include "bus_ccu_hw.h"
#include "bus_ccu.h"            //  needed for the enumerators.
#include "cpmu_hw.h"


//typedef struct
//{
//	UINT32 mask;
//	UINT32 offset;
//}
//BitMaskAndOffset ;


typedef const struct
{
    UINT32 clockToBitMask[BUS_CCU_NUM_OF_CLKS];   // BUS_CCU clock --> relevant enable/disable bit masks.
	UINT32 clockToRegister[BUS_CCU_NUM_OF_CLKS];  // BUS_CCU clock --> relevant register to enable/disable clock.
	UINT32 clockToAlwyasOn[BUS_CCU_NUM_OF_CLKS];  //BUS CCU clock --> is this clock alwyas onb and therfore can not be turned off.
	//BitMaskAndOffset pllFreqToBitMask[COMMCCU_SYS_PLL_NUM_OF_FREQS];
} Bus_CCU_LookUpTablesS;


static Bus_CCU_LookUpTablesS _Bus_CCU_LookUpTables =
{//UINT32 clockToBitMask[COMMCCU_NUM_OF_CLKS]
  {
      //BUS_CCU_TCU_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK,

      //BUS_CCU_AIRQ_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK,

      //BUS_CCU_UART_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

      //BUS_CCU_USIM1_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

      //BUS_CCU_USIM2_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

	  //BUS_CCU_I2C_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

	  //BUS_CCU_ECIPHER_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

      //BUS_CCU_TIMER_CLK
      APB_PERIPHERAL_BUS_CLOCK_EN_MASK,      //timer will turn off the bus clock only.

      //BUS_CCU_GSSP_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

      //BUS_CCU_COMM_IPC_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK,

	  //BUS_CCU_SCK_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK,

	  //BUS_CCU_MSL_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK | APB_PERIPHERAL_BUS_CLOCK_EN_MASK,

	  //BUS_CCU_RESOURCE_IPC_CLK
      APB_PERIPHERAL_FUNC_CLOCK_EN_MASK,

	  //BUS_CCU_AXI_CFG_CLK
      AXI_CFG_CLK_EN_MASK,

	  //BUS_CCU_ETB_CLK
      ETB_READ_CLK_EN_MASK,

	  //BUS_CCU_DTC_CLK
      DTC_CLK_EN_MASK,

	  //BUS_CCU_ADMA_CLK
      ADMA_CLK_EN_MASK
  },
  {
	//BUS_CCU_TCU_CLK
      APB_TCR_tcu,

      //BUS_CCU_AIRQ_CLK
      APB_AICR,

      //BUS_CCU_UART_CLK
      APB_UACR,

      //BUS_CCU_USIM1_CLK
      APB_USCR1,

      //BUS_CCU_USIM2_CLK
      APB_USCR2,

	  //BUS_CCU_I2C_CLK
      APB_ICR,

	  //BUS_CCU_ECIPHER_CLK
      APB_ECCR,

      //BUS_CCU_TIMER_CLK
      APB_TCR_timer,      //timer will turn off the bus clock only.

      //BUS_CCU_GSSP_CLK
      APB_GCR,

      //BUS_CCU_COMM_IPC_CLK
      APB_IPCR,

	  //BUS_CCU_SCK_CLK
      APB_SCCR,

	  //BUS_CCU_MSL_CLK
      APB_MCR,

	  //BUS_CCU_RESOURCE_IPC_CLK
      APB_RICR,

	  //BUS_CCU_AXI_CFG_CLK
      AXI_CLK_DYNC_DIS2,

	  //BUS_CCU_ETB_CLK
      AXI_CLK_DYNC_DIS2,

	  //BUS_CCU_DTC_CLK
      DTC_CTRL,

	  //BUS_CCU_ADMA_CLK
      ADMA_CTRL

  },
  {
	//BUS_CCU_TCU_CLK
      BUS_CCU_CLK_ALWAYS_ON,

      //BUS_CCU_AIRQ_CLK
      BUS_CCU_CLK_ALWAYS_ON,

      //BUS_CCU_UART_CLK
	  #if defined PRE_SILICON_UART_CLK_ALWAYS_ON
	  	BUS_CCU_CLK_ALWAYS_ON,
      #else
        BUS_CCU_CLK_CONTROLLED,
      #endif
      //BUS_CCU_USIM1_CLK
      BUS_CCU_CLK_CONTROLLED,

      //BUS_CCU_USIM2_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_I2C_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_ECIPHER_CLK
      BUS_CCU_CLK_CONTROLLED,

      //BUS_CCU_TIMER_CLK
      BUS_CCU_CLK_ALWAYS_ON,      //GB may use APB Timers so it is allways ON - GB will vote to turn off APB when not using timers

      //BUS_CCU_GSSP_CLK
      BUS_CCU_CLK_CONTROLLED,

      //BUS_CCU_COMM_IPC_CLK
      BUS_CCU_CLK_ALWAYS_ON,//BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_SCK_CLK
      BUS_CCU_CLK_ALWAYS_ON, 

	  //BUS_CCU_MSL_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_RESOURCE_IPC_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_AXI_CFG_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_ETB_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_DTC_CLK
      BUS_CCU_CLK_CONTROLLED,

	  //BUS_CCU_ADMA_CLK
      BUS_CCU_CLK_CONTROLLED
  }


};

#endif /*_COMCCU_LUTS_H_*/

