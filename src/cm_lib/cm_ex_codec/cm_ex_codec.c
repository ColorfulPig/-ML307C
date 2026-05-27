/**
 * @file        cm_ex_codec.c
 * @brief       外置codec（ES8311）接口
 * @copyright   Copyright © 2021 China Mobile IOT. All rights reserved.
 * @author      By cmiot3000
 * @date        2023/03/19
 *
 * @defgroup CODEC CODEC
 * @ingroup CODEC
 * @{
 */

/****************************************************************************
 * Included Files
 ****************************************************************************/
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "cm_os.h"
#include "cm_sys.h"
#include "cm_i2c.h"
#include "AudioHAL.h"

/****************************************************************************
 * Pre-processor Definitions
 ****************************************************************************/

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
/***************参数定义***************/

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

/* 调试所用开发板不含PA，此处设置较大值用于收听，客户开发时此值可能需要修改 */
#define ADC_Volume          191             //ADC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step
#define DAC_Volume          255             //DAC数字增益:(选择范围0~255),191:0DB,±0.5dB/Step

#define Dmic_Selon          0               //DMIC选择:默认选择关闭0,打开为1
#define ADC2DAC_Sel         0               //LOOP选择:内部ADC数据给到DAC自回环输出:默认选择关闭0,打开为1
#define DACHPModeOn         0               //输出负载开启HP驱动:默认选择关闭0,打开为1

/* 基于调试所用开发板电路设计，客户开发时此值可能需要修改 */
#define AUDIO_CODEC_ES8311_SLAVE_WRITE_ADDR    (0x18)
#define AUDIO_CODEC_ES8311_SLAVE_READ_ADDR    (0x18)

/* 基于调试所用开发板电路使用的I2C通道，客户开发时此值可能需要修改 */
#define CODEC_ES8311_I2C_ID        CM_I2C_DEV_1

/***************参数选择***************/

/****************************************************************************
 * Public Types
 ****************************************************************************/

static void cm_ex_codec_i2c_init(void)
{
    int32_t ret = -1;

    cm_i2c_cfg_t config = 
    {
        CM_I2C_ADDR_TYPE_7BIT,
        CM_I2C_MODE_MASTER,
        CM_I2C_CLK_100KHZ
    };

    ret = cm_i2c_open(CODEC_ES8311_I2C_ID, &config);

    if(ret != 0)
    {
        cm_log_printf(0, "i2c init err,ret=%d\n", ret);
    }
}

static void cm_ex_codec_set_i2c_es8311_reg(unsigned char reg, unsigned char val)
{
    int32_t ret = -1;
    unsigned char param_data[2] = {0x00, 0x00};
    unsigned char retry_count = 5;

    param_data[0] = reg;
    param_data[1] = val;

    do {
        ret = cm_i2c_write(CODEC_ES8311_I2C_ID , AUDIO_CODEC_ES8311_SLAVE_WRITE_ADDR, param_data, 2);
        if (0 >= ret)
        {
            cm_log_printf(0, "ret:0x%lx, reg:0x%lx, val:0x%lx, retry_count:0x%lx",ret, reg,val, retry_count);
        }
       else
            break;
    } while (--retry_count);
}

static void I2CWRNBYTE_CODEC(unsigned char reg, unsigned char val)
{
    return cm_ex_codec_set_i2c_es8311_reg(reg, val);
}

