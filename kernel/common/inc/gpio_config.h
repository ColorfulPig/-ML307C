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
/* Title: GPIO (General Purpose Input Output) Configuration             */
/*        Header File                                                   */
/*                                                                      */
/* Filename: Gpio_Config.h                                              */
/*                                                                      */
/* Author: Yossi Gabay                                                  */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/*                                                                      */
/* Yaeli Karni		Mar 2006, Enhance driver to support Boerne          */
/*                                                                      */
/************************************************************************/
#ifndef _GPIO_CONFIG_H_
#define _GPIO_CONFIG_H_

#include "syscfg.h"
#include "intc_config.h"  // some configurations are shared as INTC and GPIO are coupled

// Assumption is that our of hte 64 (hermon)/ 128 (Bulverde/Boerne) GPIOs the user will not
// define more then 32 groups of GPIOs. If need arise, the number should be increased.
// (user interface to GPIO is via the notion of PORT!)
#define GPIO_MAX_PORT_HANDLES               32

#define GPIO_DEFAULT_DEBOUNCE_CLOCK         1

#if defined(_HERMON_B0_SILICON_) && !defined (INTEL_2CHIP_PLAT_BVD) && !defined(_TAVOR_BOERNE_)
#if !defined(_EXTENDED_GPIO_)
// EGPIO: Hermon specific feature
#define _EXTENDED_GPIO_
#endif
#endif

#if defined (_TAVOR_BOERNE_) || defined(SILICON_TTC)
#define GPIO_128_ENTRIES
#else
#define GPIO_64_ENTRIES
#endif

// GPIO[127:64]: APP CPU's only
// Defined in intc_config.h: see GPIO_128_ENTRIES

#if defined(_HERMON_B0_SILICON_) && !defined(INTEL_2CHIP_PLAT_BVD)
// Hermon has a special GPIO register layout for edge detectio, interrupt, and pulling control registers
#else
#define GPIO_APP_LAYOUT
#endif

#if !defined(GPIO_APP_LAYOUT)
#define GPIO_DEBOUNCE_CONTROL     /* Support debouncer control per GPIO pin */
#define GPIO_PUPD_CONTROL         /* Support pull-up/pull-down configuration from GPIO module */
#define GPIO_LEVEL_DET_SUPPORTED  /* Support level detection */
#define GPIO_INT_MASK_REQ_REGS    /* GIMR and GIRR allow masking of *enabled* edge detectors and reading interrupt status */
#endif

#if defined(_TAVOR_BOERNE_) || defined(SILICON_TTC)
// Bit-wise set/clear type registers for direction and edge detection configuration
#define GPIO_SETCLEAR_ACCESS
#endif

// The legacy register map has 3 (GPIO_LEGACY_REG_AMOUNT) registers of each type (sequential), which allows for up to 96 GPIOs; 
// For 128 GPIO's option the 4th is in offset 0x100 bytes from the first one (hence, 0x40 words from the first register).
// This is applicable for registers {GPLRx,GPDRx,GPSRx,GPCRx,GRERx,GFERx,GEDRx}
// For new Set/Clear registers {GSDR,GCDR,GSFER,GCFER,GSRER,GCRER} two options exist: 
// - GPIO_SC_REGS_EMBEDDED: the same approach, while these registers follow previous ones in the register map (TTC layout)
// - otherwise: 4 registers sequentially located in a separate space after all legacy registers             (Tavor layout)
#if defined (SILICON_TTC)
#define GPIO_SC_REGS_EMBEDDED
#endif

#if defined(SILICON_TTC)
// GPIO edge detection interrupts only the core that unmasks it (per GPIO pin).
// This feature allows controlling interrupt generation (from INTC package) without affecting the edge detector settings
#define GPIO_CORE_INT_MASK
#if defined(SILICON_TTC_CORE_MOHAWK)
#define GPIO_CORE_INT_MASK_AP
#else
#define GPIO_CORE_INT_MASK_CP
#endif
#endif


#if !defined (INTEL_2CHIP_PLAT_BVD)
#define GPIO_SETUP_HW_ON_INIT
#endif

#if defined(SILICON_TTC)
#define GPIO_HW_ADDRESS             0xD4019000L
#else
#define GPIO_HW_ADDRESS             0x40E00000L
#endif

#endif /* _GPIO_CONFIG_H_ */


