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

#ifndef PL_DM_ADAP_H
#define PL_DM_ADAP_H

#include "global_types.h"
#include "pl_g_dm_bind.h"

extern pldmInitialise_t                     pldmInitialise_ptr;
extern pldmAdcEvents_t                      pldmAdcEvents_ptr;
extern pldmSendAdcIntReadCnf_t              pldmSendAdcIntReadCnf_ptr;
extern pldmAdcSeqRunning_t                  pldmAdcSeqRunning_ptr;
extern pldmAudioInit_t                      pldmAudioInit_ptr;
extern pldmAuStartEncoder_t                 pldmAuStartEncoder_ptr;
extern pldmAuSuspendEncoderAndVoice_t       pldmAuSuspendEncoderAndVoice_ptr;
extern pldmAuStopEncoderAndVoice_t          pldmAuStopEncoderAndVoice_ptr;
extern pldmAuSuspendEncoder_t       		pldmAuSuspendEncoder_ptr;
extern pldmAuStopEncoder_t          		pldmAuStopEncoder_ptr;
extern pldmAuEchoSuppressionSetting_t       pldmAuEchoSuppressionSetting_ptr;
extern pldmAuEchoCancelSetting_t            pldmAuEchoCancelSetting_ptr;
extern pldmAuNoiseSuppressionSetting_t      pldmAuNoiseSuppressionSetting_ptr;
extern pldmNvramInitialise_t                pldmNvramInitialise_ptr;
extern pldmNvramSelectCal_t                 pldmNvramSelectCal_ptr;
extern pldmFrCanSleep_t                     pldmFrCanSleep_ptr;
extern pldmIsSpeechChannelActive_t          pldmIsSpeechChannelActive_ptr;
extern pldmAuCfForceCodec_t                 pldmAuCfForceCodec_ptr;
extern pldmFrDedEnableDtx_t                 pldmFrDedEnableDtx_ptr;
extern pldmFrDedDisableDtx_t                pldmFrDedDisableDtx_ptr;
extern pldmInitAuTds_t                      pldmInitAuTds_ptr;
extern pldmFrHandler_t                      pldmFrHandler_ptr;
extern pldmWdtKick_t                        pldmWdtKick_ptr;
extern pldmMicInitialise_t                  pldmMicInitialise_ptr;
extern pldmInterruptInit_t                  pldmInterruptInit_ptr;
extern pldmAuChnUlConfigure_t               pldmAuChnUlConfigure_ptr;
extern pldmAuChnDlConfigure_t               pldmAuChnDlConfigure_ptr;
extern pldmBgDaiMode_t                      pldmBgDaiMode_ptr;
extern pldmBgDaiInOperation_t               pldmBgDaiInOperation_ptr;
extern pldmNumAdcSeqRunning_t              pldmNumAdcSeqRunning_ptr;

extern pldmAuPlayBufferGet_t           pldmAuPlayBufferGet_ptr ;
extern pldmAuPlayBufferFree_t          pldmAuPlayBufferFree_ptr ;
extern pldmAuRecordBufferGet_t           pldmAuRecordBufferGet_ptr ;
extern pldmAuRecordBufferFree_t          pldmAuRecordBufferFree_ptr ;
extern pldmAudioDspIfTest_t            pldmAudioDspIfTest_ptr ;
extern pldmAudioDtxEnabled_t          pldmAudioDtxEnabled_ptr  ;
extern pldmAuRecordStopSdvr_t        pldmAuRecordStopSdvr_ptr  ;

/********************************************************************************/
void            pldmAdapAdcIntEvents   (void);
void            pldmAdapSendAdcIntReadCnf   (unsigned char physicalAdc,
                                const unsigned short* rawValue,
                                unsigned char success,
                                unsigned char tableEntry);
unsigned char   pldmAdcSeqRunning (void);
void            pldmAdapAudioInit (void);
void            pldmAdapAuStartEncoder (void);
void            pldmAdapAuSuspendEncoder ( void );
void            pldmAdapAuStopEncoder ( void );
unsigned short           pldmAdapAuEchoSuppressionSetting ( void );
unsigned short           pldmAdapAuEchoCancelSetting ( void );
unsigned short          pldmAdapAuNoiseSuppressionSetting (void);
void            pldmAdapNvramInitialise (void);
void*           pldmAdapNvramSelectCal (unsigned char calDataId, void *defaultData, unsigned short blockLength);
unsigned char   pldmAdapFrCanSleep (void);
unsigned char   pldmAdapIsSpeechChannelActive (void);
unsigned short  pldmAdapAuCfForceCodec (void);
void            pldmAdapFrDedEnableDtx (void);
void            pldmAdapFrDedDisableDtx (void);
void            pldmAdapInitAuTds (void);
void            pldmAdapAlFrHandler ( void );
void            pldmAdapWdtKick ( void );
void            pldmAdapMicInitialise( void );
void            pldmAdapInterruptInit( void );
void*           pldmAdapAuChnUlConfigure( unsigned char );
void*           pldmAdapAuChnDlConfigure( unsigned char );
void            pldmAdapBgDaiMode( unsigned char );
unsigned char   pldmAdapBgDaiInOperation( void );
unsigned short pldmAdapNumAdcSeqRunning(void);

unsigned char pldmAdapAuPlayBufferGet( unsigned char dataFormat,
        unsigned short** virtualAddress );
void            pldmAdapAuPlayBufferFree( unsigned char dataFormat,
        unsigned short *recordBuff);
unsigned char pldmAdapAuRecordBufferGet( unsigned char dataFormat,
        unsigned short **virtualAddress);
void            pldmAdapAuRecordBufferFree( unsigned char dataFormat,
        unsigned short *recordBuff);
void            pldmAdapAudioDspIfTest( void );
unsigned char   pldmAdapAudioDtxEnabled( void );
void            pldmAdapAuRecordStopSdvr( void );
void pldmAdapInitialise( void);
void pldmAdapAuSuspendEncoderAndVoice ( void );
void pldmAdapAuStopEncoderAndVoice ( void );


#endif


