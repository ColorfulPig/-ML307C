/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2020 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/
#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>
#include "acm_audio_def.h"

    /**
    * @file mp3dec_api.h
    * @brief ASR mp3 related API describes the process and functions used to play mp3 file and stream on ASR RTOS platform.
    */

    typedef enum Mp3PlayEventValue {
        /** play out all for file or stream*/
        MP3_PLAYBACK_STATUS_ENDED = 0,
        /** indicate current playback is started*/
        MP3_PLAYBACK_STATUS_STARTED,
        /** indicate output device is opened*/
        MP3_PLAYBACK_STATUS_OPENED,
        /** reach file end*/
        MP3_PLAYBACK_STATUS_FILE_READED,

        MP3_PLAYBACK_STATUS_STREAM = 100,
        /** indicate mp3 data producer should slow down to avoid overrun*/
        MP3_STREAM_STATUS_NEARLY_OVERRUN,
        MP3_STREAM_STATUS_SLOW_DOWN = MP3_STREAM_STATUS_NEARLY_OVERRUN,
        /** indicate mp3 data producer should fast up to avoid underrun*/
        MP3_STREAM_STATUS_NEARLY_UNDERRUN,
        MP3_STREAM_STATUS_FAST_UP = MP3_STREAM_STATUS_NEARLY_UNDERRUN,
    }Mp3PlayEventValue;

    typedef enum Mp3PlayEventType {
        /** mp3 playback status, value with type <tt>Mp3PlayEventValue</tt>*/
        MP3_PLAYBACK_EVENT_STATUS,
        /** id3 header size, value with type <tt>int<tt> */
        MP3_FILE_EVENT_ID3OFFSET,
        /** sample rate, value with type <tt>int<tt>*/
        MP3_FILE_EVENT_SAMPLERATE,
        /** channel, value with type <tt>int<tt>*/
        MP3_FILE_EVENT_CHANNEL,
        /** bit rat, value with type <tt>int<tt>*/
        MP3_FILE_EVENT_BITRATE,
        /** file name request, value with type <tt>char*<tt>*/
        MP3_FILE_EVENT_FILENAME,
    }Mp3PlayEventType;

    /** mp3 playback event callback prototype*/
    typedef void(*Mp3PlaybackEventCallback)(Mp3PlayEventType, int);
    /** mp3 playback handle, held and used by mp3 playback user*/
    typedef uint32_t Mp3PlaybackHandle;

    typedef struct Mp3PlayConfigInfo {
        /** mp3 playback property in bitmap format, refer <tt>AUDIO_PLAY_OPTION</tt> in acm_audio_def.h*/
        int64_t option;
        /** block size for file only, 0 for frame mode*/
        int block_size;
        /** how many mp3 frames will be skipped before playback*/
        int skip_frames;
        /** how many mp3 frames will be held before playback*/
        int cache_frames;
        /** user registers callback to get playback event such as file end etc */
        Mp3PlaybackEventCallback listener;
        /** start with sync mode*/
        int sync_mode;
    }Mp3PlayConfigInfo;

    typedef Mp3PlayConfigInfo Mp3StreamConfigInfo;

    typedef struct Mp3StreamDataInfo {
        /** mp3 playback handle held by user, used to send following mp3 data to corresponding mixer instance*/
        Mp3PlaybackHandle handle;
        /** user space mp3 data ptr*/
        uint8_t* buf;
        /** user space mp3 data size*/
        uint32_t size;
    }Mp3StreamDataInfo;

    /** Start mp3 file playback with default property.
    * @param [in] name <tt>const char*</tt>: mp3 file name
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3Start(const char* file_name);

    /** Stop all the mp3 file playbacks ongoing.
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3Stop(void);

    /** Start mp3 file playback with dedicated property.
    * @param [in] name <tt>const char*</tt>: mp3 file name
    * @param [in] configInfo <tt>const Mp3PlayConfigInfo*</tt>: mp3 file playback settings
    * @param [in,out] handle <tt>Mp3PlaybackHandle*</tt>: mp3 file playback handle
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3PlayStart(const char* file_name, const Mp3PlayConfigInfo* configInfo, Mp3PlaybackHandle* handle);

    /** Stop mp3 file playback with handle.
    * @param [in] handle <tt>Mp3PlaybackHandle</tt>: mp3 file playback handle
    * @param [in] drain <tt>int</tt>: drain or not
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3PlayStop(Mp3PlaybackHandle handle, int drain);

    /** Stop mp3 file playback with file name.
    * @param [in] file_name <tt>const char*</tt>: mp3 file name
    * @param [in] drain <tt>int</tt>: drain or not
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3PlayStopWithName(const char* file_name, int drain);

    /** Start mp3 stream playback with dedicated option.
    * @param [in] configInfo <tt>const Mp3PlayConfigInfo*</tt>: mp3 stream playback option
    * @param [in,out] handle <tt>Mp3PlaybackHandle*</tt>: mp3 stream playback handle
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3StreamStart(const Mp3StreamConfigInfo* configInfo, Mp3PlaybackHandle* handle);

    /** Send one mp3 data block from mp3 stream data producer.
    * @param [in] dataInfo <tt>const Mp3StreamDataInfo*</tt>: mp3 data
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3StreamPlayBuffer(const Mp3StreamDataInfo* dataInfo);

    /** Stop mp3 stream playback with handle.
    * @param [in] handle <tt>Mp3PlaybackHandle</tt>: mp3 stream playback handle
    * @param [in] drain <tt>int</tt>: drain or not
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3StreamStop(Mp3PlaybackHandle handle, int drain);

    /** Get audio track handle, which is used to manipulate pcm stream.
    * @param [in] handle <tt>Mp3PlaybackHandle</tt>: mp3 playback handle
    * @param [in] pcm_handle <tt>uint32_t*</tt>: pcm stream out handle
    * @returns error code in <tt>int</tt>, non-zero on failure
    */
    int mp3GetPCMOutHandle(Mp3PlaybackHandle handle, uint32_t* pcm_handle);

    /** Init mp3 playback resource.
    * @note for mp3 playback user do @b not call this api!
    */
    void mp3PlayInit(void);

#ifdef __cplusplus
}
#endif
