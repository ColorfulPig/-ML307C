/******************************************************************************
 *
 *  (C)Copyright ASRMicro. All Rights Reserved.
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
* Title: codec nau8810 related
*
* Filename: acm_nau8810_private.h
*
* Target, platform: Common Platform, SW platform
*
* Authors: Chao Liu
*
* Description: Controls nau8810.
*
* Last Updated:
*
* Notes:
*******************************************************************************/
#ifndef ACM_NAU8810_PRIVATE_H
#define ACM_NAU8810_PRIVATE_H

int codec_nau8810_init(void);
int codec_nau8810_is_connect(void);
void codec_nau8810_enable_path(void);
void codec_nau8810_disable_path(void);


#endif 
