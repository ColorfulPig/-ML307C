/**************************************************************************//**
 *
 * \file        eCallShell.h
 *
 * \brief       Header file of the eCall (shell) Module
 *
 *****************************************************************************/
 /* Programmer   Avishai & Michael
  *****************************************************************************/
#ifdef VOICE_ECALL_BUILD

#ifndef _ECALL_SHELL_H_
#define _ECALL_SHELL_H_

/******************************************************************************
   * Include Files
*****************************************************************************/
#include "cfw_typedef.h"

/******************************************************************************
    * Defines
*****************************************************************************/
#define ECALL_NUM_OF_FRAME_PER_SECOND (50)
#define ECALL_T3_DEFAULT_SEC (2)
#define ECALL_T5_DEFAULT_SEC (5)
#define ECALL_T6_DEFAULT_SEC (5)
#define ECALL_T7_DEFAULT_SEC (20)
#define ECALL_TH_FAST_DEFAULT_SEC (1.2)
#define ECALL_TH_ROBUST_DEFAULT_SEC (2.4)

#define ECALL_T3_DEFAULT_FRAMES (ECALL_T3_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND)
#define ECALL_T5_DEFAULT_FRAMES (ECALL_T5_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND)
#define ECALL_T6_DEFAULT_FRAMES (ECALL_T6_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND)
#define ECALL_T7_DEFAULT_FRAMES (ECALL_T7_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND)
#define ECALL_TH_FAST_DEFAULT_FRAMES  ((INT16)(ECALL_TH_FAST_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND))
#define ECALL_TH_ROBUST_DEFAULT_FRAMES ((INT16)(ECALL_TH_ROBUST_DEFAULT_SEC * ECALL_NUM_OF_FRAME_PER_SECOND))
#define ECALL_SP_MUTE_AF_START_DEFAULT_FRAMES 60	/* 120 x10ms , in order to override set to 0 */
#define ECALL_SP_MUTE_AF_SYNC_DEFAULT_FRAMES 85	/* 170 x10ms , in order to override set to 0 */

#define ECALL_SHELL_DISABLE_URC (UINT32)0
#define ECALL_SHELL_ENABLE_ALL_URC (UINT32)0xffffffff

/******************************************************************************
 * Enumerations
 *****************************************************************************/

typedef enum {

    _ECALL_DATA_SET = 0,			//0x0078
    _ECALL_DATA_GET = 1,			//0x0079
    _ECALL_VOICE_SET = 2,			//0x007a
    _ECALL_VOICE_GET = 3,			//0x007b
    _ECALL_TEST_CONTROL = 4,		//0x007c
    _ECALL_TEST_DATA_CONTROL = 5,	//0x007d
    _ECALL_CONNECTED = 6
}_EcallIpcCmdSubOpEnum;

typedef enum {
    _ECALL_DATA_INDICATION = 0, 	//0x0077
    _ECALL_DATA_GET_MSG = 1,	    //0x0078
    _ECALL_VOICE_INDICATION = 2,    //0x0079
    _ECALL_VOICE_GET_MSG = 3,	    //0x007a
    _ECALL_TEST_DATA_ACK = 4,	    //0x007b
    _ECALL_TEST_TX_BUFFER_END = 5,  //0x007c
    _ECALL_TEST_RX_CTRL = 6,	    //0x007d
    _ECALL_DEBUG = 7		        //0x007e
}_EcallIpcMsgSubOpEnum;


