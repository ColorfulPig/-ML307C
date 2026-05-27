/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallControl.h                                              */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  control message functions (header file)                      */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_CONTROL_H_
#define ECALL_CONTROL_H_

#include "eCallDefines.h"
#include "eCallSync.h"

//ICAT EXPORTED ENUM
typedef enum {
    DlMsgNoop = -2,
    DlMsgReset,
    DlMsgStart,
    DlMsgNack,
    DlMsgAck,
    DlMsgSend,
    DlMsgHlack = SET_LLMSG + SET_HLMSG
} DlData;

//ICAT EXPORTED ENUM
typedef enum {
    CtrlRxIdle,
    CtrlRxSync,
    CtrlRxLock,
    CtrlTxIdle,
    CtrlTxSend
} PortState;


/*============================================================================*/
/* Control structures                                                         */
/*----------------------------------------------------------------------------*/

typedef struct {
    INT16 dlData;           /* message symbol */
    INT16 dlIndex;          /* message frame counter */
} CtrlTxPort;

typedef struct {
    INT16 dlData;           /* detected message symbol */
    INT16 dlMetric;         /* receiver metric */
} CtrlRxPort;

typedef struct {
    union {
        CtrlTxPort tx;        /* port control transmitter */
        CtrlRxPort rx;        /* port control receiver */
    } u;
    PortState state;        /* port state */
    Bool invert;            /* port inversion flag */
} CtrlPort;

typedef struct {
    CtrlPort port;          /* port struct */
} CtrlTxData;

typedef struct {
    CtrlPort port;          /* port struct */

    SyncState *sync;        /* pointer to sync struct */
    INT16 *buffer;          /* pointer to control receiver buffer */

    Tern  dlRead;           /* sync indication (ternary variable) */
    INT16 dlIndex;          /* internal frame counter */
    INT16 dlSyncLock;       /* number of sync events required */
} CtrlRxData;


/*============================================================================*/
/* Control functions                                                          */
/*----------------------------------------------------------------------------*/

void CtrlSync(CtrlRxData*, const INT16*);

void CtrlTxReset(CtrlTxData*);
void CtrlTxProcess(CtrlTxData*, INT16*);
void CtrlTxMod(INT16*, INT16, INT16);

void CtrlRxReset(CtrlRxData*, SyncState*, INT16*, INT16);
void CtrlRxProcess(CtrlRxData*, const INT16*);
INT16 CtrlRxDemod(const INT16*, INT16*);

#endif /*ECALL_CONTROL_H_*/
#endif /*#ifdef VOICE_ECALL_BUILD*/