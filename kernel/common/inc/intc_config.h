/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code (“Material”) are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel’s prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: Interrupt Controller Configuration Header File                */
/*                                                                      */
/* Filename: intc_config.h                                              */
/*                                                                      */
/* Author: Yossi Gabay                                                  */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/* 																		*/
/*	Yaeli Karni	7 March 06,	Add Tavor_Boerne interrupts and changes		*/
/************************************************************************/
#ifndef _INTC_CONFIG_H_
#define _INTC_CONFIG_H_

/*
  INTC package provides a common API and framework for interrupt handing.
  INTC package supports Interrupt Controller modules (P-ICU, S-ICU, XIRQ/AIRQ etc, and GPIO interrupts) using the following HW modules:
  = P_ICU: 
  - Hermon or Tavor (boerne) or PXA27x; implemented in Intc_HT.c
  - TTC (Tavor-L); implemented in Intc_TTC.c
  = S-ICU (SINTC): 
  - Only available on Hermon; ; implemented in SIntc.c
  = GPIO (edge detection with interrupt generation):
  - Available on all silicon implementations except for Tavor-Harbell.; implemented in Intc.c (as part of common framework)
  = XIRQ: CURRENTLY NOT SUPPORTED; UNDER CONSTRUCTION.
  INTERFACE DEPENDANCES (see intc.h, intc_list.h): global_types.h, gpio.h

  IMPLEMENTATION DEPENDANCES:
  - gpio.h, gpio_hw.h, utils.h, pmu.h, power_manager.h, cpu_mips_test.h

  MEGA-SWITCHES:
  - THIS FILE: SILICON_TTC, _HERMON_B0_SILICON_, INTEL_2CHIP_PLAT_BVD, _TAVOR_BOERNE_
               SILICON_TTC_CORE_MOHAWK, SILICON_TTC_CORE_SEAGULL (note: on TTC P-ICU is dual-core aware).
  - MAKE:      none
*/

#include "syscfg.h"
#include "gpio_config.h"

#if defined (INTEL_2CHIP_PLAT_BVD)
/* INTC Usage options for INTC_P_ICU_HERMON_TAVOR_TYPE
 *      1) Regular APB access (using C code only)
 *      2) Access by CoProcessor registers (still withing the C code)
 * CoProcessor access is:
 * - available on Hermon and Boerne; 
 * - not available on Bulverde;
 * - not relevant on TTC
 */ 
 
  #if !defined (_TAVOR_BOERNE_)
    #define INTC_USE_APB_ACCESS_ONLY
  #endif
#endif //(INTEL_2CHIP_PLAT_BVD)

#if defined (INTEL_2CHIP_PLAT_BVD)
 // APP processors have GPIO#0 and #1 edge interrupts directly wired to primary interrupt controller
#define INTC_GPIO_01_WIRED_TO_ICU
 // APP processors have ICHP register order swapped: upper=FIQ, lower=IRQ
#define ICU_ICHP_IRQ_UPPER_BITS
#endif



#if defined(INTEL_2CHIP_PLAT_BVD) || defined(_TAVOR_BOERNE_) || defined(_HERMON_B0_SILICON_)
#define INTC_P_ICU_HERMON_TAVOR_TYPE
#endif

#if defined(_HERMON_B0_SILICON_) && !defined (INTEL_2CHIP_PLAT_BVD)
// S-ICU is Hermon-specific
#if !defined(_S_INTC_)
#define _S_INTC_
#endif
#define SINTC_HERMON
#endif

#define ICU_32_ENTRIES      32
#define ICU_64_ENTRIES      64
#define ICU_96_ENTRIES      96


// SINTC functions that present interface to INTC (sintc.h) can accept either
// 1. INTC_InterruptSources (and convert into SINTC source number.
// or
// 2. Relative source number inside the SINTC range.
// Option 1 (Hermon legacy) requires extra conversion when SINTC functions call one another
// Option 2 is also suitable for Hermon and also matches the XIRQ model as well (XIRQ driver may serve as either INTC or SINTC).
// This is controlled by the following macro:
//#define SINTC_DOES_SRC_CONVERSION

#if defined(SILICON_TTC)
#define INTC_P_ICU_TTC_TYPE
#if defined(SILICON_TTC_CORE_MOHAWK)
#define INTC_CORE_AP
#elif defined(SILICON_TTC_CORE_SEAGULL)
#define INTC_CORE_CP
#else
#error INTC requires TTC core type specification
#endif

//#if defined(FLAVOR_COM) || !defined(FLAVOR_DUALCORE)
#if 1
// Only on one core (COM) XIRQ can be used as secondary interrupt controller
#define SINTC_XIRQ /* XIRQ is connected to P-ICU as secondary interrupt controller */
#define _S_INTC_      /* INTC enables SINTC connection */
#endif

#endif


#endif  /* _INTC_CONFIG_H_ */
