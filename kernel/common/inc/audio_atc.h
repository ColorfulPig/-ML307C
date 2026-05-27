/*
* All Rights Reserved
*
* MARVELL CONFIDENTIAL
* Copyright 2012 Marvell International Ltd All Rights Reserved.
* The source code contained or described herein and all documents related to
* the source code ("Material") are owned by Marvell International Ltd or its
* suppliers or licensors. Title to the Material remains with Marvell International Ltd
* or its suppliers and licensors. The Material contains trade secrets and
* proprietary and confidential information of Marvell or its suppliers and
* licensors. The Material is protected by worldwide copyright and trade secret
* laws and treaty provisions. No part of the Material may be used, copied,
* reproduced, modified, published, uploaded, posted, transmitted, distributed,
* or disclosed in any way without Marvell's prior express written permission.
*
* No license under any patent, copyright, trade secret or other intellectual
* property right is granted to or conferred upon you by disclosure or delivery
* of the Materials, either expressly, by implication, inducement, estoppel or
* otherwise. Any license under such intellectual property rights must be
* express and approved by Marvell in writing.
*
*/

#ifndef AUDIO_AT_COMMAND_H
#define AUDIO_AT_COMMAND_H

#define CMD_CODE_AUDIO 0x09

#define SUB_CMD_HANDSHAKE 0x00
#define SUB_CMD_VOLUME_CONTROL 0x01
#define SUB_CMD_MUTE_CONTROL 0x02
#define SUB_CMD_PATH_CONTROL 0x03
#define SUB_CMD_EQ_CONTROL 0x04
#define SUB_CMD_LOOPBACK_CONTROL 0x05
#define SUB_CMD_PCM_RECORDING_CONTROL 0x06
#define SUB_CMD_PCM_PLAYBACK_CONTROL 0x07
#define SUB_CMD_PCM_RECORDING_STREAM 0x08
#define SUB_CMD_PCM_PLAYBACK_STREAM 0x09
#define SUB_CMD_ECALL_CONTROL 0x0a
#define SUB_CMD_PCM_CONTROL   0x0b
#define SUB_CMD_DTMFDETECTION_CONTROL   0x0c



#define CMD_TYPE_EXECUTE 0x01    /*AP to CP: Request the action to CP*/
#define CMD_TYPE_RESPONSE 0x02   /*AP to CP: Response of the INDICATION*/
#define CMD_TYPE_CONFIRM 0x01    /*CP to AP: Response of the EXECUTE*/
#define CMD_TYPE_INDICATION 0x02 /*CP to AP: Request the action or send data to AP*/

#define ATC_COMMON_CFM_LEN_MSG_LEN 0x08

#define ATC_VERSION_NUM 0x12

//ICAT EXPORTED ENUM
typedef enum
{
	ATC_DEFAULT = 0,
    ATC_HANDSET,
    ATC_HEADSET,
    ATC_HANDSFREE,
    ATC_BLUETOOTH,
    ATC_STEREO_BT,
    ATC_SPEAKERPHONE,
    ATC_HEADPHONE,
    ATC_BT_NREC_OFF,
    ATC_BT_WB,
    ATC_BT_NREC_OFF_WB,
    ATC_HANDSET_DUALMIC,
    ATC_HEADSET_DUALMIC,
    ATC_HANDSFREE_DUALMIC,
    ATC_HANDSET_EXTRAVOL_ON,
    ATC_HANDSFREE_EXTRAVOL_ON,
    ATC_HANDSET_DUALMIC_EXTRAVOL_ON,
    ATC_HANDSFREE_DUALMIC_EXTRAVOL_ON,

    ATC_TTY,
    ATC_TTY_HCO,
    ATC_TTY_VCO,
    ATC_TTY_VCO_DUALMIC,

    ATC_HANDSET_LOOP,
	ATC_HEADSET_LOOP,
	ATC_HANDSFREE_LOOP,
	ATC_HEADPHONE_LOOP,
    ATC_STEREO_BT_LOOP,

	ATC_HANDSET_ENH_OFF,
	ATC_HEADSET_ENH_OFF,
	ATC_HANDSFREE_ENH_OFF,
	ATC_HEADPHONE_ENH_OFF,
    ATC_STEREO_BT_ENH_OFF,

    ATC_FM,

    ATC_PATH_NUM
} ATCPath;

