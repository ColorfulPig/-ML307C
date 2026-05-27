/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2018 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for Audio HAL
*
* Filename: AudioHAL.h
*
* Authors: Jackie Fan
*
* Description: Header file for Audio HAL.
*
* Last Updated:
*
* Notes:
******************************************************************************/
#ifndef _AUDIOHAL_H_
    #define _AUDIOHAL_H_

#ifdef __cplusplus
extern "C"{
#endif
int

/* cmiot modify start  直接将此文件加入编译缺相关依赖，暂先自行处理*/
#ifndef VOID
    #define VOID	void
#endif
typedef signed short    INT16;        /* Signed 16-bit quantity         */
typedef signed long     INT32;        /* Signed 32-bit quantity         */
typedef unsigned char   UINT8;        /* Unsigned 8-bit quantity        */
typedef unsigned short  UINT16;       /* Unsigned 16-bit quantity       */
typedef unsigned long   UINT32;       /* Unsigned 32-bit quantity       */
typedef  unsigned char BOOL;
typedef unsigned long long    UINT64;
typedef int                                     INT;
/* cmiot modify end  直接将此文件加入编译缺相关依赖，暂先自行处理*/

/******************************************************************
	CP audio local enumerations
*******************************************************************/
//ICAT EXPORTED ENUM
typedef enum{
    HEAD_PHONE_INIT = 0,
    HEAD_PHONE_OUT,		// 1
    HEAD_TYPE_UNKNOWN,	// 2
    HEAD_PHONE_TYPE3,	// 3
    HEAD_PHONE_TYPE4,	// 4
    HEAD_PHONE_TYPE_C_REVERSE,	// 5
    HEAD_PHONE_TYPE_C,	// 6

    HEAD_PHONE_ERROR = 0xFFFF
}HEADPHONE_STATE;


//ICAT EXPORTED ENUM
typedef enum{
    HOOK_KEY_EVENT_NULL = 0,
    VOL_UP_PRESS,		// 1
    VOL_DOWN_PRESS,		// 2
    HOOK_KEY_PRESS,		// 3
    VOL_UP_RELEASE,		// 4
    VOL_DOWN_RELEASE,	// 5
    HOOK_KEY_RELEASE,	// 6

    HEADPHONE_EVENT_ERROR = 0xFFFF    
}HEADPHONE_EVENT;

#define AUDIOHAL_STREAM_SUPPORT
#if FEATURE_AUDIO_HAL_STREAM == 0
#undef AUDIOHAL_STREAM_SUPPORT
#endif


//AudioHAL task flags:
#define AUDIO_RESAMPLE_FLAGS_PLAYBACK   (0x01)
#define AUDIO_RESAMPLE_FLAGS_RECORD     (0x02)
#define AUDIO_HEADSET_FLAG              (0x04)
#define AUDIO_CODEC_UPDATE_EVENT        (0x08)
#define AUDIO_VOICE_CALL_START          (0x10)
#define AUDIO_VOICE_CALL_END            (0x20)
#define AUDIO_MEDIA_STOP                (0x40)
#define AUDIO_MEDIA_START               (0x80)

#define AUDIO_GETVOICE_EVENT       		(0x100)
#define AUDIO_PMIC_CHECK                (0x200)
#define AUDIO_POC_DATA_EVENT            (0x400)
#define AUDIO_DTMFDETECTION_EVENT       (0x800)
#define AUDIO_EXTRA_EVENT       		(0x1000)


#define AUDIOHAL_FLAGS_MASK    (AUDIO_RESAMPLE_FLAGS_PLAYBACK\
                                | AUDIO_RESAMPLE_FLAGS_RECORD\
                                | AUDIO_HEADSET_FLAG\
                                | AUDIO_CODEC_UPDATE_EVENT\
                                | AUDIO_VOICE_CALL_END\
                                | AUDIO_VOICE_CALL_START\
                                | AUDIO_MEDIA_START\
                                | AUDIO_MEDIA_STOP\
                                | AUDIO_PMIC_CHECK\
                                | AUDIO_POC_DATA_EVENT\
                                | AUDIO_DTMFDETECTION_EVENT\
                                | AUDIO_GETVOICE_EVENT\
                                | AUDIO_EXTRA_EVENT)                                




