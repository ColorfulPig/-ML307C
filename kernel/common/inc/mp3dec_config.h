#pragma once
#ifdef __cplusplus
extern "C" {
#endif

#ifndef MP3_STREAM_SUPPORT
#define MP3_STREAM_SUPPORT              (1)
#endif

#ifndef MP3_STREAM_API_TEST
#define MP3_STREAM_API_TEST             (0)
#endif

#ifndef MP3_MIXER_MODE
#define MP3_MIXER_MODE                  (1)
#endif

#ifndef MP3_FRAMES_CACHE_MAX
#define MP3_FRAMES_CACHE_MAX            (300)
#endif

#ifndef MP3_DEC_CALCULATE_TIME
#define MP3_DEC_CALCULATE_TIME          (1)
#endif

#ifndef MP3_DEC_FRAMES_PER_REQUEST
#define MP3_DEC_FRAMES_PER_REQUEST      (1)
#endif

#ifndef AUDIO_MALLOC
#define AUDIO_MALLOC                    malloc
#endif

#ifndef AUDIO_FREE
#define AUDIO_FREE                      free
#endif

#ifdef __cplusplus
}
#endif
