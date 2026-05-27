/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/****************************************************************************
 *
 * Name:          global_types.h
 *
 * Description:   Standard type definitions
 *
 ****************************************************************************
 *
 *
 *
 *
 ****************************************************************************
 *                  Copyright (c) Intel 2000
 ***************************************************************************/

#ifndef _GLOBAL_TYPES_H_
#define _GLOBAL_TYPES_H_

#include "gbl_types.h"
#include "utils.h"  /* Required for ASSERT macro definition */

  /* Standard typedefs */
  typedef volatile UINT8  *V_UINT8_PTR;  /* Ptr to volatile unsigned 8-bit quantity       */
  typedef volatile UINT16 *V_UINT16_PTR; /* Ptr to volatile unsigned 16-bit quantity       */
  typedef volatile UINT32 *V_UINT32_PTR; /* Ptr to volatile unsigned 32-bit quantity       */

  typedef unsigned int    U32Bits;
  typedef BOOL BOOLEAN;
  #define INLINE __inline

  typedef const char *    SwVersion;

  #define DO_FOREVER  while (1)

  /* Handy macros */
  #define MAX(x,y)  (( (x) > (y) ) ? (x) : (y))
  #undef MIN
  #define MIN(x,y)  (( (x) < (y) ) ? (x) : (y))

  #define ABS(x)    (( (x) <  0  ) ? (-(x)) : (x))
  /* Bit fields macros */
  // Yaeli Karni - need to work also when number GT 32 ! (march 06)
  #define CONVERT_NUMBER_TO_32BIT_MASK(n)   ((0x00000001L) << ((UINT8)((n)%32)))

//strncat by shashal 
#define STRNCAT_CHAR_BYTES(arr1, arr2, len)    {   (len) = sizeof(arr1)-strlen(arr1);  \
													 if((len) > 1)		 strncat(arr1, arr2, (len) -1);  \
												 }
 

  #define TURN_BIT_ON(r, b)					((r) |= (b))
  #define TURN_BIT_OFF(r, b)        ((r) &= ~(b))
  #define IS_BIT_SET(r, b)					((r) & (b))

#endif  /* _GLOBAL_TYPES_H_ */