/******************************************************************
	Enumerations shared with upper layer
*******************************************************************/
#define AUDIOHAL_VOLCNT	9		//0~8

//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_ERR_NO = 0,  //No error
    AUDIOHAL_ERR_RESOURCE_RESET,
    AUDIOHAL_ERR_RESOURCE_BUSY,
    AUDIOHAL_ERR_RESOURCE_TIMEOUT,
    AUDIOHAL_ERR_RESOURCE_NOT_ENABLED,
    AUDIOHAL_ERR_BAD_PARAMETER,
    
    AUDIOHAL_ERR_UART_RX_OVERFLOW,
    AUDIOHAL_ERR_UART_TX_OVERFLOW,
    AUDIOHAL_ERR_UART_PARITY,
    AUDIOHAL_ERR_UART_FRAMING,
    AUDIOHAL_ERR_UART_BREAK_INT,

    AUDIOHAL_ERR_TIM_RTC_NOT_VALID,
    AUDIOHAL_ERR_TIM_RTC_ALARM_NOT_ENABLED,
    AUDIOHAL_ERR_TIM_RTC_ALARM_NOT_DISABLED,

    AUDIOHAL_ERR_COMMUNICATION_FAILED,
    
    /* Must be at the end */
    AUDIOHAL_ERR_QTY,


	AUDIOHAL_ERR_ENUM_32_BIT		    	= 0x7FFFFFFF //32bit enum compiling enforcement
} AUDIOHAL_ERR_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_ITF_RECEIVER         = 0,
    AUDIOHAL_ITF_EARPIECE, 
    AUDIOHAL_ITF_HEADPHONE = AUDIOHAL_ITF_EARPIECE,
    AUDIOHAL_ITF_LOUDSPEAKER,
    AUDIOHAL_ITF_LOUDSPEAKER_AND_HEADPHONE,
    //AUDIOHAL_ITF_LOUDSPEAKER_AND_HEADPHONE = AUDIOHAL_ITF_LOUDSPEAKER_AND_EARPIECE,
    AUDIOHAL_ITF_BLUETOOTH,
    AUDIOHAL_ITF_FM,
    AUDIOHAL_ITF_FM2SPK,
    AUDIOHAL_ITF_TV,
    AUDIOHAL_ITF_BLUETOOTH_WB,
    AUDIOHAL_ITF_HS_LEFT,
    AUDIOHAL_ITF_HS_RIGHT,
    AUDIOHAL_ITF_VAD,
    AUDIOHAL_ITF_FM2RCV,

    AUDIOHAL_ITF_QTY,
    AUDIOHAL_ITF_NONE = 255,
} AUDIOHAL_ITF_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_SPK_RECEIVER         = 0,
    AUDIOHAL_SPK_EARPIECE,
    AUDIOHAL_SPK_LOUDSPEAKER,
    AUDIOHAL_SPK_LOUDSPEAKER_EARPIECE,  //Output on both hands-free loud speaker and earpiece

    AUDIOHAL_SPK_QTY,
    AUDIOHAL_SPK_DISABLE = 255,
} AUDIOHAL_SPK_T;

//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_SPEAKER_STEREO         = 0,
    AUDIOHAL_SPEAKER_MONO_RIGHT,
    AUDIOHAL_SPEAKER_MONO_LEFT,
    AUDIOHAL_SPEAKER_STEREO_NA,  //Output is mono only

    AUDIOHAL_SPEAKER_QTY,
    AUDIOHAL_SPEAKER_DISABLE = 255,
} AUDIOHAL_SPEAKER_TYPE_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_MIC_RECEIVER         = 0,
    AUDIOHAL_MIC_EARPIECE,
    AUDIOHAL_MIC_LOUDSPEAKER,

    AUDIOHAL_MIC_QTY,
    AUDIOHAL_MIC_DISABLE = 255,
} AUDIOHAL_MIC_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_SPK_MUTE         = 0,
    AUDIOHAL_SPK_VOL_1,
    AUDIOHAL_SPK_VOL_2,
    AUDIOHAL_SPK_VOL_3,
    AUDIOHAL_SPK_VOL_4,
    AUDIOHAL_SPK_VOL_5,
    AUDIOHAL_SPK_VOL_6,
    AUDIOHAL_SPK_VOL_7,
    AUDIOHAL_SPK_VOL_8,
    AUDIOHAL_SPK_VOL_9,
    AUDIOHAL_SPK_VOL_10,
    AUDIOHAL_SPK_VOL_11,	// 11

	AUDIOHAL_SPK_VOL_QTY	// 12
} AUDIOHAL_SPK_LEVEL_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_MIC_MUTE         = 0,
    AUDIOHAL_MIC_ENABLE,

    AUDIOHAL_MIC_VOL_QTY,
} AUDIOHAL_MIC_LEVEL_T;

