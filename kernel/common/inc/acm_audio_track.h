/******************************************************************************
 *
 *  (C)Copyright ASRMicro. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ASRMicro.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of ASRMicro and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of ASRMicro.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /**
    * @file acm_audio_track.h
    * @brief ASR audio track API describes the process and functions for PCM playback.
    */

    /** audio track pcm format supported*/
    typedef enum audio_track_format {
        /** Signed, 8-bit */
        AUDIO_TRACK_FORMAT_S8 = 1,
        /** Signed 16-bit, little endian */
        AUDIO_TRACK_FORMAT_S16_LE = 0,
        /** Signed, 16-bit, big endian */
        AUDIO_TRACK_FORMAT_S16_BE = 2,
        /** float point 32-bit */
        AUDIO_TRACK_FORMAT_F32,
        /** Unsigned 8-bit,a-law */
        AUDIO_TRACK_FORMAT_U8_ALAW,
        /** Unsigned 8-bit,u-law */
        AUDIO_TRACK_FORMAT_U8_ULAW,

        AUDIO_TRACK_FORMAT_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_format_t;

    /** audio track event to indicate the internal playback status*/
    typedef enum audio_track_event {
        /** dummy event */
        AUDIO_TRACK_EVENT_NONE,
        /** tick event, 20 ms typically */
        AUDIO_TRACK_EVENT_TICK,
        /** start event, current track is involved in schedule*/
        AUDIO_TRACK_EVENT_STARTED,
        /** waiting event, current track is preempted by other track*/
        AUDIO_TRACK_EVENT_WAITING,
        /** running event, current track is recovered from waiting*/
        AUDIO_TRACK_EVENT_RUNNING,
        /** underrun event, current track has played out all the pcm data*/
        AUDIO_TRACK_EVENT_UNDERRUN,
        /** overrun event, current track can not hold more pcm data*/
        AUDIO_TRACK_EVENT_OVERRUN,
        /** fast up event, current track need more pcm data*/
        AUDIO_TRACK_EVENT_NEARLY_UNDERRUN,
        AUDIO_TRACK_EVENT_FASTUP = AUDIO_TRACK_EVENT_NEARLY_UNDERRUN,
        /** slow down event, current track need less pcm data*/
        AUDIO_TRACK_EVENT_NEARLY_OVERRUN,
        AUDIO_TRACK_EVENT_SLOWDOWN = AUDIO_TRACK_EVENT_NEARLY_OVERRUN,
        /** close event, current track is closed*/
        AUDIO_TRACK_EVENT_CLOSED,
        /** draining event, current track is in draining*/
        AUDIO_TRACK_EVENT_DRAINING,
        /** terminate event, current track is terminated in an unexpected way*/
        AUDIO_TRACK_EVENT_TERMINATED,
        /** consume event, current track has consumed vendor buffer*/
        AUDIO_TRACK_EVENT_CONSUMED,

        AUDIO_TRACK_EVENT_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_event_t;

    /** audio track schedule mode*/
    typedef enum audio_track_mode {
        /** combine mode, current track will mix with other tracks which have the same mode*/
        AUDIO_TRACK_MODE_COMBINE,
        /** exclusive mode, current track will not mix with other tracks, can be preempted*/
        AUDIO_TRACK_MODE_EXCLUSIVE,
        /** preempt mode, current track will not mix with other tracks, can not be preempted**/
        AUDIO_TRACK_MODE_PREEMPT,
        /** terminate mode, current track will mix with other tracks which have the same mode, terminate others if possible*/
        AUDIO_TRACK_MODE_TERMINATE,

        AUDIO_TRACK_MODE_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_mode_t;

    /** audio track direction to specify near or far or both*/
    typedef enum audio_track_direction {
        /** direction near, current track will play to near side*/
        AUDIO_TRACK_DIRECTION_NEAR,
        AUDIO_TRACK_DIRECTION_SPEAKER = AUDIO_TRACK_DIRECTION_NEAR,
        /** direction far, current track will play to far side*/
        AUDIO_TRACK_DIRECTION_FAR,
        AUDIO_TRACK_DIRECTION_MIC = AUDIO_TRACK_DIRECTION_FAR,
        /** direction both, current track will play to near and far side*/
        AUDIO_TRACK_DIRECTION_BOTH,

        AUDIO_TRACK_DIRECTION_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_direction_t;

    /** audio track control supported*/
    typedef enum audio_track_control {
        /** suspend current track*/
        AUDIO_TRACK_CTRL_SUSPEND,
        /** resume current track*/
        AUDIO_TRACK_CTRL_RESUME,
        /** mute on current track*/
        AUDIO_TRACK_CTRL_MUTE_ON,
        /** mute off current track*/
        AUDIO_TRACK_CTRL_MUTE_OFF,
        /** extra gain for current track*/
        AUDIO_TRACK_CTRL_GAIN,
        /** ramp level for current track*/
        AUDIO_TRACK_CTRL_RAMP,
        /** private data for current track*/
        AUDIO_TRACK_CTRL_PRIVATE,

        AUDIO_TRACK_CTRL_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_control_t;

    /** audio track interaction method supported*/
    typedef enum audio_track_write_mode {
        /** asynchronous write mode, fail when too much pcm data to copy*/
        AUDIO_TRACK_WRITE_MODE_ASYNC,
        /** synchronous write mode, block when too much pcm data to copy*/
        AUDIO_TRACK_WRITE_MODE_SYNC,
        /** asynchronous vendor mode, do not block, do not fail*/
        AUDIO_TRACK_WRITE_MODE_VENDOR,

        AUDIO_TRACK_WRITE_MODE_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_track_write_mode_t;

    /** audio track handle, held and used by audio track user*/
    typedef uint32_t acm_audio_track_handle;
    /** audio track event callback, inform audio track user of internal state*/
    typedef void(*acm_audio_track_event_callback_t)(acm_audio_track_handle, acm_audio_track_event_t);
    /** audio track handle, held and used by audio track user*/
    typedef acm_audio_track_handle audio_track_handle;
    /** audio track event callback, inform audio track user of internal state*/
    typedef void(*audio_track_callback)(audio_track_handle, enum audio_track_event);

    /** audio track configuration to specify playback property*/
    typedef struct audio_track_config {
        /** pcm channel, non-zero expected*/
        uint32_t channels;
        /** pcm sample rate, non-zero expected*/
        uint32_t rate;
        /** audio track capacity, how many microsecond of pcm can be held*/
        uint32_t capacity;
        /** audio track start threshold, how many microsecond of pcm is held before involving in schedule*/
        uint32_t start_threshold;
        /** audio track prepare threshold, how many microsecond of pcm is held before playing out*/
        uint32_t prepare_threshold;
        /** initial gain in q8 format, 0db by default*/
        int32_t gain_value;
        /** initial speed factor in q8 format, x1 by default*/
        int32_t speed_factor;
        /** eq effect index, 0 for none, minus 1 for lookup*/
        int32_t eq;
        /** playback property in bitmap format, refer @ref AUDIO_PLAY_OPTION*/
        int64_t option;
        /** enhance in dsp further more if any*/
        int32_t dsp_mode;
        /** override voice call data if any*/
        int32_t override_voice;
        /** pcm format, 16 bit depth little endian by default*/
        acm_audio_track_format_t format;
        /** schedule mode, combine mode by default*/
        acm_audio_track_mode_t mode;
        /** audio track direction, near side by default*/
        acm_audio_track_direction_t direction;
        /** audio track event callback*/
        acm_audio_track_event_callback_t event_cb;
        /** audio track write mode, 0 for non-block*/
        acm_audio_track_write_mode_t write_mode;
        /** turn off audio effect for gain etc*/
        uint32_t effect_off;
    }acm_audio_track_config_t;

    /** Allocates and initializes an audio track handle.
    * @param [in] config               configuration from audio track user
    * @param [in,out] handle           audio track handle
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_open(const acm_audio_track_config_t* config, acm_audio_track_handle* handle);

    /** Write pcm data to an audio track handle.
    * @param [in] handle               audio track handle
    * @param [in] data                 pcm buffer address
    * @param [in] size                 pcm buffer size
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_write(acm_audio_track_handle handle, const uint8_t* data, uint32_t size);

    /** Set control to an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] ctrl                  audio track control
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_control(acm_audio_track_handle handle, acm_audio_track_control_t ctrl);

    /** Set software gain to an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] gain_value            software gain value
    * @returns error code in int, non-zero on failure
    * @note gain_value has q8 format!
    */
    int acm_audio_track_set_softgain(acm_audio_track_handle handle, int32_t gain_value);

    /** Get software gain of an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in,out] gain_value        software gain value
    * @returns error code in int, non-zero on failure
    * @note gain_value has q8 format!
    */
    int acm_audio_track_get_softgain(acm_audio_track_handle handle, int32_t* gain_value);

    /** Get frame size of an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in,out] frame_size        frame size
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_get_framesize(acm_audio_track_handle handle, uint32_t* frame_size);

    /** Get available size for writing of an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in,out] available_size    available size for writing
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_get_available(acm_audio_track_handle handle, uint32_t* available_size);

    /** Close and free memory etc resource of an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] drain                 drain or not
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_track_close(acm_audio_track_handle handle, int32_t drain);

    /** Load audio track global eq params.
    * @param [in] index                eq index
    * @param [in] pparam               eq params
    * @param [in] size                 size of eq params
    * @note for audio track user do @b not call this api!
    */
    void acm_audio_track_load_eq(int32_t index, const void* pparam, int32_t size);

    /** Load audio track global gain setting.
    * @param [in] gain                  global gain in q8 format
    * @note for audio track user do @b not call this api!
    */
    void acm_audio_track_load_gain(int32_t gain);

    /** Load audio track drc setting.
    * @param [in] enable                enable drc or not
    * @note for audio track user do @b not call this api!
    */
    void acm_audio_track_load_drc(int32_t enable);

    /** Set audio track debug option.
    * @param [in] debug_option          turn on more debug print or not
    */
    void acm_audio_track_debug(int32_t debug_option);

    /** Init audio track resource.
    * @note for audio track user do @b not call this api!
    */
    void acm_audio_track_init(void);

    /** Configure audio track multimedia format.
    * @note for audio track user do @b not call this api!
    */
    void acm_audio_track_config(uint32_t hw_rate, uint32_t hw_channel);

    /** Allocates and initializes an audio track handle, use control words rather than @ref acm_audio_track_config_t.
    * @param [in] rate                  stream out sample rate
    * @param [in] channels              stream out channels
    * @param [in] option                stream out property, refer @ref AUDIO_PLAY_OPTION
    * @param [in] event_cb              stream out status listener
    * @returns stream out handle @ref audio_track_handle
    */
    audio_track_handle audio_track_open(uint32_t rate, uint32_t channels, int64_t option, audio_track_callback event_cb);

    /** Write pcm data to an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] data                  pcm buffer address
    * @param [in] size                  pcm buffer size
    * @returns error code in int, non-zero on failure
    */
    int audio_track_write(audio_track_handle handle, const uint8_t* data, uint32_t size);

    /** Close and free memory etc resource of an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] force                 stop immediately or not
    * @returns error code in int, non-zero on failure
    */
    int audio_track_close(audio_track_handle handle, int32_t force);

    /** Set control to an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] ctrl_type             control type
    * @param [in] ctrl_value            control value
    * @returns error code in int, non-zero on failure
    */
    int audio_track_ctrl(audio_track_handle handle, enum audio_track_control ctrl_type, int32_t ctrl_value);

    /** Start wav format file playback.
    * @param [in] file_name             wav file name
    * @param [in] config                wav playback property
    * @param [in,out] handle            audio track handle
    * @returns error code in int, non-zero on failure
    */
    int wave_playback_start(const char* file_name, const struct audio_track_config* config, audio_track_handle* handle);

    /** Stop wav file playback with an audio track handle.
    * @param [in] handle                audio track handle
    * @param [in] force                 stop immediately or not
    * @returns error code in int, non-zero on failure
    */
    int wave_playback_stop(audio_track_handle handle, int32_t force);

#ifdef __cplusplus
}
#endif
