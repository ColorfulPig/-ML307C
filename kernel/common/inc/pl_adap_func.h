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

#ifndef PL_ADAP_FUNC_H
#define PL_ADAP_FUNC_H

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

#endif


