/**
 * @file        cm_audio_player.h
 * @brief       Audio player接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By cmiot3000
 * @date        2023/07/27
 *
 * @defgroup audio_player
 * @ingroup audio_player
 * @{
 */

#ifndef __OPENCPU_AUDIO_PLAYER_H__
#define __OPENCPU_AUDIO_PLAYER_H__


/****************************************************************************
 * Included Files
 ****************************************************************************/

#include "cm_audio_common.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/


/****************************************************************************
 * Public Types
 ****************************************************************************/

/** 音频播放通道支持 */
typedef enum
{
    CM_AUDIO_PLAY_CHANNEL_RECEIVER = 1,         /*!< 听筒通道，不支持设置 */
    CM_AUDIO_PLAY_CHANNEL_HEADSET,              /*!< 耳机通道，不支持设置 */
    CM_AUDIO_PLAY_CHANNEL_SPEAKER,              /*!< 扬声器通道，不支持设置 */
#if 0 
    CM_AUDIO_PLAY_CHANNEL_REMOTE,               /*!< 远端播放（需建立通话），暂不支持 */
#endif
} cm_audio_play_channel_e;

/** 音频播放回调的事件类型 */
typedef enum
{
    CM_AUDIO_PLAY_EVENT_FINISHED = 1,           /*!< 播放结束 */
#if 0
    CM_AUDIO_PLAY_EVENT_INTERRUPT,              /*!< 播放中断 */
#endif
} cm_audio_play_event_e;

/** 音频播放设置类型 */
typedef enum
{
    CM_AUDIO_PLAY_CFG_CHANNEL = 1,              /*!< 播放通道，支持范围参见cm_audio_play_channel_e枚举量，不支持设置 */
    CM_AUDIO_PLAY_CFG_VOLUME,                   /*!< 播放音量，支持范围参见0~100 */
} cm_audio_play_cfg_type_e;

/**
 *   流播放事件枚举
*/
typedef enum
{
    /* 下述事件与ASR底层PCM播放事件相对应 */
    CM_AUDIO_TRACK_EVENT_NONE = 0,              /*!< dummy event */
    CM_AUDIO_TRACK_EVENT_TICK,                  /*!< tick event, 20 ms typically */
    CM_AUDIO_TRACK_EVENT_STARTED,               /*!< start event, current track is involved in mixing */
    CM_AUDIO_TRACK_EVENT_WAITING,               /*!< waiting event, current track is preempted by other track which has higher priority */
    CM_AUDIO_TRACK_EVENT_RUNNING,               /*!< running event, current track is recovered from waiting */
    CM_AUDIO_TRACK_EVENT_UNDERRUN,              /*!< underrun event, current track has played out all the pcm data */
    CM_AUDIO_TRACK_EVENT_OVERRUN,               /*!< overrun event, current track can not hold more pcm data */
    CM_AUDIO_TRACK_EVENT_NEARLY_UNDERRUN,       /*!< fast up event, current track need more pcm data */
    CM_AUDIO_TRACK_EVENT_NEARLY_OVERRUN,        /*!< slow down event, current track need less pcm data */
    CM_AUDIO_TRACK_EVENT_CLOSED,                /*!< close event, current track is closed */
    CM_AUDIO_TRACK_EVENT_DRAINING,              /*!< draining event, current track is in draining */
    CM_AUDIO_TRACK_EVENT_TERMINATED,            /*!< terminate event, current track is terminated in an unexpected way */
    CM_AUDIO_TRACK_EVENT_CONSUMED,              /*!< consume event, current track has consumed vendor buffer */
    CM_AUDIO_TRACK_EVENT_ENUM_32_BIT,           /*!< 32bit enum compiling enforcement */

    /* 下述事件与ASR底层AMR播放事件相对应 */
    CM_AMR_PLAYBACK_STATUS_ENDED = 100,         /*!< play out all for file or stream */
    CM_AMR_PLAYBACK_STATUS_STARTED,             /*!< indicate current playback is started */
    CM_AMR_PLAYBACK_STATUS_STREAM,
    CM_AMR_STREAM_STATUS_NEARLY_OVERRUN,        /*!< indicate amr data producer should slow down to avoid overrun */
    CM_AMR_STREAM_STATUS_NEARLY_UNDERRUN,       /*!< indicate amr data producer should fast up to avoid underrun */

    /* 下述事件与ASR底层MP3播放事件相对应 */
    CM_MP3_PLAYBACK_STATUS_ENDED = 200,         /*!< play out all for file or stream */
    CM_MP3_PLAYBACK_STATUS_STARTED,             /*!< indicate current playback is started */
    CM_MP3_PLAYBACK_STATUS_OPENED,              /*!< indicate output device is opened */
    CM_MP3_PLAYBACK_STATUS_FILE_READED,         /*!< reach file end */
    CM_MP3_PLAYBACK_STATUS_STREAM,
    CM_MP3_STREAM_STATUS_NEARLY_OVERRUN,        /*!< indicate mp3 data producer should slow down to avoid overrun */
    CM_MP3_STREAM_STATUS_NEARLY_UNDERRUN,       /*!< indicate mp3 data producer should fast up to avoid underrun */
} cm_audio_player_stream_event_e;

