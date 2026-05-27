#include "cm_os.h"
#include "cm_sys.h"
#include "cm_local_tts.h"
#include "cm_pm.h"
#include "string.h"
#include "stdbool.h"
#include "cm_audio_player.h"
#include "cm_audio_recorder.h"
#include "cm_fs.h"
#include "cm_mem.h"


#define ARRAY_SIZE(x) (sizeof(x)/sizeof((x)[0]))

#define APP_RECORDER_TTS_SOUND1                 ("您的录音将被保存为WAV文件，录音时间为5秒，请开始录音")
#define APP_RECORDER_TTS_SOUND2                 ("录音已完成，您的录音是")
#define APP_RECORDER_TTS_SOUND3                 ("您的录音将被保存为AMR文件，录音时间为8秒，请开始录音")

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




uint8_t Tts_sound2_buff[320 * 50 * 4] = {0};    //记录APP_RECORDER_TTS_SOUND2转码数据的数组
uint8_t Tts_sound3_buff[320 * 50 * 10] = {0};   //记录APP_RECORDER_TTS_SOUND3转码数据的数组
uint32_t Tts_sound2_buff_len = 0;               //记录已完成的APP_RECORDER_TTS_SOUND2转码数据的长度
uint32_t Tts_sound3_buff_len = 0;               //记录已完成的APP_RECORDER_TTS_SOUND3转码数据的长度

uint32_t app_pcm_data_buff_len = 0;             //记录录音的PCM数据长度


int32_t app_wav_file_fd = -1;                   //保存至文件系统中的WAV录音文件句柄
int32_t app_amr_file_fd = -1;                   //保存至文件系统中的AMR录音文件句柄


bool app_tts_is_working = false;                //TTS是否处于工作状态
bool app_player_is_working = false;             //播放器是否处于工作状态


osThreadId_t RECORDER_APP_TaskHandle = NULL;    //APP主任务句柄

static void __app_recorder_tts_callback(cm_local_tts_event_e event, void *param)
{
    switch(event)
    {
        case CM_LOCAL_TTS_EVENT_SYNTH_DATA:
        {
            cm_local_tts_synth_data_t *synth_data = (cm_local_tts_synth_data_t *)param;

            /* APP中，宏APP_RECORDER_TTS_SOUND2记录的文本会多次播报，故实现将其转码为本地数据，后续使用时直接播放该数据 */
            if (0 == strncmp(synth_data->user, "sound2", strlen("sound2")))
            {
                memcpy (Tts_sound2_buff + Tts_sound2_buff_len, (uint8_t *)synth_data->data, synth_data->len);
                Tts_sound2_buff_len = Tts_sound2_buff_len + synth_data->len;

            }

            /* APP中，宏APP_RECORDER_TTS_SOUND3记录的文本会多次播报，故实现将其转码为本地数据，后续使用时直接播放该数据 */
            if (0 == strncmp(synth_data->user, "sound3", strlen("sound3")))
            {
                memcpy (Tts_sound3_buff + Tts_sound3_buff_len, (uint8_t *)synth_data->data, synth_data->len);
                Tts_sound3_buff_len = Tts_sound3_buff_len + synth_data->len;
            }

            break;
        }
        case CM_LOCAL_TTS_EVENT_SYNTH_FAIL:
        case CM_LOCAL_TTS_EVENT_SYNTH_INTERRUPT:
        case CM_LOCAL_TTS_EVENT_SYNTH_FINISH:
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_FAIL:
            app_tts_is_working = false;
            cm_log_printf(0, "[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_FAIL\n", (char *)param);
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT:
            app_tts_is_working = false;
            cm_log_printf(0, "[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_INTERRUPT\n", (char *)param);
            break;
        case CM_LOCAL_TTS_EVENT_PLAY_FINISH:
            app_tts_is_working = false;
            cm_log_printf(0, "[TTS] [%s] CM_LOCAL_TTS_EVENT_PLAY_FINISH\n", (char *)param);
            break;
        default:
            break;
    }
}

