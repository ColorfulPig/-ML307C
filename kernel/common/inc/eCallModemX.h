/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallModemX.h                                                     */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  IVS and PSAP structures and functions (header file)          */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef MODEM_X_H_
#define MODEM_X_H_

#include <stdio.h>
#include <string.h>
#ifndef COMPILATION_TARGET
#include "SimulationDefines.h"
#endif
#if (COMPILATION_TARGET == MSA)
#include <int_math.h>
#include <MSAfract.h>
#else
extern void ecall_log(const char *format, ...);
#if FEATURE_AUDIO_ECALL_IVS == 0
#define DIAG_FILTER(...)
#define diagPrintf ecall_log
#else
#include "diags.h"
#endif
#endif

#include "eCallDefines.h"
#include "eCallControl.h"
#include "eCallModem.h"
#include "eCallSync.h"
#include "eCallFec.h"
#include "eCallRom.h"
#include "eCallDebug.h"


extern void IVS_ABORT(const char *format, short a, short b);
/* callback functions: to be implemented by the user application */
#if (COMPILATION_TARGET == MSA)

#define DspWarning(ErrorID,NumberOfParameters,MessageBuffPtr,UnusedString)      \
        MainWarningID((ErrorID), (NumberOfParameters), (MessageBuffPtr))

#if (ECALL_DEBUG_PRINTS_LVL>=ECALL_DEBUG_PRINTS_HLP)
#define DIAG_MSA_ECALL_HL
void IvsAbort(INT16 errorEnum, INT16 printOneParam);
#define IVS_ABORT(_format, _printOneParam, _errorEnum) IvsAbort(_errorEnum,_printOneParam)
void LogTopInfo(const char *format, ...);
#define eCallHlPrintf(_format, _param1, _param2, _param3, _header) EcallDebugMessage(ECALL_DEBUG_PRINTS_HLP,_header,_param1,_param2,_param3)
#else
#define IVS_ABORT(_format, ...)
#define LogTopInfo(_format, ...)
#define eCallHlPrintf(_format, _param1, _param2, _param3, _header)
#endif

#if (ECALL_DEBUG_PRINTS_LVL>=ECALL_DEBUG_PRINTS_LLP)
#define DIAG_MSA_ECALL_LL
#define LogInfo(_format, _param1, _param2,_header) EcallDebugMessage(ECALL_DEBUG_PRINTS_LLP, _header, _param1, _param2, NULL)
#else
#define eCallPrintf(_format, _param1, _param2, _header)
#define LogInfo(_format, ...)
#endif

#else
#define DspWarning(ErrorID,NumberOfParameters,MessageBuffPtr,UnusedString)  ecall_log("DspWarning:0x%lx",ErrorID)
#if (ECALL_DEBUG_PRINTS_LVL>=ECALL_DEBUG_PRINTS_HLP)
#define DIAG_MSA_ECALL_HL
#define eCallHlPrintf diagPrintf
#else
#define eCallHlPrintf(_format, _param1, _param2, _param3, _header)
#endif

#if (ECALL_DEBUG_PRINTS_LVL>=ECALL_DEBUG_PRINTS_LLP)
#define DIAG_MSA_ECALL_LL
#define LogInfo ecall_log
#define LogTopInfo ecall_log
#else
#define LogInfo(_format, ...)
#endif

#endif


/*============================================================================*/
/* IVS declarations                                                           */
/*----------------------------------------------------------------------------*/

typedef enum {
    IVSEVENT_SENDINGSTART,
    IVSEVENT_SENDINGMSD,
    IVSEVENT_RESTARTMSD,
    IVSEVENT_CONTROLSYNC,
    IVSEVENT_CONTROLLOCK,
    IVSEVENT_LLACKRECEIVED,
    IVSEVENT_HLACKRECEIVED,
    IVSEVENT_IDLEPOSTRESET,
    IVSEVENT_IDLEPOSTSTART,
    IVSEVENT_TIMEOUT,
    IVSEVENT_TRANSMITMSD,
    IVSEVENT_NACKRECEIVED,
    IVSEVENT_NUM
} IvsEvent;

