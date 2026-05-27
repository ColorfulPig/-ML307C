/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
*                                           *                                     *
*     File name:      typedef.h				*   DSPC Proprietary Information      *
*     Programmer:					        *   (C) COPYRIGHT 2000 DSPC, an Intel *
*                                           *   Company                           *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                                 *
*       Name:		  Common Firmware software                                    *
*                                                                                 *
*       Create Date:  October, 2000		                                          *
*                                                                                 *
*       Description:  Standard Type Definitions									  *
*                                                                                 *
*       Notes:                                                                    *
*                                                                                 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*
$Header:: /WhiteSail/FW/Inc/cfw_typedef.h 5     10/23/01 12:05p Shirid           $
*/

#ifndef _TYPEDEF_H_
#define _TYPEDEF_H_ 1
#if FEATURE_AUDIO_ECALL_IVS == 0
typedef signed char     INT8;
typedef unsigned char   UINT8;

typedef signed short    INT16;
typedef unsigned short  UINT16;
typedef signed short    fr1x16;

#ifndef _BASETSD_H_ // Microsoft definitions
typedef signed long     INT32;
typedef unsigned long   UINT32;
typedef signed long     fr1x32;
#endif


#ifdef __ADSPBLACKFIN__
#define SEGMENT(a) segment (a)
#else
#define SEGMENT(a)
#endif

#ifndef _BASETSD_H_ // Microsoft definitions
#if defined (__CCC__) || !defined (__ADSPBLACKFIN__) //ccc compiler
typedef UINT8 BOOL;
#endif //__CCC__
#define FALSE ((BOOL)0)
#define TRUE  ((BOOL)1)
#endif

/* Similar typedefs - to retain compatibility */

typedef UINT8           BYTE;
typedef UINT8           UBYTE;
typedef INT8            Word8;
typedef UINT8           UWord8;
typedef UINT16          UWORD;
typedef UINT16          WORD;
typedef INT16           SWORD;
typedef INT16           Word16;

#ifndef _BASETSD_H_ // Microsoft definitions
typedef UINT32          DWORD;
#endif

typedef INT32           Word32;
typedef int             Flag;

/** EDIT the following assignments from basic compiler types to compiler
 ** independent storage size type names according to the characteristics
 ** of the compiler */
typedef       char  SignedInt8;  /* on a PC a char is 8 bits ...         */
typedef       short SignedInt16;
typedef       int   SignedInt32;

typedef unsigned char     Int8; /* on PC unsigned char is 8 bits ...    */
typedef unsigned short    Int16;
typedef unsigned int      Int32;

#else
#include <stdbool.h>
#include "gbl_types.h"
typedef INT16           Word16;
typedef INT32           Word32;
typedef signed short    fr1x16;
typedef signed long     fr1x32;

#endif

#endif // _TYPEDEF_H_
