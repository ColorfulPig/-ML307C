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
#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

    /**
    * @file acm_audio_record.h
    * @brief ASR audio record API describes the process and functions used for PCM capture.
    */

    /** audio record handle record mode to specify TX or RX direction to be captured*/
    typedef enum audio_record_mode {
        /** Record TX direction pcm from main mic after enhancement*/
        AUDIO_RECORD_MODE_TX,
        AUDIO_RECORD_MODE_NEAR = AUDIO_RECORD_MODE_TX,
        /** Record TX direction pcm from main mic before enhancement*/
        AUDIO_RECORD_MODE_TX_RAW,
        /** Record TX direction pcm from ref mic if any*/
        AUDIO_RECORD_MODE_TX2,
        /** Record RX direction pcm after enhancement*/
        AUDIO_RECORD_MODE_RX,
        AUDIO_RECORD_MODE_FAR = AUDIO_RECORD_MODE_RX,
        /** Record RX direction pcm before enhancement*/
        AUDIO_RECORD_MODE_RX_RAW,
        /** Record mixed pcm of TX and RX*/
        AUDIO_RECORD_MODE_MIXED,
        AUDIO_RECORD_MODE_BOTH = AUDIO_RECORD_MODE_MIXED,
        /** Record mixed pcm of TX RAW and RX*/
        AUDIO_RECORD_MODE_BOTH_CODEC,
        /** Record mixed pcm of RX RAW and TX*/
        AUDIO_RECORD_MODE_BOTH_VOCODER,
        /** Record TX direction pcm from main mic with high quality if possible*/
        AUDIO_RECORD_MODE_TX_HIGH_QUALITY,

        AUDIO_RECORD_MODE_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_record_mode_t;

    /** audio record handle, held and used by audio record user*/
    typedef uint32_t acm_audio_record_handle;
    /** audio record handle, held and used by audio record user*/
    typedef acm_audio_record_handle audio_record_handle;

    /** audio record event to indicate the internal capture status*/
    typedef enum audio_record_event {
        /** dummy event */
        AUDIO_RECORD_EVENT_NONE,
        /** tick event, 20 ms typically*/
        AUDIO_RECORD_EVENT_TICK,
        /** underrun event, current record has no pcm data*/
        AUDIO_RECORD_EVENT_UNDERRUN,
        /** overrun event, current record can not hold more pcm data*/
        AUDIO_RECORD_EVENT_OVERRUN,
        /** close event, current record is closed*/
        AUDIO_RECORD_EVENT_CLOSED,
        /** nearly underrun event, current record will underrun soon*/
        AUDIO_RECORD_EVENT_NEARLY_UNDERRUN,
        /** nearly overrun event, current record will overrun soon*/
        AUDIO_RECORD_EVENT_NEARLY_OVERRUN,

        AUDIO_RECORD_EVENT_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_record_event_t;

    /** audio record control supported*/
    typedef enum audio_record_control {
        /** suspend current record*/
        AUDIO_RECORD_CTRL_SUSPEND,
        /** resume current record*/
        AUDIO_RECORD_CTRL_RESUME,
        /** mute on current record*/
        AUDIO_RECORD_CTRL_MUTE_ON,
        /** mute off current record*/
        AUDIO_RECORD_CTRL_MUTE_OFF,
        /** extra gain for current record*/
        AUDIO_RECORD_CTRL_GAIN,
        /** ramp level for current record*/
        AUDIO_RECORD_CTRL_RAMP,
        /** private data for current record*/
        AUDIO_RECORD_CTRL_PRIVATE,

        AUDIO_RECORD_CTRL_ENUM_32_BIT = 0x7FFFFFFF //32bit enum compiling enforcement
    }acm_audio_record_control_t;

    /** audio record event callback, inform audio record user of internal state*/
    typedef void(*acm_audio_record_event_callback_t)(acm_audio_record_handle, acm_audio_record_event_t);
    /** audio record event callback, inform audio record user of internal state*/
    typedef void(*audio_record_callback)(audio_record_handle, enum audio_record_event);

    /** audio record configuration to specify the capture property*/
    typedef struct audio_record_config {
        /** record mode, TX pcm by default*/
        acm_audio_record_mode_t mode;
        /** audio record event callback*/
        acm_audio_record_event_callback_t event_cb;
        /** pcm channel, non-zero expected*/
        uint32_t channels;
        /** pcm sample rate, non-zero expected*/
        uint32_t rate;
        /** audio record capacity, how many microsecond of pcm can be held*/
        uint32_t capacity;
        /** initial gain in q8 format, 0db by default*/
        int32_t gain_value;
        /** record property in bitmap format, refer @ref AUDIO_RECORD_OPTION*/
        int64_t option;
        /** record timeout in microsecond*/
        int32_t timeout;
        /** turn off audio effect for gain etc*/
        uint32_t effect_off;
        /** do not operate audio path*/
        uint32_t observer;
    }acm_audio_record_config_t;

    /** Allocates and initializes an audio record handle.
    * @param [in] config               configuration from audio record user
    * @param [in,out] handle           audio record handle
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_open(const acm_audio_record_config_t* config, acm_audio_record_handle* handle);

    /** Read one frame of pcm data from an audio record handle.
    * @param [in] handle                audio record handle
    * @param [out] data                 pcm buffer address used to copy pcm frame
    * @param [in,out] size              pcm buffer size, should >= frame size
    * @param [out] frame_id             pcm frame index
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_read(acm_audio_record_handle handle, int16_t* data, uint32_t* size, uint32_t* frame_id);

    /** Get one frame of pcm data in heap from an audio record handle.
    * @param [in] handle                audio record handle
    * @param [out] data_ptr             store pcm frame heap address
    * @param [out] size                 store pcm frame heap size
    * @param [out] frame_id             pcm frame index
    * @returns error code in int, non-zero on failure
    * @note user need to free the buffer!
    */
    int acm_audio_record_get(acm_audio_record_handle handle, int16_t** data_ptr, uint32_t* size, uint32_t* frame_id);

    /** Free pcm data in heap.
    * @param [in] handle                audio record handle
    * @param [out] data_ptr             pcm buffer heap address stored
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_free(acm_audio_record_handle handle, int16_t* data_ptr);

    /** Set control to an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in] ctrl                  audio record control
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_control(acm_audio_record_handle handle, acm_audio_record_control_t ctrl);

    /** Set software gain to an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in] gain_value            software gain value
    * @returns error code in int, non-zero on failure
    * @note gain_value has q8 format!
    */
    int acm_audio_record_set_softgain(acm_audio_record_handle handle, int32_t gain_value);

    /** Get software gain of an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in] gain_value            software gain value
    * @returns error code in int, non-zero on failure
    * @note gain_value has q8 format!
    */
    int acm_audio_record_get_softgain(acm_audio_record_handle handle, int32_t* gain_value);

    /** Get frame size of an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in,out] frame_size        frame size
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_get_framesize(acm_audio_record_handle handle, uint32_t* frame_size);

    /** Get available size for reading of an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in,out] available_size    available size to read
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_get_available(acm_audio_record_handle handle, uint32_t* available_size);

    /** Close and free memory etc resource of an audio record handle.
    * @param [in] handle                audio record handle
    * @returns error code in int, non-zero on failure
    */
    int acm_audio_record_close(acm_audio_record_handle handle);

    /** Set audio record debug option.
    * @param [in] debug_option          turn on more debug print or not
    */
    void acm_audio_record_debug(int32_t debug_option);

    /** Init audio record resource.
    * @note for audio record user do @b not call this api!
    */
    void acm_audio_record_init(void);

    /** Configure audio record multimedia format.
    * @note for audio record user do @b not call this api!
    */
    void acm_audio_record_config(uint32_t hw_rate, uint32_t hw_channel);

    /** Produce pcm raw data for audio records.
    * @note for audio record user do @b not call this api!
    */
    void acm_audio_record_produce(const void* data, uint32_t size);

    /** Produce high quality pcm raw data for audio records.
    * @note for audio record user do @b not call this api!
    */
    void acm_audio_record_produce2(const void* data, uint32_t size);

    /** Allocates and initializes an audio record handle.
    * @param [in] rate                  stream in sample rate
    * @param [in] channels              stream in channels
    * @param [in] option                stream in property, refer @ref AUDIO_RECORD_OPTION
    * @param [in] event_cb              stream in status listener
    * @returns stream in handle in @ref acm_audio_record_handle
    */
    audio_record_handle audio_record_open(uint32_t rate, uint32_t channels, int64_t option, audio_record_callback event_cb);

    /** Read pcm data from an audio record handle.
    * @param [in] handle                audio record handle
    * @param [out] data                 pcm buffer address used to copy pcm frames
    * @param [in,out] size              pcm buffer size
    * @returns error code in int, non-zero on failure
    */
    int audio_record_read(audio_record_handle handle, uint8_t* data, uint32_t* size);

    /** Get pcm data in heap from an audio record handle.
    * @param [in] handle                audio record handle
    * @param [out] data_ptr             store pcm frames heap address
    * @param [out] size                 store pcm frames heap size
    * @returns error code in int, non-zero on failure
    * @note user need to free the buffer!
    */
    int audio_record_get(acm_audio_record_handle handle, uint8_t** data_ptr, uint32_t* size);

    /** Close and free memory etc resource of an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in] force                 stop immediately or not
    * @returns error code in int, non-zero on failure
    */
    int audio_record_close(audio_record_handle handle, int32_t force);

    /** Set control to an audio record handle.
    * @param [in] handle                audio record handle
    * @param [in] ctrl_type             control type
    * @param [in] ctrl_value            control value
    * @returns error code in int, non-zero on failure
    */
    int audio_record_ctrl(audio_record_handle handle, enum audio_record_control ctrl_type, int32_t ctrl_value);

    /** Start wav format file capture.
    * @param [in] file_name             wav file name
    * @param [in] config                wav record config
    * @param [in,out] handle            audio record handle
    * @returns error code in int, non-zero on failure
    */
    int wave_capture_start(const char* file_name, const struct audio_record_config* config, audio_record_handle* handle);

    /** Stop wav file capture with an audio record handle.
    * @param [in] handle                audio record handle
    * @returns error code in int, non-zero on failure
    */
    int wave_capture_stop(audio_record_handle handle);

#ifdef __cplusplus
}
#endif