static void cm_ex_codec_es8311_codec(void)
{
    cm_log_printf(0, "cm_ex_codec_es8311_codec, Ratio:%d", Ratio);

    I2CWRNBYTE_CODEC(0x45,0x00);
    I2CWRNBYTE_CODEC(0x01,0x30);
    I2CWRNBYTE_CODEC(0x02,0x10);

    if(Ratio == 1536)//Ratio=MCLK/LRCK=1536：12M288-8K；
    {
        I2CWRNBYTE_CODEC(0x02,0xA0);//MCLK DIV=6
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + 0x15);//21：SCLK=512K=12M288/24
        I2CWRNBYTE_CODEC(0x07,0x05);
        I2CWRNBYTE_CODEC(0x08,0xFF);
    }
    if(Ratio == 1500)//Ratio=MCLK/LRCK=1500：12M-8K；
    {
        I2CWRNBYTE_CODEC(0x02,0x90);//MCLK = 12/5*4 =9M6 1200Ratio
        I2CWRNBYTE_CODEC(0x03,0x19);//400
        I2CWRNBYTE_CODEC(0x16,0x20);
        I2CWRNBYTE_CODEC(0x04,0x19);
        I2CWRNBYTE_CODEC(0x05,0x22);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + 0x17);//23：SCLK=400K=12M/30
        I2CWRNBYTE_CODEC(0x07,0x05);//LRCK=8K=12M/1500
        I2CWRNBYTE_CODEC(0x08,0xDB);//LRCK=8K=12M/1500
    }
    if(Ratio == 1280)//Ratio=MCLK/LRCK=1280：24M576-19K2；
    {
        I2CWRNBYTE_CODEC(0x02,0x80);//MCLK DIV=5
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x04);
        I2CWRNBYTE_CODEC(0x08,0xFF);
    }
    if(Ratio == 1024)//Ratio=MCLK/LRCK=1024：8M192-8K；
    {
        I2CWRNBYTE_CODEC(0x02,0x60);//MCLK DIV=4
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x03);
        I2CWRNBYTE_CODEC(0x08,0xFF);
    }
    if(Ratio == 960)//Ratio=MCLK/LRCK=960：15M36-16K；
    {
        I2CWRNBYTE_CODEC(0x02,0x20);//MCLK = 15M36 /2=7M68 480Ratio
        I2CWRNBYTE_CODEC(0x03,0x1E);
        I2CWRNBYTE_CODEC(0x16,0x20);
        I2CWRNBYTE_CODEC(0x04,0x1E);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + 0x17);//23：SCLK=512K=15M36/30
        I2CWRNBYTE_CODEC(0x07,0x03);//LRCK=16K=15M36/960
        I2CWRNBYTE_CODEC(0x08,0xBF);//LRCK=16K=15M36/960
    }
    if(Ratio == 768)//Ratio=MCLK/LRCK=768：12M288-16K；6M144-8K;
    {
        I2CWRNBYTE_CODEC(0x02,0x40);//MCLK DIV=3
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x02);//LRCK=16K=12M288/768
        I2CWRNBYTE_CODEC(0x08,0xFF);//LRCK=16K=12M288/768
    }
    if(Ratio == 750)//Ratio=MCLK/LRCK=750：12M-16K；
    {
        I2CWRNBYTE_CODEC(0x02,0x9A);//MCLK = 12/5*8 =9M6 1200Ratio
        I2CWRNBYTE_CODEC(0x03,0x19);//400
        I2CWRNBYTE_CODEC(0x16,0x21);
        I2CWRNBYTE_CODEC(0x04,0x19);
        I2CWRNBYTE_CODEC(0x05,0x22);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + 0x0E);//23：SCLK=800K=12M/15
        I2CWRNBYTE_CODEC(0x07,0x02);//LRCK=16K=12M/750
        I2CWRNBYTE_CODEC(0x08,0xED);//LRCK=16K=12M/750
    }
    if(Ratio == 544)//Ratio=MCLK/LRCK=544：
    {//外部提供24M-44K1的频率会影响THDN，但是SNR没影响
        I2CWRNBYTE_CODEC(0x02,0x20);//MCLK DIV=2
        I2CWRNBYTE_CODEC(0x03,0x11);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x11);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,0x40 + (SCLK_INV<<5) + SCLK_DIV -1);//SCLK和LRCK无法对齐，所以发数据长度后STOP对对齐
        I2CWRNBYTE_CODEC(0x07,0x02);
        I2CWRNBYTE_CODEC(0x08,0x1F);
    }
    if(Ratio == 512)//Ratio=MCLK/LRCK=512：24M576-48K；8M192-16K
    {
        I2CWRNBYTE_CODEC(0x02,0x20);//MCLK DIV=2
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x01);
        I2CWRNBYTE_CODEC(0x08,0xFF);
    }
    if(Ratio == 500)//Ratio=MCLK/LRCK=500：24M-48K；8M-16K
    {
        I2CWRNBYTE_CODEC(0x02,0x92);//CLK =MCLK/5*4=400
        I2CWRNBYTE_CODEC(0x03,0x19);//ADC 400 FS
        I2CWRNBYTE_CODEC(0x16,0x21);
        I2CWRNBYTE_CODEC(0x04,0x19);//DAC 400 FS
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x01);
        I2CWRNBYTE_CODEC(0x08,0xF3);//500
    }
    if(Ratio == 480)//Ratio=MCLK/LRCK=480：15M36-32K；
    {
        I2CWRNBYTE_CODEC(0x02,0x00);//MCLK = 480Ratio
        I2CWRNBYTE_CODEC(0x03,0x1E);
        I2CWRNBYTE_CODEC(0x16,0x20);
        I2CWRNBYTE_CODEC(0x04,0x1E);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + 0x17);//23：SCLK=512K=15M36/30
        I2CWRNBYTE_CODEC(0x07,0x01);//LRCK=32K=15M36/480
        I2CWRNBYTE_CODEC(0x08,0xDF);//LRCK=32K=15M36/480
    }
    if(Ratio == 384)// Ratio=MCLK/LRCK=384：12M288-32K；6M144-16K；3M072-8K;
    {
        I2CWRNBYTE_CODEC(0x02,0x00);//
        I2CWRNBYTE_CODEC(0x03,0x18);//384
        I2CWRNBYTE_CODEC(0x16,0x21);//384
        I2CWRNBYTE_CODEC(0x04,0x18);//384
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x01);
        I2CWRNBYTE_CODEC(0x08,0x7F);
    }
    if(Ratio == 320)//Ratio=MCLK/LRCK=320：5M12-16K；
    {
        I2CWRNBYTE_CODEC(0x02,0x90);//320 /5 *4 = 256Ratio
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x01);
        I2CWRNBYTE_CODEC(0x08,0x3F);
    }
    if(Ratio == 272)//Ratio=MCLK/LRCK=272：
    {//外部提供12M-44K1的频率会影响THDN，但是SNR没影响
        I2CWRNBYTE_CODEC(0x02,0x00);//MCLK DIV=1
        I2CWRNBYTE_CODEC(0x03,0x11);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x11);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,0x40 + (SCLK_INV<<5) + SCLK_DIV -1);//SCLK和LRCK无法对齐，所以发数据长度后STOP对对齐
        I2CWRNBYTE_CODEC(0x07,0x01);
        I2CWRNBYTE_CODEC(0x08,0x0F);
    }
    if(Ratio == 256)//Ratio=MCLK/LRCK=256：12M288-48K；4M096-16K; 2M048-8K
    {
        I2CWRNBYTE_CODEC(0x02,0x00);//MCLK DIV=1
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0xFF);
    }
    if(Ratio == 250)//Ratio=MCLK/LRCK=250：12M-48K；4M-16K
    {
        I2CWRNBYTE_CODEC(0x02,0x9A);//CLK =MCLK/5*8=400
        I2CWRNBYTE_CODEC(0x03,0x19);//ADC 400 FS
        I2CWRNBYTE_CODEC(0x16,0x21);
        I2CWRNBYTE_CODEC(0x04,0x19);//DAC 400 FS
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0xF9);//250
    }
    if(Ratio == 192)// Ratio=MCLK/LRCK=192：12M288-64K；6M144-32K；3M072-16K; 1M536-8K
    {
        I2CWRNBYTE_CODEC(0x02,0x51);////MCLK = MCLK/3*4 =9M6 256Ratio
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0xBF);
    }
    if(Ratio == 128)// Ratio=MCLK/LRCK=128：6M144-48K；2M048-16K; 1M024-8K
    {
        I2CWRNBYTE_CODEC(0x02,0x08);
        I2CWRNBYTE_CODEC(0x03,0x10);
        I2CWRNBYTE_CODEC(0x16,0x24);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0x7F);
    }
    if(Ratio == 64)// Ratio=MCLK/LRCK=64：3M072-48K；1M024-16K; 512k-8K
    {
        /*当512K/8K时，DVDD必须接1V8，否则无法正常工作*/
        I2CWRNBYTE_CODEC(0x02,0x18);
        I2CWRNBYTE_CODEC(0x03,0x20);
        I2CWRNBYTE_CODEC(0x16,0x20);
        I2CWRNBYTE_CODEC(0x04,0x20);
        I2CWRNBYTE_CODEC(0x05,0x00);
        I2CWRNBYTE_CODEC(0x06,(SCLK_INV<<5) + SCLK_DIV -1);
        I2CWRNBYTE_CODEC(0x07,0x00);
        I2CWRNBYTE_CODEC(0x08,0x3F);
    }

    I2CWRNBYTE_CODEC(0x09,(DACChannelSel<<7) + Format + (Format_Len<<2));
    //I2CWRNBYTE_CODEC(0x09,(1<<7) + Format + (Format_Len<<2));
    I2CWRNBYTE_CODEC(0x0A,Format + (Format_Len<<2));
    //I2CWRNBYTE_CODEC(0x0A,NORMAL_DSPB + (Format_Len<<2) );

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

