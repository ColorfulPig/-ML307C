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

#ifdef __cplusplus
extern "C" {
#endif
#include <stdint.h>

#define ID_RIFF 0x46464952
#define ID_WAVE 0x45564157
#define ID_FMT  0x20746d66
#define ID_DATA 0x61746164

    typedef struct riff_wave_header {
        uint32_t riff_id;
        uint32_t riff_sz;
        uint32_t wave_id;
    }riff_wave_header;

    typedef struct chunk_header {
        uint32_t id;
        uint32_t sz;
    }chunk_header;

    typedef struct chunk_fmt {
        uint16_t audio_format;
        uint16_t num_channels;
        uint32_t sample_rate;
        uint32_t byte_rate;
        uint16_t block_align;
        uint16_t bits_per_sample;
    }chunk_fmt;

    typedef struct wave_header {
        riff_wave_header riff;
        chunk_header chunk;
        chunk_fmt fmt;
        chunk_header chunk2;
    }wave_header;

#ifdef __cplusplus
}
#endif