typedef struct {
    UINT8 cmd_code;
    UINT8 sub_cmd;
    UINT8 cmd_type;
    UINT8 data_len; /*in byte*/
} ATCHeader;

typedef struct {
    ATCHeader header;
    UINT32 status;
    UINT32 confirm_id;
} ATCCommonCfmMsg;

/*0x00 Handshake Message*/
typedef struct {
    ATCHeader header;
    UINT8  cp_pcm_mode;
    UINT8  wb;
    UINT8  SLIC_Connected;  //0: no SLIC; 1: has SLIC
    UINT8  rsvd[1];
    UINT32 version_num;
    UINT32 confirm_id;
} ATCHandshakeExecMsg;

typedef ATCHandshakeExecMsg ATCHandshakeCfmMsg;

/*0x01 Volume Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  direction;
    UINT8  rsvd[3];
    UINT32 gain;
    UINT32 misc;
    UINT32 confirm_id;
} ATCVolumeCtrlExecMsg;

/*0x02 Mute Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  direction;
    UINT8  mute;
    UINT8  rsvd[2];
    UINT32 confirm_id;
} ATCMuteCtrlExecMsg;

/*0x03 Path Control Message*/
typedef struct {
    ATCHeader header;
    UINT32 path;
    UINT32 confirm_id;
} ATCPathCtrlExecMsg;

/*0x04 EQ Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  rsvd[2];
    UINT16 dha_mode;
    UINT32 dha_ch_flag;
    UINT16 dha_hearing_level[12];
    UINT32 confirm_id;
} ATCEQCtrlExecMsg;

/*0x05 Loopback Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  loopback_test_mode;
    UINT8  rsvd[3];
    UINT32 path;
    UINT32 confirm_id;
} ATCLoopbackCtrlExecMsg;

/*0x06 PCM Recording Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  on_off;
    UINT8  near_far_end;
    UINT8  near_codec_or_vocoder;
    UINT8  rsvd;
    UINT32 callback;
    UINT32 confirm_id;
} ATCPCMRecCtrlExecMsg;

/*0x07 PCM Playback Control Message*/
typedef struct {
    ATCHeader header;
    UINT8  on_off;
    UINT8  near_far_end;
    UINT8  near_codec_or_vocoder;
    UINT8  comb_with_call;
    UINT32 callback;
    UINT32 confirm_id;
} ATCPCMPlayCtrlExecMsg;

/*0x08 PCM Recording Stream Transmit Message*/
typedef struct {
    ATCHeader header;
    UINT32 callback;
    UINT32 confirm_id;
    UINT32 pcm_data_len;
} ATCPCMRecStrmIndiMsg;

/*0x09 PCM Playback Stream Transmit Message*/
typedef struct {
    ATCHeader header;
    UINT32 callback;
    UINT32 confirm_id;
} ATCPCMPlayStrmIndiMsg;

/*0x0A Ecall Control Message */
typedef struct {
    ATCHeader header;
    char ecall_data[160];
	unsigned long data_len;
    UINT32 confirm_id;
} ATCEcallMsg;

/*0x0b PCM Control Message*/
typedef struct {
    ATCHeader header;
    UINT16 on_off;
    UINT16 rsvd;
    UINT32 confirm_id;
} ATCPCMCtrlExecMsg;

/*0x0c DTMF detection Control Message*/
typedef struct {
    ATCHeader header;
    UINT16 onoff;
    UINT16 dialToneToOthersTones;
    UINT16 dialTonesToOthersDialTones;
    UINT16 dialVadDuration;
    UINT32 confirm_id;
} ATCDTMFDetectionCtrlExecMsg;

typedef struct {
    ATCHeader header;
    UINT16 DTMFCode;
} ATCDTMFCodeMsg;


typedef ATCPCMRecStrmIndiMsg ATCPCMPlayStrmRspMsg;

#endif /*AUDIO_AT_COMMAND_H */

