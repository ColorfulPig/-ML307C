/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006 Marvell International Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code (“Material? are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel’s prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title: l1_default_diag_filter
*
* Filename: l1_tdl1c_default_diag_filter.h
*
*
* Description: holds all default APLP diag filter in powerup
*
* Last Updated:
*
* Notes:
*******************************************************************************/

#ifndef _L1_TDL1C_DEFAULT_DIAG_FILTER_H_
#define _L1_TDL1C_DEFAULT_DIAG_FILTER_H_


/* Please add you traces below accoroding to the relevant subject
 *  The interface is:
	DIAG_SET_FILTER(CAT1,CAT2,CAT3,FALSE);  -> by default after L1 power up this trace will be close
	DIAG_SET_FILTER(CAT1,CAT2,CAT3,TRUE);  -> by default after L1 power up this trace will be Open
 *
 * */

#if 0//!defined (GPLC_ONLY)
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_WBCDMA, FALSE )
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_GSM, FALSE )
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_NULL, FALSE )
#endif

#if defined (UPGRADE_ARBEL_PLATFORM)
DIAG_SET_FILTER( DRAT, DVFM, plDvfmMainLoop, FALSE )
DIAG_SET_FILTER( DRAT, DVFM, commDvfmHandleMsTrigFromBase_data, FALSE )
DIAG_SET_FILTER( DRAT, DVFM, commDvfmHandleMsTrigFromNotBase_data, FALSE )
DIAG_SET_FILTER( DRAT, DVFM, commDvfmSetCurrentState, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, commDvfmMsToDvfmModeChangeReq, FALSE )
DIAG_SET_FILTER( DRAT, DVFM, dvfmToMsModeChangeAck_called, FALSE )
#endif // UPGRADE_ARBEL_PLATFORM

#endif  //_L1_TDL1C_DEFAULT_DIAG_FILTER_H_