//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_SIDE_MUTE         = 0,
    AUDIOHAL_SIDE_VOL_1,
    AUDIOHAL_SIDE_VOL_2,
    AUDIOHAL_SIDE_VOL_3,
    AUDIOHAL_SIDE_VOL_4,
    AUDIOHAL_SIDE_VOL_5,
    AUDIOHAL_SIDE_VOL_6,
    AUDIOHAL_SIDE_VOL_7,
    AUDIOHAL_SIDE_VOL_TEST,
    AUDIOHAL_SIDE_VOL_QTY,
} AUDIOHAL_SIDE_LEVEL_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_FREQ_8000HZ  =  8000,
    AUDIOHAL_FREQ_11025HZ = 11025,
    AUDIOHAL_FREQ_12000HZ = 12000,
    AUDIOHAL_FREQ_16000HZ = 16000,
    AUDIOHAL_FREQ_22050HZ = 22050,
    AUDIOHAL_FREQ_24000HZ = 24000,
    AUDIOHAL_FREQ_32000HZ = 32000,
    AUDIOHAL_FREQ_44100HZ = 44100,
    AUDIOHAL_FREQ_48000HZ = 48000,    
    AUDIOHAL_FREQ_64000HZ = 64000,    
    AUDIOHAL_FREQ_88200HZ = 88200,    
    AUDIOHAL_FREQ_96000HZ = 96000,   
} AUDIOHAL_FREQ_T;

//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_MONO         = 1,
    AUDIOHAL_STEREO       = 2,
} AUDIOHAL_CH_NB_T;

//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_DTMF_0 = 0,
    AUDIOHAL_DTMF_1,
    AUDIOHAL_DTMF_2,
    AUDIOHAL_DTMF_3,
    AUDIOHAL_DTMF_4,
    AUDIOHAL_DTMF_5,
    AUDIOHAL_DTMF_6,
    AUDIOHAL_DTMF_7,
    AUDIOHAL_DTMF_8,
    AUDIOHAL_DTMF_9,
    AUDIOHAL_DTMF_A,
    AUDIOHAL_DTMF_B,
    AUDIOHAL_DTMF_C,
    AUDIOHAL_DTMF_D,
    AUDIOHAL_DTMF_S,        //* key
    AUDIOHAL_DTMF_P,        //# key
    AUDIOHAL_COMFORT_425,
    AUDIOHAL_COMFORT_950,
    AUDIOHAL_COMFORT_1400,
    AUDIOHAL_COMFORT_1800,
} AUDIOHAL_TONE_TYPE_T;


//ICAT EXPORTED ENUM
typedef enum
{
    AUDIOHAL_TONE_0DB = 0,
    AUDIOHAL_TONE_M3DB,     // -3 dB
    AUDIOHAL_TONE_M9DB,     // -9 dB
    AUDIOHAL_TONE_M15DB,    // -15 dB
    AUDIOHAL_TONE_QTY
} AUDIOHAL_TONE_ATTENUATION_T;


//ICAT EXPORTED ENUM
typedef enum{
    AUDIOHAL_HEADPHONE_PLUG_OUT = 0,
    AUDIOHAL_HEADPHONE_PLUG_IN	= 1
}AUDIOHAL_HEADPHONE_PLUG_T;


//ICAT EXPORTED ENUM
typedef enum{
    AUDIOHAL_HEADPHONE_TYPE_NULL 	= 0,
	AUDIOHAL_HEADPHONE_TYPE_UNKNOWN = 1,

    AUDIOHAL_HEADPHONE_TYPE_3 		= 3,	//HEAD_PHONE_TYPE3
    AUDIOHAL_HEADPHONE_TYPE_4 		= 4		//HEAD_PHONE_TYPE4
}AUDIOHAL_HEADPHONE_TYPE_T;