//ICAT EXPORTED ENUM
typedef enum {
    _ECALL_CMD_EIM_ABORT_TERMINATED = 0,
    _ECALL_CMD_EIM_ARM_ACTIVATE = 1,
    _ECALL_CMD_UPDATE_MSD = 2,
    _ECALL_CMD_URC_CONTROL = 3,
    _ECALL_CMD_FEATURE_CONTROL = 4,
    _ECALL_CMD_TIMERS_UPDATE = 5,
    _ECALL_CMD_MSD_UPDATE_CONFIG = 6,
    _ECALL_CMD_CALL_CONNECTED = 7,
    _ECALL_CMD_NUM_OPCODES
} _EcallUecallDataCmdIdEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_MSG_REPORT_HL_ACK = 0,/*0*/
    ECALL_MSG_LOST_SYNC_WITH_PSAP = 1,/*1*/
    ECALL_MSG_IVS_EVENT_NACK_RECIVED = 2,/*2*/
    ECALL_MSG_RESERVED_3 = 3,/*3*/
    ECALL_MSG_RESERVED_4 = 4,/*4*/
    ECALL_MSG_RESERVED_5 = 5,/*5*/ /*reserved for ECALL_MSG_EIM_TER_AUDIO_ERROR*/
    ECALL_MSG_EIM_TER_FATTAL_ERROR = 6,/*6*/
    ECALL_MSG_EIM_TER_CALL_DROP = 7,/*7*/ /*reserved for Modem use, not to be used eCall*/
    ECALL_MSG_RESERVED_8 = 8,/*8*/ /*reserved for ECALL_MSG_GENERIC_EIM_ERROR*/
    ECALL_MSG_IVS_EVENT_REQUEST_MSD = 9,/*9*/
    ECALL_MSG_EIM_START_SENDING_MSD = 10,/*A*/
    ECALL_MSG_IVS_EVENT_SENDING_START = 11,/*B*/
    ECALL_MSG_IVS_EVENT_SENDING_MSD = 12,/*C*/
    ECALL_MSG_IVS_EVENT_RESTART_MSD = 13,/*D*/
    ECALL_MSG_IVS_EVENT_CONTROL_SYNC = 14,/*E*/
    ECALL_MSG_IVS_EVENT_CONTROL_LOCK = 15,/*F*/
    ECALL_MSG_IVS_EVENT_LL_ACK_RECIVED = 16,/*10*/
    ECALL_MSG_IVS_EVENT_HL_ACK_RECIVED = 17,/*11*/
    ECALL_MSG_IVS_EVENT_IDLE_POST_RESET = 18,/*12*/
    ECALL_MSG_IVS_EVENT_IDLE_POST_START = 19,/*13*/
    ECALL_MSG_MSD_UPDATE_REQUEST_RECIVED = 20,/*14*/ /*MSD update deferred since the IVS is currently transmitting*/
    ECALL_MSG_MSD_UPDATE_DONE = 21,/*15*/ /*The IVS transmitter is ready to send the new MSD at next PULL request*/
    ECALL_MSG_MSD_UPDATE_REQUEST_FAIL = 22,/*16*/ /*MSD update request cannot be processed*/
    ECALL_MSG_T3_EXPIRED = 23,/*17*/
    ECALL_MSG_T5_EXPIRED = 24,/*18*/
    ECALL_MSG_T6_EXPIRED = 25,/*19*/
    ECALL_MSG_T7_EXPIRED = 26,/*1A*/
    ECALL_MSG_TH_EXPIRED = 27,/*1B*/
    ECALL_MSG_TIMER_SP_MUTE_AF_START = 28,/*1C*/
    ECALL_MSG_TIMER_SP_MUTE_AF_SYNC = 29,/*1D*/
    ECALL_MSG_RESERVED_30 = 30,/*1E*/
    ECALL_MSG_IVS_EVENT_TIMEOUT = 31,/*1F*/
    ECALL_MSG_NUM_OPCODES
}EcallMsgOpcodeEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_CURRENT = 0,
    ECALL_NEXT = 1,
    ECALL_UPDATE_POS_NUM
}EcallUpdatePosEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_UECALLVOICE_CMD_ID_RESERVED = 0,
    ECALL_UECALLVOICE_CMD_ID_EIM_INTERNAL_CONTROL = 1,
    ECALL_UECALLVOICE_CMD_ID_MUTE_CONTROL = 2,
    ECALL_UECALLVOICE_CMD_ID_URC_CONTROL = 3,
    ECALL_UECALLVOICE_CMD_ID_WA_EN = 4,
    ECALL_UECALLVOICE_CMD_ID_NUM
}EcallUecallvoiceCmdIdEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_MICROPHONE = 0,
    ECALL_SPEAKER = 1,
    ECALL_VOICE_RESOUCE_NUM
}EcallVoiceResouceEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_EIM_INTERNAL_CONTROL_DISABLED = 0,
    ECALL_EIM_INTERNAL_CONTROL_ENABLED = 1,
    ECALL_EIM_INTERNAL_CONRTOL_NUM
}EcallEimInternalCntrlEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_MUTE = 0,
    ECALL_UNMUTE = 1,
    ECALL_MUTE_EXTERNAL = 3,
    ECALL_UNMUTE_EXTERNAL = 4,
    ECALL_MUTE_CONTROL_NUM
}EcallMuteControlEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_URC_STATE_DISABLED = 0,
    ECALL_URC_STATE_ENABLED = 1,
    ECALL_URC_STATE_NUM,
}EcallUrcStateEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_RES_STATE_MUTED = 0,
    ECALL_RES_STATE_UNMUTED = 1,
    ECALL_RES_STATE_NUM,
}EcallResStateEnum;


