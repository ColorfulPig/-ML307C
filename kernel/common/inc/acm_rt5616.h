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

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: codec alc5616 related
*
* Filename: acm_rt5616.h
*
* Target, platform: Common Platform, SW platform
*
* Authors: Chao Liu
*
* Description: Controls alc5616.
*
* Last Updated:
*
* Notes:
*******************************************************************************/

#ifndef _RT5616_H_
#define _RT5616_H_

#include "I2C.h"

#define false 0
#define true  1

#define RT5616_I2C_CotlAddr                 0xD4011000
#define RT5616_I2C_SLAVE_ADDR               0x36
#define RT5616_SLAVE_WRITE_ADDR             0x36
#define RT5616_SLAVE_READ_ADDR              0x37
/* Info */
#define RT5616_RESET                        0x00
#define RT5616_VERSION_ID                   0xfd
#define RT5616_VENDOR_ID                    0xfe
#define RT5616_DEVICE_ID                    0xff
/*  I/O - Output */
#define RT5616_HP_VOL                       0x02
#define RT5616_LOUT_CTRL1                   0x03
#define RT5616_LOUT_CTRL2                   0x05
/* I/O - Input */
#define RT5616_IN1_IN2                      0x0d
#define RT5616_INL1_INR1_VOL                0x0f
/* I/O - ADC/DAC/DMIC */
#define RT5616_DAC1_DIG_VOL                 0x19
#define RT5616_ADC_DIG_VOL                  0x1c
#define RT5616_ADC_BST_VOL                  0x1e
/* Mixer - D-D */
#define RT5616_STO1_ADC_MIXER               0x27
#define RT5616_AD_DA_MIXER                  0x29
#define RT5616_STO_DAC_MIXER                0x2a

/* Mixer - ADC */
#define RT5616_REC_L1_MIXER                 0x3b
#define RT5616_REC_L2_MIXER                 0x3c
#define RT5616_REC_R1_MIXER                 0x3d
#define RT5616_REC_R2_MIXER                 0x3e
/* Mixer - DAC */
#define RT5616_HPO_MIXER                    0x45
#define RT5616_OUT_L1_MIXER                 0x4d
#define RT5616_OUT_L2_MIXER                 0x4e
#define RT5616_OUT_L3_MIXER                 0x4f
#define RT5616_OUT_R1_MIXER                 0x50
#define RT5616_OUT_R2_MIXER                 0x51
#define RT5616_OUT_R3_MIXER                 0x52
#define RT5616_LOUT_MIXER                   0x53
/* Power */
#define RT5616_PWR_DIG1                     0x61
#define RT5616_PWR_DIG2                     0x62
#define RT5616_PWR_ANLG1                    0x63
#define RT5616_PWR_ANLG2                    0x64
#define RT5616_PWR_MIXER                    0x65
#define RT5616_PWR_VOL                      0x66
/* Private Register Control */
#define RT5616_PRIV_INDEX                   0x6a
#define RT5616_PRIV_DATA                    0x6c
/* Format - ADC/DAC */
#define RT5616_I2S1_SDP                     0x70
#define RT5616_ADDA_CLK1                    0x73
#define RT5616_ADDA_CLK2                    0x74