//ICAT EXPORTED ENUM
typedef enum{
    AUDIOHAL_HEADPHONE_EVENT_NULL		= 0,
    AUDIOHAL_HEADPHONE_EVENT_VOLUME_UP 	= 1,
    AUDIOHAL_HEADPHONE_EVENT_VOLUME_DOWN= 2,
    AUDIOHAL_HEADPHONE_EVENT_HOOK_KEY	= 3,
    
    AUDIOHAL_HEADPHONE_EVENT_TYPE_C_REVERSE	= 7
    
}AUDIOHAL_HEADPHONE_EVENT_T;

//ICAT EXPORTED ENUM
typedef enum {
    AUDIOHAL_PORT_VOICE = 0,
    AUDIOHAL_PORT_SSPA = AUDIOHAL_PORT_VOICE,
    AUDIOHAL_PORT_HW_SSP0 = 1,
    AUDIOHAL_PORT_SSP1 = AUDIOHAL_PORT_HW_SSP0,
    AUDIOHAL_PORT_HW_SSP1 = 2,
    AUDIOHAL_PORT_SSP2 = AUDIOHAL_PORT_HW_SSP1,
    AUDIOHAL_PORT_HW_SSP2 = 3,
    AUDIOHAL_PORT_SSP3 = AUDIOHAL_PORT_HW_SSP2,

    AUDIOHAL_PORT_QTY
}AUDIOHAL_DEVICE_PORT_T;


typedef void (*AUDIOHAL_HANDLER_T) (void);
typedef void (*AUDIOHAL_HeadsetReport_T) (UINT32 plug, UINT32 type, UINT32 event);
typedef void (*AUDIOHAL_SpeakerPA_T) (UINT32 on);
typedef void (*AUDIOHAL_Codec_T) (UINT32 on);
typedef void (*AUDIOHAL_BT_CALLBACK_T) (UINT32 on);

typedef void (*AUDIOHAL_KWS_CB_T)(unsigned int key);

typedef int  (*AUDIO_EXTRA_CB)(short* dataPtr, UINT16 dataSize);


//ICAT EXPORTED STRUCT
typedef struct {
    UINT32 *            startAddress;
    UINT16              length;

    AUDIOHAL_FREQ_T     sampleRate;
    AUDIOHAL_CH_NB_T    channelNb;
    //BOOL                voiceQuality;
    //BOOL                playSyncWithRecord;
    INT                voiceQuality;
    INT                playSyncWithRecord;

    AUDIOHAL_HANDLER_T  halfHandler;
    AUDIOHAL_HANDLER_T  endHandler;
} AUDIOHAL_STREAM_T;

typedef struct {
	BOOL				isVoice;
	BOOL				isMaster;
    AUDIOHAL_FREQ_T     samplerate;
    AUDIOHAL_CH_NB_T    channelNb;
} AUDIOHAL_AIF_CONFIG;


typedef struct {
    AUDIOHAL_SPK_LEVEL_T        spkLevel;
    AUDIOHAL_MIC_LEVEL_T        micLevel;
    AUDIOHAL_SIDE_LEVEL_T       sideLevel;
    AUDIOHAL_TONE_ATTENUATION_T toneLevel;
} AUDIOHAL_AIF_LEVEL_T;


typedef struct {
    AUDIOHAL_SPK_T          spkSel;
    AUDIOHAL_SPEAKER_TYPE_T spkType;    //Kind of speaker(stereo/mono/etc)
    AUDIOHAL_MIC_T          micSel;
    AUDIOHAL_AIF_LEVEL_T  * level;
} AUDIOHAL_AIF_DEVICE_CFG_T;




///////////////////////Control API for MMI///////////////////////////
#if 0
typedef union
{
    UINT32 reg;
    struct
    {
        BOOL auxMicSel:1;
        BOOL spkSel:1;
        BOOL spkmonoSel:1;
        BOOL rcvSel:1;
        BOOL headSel:1;
        BOOL fmSel:1;
    };
} AUDIOHAL_AUD_SEL_T;