static void __app_pcm_record_cb(cm_audio_record_event_e event, void *param)
{
    /* 通知事件为中断触发，不可阻塞，不可做耗时较长的操作 */

    cm_audio_record_data_t *record_data = (cm_audio_record_data_t *)param;

    if (CM_AUDIO_RECORD_EVENT_DATA == event)
    {
        if (0 < app_wav_file_fd)
        {
            app_pcm_data_buff_len = app_pcm_data_buff_len + record_data->len;

            /* 将录音的音频数据写入文件系统中的WAV文件 */
            cm_fs_write(app_wav_file_fd, record_data->data, record_data->len);
        }
    }
}

static void __app_amr_record_cb(cm_audio_record_event_e event, void *param)
{
    cm_audio_record_data_t *record_data = (cm_audio_record_data_t *)param;

    /* 不存在录音文件句柄，结束录音并报错 */
    if (0 > app_amr_file_fd)
    {
        cm_log_printf(0, "[%s] fs error, no user_amr_fd\n", __func__);
        cm_pm_reboot();
    }

    /* 将录音的音频数据写入文件系统中的AMR文件 */
    int32_t write_len = cm_fs_write(app_amr_file_fd, record_data->data, record_data->len);

    /* 数据写入错误 */
    if (write_len != record_data->len)
    {
        cm_log_printf(0, "[%s] fs write fail\n", __func__);
        cm_fs_close(app_amr_file_fd);
        app_amr_file_fd = -1;
        cm_pm_reboot();
    }
}

//播放回调函数
static void __app_player_process_event(cm_audio_play_event_e event, void *param)
{
    if (event == CM_AUDIO_PLAY_EVENT_FINISHED)                      //判断播放结束
    {
        /* 通知事件为中断触发，不可阻塞，不可做耗时较长的操作 */
        app_player_is_working = false;
    }
}

/**
 * TTS应用初始化
 *
 */
static void app_recorder_tts_init(void)
{
    /* 播放器音量设置为最大值 */
    int32_t volume = 100;
    cm_audio_play_set_cfg(CM_AUDIO_PLAY_CFG_VOLUME, &volume);

    /* TTS初始化 */
    cm_local_tts_cfg_t cfg = {0};

    cfg.speed = 5;
    cfg.volume = 15;
    cfg.encode = CM_LOCAL_TTS_ENCODE_TYPE_UTF8;
    cfg.digit = 0;

    if (0 != cm_local_tts_init(&cfg))       //若初始化失败，可能是由于使用的模组型号并非是TTS版本模组型号导致
    {
        cm_log_printf(0, "[TTS] init fail, reboot\n");

        osDelay(200);
        cm_pm_reboot();
    }

    /* 常用TTS语句预转码 */
    /* 转码TTS，离线TTS仅支持中文文本（不支持英文文本），且中文编码格式仅支持UTF8格式
       使用如下示例需注意转码的中文文本内容为UTF8格式（本源文件编码格式为UTF8时，才能按下述代码实现*/
    if (0 != cm_local_tts_synth(APP_RECORDER_TTS_SOUND2, strlen(APP_RECORDER_TTS_SOUND2), __app_recorder_tts_callback, "sound2"))
    {
        cm_log_printf(0, "[TTS] synth fail, reboot\n");

        osDelay(200);
        cm_pm_reboot();
    }

    /* 常用TTS语句预转码 */
    /* 转码TTS，离线TTS仅支持中文文本（不支持英文文本），且中文编码格式仅支持UTF8格式
       使用如下示例需注意转码的中文文本内容为UTF8格式（本源文件编码格式为UTF8时，才能按下述代码实现*/
    if (0 != cm_local_tts_synth(APP_RECORDER_TTS_SOUND3, strlen(APP_RECORDER_TTS_SOUND3), __app_recorder_tts_callback, "sound3"))
    {
        cm_log_printf(0, "[TTS] synth fail, reboot\n");

        osDelay(200);
        cm_pm_reboot();
    }
}

