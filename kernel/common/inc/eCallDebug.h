/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallSync.h                                                 */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  synchronization functions (header file)                      */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_DEBUG_H_
#define ECALL_DEBUG_H_

#include "eCallDefines.h"


/*============================================================================*/
/* Debug Defines                                                            */
/*----------------------------------------------------------------------------*/

#define ECALL_DEBUG_PRINTS_OFF 0
#define ECALL_DEBUG_PRINTS_HLP 1
#define ECALL_DEBUG_PRINTS_LLP 2

#if (COMPILATION_TARGET == MSA)
#define ECALL_DEBUG_PRINTS_LVL ECALL_DEBUG_PRINTS_OFF
#else
#ifndef ECALL_DEBUG_PRINTS_LVL
#define ECALL_DEBUG_PRINTS_LVL ECALL_DEBUG_PRINTS_LLP
#endif
#endif

/*============================================================================*/
/* Debug Enums                                                        */
/*----------------------------------------------------------------------------*/
//ICAT EXPORTED ENUM
typedef enum {
    ECALL_DEBUG_HL_MSG_EIM_ABORT_FUNCTION_CALLED = 0,
    ECALL_DEBUG_HL_MSG_ARMED = 1,
    ECALL_DEBUG_HL_MSG_UPDATE_MSD = 2,
    ECALL_DEBUG_HL_MSG_URC_CONTROL_MASK = 3,
    ECALL_DEBUG_HL_MSG_FEATURE_ENABLE = 4,
    ECALL_DEBUG_HL_MSG_CONTROL_UPDATED = 5,
    ECALL_DEBUG_HL_MSG_STATE_AND_MODEM_SYNC_RESET = 6,
    ECALL_DEBUG_HL_MSG_FULL_RESET = 7,
    ECALL_DEBUG_HL_MSG_STATE_RESET = 8,
    ECALL_DEBUG_HL_MSG_CONTROL_RESET = 9,
    ECALL_DEBUG_HL_MSG_TIMER_SET = 10,
    ECALL_DEBUG_HL_MSG_TIMER_OFF = 11,
    ECALL_DEBUG_HL_MSG_TIMER_EXPIERS = 12,
    ECALL_DEBUG_HL_MSG_INT_MUTE_MIC = 13,
    ECALL_DEBUG_HL_MSG_INT_UNMUTE_MIC = 14,
    ECALL_DEBUG_HL_MSG_INT_MUTE_SPK = 15,
    ECALL_DEBUG_HL_MSG_INT_UNMUTE_SPK = 16,
    ECALL_DEBUG_HL_MSG_IVS_CATCH_EVENT = 17,
    ECALL_DEBUG_HL_MSG_IVS_RECEIVE_HL_ACK = 18,
    ECALL_DEBUG_HL_MSG_LOST_SYNC_WITH_PSAP = 19, //HAS DUPLICATE URC REPORT
    ECALL_DEBUG_HL_MSG_IVS_TH_TIMER = 20,
    ECALL_DEBUG_HL_MSG_IVS_MODULATE_REPORT = 21,
    ECALL_DEBUG_HL_MSG_TEST_DATA_BUFFER_PTR_RESET = 22,
    ECALL_DEBUG_HL_MSG_TEST_CONTROL_COMMAND = 23,
    ECALL_DEBUG_HL_MSG_TEST_DATA_COMMAND_INDEX = 24,
    ECALL_DEBUG_HL_MSG_TEST_READ_RX_CTRL_INDEX = 25,
    ECALL_DEBUG_HL_MSG_STATE_AND_MODEM_SYNC_RESET_CHK = 26,
    ECALL_DEBUG_HL_MSG_TEST_DATA_COMMAND_METRIC = 27,
    ECALL_DEBUG_HL_MSG_TEST_READ_RX_CTRL_METRIC = 28,
    ECALL_DEBUG_HL_MSG_TEST_DATA_COMMAND = 29,
    ECALL_DEBUG_HL_MSG_NUM_OPCODES
}EcallDebugHlMsgOpcodeEnum;

