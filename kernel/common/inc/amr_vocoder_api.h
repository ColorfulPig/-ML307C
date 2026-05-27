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
#include "acm_audio_def.h"

    /**
    * @file amr_vocoder_api.h
    * @brief ASR amr API describes the process and functions used for AMR encoder and decoder.
    */

    typedef enum AmrFileStatus {
        /** indicate current file has been readed and played out */
        AMR_FILE_STATUS_ENDED = 0,
        /** indicate output device is opened*/
        AMR_FILE_STATUS_STARTED,
    }AmrFileStatus;

    typedef enum AmrPlaybackEventType {
        /** status related event type, refer @ref AmrFileStatus for event value*/
        AMR_PLAYBACK_EVENT_STATUS,
        /** amr rate event type, value ranges from 0 to 7*/
        AMR_PLAYBACK_EVENT_RATE,
        /** file name request, value with type <tt>char*<tt>*/
        AMR_PLAYBACK_EVENT_FILENAME,
    }AmrPlaybackEventType;
    typedef AmrPlaybackEventType AmrFileEventType;

    typedef enum AmrPlaybackEventValue {
        /** play out all for file or stream*/
        AMR_PLAYBACK_STATUS_ENDED = 0,
        /** indicate current playback is started*/
        AMR_PLAYBACK_STATUS_STARTED,

        AMR_PLAYBACK_STATUS_STREAM = 100,
        /** indicate amr data producer should slow down to avoid overrun*/
        AMR_STREAM_STATUS_NEARLY_OVERRUN,
        AMR_STREAM_STATUS_SLOW_DOWN = AMR_STREAM_STATUS_NEARLY_OVERRUN,
        /** indicate amr data producer should fast up to avoid underrun*/
        AMR_STREAM_STATUS_NEARLY_UNDERRUN,
        AMR_STREAM_STATUS_FAST_UP = AMR_STREAM_STATUS_NEARLY_UNDERRUN,
    }AmrPlaybackEventValue;

    typedef void(*AmrPlaybackEventCallback)(AmrPlaybackEventType, int);
    typedef struct AmrPlaybackConfigInfo {
        /** playback option in bitmap format, refer @ref AUDIO_PLAY_OPTION in @ref acm_audio_def.h */
        int64_t option;
        /** block size for file only, 0 for frame mode*/
        int block_size;
        /** user registers callback to get playback event such as file end etc */
        AmrPlaybackEventCallback listener;
    }AmrPlaybackConfigInfo;
    typedef AmrPlaybackConfigInfo AmrFileConfigInfo;

    typedef void(*AmrStreamCallback)(const uint8_t*, uint32_t);
    typedef struct AmrEncConfigInfo {
        /** encoder mode, refer @ref acm_audio_record_mode_t*/
        int mode;
        /** extra digital gain for recorded pcm before encoding, unit:dB, default:0 */
        int gain;
        /** encoder rate, 0~7 for nb, 0~8 for wb, 0 for lowest rate*/
        int rate;
        /** dtx on or off, 0:off, 1:on*/
        int dtx;
        /** wb-amr mode*/
        int wb_mode;
        /** record timeout in microsecond*/
        int timeout;
        /** user registers callback to get encoded amr stream */
        AmrStreamCallback callback;
    }AmrEncConfigInfo;

    /** amr playback handle, held and used by amr playback user*/
    typedef uint32_t AmrPlaybackHandle;
    /** amr encoder handle, held and used by amr encoder user*/
    typedef uint32_t AmrEncodeHandle;

    /** Start amr file playback with default option, playback to near side.
    * @param [in] name                      amr file name
    * @returns error code in int, non-zero on failure
    */
    int amrStart(const char* name);

    /** Stop all the amr file playbacks ongoing.
    * @returns error code in int, non-zero on failure
    */
    int amrStop(void);

    /** Start amr file playback with configuration.
    * @param [in] file_name                 amr file name, 0 for stream mode
    * @param [in] config                    amr playback configuration
    * @param [in,out] handle                amr playback handle
    * @returns error code in int, non-zero on failure
    */
    int amrPlayStart(const char* file_name, const AmrPlaybackConfigInfo* config, AmrPlaybackHandle* handle);

    /** Write an amr data block to an amr playback handle.
     * @param [in] handle                   amr playback handle
     * @param [in] data                     pcm buffer address
     * @param [in] size                     pcm buffer size
     * @returns error code in int, non-zero on failure
     */
    int amrPlayBuffer(AmrPlaybackHandle handle, const uint8_t* data, uint32_t size);

    /** Close and free memory etc resource of an amr playback instance specified by handle.
    * @param [in] handle                    amr playback handle
    * @param [in] drain                     drain or not
    * @returns error code in int, non-zero on failure
    */
    int amrPlayStop(AmrPlaybackHandle handle, int drain);

    /** Get audio track handle, which is used to manipulate pcm stream.
    * @param [in] handle                    amr playback handle
    * @param [in] track_handle              audio track handle
    * @returns error code in int, non-zero on failure
    */
    int amrGetPCMOutHandle(AmrPlaybackHandle handle, uint32_t* track_handle);

    /** Init amr playback resource.
    * @note for amr playback user do @b not call this api!
    */
    void amrPlayInit(void);

    /** Close and free memory etc resource of an amr playback instance specified by name.
    * @param [in] handle                    amr playback handle
    * @param [in] drain                     drain or not
    * @returns error code in int, non-zero on failure
    */
    int amrPlayStopWithName(const char* file_name, int drain);

    /** Start amr encoder with configuration.
    * @param [in] file_name                 file name into which amr is encoded
    * @param [in] config                    amr encoder configuration
    * @param [in,out] handle                amr encoder handle
    * @returns error code in int, non-zero on failure
    */
    int amrEncStart(const char* file_name, const AmrEncConfigInfo* config, AmrEncodeHandle* handle);

    /** Close and free memory etc resource of an amr encoder instance specified by handle.
    * @param [in] handle                    amr encoder handle
    * @returns error code in int, non-zero on failure
    */
    int amrEncStop(AmrEncodeHandle handle);

    /** Close and free memory etc resource of an amr encoder instance specified by name.
    * @param [in] file_name                 file name for amr encoder result
    * @returns error code in int, non-zero on failure
    */
    int amrEncStopWithName(const char* file_name);

    /** Get audio record handle, which is used to manipulate pcm stream.
    * @param [in] handle                    amr encoder handle
    * @param [in] record_handle             audio record handle
    * @returns error code in int, non-zero on failure
    */
    int amrGetPCMInHandle(AmrEncodeHandle handle, uint32_t* record_handle);

    /** Init amr encoder resource.
    * @note for amr encoder user do @b not call this api!
    */
    void amrEncInit(void);
#ifdef __cplusplus
}
#endif
