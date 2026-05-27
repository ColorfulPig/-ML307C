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

/**********************************************************************
*
* Filename: pl_adap_cb.h
*
* Programmers:  Marcelo Senter (ms)
*
* Description:  Callback pointers
*
* --------------------------------------------------------------------
* Revision History
*
* Date         Who        Version      Description
* --------------------------------------------------------------------
* 17-Dec-2002  ms
**********************************************************************/
#ifndef PL_ADAP_CB_H
#define PL_ADAP_CB_H

#include "pl_g_gsm_bind.h"
#include "pl_g_gprs_bind.h"
#include "pl_g_test_bind.h"

/* Callback functions pointers */
/* --------------------------- */
/* MPH Interface */
extern plgMphBcchMeasInd_t					plgMphBcchMeasInd_ptr;
extern plgMphBsicDecodeInd_t				plgMphBsicDecodeInd_ptr;
extern plgMphUnitDataInd_t					plgMphUnitDataInd_ptr;
extern plgMphDownlinkSignalFailInd_t		plgMphDownlinkSignalFailInd_ptr;
extern plgMphIdleNcellMeasInd_t				plgMphIdleNcellMeasInd_ptr;
extern plgMphIdleScellMeasInd_t				plgMphIdleScellMeasInd_ptr;
extern plgMphHandoverStartInd_t				plgMphHandoverStartInd_ptr;
extern plgMphRadioLinkTimeoutInd_t			plgMphRadioLinkTimeoutInd_ptr;
extern plgMphDedicatedMeasInd_t				plgMphDedicatedMeasInd_ptr;
extern plgMphEngModeDedSrvCellInfoInd_t 	plgMphEngModeDedSrvCellInfoInd_ptr;
extern plgMphErrorInd_t						plgMphErrorInd_ptr;
extern plgMphMonitorPlmnInd_t				plgMphMonitorPlmnInd_ptr;
extern plgMphBchConfigCnf_t					plgMphBchConfigCnf_ptr;
extern plgMphDsConfigCnf_t					plgMphDsConfigCnf_ptr;
extern plgMphRandomAccessCnf_t				plgMphRandomAccessCnf_ptr;
extern plgMphBsicDecodeCnf_t				plgMphBsicDecodeCnf_ptr;
extern plgMphImmAssignmentCnf_t				plgMphImmAssignmentCnf_ptr;
extern plgMphChanAssignmentCnf_t			plgMphChanAssignmentCnf_ptr;
extern plgMphChanAssignmentFailCnf_t		plgMphChanAssignmentFailCnf_ptr;
extern plgMphCipherModeCnf_t				plgMphCipherModeCnf_ptr;
extern plgMphFrequencyChangeCnf_t			plgMphFrequencyChangeCnf_ptr;
extern plgMphChannelModeCnf_t				plgMphChannelModeCnf_ptr;
extern plgMphHandoverCnf_t					plgMphHandoverCnf_ptr;
extern plgMphHandoverFailCnf_t				plgMphHandoverFailCnf_ptr;
extern plgMphExtMeasurementCnf_t			plgMphExtMeasurementCnf_ptr;
extern plgMphDeactivateCnf_t				plgMphDeactivateCnf_ptr;
extern plgMphClassmarkCnf_t					plgMphClassmarkCnf_ptr;
#if defined (GPLC_DUALSIM_SUPPORT)
extern plgMphDsResumeCnf_t					plgMphDsResumeCnf_ptr;
extern plgMphDsPagingFailureInd_t			plgMphDsPagingFailureInd_ptr;
#endif
extern L1GsmCapabilityReportCnf_t				pldL1GsmCapabilityReportCnf_ptr;
extern L1GsmCapabilityReportCnf_t				pldL1GsmCapabilityReportCnf_ptr;
extern plgGrrCbControlCnf_t					plgGrrCbControlCnf_ptr;
extern plgMphSetGsmCnf_t                    plgMphSetGsmCnf_ptr;



extern plgMphCellLockCnf_t					plgMphCellLockCnf_ptr;
extern plgMphMonitorPlmnCnf_t				plgMphMonitorPlmnCnf_ptr;
#if defined (INTEL_UPGRADE_DUAL_RAT)
/* UMPH Interface */
extern plgUmphHandoverToUtranCnf_t 			plgUmphHandoverToUtranCnf_ptr;
extern plgUmphHandoverToUtranFailCnf_t 		plgUmphHandoverToUtranFailCnf_ptr;
extern plgUmphHoldGsmCnf_t 					plgUmphHoldGsmCnf_ptr;
extern plgUmphRestoreGsmCnf_t 				plgUmphRestoreGsmCnf_ptr;

