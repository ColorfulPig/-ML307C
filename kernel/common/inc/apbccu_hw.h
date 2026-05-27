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
 *  Title: APB Clock Control Unit HW Header file
 *
 *  Filename: apbccu_hw.h
 *
 *  Target, subsystem: Tavor Platform
 *
 *  Authors: Abraham Coifman
 *
 *  Description:  APB Clock Control Unit HW Header file.
 *
 *  Last Modified: <date>
 *
 *  Notes:
 ******************************************************************************/


#ifndef _APBCCU_HW_H_
#define _APBCCU_HW_H_

#include "global_types.h"

#define ARBEL_HW_BASE_ADDRESS	0xD4060000L
#define APBC_HW_BASE_ADDRESS            (   ARBEL_HW_BASE_ADDRESS + 0x104                             )

/* ACCU Registers Definitions:      */
typedef volatile struct {
    UINT32      APBC;           // APB Control      Read/Write
} APBCCUController;

#define APBCCURegisters                 (   *( APBCCUController * )APBC_HW_BASE_ADDRESS               )

/* APBC register Bits definitions   */
#define APBC_PDIV_MASK          0x00000003
#define APBC_PGATE_MASK         0x00000004
#define APBC_ARDY_MASK          0x00000008

/* macro definitions:  				*/

// Set PGATE bit:
#define APBC_CLOCK_GATE_SET             (   TURN_BIT_ON(APBCCURegisters.APBC, APBC_PGATE_MASK)        )

// Reset PGATE bit:
#define APBC_CLOCK_GATE_RESET           (    TURN_BIT_OFF(APBCCURegisters.APBC, APBC_PGATE_MASK)      )

// Get PGATE bit (bit No. 3):
#define APBC_CLOCK_GATE_GET(m)          (   (m) = (APBCCU_StatusE)( (APBCCURegisters.APBC & APBC_PGATE_MASK) >> 2 )   )

// Set ARDY bit:
#define APBC_READY_CONTROL_SET          (   TURN_BIT_ON(APBCCURegisters.APBC, APBC_ARDY_MASK)         )

// Reset ARDY bit:
#define APBC_READY_CONTROL_RESET        (    TURN_BIT_OFF(APBCCURegisters.APBC, APBC_ARDY_MASK)       )

// Get ARDY bit ( bit No. 4):
#define APBC_READY_CONTROL_GET(m)       (   (m) = (APBCCU_ReadyE)( (APBCCURegisters.APBC & APBC_ARDY_MASK) >> 3 )    )

// Set Division value at PDIV field	(val = 0-3)
#define APBC_DIVISION_VALUE_SET(val)    (   APBCCURegisters.APBC = (APBCCURegisters.APBC & ~APBC_PDIV_MASK) | \
                                               ( val & 0x03 )  )

// Get division value from PDIV field
#define APBC_DIVISION_VALUE_GET(m)      (   (m) = (APBCCU_ClkDivE)(APBCCURegisters.APBC & APBC_PDIV_MASK))

#endif     //_APBCCU_HW_H_
