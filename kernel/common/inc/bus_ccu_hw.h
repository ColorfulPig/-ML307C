/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
 *              MODULE IMPLEMENTATION FILE
 *******************************************************************************
 *  COPYRIGHT (C) 2004 Intel Corporation.
 *
 *  This file and the software in it is furnished under
 *  license and may only be used or copied in accordance with the terms of the
 *  license. The information in this file is furnished for informational use
 *  only, is subject to change without notice, and should not be construed as
 *  a commitment by Intel Corporation. Intel Corporation assumes no
 *  responsibility or liability for any errors or inaccuracies that may appear
 *  in this document or any software that may be provided in association with
 *  this document.
 *  Except as permitted by such license, no part of this document may be
 *  reproduced, stored in a retrieval system, or transmitted in any form or by
 *  any means without the express written consent of Intel Corporation.
 *
 *  Title: BUS Clock Control Unit HW Header file
 *
 *  Filename: bus_ccu_hw.h
 *
 *  Target, subsystem: Tavor PV2 Platform
 *
 *  Authors: Idan Bartura
 *
 *  Description:  BUSES (APB & AXI) Clock Control Unit HW Header file.
 *
 *  Last Modified: <date>
 *
 *  Notes:
 ******************************************************************************/


#ifndef _BUS_CCU_HW_H_
#define _BUS_CCU_HW_H_

#include "global_types.h"

#define SEAGULL_APB_BASE_ADDRESS  0xD4044800
#define SEAGULL_APB_BASE_ADDRESS1 0xD4100000 

// Frequency Changes regsiter
#define FCAR 						(0xD4044008)
#define FCAR_F_BIT              	 0x00000001
#define FCAR_F_BIT_SHIFT             0x0

// this register is defined in order to support DVFM from Bus_CCU side 
#define APB_CCSR			(0xD4044004)
// note: this register is not APB_CPLLC this register determines whether or not the PLL is
#define COMM_CPLL				(0xD4044148)

#define APB_TCR_tcu	   		(SEAGULL_APB_BASE_ADDRESS +0x4)
#define APB_AICR	   		(SEAGULL_APB_BASE_ADDRESS +0x8)
#define APB_UACR	   		(SEAGULL_APB_BASE_ADDRESS +0xC)
#define APB_USCR1	   		(SEAGULL_APB_BASE_ADDRESS +0x10)
#define APB_USCR2	   		(SEAGULL_APB_BASE_ADDRESS +0x14)
#define APB_ICR		   		(SEAGULL_APB_BASE_ADDRESS +0x18)
#define APB_ECCR	   		(SEAGULL_APB_BASE_ADDRESS +0x1C)
#define APB_TCR_timer  		(SEAGULL_APB_BASE_ADDRESS +0x20)
#define APB_GCR		   		(SEAGULL_APB_BASE_ADDRESS +0x24)
#define APB_IPCR	   		(SEAGULL_APB_BASE_ADDRESS +0x28)
#define APB_SCCR	   		(SEAGULL_APB_BASE_ADDRESS +0x2C)
#define APB_MCR		   		(SEAGULL_APB_BASE_ADDRESS +0x30)
#define APB_RICR	   		(SEAGULL_APB_BASE_ADDRESS +0x70)


#define APB_CRATIO	   		(SEAGULL_APB_BASE_ADDRESS +0x34)
#define APB_CFREQ	   		(SEAGULL_APB_BASE_ADDRESS +0x38)
#define APB_CPLLC	   		(SEAGULL_APB_BASE_ADDRESS +0x40)

#define APB_SGV             (SEAGULL_APB_BASE_ADDRESS +0x44)

#define DTC_CTRL       		(SEAGULL_APB_BASE_ADDRESS +0x48)
#define ADMA_CTRL      	 	(SEAGULL_APB_BASE_ADDRESS +0x4C)

#define AXI_CLK_DYNC_DIS1 	(SEAGULL_APB_BASE_ADDRESS +0x54)
#define AXI_CLK_DYNC_DIS2 	(SEAGULL_APB_BASE_ADDRESS +0x58)
#define AXI_CLK_DYNC_DIS3 	(SEAGULL_APB_BASE_ADDRESS +0x5c)
#define AXI_CLK_DYNC_DIS4 	(SEAGULL_APB_BASE_ADDRESS +0x60)
#define AXI_CLK_DYNC_DIS5 	(SEAGULL_APB_BASE_ADDRESS +0x64)
#define AXI_CLK_DYNC_DIS6 	(SEAGULL_APB_BASE_ADDRESS +0x68)
#define AXI_CLK_DYNC_DIS7 	(SEAGULL_APB_BASE_ADDRESS +0x6c)
#define AXI_CLK_DYNC_DIS8 	(SEAGULL_APB_BASE_ADDRESS +0x78)
#define AXI_CLK_DYNC_DIS9 	(SEAGULL_APB_BASE_ADDRESS1 +0xA4)


/* These are the definition of the dynamic gaters bits */

// AXI_CLK_DYNC_DIS1 
#define AXI2TCM_AXICLK_DYNC_DIS 	 				0x00000001 // bit 0
#define AXI2TCM_CORE_BUS_DYNC_DIS  					0x00000002 // bit 1

// AXI_CLK_DYNC_DIS2 
#define ETB_READ_CLK_EN 							0x00000001 // bit 0
#define AXI_SWITCH_CFG_CLK_EN	 		 	  	    0x00000002 // bit 1