/****************************************************************************
 * Public Data
 ****************************************************************************/


/****************************************************************************
 * Public Function Prototypes
 ****************************************************************************/

#ifdef __cplusplus
#define EXTERN extern "C"
extern "C"
{
#else
#define EXTERN extern
#endif

/**
*  @brief 流播放状态回调（仅适用于ASR平台芯片）
*
*  @param [in] event 事件
*
*/
typedef void (*cm_audio_player_stream_cb)(cm_audio_player_stream_event_e event);

/**
 * @brief 播放回调函数
 *
 * @param [in] event 事件类型
 * @param [in] param  事件参数
 *
 * @details  须在播放API中传入 \n
 *           通知事件为中断触发，不可阻塞，不可做耗时较长的操作，例如不可使用UART打印log
 */
typedef void (*cm_audio_play_callback_t)(cm_audio_play_event_e event, void *param);


/**
 * @brief 设置播放参数
 *
 * @param [in] type  设置参数类型
 * @param [in] value 设置参数数值
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 * @details 不支持设置播放通道（模组仅一个播放通道，默认使用该通道，该通道外接扬声器/耳机/听筒由用户决定） \n
 *          CODEC方案下，由于用户CODEC方案下芯片选型、CODEC方案下参数不一，音频增益无法保证与CODEC芯片驱动能力适配，无法保证本接口设置音量功能正常，不建议使用本接口设置音量功能 \n
 *          使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途
 */
int32_t cm_audio_play_set_cfg(cm_audio_play_cfg_type_e type, void *value);

/**
 * @brief 读取播放参数
 *
 * @param [in]  type  读取参数类型
 * @param [out] value 读取参数数值
 *
 * @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *
 * @details 不支持获取播放通道（模组仅一个播放通道，默认使用该通道，该通道外接扬声器/耳机/听筒由用户决定） \n
 *          使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途
 */
int32_t cm_audio_play_get_cfg(cm_audio_play_cfg_type_e type, void *value);

/**
 *  @brief 从文件系统播放音频文件
 *  
 *  @param [in] path         文件路径/名称
 *  @param [in] format       播放格式
 *  @param [in] sample_param 播放音频PCM采样参数（format参数为CM_AUDIO_PLAY_FORMAT_PCM使用，其余情况传入NULL）
 *  @param [in] cb           音频播放回调函数（回调函数在音频处理线程中被执行）
 *  @param [in] cb_param     用户参数（与cm_audio_play_callback回调函数中param参数相对应）
 *
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 *  @details 支持mp3、amr格式 \n
 *           通知事件为中断触发，不可阻塞，不可做耗时较长的操作 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *
 *           ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
int32_t cm_audio_play_file(const char *path, cm_audio_play_format_e format, cm_audio_sample_param_t *sample_param, cm_audio_play_callback_t cb, void *cb_param);

/**
 *  @brief 暂停播放
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 *  @details 支持MP3、AMR格式;调用该接口到暂停播放存在一定延时 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途
 */
int32_t cm_audio_player_pause(void);

/**
 *  @brief 继续播放
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 */
int32_t cm_audio_player_resume(void);

/**
 *  @brief 停止播放
 *  
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 *  @details 支持MP3、AMR格式;调用该接口到停止播放存在一定延时 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途
 */
int32_t cm_audio_player_stop(void);

 /**
 *  @brief 注册流播放状态回调（仅适用于ASR平台芯片）
 *
 *  @return void
 *
 *  @details 回调事件仅适用于cm_audio_player_stream_open()/cm_audio_player_stream_push()/cm_audio_player_stream_close()等接口 \n
 *           该接口应在调用上述接口前先注册 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *
 *           ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
void cm_audio_player_stream_cb_reg(cm_audio_player_stream_cb cb);

/**
 *  @brief 从管道/消息队列中播放音频（开启）
 *  
 *  @param [in] format       播放格式
 *  @param [in] sample_param 播放音频PCM采样参数（format参数为CM_AUDIO_PLAY_FORMAT_PCM使用，其余情况传入NULL）
 *
 *  @return
 *   =  0 - 成功 \n
 *   = -1 - 失败
 *  
 *  @details 支持PCM、MP3、AMR格式 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *
 *           ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
int32_t cm_audio_player_stream_open(cm_audio_play_format_e format, cm_audio_sample_param_t *sample_param);

/**
 *  @brief 往管道/消息队列中发送要播放的音频数据
 *  
 *  @param [in] data 播放的数据
 *  @param [in] size 播放数据的长度
 *
 *  @return 
 *   =  0 - 成功 \n
 *   >  0 - 实际写入的数据长度（本平台不支持） \n
 *   = -1 - 失败
 *
 *  @details 支持PCM、MP3、AMR格式 \n
 *           对于PCM格式，传输数据须是帧长度的整数倍（例如，8k 16bit的PCM数据，每帧为320字节，传输的数据应是320的整数倍） \n
 *           对于AMR格式，传输数据若包含不完整帧数据可能出现爆破音 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *
 *           ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
int32_t cm_audio_player_stream_push(uint8_t *data, uint32_t size);

/**
 *  @brief 从管道/消息队列中播放音频（关闭）
 *
 *  @details ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
void cm_audio_player_stream_close(void);

/**
 *  @brief 从管道/消息队列中播放音频（清除待播放数据及关闭）
 *  
 *  @details ML307C音频接口支持PWM接口输出音频模拟信号驱动扬声器方案和PCM接口输出音频数字信号至用户外挂音频设备由用户外挂音频设备执行播放方案 \n
 *           使用音频功能时，相关PIN脚（PCM_CLK、PCM_SYNC、PCM_IN、PCM_OUT）不可做其他功能用途 \n
 *           PWM接口输出音频模拟信号驱动扬声器方案有如下特性： \n
 *           1.模组无内置CODEC芯片，播放声音的方式为PWM输出音频模拟信号波形驱动扬声器进行播放 \n
 *           2.因模组无内置CODEC芯片，不支持音频参数校准 \n
 *           3.PWM驱动能力有限，无法驱动大功率的扬声器，若用户需要使用大功率扬声器，需自行选型PA芯片并进行软硬件开发 \n
 *           4.若用户引入PA芯片和PA电路，开启或关闭音频设备时电路可能会出现电压跳变导致爆破音 \n
 *           5.若出现爆破音且用户的硬件或者使用的PA芯片无法处理爆破音，应用逻辑上则可考虑从软件上进行规避 \n
 *           PCM接口输出音频数字信号至用户外挂设备由用户外挂音频设备执行播放方案有如下特性： \n
 *           1.模组自身无音频播放能力，播放相关API执行效果为通过硬件PCM口将API接口传入的PCM数据（或MP3/AMR解码后的PCM数据）发送至用户外挂音频设备 \n
 *           2.音频播放操作由用户外挂音频设备完成，音效校准、降噪等由用户外挂音频设备实现 \n
 *           3.如用户外挂音频设备需要驱动才可使用，可使用AudioHAL_AifBindCodec_CB()注册用户外挂音频设备开启/关闭操作的回调函数，底层在相应操作下自行调用用户注册的回调 \n
 *           4.cm_ex_codec.c中默认提供“ES8311 CODEC芯片+无DSP降噪芯片”方案，用户使用此方案仅需完成IIC从设备地址和ES8311 CODEC参数适配后即可使用 \n
 *           5.本方案音频播放能力为用户外挂音频设备提供，相关音效、降噪、校准等能力请咨询外挂音频设备厂商 \n
 *           6.用户应在完成外挂音频芯片驱动开发并确保音频芯片工作正常和具备播放功能下，方可使用此接口 \n
 */
void cm_audio_player_stream_clear_close(void);

#undef EXTERN
#ifdef __cplusplus
}
#endif

#endif

/** @}*/

