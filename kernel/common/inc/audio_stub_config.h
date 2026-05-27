/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
**
**  COPYRIGHT (C) 2001, 2002 Intel Corporation.
**
**  This software as well as the software described in it is furnished under 
**  license and may only be used or copied in accordance with the terms of the 
**  license. The information in this file is furnished for informational use 
**  only, is subject to change without notice, and should not be construed as 
**  a commitment by Intel Corporation. Intel Corporation assumes no 
**  responsibility or liability for any errors or inaccuracies that may appear 
**  in this document or any software that may be provided in association with 
**  this document. 
**  Except as permitted by such license, no part of this document may be 
**  reproduced, stored in a retrieval system, or transmitted in any form or by 
**  any means without the express written consent of Intel Corporation. 
**
**  FILENAME:       audio_stub_conf.h
**
**  PURPOSE:       contains functions for the audio stub functions
**					
**  NOTES:			The following funcs are implemented in this file                
**                  
**
******************************************************************************/
#ifndef _AUDIO_STUB_CONFIG_H
#define _AUDIO_STUB_CONFIG_H

#include "gbl_types.h"


// parameters used for audio stub stask init
#ifdef OSA_NO_PRIORITY_CONVERSION
#define MSL_AUDIO_TASK_PRIORITY		71
#else
#define MSL_AUDIO_TASK_PRIORITY		98
#endif
#define MSL_AUDIO_STACK_SIZE        1024*4  //1024*2 HEZI - Increase stack size


#define MSL_AUDIO_MESSAGE_Q_SIZE	sizeof(MslMessage) //128 why???
#define MSL_AUDIO_MESSAGE_Q_MAX		100

#endif  //_AUDIO_STUB_CONFIG_H