// AXI_CLK_DYNC_DIS3,4,5,6,7 generic definitions - same form 
#define AXI_CLK_DYNC_GATER_DISABLE_BIT_MASK 		0x00000003 // bits [1:0]
#define AXI_CLK_DYNC_GATER_DELAY_MASK 				0x000000FC // bits [7:2]
#define AXI_CLK_DYNC_GATER_COMM_TO_DMEMC	 		0x000000FC // bits [7:2]
#define AXI_CLK_DYNC_GATER_AXI_DECODER				0x000000FC // bits [7:2]
#define AXI_CLK_DYNC_GB_AXIS			  			0x000000FC // bits [7:2]
#define AXI_CLK_DYNC_GATER_TCM_INTERFACE 	  		0x000000FC // bits [7:2]
#define AXI_CLK_DYNC_GATER_SQU  					0x000000FC // bits [7:2]

// AXI_CLK_DYNC_DIS8 
#define AXI_SWITCH_CLOCK_DISABLE 				   	0x00000001 // bit 0
#define AXI_SEAGULL_CORE_IF_TO_AXI_SWITCH_DISABLE  	0x00000002 // bit 1
#define AXI_GRAYBACK_MASTER_AXI_CLOCK_DISABLE	  	0x00000004 // bit 2

// AXI_CLK_DYNC_DIS9  //
#define AXI_SWITCH_RSVD				   				0x00000001 // bit 0
#define AXI_SWITCH_CLOCK_DMA_BRIDGE  				0x00000002 // bit 1
#define APB_CONTROL_CLOCK_DMA_BRIDGE	  			0x00000004 // bit 2

/* generic bit definitions. these bits are true for most APB periperals. if these bits are not true for a certain periphral, they will be
 * defined as specific bits for that peripheral*/
#define APB_PERIPHERAL_BUS_CLOCK_EN_MASK   	 0x1
#define APB_PERIPHERAL_FUNC_CLOCK_EN_MASK    0x2
#define APB_PERIPHERAL_RESET_MASK    		 0x4
#define APB_PERIPHERAL_RESET_BUS_MASK   	 0x8

/* specific bit definitions. these bits are specific for a certain peripheral */
#define I2C_FUNC_CLK_SELECT_MASK			0x10
#define ECIPHER_FUNC_CLK_SELECT_MASK		0x30
#define RESET_FUNC_WDT_MASK   				0x10
#define RESET_BUS_WDT_MASK    				0x20

#define ETB_READ_CLK_EN_MASK    			0x1
#define AXI_CFG_CLK_EN_MASK    				0x2
#define DTC_CLK_EN_MASK    					0x2
#define ADMA_CLK_EN_MASK    		   		0x2


#define SEAGULL_AXI_VOTE_MASK               0x1
#define SEAGULL_APB_VOTE_MASK               0x4
#define SEAGULL_GPB_VOTE_MASK               0x8

#define APB_CONFIG_FREQUENCIES				0x00000001
#define AIB_CONFIG_FREQUENCIES				0x00000000
#define APB_CONFIG_FREQUENCIES_MASK			0x00000001

// CREQ definitions
// same configuration model for both seagull and grayback
#define APB_GB_SG_PLL_DIV_4					0x0
#define APB_GB_SG_PLL_DIV_3					0x1
#define APB_GB_SG_PLL_DIV_2					0x2
#define APB_GB_SG_PLL_DIV_1					0x3

#define APB_GB_PLL_DIV_MASK					0x6
#define APB_GB_PLL_DIV_SHIFT				0x1
#define APB_SG_PLL_DIV_MASK					0x30
#define APB_SG_PLL_DIV_SHIFT				0x4

// MG2 definitions
// same configuration model for both seagull and grayback
#define APB_GB_SG_USE_SYS_PLL_DIV_2			0x0
#define APB_GB_SG_USE_COMM_PLL_MG			0x1 

#define APB_GB_PLL_SELECT_MASK				0x8
#define APB_GB_PLL_SELECT_SHIFT				0x3
#define APB_SG_PLL_SELECT_MASK				0x40
#define APB_SG_PLL_SELECT_SHIFT				0x6

// Nevo definitions
// same configuration model for both seagull and grayback
#define APB_GB_SG_USE_SYS_PLL_416_BRANCH	0x1
#define APB_GB_SG_USE_COMM_PLL_NEVO			0x3  
#define APB_GB_SG_GET_LSB(PLLsource)		(PLLsource & 0x1)
#define APB_GB_SG_GET_MSB(PLLsource)		(PLLsource & 0x2)
#define APB_GB_PLL_SELECT_LSB_MASK			0x8
#define APB_GB_PLL_SELECT_MSB_MASK			0x80
#define APB_GB_PLL_SELECT_LSB_SHIFT			0x3
#define APB_GB_PLL_SELECT_MSB_SHIFT			0x6
#define APB_SG_PLL_SELECT_LSB_MASK			0x40
#define APB_SG_PLL_SELECT_MSB_MASK			0x100 
#define APB_SG_PLL_SELECT_LSB_SHIFT			0x6
#define APB_SG_PLL_SELECT_MSB_SHIFT			0x7   


// CPLL definitions

#define COMM_CPLL_COMM_PLL_ON				0x1
#define COMM_CPLL_COMM_PLL_OFF				0x0
#define COMM_CPLL_COMM_PLL_ON_OFF_MASK		0x1
#define COMM_CPLL_COMM_PLL_ON_SHIFT			0

/* macro definitions:  				*/

#define GET_SEAGULL_VOTE_BUS_SHUTDOWN(mask)	   			         (Reg & mask)       		  \



#endif     //_BUS_CCU_HW_H_

