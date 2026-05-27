/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallFec.h                                                  */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  Turbo encoder and decoder (header file)                      */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_FEC_H_
#define ECALL_FEC_H_

#include "eCallDefines.h"
#include "eCallRom.h"


/*============================================================================*/
/* Encoder functions                                                          */
/*----------------------------------------------------------------------------*/

void FecEncode(Ord1*);

void AttachCrc(const Ord1*, Ord1*);
void Scramble(const Ord1*, Ord1*);
void EncodeTwo(Ord1*);
void EncodeOne(Ord1*, INT16);

/*============================================================================*/
/* Decoder functions                                                          */
/*----------------------------------------------------------------------------*/

Bool FecDecode(const IntLLR*, INT16, Ord1*);

void UpdateBuffer(IntLLR*, const IntLLR*, INT16);
void DecodeBuffer(const IntLLR*, const IntLLR*,
    const IntLLR*, const IntLLR*, Ord1*);

Bool DecodeCrc(const Ord1*);
void Interleave(const IntLLR*, IntLLR*);
void Deinterleave(IntLLR*);
void Descramble(Ord1*);
void Bcjr(const IntLLR*, IntLLR*);

IntLLR GammaQ(INT16, INT16, const IntLLR*, const IntLLR*);
IntLLR JacLog(INT32, INT32);

#endif /*ECALL_FEC_H_*/
#endif /*VOICE_ECALL_BUILD*/