//ICAT EXPORTED ENUM
typedef enum {
    Udef = -1,
    Ivs,
    IvsRx/*Test Mode*/,
    IvsTx /*TestMode*/
} EcallShellRunModeEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_CURRENT_CONTROL_INVALID = 0,
    ECALL_SHELL_CURRENT_CONTROL_VALID = 1,
    ECALL_SHELL_CURRENT_CONTROL_VALID_NUM
}EcallShellControlValidEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_DISARMED = 0,
    ECALL_SHELL_ARMED = 1,
    ECALL_SHELL_ARMED_NUM
}EcallShellArmedEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_PULL = 0,
    ECALL_SHELL_PUSH = 1,
    ECALL_SHELL_START_MODE_NUM
}EcallShellStartModeEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_UPDATE_ON_IDLE = 0,
    ECALL_SHELL_UPDATE_IMM = 1,
    ECALL_SHELL_NO_UPDATE_NEEDED = 2,
    ECALL_SHELL_MSD_UPDATE_MODE_NUM
}EcallShellMsdUpdateModeEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_DISABLE_FEATURE = 0,/*Factory Programed Value*/
    ECALL_SHELL_ENABLE_FEATURE = 1,
    ECALL_SHELL_FEATURE_MODE_NUM
}EcallShellFeatureEnableEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_SHELL_DATA_SET = 0,
    ECALL_SHELL_VOICE_SET = 1,
    ECALL_SHELL_VOICE_GET = 2,
    ECALL_SHELL_IPC_CMD = 4,
    ECALL_SHELL_IPC_MSG = 5,
    ECALL_SHELL_SET_GET_MODE_NUM
}EcallShellSetGetEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_TIMER_3,
    ECALL_TIMER_5,
    ECALL_TIMER_6,
    ECALL_TIMER_7,
    ECALL_TIMER_H,
    ECALL_TIMER_SP_MUTE_AF_START,
    ECALL_TIMER_SP_MUTE_AF_SYNC,
    ECALL_NUM_OF_TIMERS
}EcallShellTimerEnum;


