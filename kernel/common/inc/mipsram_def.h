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

#if !defined(_MIPSRAM_DEF_H_)
#define _MIPSRAM_DEF_H_


#define MIPS_TEST_EVENT_LIST_BUFSIZE   (4*1024)

#if defined(MIPS_TEST)
#include "cpu_mips_test.h"

#if defined(MIPS_TEST_RAM)

//#if defined (_TAVOR_HARBELL_) || defined (_TAVOR_BOERNE_) || defined(SILICON_PV2)
#if defined (_TAVOR_BOERNE_) || defined(SILICON_PV2)
  //Use extended buffer for debugging
#define MIPS_RAM_BUFFER_SIZE (0x1000*4)
#else
#define MIPS_RAM_BUFFER_SIZE 0x1000
#endif

//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
  // Don't use the MIPS_RAM_BUFFER_INTERNAL define but use explicit SCATTER
  extern __align(32) UINT32 mipsRamBuffer[MIPS_RAM_BUFFER_SIZE/sizeof(UINT32)];
#else
  #if MIPS_RAM_BUFFER_SIZE<0x10000   /*For reasonable buffer sizes allow internal RAM use if desired*/
  #define MIPS_RAM_BUFFER_INTERNAL
  #endif
#endif//_TAVOR_HARBELL_

#endif//MIPS_TEST_RAM
#endif//MIPS_TEST

extern __align(32) UINT8  mipsEventList[MIPS_TEST_EVENT_LIST_BUFSIZE];
extern             UINT32 mipsEventListSize;

extern void mipsRamStop(void);
extern void mipsRamStart(void);
extern void mipsRamStart(void);
extern void mipsRamSaveToFile(void);
extern void mipsRamSaveEventToRam(void);

#endif//_MIPSRAM_DEF_H_