extern plgUtranBchDecodeInd_t 				plgUtranBchDecodeInd_ptr;
extern plgUtranCellMeasInd_t	            plgUtranCellMeasInd_ptr;
extern plgUtranDetectedCellMeasInd_t	    plgUtranDetectedCellMeasInd_ptr;
extern plgUtranBchDecodeErrInd_t 		   	plgUtranBchDecodeErrInd_ptr;
extern plgUtranRssiMeasInd_t 				plgUtranRssiMeasInd_ptr;
extern plgUtranRssiMeasInd_t				plgUtranRssiScanInd_ptr;



#endif /* INTEL_UPGRADE_DUAL_RAT */
#if defined (UPGRADE_MULTI_RAT)
/* EMPH Interface */
extern plgEmphHandoverToLteCnf_t		    plgEmphHandoverToLteCnf_ptr;
extern plgEmphHandoverToLteFailCnf_t	    plgEmphHandoverToLteFailCnf_ptr;
extern plgEmphHoldGsmCnf_t				    plgEmphHoldGsmCnf_ptr;
extern plgEmphRestoreGsmCnf_t			    plgEmphRestoreGsmCnf_ptr;

extern plgEmphLteSibDecodeStopCnf_t			plgEmphLteSibDecodeStopCnf_ptr;
extern plgEmphLteSibUnitDataInd_t	        plgEmphLteSibUnitDataInd_ptr;
extern plgEmphLteNcellMeasureInd_t   	    plgEmphLteNcellMeasureInd_ptr;
extern plgEmphDrxFindLteCellInd_t	        plgEmphDrxFindLteCellInd_ptr;
#if defined (GPLC_LTE_RSSI_SCAN)	
extern plgEmphRssiScanCnf_t	                plgEmphRssiScanCnf_ptr;
extern plgEmphRssiScanAbortCnf_t	        plgEmphRssiScanAbortCnf_ptr;
#endif //GPLC_LTE_RSSI_SCAN
#endif /* UPGRADE_MULTI_RAT */

#if defined (UPGRADE_MBCCH)
extern plgMphMultiBcchDecodeInd_t           plgMphMultiBcchDecodeInd_ptr;
#endif //UPGRADE_MBCCH


