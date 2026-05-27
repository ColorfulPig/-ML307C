/*============================================================================*/
/* eCall ANSI C fixed-point reference source code                             */
/*                                                                            */
/* File:         EcallDefines.h                                              */
/* Version:      8.6.0 (Rel8) / 9.4.0 (Rel9)                                  */
/* Date:         2011-02-08                                                   */
/* Description:  constants and common data structures                         */
/*----------------------------------------------------------------------------*/
#ifdef VOICE_ECALL_BUILD

#ifndef ECALL_DEFINES_H_
#define ECALL_DEFINES_H_

#include "cfw_typedef.h"
#include "ecall_utils.h"
#define segment(_a)


/*============================================================================*/
/* Global Flags                                                       */
/*----------------------------------------------------------------------------*/

/*============================================================================*/
/* General conventions                                                        */
/*----------------------------------------------------------------------------*/

#if FEATURE_AUDIO_ECALL_IVS == 0
typedef enum { False, True } Bool;
#else
#include "gbl_types.h"
#ifndef False
#define False   0
#endif
#ifndef True
#define True    1
#endif
#endif

typedef enum { Minus = -1, Zero, Plus } Tern;
typedef unsigned char        Ord1;        /*  1 bit variable                  */

#ifndef MAX
#define MAX(a,b) ((a)>(b) ? (a) : (b))    /* macro: maximum                   */
#endif

#ifndef MIN
#define MIN(a,b) ((a)<(b) ? (a) : (b))    /* macro: minimum                   */
#endif

#ifndef ABS
#define ABS(a)   ((a)< 0 ? -(a) : (a))    /* macro: absolute value            */
#endif

#if (COMPILATION_TARGET != MSA)
#define MAX16(a,b) MAX(a,b)    /* macro: maximum                   */
#define MAX32(a,b) MAX(a,b)    /* macro: maximum                   */
#else
#define MAX16(a,b) max_fr1x16(a,b)    /* macro: maximum of INT16                   */
#define MAX32(a,b) max_fr1x32(a,b)    /* macro: maximum of INT32          */
#endif

#if (COMPILATION_TARGET != MSA)
#define MIN16(a,b) MIN(a,b)    /* macro: maximum                   */
#define MIN32(a,b) MIN(a,b)    /* macro: maximum                   */
#else
#define MIN16(a,b) min_fr1x16(a,b)    /* macro: maximum of INT16                   */
#define MIN32(a,b) min_fr1x32(a,b)    /* macro: maximum of INT32          */
#endif

#if (COMPILATION_TARGET != MSA)
#define ABS16(a) ABS(a)    /* macro: maximum                   */
#define ABS32(a) ABS(a)    /* macro: maximum                   */
#else
#define ABS16(a) abs_fr1x16(a)    /* macro: maximum of INT16                   */
#define ABS32(a) abs_fr1x32(a)    /* macro: maximum of INT32          */
#endif

#ifndef SIGN
#define SIGN(a)  ((a)< 0 ? (-1) : (1))    /* macro: sign                      */
#endif

#ifndef PCM_LENGTH
#define PCM_LENGTH        (160)           /* length of PCM frame in samples   */
#endif
#ifndef MSD_MAX_LENGTH
#define MSD_MAX_LENGTH    (140)           /* length of MSD message in bytes   */
#endif
#ifndef PCM_MIN
#define PCM_MIN           (-32768)//(0x8000)/*SHRT_MIN*/      /* minimum PCM value                */
#endif
#ifndef PCM_MAX
#define PCM_MAX           (0x7fff)/*SHRT_MAX*/      /* maximum PCM value                */
#endif

static __inline INT16 ModNonP2(INT16 num, INT16 mod)
{
    /*
    INT16 test;
    while(num>=mod)
    {
        num-=mod;
    }

    test = rem_32_16_primitive(num,mod);
    fprintf (stderr, "%d mod %d = %d = %d\n",num,mod,test,num);
    return num;
    */


    return num % mod;

}

