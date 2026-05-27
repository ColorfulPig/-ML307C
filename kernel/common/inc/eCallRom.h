/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallRom.h                                                  */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  ROM data arrays (header file)                                */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_ROM_H_
#define ECALL_ROM_H_

#include "eCallDefines.h"


/*============================================================================*/
/* Synchronization                                                            */
/*----------------------------------------------------------------------------*/

extern const INT16 wakeupSin500[16];
extern const INT16 wakeupCos500[16];
extern const INT16 wakeupSin800[10];
extern const INT16 wakeupCos800[10];

extern const INT16 syncPulseForm[5];
extern const INT16 syncSequence[15];
extern const INT16 syncIndexPreamble[SYNC_IDXLEN];
extern const INT16 syncFrame[10 * PCM_LENGTH];

/*============================================================================*/
/* Uplink/Downlink format                                                     */
/*----------------------------------------------------------------------------*/

extern const INT16 indexBits[24];

extern const INT16 m4smp_ulPulse[16];
extern const INT16 m8smp_ulPulse[32];
extern const INT16 m4smp_ulPulseMatch[64];
extern const INT16 m8smp_ulPulseMatch[128];

extern const INT16 m4smp_mgTable[66];
extern const INT16 m8smp_mgTable[116];

extern const INT16 dlPcmData[4][NRF_DLDATA*PCM_LENGTH];
extern const INT16 dlPcmDataMatch[4][NRF_DLDATA*PCM_LENGTH];

/*============================================================================*/
/* FEC encoder/decoder                                                        */
/*----------------------------------------------------------------------------*/

extern const UINT16 stateTrans[16];
extern const UINT16 revStateTrans[16];
extern const Ord1  outputParity[16];

extern const Ord1  crcPolynomial[NRB_CRC + 1];
extern const Ord1  scramblingSeq[NRB_INFO_CRC];
extern const UINT16 interleaverSeq[NRB_INFO_CRC];
extern const UINT16 redVerIndex[8][NRB_CODE_ARQ];

extern const IntLLR logExpTable[LOGEXP_RES];

#endif /*ECALL_ROM_H_*/
#endif /*VOICE_ECALL_BUILD*/