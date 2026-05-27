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

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Audio Component Manager (ACM)
*
* Filename: acmDspIf.h
*
* Authors: Yishai Gil
*
* Description: ACM DSP I/F functions
*
* Last Updated:
*
* Notes:
******************************************************************************/
#ifndef _ACM_DSP_IF_H_
    #define _ACM_DSP_IF_H_

/*----------- Local include files --------------------------------------------*/
#include "acmTypes.h"
#include "PCA_api.h"
#include "acm_comm.h"
/*----------- Global defines -------------------------------------------------*/
/*----------- Global macro definitions ---------------------------------------*/
/*----------- Global type definitions ----------------------------------------*/

//source\destination for streams
typedef enum
{
	BOTH,  /* Both near and far ends */
    NEAR_END,
    FAR_END,


  	NO_END,
	NUM_OF_ENDS	= NO_END,

	AUDIO_SRC_DST_ENUM_32_BIT		    	    = 0x7FFFFFFF //32bit enum compiling enforcement
} AudioSrcDst;

//override\add with voice\video call for streams
typedef enum
{
	OVERRIDE_CALL,
	ADD_TO_CALL,

	NO_COMB_WITH_CALL_MODE,
	NUM_OF_COMB_WITH_CALL_MODES	= NO_COMB_WITH_CALL_MODE,

	AUDIO_COMB_WITH_CALL_ENUM_32_BIT		    	    = 0x7FFFFFFF //32bit enum compiling enforcement
} AudioCombWithCallMode;


//DSP modes
typedef enum
{
	  ACM_DSP_IF_MODE_OFF = 0
	, ACM_DSP_IF_MODE_COMB_VMP_PLAY
	, ACM_DSP_IF_MODE_GSM_CALL
	, ACM_DSP_IF_MODE_SIVR_RECORD
	, ACM_DSP_IF_MODE_SDVR_RECORD
    , ACM_DSP_IF_MODE_SDVR_ACQ_ABORT
	, ACM_DSP_IF_MODE_TONE_GEN
	, ACM_DSP_IF_MODE_VMP_PLAY
	, ACM_DSP_IF_MODE_VMP_RECORD
    , ACM_DSP_IF_MODE_LOOP
    , ACM_DSP_IF_MODE_DAI_ACOUSTIC
    , ACM_DSP_IF_MODE_DAI_CALIBRATION
    , ACM_DSP_IF_MODE_DUMMY
	, ACM_DSP_IF_MODE_VOICE_CALL
	, ACM_DSP_IF_MODE_VOCODER_STREAM
	, ACM_DSP_IF_MODE_PCM_STREAMING
	, ACM_DSP_IF_MODE_PCM_WB_STREAMING
	, ACM_DSP_IF_MODE_PCM_LOOPBACK
	, ACM_DSP_IF_MODE_VOCODER_LOOPBACK
	, ACM_DSP_IF_LAST_MODE
	, ACM_DSP_IF_NUMBER_OF_MODES =  ACM_DSP_IF_LAST_MODE,

	ACM_DSP_IF_MODE_ENUM_32_BIT		    	    = 0x7FFFFFFF //32bit enum compiling enforcement
} ACMDspIfAudioMode;

//voice enhance module state
typedef enum
{
	  ACM_DSP_IF_MODULE_OFF = 0
	, ACM_DSP_IF_MODULE_ON  = 1

	, ACM_DSP_IF_MODULE_LAST_STATE
	, ACM_DSP_IF_NUMBER_OF_MODULES_STATES =  ACM_DSP_IF_MODULE_LAST_STATE,

	ACM_DSP_IF_ENHANCE_STATE_ENUM_32_BIT  = 0x7FFFFFFF //32bit enum compiling enforcement
} ACMDspIfEnhanceState;


//CTM struct
typedef struct
{
	unsigned short baudotRate;  /* 0: 45.45; 1: 50 */
	unsigned short baudotInterface;  /* 0 = TTY; 1 = Application */
	unsigned short forceNegotiationIsOn;
	unsigned short disableNegotiation;
	unsigned short disableVcoOrHco;
	signed short   volumeDifferenceIndex;  /* (-49) - (+49); 0 stands for 0 dB difference */
} ACMDspIfCTM_ts;