#define IS_NOT_POWER_OF_TWO(_a) ((_a & (_a - 1)) != 0)

#if (COMPILATION_TARGET == MSA)
__inline void eCallMemset32(INT32 * src, INT16 pattern, INT16 sizeInBytes)
{
    INT16 i, sizeInDWords;
    UINT32 pattern32 = create_fr2x32(pattern, pattern);
    sizeInDWords = sizeInBytes / 4;



    for (i = 0; i < sizeInDWords; i++)
    {
        *src++ = pattern32;
    }

}

__inline void eCallMemset16(INT16 * src, INT16 pattern, INT16 sizeInBytes)
{
    INT16 i, sizeInWords;
    sizeInWords = sizeInBytes / 2;


    for (i = 0; i < sizeInWords; i++)
    {
        *src++ = pattern;
    }

}

__inline void eCallCopy32(void * destPtr, const void * srcPtr, UINT16 sizeInBytes)
{
    INT16 i;
    INT16 sizeInDWords;
    UINT32 * currSrcPtr = (UINT32 *)srcPtr;
    UINT32 * currDestPtr = (UINT32 *)destPtr;

    sizeInDWords = sizeInBytes / 4;


    for (i = 0; i < sizeInDWords; i++)
    {
        *currDestPtr++ = *currSrcPtr++;
    }
}

__inline void eCallCopy16(void * destPtr, const void * srcPtr, UINT16 sizeInBytes)
{
    INT16 i;
    INT16 sizeInWords;
    UINT16 * currSrcPtr = (UINT16 *)srcPtr;
    UINT16 * currDestPtr = (UINT16 *)destPtr;

    sizeInWords = sizeInBytes / 2;


    for (i = 0; i < sizeInWords; i++)
    {
        *currDestPtr++ = *currSrcPtr++;
    }
}
#endif

#if (COMPILATION_TARGET == MSA)
#define memset32(src, pattern, sizeInBytes) eCallMemset32(src,  pattern, sizeInBytes)
#define memset16(src, pattern, sizeInBytes) eCallMemset16(src,  pattern, sizeInBytes)

#define memcpy32(dest, src, size) eCallCopy32(dest, src, size)
#define memcpy16(dest, src, size) eCallCopy16(dest, src, size)
#define memcpy(dest, src, size) CfwUtilsMemCopy(src, dest, size)
#else
#define memset32(src, pattern, sizeInBytes) memset(src,  pattern, sizeInBytes)
#define memset16(src, pattern, sizeInBytes) memset(src,  pattern, sizeInBytes)

#define memcpy32(dest, src, size) memcpy(dest, src, size)
#define memcpy16(dest, src, size) memcpy(dest, src, size)
#endif

/*============================================================================*/
/* Synchronization                                                            */
/*----------------------------------------------------------------------------*/

#define SYNC_BADCHECK     (3)             /* sync consecutive bad check       */
#define SYNC_BADTRACK     (4)             /* sync consecutive bad track       */
#define SYNC_IDXLEN       (75)            /* sync index length                */
#define SYNC_THRESHOLD    (10000000 /*10e6*/)          /* sync threshold                   */

#define LOCK_RESYNC       (2)             /* messages to lock after sync loss */
#define LOCK_START_UL     (2)             /* START messages to lock sync (UL) */
#define LOCK_START_DL     (3)             /* START messages to lock sync (DL) */
#define FAIL_RESTART      (3)             /* START messages to restart        */

#define NRF_WAKEUP        (3)             /* number of wakeup frames          */
#define NRF_SYNC          (13)            /* number of sync frames            */
#define NRF_OBSERVE       (10)            /* number of sync observer frames   */
#define NRF_RESYNC        (60)            /* resync frames after sync loss    */

#define NRS_CHECK         (480)           /* number of samples to check       */
#define NRS_TRACK         (240)           /* number of samples to track       */
#define NRS_CP            (2)             /* number of samples next to peaks  */