/**
 * 录音为WAV文件并播放
 *
 */
static void app_recorder_wav(void)
{
    /* TTS播放开始录音 */
    if (0 != cm_local_tts_play(APP_RECORDER_TTS_SOUND1, strlen(APP_RECORDER_TTS_SOUND1), __app_recorder_tts_callback, "sound1"))
    {
        cm_log_printf(0, "[TTS] synth play, reboot\n");

        osDelay(200);
        cm_pm_reboot();
    }

    app_tts_is_working = true;

    /* 等待TTS播放完成，__app_recorder_tts_callback()回调中收到相关事件会将app_tts_is_working置为false */
    while(1)
    {
        if (false == app_tts_is_working)
        {
            break;
        }

        osDelay(20);
    }

    /* 开始录音 */
    if (true == cm_fs_exist("record_file1.wav"))
    {
        cm_fs_delete("record_file1.wav");
    }

    app_wav_file_fd = cm_fs_open("record_file1.wav", CM_FS_WB);

    if (app_wav_file_fd < 0)
    {
        cm_log_printf(0, "[%s] fs open fail\n", __func__);
        osDelay(200);
        cm_pm_reboot();
    }

    char header[44] = {0x52,0x49,0X46,0x46, 0x00,0x00,0x00,0x00, 0x57,0x41,0x56,0x45,
                       0x66,0x6D,0x74,0x20, 0x10,0x00,0x00,0x00, 0x01,0x00,0x01,0x00,
                       0x40,0x1f,0x00,0x00, 0x80,0x3E,0x00,0x00, 0x02,0x00,0x10,0x00,
                       0x64,0x61,0x74,0x61, 0x00,0x00,0x00,0x00}; //前44个字节为WAV的头部 其中index[40-43]为后续数据长度 index[4-7]为数据长度+36

    /* 使用前提，文件系统至少预留满足音频文件存放要求的空间 */
    cm_fs_write(app_wav_file_fd, header, ARRAY_SIZE(header));

    cm_audio_sample_param_t frame = {.sample_format = CM_AUDIO_SAMPLE_FORMAT_16BIT, .rate = CM_AUDIO_SAMPLE_RATE_8000HZ, .num_channels = CM_AUDIO_SOUND_MONO};
    cm_audio_recorder_start(CM_AUDIO_RECORD_FORMAT_PCM, &frame, (cm_audio_record_callback_t)__app_pcm_record_cb, "PCM");

    osDelay(1000);

    /* 结束录音 */
    cm_audio_recorder_stop();
    osDelay(40);

    header[40] = app_pcm_data_buff_len & 0xFF;
    header[41] = (app_pcm_data_buff_len >> 8 ) & 0xFF;
    header[42] = (app_pcm_data_buff_len >> 16 ) & 0xFF;
    header[43] = (app_pcm_data_buff_len >> 24 ) & 0xFF;

    header[4] = (app_pcm_data_buff_len+36) & 0xFF;
    header[5] = ((app_pcm_data_buff_len+36) >> 8 ) & 0xFF;
    header[6] = ((app_pcm_data_buff_len+36) >> 16 ) & 0xFF;
    header[7] = ((app_pcm_data_buff_len+36) >> 24 ) & 0xFF;

    cm_fs_seek(app_wav_file_fd, 0, CM_FS_SEEK_SET);
    cm_fs_write(app_wav_file_fd, header, ARRAY_SIZE(header));

    cm_fs_close(app_wav_file_fd);
    app_wav_file_fd = -1;

    int32_t ret = cm_audio_player_stream_open(CM_AUDIO_PLAY_FORMAT_PCM, &frame);

    if (-1 == ret)
    {
        cm_log_printf(0, "%s() __%d__ cm_audio_player_stream_open() error, ret is %d", __func__, __LINE__, ret);
        osDelay(200);
        cm_pm_reboot();
    }

    int32_t index = 0;

    while (index < Tts_sound2_buff_len)
    {
        /* 每200ms传10帧PCM数据 */
        if (index + 3200 <= Tts_sound2_buff_len)
        {
            cm_audio_player_stream_push(Tts_sound2_buff + index, 3200);
            index += 3200;
        }
        else
        {
            if (index >= Tts_sound2_buff_len)     //播放完成
            {
                break;
            }
            else
            {
                cm_audio_player_stream_push(Tts_sound2_buff + index, (Tts_sound2_buff_len - index));
                index = Tts_sound2_buff_len;
            }
        }

        osDelay(40);
    }
    cm_audio_player_stream_close();

    /* 播放录音 */
    int32_t wav_file_size = cm_fs_filesize("record_file1.wav");

    int32_t fd = cm_fs_open("record_file1.wav", CM_FS_RB);

    if (fd < 0)
    {
        cm_log_printf(0, "[%s] fs open fail\n", __func__);
        osDelay(200);
        cm_pm_reboot();
    }

    wave_header wav_header = { 0 };

    cm_fs_read(fd, &wav_header, sizeof(wave_header));

    if (wav_header.riff.riff_id == ID_RIFF && wav_header.riff.wave_id == ID_WAVE && wav_header.chunk.id == ID_FMT)
    {
        cm_log_printf(0, "[%s] test.wav rate[%d] num_channels[%d] sample_format[%d]\n", __func__, wav_header.fmt.sample_rate, wav_header.fmt.num_channels, wav_header.fmt.bits_per_sample);
        frame.rate = wav_header.fmt.sample_rate;
        frame.num_channels = wav_header.fmt.num_channels;

        if (16 == wav_header.fmt.bits_per_sample)
        {
            frame.sample_format = CM_AUDIO_SAMPLE_FORMAT_16BIT;
        }
        else
        {
            cm_log_printf(0, "[%s] test.wav sample is not support\n", __func__);
            osDelay(200);
            cm_pm_reboot();
        }

        cm_audio_player_stream_open(CM_AUDIO_PLAY_FORMAT_PCM, &frame);

        uint8_t* buff = cm_malloc(wav_file_size - sizeof(wave_header));

        cm_fs_read(fd, buff, (wav_file_size - sizeof(wave_header)));

        /* 缓存区约能放在约10秒长度的音频数据（已实际测试结果为准），若播放数据过长，需要有间隔的push数据（例如：每隔200ms时间push 10帧音频数据） */
        cm_audio_player_stream_push(buff, (wav_file_size - sizeof(wave_header)));

        /* 此处因为录音时长为5秒，故可直接延迟5秒等待播放完毕（对于不知道播放时间的场景，需要通过数据长度和PCM格式参数计算出音频数据时长） */
        osDelay(1000);

        /* cm_audio_player_stream_close()接口调用后会等待已传入数据播放完毕后再close */
        cm_audio_player_stream_close();

        cm_fs_close(fd);
    }
    else
    {
        cm_log_printf(0, "[%s] test.wav data error\n", __func__);
        osDelay(200);
        cm_pm_reboot();
    }

    /* 结束播放录音 */

    cm_audio_player_stream_close();
}