typedef enum {
    IVS_ABORT_ERROR_TX_DL_INDEX_TOO_LARGE,
    IVS_ABORT_ERROR_TX_CTRL_MSG_SYMBOL_OUTOFRANGE,
    IVS_ABORT_ERROR_SYMBOL_INDEX_OUT_OF_RANGE,
    IVS_ABORT_ERROR_UNKOWN_TYPE_OF_MODULATOR,
    IVS_ABORT_ERROR_TX_STATE_MSG_SYMBOL_OUTOFRANGE,
    IVS_ABORT_ERROR_SYNC_DELAY_OUT_OF_RANGE
} IvsAbortErrorEnum;

/* callback functions: to be implemented by the user application */
void IvsCatchEvent(IvsEvent ie);
void IvsReceiveAck(void);
void IvsReceiveHlack(const UINT8 data);
void IvsLostSyncWithPsap(void);
void IvsTriggerThTimer(void);
void IvsModulateReport(INT16 fastRobustModulation);


/*----------------------------------------------------------------------------*/

void IvsReset(const UINT8 *msd, int length);
void IvsProcess(INT16 *pcm);
void IvsSendStart(void);

void IvsRxReset(void);
void IvsRxProcess(const INT16 *pcm);

void IvsTxReset(const UINT8 *msd, int length);
void IvsMsdUpdate(const UINT8 *msd, int length);
void IvsTxProcess(INT16 *pcm);
void IvsTxState(INT16 msg, INT16 metric, Bool syncLock);
#ifdef USE_IBM_TONE_DETECTION_FIX
void IvsToneCheck(SyncState *sync); /* ibm sync tone detection fix */
Bool IvsGetToneFlag(void); /* ibm sync tone detection fix */
#endif

/*----------------------------------------------------------------------------*/

typedef enum {
    IvsIdle,
    IvsTrigger,
    IvsStart,
    IvsSendMsd,
    IvsAck,
    IvsHlack
} IvsState;

typedef struct {
    IvsState   state;       /* IVS transmitter state - enum UINT32*/
    CtrlTxData ctrl;        /* IVS control struct */
    ModState   mod;         /* IVS modulator struct */

    INT16 rv;               /* redundancy version */
    INT16 ulN;              /* uplink number of frames */
    INT16 ulIndex;          /* uplink frame counter */
    INT16 ulDelay;          /* uplink transmit offset in samples */
    INT16 dlMsgOld;         /* previous control message */

    Bool  pendingStart;     /* indicates pending START message */
    INT16 overallNack;      /* cumulative NACK count */

    INT16 stateCnt[SET_LLMSG + SET_HLMSG + 1];
    INT16 stateIgn[SET_LLMSG + SET_HLMSG + 1];

    UINT32  memCode[NRB_CODE_BUFFER / 4];/*This is ord1 (char = 1Byte) buffer that was changed to INT32 for optimization reasons.*/
    INT16 memDelay[2 * PCM_LENGTH];
} IvsTxData;

typedef struct {
    INT32 memSync[NRS_MEMSYNC];
#ifdef USE_IBM_TONE_DETECTION_FIX
    INT32 memWakeup[2 * (NRF_SYNC + 1)]; /* ibm sync tone detection fix */
#endif
    INT16 memCtrl[NRF_MEMCTRL*PCM_LENGTH];

    INT16 dlData;           /* downlink message symbol */
    INT16 dlIndex;          /* donwlink frame counter */
    INT16 dlMsgCnt;         /* downlink message counter */

    IvsState   state;       /* IVS receiver state */
    CtrlRxData ctrl;        /* IVS control struct */
    SyncState  sync;        /* IVS sync struct */

} IvsRxData;

typedef struct {
    IvsRxData rx;           /* IVS receiver struct */
    IvsTxData tx;           /* IVS transmitter struct */
} IvsData;

IvsData* IvsGetData(void);

//extern void mms_inband_modem_timediff(const struct timeval *start, const struct timeval *end, struct timeval *diff);
#endif /*MODEM_X_H_*/
#endif /*VOICE_ECALL_BUILD*/