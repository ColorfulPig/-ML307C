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

/************************************************************************/
/*                                                                      */
/* Title: UART config header file                                       */
/*                                                                      */
/* Filename: UART_config.h                                              */
/*                                                                      */
/* Author: Shirley Elbling                                              */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/* 																		*/
/************************************************************************/
/* name of macro include an extra underscore since Hermon SW already include the same macro name */
#ifndef _UART_CONFIG_H__
#define _UART_CONFIG_H__

/*  MEGA-SWITCHES:
  - THIS FILE: SILICON_TTC, _HERMON_B0_SILICON_, INTEL_2CHIP_PLAT_BVD, _TAVOR_BOERNE_, _TAVOR_HARBELL_
  - MAKE:      none
*/

#include "syscfg.h"

#if defined(_HERMON_B0_SILICON_) || defined(_TAVOR_BOERNE_) || defined(INTEL_2CHIP_PLAT_BVD)
// GPIO wakeup feature allows to shutdown the UART clocks and use the GPIO edge detectors to
// wake the driver up when activity is detected on one of the input lines (RX,RTS,DCD...)
// This requires access to GPIO module and pin-mux.
#define UART_GPIO_WAKEUP_FEATURE
#endif

//#if defined(_TAVOR_HARBELL_) || defined(SILICON_TTC) || defined(SILICON_PV2)
#if defined(SILICON_TTC) || defined(SILICON_PV2)
// The driver supports one port only (otherwise 3 ports known as FF,BT,ST uarts - legacy names).
// On Tavor COM-side only one UART is available
// On TTC one compatible UART is available (intended for COM use). The two other ports are new and are not yet supported.
#define UART_SINGLE_PORT
#endif


//#if defined(_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
// UART registers do not retain state in sleep (D2): special care is required to backup and restore the values.
#define UART_D2_SPECIFIC
#define INTC_CONFIGURE_WITH_3_ARGUMENTS
#endif

#if defined(_HERMON_B0_SILICON_) && !defined(_TAVOR_BOERNE_)
// Modem signals control (DCD, RI) is supported in UART API but is implemented on Hermon only.
#define UART_MODEM_SIGNAL_CONTROL
#endif

//#if defined(_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
// A dedicated interrupt is generated to signal UART wakeup: supported only on Tavor - Harbell UART.
#define UART_WAKEUP_INT INTC_CRXD_32_28_EVENT
#endif

//#if !defined(_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if 1
// UART module output to the interrupt controller is level: clear by either IIR read or RHR/MCR access depending on interrupt type
// Harbell case: it is clear the interrupt is level; however, leave the "legacy" BUG until confirmed
#define UART_INTERRUPT_IS_LEVEL
#endif

#if defined(_HERMON_B0_SILICON_) || defined(_TAVOR_BOERNE_)
// UART may issue spurious interrupts (interrupt pending while IIR indicates no interrupt).
// In this case:
// 1) If LSR indicates there's data in RX FIFO: read as normal RX data
// 2) Otherwise, dummy-read RHR to clear
#define UART_SPURIOUS_INT_RHR_READ_WA
// UART Si bug w/a: it is prohibited to read IIR UNLESS a UART interrupt is really pending: 1188494
#define UART_IIR_READ_ONLY_WHEN_PENDING
#endif

#if !defined (_QT_)
// Normal UART clock rate: this clock setting is out of scope of UART driver.
#define UART_CLOCK                  14745600L  /* the UART clock Hz input clock */
#else
#if defined(_HERMON_B0_SILICON_) && !defined(_TAVOR_BOERNE_)
// SLE (QT), Hermon: SLE clock corresponds to 312MHz (PLL highest rate)
#define UART_CLOCK                  12320L /* the UART clock Hz input clock for Hermon */
#else
// SLE (QT): SLE clock corresponds to 624MHz (PLL highest rate)
// PLEASE, ADD MORE DETAILS ABOUT THE CALCULATION
#define UART_CLOCK                  7040L  /* the UART clock Hz input clock for Tavor*/
#endif
#endif //QT

// UART peripheral base addresses
    #define UART_BASE_ADDRESS         0xD4010000 /* the base addres of harbel uart */
	   
	#define STUART_OFFSET             0x8000   /* the offset of Slow UART  */
	#define FFUART_OFFSET             0x7000
	#define BTUART_OFFSET             0xF000


#endif  /* _INTC_CONFIG_H_ */
