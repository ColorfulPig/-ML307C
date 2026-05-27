/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Audio Component Manager (ACM)
*
* Filename: acmTypes.h
*
* Authors: Isar Ariel
*
* Description: ACM typedefs
*
* Last Updated:
*
* Notes:
******************************************************************************/


#ifndef _ACM_TYPES_H_
    #define _ACM_TYPES_H_


#include "global_types.h"
#define ACM_ASSERT(cOND) ASSERT(cOND)

#define RSV_DUALMIC_PARAMETER               28
#define MAX_DESCRIPTION_LEN 			    28


#define NUM_OF_FILT_COEFFS 3   //Filter coefficients number

#define HLPF_TYPE_HPF   0   //HPF
#define HLPF_TYPE_LPF   1   //LPF




#ifndef ON
  	#define ON      1
	#define OFF     0
#endif

#define MSA_SIDETONE_16KHz_MASK         0xff000000
#define MSA_SIDETONE_8KHz_MASK          0x00ff0000
#define MSA_GAIN_16K_MASK               0x0000ff00
#define MSA_GAIN_8K_MASK                0x000000ff

#define MSA_SIDETONE_16KHz_SHIFT        24
#define MSA_SIDETONE_8KHz_SHIFT         16
#define MSA_GAIN_16K_SHIFT              8
#define MSA_GAIN_8K_SHIFT               0

#define MSA_PARAMETER_OFFSET_MASK       0x000000ff
#define MSA_PARAMETER_OFFSET_SHIFT      0

typedef unsigned int ACM_AudioConfirmID;   //Primitive ID send from AP, need to send back to AP.
typedef unsigned int ACM_MSAGain;  /* Byte0:8Khz digital gain; Byte 1: 16KHz digital gain ; Byte2:8Khz side-tone gain; Byte 3: 16KHz side-tone gain*/

/*
    For handset/Headset/handsfree mode, we allow set different MSA parameters for different volume

    Byte 0:
    AP volume(0~100), CP need to map to correct NVM offset

    Byte 1~3: Reserved
*/
typedef unsigned int ACM_AudioMISC;  /* Byte0:AP volume(0~100)*/

typedef signed char   ACM_DigitalGain;


//ICAT EXPORTED ENUM
typedef enum
{
    ACM_MUTE_OFF = 0,
    ACM_MUTE_ON = 1,

	ACM_AUDIO_MUTE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement
} ACM_AudioMute;




/*----------- Extern definition ----------------------------------------------*/


/*----------- Global variable declarations -----------------------------------*/


/*----------- Global function prototypes -------------------------------------*/

/*----------- Global macro definitions ---------------------------------------*/


#define INVALID_DIGITAL_GAIN	((ACM_DigitalGain)0x7F)

#endif  /* _ACM_TYPES_H_ */


