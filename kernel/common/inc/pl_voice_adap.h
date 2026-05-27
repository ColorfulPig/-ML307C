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

#if defined (INTEL_UPGRADE_UNIFIED_VOICE_TASK)
/**********************************************************************
*
* Filename: pl_voice_adap.h
*
* Programmers:  Yishai Gil (yg)
*
* Description:  definitions for adaptation for unified voice task
*
* --------------------------------------------------------------------
* Revision History
*
* Date         Who        Version      Description
* --------------------------------------------------------------------
* 26-Aug-2004  yg
**********************************************************************/
#ifndef PL_VOICE_ADAP_H
#define PL_VOICE_ADAP_H

/*----------- Extern definition ----------------------------------------------*/
#ifndef _VOICE_ADAP_NO_EXTERN_
  #define EXTERN extern
#else
  #define EXTERN
#endif /* _VOICE_ADAP_NO_EXTERN_ */


#define ENCODER_SP_IND			0x1


/*----------- Global variable declarations -----------------------------------*/
EXTERN void *_codedUlEvenSpeechBufPtr;
EXTERN void *_codedUlOddSpeechBufPtr;
EXTERN void *_codedUlSpeechBufPtr;
EXTERN void *_audioGsmSuspend;
EXTERN void *_audioGsmResume;
EXTERN void *_audioGsmTestStart;
EXTERN void *_audioGsmTestStop;

void * plgVoiceAdapGetUlCodedBufPtr(void);
//unsigned short plgVoiceAdapGetVocoderType(void);

void plgVoiceAdapGetVocoderType(unsigned short *vocoderType, unsigned short *vocoderRate);

unsigned short plgVoiceAdapGetAmrFrameStatus(void);

unsigned short plgVoiceAdapGetDtxSupport(void);
void plgVoiceAdapSetDtxSupport(unsigned short dtxInd);

unsigned short plgVoiceAdapGetEncoderFlags(void);
void plgVoiceAdapSetEncoderFlags(unsigned short encodeStatusInd);

unsigned short plgVoiceAdapGetDecoderSidInd(void);
void plgVoiceAdapSetAmrToc(unsigned short toc);
void plgVoiceAdapSetAmrSidTypeInd(unsigned short sidInd);
void plgVoiceAdapSetDecoderSidInd(unsigned short sidInd);

void plgVoiceAdapReSetAmrToc(void);
void plgVoiceAdapReSetAmrSidTypeInd(void);
typedef void (*audioGsmSuspend_t)(void);
typedef void (*audioGsmResume_t)(void);
typedef void (*audioGsmTestStart_t)(void);
typedef void (*audioGsmTestStop_t)(void);

#define audioGsmSuspend(a) (*(audioGsmSuspend_t)(_audioGsmSuspend))(a)
#define audioGsmResume(a)  (*(audioGsmResume_t)(_audioGsmResume))(a)
#define audioGsmTestStart(a) (*(audioGsmTestStart_t)(_audioGsmTestStart))(a)
#define audioGsmTestStop(a)  (*(audioGsmTestStop_t)(_audioGsmTestStop))(a)

void audioGsmStartTestDefaultCB(void);
void audioGsmStopTestDefaultCB(void);
void audioGsmResumeDefaultCB(void);
void audioGsmSuspendDefaultCB(void);
void audioBindGsmSuspend(audioGsmSuspend_t audioGsmSuspend);
void audioBindGsmResume(audioGsmResume_t audioGsmResume);
void audioBindGsmStartTest(audioGsmTestStart_t audioGsmTestStartCB);
void audioBindGsmStopTest(audioGsmTestStop_t audioGsmTestStopCB);

#undef EXTERN

#endif /*PL_VOICE_ADAP_H*/
#endif /*INTEL_UPGRADE_UNIFIED_VOICE_TASK*/