/* PH Interface */
extern plgPhConnectInd_t 					plgPhConnectInd_ptr;
extern plgPhReadyToSendInd_t                plgPhReadyToSendInd_ptr;
extern plgPhDataInd_t						plgPhDataInd_ptr;
/* RTL1 Interface */
extern plgRtl1DownlinkDataInd_t				plgRtl1DownlinkDataInd_ptr;
/* L1AL Interface */
extern plgL1AlStatusInd_t					plgL1AlStatusInd_ptr;
/* GMPH Interface */
extern plgGmphUnitDataInd_t 				plgGmphUnitDataInd_ptr;
extern plgGmphUlFxdAllocEndInd_t 			plgGmphUlFxdAllocEndInd_ptr;
extern plgGmphPtmEnterInd_t 				plgGmphPtmEnterInd_ptr;
extern plgGmphPtmExitInd_t 					plgGmphPtmExitInd_ptr;
extern plgGmphIntMeasInd_t 					plgGmphIntMeasInd_ptr;
extern plgGmphPtmMeasInd_t 					plgGmphPtmMeasInd_ptr;
extern plgGmphEngModeGprsEdgeLinkQualInd_t 	plgGmphEngModeGprsEdgeLinkQualInd_ptr;
extern plgGmphPccchConfigCnf_t 				plgGmphPccchConfigCnf_ptr;
extern plgGmphPrachCnf_t 					plgGmphPrachCnf_ptr;
extern plgGmphUlSbConfigCnf_t 				plgGmphUlSbConfigCnf_ptr;
extern plgGmphDlSbConfigCnf_t 				plgGmphDlSbConfigCnf_ptr;
extern plgGmphDlTbfConfigCnf_t 				plgGmphDlTbfConfigCnf_ptr;
extern plgGmphUlDynTbfConfigCnf_t 			plgGmphUlDynTbfConfigCnf_ptr;
extern plgGmphDynTsReconfigCnf_t			plgGmphDynTsReconfigCnf_ptr;
extern plgGmphFxdTsReconfigCnf_t			plgGmphFxdTsReconfigCnf_ptr;
extern plgGmphUlFxdTbfConfigCnf_t			plgGmphUlFxdTbfConfigCnf_ptr;
extern plgGmphPktOpCtrlInd_t                plgGmphPktOpCtrlInd_ptr;
extern plgGmphPdchReleaseCnf_t 				plgGmphPdchReleaseCnf_ptr;
extern plgGmphExtMeasCnf_t 					plgGmphExtMeasCnf_ptr;
/* GL1 Interface APIs */
extern plgGl1CipherDataCnf_t                plgGl1CipherDataCnf_ptr;
extern plgGl1CipherFlushCnf_t               plgGl1CipherFlushCnf_ptr;
/* MAC Interface */
extern plgMacTxPayloadReq_t					plgMacTxPayloadReq_ptr;
extern plgMacL1Sequencer_t					plgMacL1Sequencer_ptr;
extern plgMacRxRadioBlockInd_t				plgMacRxRadioBlockInd_ptr;
extern plgMacControlAckReq_t				plgMacControlAckReq_ptr;
extern plgMacDlConfigReq_t					plgMacDlConfigReq_ptr;
extern plgMacUlConfigReq_t					plgMacUlConfigReq_ptr;
extern plgMacUlDlDsConfigReq_t              plgMacUlDlDsConfigReq_ptr;
extern plgMacDlStartInd_t					plgMacDlStartInd_ptr;
extern plgMacRxGetLinkRxQualMeas_t          plgMacRxGetLinkRxQualMeas_ptr;
extern plgMacRxGetEgprsTsBepLinkQualMeas_t	plgMacRxGetEgprsTsBepLinkQualMeas_ptr;
extern plgMacUlStartInd_t 					plgMacUlStartInd_ptr;
extern plgMacUlFlushReq_t					plgMacUlFlushReq_ptr;
extern plgMacDlFlushReq_t					plgMacDlFlushReq_ptr;
extern plgMacUlSblockReq_t					plgMacUlSblockReq_ptr;
extern plgMacRxDecodeErrorInd_t				plgMacRxDecodeErrorInd_ptr;
extern plgMacRemoveUlSblockReq_t			plgMacRemoveUlSblockReq_ptr;
extern plgMacTxBlockInfoReq_t				plgMacTxBlockInfoReq_ptr;
extern plgMacTxDataBlocksPurgedInd_t		plgMacTxDataBlocksPurgedInd_ptr;
extern plgMacTxBlockSentInd_t				plgMacTxBlockSentInd_ptr;
extern plgMacUlTimeslotAllocInd_t			plgMacUlTimeslotAllocInd_ptr;
extern plgMacDlTimeslotAllocInd_t			plgMacDlTimeslotAllocInd_ptr;
#if defined (UPGRADE_EDGE)
extern plgGmphEprachCnf_t 				    plgGmphEprachCnf_ptr;
extern plgMacRxEgprsHeaderDecode_t			plgMacRxEgprsHeaderDecode_ptr;
extern plgMacRxEgprsDataBlockInd_t			plgMacRxEgprsDataBlockInd_ptr;
#endif
/* TI Interface */
extern plgTiDaiCnf_t			 			plgTiDaiCnf_ptr;
extern plgTiTchCnf_t			 			plgTiTchCnf_ptr;
/* CDMG Interface */
extern plgCalDevGsmCnf_t 					plgCalDevGsmCnf_ptr;
extern plgCalDevGsmFinishCnf_t 				plgCalDevGsmFinishCnf_ptr;
extern plgCalDevGsmRssiCnf_t 				plgCalDevGsmRssiCnf_ptr;
extern plgCalDevGsmFreqOffsetMeasCnf_t		plgCalDevGsmFreqOffsetMeasCnf_ptr;
extern plgCalDevGsmBurstCnf_t 				plgCalDevGsmBurstCnf_ptr;
extern plgCalDevGsmIqFetchCnf_t 			plgCalDevGsmIqFetchCnf_ptr;
extern plgCalDevGsmSetAfcDacCnf_t 			plgCalDevGsmSetAfcDacCnf_ptr;
extern plgCalDevGsmSetPowerRampCnf_t 		plgCalDevGsmSetPowerRampCnf_ptr;
extern plgCalDevGsmPowerRampAttributesCnf_t plgCalDevGsmPowerRampAttributesCnf_ptr;
extern plgCalDevGsmSetBurstDataCnf_t 		plgCalDevGsmSetBurstDataCnf_ptr;
extern plgCalDevGsmDcOffsetCnf_t 			plgCalDevGsmDcOffsetCnf_ptr;
extern plgCalDevGsmGainProgramCnf_t 		plgCalDevGsmGainProgramCnf_ptr;
extern plgCalDevGsmRxControlCnf_t 			plgCalDevGsmRxControlCnf_ptr;
extern plgCalDevGsmRampScaleCnf_t 			plgCalDevGsmRampScaleCnf_ptr;
extern plgCalDevGsmSetBandModeCnf_t 		plgCalDevGsmSetBandModeCnf_ptr;
extern plgCalDevGsmLoopBackCnf_t 			plgCalDevGsmLoopBackCnf_ptr;
extern plgCalDevGsmLoopBackDataCnf_t 		plgCalDevGsmLoopBackDataCnf_ptr;
extern plgCalDevGsmRxSelfCalibrationCnf_t 	plgCalDevGsmRxSelfCalibrationCnf_ptr;
extern plgCalDevGsmTxSelfCalibrationCnf_t 	plgCalDevGsmTxSelfCalibrationCnf_ptr;
extern plgCalDevGsmSlotDefineCnf_t 			plgCalDevGsmSlotDefineCnf_ptr;
extern plgCalDevGsmFrameDefineCnf_t 		plgCalDevGsmFrameDefineCnf_ptr;
extern plgCalDevGsmFrameTimingCnf_t 		plgCalDevGsmFrameTimingCnf_ptr;
extern plgCalDevGsmFrameUseCnf_t 			plgCalDevGsmFrameUseCnf_ptr;
extern plgCalDevGsmTxFastCalibrationSequenceCnf_t 			plgCalDevGsmTxFastCalibrationSequenceCnf_ptr;
extern plgCalDevGsmTxFastCalibrationStepInd_t				plgCalDevGsmTxFastCalibrationStepInd_ptr;
extern plgCalDevGsmRxFastCalibrationSequenceCnf_t 			plgCalDevGsmRxFastCalibrationSequenceCnf_ptr;
extern plgCalDevGsmRxFastCalibrationStepInd_t				plgCalDevGsmRxFastCalibrationStepInd_ptr;
#if defined(GSM_LMT_SUPPORT)    
extern plgCalDevGsmLmtSequenceCnf_t        plgCalDevGsmLmtSequenceCnf_ptr;
#endif

