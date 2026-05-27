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

#define AUDIO_GAIN_PRECISION            (8)    
#define AUDIO_GAIN_QFORMAT              (1 << AUDIO_GAIN_PRECISION)

#define AUDIO_GAIN_DB_MINIMUM           (-36)
#define AUDIO_GAIN_DB_MAXIMUM           (12)

#define AUDIO_EQ_GROUP_NUM              (5)

    typedef struct eq_effect_params {
        int16_t a[3];
        int16_t b[3];
    } eq_effect_params_t;

    typedef struct audio_effect_eq {
        //bit 0 ~bit 4 is on/off of each filter.
        int16_t ctrl;
        eq_effect_params_t params[AUDIO_EQ_GROUP_NUM];
    }audio_effect_eq_t;

    typedef struct audio_effect_config {
        uint32_t rate;
        uint32_t frame_length;
        int32_t target_gain;
        int32_t ramp_ctrl;
        int32_t gain_ramp_off;
        audio_effect_eq_t eq;
    }audio_effect_config_t;

    typedef uint32_t audio_effect_handle;

    int audio_effect_create(const audio_effect_config_t* config, audio_effect_handle* handle);
    int audio_effect_run(audio_effect_handle handle, const int16_t* input, int16_t* output);
    int audio_effect_gain_ctrl(audio_effect_handle handle, int32_t gain);
    int audio_effect_mute_ctrl(audio_effect_handle handle, int32_t mute);
    int audio_effect_ramp_ctrl(audio_effect_handle handle, int32_t ctrl);
    int audio_effect_eq_ctrl(audio_effect_handle handle, const audio_effect_eq_t* eq);
    int audio_effect_destroy(audio_effect_handle handle);

#ifdef __cplusplus
}
#endif