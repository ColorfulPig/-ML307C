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
    * @file acm_audio_tone.h
    * @brief ASR audio tone API describes the process and functions used for tone generation in DSP.
    */

#define TONE_FREQUENCY_SET_MIN      (5)
#define TONE_FREQUENCY_SET_MAX      (4000)

    /** Start tone generation, set from ascii code.
    * @param [in] ascii_code        ascii code of key
    * @returns error code in int, non-zero on failure
    */
    int audio_tone_start_ascii(int32_t ascii_code);

    /** Start tone generation, set from frequency.
    * @param [in] frequency1        the first frequency
    * @param [in] frequency2        the second frequency
    * @returns error code in int, non-zero on failure
    */
    int audio_tone_start_frequency(int32_t frequency1, int32_t frequency2);

    /** Set duration for tone generation.
    * @param [in] ms                tone duration in microsecond
    * @returns error code in int, non-zero on failure
    */
    int audio_tone_duration(int32_t ms);

    /** Stop tone generation.
    * @returns error code in int, non-zero on failure
    */
    int audio_tone_stop(void);

#ifdef __cplusplus
}
#endif
