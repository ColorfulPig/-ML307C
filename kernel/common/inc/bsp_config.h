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

#ifndef _BSP_CONFIG_H_
#define _BSP_CONFIG_H_
/*
 * NOTE:
 *
 * Beware to chose right stack sizes.
 * Improper stack sizes may cause severe SW failures !!!
 */
#define HISR_STACK_SIZE         (2*1024)
#define HISR_PRIORITY_LEVELS    3

#if !defined (BSP_MATHISV1) || defined (INTEL_2CHIP_PLAT_BVD) || defined (FLAVOR_MINIPLAT)
#else /* if !defined (BSP_MATHISV1) || defined (INTEL_2CHIP_PLAT_BVD) */
#define ENABLE_LEDS
#endif /* !defined (BSP_MATHISV1) || defined (INTEL_2CHIP_PLAT_BVD) */

//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#ifndef PHS_SW_DEMO_TTC
#define RF_CONT_MUX 0xD4000D20  /*TCU control over RF-GPIO's for DRAT*/
#else
//#if defined (EDEN_1928) || defined (NEZHA3_1826)
#if 0
#define RF_CONT_MUX 0xD0270008, /*TCU control over RF-GPIO's for DRAT*/
#else
#define RF_CONT_MUX 0xD4070008  /*TCU control over RF-GPIO's for DRAT*/
#endif
#endif

//------------------------------------------------------
//shachar - test platform for audio codec - CCR or Micco
//------------------------------------------------------
typedef enum{
	MICCO_CONNECTED,
	CCR_CONNECTED
}MICCO_CCR_ON_BOARD;
#endif//_TAVOR_HARBELL_

#if defined (SILICON_TTC)
#define RF_CONT_MUX 0xD4070008  /*TCU control over RF-GPIO's for DRAT*/

#endif

/* UART */
//#if defined (_TAVOR_HARBELL_) || defined (_TAVOR_BOERNE_) || defined(SILICON_TTC) || defined(SILICON_PV2)
#if defined (_TAVOR_BOERNE_) || defined(SILICON_TTC) || defined(SILICON_PV2)
#define ACTIVE_UARTS 1
#else
#define ACTIVE_UARTS 3
#endif

//#if defined (_TAVOR_HARBELL_)|| defined(SILICON_TTC) || defined(SILICON_PV2)
#if defined(SILICON_TTC) || defined(SILICON_PV2)
#define ACTIVE_UART_PORT_NUMBER UART_PORT_STUART
#elif defined (_TAVOR_BOERNE_)
#define ACTIVE_UART_PORT_NUMBER UART_PORT_FFUART
#else
#define ACTIVE_UART_PORT_NUMBER UART_PORT_BTUART
#endif

/* DSP BOOT */

#undef MSA_INIT
#if !defined (FLAVOR_MINIPLAT)
#if (defined (_HERMON_B0_SILICON_) && !defined(_TAVOR_BOERNE_)) || defined(SILICON_TTC_CORE_SEAGULL)
// silicon support MSA control (reset and boot assistance): either Hermon or TTC CP core
//#define MSA_INIT
#endif
#endif

// TTC specific
#if defined (SILICON_TTC) && defined(MSA_INIT)
/* definition for TTC specific DSP init code */
#define TTC_DSP_INIT
#endif

#if defined(_HERMON_B0_SILICON_) && !defined(INTEL_2CHIP_PLAT_BVD) || defined(SILICON_TTC)
#define BSP_SWITCH_TO_32KHZ_CLOCK
#endif

#endif//_BSP_CONFIG_H_
