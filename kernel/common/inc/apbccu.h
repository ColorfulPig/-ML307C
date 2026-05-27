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
 *  Title: APB Clock Control Unit Header file
 *
 *  Filename: apbccu.h
 *
 *  Target, subsystem: Tavor Platform
 *
 *  Authors: Abraham Coifman
 *
 *  Description:  APB Clock Control Unit Header file.
 *
 *  Last Modified: <date>
 *
 *  Notes:
 ******************************************************************************/


/******************************************************************************
*				OVERVIEW
*******************************************************************************
    The APB Clock Control Unit (APBCCU), is a HW unit that is used to control
	the APB clock frequency.

******************************************************************************/

#ifndef _APBCCU_H_
#define _APBCCU_H_

#include "global_types.h"
#include "apbccu_hw.h"

/*----------- Global macro definitions ---------------------------------------*/

/*----------- Global definitions ---------------------------------------------*/
 // Comment this flag definition to avoid parameters checks:
#define _APBCCU_CHECK_PARAMETERS_FLAG_

/*----------- Global type definitions ----------------------------------------*/

// State of PGATE bit
typedef enum
{
    APBCCU_PGATE_DISABLE = 0,
    APBCCU_PGATE_ENABLE = 1

}APBCCU_StatusE;

// State of ARDY bit
typedef enum
{
    APBCCU_ARDY_DISABLE = 0,
    APBCCU_ARDY_ENABLE = 1

}APBCCU_ReadyE;


// States of clock division
typedef enum
{
    APBCCU_NO_DIV   = 0,
    APBCCU_DIV_BY_2 = 1,
    APBCCU_DIV_BY_4 = 2,
    APBCCU_DIV_BY_8 = 3

}APBCCU_ClkDivE;


// Return Codes:
typedef enum
{
    APBCCU_RC_OK = 10,
    #ifdef _APBCCU_CHECK_PARAMETERS_FLAG_
        APBCCU_RC_NULL_POINTER = 11,
    #endif
    APBCCU_RC_BAD_MATCH_VALUE = 12

}APBCCU_ReturnCodeE;


/*----------- Global function prototypes -------------------------------------*/

APBCCU_ReturnCodeE APBCCUStatusSet(APBCCU_StatusE status);
APBCCU_ReturnCodeE APBCCUStatusGet(APBCCU_StatusE *pStatus);
APBCCU_ReturnCodeE APBCCUReadySet(APBCCU_ReadyE ready);
APBCCU_ReturnCodeE APBCCUReadyGet(APBCCU_ReadyE *pReady);
APBCCU_ReturnCodeE APBCCUClkDivSet(APBCCU_ClkDivE divValue);
APBCCU_ReturnCodeE APBCCUClkDivGet(APBCCU_ClkDivE *pDivValue);
APBCCU_ReturnCodeE APBCCUPhase1Init(void);

#endif  /* _APBCCU_H_ */
