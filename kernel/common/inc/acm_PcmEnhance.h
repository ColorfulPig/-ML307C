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
#ifndef ACM_PCM_ENHANCE_H
#define ACM_PCM_ENHANCE_H

#include "osa.h"

#if defined (SAMSUNG_TOTORO_DIAMONDVOICE)
#include "DiamondVoiceTask.h"
#endif
#if defined (SAMSUNG_LVVE)
#include "LVVE_Para.h"
#endif

#if defined (SAMSUNG_TOTORO_DIAMONDVOICE) || defined (SAMSUNG_LVVE) 

#define PCM_WB_AUDIO_MODE	1
#define PCM_NB_AUDIO_MODE	0

#define PCM_EHANCEMENT_DATA_LEN_NB 160
#define PCM_EHANCEMENT_DATA_LEN_WB 320

#define PCM_ENHANCE_DATA_RECEIVED     0x1
#define WISE_VOICE_MSG_RECEIVED     0x2

#define PCM_ENHANCE_MASK		 (PCM_ENHANCE_DATA_RECEIVED|WISE_VOICE_MSG_RECEIVED)


extern  short	*PCMEnhance_InBuffer;
extern	short	*PCMEnhance_OutBuffer;
extern  unsigned short   PCM_Mode_NbWb;
extern OSAFlagRef audioPcmEnhanceRef;
extern UINT32 PCMSize_WiseVoice;
extern short	*WiseVoice_InBuffer;

#define RX_OUTPUT_TEMP_BUF (PCMEnhance_InBuffer + 3 * PCM_EHANCEMENT_DATA_LEN_WB)

void ACMPcmEnhancementInit(void);
void PCMEnhance_ReloadFDI(void);
void PCMEnhance_CreateDefaultTable(void);
void PCMEnhance_CreateDefaultTable_IfNoExist(void);

#endif
#endif 
