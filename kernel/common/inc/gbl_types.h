/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ============================================================================
File        : gbl_types.h
Description : Global types file for testing the
              os/kal package.

Notes       : This file is only used to test the compilation and
              archiving for the os/kal package.

Copyright 2001, Intel Corporation, All rights reserved.
============================================================================ */

#if !defined(_GBL_TYPES_H_)
#define _GBL_TYPES_H_

/* Use the Xscale environment types */
#if defined(ENV_XSCALE)
#include "xscale_types.h"
#endif

/* Use the NordHeim environment types */
#if defined(ENV_ICT)
#include "ict_types.h"
#endif

/* Use the Arm environment types */
#if defined(ENV_ARM)
#include "arm_types.h"
#endif

/* Use the Gnu environment types */
#if defined(ENV_GNU)
#include "gnu_types.h"
#endif

/* Use the Microsoft Visual C environment types */
#if defined(ENV_MSVC)
#include "msvc_types.h"
#endif

  /* Standard typedefs */
  typedef unsigned char   Bool;         /* Boolean                        */

  /* Standard typedefs - to retain compatibility with TDMA */
  typedef UINT8           		 BYTE;         			/* Unsigned 8-bit quantity        */
  typedef UINT8            		 UBYTE;        			/* Unsigned 8-bit quantity        */
  typedef UINT16          		 UWORD;        			/* Unsigned 16-bit quantity       */
  typedef UINT16          		 WORD;         			/* Unsigned 16-bit quantity       */
  typedef INT16           		 SWORD;        			/* Signed 16-bit quantity         */
  typedef UINT32                 DWORD;        			/* Unsigned 32-bit quantity       */
  typedef unsigned long long     UINT64;                /* Unsigned 64-bit quantity       */
  typedef void* 		         VOID_PTR;

  #define CONST const

  #ifndef TRUE
    #define FALSE   0
    #define TRUE    1
  #endif  /* TRUE */

  #ifndef YES
    #define YES     TRUE
    #define NO      FALSE
  #endif  /* YES */

  #ifndef ON
    #define ON      1
    #define OFF     0
  #endif

  #ifndef PASS
    #define PASS      1
    #define FAIL     0
  #endif

  /* A NULL value is required such that it is not mistaken for a valid */
  /* value which includes values in the range of modulo 64. */
  #define NULL_VAL  0xFF

  /* Definition of NULL is required */
  #ifndef NULL
    #define NULL    0L
  #endif  /* NULL */

  #ifndef VOID
    #define VOID	void
  #endif


  #define ENABLED     1
  #define DISABLED    0


#endif /* _GBL_TYPES_H_ */

/*                      end of gbl_types.h
--------------------------------------------------------------------------- */



