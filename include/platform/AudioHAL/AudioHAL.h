/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2018 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for Audio HAL
*
* Filename: AudioHAL.h
*
* Authors: Jackie Fan
*
* Description: Header file for Audio HAL.
*
* Last Updated:
*
* Notes:
******************************************************************************/


typedef void (*AUDIOHAL_SpeakerPA_T) (unsigned long on);

/* PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放和用户外挂音频设备完成录音并通过PCM接口将录音PCM数据发送至模组方案下支持此接口 */
void AudioHAL_AifBindCodec_CB(AUDIOHAL_SpeakerPA_T cb);