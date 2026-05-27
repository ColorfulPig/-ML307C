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
* Filename: acm_nau8810.h
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
#ifndef ACM_NAU8810_H
#define ACM_NAU8810_H
/******************************************************************************
* Function     : codec_nau8810_set_speaker_gain
*******************************************************************************
* Description  :  set codec speaker gain
* Parameters   :  unsigned char index,    0 <= index <= 7
* Output Param   : None.
* Return value   :  
* Notes		  : if index > 7,  will reset index into 7
*******************************************************************************/
void codec_nau8810_set_speaker_gain(unsigned char index);

/******************************************************************************
* Function     : codec_nau8810_set_mic_gain
*******************************************************************************
* Description  :  set codec mic gain
* Parameters   :  unsigned char index,    0 <= index <= 7
* Output Param   : None.
* Return value   :  
* Notes		  : if index > 7,  will reset index into 7
*******************************************************************************/
void codec_nau8810_set_mic_gain(unsigned char index);

/******************************************************************************
* Function     : codec_nau8810_get_speaker_gain
*******************************************************************************
* Description  :  get codec speaker gain
* Parameters   :  void,    
* Output Param   : None.
* Return value   : unsigned char,  0 ~ 7
* Notes		  : 
*******************************************************************************/
unsigned char codec_nau8810_get_speaker_gain(void);

/******************************************************************************
* Function     : codec_nau8810_get_mic_gain
*******************************************************************************
* Description  :  get codec mic gain
* Parameters   :  void,    
* Output Param   : None.
* Return value   : unsigned char,  0 ~ 7
* Notes		  : 
*******************************************************************************/
unsigned char codec_nau8810_get_mic_gain(void);

/******************************************************************************
* Function     : codec_nau8810_is_connect
*******************************************************************************
* Description  :  nau8810 is connect via I2C
* Parameters   :  void,    
* Output Param   : None.
* Return value   : 0: disconnect,  1: connect
* Notes		  : 
*******************************************************************************/
int codec_nau8810_is_connect(void);


#endif 
