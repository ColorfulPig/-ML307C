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

    /**
    * @file acm_audio_def.h
    * @brief ASR audio macros, structures that define the PCM property for playback and capture.
    */

    /** audio playback property in bitmap format, set bitfield and get the option field in int64 */
    typedef struct AUDIO_PLAY_OPTION {
        union {
            long long option;
            struct {
                /** bit0~bit1, dest play end, 0->near,1->far,2->both, refer @ref acm_audio_track_direction_t*/
                unsigned int dest_end : 2;
                /** bit2, override voice if any, 0->false,1->true */
                unsigned int override : 1;
                /** bit3, near codec or vocoder, 0->codec,1->vocoder */
                unsigned int active_point : 1;
                /** bit4, reserved */
                unsigned int : 1;
                /** bit5, play current file in cyclic mode, 0->false,1->true */
                unsigned int cyclic : 1;
                /** bit6~bit7, schedule mode, 0->combine,1->exclusive,2->preempt, refer @ref acm_audio_track_mode_t */
                unsigned int mode : 2;
                /** bit8~bit11, eq effect index, 0 for EQ off */
                unsigned int eq : 4;
                /** bit12~bit20, speed factor in Q8, 0 for normal*/
                unsigned int speed : 9;
                /** bit21~bit23, write mode, refer @ref acm_audio_track_write_mode_t*/
                unsigned int write_mode : 3;
                /** bit24~bit27, pcm format, refer @ref acm_audio_track_format_t*/
                unsigned int fmt : 4;
                /** bit28, turn off audio effect */
                unsigned int effect_off : 1;
                /** bit29~bit31, reserved */
                unsigned int : 3;
                /** bit32~bit41, how many frames can be held*/
                unsigned int capacity : 10;
                /** bit42~bit45, how many frames to keep dynamically, level 0~7*/
                unsigned int latency : 3;
                /** bit46~bit48, how many mixed frames to keep dynamically, level 0~7*/
                unsigned int latency2 : 3;
            };
        };
    }AUDIO_PLAY_OPTION;

    /** audio record property in bitmap format, set bitfield and get the option field in int64 */
    typedef struct AUDIO_RECORD_OPTION {
        union {
            long long option;
            struct {
                /** bit0~bit3, record mode, refer @ref acm_audio_record_mode_t */
                unsigned int mode : 4;
                /** bit4~bit9, record gain in Q0 */
                int gain : 6;
                /** bit10, turn off audio effect */
                unsigned int effect_off : 1;
                /** bit11, do not operate audio path */
                unsigned int observer : 1;
            };
        };
    }AUDIO_RECORD_OPTION;

#define AUDIO_PLAY_MASK_DEST_END        (0x3 << 0)
#define AUDIO_PLAY_MASK_OVERRIDE        (0x1 << 2)
#define AUDIO_PLAY_MASK_ACTIVE_POINT    (0x1 << 3)
#define AUDIO_PLAY_MASK_DRAIN           (0x1 << 4)
#define AUDIO_PLAY_MASK_CYCLIC          (0x1 << 5)
#define AUDIO_PLAY_MASK_MODE            (0x3 << 6)
#define AUDIO_PLAY_MASK_EQ              (0xf << 8)
#define AUDIO_PLAY_MASK_SPEED           (0x1ff << 12)
#define AUDIO_PLAY_MASK_WRITE_MODE      (0x7 << 21)
#define AUDIO_PLAY_MASK_FORMAT          (0xf << 24)
#define AUDIO_PLAY_MASK_EFFECT          (0x1 << 28)
#define AUDIO_PLAY_MASK_CAPACITY        (0xall << 32)
#define AUDIO_PLAY_MASK_LATENCY         (0x3ll << 42)
#define AUDIO_PLAY_MASK_LATENCY2        (0x3ll << 45)

#define AUDIO_RECORD_MASK_MODE          (0xf << 0)
#define AUDIO_RECORD_MASK_GAIN          (0x3f << 4)
#define AUDIO_RECORD_MASK_EFFECT        (0x1 << 10)
#define AUDIO_RECORD_MASK_OBSERVER      (0x1 << 11)

#define AUDIO_SERVICE_REVISION          0x01060007
#ifdef __cplusplus
}
#endif
