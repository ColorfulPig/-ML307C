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

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title: Watch Dog Timer Driver Header - Extention called by APPS for handling COMM
*
* Filename: watchdog_apps4com.h
*
*  The Watchdog Timer (WDT) is a programmable timer causing an INT or RESET.
*  The COMM-side WDT is !!configured!! to INT or RESET independently from APPS,
*             BUT
*  the real behaviour depends upon APPS configuration.
*  The default behavior is configured here.
*******************************************************************************/

/*=======================================================================*/
/*        NOTE: This file may be used (re-used) by OBM or WIN-CE         */         
/*=======================================================================*/

#ifndef _WATCHDOG_APPS4COMM_H_
#define _WATCHDOG_APPS4COMM_H_


typedef void (*WdtApps4comIntHandler)(void);  //Interrupt handler function prototype

//ICAT EXPORTED ENUM
typedef enum
{
    WDT_COM_RESET_DISABLE   = 0,
    WDT_COM_RESET_IMMEDIATE,
    WDT_COM_RESET_KEEP_PENDING
} WdtComResetCfgType;


#if !defined (_TAVOR_BOERNE_) || defined (_QT_) || defined (_VIRTIO_PLATFORM_)
  #define WATCHDOG_APPS_4_COMM()  /* nothing to do */
  #define wdtApps4comInit(void)
  #define wdtApps4comBindUserHander(void)
  #define wdtApps4comResetCfg(X)     WDT_COM_RESET_DISABLE
  #define wdtApps4comResetActive()      TRUE
  #define wdtApps4comResetInactive()    TRUE
#else
  #define BRN_SBCR_REG       *(volatile unsigned long*)0x42404008 /* CWSBR Sticky Bit Register*/
  // Prevent startup Comm WDT RESET. Keep Sticky Bit mechanism "notInUse"
  #define WATCHDOG_APPS_4_COMM()  { BRN_SBCR_REG |= (1L<<1); } 

  void wdtApps4comInit(void);
  void wdtApps4comIntReenableAfterD2(void);
  void wdtApps4comBindUserHander(WdtApps4comIntHandler handler);
WdtComResetCfgType  wdtApps4comResetCfg(WdtComResetCfgType newResetCfg); //returns old cfg
  BOOL wdtApps4comResetActive(void);
  BOOL wdtApps4comResetInactive(void);
#endif//_TAVOR_BOERNE_

#endif	/* _WATCHDOG_APPS4COMM_H_ */
