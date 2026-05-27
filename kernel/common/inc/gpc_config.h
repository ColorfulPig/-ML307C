/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/***************************************************************************************************
 * INTEL CONFIDENTIAL
 * Copyright 2006 Intel Corporation All Rights Reserved. 
 * The source code contained or described herein and all documents related to the source code
 * ("Material") are owned by Intel Corporation or its suppliers or licensors. Title to the 
 * Material remains with Intel Corporation or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Intel or its suppliers and
 * licensors. The Material is protected by worldwide copyright and trade secret laws and 
 * treaty provisions. No part of the Material may be used, copied, reproduced, modified, published,
 * uploaded, posted, transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.

 * No license under any patent, copyright, trade secret or other intellectual property right is
 * granted to or conferred upon you by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license under such intellectual property 
 * rights must be express and approved by Intel in writing.

 * Unless otherwise agreed by Intel in writing, you may not remove or alter this notice or any
 * other notice embedded in Materials by Intel or Intel's suppliers or licensors in any way. 

 *  FILENAME:       gpc_config.h

 *  PURPOSE:        Configuration parameters for GPC 
 **************************************************************************************************/

#if !defined(_GPC_CONFIG_H_)
#define _GPC_CONFIG_H_

#include "msldl_config.h"

/* ---------------------------------------------------------------------------
Parameter     : GPC_TASK_PRIORITY
Description   : The GPC task priority.
Notes         : 
--------------------------------------------------------------------------- */
/* GPC_TASK_PRIORITY engineering:
 * The GPC channels are the transport for next SSE: Diag-over-MSL, AUDIO...
 * GPC transport should have same or higer priority than SSE.
 * or receiving from MSL.
 * Lowest-prio=255
 *  SSE  SSE  SSE  SSE
 *    Max from SSE
 *        GPC
 *        MSL=8
 * For packet transmission into MSL: GPC_TASK_PRIORITY >= MSL_DL_TASK_PRIORITY
 * For packet receiving    from MSL: GPC_TASK_PRIORITY <= MSL_DL_TASK_PRIORITY
*/
#define GPC_TASK_PRIORITY   MSL_DL_TASK_PRIORITY

/* Include the global configuration file, so these values can be overridden */
#if defined(_GBL_CONFIG_H_)
#undef _GBL_CONFIG_H_
#endif
#include "gbl_config.h"

#endif /* _GPC_CONFIG_H_ */
/*                      end of gpc_config.h
--------------------------------------------------------------------------- */