typedef union
{
    UINT32 reg;
    struct
    {
        UINT32  MicGain:4;
        BOOL    MicMute:1;
    };
} AUDIOHAL_MIC_CFG_T;


typedef union
{
    UINT32 reg;
    struct
    {
        UINT32  SpkGain:4;
        BOOL    SpkMute:1;
    };
} AUDIOHAL_SPK_CFG_T;

BOOL AudioHAL_AudOpen(unsigned int user);
BOOL AudioHAL_AudClose(void);
BOOL AudioHAL_AudEarpieceDetectInit(void);
void AudioHAL_AudForceReceiverMicSelection(BOOL on);
void AudioHAL_AudForcePowerDown(void);
void AudioHAL_AudSetVoiceMode(BOOL isVoiceMode);
void AudioHAL_AudSetAdcVolume(UINT32 vol);
void AudioHAL_AudSetDacVolume(UINT32 vol);
void AudioHAL_AudSetHeadGain(UINT32 gain);
void AudioHAL_AudSetRcvGain(UINT32 gain);
void AudioHAL_AudSetMicCfg(UINT32 cfg);
void AudioHAL_AudSetSpkCfg(UINT32 cfg);
void AudioHAL_AudSetSelcfg(UINT32 selCfg);
AUDIOHAL_SPK_CFG_T AudioHAL_AudGetSpkCfg(void);
UINT32 AudioHAL_AudGetHeadGain(void);
UINT32 AudioHAL_AudGetRcvGain(void);
BOOL AudioHAL_AudMuteOutput(BOOL mute);
void AudioHAL_AudLcdPowerOnCallback(void);
void AudioHAL_AudLcdPowerOffCallback(void);
#endif


///////////////////////Control API for CP Audio///////////////////////////
int AudioHAL_Init(void);
void AudioHAL_TaskInit(void);
void AudioHAL_ReloadFDI(void);
int AudioHAL_UpdateCodecGain(void);
void AudioHAL_HeadsetOps(UINT16 plug, UINT16 state, UINT16 event);
void AudioHAL_StreamStart(BOOL onoff);



///////////////////////Control API for MMI///////////////////////////
AUDIOHAL_ERR_T AudioHAL_AifLoopBack(unsigned char loop);
AUDIOHAL_ERR_T AudioHAL_AifConfigure(AUDIOHAL_AIF_CONFIG * config);
AUDIOHAL_ERR_T AudioHAL_AifOpen(AUDIOHAL_ITF_T itf, AUDIOHAL_AIF_DEVICE_CFG_T *config);
AUDIOHAL_ERR_T AudioHAL_AifTone(AUDIOHAL_TONE_TYPE_T tone, AUDIOHAL_TONE_ATTENUATION_T attenuation, BOOL start);
AUDIOHAL_ERR_T AudioHAL_AifTonePause(BOOL pause);
AUDIOHAL_ERR_T AudioHAL_AifDTMF(BOOL start, int f1, int f2);


void AudioHAL_AifClose(void);
void AudioHAL_AifHeadsetDetection(unsigned char input);
void AudioHAL_AifBindHeadsetDetectionCB(AUDIOHAL_HeadsetReport_T cb);
void AudioHAL_AifBindSpeakerPA_CB(AUDIOHAL_SpeakerPA_T cb);
void AudioHAL_AifBindCodec_CB(AUDIOHAL_SpeakerPA_T cb);
void AudioHAL_AifGetHeadsetInfo(UINT32* plug, UINT32* type);
void AudioHAL_AifSetVolume(UINT32 vol);
void AudioHAL_AifSetMute(BOOL onoff);
void AudioHAL_AifSetDspRxMute(BOOL onoff);
void AudioHAL_AifSetSideTone(UINT32 vol);
void AudioHAL_AifSwitch(AUDIOHAL_ITF_T itf);

UINT32 AudioHAL_AifGetVolume(void);
AUDIOHAL_ITF_T AudioHAL_AifGetOpenedDevice(void);



