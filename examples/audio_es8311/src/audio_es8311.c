#include "cm_os.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "string.h"
#include "stdbool.h"
#include "cm_audio_player.h"
#include "cm_audio_recorder.h"
#include "cm_fs.h"
#include "cm_mem.h"
#include "cm_i2c.h"
#include "AudioHAL.h"

/***************参数定义***************/
#define STATEconfirm        0xFC            //状态机确认 回读STATEconfirm的寄存值确认IC正常工作状态
#define NORMAL_I2S          0x00
#define NORMAL_LJ           0x01
#define NORMAL_DSPA         0x03
#define NORMAL_DSPB         0x23
#define Format_Len24        0x00
#define Format_Len20        0x01
#define Format_Len18        0x02
#define Format_Len16        0x03
#define Format_Len32        0x04

#define VDDA_3V3            0x00
#define VDDA_1V8            0x01
#define MCLK_PIN            0x00
#define SCLK_PIN            0x01

/***************参数选择***************/
#define MSMode_MasterSelOn  0               //产品主从模式选择:默认选择0为SlaveMode,打开为1选择MasterMode
#define Ratio               64              //实际Ratio=MCLK/LRCK比率，需要和实际时钟比例匹配
#define Format              NORMAL_DSPA     //数据格式选择,需要和实际时序匹配
#define Format_Len          Format_Len16    //数据长度选择,需要和实际时序匹配
#define SCLK_DIV            4               //SCLK分频选择:(选择范围1~20),SCLK=MCLK/SCLK_DIV，超过后非等比增加具体对应关系见相应DS说明
#define SCLK_INV            0               //默认对齐方式为下降沿,1为上升沿对齐,需要和实际时序匹配
#define MCLK_SOURCE         MCLK_PIN        //是否硬件没接MCLK需要用SCLK当作MCLK

#define ADCChannelSel       1               //单声道ADC输入通道选择是CH1(MIC1P/1N)还是CH2(MIC2P/2N)，
#define DACChannelSel       0               //单声道DAC输出通道选择:默认选择0:L声道,1:R声道
#define VDDA_VOLTAGE        VDDA_1V8        //模拟电压选择为3V3还是1V8,需要和实际硬件匹配
#define ADC_PGA_GAIN        8               //ADC模拟增益:(选择范围0~10),具体对应关系见相应DS说明

#define ADC_Volume          200             //ADC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step
#define DAC_Volume          255             //DAC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step

#define Dmic_Selon          0               //DMIC选择:默认选择关闭0,打开为1
#define ADC2DAC_Sel         0               //LOOP选择:内部ADC数据给到DAC自回环输出:默认选择关闭0,打开为1
#define DACHPModeOn         0               //输出负载开启HP驱动:默认选择关闭0,打开为1

#define AUDIO_CODEC_ES8311_IIC_ADDR    (0x18)
#define CODEC_ES8311_I2C_ID        CM_I2C_DEV_1


int32_t app_amr_file_fd = -1;                   //保存至文件系统中的AMR录音文件句柄


osThreadId_t AUDIO_ES8311_APP_TaskHandle = NULL;    //APP主任务句柄


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
        cm_log_printf(0, "CM_AUDIO_PLAY_EVENT_FINISHED %s\n", param);
    }
}

/**
 * 录音为AMR文件并播放
 *
 */
static void app_recorder_amr(void)
{
    /* 开始录音 */
    if (true == cm_fs_exist("record_file.amr"))
    {
        cm_fs_delete("record_file.amr");
    }

    app_amr_file_fd = cm_fs_open("record_file.amr", CM_FS_WB);

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

    /* 播放录音 */
    cm_audio_play_file("record_file.amr", CM_AUDIO_PLAY_FORMAT_AMRNB, NULL, __app_player_process_event, "FILEAMR");

    osDelay(1800);
}

static void I2CWRNBYTE_CODEC(unsigned char reg, unsigned char val)
{
    int32_t ret = -1;
    unsigned char param_data[2] = {0x00, 0x00};
    unsigned char retry_count = 1;

    param_data[0] = reg;
    param_data[1] = val;

    do {
        ret = cm_i2c_write(CODEC_ES8311_I2C_ID , AUDIO_CODEC_ES8311_IIC_ADDR, param_data, 2);
        if (0 >= ret)
        {
            cm_log_printf(0, "ret:0x%lx, reg:0x%lx, val:0x%lx, retry_count:0x%lx",ret, reg,val, retry_count);
        }
       else
            break;
    } while (--retry_count);
}


