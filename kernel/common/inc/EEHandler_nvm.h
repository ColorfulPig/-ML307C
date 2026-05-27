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
* Title: Error Handler
*
* Filename: EEHandler_nvm.h
*
* Target, platform: Hermon
*
* Authors: Anton Eidelman
*
* Description: Implementation file: Error handler configuration and NVM definitions
*
*
*
* Notes:
******************************************************************************/


#ifndef _EEHANDLER_NVM_
#define _EEHANDLER_NVM_
#include "global_types.h"
#include "nvm_header.h"
#include "EEHandler.h"


//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
 #define NVM_ON_REMOTE  1
#else
 #define NVM_ON_REMOTE  0
#endif


//ICAT EXPORTED ENUM
typedef enum
{
	/* @ENUM_DESC@ Configures Error handling final action*/
    EE_RESET,               /* @ENUM_VAL_DESC@ final action RESET*/
    EE_STALL,               /* @ENUM_VAL_DESC@ final action STALL*/
	EE_CONTINUE,            /* @ENUM_VAL_DESC@ report but continue (like ignore or warning)*/
	EE_EXTERNAL,            /* @ENUM_VAL_DESC@ final action EXTERNAL*/
	EE_RESET_START_BASIC,   /* @ENUM_VAL_DESC@ final action RESET START BASIC*/
    EE_NON_WDT_SERIAL       /* @ENUM_VAL_DESC@ final action NON WDT SERIAL*/
}FinalAct_t;

//ICAT EXPORTED ENUM
typedef enum
{
	/* @ENUM_DESC@ Error Handler options ON/OFF enum*/
    EE_OFF,	/* @ENUM_VAL_DESC@ Option turned OFF*/
    EE_ON   /* @ENUM_VAL_DESC@ Option turned ON*/
}EE_OnOff_t;

//ICAT EXPORTED ENUM
typedef enum
{
	/* @ENUM_DESC@ Configure EE logs data in case of warning*/
    EE_WARN_OFF,            /* @ENUM_VAL_DESC@ EE warning log OFF*/
    EE_WARN_ASSERTCONTINUE, /* @ENUM_VAL_DESC@ EE logs warning assert and continue */
    EE_WARN_ASSERT          /* @ENUM_VAL_DESC@ EE logs warning assert */
}EE_WarningOn_t;

//ICAT EXPORTED ENUM
typedef enum
{
  EE_VER_3 = 3,
  EE_VER
}EE_Version_t;

//ICAT EXPORTED ENUM
typedef enum
{
  EE_HSL_OFF = 0,
  EE_HSL_1_8V= 1,
  EE_HSL_3V = 3
}EE_HSL_V_t;

//ICAT EXPORTED ENUM
typedef enum
{
	/* @ENUM_DESC@ WatchDog timer (WDT) configuration*/
  EE_WDT_OFF        =  0,       /* @ENUM_VAL_DESC@ Set WDT OFF*/
  EE_WDT_TIME_6SEC  =  6000,	/* @ENUM_VAL_DESC@ Set WDT to 6 sec*/     //in miliseconds; for max WCDMA/GSM DRX cycle
  EE_WDT_TIME_7SEC  =  7000,	/* @ENUM_VAL_DESC@ Set WDT to 7 sec*/     //in miliseconds
  EE_WDT_TIME_8SEC  =  8000,	/* @ENUM_VAL_DESC@ Set WDT to 8 sec*/     //in miliseconds
  EE_WDT_TIME_10SEC = 10000,	/* @ENUM_VAL_DESC@ Set WDT to 10 sec*/     //in miliseconds
  EE_WDT_TIME_20SEC = 20000,	/* @ENUM_VAL_DESC@ Set WDT to 20 sec*/	     //in miliseconds
  EE_WDT_TIME_30SEC = 30000,	/* @ENUM_VAL_DESC@ Set WDT to 30 sec*/     //in miliseconds
  EE_WDT_TIME_MAX   = 0xFFFF 	/* @ENUM_VAL_DESC@ Set WDT to MAX (65.535 sec)*/    //UINT16
}EE_WdtTimeCfg_t;

#if defined(EE_DEFERRED_ACTIONS)
//ICAT EXPORTED ENUM
typedef enum
{
	/* @ENUM_DESC@ Configure what deffered modes are Enabled*/
  EE_DEFER_NONE,        /* @ENUM_VAL_DESC@ Defer no actions*/
  EE_DEFER_ASSERTS,     /* @ENUM_VAL_DESC@ Defer asserts*/
  EE_DEFER_EXCEPTIONS,  /* @ENUM_VAL_DESC@ Defer exceptions*/
  EE_DEFER_ALL          /* @ENUM_VAL_DESC@ Defer all actions*/
}EE_DeferredMode_t;


