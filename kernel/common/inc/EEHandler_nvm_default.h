/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code (揗aterial? are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel抯 prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

#ifndef EEHANDLER_NVM_DEFAULT
#define EEHANDLER_NVM_DEFAULT
 #include "timer_config.h"
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
// This file defines the default configuration set by EEHandler when no NVM file is found
// In order to override the settings copy the file into target\inc and modify
//
// Do not include this file from any module other than EEHandler_config.h
//////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if defined(EEHANDLER_NVM_VERSION)

// File used by printRecentLogOnStartup
#if defined(EEH_ASSISTED_CLIENT)
#define EEHANDLER_DEFAULT_LOGFILE_SIZE      0 /* Don't save RecentLogOnStartup into file */
#else
#define EEHANDLER_DEFAULT_LOGFILE_SIZE     (10 * sizeof(EE_Entry_t))
#endif
//local default defines
#if defined TIMER_32K_MULTIPLIER
#define EE_DEFER_DELAY 500*TIMER_32K_MULTIPLIER /* for TTC QT 32K clock is 3.25M*/
#else
#define EE_DEFER_DELAY 500 /*0.5s*/
#endif
/* ASSERTS should not return */
const EE_Configuration_t eeDefaultConfiguration ={
	EE_ON,                                           //  EE_OnOff_t       AssertHandlerOn;
	EE_ON,                                           //  EE_OnOff_t       ExcepHandlerOn;
	EE_WARN_OFF,                                     //  EE_WarningOn_t   WarningHandlerOn;
	EE_OFF,                                          //  EE_OnOff_t       powerUpLogOn;
	EE_ON,                                           //  EE_OnOff_t       extHandlersOn;
	EE_ON,                                           //  EE_OnOff_t       printRecentLogOnStartup;
	//EE_STALL,                                      //  FinalAct_t       finalAction;
	EE_RESET,                                     //  FinalAct_t       finalAction; // by cmiot 设置为 EE_RESET 默认关闭DUMP
	EEHANDLER_DEFAULT_LOGFILE_SIZE,                  //  UINT32           EELogFileSize;
	0,                                               //  UINT16           delayOnStartup; // Delay
	EE_CT_StackDump,                                      //  EE_ContextType_t assertContextBufType;
	EE_CT_StackDump,                                      //  EE_ContextType_t exceptionContextBufType;
	EE_CT_StackDump,                                       //  EE_ContextType_t warningContextBufType;
#if defined(EE_DEFERRED_ACTIONS)
    { EE_DEFER_ALL, EE_ON, EE_DEFER_DELAY , 10, 0 },    //EE_DeferredCfg_t deferredCfg;
#else
    {0},                                              //UINT8            reserved1[8];
#endif

    EE_WDT_TIME_10SEC,                              //EE_WdtTimeCfg_t  wdtConfigTime;  //UINT16

	0,        /* ZERO is "no limits" */             //  UINT16           sysEeHandlerLimit; //relevant for EE_ASSISTING_MASTER only

#if defined(DATALIGHT) && defined(EEH_NVM_ENABLED_LOCAL)
//The correct #ifdef here should be about FlashFileSpace size
//But since we haven't, use quick&durty by DATALIGHT, which is dealing with "big" flash
// !!! The parameter is relevant for EE_ASSISTING_MASTER only; has no influence in other cases !!!
    1, // "1"=dump-All, or size in BYTES!  //  UINT32           dumpDdrSizeBytes;
#else
    0x0,        // "1"=dump-All, or size in BYTES!  //  UINT32           dumpDdrSizeBytes;
#endif


        {0},                                         //  UINT8            reserved[23];
        0						//dump reset flag
};

#else //EEHANDLER_NVM_VERSION
#error  EEHANDLER_NVM_VERSION   IS NOT SUPPORTED
#endif//EEHANDLER_NVM_VERSION

#endif//EEHANDLER_NVM_DEFAULT
