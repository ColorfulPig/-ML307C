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
    * @file acm_audio_vocoder.h
    * @brief ASR audio vocoder API describes the process and functions used for AMR encoder and decoder in DSP.
    */

    /** audio vocoder callback to receive the encoded amr frame*/
    typedef void(*AmrFrameCallback)(const uint8_t*, uint32_t);
    /** audio vocoder callback to request amr frame for decoder*/
    typedef void(*AmrFrameRequest)(uint8_t*, uint32_t*);

    /** Start amr encoder.
    * @param [in] rate              amr encoder rate, 0~7 for nb, 0~8 for wb
    * @param [in] wb                use wb mode or not
    * @param [in] cb                callback function from user
    * @returns error code in int, non-zero on failure
    */
    int audio_encoder_amr_start(int32_t rate, int32_t wb, AmrFrameCallback cb);

    /** Stop amr encoder.
    * @returns error code in int, non-zero on failure
    */
    int audio_encoder_amr_stop(void);

    /** Start amr decoder.
    * @param [in] wb                use wb mode or not
    * @param [in] cb                callback function from user
    * @returns error code in int, non-zero on failure
    */
    int audio_decoder_amr_start(int32_t wb, AmrFrameRequest cb);

    /** Stop amr decoder.
    * @returns error code in int, non-zero on failure
    */
    int audio_decoder_amr_stop(void);

#ifdef __cplusplus
}
#endif
