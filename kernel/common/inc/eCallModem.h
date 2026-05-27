/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallModem.h                                                */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  modulator and demodulator functions (header file)            */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_MODEM_H_
#define ECALL_MODEM_H_

#include "eCallDefines.h"


typedef enum {
    ModUndef,
    Mod3bit4smp,
    Mod3bit8smp
} ModType;

typedef struct {
    ModType type;      /* identifies modulator type */

    INT16 bpsym;       /* bits per symbol */
    INT16 spmf;        /* samples per modulation frame */
    INT16 mfpf;        /* modulation frames per frame = PCM_LENGTH/spmf */
    INT16 decpos1;     /* position 1st decoding trial */
    INT16 decpos2;     /* position 2nd decoding trial */
    INT16 wutperiod;   /* wakeup tone period in samples */
    INT16 nfmute1;     /* number of muting frames 1st interval */
    INT16 nfmute4;     /* number of muting frames 4th interval */
    INT16 nfmuteall;   /* number of muting frames total */
    INT16 nfdata;      /* number of data frames = NRB_CODE_ARQ/(mfpf*bpsym) */

    const INT16 *ulPulse;
    const INT16 *ulPulseMatch;
    const INT16 *mgTable;
    const INT16 *wakeupSin;
    const INT16 *wakeupCos;
} ModState;


/*============================================================================*/
/* Modem functions                                                            */
/*----------------------------------------------------------------------------*/

void SetModState(ModState*, ModType);

void IvsTransmitter(const ModState*, const Ord1*, INT16*, INT16, INT16);

void SymbolMod(const ModState*, INT16, INT16*);
void SymbolDemod(const ModState*, const INT16*, IntLLR*);

void Byte2Bit(const UINT8*, Ord1*, INT16);
void Bit2Byte(const Ord1*, UINT8*, INT16);

INT32 MpyLacc(INT32, INT16);

#endif /*ECALL_MODEM_H_*/
#endif /*VOICE_ECALL_BUILD*/