static void app_codec_es8311_control_cb(uint32_t on)
{
    cm_log_printf(0, "app_codec_es8311_control_cb: on:%d",on);

    if(on)
    {
        I2CWRNBYTE_CODEC(0x45,0x00);
        I2CWRNBYTE_CODEC(0x01,0x30);
        I2CWRNBYTE_CODEC(0x02,0x10);

        /* Ratio=MCLK/LRCK=64：3M072-48K；1M024-16K; 512k-8K */

        /* 当512K/8K时，DVDD必须接1V8，否则无法正常工作 */
        I2CWRNBYTE_CODEC(0x02,0x18);
        I2CWRNBYTE_CODEC(0x03,0x20);
        I2CWRNBYTE_CODEC(0x16,0x20);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0x3F);

        I2CWRNBYTE_CODEC(0x09,(DACChannelSel<<7) + Format + (Format_Len<<2));
        I2CWRNBYTE_CODEC(0x0A,Format + (Format_Len<<2));
        I2CWRNBYTE_CODEC(0x0B,0x00);
        I2CWRNBYTE_CODEC(0x0C,0x00);
        I2CWRNBYTE_CODEC(0x10,(0x1C*DACHPModeOn) + (0x60*VDDA_VOLTAGE) + 0x03);
        I2CWRNBYTE_CODEC(0x11,0x7F);
        I2CWRNBYTE_CODEC(0x00,0x80 + (MSMode_MasterSelOn<<6));//Slave  Mode
        I2CWRNBYTE_CODEC(0x0D,0x01);
        I2CWRNBYTE_CODEC(0x01,0x3F + (MCLK_SOURCE<<7));//做主情况下BCLK只能输出，不能选择引MCLK
        I2CWRNBYTE_CODEC(0x14,(Dmic_Selon<<6) + (ADCChannelSel<<4) + ADC_PGA_GAIN);//选择CH1输入+30DB GAIN
        I2CWRNBYTE_CODEC(0x12,0x28);
        I2CWRNBYTE_CODEC(0x13,0x00 + (DACHPModeOn<<4));
        I2CWRNBYTE_CODEC(0x0E,0x02);
        I2CWRNBYTE_CODEC(0x0F,0x44);
        I2CWRNBYTE_CODEC(0x15,0x00);
        I2CWRNBYTE_CODEC(0x1B,0x0A);
        I2CWRNBYTE_CODEC(0x1C,0x6A);
        I2CWRNBYTE_CODEC(0x37,0x08);
        I2CWRNBYTE_CODEC(0x44,(ADC2DAC_Sel <<7));
        I2CWRNBYTE_CODEC(0x17,ADC_Volume);
        I2CWRNBYTE_CODEC(0x32,DAC_Volume);
    }
    else
    {
        I2CWRNBYTE_CODEC(0x32,0x00);
        I2CWRNBYTE_CODEC(0x17,0x00);
        I2CWRNBYTE_CODEC(0x0E,0xFF);
        I2CWRNBYTE_CODEC(0x12,0x02);
        I2CWRNBYTE_CODEC(0x14,0x00);
        I2CWRNBYTE_CODEC(0x0D,0xFA);
        I2CWRNBYTE_CODEC(0x15,0x00);
        I2CWRNBYTE_CODEC(0x37,0x08);
        I2CWRNBYTE_CODEC(0x02,0x10);
        I2CWRNBYTE_CODEC(0x00,0x00);
        I2CWRNBYTE_CODEC(0x00,0x1F);
        I2CWRNBYTE_CODEC(0x01,0x30);
        I2CWRNBYTE_CODEC(0x01,0x00);
        I2CWRNBYTE_CODEC(0x45,0x00);
        I2CWRNBYTE_CODEC(0x0D,0xFC);
        I2CWRNBYTE_CODEC(0x02,0x00);
    }
}

/**
 * 外置ES8311芯片初始化
 *
 */
static void app_codec_es8311_init(void)
{
    extern int vpathSetMode(uint32_t mode);

    /* 设置为输出输入PCM数字音频信号模式（外置CODEC），上电后仅需设置一次
       注意：默认为PWM输出模拟音频信号模式
       对于使用输出输入PCM数字音频信号模式（外置CODEC）的用户，需调用vpathSetMode(1)接口设置为外置codec模式，并调用AudioHAL_AifBindCodec_CB()注册codec驱动（用户开发，我司提供ES8311芯片驱动demo）后，方可使用
       对于使用PWM输出模拟音频信号模式的用户，设备上电默认为该模式，无需调用vpathSetMode(0)接口进行设置
       对于使用输出输入PCM数字音频信号模式（外置CODEC）的用户，设备上电后需调用vpathSetMode(1)接口进行设置
       不建议多次调用vpathSetMode()接口反复切换模式和播放数据，此操作可能导致数据异常 */
    vpathSetMode(1);


    int32_t ret = -1;

    cm_i2c_cfg_t config = 
    {
        CM_I2C_ADDR_TYPE_7BIT,
        CM_I2C_MODE_MASTER,
        CM_I2C_CLK_100KHZ
    };

    /* IIC初始化 */
    ret = cm_i2c_open(CODEC_ES8311_I2C_ID, &config);

    if(ret != 0)
    {
        cm_log_printf(0, "i2c init err,ret=%d\n", ret);
        cm_pm_reboot();
    }

    /* 外置ES8311驱动注册 */
    AudioHAL_AifBindCodec_CB(app_codec_es8311_control_cb);
}

/**
 * ES8311 CODEC驱动应用线程
 *
 */
void audio_es8311_demo_appimg_enter(char *param)
{
    osDelay(400);

    /* 外置ES8311芯片初始化 */
    app_codec_es8311_init();

    while(1)
    {
        osDelay(200);

        /* 录音为AMR文件并播放 */
        app_recorder_amr();
    }
}

int cm_opencpu_entry(char * param)
{
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "audio_es8311_demo_appimg_enter_demo_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    AUDIO_ES8311_APP_TaskHandle = osThreadNew((osThreadFunc_t)audio_es8311_demo_appimg_enter,0,&app_task_attr);

    return 0;
}