/* Function - Analog */
#define RT5616_GLB_CLK                      0x80
#define RT5616_PLL_CTRL1                    0x81
#define RT5616_PLL_CTRL2                    0x82
#define RT5616_HP_OVCD                      0x8b
#define RT5616_DEPOP_M1                     0x8e
#define RT5616_DEPOP_M2                     0x8f
#define RT5616_DEPOP_M3                     0x90
#define RT5616_CHARGE_PUMP                  0x91
#define RT5616_PV_DET_SPK_G                 0x92
#define RT5616_MICBIAS                      0x93
#define RT5616_A_JD_CTL1                    0x94
#define RT5616_A_JD_CTL2                    0x95
/* Function - Digital */
#define RT5616_EQ_CTRL1                     0xb0
#define RT5616_EQ_CTRL2                     0xb1
#define RT5616_WIND_FILTER                  0xb2
#define RT5616_DRC_AGC_1                    0xb4
#define RT5616_DRC_AGC_2                    0xb5
#define RT5616_DRC_AGC_3                    0xb6
#define RT5616_SVOL_ZC                      0xb7
#define RT5616_JD_CTRL1                     0xbb
#define RT5616_JD_CTRL2                     0xbc
#define RT5616_IRQ_CTRL1                    0xbd
#define RT5616_IRQ_CTRL2                    0xbe
#define RT5616_INT_IRQ_ST                   0xbf
#define RT5616_GPIO_CTRL1                   0xc0
#define RT5616_GPIO_CTRL2                   0xc1
#define RT5616_GPIO_CTRL3                   0xc2
#define RT5616_PGM_REG_ARR1                 0xc8
#define RT5616_PGM_REG_ARR2                 0xc9
#define RT5616_PGM_REG_ARR3                 0xca
#define RT5616_PGM_REG_ARR4                 0xcb
#define RT5616_PGM_REG_ARR5                 0xcc
#define RT5616_SCB_FUNC                     0xcd
#define RT5616_SCB_CTRL                     0xce
#define RT5616_BASE_BACK                    0xcf
#define RT5616_MP3_PLUS1                    0xd0
#define RT5616_MP3_PLUS2                    0xd1
#define RT5616_ADJ_HPF_CTRL1                0xd3
#define RT5616_ADJ_HPF_CTRL2                0xd4
#define RT5616_HP_CALIB_AMP_DET             0xd6
#define RT5616_HP_CALIB2                    0xd7
#define RT5616_SV_ZCD1                      0xd9
#define RT5616_SV_ZCD2                      0xda
#define RT5616_D_MISC                       0xfa
/* Dummy Register */
#define RT5616_DUMMY2                       0xfb
#define RT5616_DUMMY3                       0xfc


/* Index of Codec Private Register definition */
#define RT5616_BIAS_CUR1                    0x12
#define RT5616_BIAS_CUR3                    0x14
#define RT5616_CLSD_INT_REG1                0x1c
#define RT5616_MAMP_INT_REG2                0x37
#define RT5616_CHOP_DAC_ADC                 0x3d
#define RT5616_3D_SPK                       0x63
#define RT5616_WND_1                        0x6c
#define RT5616_WND_2                        0x6d
#define RT5616_WND_3                        0x6e
#define RT5616_WND_4                        0x6f
#define RT5616_WND_5                        0x70
#define RT5616_WND_8                        0x73
#define RT5616_DIP_SPK_INF                  0x75
#define RT5616_HP_DCC_INT1                  0x77
#define RT5616_EQ_BW_LOP                    0xa0
#define RT5616_EQ_GN_LOP                    0xa1
#define RT5616_EQ_FC_BP1                    0xa2
#define RT5616_EQ_BW_BP1                    0xa3
#define RT5616_EQ_GN_BP1                    0xa4
#define RT5616_EQ_FC_BP2                    0xa5
#define RT5616_EQ_BW_BP2                    0xa6
#define RT5616_EQ_GN_BP2                    0xa7
#define RT5616_EQ_FC_BP3                    0xa8
#define RT5616_EQ_BW_BP3                    0xa9
#define RT5616_EQ_GN_BP                     0xaa
#define RT5616_EQ_FC_BP4                    0xab
#define RT5616_EQ_BW_BP4                    0xac
#define RT5616_EQ_GN_BP4                    0xad
#define RT5616_EQ_FC_HIP1                   0xae
#define RT5616_EQ_GN_HIP1                   0xaf
#define RT5616_EQ_FC_HIP2                   0xb0
#define RT5616_EQ_BW_HIP2                   0xb1
#define RT5616_EQ_GN_HIP2                   0xb2
#define RT5616_EQ_PRE_VOL                   0xb3
#define RT5616_EQ_PST_VOL                   0xb4


enum {
    RT5616_OUTPUT_DEVICE_SPEAKER = 0,
    RT5616_OUTPUT_DEVICE_HEADPHONE,
    RT5616_INPUT_DEVICE_MAIN_MIC,
    RT5616_INPUT_DEVICE_HEADSET_MIC,
    RT5616_INPUT_DEVICE_HEADSET_DETECT,
};

enum {
    RT5616_DEVICE_OFF = 0,
    RT5616_DEVICE_ON,
};


INT8 rt5616_reg_dump(void);
int rt5616_init(void);
int codec_rt5616_is_connect(void);
void codec_rt5616_disable_path(void);
void codec_rt5616_enable_headphone(void);
void codec_rt5616_enable_speaker(void);

void codec_rt5616_mute_headphone(int mute);
void codec_rt5616_mute_speaker(int mute);


#endif