//ICAT EXPORTED ENUM
typedef enum {
    ECALL_NO_RESET,
    ECALL_RESET_ONLY_IVS_MODEM,
    ECALL_RESET_ONLY_STATE,
    ECALL_RESET_IVS_MODEM_AND_STATE,
    ECALL_RESET_IVS_TX_ONLY_MODEM_AND_STATE
}EcallShellResetEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_IM_SATE_IDLE,
    ECALL_IM_SATE_CONNECTING_NO_MUTE, /* ibm sync tone detection fix */
    ECALL_IM_SATE_CONNECTING,
    ECALL_IM_SATE_CONNECTED,
    ECALL_IM_SATE_TRANSMITTING
}EcallShellImStateEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_WA_ID_SYNC_TONE_FA,/* ibm sync tone detection fix */
    ECALL_WA_ID_INIT_COUNT,
    ECALL_WA_ID_RESERVED2,
    ECALL_WA_ID_NUM
}EcallShellWaIdEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_FAST_MOD = 1, //Mod3bit4smp
    ECALL_ROBUST_MOD = 2,  //Mod3bit8smp
    ECALL_NUM_OF_MOD_TYPE = 2
}
EcallShellModTypeEnum;//This should be aligned to ModType ENUM at eCallModem.H

/******************************************************************************
 * Struts
 *****************************************************************************/

typedef struct {
    EcallMuteControlEnum mute;
    EcallEimInternalCntrlEnum eimInternalControl;
    EcallUrcStateEnum urcStateVoiceResource;
} EcallVoiceResStruct;

typedef struct {
    EcallVoiceResStruct mic;
    EcallVoiceResStruct spk;
} EcallVoiceControlStruct;

typedef struct {
    EcallVoiceControlStruct voiceControl;
    EcallShellRunModeEnum runMode;
    EcallShellArmedEnum ecallArmed;
    EcallShellStartModeEnum startMode;
    EcallShellMsdUpdateModeEnum updateMsdMode;
    EcallShellMsdUpdateModeEnum updateMsdModeShadow;
    UINT32 urcMode;
    EcallShellFeatureEnableEnum featureEnable;
    EcallShellResetEnum resetMode;
    INT16 TimerValuesInFrames[ECALL_NUM_OF_TIMERS];
    INT16 ExternalPullMode;
    BOOL timerControl[ECALL_NUM_OF_TIMERS];
    INT16 wa_id_flag[ECALL_WA_ID_NUM];
} EcallShellControlStruct;


typedef struct {
    EcallResStateEnum micResState;
    EcallResStateEnum spkResState;
    INT16 armedCntr;
    INT16 msdUpdateStatus;
    INT16 timerState[ECALL_NUM_OF_TIMERS];
    INT16 modulationType;
    INT16 syncCntr;
    BOOL firstFrameOfECallSession;
    BOOL firstStartOfEcallSession;
    BOOL firstSpkMuteFrame;
    BOOL msdUpdateStatusReportedOnce;
    BOOL waitForMsdUpdate;
    BOOL firstMsdSent;
    BOOL msdTransmitting;
    EcallShellImStateEnum imState;
} EcallShellStateStruct;

/*********************************************************************************
 * External Control Functions Prototype - For Code located on ROM (use ADP_LYR)
 *********************************************************************************/
void ECallDataIpcCmd(INT16 *cmdPtr, INT16 CmdLength);
void ECallDataSet(INT16 *cmdPtr, INT16 CmdLength);
void ECallDataQueryCmd(const INT16 *cmdPtr);
void ECallVoiceSet(INT16 *cmdPtr);
void ECallVoiceGet(INT16 *cmdPtr, INT16 CmdLength);
void ECallUpdateCurrentControl(void);
void ECallEim(INT16 *EimTxBuffer);
void ECallIntegrateMsdtoEim(void);
void ECallMuteFrame(INT16 *inPtr, INT16 pcmRate);
void ECallStateAndModemResetCheck(void);

/******************************************************************************
 * External Control Functions Prototype - For Code located on DDR
 *****************************************************************************/
void ECallEimAbort(void);

/**************************************************************************
* External Get functions prototype - For Code located on ROM (use ADP_LYR)
**************************************************************************/
BOOL ECallIsFeatureEnabled(void);
BOOL ECallIsArmed(void);
BOOL ECallIsCurrentControlInvalid(void);
INT16 ECallIsSpkUnmute(void);
INT16 ECallIsMicUnmute(void);

void Ecall_app(short *VoiceTxBuffer);

#endif
#endif /*VOICE_ECALL_BUILD*/