/* TT Interface */
extern plgDspTaskTestDspGetBufCnf_t 		plgDspTaskTestDspGetBufCnf_ptr;
extern plgDspTaskTestDspFreeBufCnf_t 		plgDspTaskTestDspFreeBufCnf_ptr;
extern plgDspTaskTestDspCopyBufCnf_t 		plgDspTaskTestDspCopyBufCnf_ptr;
extern plgDspTaskTestTdsAllocBufCnf_t 		plgDspTaskTestTdsAllocBufCnf_ptr;
extern plgDspTaskTestTdsFreeBufCnf_t 		plgDspTaskTestTdsFreeBufCnf_ptr;
extern plgDspTaskTestTdsCopyBufCnf_t 		plgDspTaskTestTdsCopyBufCnf_ptr;
extern plgDspTaskTestMcuAllocBufCnf_t 		plgDspTaskTestMcuAllocBufCnf_ptr;
extern plgDspTaskTestMcuFreeBufCnf_t 		plgDspTaskTestMcuFreeBufCnf_ptr;
extern plgDspTaskTestMcuCopyBufCnf_t 		plgDspTaskTestMcuCopyBufCnf_ptr;
extern plgDspTaskTestCfgCopyBufCnf_t 		plgDspTaskTestCfgCopyBufCnf_ptr;
extern plgDspTaskTestSimCopyBufCnf_t 		plgDspTaskTestSimCopyBufCnf_ptr;
extern plgDspTaskTestDspRunTaskCnf_t 		plgDspTaskTestDspRunTaskCnf_ptr;
extern plgDspTaskTestDspResetCnf_t 			plgDspTaskTestDspResetCnf_ptr;
extern plgDspTaskTestIniTstHarnessCnf_t 	plgDspTaskTestIniTstHarnessCnf_ptr;

#endif /* PL_ADAP_CB_H */
