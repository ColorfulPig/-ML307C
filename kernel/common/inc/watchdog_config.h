/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title: Watch Dog Timer Driver CONFIGURATION
*
* Filename: watchdog_config.h
*
* NOTE: this file configures
*  - APPS WDT
*  - COMM WDT
*  - Handling the COMM's WDT on APPS side (APPS4COM)
*  Refer also WATCHDOG_ENABLED and CROSS_PLATFORM_INCLUDE
*  obtained from "above layers" but used "below" in the module
* 
*******************************************************************************/

#ifndef _WATCHDOG_CONFIG_H_
#define _WATCHDOG_CONFIG_H_

//==========================================================
// The registers, bits, interrupt-no, behavior depending upon HW-RTL
// Right now there are 3 kind of HW-RTL: HERMON, BOERNE, HARBELL
// In a couple of words
//  HERMON  is WDT-dedicated and counts forward till MATCH
//  HARBELL is WDT-dedicated and counts down till ZERO
//  BOERNE is not exactly the WDT, but regular timer and should read current value and make value+MATCH
//      !!! Since BOERNE is regular timer it could in use by othres. Check the potential collision. 
#if   defined (_TAVOR_BOERNE_)
                            #define WDT_HW_RTL_BEHAVIOR_BOERNE
#elif defined (SILICON_HARBELL)
                            #define WDT_HW_RTL_BEHAVIOR_HARBELL
#elif defined (SILICON_TTC)
                            #error TTC currently not supported
#else
                            #define WDT_HW_RTL_BEHAVIOR_HERMON
#endif//behavior

//==========================================================
//      WATCHDOG_HW_BASE_ADDRESS
#if defined (SILICON_HARBELL)
#define WATCHDOG_HW_BASE_ADDRESS     0xF00C0030L     // GB Timers base address
#elif defined (SILICON_PV2)
#define WATCHDOG_HW_BASE_ADDRESS     0xD4042000UL    // GB Timers base address
#elif defined (SILICON_TTC)
#define WATCHDOG_HW_BASE_ADDRESS     0xD4080000UL    //0xD403A000UL
#else
#define WATCHDOG_HW_BASE_ADDRESS     0x40A00000L
#endif//WATCHDOG_HW_BASE_ADDRESS

//Temporary workaround for PM APIs
#if !defined(SILICON_PV2)
#define PRMReleaseWdgFromReset()
#endif
//#if defined (WDT_HW_RTL_BEHAVIOR_HARBELL)    __defined_in_.C_file__
//#define WATCHDOG_RESET_INDICATION_CLEAR   CPMUClearLastResetReason()
//#endif

//== Refer also in the .C next MACROs:  ==================
// INTC_SRC_WDT
// _WATCHDOG_CHECK_PARAMETERS_FLAG_
// CSSR_WDT_KICK_** for HARBELL-behavior
//========================================================


//============================= APPS for COMM =============================
#if defined (_TAVOR_BOERNE_)
// See full bit description in the file watchdog_apps4com.c where it is used
#define BRN_SBCR_REG       *(volatile unsigned long*)0x42404008 /* CWSBR Sticky Bit Register*/

// #define COMM_WDT_AUTORESET_ENABLED   /*immidiate COMM-reset without any APPS handling*/

#define SBR_BIT_SET(BITno)  { BRN_SBCR_REG |=  (1L<<BITno); }
#define SBR_BIT_CLR(BITno)  { BRN_SBCR_REG &= ~(1L<<BITno); }

#define SBR_BIT0_SET         SBR_BIT_SET(0)
#define SBR_BIT0_CLR         SBR_BIT_CLR(0)

#define SBR_BIT1_SET         SBR_BIT_SET(1)
#define SBR_BIT1_CLR         SBR_BIT_CLR(1)

#define SBR_BIT3_SET         SBR_BIT_SET(3)
#define SBR_BIT3_CLR         SBR_BIT_CLR(3)

#if defined (COMM_WDT_AUTORESET_ENABLED)
#define SBR_BIT2_SET         SBR_BIT_SET(2)
#define SBR_BIT2_CLR         SBR_BIT_CLR(2)
#else
#define SBR_BIT2_SET
#define SBR_BIT2_CLR
#endif

#endif//_TAVOR_BOERNE_
//============================= APPS for COMM =============================


#if defined(SILICON_SEAGULL)
#define WDT_FIQ_USED
#endif

#endif	/* _WATCHDOG_CONFIG_H_ */