///////////////////////Stream API for MMI///////////////////////////
AUDIOHAL_ERR_T AUDIOHAL_GetBufferSize(AUDIOHAL_FREQ_T rate, AUDIOHAL_CH_NB_T chan, int* min, int* max);
AUDIOHAL_ERR_T AudioHAL_AifPlayStream(AUDIOHAL_STREAM_T *playedStream);
AUDIOHAL_ERR_T AudioHAL_AifRecordStream(AUDIOHAL_STREAM_T *recordedStream);
AUDIOHAL_ERR_T AudioHAL_AifPause(BOOL pause);
void AudioHAL_AifStopPlay(void);
void AudioHAL_AifStopRecord(void);

AUDIOHAL_ERR_T AudioHAL_AifDrain(void);

AUDIOHAL_ERR_T AudioHAL_SetResBufCnt(unsigned int bufCnt);

UINT16 AudioHAL_vibration_setGain(UINT16 gain);
void AudioHAL_vibration_control(UINT16 onoff);
void AudioHAL_spkProSet(void);

int AudioHal_AifPlay2Farend(int farend);


/**********************************************************************
 *
 * Function: AudioHAL_DRCReset
 *
 * Description: set media stream drc and cutoff freq
 *
 * gain:  
   index:   
    0:  cutoff freq - 200Hz
    1:  cutoff freq - 400Hz
    2:  cutoff freq - 500Hz
    3:  cutoff freq - 700Hz
    n(n>3, n <= 1500): cutoff freq - n
    N(N>1500): cutoff freq - 1500
    

 *  return value: commit gain
 
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 12-Jul-2020	ChaoLiu                          Initial version
 **********************************************************************/
UINT16 AudioHAL_DRCReset(UINT16 gain, UINT16 index);
void AudioHAL_set_close_delay(unsigned int cnt);

void AudioHAL_FadingControl(int on);
signed short AudioHAL_SetFadingStep(signed short step);



/**********************************************************************
 *
 * Function: AudioHAL_AifSetPhoneStatus
 *
 * Description: set phone voice call status 
 *
 * status:  
    0:  let audio hal decide Voice Call status
    1:  in Media Audio play status
    2:  in Voice Call status

 *  return value: 
    AUDIOHAL_ERR_NO: success
    AUDIOHAL_ERR_BAD_PARAMETER: status large than 2
 
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 22-Oct-2004	yg                          Initial version
 **********************************************************************/
AUDIOHAL_ERR_T AudioHAL_AifSetPhoneStatus(unsigned int status);

/**********************************************************************
 *
 * Function: AudioHAL_SetUsedMIC
 *
 * Description: set used mic
 *
 * mic:  
    0(default):  let audio hal decide which mic to use  
    1:  use mic1
    2:  use mic2

 *  return value: 
    0: success
    -1:large than 2 
 *  NOTE:  need to disable Headset detection
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_SetUsedMIC(int mic);

int AudioHAL_sspaSetFormat(unsigned short pcmFormat) ;
int AudioHAL_sspaSetFormat2(unsigned short pcmFormat2) ;

/**********************************************************************
 *
 * Function: AudioHAL_sspaSetDelay
 *
 * Description: set PCM interface SSPA's data delay
 *
 * delay:  
    0: no delay;  
    1: 1 bitclk delay; (default) 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetDelay(unsigned int delay) ;


/**********************************************************************
 *
 * Function: AudioHAL_sspaSetSampleRate
 *
 * Description: set PCM interface SSPA's sample Rate
 *
 * rate:  
    1: 8k; 
    2: 16k; 
    6: 48k

 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetSampleRate(unsigned int rate);


/**********************************************************************
 *
 * Function: AudioHAL_sspaSetBCLKType
 *
 * Description: set PCM interface SSPA's bitclk frequency
 *
 * type:  
    0: 32fs; 
    1: 64fs; 
    2: 128fs; 
    3: 256fs; 
    4: 512fs; 
    5: 1024fs;  
    6: 2048fs; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetBCLKType(unsigned int type) ;

/**********************************************************************
 *
 * Function: AudioHAL_sspaSetBCLKInvert
 *
 * Description: set PCM interface SSPA's bitclk INVERT
 *
 * invert:  
    0: keep default; 
    1: invert; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/;
int AudioHAL_sspaSetBCLKInvert(unsigned int invert) ;


/**********************************************************************
 *
 * Function: AudioHAL_sspaSetI2S
 *
 * Description: set PCM interface SSPA's PCM/I2S format
 *
 * i2s:  
    0: PCM; 
    1: I2S; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetI2S(unsigned int i2s) ;
/**********************************************************************
 *
 * Function: AudioHAL_btsco_formatSet
 *
 * Description: set PCM format for BT SCO (PCM)
 *
 * slave:  
    0: sspa as master; 
    1: sspa as slave; 
 * delay:  
    0: no delay;  
    1: 1 bitclk delay; (default) 
 * invert:  
    0: keep default; 
    1: invert; 
 * rsv:  for future usage

 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_btsco_formatSet(unsigned int slave, unsigned int delay, unsigned int invert, void* rsv);

/**********************************************************************
 *
 * Function: AudioHAL_sspaSetSlave
 *
 * Description: set PCM interface SSPA's slave mode
 *
 * slave:  
    0: sspa as master(default)  
    1: sspa as slave 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 29-NOV-2021	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetSlave(unsigned int slave) ;

/**********************************************************************
 *
 * Function: AudioHAL_sspaSetSyncClkInvert
 *
 * Description: set PCM interface SSPA's Sync Clock  INVERT
 *
 * invert:  
    0: keep default; 
    1: invert; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 29-NOV-2021	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetFrameClkInvert(unsigned int invert) ;

/**********************************************************************
 *
 * Function: AudioHAL_sspaSetSyncClkWidth
 *
 * Description: set PCM interface SSPA's sync clock  width
 *
 * length: 1~0x10 
    1: sync frame  width is  1 bitclk;  
    2: sync frame  width is  2 bitclk; 
    ..........................
    0x10: sync frame  width is  16 bitclk; 


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 29-NOV-2021	Chaoliu                      Initial version
 **********************************************************************/
