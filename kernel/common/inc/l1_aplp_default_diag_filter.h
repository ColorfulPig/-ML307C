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

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title: l1_default_diag_filter
*
* Filename: l1_aplp_default_diag_filter.h
*
*
* Description: holds all default APLP diag filter in powerup
*
* Last Updated:
*
* Notes:
*******************************************************************************/

#ifndef _L1_APLP_DEFAULT_DIAG_FILTER_H_
#define _L1_APLP_DEFAULT_DIAG_FILTER_H_


/* Please add you traces below accoroding to the relevant subject
 *  The interface is:
	DIAG_SET_FILTER(CAT1,CAT2,CAT3,FALSE);  -> by default after L1 power up this trace will be close
	DIAG_SET_FILTER(CAT1,CAT2,CAT3,TRUE);  -> by default after L1 power up this trace will be Open
 *
 * */

#if !defined (GPLC_ONLY)
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_WBCDMA, FALSE )
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_GSM, FALSE )
DIAG_SET_FILTER( APLP, CALIBRATION, ACTIVE_RAT_NULL, FALSE )
#endif

//DIAG_SET_FILTER( DRAT, DVFM, plDvfmMainLoop, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, commDvfmHandleMsTrigFromBase_data, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, commDvfmHandleMsTrigFromNotBase_data, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, commDvfmSetCurrentState, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, commDvfmMsToDvfmModeChangeReq, FALSE )
//DIAG_SET_FILTER( DRAT, DVFM, dvfmToMsModeChangeAck_called, FALSE )

//DIAG_SET_FILTER(APLP_HIGH,MSR_HIGH,PRINT_OUT_MSR_HIGH_DB, FALSE)
//DIAG_SET_FILTER(APLP,TM,processMsrSearchResult_StaticDb, FALSE)
//DIAG_SET_FILTER(APLP,TM,processMsrSearchResult, FALSE)
#ifndef  L1_WB_R99_ONLY
DIAG_SET_FILTER(APLP,MSR,plMSRRakeUpdatePathInfoEvent,FALSE)
#endif
//DIAG_SET_FILTER(APLP,TM,handleDpchRxWindow,FALSE)
//DIAG_SET_FILTER(APLP,TM,handleDpchRxWindowInfo,FALSE)
//DIAG_SET_FILTER(APLP,TM,handleDpchRxWindowRemovePath,FALSE)
//DIAG_SET_FILTER(APLP,TM,oldAnchorSelectionDebug0,FALSE)
//DIAG_SET_FILTER(APLP,TM,oldAnchorSelectionDebug3,FALSE)
//DIAG_SET_FILTER(APLP,TM,handleHsdpaRxWindowEveryWindowPrint1,FALSE)
//DIAG_SET_FILTER(APLP,TM,handleHsdpaRxWindowEveryWindowPrint2,FALSE)
//DIAG_SET_FILTER(APLP,MSR,plMSRHighBasicMeasPeriod,FALSE)
//DIAG_SET_FILTER (APLP, ATL, SFN_Value, FALSE)
//DIAG_SET_FILTER(APLP,MS,plwPhyHsPointerAssignReq_Called,FALSE)
#ifndef  REMOVE_GSM
DIAG_SET_FILTER(APLP, DATA, plDataHandleFrameInterrupt, FALSE)
//DIAG_SET_FILTER (APLP, TCC, plTccHsupaMaceInterruptHisr, FALSE)

DIAG_SET_FILTER(APLP, SCHD, schdStartSchdTimer, FALSE)

//DIAG_SET_FILTER(APLP, SCHD, schdChainsAddNode0, FALSE)

DIAG_SET_FILTER(APLP, SCHD, schdChainsAddIntraBchSetupNodeToChain0, FALSE)
DIAG_SET_FILTER(APLP, SCHD, schdChainsChainJudgement, FALSE)

DIAG_SET_FILTER ( APLP, MSR, MSR_EXT_API_INTER_FR_CELL_MEAS_REQ, FALSE )
DIAG_SET_FILTER ( APLP, MSR, MSR_EXT_API_INTER_FR_CELL_MEAS_REQ, FALSE )
DIAG_SET_FILTER ( APLP, MS, plMsAddTrChSetupMsg444, FALSE )
DIAG_SET_FILTER ( APLP, MS, plMsAddTrChSetupMsg, FALSE )
DIAG_SET_FILTER(APLP,MSR,plMsConcMgrPrintMsConcDb, FALSE)
DIAG_SET_FILTER(APLP,MSR,plMsConcMgrPrintMsConcDb_DpchSetup, FALSE)
DIAG_SET_FILTER(APLP,MSR,plMsConcMgrPrintMsConcDb_DpchModify, FALSE)
DIAG_SET_FILTER(APLP,MSR,plMsConcMgrPrintMsConcFailureDb, FALSE)
#endif

DIAG_SET_FILTER(HAL,IPC,PLP_DATA_RECEIVED,FALSE)
//DIAG_SET_FILTER(APLP,RF_NVM,plRFDSendTxApcIQLevelingParams,FALSE)
#ifndef  REMOVE_GSM
DIAG_SET_FILTER ( APLP, MSR, API_COMPRESSED_MODE_REQ, FALSE )
#endif
#if 1
//DIAG_SET_FILTER(APLP,TCC,plTccSetTrch_CMD, FALSE)
//DIAG_SET_FILTER(APLP,TCC,plTccSetUlGains_CMD, FALSE)
//DIAG_SET_FILTER (APLP,TCC,plTccSetDlTfcs_CMD, FALSE)
//DIAG_SET_FILTER(APLP,TCC,plTccSetUlTfcs_CMD, FALSE)
//DIAG_SET_FILTER (APLP, TCC, UL_TFCS_TABLE, FALSE)
//DIAG_SET_FILTER (APLP, TCC, DL_TFCS_TABLE, FALSE)
#ifndef  REMOVE_GSM

DIAG_SET_FILTER(APLP,TM,plTmPlpLoadActiveCellActionCmd, FALSE)
#endif

#endif
DIAG_SET_FILTER(HAL,IPC,PLP_DATA_RECEIVED,FALSE)

#endif  //_L1_APLP_DEFAULT_DIAG_FILTER_H_

