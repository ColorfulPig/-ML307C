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
 * Filename: pl_g_gsm_bind.h
 *
 * Programmers: Marcelo Senter  (ms)
 *
 *
 * Description: Declarations of the GSM bind functions
 *
 *
 *
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version      Description
 * --------------------------------------------------------------------
 * 15-Dec-2002  ms        I00.02.00    Initial version (L1 API v 0.01)
 **********************************************************************/
#ifndef PL_G_DM_BIND_H
#define PL_G_DM_BIND_H

#include "global_types.h"

/************************************************************************
 * define types for bind functions
 ************************************************************************/
/* DM Interface */
typedef void (*pldmInitialise_t) (void);
typedef void (*pldmAdcEvents_t) (void);
typedef void (*pldmSendAdcIntReadCnf_t) (unsigned char, const unsigned short*, unsigned char, unsigned char);
typedef unsigned char (*pldmAdcSeqRunning_t) (void );
typedef void (*pldmAudioInit_t) (void);
typedef void (*pldmAuStartEncoder_t) (void);
typedef void (*pldmAuSuspendEncoderAndVoice_t) (void);
typedef void (*pldmAuStopEncoderAndVoice_t) (void);
typedef void (*pldmAuSuspendEncoder_t) (void);
typedef void (*pldmAuStopEncoder_t) (void);
typedef unsigned short (*pldmAuEchoSuppressionSetting_t) (void);
typedef unsigned short (*pldmAuEchoCancelSetting_t) (void);
typedef unsigned short (*pldmAuNoiseSuppressionSetting_t) (void);
typedef void (*pldmNvramInitialise_t) (void);
typedef void* (*pldmNvramSelectCal_t) (unsigned char, void*, unsigned short);
typedef unsigned char (*pldmFrCanSleep_t) (void );
typedef unsigned char (*pldmIsSpeechChannelActive_t) (void );
typedef unsigned short (*pldmAuCfForceCodec_t) (void);
typedef void (*pldmFrDedEnableDtx_t) (void);
typedef void (*pldmFrDedDisableDtx_t) (void);
typedef void (*pldmInitAuTds_t) (void);
typedef void (*pldmFrHandler_t) (void);
typedef void (*pldmWdtKick_t) (void);
typedef void (*pldmMicInitialise_t) (void);
typedef void (*pldmInterruptInit_t) (void);
typedef void* (*pldmAuChnUlConfigure_t) (unsigned char);
typedef void* (*pldmAuChnDlConfigure_t) (unsigned char);
typedef void (*pldmBgDaiMode_t) (unsigned char);
typedef unsigned char (*pldmBgDaiInOperation_t) (void);
typedef unsigned short (*pldmNumAdcSeqRunning_t) (void);

typedef unsigned char (*pldmAuPlayBufferGet_t) (unsigned char, unsigned short**);
typedef void (*pldmAuPlayBufferFree_t) (unsigned char, unsigned short*);
typedef unsigned char (*pldmAuRecordBufferGet_t) (unsigned char, unsigned short**);
typedef void (*pldmAuRecordBufferFree_t) (unsigned char, unsigned short*);
typedef void (*pldmAudioDspIfTest_t) (void);
typedef unsigned char (*pldmAudioDtxEnabled_t) (void);
typedef void (*pldmAuRecordStopSdvr_t) (void);


/************************************************************************
 * define bind functions
 ************************************************************************/
/* MPH Interface */
void    pldmBindInitialise (pldmInitialise_t initialise);
void    pldmBindAdcEvents (pldmAdcEvents_t adcEvents);
void    pldmBindSendAdcIntReadCnf (pldmSendAdcIntReadCnf_t sendAdcIntReadCnf);
void    pldmBindAdcSeqRunning (pldmAdcSeqRunning_t adcSeqRunning);
void    pldmBindAudioInit (pldmAudioInit_t audioInit);
void    pldmBindAuStartEncoder (pldmAuStartEncoder_t auStartEncoder);
void    pldmBindAuSuspendEncoderAndVoice (pldmAuSuspendEncoderAndVoice_t auSuspendEncoderAndVoice);
void    pldmBindAuStopEncoderAndVoice (pldmAuStopEncoderAndVoice_t auStopEncoderAndVoice);
void    pldmBindAuSuspendEncoder (pldmAuSuspendEncoder_t auSuspendEncoder);
void    pldmBindAuStopEncoder (pldmAuStopEncoder_t auStopEncoder);
void    pldmBindAuEchoSuppressionSetting (pldmAuEchoSuppressionSetting_t auEchoSuppressionSetting);
void    pldmBindAuEchoCancelSetting (pldmAuEchoCancelSetting_t auEchoCancelSetting);
void    pldmBindAuNoiseSuppressionSetting (pldmAuNoiseSuppressionSetting_t auNoiseSuppressionSetting);
void    pldmBindNvramInitialise (pldmNvramInitialise_t mvramInitialise);
void    pldmBindFrCanSleep (pldmFrCanSleep_t frCanSleep);
void    pldmBindIsSpeechChannelActive (pldmIsSpeechChannelActive_t isSpeechChannelActive);
void    pldmBindAuCfForceCodec (pldmAuCfForceCodec_t auCfForceCodec);
void    pldmBindFrDedEnableDtx (pldmFrDedEnableDtx_t frDedEnableDtx);
void    pldmBindFrDedDisableDtx (pldmFrDedDisableDtx_t frDedDisableDtx);
void    pldmBindInitAuTds (pldmInitAuTds_t initAuTds);
void    pldmBindFrHandler (pldmFrHandler_t frHandler);
void    pldmBindWdtKick (pldmWdtKick_t wdtKick);
void    pldmBindMicInitialise (pldmMicInitialise_t micInitialise);
void    pldmBindInterruptInit (pldmInterruptInit_t interruptInit);
void    pldmBindAuChnUlConfigure (pldmAuChnUlConfigure_t auChnUlConfigure);
void    pldmBindAuChnDlConfigure (pldmAuChnDlConfigure_t auChnDlConfigure);
void    pldmBindBgDaiMode (pldmBgDaiMode_t bgDaiMode);
void    pldmBindBgDaiInOperation (pldmBgDaiInOperation_t bgDaiInOperation);
void    pldmBindNumAdcSeqRunning (pldmNumAdcSeqRunning_t numAdcSeqRunning);

void    pldmBindAuPlayBufferGet (pldmAuPlayBufferGet_t auPlayBufferGet);
void    pldmBindAuPlayBufferFree (pldmAuPlayBufferFree_t auPlayBufferFree);
void    pldmBindAuRecordBufferGet (pldmAuRecordBufferGet_t auRecordBufferGet);
void    pldmBindAuRecordBufferFree (pldmAuRecordBufferFree_t auRecordBufferFree);
void    pldmBindAudioDspIfTest (pldmAudioDspIfTest_t audioDspIfTest);
void    pldmBindAudioDtxEnabled (pldmAudioDtxEnabled_t audioDtxEnabled);
void    pldmBindAuRecordStopSdvr (pldmAuRecordStopSdvr_t auRecordStopSdvr);


#endif /* PL_G_DM_BIND_H */