static void cm_ex_codec_es8311_powerdown(void)
{
    cm_log_printf(0, "cm_ex_codec_es8311_powerdown");

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

static void cm_ex_codec_es8311_control_cb(uint32_t on)
{
    cm_log_printf(0, "cm_ex_codec_es8311_control_cb: on:%d",on);

    if(on){
        cm_ex_codec_es8311_codec();
    }else{
        cm_ex_codec_es8311_powerdown();
    }
}

static void cm_ex_codec_es8311_register_cb(void)
{
    cm_log_printf(0, "cm_ex_codec_es8311_register_cb");

    AudioHAL_AifBindCodec_CB(cm_ex_codec_es8311_control_cb);
}

/**
 *  @brief 外置codec驱动初始化
 *  
 *  
 *  @details 使用外置codec时，须在执行audio相关接口前先调用本接口 \n
 *           默认的codec配置参数仅为示例，用户开发产品时可能需要修改 \n
 *           cm_ex_codec.c以使用es8311芯片，按照中移物联网参考电路设计为示例，若codec芯片或电路变化，用户需进行适配开发
 */
void cm_ex_codec_init(void)
{
    static int inited = 0;
    if(!inited){
        cm_log_printf(0, "cm_ex_codec_init");

        cm_ex_codec_i2c_init();
        cm_ex_codec_es8311_codec();
        cm_ex_codec_es8311_powerdown();
        cm_ex_codec_es8311_register_cb();
        inited = 1;
    }
}
