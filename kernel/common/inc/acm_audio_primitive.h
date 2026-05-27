/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2020 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "gbl_types.h"


    //ICAT EXPORTED ENUM
    typedef enum
    {
        ACM_RC_OK = 1,
        ACM_RC_DEVICE_ALREADY_ENABLED,
        ACM_RC_DEVICE_ALREADY_DISABLED,
        ACM_RC_NO_MUTE_CHANGE_NEEDED,
        ACM_RC_INVALID_VOLUME_CHANGE,
        ACM_RC_DEVICE_NOT_FOUND,
        ACM_RC_BUFFER_GET_FUNC_INVALID,
        ACM_RC_STREAM_OUT_NOT_PERFORMED,
        ACM_RC_STREAM_IN_NOT_PERFORMED,
        ACM_RC_STREAM_OUT_TO_BE_STOPPED_NOT_ACTIVE,
        ACM_RC_STREAM_IN_TO_BE_STOPPED_NOT_ACTIVE,
        ACM_RC_I2S_INVALID_DATA_POINTER,
        ACM_RC_I2S_INVALID_DATA_SIZE,
        ACM_RC_I2S_INVALID_NOTIFICATION_THRESHOLD,
        ACM_RC_I2S_MESSAGE_QUEUE_IS_FULL,
        ACM_RC_MEMORY_ALREADY_INITIALISED,

        ////Jackie add
        ACM_RC_NEED_DISABLE_PATH,	// need APPS to disable codec path
        ACM_RC_MALLOC_ERROR,        // memory error
        ACM_RC_OUTOF_RANGE,

        /* Must be at the end */
        ACM_RC_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    } ACM_ReturnCode;

    /*0x01 Volume Control Message*/
    ACM_ReturnCode ATCVolumeCtrl(UINT8  direction,
        UINT32 gain,
        UINT32 misc,
        UINT32 confirm_id);

    /*0x02 Mute Control Message*/
    ACM_ReturnCode ATCMuteCtrl(UINT8  direction,
        UINT8  mute,
        UINT32 confirm_id);

    /*0x03 Path Control Message*/
    ACM_ReturnCode ATCPathCtrl(UINT32 path,
        UINT32 confirm_id);

    /*0x04 EQ Control Message*/
    ACM_ReturnCode ATCEQCtrl(UINT16 para[15],
        UINT32 confirm_id);

    /*0x05 Loopback Control Message*/
    ACM_ReturnCode ATCLoopbackCtrl(UINT8  loopback_test_mode,
        UINT32 path,
        UINT32 confirm_id);

    /*0x06 PCM Recording Control Message*/
    ACM_ReturnCode ATCPCMRecCtrl(UINT8  on_off,
        UINT8  near_far_end,
        UINT8  near_codec_or_vocoder,
        UINT32 callback,
        UINT32 confirm_id);

    /*0x07 PCM Playback Control Message*/
    ACM_ReturnCode ATCPCMPlayCtrl(UINT8  on_off,
        UINT8  near_far_end,
        UINT8  near_codec_or_vocoder,
        UINT8  comb_with_call,
        UINT32 callback,
        UINT32 confirm_id);

    /*0x0a Ecall Control Message*/
    ACM_ReturnCode ATCECallCtrl(char*  ecall_data,
        unsigned long  ecall_len,
        UINT32 confirm_id);

    /*0x0b PCM Control Message*/
    ACM_ReturnCode ATCPCMCtrl(UINT16    on_off,
        UINT32 confirm_id);

    /*0x0c DTMF detection Control Message*/
    ACM_ReturnCode ATCDTMFDetectionCtrl(UINT16 onoff,
        UINT16 dialToneToOthersTones,
        UINT16 dialTonesToOthersDialTones,
        UINT16 dialVadDuration,
        UINT32 callback,
        UINT32 confirm_id);

    /*0x0d PCM Config Message*/
    ACM_ReturnCode ATCPCMCfg(UINT16 pcm,
        UINT32 confirm_id);

    /*0x0e PCM Expert Config Message*/
    ACM_ReturnCode ATCPCMExpertCfg(UINT16 pcm,
        UINT32 confirm_id);

    /*0x0f DTMF Control Message*/
    ACM_ReturnCode ATCDTMFControl(UINT16 onoff,
        UINT16 tone1_index,
        UINT16 tone2_index,
        UINT32 confirm_id);

#ifdef __cplusplus
}
#endif
