/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2018 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for Audio pcm
*
* Filename: audio_pcm.h
*
* Authors: Chao Liu
*
* Description: Header file for tg pcm.
*
* Last Updated:
*
* Notes:
******************************************************************************/
#ifndef _AUDIO_PCM_H_
    #define _AUDIO_PCM_H_

#ifdef __cplusplus
extern "C"{
#endif



typedef void*  tg_pcm_handle_t ;
typedef enum {
    PCM_MODE_OUT = 0,
    PCM_MODE_IN = 1,
    PCM_MODE_ERROR = 0x7FFFFFFF
}tg_pcm_mode;

typedef enum {
    PCM_FORMAT_U16 = 0,
    PCM_FORMAT_ERROR = 0x7FFFFFFF
}tg_pcm_format;


#define TG_PCM_UNDERRUN (0x01)
#define TG_PCM_OVERRUN (0x02)
typedef void (*tg_pcm_state_callback_t)(tg_pcm_handle_t handle, unsigned int event, void* userPtr);



tg_pcm_handle_t tg_pcm_open(tg_pcm_mode mode, 
                                int samplerate, 
                                int channels, 
                                tg_pcm_format format);

void tg_pcm_close(tg_pcm_handle_t pcm);


void tg_pcm_set_state_listener(tg_pcm_handle_t pcm, tg_pcm_state_callback_t cbf, void* user);

int tg_pcm_start(tg_pcm_handle_t pcm);
int tg_pcm_stop(tg_pcm_handle_t pcm);

int tg_pcm_write(tg_pcm_handle_t pcm, void* buf, unsigned int size);
int tg_pcm_read(tg_pcm_handle_t pcm, void* buf, unsigned int size);

void tg_volume_set(int volume);


#ifdef __cplusplus
}
#endif
#endif  /* _AUDIOHAL_H_ */
