/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
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

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for ACM_COMM in COMM subsystem
*
* Filename: ACM_COMM.h
*
* Authors: Jackie Fan
*
* Description: Header file for ACM_COMM.
*
* Last Updated:
*
* Notes:
******************************************************************************/
#ifndef _ACM_H_
    #define _ACM_H_

#ifdef __cplusplus
extern "C"{
#endif

/*----------- Local include files --------------------------------------------*/
#include "acmTypes.h"
#ifdef SAMSUNG_TOTORO_DIAMONDVOICE
#include "DiamondVoiceTask.h"
#endif

#include "bsp.h"

#include "audio_atc.h"
#include "acm_audio_primitive.h"
#include "aam.h"    //needed by AAMManage()
extern UINT32	_audioAamHandle;


/*----------- Global constant definitions ------------------------------------*/
#define ACM_INDEX_MODULE_OFF							0xFFFF


/*----------- Global macro definitions ---------------------------------------*/

#define NUM_OF_DB_ENTRIES       (8)

//For Nezha3, CP I2S clock control register is 0xd0250128
#ifdef NEZHA3_1826
#define GSSP_CLOCK_REG_ADDR     0xd0250128
#else
#define GSSP_CLOCK_REG_ADDR     0xd4050040
#endif


UINT32 GSSP_SEL_REG_ADDR(void);
void GSSP_SEL_BY_CP(void);


/*----------- Global type definitions ----------------------------------------*/

// procedure IDs
//ICAT EXPORTED ENUM
typedef enum
{
/******************************************************************
    These ProcIDs are audio stub related
*******************************************************************/
//    AUDIO_STUB_START_ID, 
//    AUDIO_STUB_CONFIRM,
//    AUDIO_STUB_STOP_ID,

/******************************************************************
    These ProcIDs are ACM_COMM related
*******************************************************************/
    AUDIO_ACM_START_ID=100,         //ACM_COMM procID sarts from 100
    AUDIO_REGCPNT_ID,               // 101
    AUDIO_REGCPNT_CONFIRM,          // 102
    
    AUDIO_ENABLEHWCPNT_ID,          // 103
    AUDIO_ENABLEHWCPNT_CONFIRM,     // 104
    
    AUDIO_DISABLEHWCPNT_ID,         // 105
    AUDIO_DISABLEHWCPNT_CONFIRM,    // 106
    
    AUDIO_SETCPNTVOL_ID,            // 107
    AUDIO_SETCPNTVOL_CONFIRM,       // 108
    
    AUDIO_HWCPNTMUTE_ID,            // 109
    AUDIO_HWCPNTMUTE_CONFIRM,       // 110
   
    AUDIO_STREAMINDICATE_ID,        // 111, From COMM to APPS
    AUDIO_STREAMRESPONSE,           // 112, From APPS to COMM
    
    AUDIO_ENABLEOUTSTREAM_ID,       // 113
    AUDIO_ENABLEOUTSTREAM_CONFIRM,  // 114
    
    AUDIO_DISABLEOUTSTREAM_ID,      // 115
    AUDIO_DISABLEOUTSTREAM_CONFIRM, // 116
    
    AUDIO_ENABLEINSTREAM_ID,        // 117
    AUDIO_ENABLEINSTREAM_CONFIRM,   // 118
    
    AUDIO_DISABLEINSTREAM_ID,       // 119
    AUDIO_DISABLEINSTREAM_CONFIRM,  // 120

 /* for enabling/disabling the audio at the begining/ending of a call */
    AUDIO_CONVERSATIONSTART_ID,     // 121, From COMM to APPS
    AUDIO_CONVERSATIONSTOP_ID,      // 122, From COMM to APPS

   /*
    *Jackie, 2010-0518
    * APPS audio send AUDIO_SETMSASETTING_ID to control MSA
    * COMM audio send AUDIO_GETMSASETTING_ID to APPS audio, APPS  can refresh its audio calibration UI
    */
    AUDIO_SETMSASETTING_ID,         // 123
    AUDIO_GETMSASETTING_ID,         // 124

   /*
    *Jackie, 2010-0817
    * APPS audio send AUDIO_SYNC_CODEC_ID to sync gain information of codec chip
    */
    AUDIO_SYNC_CODEC_ID,            // 125

    /*
     *Jackie, 2010-1207
     * MSA detect DTMF tone from far-end.
     */
	AUDIO_DTMFDETECT_ID,	        // 126, From AP to CP: enable/disable
	AUDIO_DTMFDETECTED_ID,	        // 127, From CP to AP:Forward detected DTMF code to AP audio

	/*
	  *Jackie, 2011-0212
	  * Speech logging feature
	  */
	AUDIO_ENABLESPEECHLOGGING_ID,	// 128, From APPS to COMM: start speech logging
	AUDIO_DISABLESPEECHLOGGING_ID,	// 129, From APPS to COMM: stop speech logging
	AUDIO_TXSPEECHDATA_ID,		    // 130, From COMM to APPS: tx speech data
	AUDIO_RXSPEECHDATA_ID,		    // 131, From COMM to APPS: rx speech data

    AUDIO_SETMSAGAIN_ID,            // 132, From APPS to COMM: Set MSA gain
    AUDIO_SETMSAGAIN_CONFIRM,       // 133, From COMM to APPS: Confirm

    AUDIO_SETEQSETTING_ID,          //134,From APPS to COMM: Set USER-EQ
    AUDIO_SETEQSETTING_CONFIRM,     //135,From COMM to APPS: Confirm


}ACM_EXTENT_ID;

typedef enum
{
    ACM_I2S_8KHZ,
    ACM_I2S_11KHZ,
	ACM_I2S_12KHZ,
	ACM_I2S_16KHZ,
    ACM_I2S_22KHZ,
	ACM_I2S_24KHZ,
	ACM_I2S_32KHZ,
    ACM_I2S_44KHZ,
    ACM_I2S_48KHZ,

    /* Must be at the end */
    ACM_I2S_NO_RATE,
    ACM_NUM_OF_I2S_RATES = ACM_I2S_NO_RATE,


	ACM_I2S_RATE_ENUM_32_BIT	 				= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_I2SRate;

typedef enum
{
	ACM_I2S_MONO_TO_LR = 1,
    ACM_I2S_STEREO_TO_LR,
	ACM_I2S_MONO_TO_L = 16,
	ACM_I2S_MONO_TO_R,
	ACM_I2S_NUM_OF_MODES,
	ACM_I2S_MODE_NONE,

	ACM_I2S_MODE_ENUM_32_BIT	 				= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_I2SMode;


typedef enum
{
    ACM_VOICE_CALL,
	ACM_TONE,
    ACM_PCM,
	ACM_PCM_WB,
	ACM_HR,
    ACM_EFR,
	ACM_FR,
    ACM_AMR_MR475,
    ACM_AMR_MR515,
    ACM_AMR_MR59,
    ACM_AMR_MR67,
    ACM_AMR_MR74,
    ACM_AMR_MR795,
    ACM_AMR_MR102,
    ACM_AMR_MR122,
    ACM_AMR_MRCNF,
    ACM_AMR_MRNO_TX_RX,
    ACM_AMR_WB_6_60,
    ACM_AMR_WB_8_85,
    ACM_AMR_WB_12_65,
    ACM_AMR_WB_14_25,
    ACM_AMR_WB_15_85,
    ACM_AMR_WB_18_25,
    ACM_AMR_WB_19_85,
    ACM_AMR_WB_23_05,
    ACM_AMR_WB_23_85,
    ACM_AMR_WB_SID,

	ACM_DUMMY,
	/* Must be at the end */
    ACM_NO_STREAM_TYPE,
    ACM_NUM_OF_STREAM_TYPES = ACM_NO_STREAM_TYPE,


	ACM_STREAM_TYPE_ENUM_32_BIT = 0x7FFFFFFF  /* 32bit enum compiling enforcement */
} ACM_StreamType;

typedef enum
{
	ACM_TICK_SRC_0,
	ACM_TICK_SRC_1,
	ACM_TICK_SRC_2,
	ACM_TICK_SRC_3,

	/* Must be at the end */
    ACM_NO_TICK_SOURCE,
    ACM_NUM_OF_TICK_SOURCES = ACM_NO_TICK_SOURCE,

	ACM_TICK_SRC_ENUM_32_BIT				 	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_TickSource;

typedef enum
{
	ACM_RESOURCE_0,
	ACM_RESOURCE_1,
	ACM_RESOURCE_2,
	ACM_RESOURCE_3,

	/* Must be at the end */
    ACM_NO_RESOURCE,
    ACM_NUM_OF_RESOURCES = ACM_NO_RESOURCE,

	ACM_RESOURCE_ENUM_32_BIT				 	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_Resource;

//ICAT EXPORTED ENUM
typedef enum
{
    ACM_MSA_PCM = 0,  /* first format must be '0' - used by 'for' loops */
    ACM_XSCALE_PCM,
    ACM_I2S,
    ACM_AUDIO_DATA,   /* For DAI */
    ACM_AUX_FM,
    ACM_AUX_HW_MIDI,
    ACM_AUX_APP,

    /* Must be at the end */
    ACM_NO_FORMAT,
    ACM_NUM_OF_AUDIO_FORMATS = ACM_NO_FORMAT,


	ACM_AUDIO_FORMAT_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_AudioFormat;

//ICAT EXPORTED ENUM
typedef enum
{
	/* first device must be '0' - used by 'for' loops */
	  ACM_MAIN_SPEAKER     = 0      // 0, handset    speaker
	, ACM_AUX_SPEAKER               // 1, handsfree  speaker
    , ACM_HEADSET_SPEAKER           // 2, headset    speaker
	, ACM_MONO_LEFT_SPEAKER         // 3
	, ACM_MONO_RIGHT_SPEAKER        // 4
	, ACM_BUZZER                    // 5
	, ACM_MIC                       // 6, handset    mic
	, ACM_MIC_DIFF                  // 7, 
	, ACM_AUX_MIC                   // 8, handsfree  mic
	, ACM_AUX_MIC_DIFF              // 9
	, ACM_BLUETOOTH_SPEAKER         // 10,bluetooth  speaker
	, ACM_BLUETOOTH_MIC             // 11,bluetooth  mic
	, ACM_DAI_OUT                   // 12
	, ACM_DAI_IN                    // 13
	, ACM_CAR_KIT_SPEAKER           // 14
	, ACM_CAR_KIT_MIC               // 15
	, ACM_INPUT_DEVICE_8            // 16
	, ACM_HEADSET_MIC               // 17,headset   mic
	, ACM_MIC_EC                    // 18
	, ACM_AUX_MIC_EC		        // 19
	, ACM_HEADSET_MIC_EC            // 20
	, ACM_MIC_LOOP_SPEAKER          // 21
	, ACM_MIC_LOOP_EARPIECE         // 22
	, ACM_HEADSET_MIC_LOOP          // 23

	,ACM_TTY_IN_45
	,ACM_INPUT_DEVICE_10 = ACM_TTY_IN_45      //24

	,ACM_TTY_IN_50
	,ACM_INPUT_DEVICE_11 = ACM_TTY_IN_50      //25

	,ACM_TTY_IN_HCO                 // 26
	,ACM_TTY_VCO_MIC                    // 27
	,ACM_TTY_VCO_MIC_DUALMIC           // 28
	,ACM_INPUT_DEVICE_15            // 29
	,ACM_INPUT_DEVICE_16            // 30
	,ACM_INPUT_DEVICE_17            // 31
	,ACM_INPUT_DEVICE_18            // 32
	,ACM_INPUT_DEVICE_19            // 33
	,ACM_INPUT_DEVICE_20            // 34
	,ACM_INPUT_TEST_DEVICE = ACM_INPUT_DEVICE_20
	,ACM_INPUT_DEVICE_21            // 35
	,ACM_LINE_OUT = ACM_INPUT_DEVICE_21

	,ACM_INPUT_DEVICE_22            // 36
	,ACM_INPUT_DEVICE_23            // 37
	,ACM_INPUT_DEVICE_24            // 38
	,ACM_INPUT_DEVICE_25            // 39
	,ACM_INPUT_DEVICE_26            // 40
	,ACM_INPUT_DEVICE_27            // 41

	,ACM_TTY_OUT_45
	,ACM_OUTPUT_DEVICE_10 = ACM_TTY_OUT_45     // 42

	,ACM_TTY_OUT_50
	,ACM_OUTPUT_DEVICE_11 = ACM_TTY_OUT_50     // 43

	,ACM_TTY_HCO_SPEAKER            // 44
	,ACM_TTY_OUT_VCO                // 45
	,ACM_TTY_OUT_VCO_DUALMIC        // 46
	,ACM_OUTPUT_DEVICE_15           // 47
	,ACM_OUTPUT_DEVICE_16           // 48
	,ACM_OUTPUT_DEVICE_17           // 49
	,ACM_OUTPUT_DEVICE_18           // 50
	,ACM_OUTPUT_DEVICE_19           // 51
	,ACM_OUTPUT_DEVICE_20           // 52
	,ACM_OUTPUT_TEST_DEVICE = ACM_OUTPUT_DEVICE_20
	,ACM_OUTPUT_DEVICE_21           // 53
	,ACM_OUTPUT_DEVICE_22           // 54
	,ACM_OUTPUT_DEVICE_23           // 55
	,ACM_OUTPUT_DEVICE_24           // 56
	,ACM_OUTPUT_DEVICE_25           // 57
	,ACM_OUTPUT_DEVICE_26           // 58
	,ACM_OUTPUT_DEVICE_27           // 59
	,ACM_OUTPUT_DEVICE_28           // 60
	,ACM_OUTPUT_DEVICE_29           // 61

    ,ACM_WB_BLUETOOTH_SPEAKER       // 62, WB BLUETOOTH speaker
    ,ACM_WB_BLUETOOTH_MIC           // 63, WB BLUETOOTH mic
    ,ACM_WB_BLUETOOTH_NREC_SPEAKER  // 64, WB BLUETOOTH NREC speaker
    ,ACM_WB_BLUETOOTH_NREC_MIC      // 65, WB BLUETOOTH NREC mic
    ,ACM_HEADPHONE_SPEAKER        // 66, HEADSET3POLE speaker
    ,ACM_HEADPHONE_MIC            // 67, HEADSET3POLE mic
    ,ACM_EXTRA_VOLUME_MAIN_SPEAKER    // 68,Handset speaker with extra volume on
    ,ACM_EXTRA_VOLUME_MIC             // 69, Handset mic with extra volume on
    ,ACM_EXTRA_VOLUME_AUX_SPEAKER     // 70, Aux speaker with extra volume on
    ,ACM_EXTRA_VOLUME_AUX_MIC         // 71, Aux mic with extra volume on
    ,ACM_BLUETOOTH6_SPEAKER         // 72, BLUETOOTH6 speaker
    ,ACM_BLUETOOTH6_MIC             // 73, BLUETOOTH6 mic
    ,ACM_BLUETOOTH7_SPEAKER         // 74, BLUETOOTH7 speaker
    ,ACM_BLUETOOTH7_MIC             // 75, BLUETOOTH7 mic
    ,ACM_BLUETOOTH8_SPEAKER         // 76, BLUETOOTH8 speaker
    ,ACM_BLUETOOTH8_MIC             // 77, BLUETOOTH8 mic
    ,ACM_BLUETOOTH_NREC_SPEAKER     // 78, BLUETOOTH-NREC speaker
    ,ACM_BLUETOOTH_NREC_MIC         // 79, BLUETOOTH-NREC mic

    //Jackie,2011-0222
	//Loop include codec and MSA
	,ACM_MAIN_SPEAKER__LOOP         // 80, handset speaker for loopback test
	,ACM_AUX_SPEAKER__LOOP          // 81, handsfree speaker for loopback test
	,ACM_HEADSET_SPEAKER__LOOP      // 82, headset speaker for loopback test
	,ACM_MIC__LOOP                  // 83, handset mic for loopback test
	,ACM_AUX_MIC__LOOP              // 84, handsfree mic for loopback test
	,ACM_HEADSET_MIC__LOOP          // 85, headset mic for loopback test
    ,ACM_HEADPHONE_SPEAKER__LOOP    // 86, bluetooth speaker for loopback test
    ,ACM_HEADPHONE_MIC__LOOP        // 87, bluetooth mic for loopback test
    
    //Jackie, 2011-0603
    //Dual mic devices
	,ACM_MAIN_SPEAKER_DUALMIC       // 88, handset speaker for dual mic solution 
	,ACM_AUX_SPEAKER_DUALMIC        // 89, handsfree speaker for dual mic solution 
    ,ACM_HEADSET_SPEAKER_DUALMIC    // 90, headset speaker for dual mic solution 
    ,ACM_BLUETOOTH_SPEAKER_DUALMIC  // 91, bluetooth speaker for dual mic solution 
    ,ACM_BLUETOOTH_NREC_SPEAKER_DUALMIC // 92, bluetooth NREC speaker  for dual mic solution 
    
    ,ACM_MIC_DUALMIC                // 93, handset mic for dual mic solution 
    ,ACM_AUX_MIC_DUALMIC            // 94, handsfree mic for dual mic solution 
    ,ACM_HEADSET_MIC_DUALMIC        // 95, headset mic for dual mic solution 
    ,ACM_BLUETOOTH_MIC_DUALMIC      // 96, bluetooth mic for dual mic solution 
    ,ACM_BLUETOOTH_NREC_MIC_DUALMIC // 97, bluetooth NREC mic  for dual mic solution 
    

    //Jackie, 2011-0915
    //VT devices
	,ACM_MAIN_SPEAKER_VT            // 98, handset speaker for VT
	,ACM_AUX_SPEAKER_VT             // 99, handsfree speaker for VT
    ,ACM_HEADSET_SPEAKER_VT         // 100, headset speaker for VT
    ,ACM_BLUETOOTH_SPEAKER_VT       // 101, bluetooth speaker for VT
    ,ACM_BLUETOOTH_NREC_SPEAKER_VT  // 102, bluetooth NREC speaker  for VT
    
    ,ACM_MIC_VT                     // 103, handset mic for VT
    ,ACM_AUX_MIC_VT                 // 104, handsfree mic for VT
    ,ACM_HEADSET_MIC_VT             // 105, headset mic for VT
    ,ACM_BLUETOOTH_MIC_VT           // 106, bluetooth mic for VT
    ,ACM_BLUETOOTH_NREC_MIC_VT      // 107, bluetooth NREC mic  for VT

    //VT_DUALMIC devices
	,ACM_MAIN_SPEAKER_VT_DUALMIC            // 108, handset speaker for VT_DUALMIC
	,ACM_AUX_SPEAKER_VT_DUALMIC             // 109, handsfree speaker for VT_DUALMIC
    ,ACM_HEADSET_SPEAKER_VT_DUALMIC         // 110, headset speaker for VT_DUALMIC
    ,ACM_BLUETOOTH_SPEAKER_VT_DUALMIC       // 111, bluetooth speaker for VT_DUALMIC
    ,ACM_BLUETOOTH_NREC_SPEAKER_VT_DUALMIC  // 112, bluetooth NREC speaker  for VT_DUALMIC
    
    ,ACM_MIC_VT_DUALMIC                     // 113, handset mic for VT_DUALMIC
    ,ACM_AUX_MIC_VT_DUALMIC                 // 114, handsfree mic for VT_DUALMIC
    ,ACM_HEADSET_MIC_VT_DUALMIC             // 115, headset mic for VT_DUALMIC
    ,ACM_BLUETOOTH_MIC_VT_DUALMIC           // 116, bluetooth mic for VT_DUALMIC
    ,ACM_BLUETOOTH_NREC_MIC_VT_DUALMIC      // 117, bluetooth NREC mic  for VT_DUALMIC    

    //////////////Wu Bo, 2012-0525, support 'VOIP over modem'///////////////
    //VOIP devices
    ,ACM_MAIN_SPEAKER_VOIP                  // 118, handset speaker for VOIP
    ,ACM_AUX_SPEAKER_VOIP                   // 119, handfree speaker for VOIP
    ,ACM_HEADSET_SPEAKER_VOIP               // 120, headset speaker for VOIP
    ,ACM_BLUETOOTH_SPEAKER_VOIP             // 121, bluetooth speaker for VOIP
    ,ACM_BLUETOOTH_NREC_SPEAKER_VOIP        // 122, bluetooth NREC speaker for VOIP
    ,ACM_BLUETOOTH_SPEAKER_VOIP_WB          // 123, bluetooth speaker for VOIP WB
    ,ACM_BLUETOOTH_NREC_SPEAKER_VOIP_WB     // 124, bluetooth NREC speaker for VOIP WB

    ,ACM_MIC_VOIP                            // 125, handset mic for VOIP
    ,ACM_AUX_MIC_VOIP                        // 126, handfree mic for VOIP
    ,ACM_HEADSET_MIC_VOIP                    // 127, headset mic for VOIP
    ,ACM_BLUETOOTH_MIC_VOIP                  // 128, bluetooth mic for VOIP,NB
    ,ACM_BLUETOOTH_NREC_MIC_VOIP             // 129, bluetooth NREC mic for VOIP,NB
    ,ACM_BLUETOOTH_MIC_VOIP_WB               // 130, bluetooth mic for VOIP,WB
    ,ACM_BLUETOOTH_NREC_MIC_VOIP_WB          // 131, bluetooth NREC mic for VOIP,WB

    //VOIP_DUALMIC devices
    ,ACM_MAIN_SPEAKER_VOIP_DUALMIC               // 132, handset speaker for VOIP_DUALMIC
    ,ACM_AUX_SPEAKER_VOIP_DUALMIC                // 133, handfree speaker for VOIP_DUALMIC
    ,ACM_HEADSET_SPEAKER_VOIP_DUALMIC            // 134, headset speaker for VOIP_DUALMIC

    ,ACM_MIC_VOIP_DUALMIC                        // 135, handset mic for VOIP_DUALMIC,
    ,ACM_AUX_MIC_VOIP_DUALMIC                    // 136, handfree mic for VOIP_DUALMIC,
    ,ACM_HEADSET_MIC_VOIP_DUALMIC                // 137, headset mic for VOIP_DUALMIC,

	,ACM_MAIN_SPEAKER__LOOP2                     // 138, handset speaker for loopback test for factory test
	,ACM_AUX_SPEAKER__LOOP2                      // 139, handsfree speaker for loopback test for factory test
	,ACM_HEADSET_SPEAKER__LOOP2                  // 140, headset speaker for loopback test for factory test
	,ACM_MIC__LOOP2                              // 141, handset mic for loopback test for factory test
	,ACM_AUX_MIC__LOOP2                          // 142, handsfree mic for loopback test for factory test
	,ACM_HEADSET_MIC__LOOP2                      // 143, headset mic for loopback test for factory test
    ,ACM_HEADPHONE_SPEAKER__LOOP2                // 144, bluetooth speaker for loopback test for factory test
    ,ACM_HEADPHONE_MIC__LOOP2                    // 145, bluetooth mic for loopback test for factory test

	/* Must be at the end */    
    ,ACM_NUM_OF_AUDIO_DEVICES

    //Tag for search end of audio device table
	,ACM_NOT_CONNECTED = 0x7fffffff 

	,ACM_AUDIO_DEVICE_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_AudioDevice;

//ICAT EXPORTED ENUM
typedef enum
{
    ACM_FORMAT_NOT_SUPPORTED = 0,
    ACM_FORMAT_SUPPORTED = 1,

	ACM_FORMAT_SUPPORTED_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_FormatSupported;


typedef enum
{
     ACM_BOTH_ENDS = 0  /* Highest priority */
    ,ACM_NEAR_END
    ,ACM_FAR_END         /* Lowest priority */
	,ACM_NO_END

	,ACM_SRC_DST_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_SrcDst;

typedef enum
{
    ACM_NOT_COMB_WITH_CALL = 0,  /* Lowest priority */
    ACM_COMB_WITH_CALL = 1,       /* Highest priority */

	ACM_COMB_WITH_CALL_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_CombWithCall;

typedef enum
{
    ACM_NEAR_NOT_CHANGED = 0,  
    ACM_NEAR_CODEC = 1,
    ACM_NEAR_VOCODER = 2,
} ACM_NearCodecVocoder;

//ICAT EXPORTED ENUM
typedef enum
{
    ACM_PATH_IN  = 0,   //Tx
    ACM_PATH_OUT = 1,   //Rx

    /* Must be at the end */
    ACM_NUM_OF_PATHS,

	ACM_PATH_DIRECTION_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_PathDirection;

//ICAT EXPORTED ENUM
typedef enum
{
    ACM_SCENARIO_AUDIO = 0,   //audio
    ACM_SCENARIO_VOICE = 1,   //voice    

    /* Must be at the end */
    ACM_SCENARIO_CNT,

	ACM_SCENARIO_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_SCENARIOT;


/*
    Used for identify UE DSP type for CT-Audio
    CT-Audio will display different MSA audio blocks.
*/
typedef enum
{
	AUDIO_NORMAL	            = 0,
	AUDIO_SAMSUNG	            = 1,
	AUDIO_VOIP_TUNING	        = 2,
	AUDIO_SINGLE_NVM	        = 4,
	
	ACM_AUDIO_TYPE_ENUM_32_BIT				 	= 0x7FFFFFFF //32bit enum compiling enforcement
}ACM_AUDIO_TYPE;


//ICAT EXPORTED STRUCT
typedef struct
{
    unsigned short Configure;     		//bit 0:CP send confirmation to AP; bit 1: reuse speaker as receiver; bit 2: bypass PM813 PA
    unsigned short Always_Print_PCM;    //default:1(Print PCM in call); If in production phase, set this to 0 to save traffic
    unsigned short Disable_All_Modules;	//default:0
        
	unsigned char  description[MAX_DESCRIPTION_LEN];
} ACM_Configuration;



//ICAT EXPORTED STRUCT
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
} GSSP_Configuration;




/**************************************************************
 defination of stream requset

 Because audio stub have no idea of the ProcID in ACM_COMM, and
 audio stub just free pparm.
 Audio stub will not recurve into pparm and free dynamic malloc memory.

 So, here use fixed-size array "streamInd[NUM_OF_DB_ENTRIES]" 
***************************************************************/
typedef  struct
{
    UINT32  streamHandle;
    UINT16   *data;
    UINT32  dataSize;
}STREAM_INDICATE;

//Sent from COMM to APPS
//Uplink stream request needs to hold many streamHandles
typedef  struct
{
    UINT32  handle_num; //max is NUM_OF_DB_ENTRIES
    STREAM_INDICATE streamInd[NUM_OF_DB_ENTRIES];
}UPLINKSTREAM_REQUEST;

//Sent from APPS to COMM
//Downlink stream request just needs to hold one streamHandle
typedef  struct
{
    UINT32  handle_num;
    STREAM_INDICATE streamInd[2];
    UINT32  streamSrc;
}DOWNLINKSTREAM_REQUEST;

typedef void (*ACM_ResetStatusInd)(void);

/*----------------------------AudioStreamOutStart----------------------------*/
typedef struct   
{ 
    UINT32                      streamHandle;
    ACM_StreamType              streamType;
    ACM_SrcDst                  srcDst;
    ACM_CombWithCall            combWithCall;  
    ACM_AudioConfirmID          id;
} ACMAudioStreamOutStartReq;

typedef struct   
{ 
    int                         status;
    UINT32                      streamHandle;
    ACM_AudioConfirmID          id;
} ACMAudioStreamOutStartRsp;


/*----------------------------AudioStreamInStart----------------------------*/
typedef struct   
{ 
    UINT32                      streamHandle;
    ACM_StreamType              streamType;
    ACM_SrcDst                  srcDst;
    ACM_AudioConfirmID          id;
} ACMAudioStreamInStartReq;

typedef ACMAudioStreamOutStartRsp ACMAudioStreamInStartRsp;


/*----------------------------AudioStreamOutStop----------------------------*/
typedef struct   
{ 
    UINT32                      streamHandle;
    ACM_AudioConfirmID          id;
} ACMAudioStreamOutStopReq;

typedef ACMAudioStreamOutStartRsp ACMAudioStreamOutStopRsp;


/*----------------------------AudioStreamInStop----------------------------*/
typedef struct   
{ 
    UINT32                      streamHandle;
    ACM_AudioConfirmID          id;
} ACMAudioStreamInStopReq;

typedef ACMAudioStreamOutStartRsp ACMAudioStreamInStopRsp;

// ECall
#define ACIPC_VCM_ECALL_DATA_BUFFER_MAX_LEN                 (140)
#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_SET                 (0x1e)
#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_GET                 (0x1f)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_SET                (0x20)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_GET                (0x21)

#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_IND                 (0x0100 + 0x1e)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_IND                (0x0100 + 0x20)
#define ACIPC_CODE_AUDIO_VCM_ECALL_DATA_GET_CNF             (0x0200 + 0x1f)
#define ACIPC_CODE_AUDIO_VCM_ECALL_VOICE_GET_CNF            (0x0200 + 0x21)

//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_SET {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
    unsigned char data[ACIPC_VCM_ECALL_DATA_BUFFER_MAX_LEN];
} ACIPC_AUDIO_VCM_ECALL_DATA_SET;

//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_GET {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
} ACIPC_AUDIO_VCM_ECALL_DATA_GET;

//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_SET {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
    unsigned long param2;
} ACIPC_AUDIO_VCM_ECALL_VOICE_SET;

//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_GET {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
} ACIPC_AUDIO_VCM_ECALL_VOICE_GET;


//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_IND {
    unsigned long command;
    unsigned long urc_id;
    unsigned long data;
} ACIPC_AUDIO_VCM_ECALL_DATA_IND;


//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF {
    unsigned long command;
    unsigned long op;
    unsigned long param1;
    unsigned long value1;
    unsigned long value2;
} ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF;


//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_IND {
    unsigned long command;
    unsigned long res_id;
    unsigned long res_state;
} ACIPC_AUDIO_VCM_ECALL_VOICE_IND;


//ICAT EXPORTED STRUCT
typedef struct _ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF {
    unsigned long command;
    unsigned long cmd_id;
    unsigned long res_id;
    unsigned long param2;
} ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF;

typedef void(*AUDIO_ECALL_CNF_CB)(const ACIPC_AUDIO_VCM_ECALL_VOICE_GET_CNF* ecall_voice, const ACIPC_AUDIO_VCM_ECALL_DATA_GET_CNF* ecall_data);

typedef struct _AUDIO_CI_INDI {
    INT8 rawData[128];
    INT16 length;
} AUDIO_CI_INDI;




typedef struct   
{ 
    UINT8         VersionNum;
    UINT8         ModemPcmModeMaster;
	UINT8         WBEnable;
	UINT8         reserved[13];
} CPInitDataForAP;


typedef struct   
{ 
    UINT8         VersionNum;
    UINT8         ModemPcmModeMaster;
	UINT8         WBEnable;
	UINT8         reserved[13];
} APInitDataForCP;


/*----------------------------AudioPutDSPSettings----------------------------*/
#define ACMAUDIO_MSASETTINGS_MAXLENGTH	256


//ICAT EXPORTED STRUCT
typedef struct   
{ 
    UINT8                             data[ACMAUDIO_MSASETTINGS_MAXLENGTH];
} ACMAudioDSPSettings;





/*----------------------------Audio Speech Logging----------------------------*/
#define ACMAUDIO_SPEECH_MAXLENGTH	2048


//ICAT EXPORTED STRUCT
typedef struct   
{ 
	UINT16							  length;
    UINT8                             data[ACMAUDIO_SPEECH_MAXLENGTH];	
} ACMAudioSpeechData;

void set_current_scenario(ACM_SCENARIOT scenario);
int set_phone_state(unsigned int status);



           
//ICAT EXPORTED ENUM
typedef enum
{
	VC_HANDSET = 0,	
	VC_HANDSFREE,
	VC_HEADSET,		//with mic
	VC_HEADPHONE,	//without mic
	VC_BT,
	VC_LOOPBACK,
	VC_MAXCOUNT,

	AC_HANDSET = 64,	
	AC_HANDSFREE,
	AC_HEADSET,		//with mic
	AC_HEADPHONE,	//without mic
	AC_BT,
	AC_KWS,

	AC_FM,
	AC_MAXCOUNT,

	ACM_PROFILE_ID_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACM_PROFILE_ID;




/*----------- Extern definition ----------------------------------------------*/


/*----------- Global variable declarations -----------------------------------*/

//global varable to switch whether send confirm ProcID to APPS
extern BOOL ACMCOMM_ConfirmProcID;   //defauld=FALSE: not send confirm
extern UINT32 audio_PCM_Print;
extern BOOL ACMCOMM_CombinedSPK;
extern BOOL audio_IsBypassPM813PA; //default: not bypass PM813 PA


/*----------- Global API function definition ---------------------------------*/


ACM_ReturnCode ACMAudioStreamInStop(UINT32 streamHandle,
                                    ACM_AudioConfirmID id);
ACM_ReturnCode ACMAudioStreamInStopImpl(UINT32 streamHandle,
                                    ACM_AudioConfirmID id);
ACM_ReturnCode ACMAudioStreamOutStop(UINT32 streamHandle,
                                    ACM_AudioConfirmID id);

ACM_ReturnCode ACMAudioStreamInStart(
                                     UINT32 streamHandle,   
                                     ACM_StreamType         streamType,
                                     ACM_SrcDst             srcDst,
                                     ACM_AudioConfirmID id);
ACM_ReturnCode ACMAudioStreamInStartImpl(
                                     UINT32 streamHandle,
                                     ACM_StreamType         streamType,
                                     ACM_SrcDst             srcDst,
                                     ACM_AudioConfirmID id);

ACM_ReturnCode ACMAudioStreamOutStart(
                                      UINT32 streamHandle,   
                                      ACM_StreamType         streamType,
                                      ACM_SrcDst             srcDst,
                                      ACM_CombWithCall       combWithCall,
                                      ACM_AudioConfirmID id);

ACM_FormatSupported ACMAudioFormatSupported(ACM_AudioDevice device,
                                            ACM_AudioFormat format);

ACM_ReturnCode ACMComponentInit(void);
void ACMReloadFDI(void);

void ACMCOMMInit(void);
void ACMInit(void);
void ACMStreamInit(void);
void ACMStreamReset(void);

void ATCSetMSAVoicePath(void);
void ATCSetMediaPath(void);

typedef void(*SetDownLinkStream)(DOWNLINKSTREAM_REQUEST *streamReq);
typedef void(*SetUpLinkStream)(const UINT8* buf, UINT32 size);
void DownLinkStreamSet (DOWNLINKSTREAM_REQUEST *streamReq);

typedef struct ATCPCMPlayState{
	UINT8  on_off;
	UINT8  near_far_end;
	UINT8  near_codec_or_vocoder;
	UINT8  comb_with_call;
    UINT8  direct_write;
}ATCPCMPlayState;

ACM_PROFILE_ID GetProfileIDFromPath(UINT32 scenario, UINT32 path);

BOOL isVoiceEnabled(void);
BOOL isStreamScenarioActive(void);
void ACMAudioBindECallGetHandler(AUDIO_ECALL_CNF_CB cb);

// for sdk user
void ACMAudioDTMFStart(int ascii);
void ACMAudioDTMFStop(void);
void ACMAudioSetDTMFVolume(int vol);
void ACMAudioSetDTMFMode(int mode);
void ACMAudioDTMF_Start(int f1, int f2);
void ACMAudioDTMF_Stop(void);

void ACMAudioOutGainSet(int db);
void ACMVoiceInGainSet(int db);
void ACMVoiceOutGainSet(int db);
void ACMVoiceSideToneGainSet(int db);
void ACMVoiceMuteIn(int mute);
void ACMVoiceMuteOut(int mute);
void ACMVoiceGetMuteIn(int* mute);
void ACMVoiceGetMuteOut(int* mute);

#ifdef __cplusplus
}
#endif
#endif  /* _ACM_H_ */
