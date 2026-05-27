/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

//
// bsp_hisr.h
//

#ifndef _BSP_HISR_H_
#define _BSP_HISR_H_

#include "osa.h"
#if (!defined PLAT_USE_THREADX) && (!defined PLAT_USE_ALIOS)
#include	"nucleus.h"
#endif

#define HISR_STACK_DEFAULT_LEVEL    (0 << 4)
#define HISR_STACK_SIZE             (2*1024)

#define HISR_STACK_LEVEL_1          (1 << 4)
#define HISR_STACK_LEVEL_1_SIZE     (1024)

#define HISR_STACK_LEVEL_2          (2 << 4)
#define HISR_STACK_LEVEL_2_SIZE     (512)

#define HISR_PRIORITY_LEVELS        3
#define STACK_ALIGNMENT             8 /*Bytes*/

/*************************************************************
             CREATE  HISR
             ------------
The using of "Create" is very ambigutive:
- with macro OS_Create_HISR
- with procedure Os_Create_HISR
- with procedure Manitoba_Create_HISR
Go to implement Os_Create_HISR() instead of Manitoba_Create_HISR()
"Manitoba_Create_HISR" to be obsolete. Use it as macro only (may be also macroed in the global_types.h)
**/
typedef UINT8   		OS_STATUS ;
typedef		void*				OS_HISR;


/*************************************************************
             CREATE  HISR
             ------------
The using of "Create" is very ambigutive:
- with macro OS_Create_HISR
- with procedure Os_Create_HISR
- with procedure Manitoba_Create_HISR
Go to implement Os_Create_HISR() instead of Manitoba_Create_HISR()
"Manitoba_Create_HISR" to be obsolete. Use it as macro only (Refer bsp_hisr.h)
*

*/

void Os_Create_HISR(VOID** hisr, char* name, VOID (*hisr_entry)(VOID), unsigned char priority);
INT32 Os_Activate_HISR(VOID** hisr);
INT32 Os_Delete_HISR(VOID** hisr);

#define OS_Create_HISR           Os_Create_HISR /*call for NU_Create_HISR*/

#define Manitoba_Create_HISR     Os_Create_HISR /*call for NU_Create_HISR*/
#define Manitoba_Activate_HISR	 Os_Activate_HISR
#define OS_Activate_HISR 				Os_Activate_HISR
#define OS_Delete_HISR					Os_Delete_HISR

int OS_Current_Interrupt_Count(void);

#define HISR_PRIORITY_0 0 //highest priority
#define HISR_PRIORITY_1 1 //medium priority
#define HISR_PRIORITY_2 2 //lowest priority


#endif//_BSP_HISR_H_

