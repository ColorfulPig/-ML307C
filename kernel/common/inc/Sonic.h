/******************************************************************************
 *
 *  (C)Copyright 2015 - 2021 ASRMicro. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF ASRMicro.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of ASRMicro and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of ASRMicro.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for Sonic encode/decode in COMM subsystem
*
* Filename: Sonic.h
*
* Authors:
*
* Description: Header file for Sonic encode/decode.
*
* Last Updated:
*
* Notes:
******************************************************************************/

/*----------- Global macro definitions ---------------------------------------*/
#ifndef __SONIC_H__
#define __SONIC_H__

#ifdef __cplusplus
extern "C"{
#endif

/*----------- Global type definitions ----------------------------------------*/
//status 0:success, -1:error, 1:timeout
typedef void (*ACMAudioKeyCB)(int status, unsigned char *keybuff, unsigned int length, unsigned int sendcount);

/*----------- Local function definition -------------------------------------*/
int ACMAudioPlayKeyStart(unsigned char *keybuff, unsigned int length, unsigned int timeoutms, ACMAudioKeyCB cb);
void ACMAudioPlayKeyStop(void);
int ACMAudioRecordKeyStart(unsigned int timeoutms, ACMAudioKeyCB cb);
void ACMAudioRecordKeyStop(void);

#ifdef __cplusplus
}
#endif
#endif //__SONIC_H__
