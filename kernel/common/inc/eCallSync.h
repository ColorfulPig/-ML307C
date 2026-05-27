/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallSync.h                                                 */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  synchronization functions (header file)                      */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_SYNC_H_
#define ECALL_SYNC_H_

#include "eCallDefines.h"


typedef struct {
    INT32 amplitude[2];       /* amplitudes (average, maximum) */
    INT32 shape[2 * NRS_CP + 1];  /* shape of peak causing a sync event */

    Bool  flag;               /* indicates sync success */
    INT16 delay;              /* synchronization delay */
    INT16 npeaks;             /* number of sync peaks detected */
    INT16 npeaksChk;          /* number of sync peaks detected by sync check */
} SyncSub;

typedef struct {
    INT32 *mem;               /* memory for sync */
    INT32 *memWakeup;         /* memory for wakeup tone detector */

    SyncSub syncPos;          /* regular sync (non-inverted) */
    SyncSub syncNeg;          /* inverted sync */

    INT32 amplitude[3];       /* amplitudes (average, maximum, memory) */
    INT32 shape[2 * NRS_CP + 1];  /* shape of peak causing a sync event */

    Bool flag;                /* indicates sync success */
    Bool invert;              /* indicates sync inversion */
    Bool resync;              /* indicates resynchronization event */

    INT16 delay;              /* synchronization delay */
    INT16 delayMem;           /* synchronization delay (memory) */
    INT16 npeaks;             /* number of sync peaks detected */
    INT16 npeaksMem;          /* number of sync peaks detected (memory) */
    INT16 events;             /* number of subsequent equal sync events */

    Tern  check;              /* indicates sync check result (ternary variable) */
    INT16 checkCnt;           /* counter for subsequent sync check failures */
    INT16 index;              /* frame reference for sync evaluation */
#ifdef USE_IBM_TONE_DETECTION_FIX
    Bool toneFlag;			/* Indicates presence of tone , 500 Hz or 800 Hz , ibm sync tone detection fix */
#endif
} SyncState;


/*============================================================================*/
/* Sync functions                                                             */
/*----------------------------------------------------------------------------*/

void SyncReset(SyncState*, INT32*, INT32*);
void SyncSubReset(SyncSub*);

void Sync(SyncState*, const INT16*, Bool);
void SyncCheck(SyncState*, const INT16*);

void SyncFilter(SyncState*, const INT16*, Bool);

void SyncSubPut(const SyncState*, SyncSub*);
void SyncSubGet(SyncState*, const SyncSub*);
void SyncSubCpy(const SyncSub*, SyncSub*);

void SyncSubRun(SyncSub*,
    const INT32*, const INT32*, const INT32*, const INT32*);
void SyncSubChk(SyncSub*,
    const INT32*, const INT32*, const INT32*, const INT32*, INT16);

INT16 PeakUpdate(const INT32*, const INT32*, INT16);
void PeakCheck(SyncSub*, const Bool*, const INT16*,
    const INT32*, const INT32*, const INT32*,
    INT16, INT16, INT16, INT16);

void ToneDetect(SyncState*, const INT16*);

#endif /*ECALL_SYNC_H_*/
#endif /*VOICE_ECALL_BUILD*/