#define PNSEQ_OSF         (22)            /* oversampling of PN sequence      */
#define PEAK_DIST_PP      (30*PNSEQ_OSF)  /* distance outer positive peaks    */
#define PEAK_DIST_NN      (54*PNSEQ_OSF)  /* distance negative peaks          */
#define PEAK_DIST_PN      (12*PNSEQ_OSF)  /* distance positive to negative    */

#define WAKEUP_500_Len     (16)            /* Array Length of Wakup sin\cos (500Hz)*/
#define WAKEUP_800_Len     (10)            /* Array Length of Wakup sin\cos (800Hz)*/

/*============================================================================*/
/* Uplink/Downlink format                                                     */
/*----------------------------------------------------------------------------*/

#define ARQ_MAX           (8)             /* number of redundancy versions    */
#define NRB_TAIL          (3)             /* number of encoder tail bits      */
#define NRB_CRC           (28)            /* order of CRC polynomial          */

#define NRB_INFO          (8*MSD_MAX_LENGTH)
#define NRB_INFO_CRC      (8*MSD_MAX_LENGTH + NRB_CRC)
#define NRB_CODE_ARQ      (1380)
#define NRB_CODE_BUFFER   (3*(8*MSD_MAX_LENGTH + NRB_CRC) + 4*NRB_TAIL)

#define SET_LLMSG         (16)            /* set size lower-layer messages    */
#define SET_HLMSG         (16)            /* set size higher-layer messages   */

#define NRF_DLDATA        (3)             /* downlink data frames             */
#define NRF_DLMUTE1LL     (3)             /* 1st muting lower-layer message   */
#define NRF_DLMUTE1HL     (1)             /* 1st muting higher-layer message  */
#define NRF_DLCHUNK       (NRF_SYNC + NRF_DLMUTE1HL + 2*NRF_DLDATA)

#define DL_PCM_DATA_ROWS  (4)			/*num of rows of dlPcmData[][] Matrix*/

#if (ARQ_MAX%2 != 0)
#error Solution expects ARQ_MAX to be %2
#endif

#if IS_NOT_POWER_OF_TWO(ARQ_MAX)
#error Solution expects ARQ_MAX to be a power of 2.
#endif

#if IS_NOT_POWER_OF_TWO(DL_PCM_DATA_ROWS)
#error Solution expects DL_PCM_DATA_ROWS to be a power of 2.
#endif

/*============================================================================*/
/* IVS processing                                                        */
/*----------------------------------------------------------------------------*/

#define NRF_MEMCTRL       (7)
#define NRS_MEMSYNC       (508 + 38*NRS_CP)

#define IVS_NUMSEND       (5)             /* number of SEND messages          */
#define IVS_THRESHOLD     (40000)         /* threshold for control messages   */
#ifndef IVS_GOSTART
#define IVS_GOSTART       (6)             /* threshold for unreliable START   */
#endif
#ifndef IVS_TXFAST
#define IVS_TXFAST        (10)            /* fast modulator NACK condition    */
#endif
#define IVS_TXINC         (87)            /* sample increment at restart      */

#define FEC_VAR           (30206)         /* variance: 1/4550000 in Q37       */
#define FEC_MEAN          (0xB9999A)      /* mean: 5.8 in Q21                 */
#define FEC_ITERATIONS    (8)             /* number of decoder iterations     */
#define FEC_STATES        (8)             /* number of decoder states         */

#define IntLLR            INT16
#define LLR_MAX           ((INT32)(0x7fff-1))
#define LOGEXP_RES        (401)           /* resolution of logexp table       */
#define LOGEXP_DELTA      (-6)            /* internal Q-factor                */
#define LOGEXP_QIN        (8)             /* input Q-factor of LLR values     */

#endif /*ECALL_DEFINES_H_*/
#endif /*VOICE_ECALL_BUILD*/