int AudioHAL_sspaSetFrameClkWidth(unsigned int width);



void AudioHAL_AifSetTxDSPGain(signed char db);
char AudioHAL_AifGetTxDSPGain(void);
void AudioHAL_AifSetTxCodecGain(short pgaStage1, short pgaStage2);
short AudioHAL_AifGetTxCodecGainStage1(void);
short AudioHAL_AifGetTxCodecGainStage2(void);


void AudioHAL_NeedChargePump(int need);
int AudioHAL_EnableHSDetect(int isNormalClosed);

int AudioHAL_AifSet_VAD(int on);
int AudioHAL_AifGet_VAD(void);
int AudioHAL_AifBindKWS_CB(AUDIOHAL_KWS_CB_T cb);


int AudioHAL_AifControlMCLK(unsigned int type, unsigned int on);

/**********************************************************************
 *
 * Function: AudioHAL_AifSetFMCodecGain
 *
 * Description: set FM codec DAC gain
 *
 * gainIdx:  
    0: 6db;  
    1: 3db;  
    2: 0db;  
    3: -3db;  
    4: -6db; 
    
 typedef enum
 {
     ACMCODEC_DAC_GAIN_6dB   = 0,    
     ACMCODEC_DAC_GAIN_3dB,
     ACMCODEC_DAC_GAIN_0dB,
     ACMCODEC_DAC_GAIN_M3dB,
     ACMCODEC_DAC_GAIN_M6dB,
 
     ACMCODEC_DAC_GAIN_CNT,
 
     ACMCODEC_DAC_GAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
 } ACMCODEC_DAC_GAIN;


 *  return value: 
    0: success
    -1:fail 
 *  NOTE:  
 *
 * --------------------------------------------------------------------
 * Revision History
 *
 * Date         Who        Version           Description
 * --------------------------------------------------------------------
 * 7-April-2020 Chaoliu                      Initial version
 **********************************************************************/
void AudioHAL_AifSetFMCodecGain(unsigned char gainIdx);

AUDIOHAL_ERR_T AudioHAL_AifSetDevicePort(AUDIOHAL_DEVICE_PORT_T port);
AUDIOHAL_DEVICE_PORT_T AudioHAL_AifGetDevicePort(void);
int AudioHAL_start_bt_hfp_hfMode(int start);

#ifdef __cplusplus
}
#endif
#endif  /* _AUDIOHAL_H_ */
