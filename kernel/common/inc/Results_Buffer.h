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

/****************************************************************************
*                         Results_Buffer.h
*
*   Created:        02/10/2003
*   Authoer:        Maya Shtern
*   Description:    Header file for the PMU test results buffer.
*                   This buffer is located in the external RAM.
*
*   Last Changed:   02/10/2003
*
****************************************************************************/

#ifndef _RESULTS_BUFFER_H_
#define _RESULTS_BUFFER_H_


/*----------------------------------------
 |          Include Files                |
 ----------------------------------------*/
#include <stdio.h>
#include <global_types.h>
#include "PMU_Monitoring.h"

/*----------------------------------------
 |              Structs                  |
 ----------------------------------------*/

//ICAT EXPORTED STRUCT
typedef struct
{
    unsigned long size;
	#if defined (PMUNIT_TEST)
    	SResultsData  resultsArray[8000];
	#else
		SResultsData  resultsArray[2];
	#endif
}SResultsArray;


/*----------------------------------------
 |       global Variables                |
 ----------------------------------------*/
extern SResultsArray resultsArrayStruct;



#endif  /* _RESULTS_BUFFER_H_ */