//ICAT EXPORTED STRUCT
typedef struct
{
 	/* @STRUCT_DESC@ Deffered action configuration*/
  EE_DeferredMode_t  enable;                    /* @ITEM_DESC@ Configure what deffered modes are Enabled, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_OnOff_t         deferIntermediateActions;  /* @ITEM_DESC@ Set defer intermidate actions , @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  UINT16           limitMs;                     /* @ITEM_DESC@ deferred actions time limit , @ITEM_MODE@ Debug, @ITEM_UNIT@ 0-0xFFFF in milisec*/
  UINT16           limitHits;                   /* @ITEM_DESC@ deferred actions hits limit, @ITEM_MODE@ Debug, @ITEM_UNIT@ 0-0xFFFF*/
  UINT16           reserved2;                   /* @ITEM_DESC@ Reserved, @ITEM_MODE@ ReadOnly, @ITEM_UNIT@ Not relevant*/
}EE_DeferredCfg_t;
#endif

//ICAT EXPORTED STRUCT
typedef struct
{
	/* @STRUCT_DESC@ Error Handler Configuration*/
	/* @STRUCT_NVM_FILE_NAME@ EEHandlerConfig.nvm*/
  EE_OnOff_t       AssertHandlerOn;			/* @ITEM_DESC@ Configure EE logs data in case of ASSERT, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_OnOff_t       ExcepHandlerOn;          /* @ITEM_DESC@ Configure EE logs data in case of Exception handler, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_WarningOn_t   WarningHandlerOn;        /* @ITEM_DESC@ Configure EE logs data in case of warning, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_OnOff_t       powerUpLogOn;            /* @ITEM_DESC@ Configure EE logs data at power-on, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_OnOff_t       extHandlersOn;           /* @ITEM_DESC@ Configure EE logs data in case of extHandler, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_OnOff_t       printRecentLogOnStartup; /* @ITEM_DESC@ Indicates if to search for EE logs on NVM and notify regarding them on startup, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  FinalAct_t       finalAction;             /* @ITEM_DESC@ Configures Error handling final action, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  UINT32           EELogFileSize;           /* @ITEM_DESC@ Set Error Handler log file size, @ITEM_MODE@ Debug, @ITEM_UNIT@ 0-0xFFFFFFFF in bytes*/
  UINT16           delayOnStartup; 			/* @ITEM_DESC@ Set delay on startup before printing recent log, @ITEM_MODE@ Debug, @ITEM_UNIT@ 0-0xFFFF in 5 milisec ticks */	// Delay for ICAT log coverage in 5ms units
  EE_ContextType_t assertContextBufType;    /* @ITEM_DESC@ What context to save in case ASSERT happened, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_ContextType_t exceptionContextBufType; /* @ITEM_DESC@ What context to save in case Exception Handler happened, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
  EE_ContextType_t warningContextBufType;   /* @ITEM_DESC@ What context to save in case Warning happened, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/
#if defined(EE_DEFERRED_ACTIONS)
  //-------- version 1+D ----------
  EE_DeferredCfg_t deferredCfg;             /* @ITEM_DESC@ Deffered action configuration, @ITEM_MODE@ Debug, @ITEM_UNIT@ see structure*/
#else
  UINT8            reserved1[8];            /* @ITEM_DESC@ reserved place for when deferredCfg not used, @ITEM_MODE@ , @ITEM_UNIT@ Not relevant*/
#endif
  EE_WdtTimeCfg_t  wdtConfigTime;			/* @ITEM_DESC@ WatchDog timer configuration, @ITEM_MODE@ Debug, @ITEM_UNIT@ see enum*/       //UINT16
  UINT16           sysEeHandlerLimit;		/* @ITEM_DESC@ EE handler system limit (only EE_ASSISTING_MASTER), @ITEM_MODE@ Debug, @ITEM_UNIT@ 1-0xFFFF, 0-No limit*/   //relevant for EE_ASSISTING_MASTER only; ZERO is no limits
  UINT32           dumpDdrSizeBytes;		/* @ITEM_DESC@ Limit DDR size to dump (only EE_ASSISTING_MASTER), @ITEM_MODE@ Debug, @ITEM_UNIT@ 0-0xFFFFFFFF in bytes, 0x1-0x400 - No limit*/    //relevant for EE_ASSISTING_MASTER only
  UINT8            reserved[24];            /* @ITEM_DESC@ Reserved, @ITEM_MODE@ ReadOnly, @ITEM_UNIT@ Not relevant*/
  UINT32          dumpResetFlag;	/*default is 0. When finalAction is EE_RESET, 1:dump and then silent reset; 0:silent reset without dump*/
}EE_Configuration_t;


typedef enum
{
    EE_NVMCONFIG_UNCODITIONAL,
    EE_NVMCONFIG_EARLY,         /* used for Non NVM_ON_REMOTE */
    EE_NVMCONFIG_LATEST         /* used for NVM_ON_REMOTE     */
}EE_NvmConfigMode_t;


//
// EEHandler Internal services
//

void  eeSetConfiguration(const EE_Configuration_t* pConfig, int len);
void eePrintConfiguration( void );
void eeInitDfltConfiguration(void);
void SilentResetEnable(void);
void SilentResetDisable(void);

//
// EEHandler Internal data
//

extern EE_Configuration_t eeConfiguration;

#endif