//ICAT EXPORTED ENUM
typedef enum {
    ECALL_DEBUG_LL_MSG_SYNCED_LOCK = 0,
    ECALL_DEBUG_LL_MSG_CODEC_INV_DETECT = 1,
    ECALL_DEBUG_LL_MSG_IGNORE_BAD_SYNC = 2,
    ECALL_DEBUG_LL_MSG_WAKEUP_TONE = 3,
    ECALL_DEBUG_LL_MSG_SYNC_FRAME = 4,
    ECALL_DEBUG_LL_MSG_LOWER_LAYER_SYMBOL = 5,
    ECALL_DEBUG_LL_MSG_1ST_HIGHER_SYMBOL = 6,
    ECALL_DEBUG_LL_MSG_2ND_HIGHER_SYMBOL = 7,
    ECALL_DEBUG_LL_MSG_MUTING_FRAME = 8,
    ECALL_DEBUG_LL_MSG_SEND_START_RECEIVED = 9,
    ECALL_DEBUG_LL_MSG_SEND_START_DURING_MSD_IGNORED = 10,
    ECALL_DEBUG_LL_MSG_RESET_RECEIVER = 11,
    ECALL_DEBUG_LL_MSG_RESET_TX_MSD_PROVIDED = 12,
    ECALL_DEBUG_LL_MSG_RESET_TX = 13,
    ECALL_DEBUG_LL_MSG_SEND_START_IGNORED = 14,
    ECALL_DEBUG_LL_MSG_SENDING_SEND = 15,
    ECALL_DEBUG_LL_MSG_FULL_RESET_NO_SYNC = 16,
    ECALL_DEBUG_LL_MSG_IGNORING_ADITIONAL_HALCK = 17,
    ECALL_DEBUG_LL_MSG_RECEIVED_HALCK = 18,
    ECALL_DEBUG_LL_MSG_RECEIVED_HALCK_WAITING = 19,
    ECALL_DEBUG_LL_MSG_RECEIVED_START_IGNORING_MSG = 20,
    ECALL_DEBUG_LL_MSG_RECEIVED_START_TOO_OFTEN = 21,
    ECALL_DEBUG_LL_MSG_RECEIVED_START_STARTING = 22,
    ECALL_DEBUG_LL_MSG_FAST_MODULATOR_CHOSEN = 23,
    ECALL_DEBUG_LL_MSG_ROBUST_MODULATOR_CHOSEN = 24,
    ECALL_DEBUG_LL_MSG_RECEIVED_START = 26,
    ECALL_DEBUG_LL_MSG_PREVIOUS_ACK_WRONG = 27,
    ECALL_DEBUG_LL_MSG_PREVIOUS_HLACK_WRONG = 28,
    ECALL_DEBUG_LL_MSG_NACK_IGNORED_EXPECTING_START = 29,
    ECALL_DEBUG_LL_MSG_NACK_IGNORED_NOT_RELIABLE = 30,
    ECALL_DEBUG_LL_MSG_NACK_IGNORED_ASSUME_IDLE = 31,
    ECALL_DEBUG_LL_MSG_NACK_FIRST_TIME = 32,
    ECALL_DEBUG_LL_MSG_NACK = 33,
    ECALL_DEBUG_LL_MSG_ACK_PREVIOUS_WRONG = 34,
    ECALL_DEBUG_LL_MSG_HLACK_PREVIOUS_WRONG = 35,
    ECALL_DEBUG_LL_MSG_ACK_IGNORED_EXPECTING_START = 36,
    ECALL_DEBUG_LL_MSG_ACK_IGNORED_ALREADY_RECORDED = 37,
    ECALL_DEBUG_LL_MSG_ACK_IGNORED_NOT_RELIABLE = 38,
    ECALL_DEBUG_LL_MSG_ACK_IGNORED_ASSUME_IDLE = 39,
    ECALL_DEBUG_LL_MSG_ACK_RECORDED_WAIT_FOR_HLACK = 40,
    ECALL_DEBUG_LL_MSG_ACK_RECEIVED_WAITING = 41,
    ECALL_DEBUG_LL_MSG_HLACK_WAS_WRONG = 42,
    ECALL_DEBUG_LL_MSG_HLACK_IGNORED_EXPECTING_START = 43,
    ECALL_DEBUG_LL_MSG_IDLE_RECEIVED_IGNORING_MSG = 44,
    ECALL_DEBUG_LL_MSG_IDLE_MODE_REMAINING = 45,
    ECALL_DEBUG_LL_MSG_SYNC_DETECTED = 46,
    ECALL_DEBUG_LL_MSG_SYNC_DETECTED_INV = 47,
    ECALL_DEBUG_LL_MSG_SYNC_DETECTED_INV_TOO = 48,
    ECALL_DEBUG_LL_MSG_MAX_SYNC_RANGE = 49,
    ECALL_DEBUG_LL_MSG_SYNC_SAMPLE_SLIP = 50,
    ECALL_DEBUG_LL_MSG_NUM_OPCODES
}EcallDebugllMsgOpcodeEnum;

/*============================================================================*/
/*Debug Function Declaration                                                            */
/*----------------------------------------------------------------------------*/
void EcallDebugMessage(INT16 llHl, INT16 DebugPoint, INT16 Debug1, INT16 Debug2, INT16 Debug3);

#endif /*ECALL_DEBUG_H_*/
#endif /* VOICE_ECALL_BUILD*/