/**
 * 录音为AMR文件并播放
 *
 */
static void app_recorder_amr(void)
{
    /* TTS播放开始录音 */
    cm_audio_sample_param_t frame = {.sample_format = CM_AUDIO_SAMPLE_FORMAT_16BIT, .rate = CM_AUDIO_SAMPLE_RATE_8000HZ, .num_channels = CM_AUDIO_SOUND_MONO};
    int32_t ret = cm_audio_player_stream_open(CM_AUDIO_PLAY_FORMAT_PCM, &frame);

    if (-1 == ret)
    {
        cm_log_printf(0, "%s() __%d__ cm_audio_player_stream_open() error, ret is %d", __func__, __LINE__, ret);
        osDelay(200);
        cm_pm_reboot();
    }

    int32_t index = 0;

    while (index < Tts_sound3_buff_len)
    {
        /* 每200ms传10帧PCM数据 */
        if (index + 3200 <= Tts_sound3_buff_len)
        {
            cm_audio_player_stream_push(Tts_sound3_buff + index, 3200);
            index += 3200;
        }
        else
        {
            if (index >= Tts_sound3_buff_len)     //播放完成
            {
                break;
            }
            else
            {
                cm_audio_player_stream_push(Tts_sound3_buff + index, (Tts_sound3_buff_len - index));
                index = Tts_sound3_buff_len;
            }
        }

        osDelay(40);
    }
    cm_audio_player_stream_close();


    /* 开始录音 */
    if (true == cm_fs_exist("record_file1.amr"))
    {
        cm_fs_delete("record_file1.amr");
    }

    app_amr_file_fd = cm_fs_open("record_file1.amr", CM_FS_WB);

    if (app_amr_file_fd < 0)
    {
        cm_log_printf(0, "[%s] fs open fail\n", __func__);
        osDelay(200);
        cm_pm_reboot();
    }

    unsigned char amr_header[6] = {0x23, 0x21, 0x41, 0x4D, 0x52, 0x0A};

    int32_t write_len = cm_fs_write(app_amr_file_fd, amr_header, 6);

    if (write_len != 6)
    {
        cm_log_printf(0, "[%s] fs write fail\n", __func__);
        cm_fs_close(app_amr_file_fd);
        app_amr_file_fd = -1;
        osDelay(200);
        cm_pm_reboot();
    }

    cm_audio_recorder_start(CM_AUDIO_RECORD_FORMAT_AMRNB_475, NULL, (cm_audio_record_callback_t)__app_amr_record_cb, "475");

    osDelay(1600);

    /* 结束录音 */
    cm_audio_recorder_stop();
    osDelay(40);
    cm_fs_close(app_amr_file_fd);
    app_amr_file_fd = -1;

    ret = cm_audio_player_stream_open(CM_AUDIO_PLAY_FORMAT_PCM, &frame);

    if (-1 == ret)
    {
        cm_log_printf(0, "%s() __%d__ cm_audio_player_stream_open() error, ret is %d", __func__, __LINE__, ret);
        osDelay(200);
        cm_pm_reboot();
    }

    index = 0;

    while (index < Tts_sound2_buff_len)
    {
        /* 每200ms传10帧PCM数据 */
        if (index + 3200 <= Tts_sound2_buff_len)
        {
            cm_audio_player_stream_push(Tts_sound2_buff + index, 3200);
            index += 3200;
        }
        else
        {
            if (index >= Tts_sound2_buff_len)     //播放完成
            {
                break;
            }
            else
            {
                cm_audio_player_stream_push(Tts_sound2_buff + index, (Tts_sound2_buff_len - index));
                index = Tts_sound2_buff_len;
            }
        }

        osDelay(40);
    }
    cm_audio_player_stream_close();

    /* 播放录音 */
    cm_audio_play_file("record_file1.amr", CM_AUDIO_PLAY_FORMAT_AMRNB, NULL, __app_player_process_event, "FILEAMR");
    app_player_is_working = true;

    /* 结束播放录音 */
    while(1)
    {
        if (false == app_player_is_working)
        {
            break;
        }

        osDelay(20);
    }

    osDelay(200);
}

/**
 * 录音机线程
 *
 */
void recorder_demo_appimg_enter(char *param)
{
    /* TTS应用初始化 */
    app_recorder_tts_init();

    /* 录音为WAV文件并播放 */
    app_recorder_wav();

    while(1)
    {
        osDelay(400);

        /* 录音为AMR文件并播放 */
        app_recorder_amr();
    }
}

int cm_opencpu_entry(char * param)
{
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "recorder_demo_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    RECORDER_APP_TaskHandle = osThreadNew((osThreadFunc_t)recorder_demo_appimg_enter,0,&app_task_attr);

    return 0;
}
