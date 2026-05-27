#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifndef AMR_VOCODER_DSP_SUPPORT
#define AMR_VOCODER_DSP_SUPPORT     (1) //cmiot 0->1
#endif

#define AMR_DEC_FW_SUPPORT          (AMR_VOCODER_DSP_SUPPORT & 0x1)
#define AMR_ENC_FW_SUPPORT          (AMR_VOCODER_DSP_SUPPORT & 0x2)
#define AMR_DEC_SW_SUPPORT          (FEATURE_AUDIO_NBAMR_DEC || FEATURE_AUDIO_WBAMR_DEC)
#define AMR_ENC_SW_SUPPORT          (FEATURE_AUDIO_NBAMR_ENC || FEATURE_AUDIO_WBAMR_ENC)
#define FEATURE_AUDIO_AMR_DEC       (AMR_DEC_SW_SUPPORT || AMR_DEC_FW_SUPPORT)
#define FEATURE_AUDIO_AMR_ENC       (AMR_ENC_SW_SUPPORT || AMR_ENC_FW_SUPPORT)

#if AMR_DEC_FW_SUPPORT == 1
#define AMR_DEC_STREAM_SUPPORT      (1)
#else
#ifndef AMR_DEC_STREAM_SUPPORT
#define AMR_DEC_STREAM_SUPPORT      (0)
#endif
#endif

#ifndef AMR_DEC_FRAMES_PER_REQUEST
#define AMR_DEC_FRAMES_PER_REQUEST  (10)
#endif

#ifndef AUDIO_MALLOC
#define AUDIO_MALLOC                malloc
#endif

#ifndef AUDIO_FREE
#define AUDIO_FREE                  free
#endif

#ifdef __cplusplus
}
#endif