typedef struct
{
	unsigned short shift;
	unsigned short SSCR0_HIGH;
	unsigned short SSCR0_LOW;
	unsigned short SSCR1_HIGH;
	unsigned short SSCR1_LOW;
	unsigned short SSTSA_LOW;
	unsigned short SSRSA_LOW;
	unsigned short SSPSP_HIGH;
	unsigned short SSPSP_LOW;

//Tavor only:
	unsigned short SSACD_LOW;
	unsigned short SSACDD_HIGH;
	unsigned short SSACDD_LOW;

//SSP Port
	unsigned short devicePort;  /* (GSSP0_PORT=0, GSSP1_PORT=1) */

	unsigned short SSP_Rate;    /* (0=8 kHz, 1=16 kHz)    */
	unsigned short SSP_Mode;    /* (0=unpacked, 1=packed) */

//Jackie, 20110613, add a field 'Dual_Mic'
    unsigned short Dual_Mic;    /* (0:Single mic, 1: Dual Mic)  */    
} ACMDspIfAudioSspConfiguration;

/* This part is for enabling/disabling the audio at the begining/ending of a call */
typedef  void (*ACMDspIfAudioConversationCB) (BOOL isCallStart);



typedef enum
{
   ACM_DSP_IF_TX_PATH                      = 0,
   ACM_DSP_IF_RX_PATH                      = 1
} ACMDspIfPathMode;



typedef enum
{
	ACM_DSP_IF_RATE_8_KHZ = 0,
	ACM_DSP_IF_RATE_16_KHZ,
	ACM_DSP_IF_NUM_OF_RATES
} ACMDspIfVoicePathRate;



/*----------- Extern definition ----------------------------------------------*/
#ifndef _ACM_DSP_IF_NO_EXTERN_
  #define EXTERN extern
#else
  #define EXTERN
#endif /* _ACM_DSP_IF_NO_EXTERN_ */


/*----------- Global variable declarations -----------------------------------*/

/*----------- Global constant definitions ------------------------------------*/


/*----------- Global function prototypes -------------------------------------*/



//uplink & downlink DSP mode
void ACMDspIfAudioUpLinkModeSet(ACMDspIfAudioMode mode[], AudioSrcDst srcDst[], AudioDataFormat format[]);
void ACMDspIfAudioDownLinkModeSet(ACMDspIfAudioMode mode[],
								 AudioSrcDst srcDst[],
								 AudioDataFormat format[],
								 AudioCombWithCallMode audioCombWithCallOut[]);

//digital gain using DSP volume + DSP mute
void ACMDspIfAudioInVolumeSet(signed char dB_8k, signed char dB_16k);
void ACMDspIfAudioOutVolumeSet(signed char dB_8k, signed char dB_16k);
void ACMDspIfAudioMuteIn(unsigned char muteEnable);
void ACMDspIfAudioMuteOut(unsigned char muteEnable);



//voice enhancements settings

void ACMDspIfAudioSideToneGainCtrl(ACMDspIfVoicePathRate voicePathRate, signed short gain);




#ifdef OPT_IPC
void ACMDspIfInitAudioIPCCtrl(void);
void ACMDspIfAudioIPCCtrl(void);
#endif



void ACMDspIfSetDSPActiveControl(BOOL is_on);
void ACMDspIfAudioSetSspConfiguration(ACMDspIfAudioSspConfiguration*,GSSP_Configuration*);

void ACMDspIfAudioCTMCtrl(ACMDspIfPathMode pathMode, ACMDspIfCTM_ts *txCtmControl, ACMDspIfCTM_ts *rxCtmControl);

void ACMSetLoopbackControl(UINT32 OnOff,UINT32 pcmPrint);


void ACMSetECallData(unsigned long num_of_args_present, unsigned short cmd_id, unsigned long param1, void *pData, unsigned long dataLength);
void ACMGetECallData(unsigned short op, unsigned short param1);
void ACMSetECallVoice(unsigned short cmd_id, unsigned short param1, unsigned short param2);
void ACMGetECallVoice(unsigned short cmd_id, unsigned short res_id);


void ACMDspIfInvalidateShadowParams(void);
void ACMDspIfInvalidateDspModes(void);

#undef EXTERN
#endif  /* _ACM_DSP_IF_H_ */






