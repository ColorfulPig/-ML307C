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

/**********************************************************************
*
* Filename: wifi_spy.h
*
* Programmers:
*
* Functions:
*
* Description:
*
**********************************************************************/
#ifndef WIFI_SPY_H
#define WIFI_SPY_H

#ifdef L1_WIFI_LOCATION
#include "wificontroll.h"

#define DEBUG_BUFFER_SIZE (100+1)
#define SPECIAL_MAX_EVENTS (3)
#define SPECIAL_DEBUG_BUFFER_SIZE (SPECIAL_MAX_EVENTS*6 + 1)
#define ALL_DEBUG_BUFFER_SIZE (DEBUG_BUFFER_SIZE + SPECIAL_DEBUG_BUFFER_SIZE)

extern void wifiSpyInit(void);
extern UINT32 wifiSpy(wifiMsgHeader *wifiMsg,UINT32 *SpecialIndex);
extern void wifiSpyStampEnd(UINT32 idx, UINT32 SpecialIndex);
extern void wifiWriteSpecialEvent (UINT32 SpecialIndex,UINT32 AplpIndex);
extern UINT32 wifiDebugMode;

enum{
    START_SPY_RECORD,
    SPY_IDLE
};

#endif
#endif

