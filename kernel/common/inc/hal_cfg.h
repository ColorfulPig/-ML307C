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




#ifndef _HAL_CFG_
#define _HAL_CFG_

#include "syscfg.h"

/*******************************************************************************
 *                      E r r o r s   V a l i d a t i o n
 ******************************************************************************/

// #if defined(SILICON_TTC) && (defined(_HERMON_B0_SILICON_) || defined(TAVOR) || defined(_TAVOR_HARBELL_) || defined(_TAVOR_BOERNE_)|| defined(SILICON_PV2)) 
//#if defined(SILICON_TTC) && (defined(_HERMON_B0_SILICON_) || defined(_TAVOR_HARBELL_) || defined(_TAVOR_BOERNE_)|| defined(SILICON_PV2)) 
#if defined(SILICON_TTC) && (defined(_HERMON_B0_SILICON_) || defined(_TAVOR_BOERNE_)|| defined(SILICON_PV2)) 
#error Cannot mix SILICON_TTC with other silicon types
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////
//                Basic configurations!	  - Miniplat Uses this basic configuration
////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _OSA_ENABLED_
#define _OSA_ENABLED_
#endif

/* Use this line to DISABLE the use of the WATCHDOG kick*/
#define WATCHDOG_DISABLE


#if defined(NODIAG)
	#if defined(_DIAG_ENABLED_)
		#undef _DIAG_ENABLED_
	#endif
#else
	#define _DIAG_ENABLED_
#endif

//Assuming DIAGUART as the default choice for MINIPLAT
#if defined(FLAVOR_MINIPLAT) && defined(_DIAG_ENABLED_)
#define _UART_ENABLED_
#endif

#if defined(SILICON_TTC)
// Hermon os_tick module is used instead of TickManager until the later is ported to TTC.
	#define HERMON_POWER_MANAGEMENT
#if !defined (FLAVOR_MINIPLAT)
	#define EE_HANDLER_ENABLE
#endif

#endif




//////////////////////////////////////////////////////////////////////////////////////////////
//From this point all thge defines are beyond Miniplat
///////////////////////////////////////////////////////////////////////////////////////////////////

//MBMINIPLAT ALL THE DEFINES BELOW DEFINED BEYOND MINIPLAT
#if !defined (FLAVOR_MINIPLAT)

#define WATCHDOG_ALLOWED
#define _UART_ENABLED_
//#if defined(FLAVOR_APP) || !defined(FLAVOR_DUALCORE)
#if 1
#define _RTC_ENABLED_
#endif

//#if defined(FLAVOR_COM) || !defined(FLAVOR_DUALCORE)
#if 1
#define IPC_ENABLED
#endif



/*******************************************************************************
 *                      H E R M O N   B 0
 ******************************************************************************/
#if defined(_HERMON_B0_SILICON_)

/////////////////////////////////////////////////////////
// Silicon-specific configuration. Modify with caution!
/////////////////////////////////////////////////////////
// _S_INTC_ - see overrides below
//#define _TCU_ON_PRIMARY_

#if !defined (INTEL_2CHIP_PLAT_BVD) && !defined(_TAVOR_BOERNE_)
#define _EXTENDED_GPIO_
#endif

#if !defined (INTEL_2CHIP_PLAT)
#define _LCDIF_
#define _CAMIF_
#endif
/////////////////////////////////////////////////////////

#if defined(_QT_)
/*----------------------------------------------
 * Hermon B0 on QuickTurn
 *----------------------------------------------*/
	#if defined(_TAVOR_BOERNE_)
	// no need LCD , no IPC APPS-COMM yet , //YKYK fix this !!
	#else
		#define _LCD_ENABLED_

	#endif //_TAVOR_BOERNE_

//#define _CAMIF_ENABLED_
//#define _KEYPAD_ENABLED_
//#define _ONEWIRE_ENABLED_
//#define _MMCSD_ENABLED_
//#define _MSHC_ENABLED_
//#define _I2S_ENABLED_
//#define _USB_ENABLED_
//#define _USIM_ENABLED_
//#define _RTC_ENABLED_
//#define _I2C_ENABLED_
//#define _PMC_ENABLED_
//#define INTEL_FDI


#else //NOT QT IN Hermon B0
/*----------------------------------------------
 * Hermon B0
 *----------------------------------------------*/

#if !defined (INTEL_2CHIP_PLAT)
#define _LCD_ENABLED_
#define _CAMIF_ENABLED_
#endif

#if !defined (INTEL_2CHIP_PLAT)
#define _SSP_ENABLED_
#define _KEYPAD_ENABLED_
#endif
//#define _ONEWIRE_ENABLED_
//#define _MMCSD_ENABLED_
//#define _MSHC_ENABLED_
//#define _I2S_ENABLED_
#if !defined (INTEL_2CHIP_PLAT_BVD) && !defined(_TAVOR_BOERNE_)
#define _USIM_ENABLED_
#endif
#define _USB_ENABLED_
#define _RTC_ENABLED_
#define _I2C_ENABLED_
//#define _PMC_ENABLED_
//#define INTEL_FDI          //- PIlot_Target won't build FDI


#if defined(INTEL_FDI) || defined(CALIBRATION_SDRAM)
#define EE_HANDLER_ENABLE
#endif

//#if (defined (_TAVOR_B0_SILICON_) && defined (_TAVOR_BOERNE_)) || defined (INTEL_2CHIP_PLAT_BVD) || !defined (INTEL_2CHIP_PLAT)     /* <Tavor Boerne B0>, <Bolverde in Hermon 2Chip configuration>, <Hermon 1Chip> */
#if defined (INTEL_2CHIP_PLAT_BVD) || !defined (INTEL_2CHIP_PLAT)     /* <Tavor Boerne B0>, <Bolverde in Hermon 2Chip configuration>, <Hermon 1Chip> */
#define USB_CABLE_DETECTION_ENABLED
#endif

#if defined HERMON_MCP2_CFG
#undef  _LCDIF_
#undef	_CAMIF_
#undef	_LCD_ENABLED_
#undef  _CAMIF_ENABLED_
#undef	_KEYPAD_ENABLED_
#undef	_MMCSD_ENABLED_

#undef	_PWM_ENABLED_
#undef	_ACD_ENABLED_
#endif


#if defined(HERMON_MVT) || defined(HERMON_ME)
#define _ME_ENABLED_
#endif

#endif /*_QT_*/


/*******************************************************************************
 *                      BVD & Boerne
 ******************************************************************************/
#if defined (INTEL_2CHIP_PLAT_BVD)
//BOERNE+BVD
#if defined(_TAVOR_BOERNE_) && defined( _QT_)
//no  _LCD_ENABLED_BVD on QT BOERNE
#else
//#define _LCD_ENABLED_BVD  - don't enable, must remove PIN MUX reconfig from lcd_hw_init(); to OBM - Harm ASPEC output!!!.
#endif

#if !defined(_TAVOR_BOERNE_)
#if (NO_APLP==0)
/*BULVERDE only*/
#define _I2S_ENABLED_
#define _SSP_ENABLED_BVD
#endif
#endif
#endif //!(INTEL_2CHIP_PLAT_BVD)




//#elif defined(_TAVOR_HARBELL_) || defined (SILICON_PV2)
#elif defined (SILICON_PV2)
#define I2C_ENABLED_
#define MEMC_ENABLED_
#define EE_HANDLER_ENABLE
#define _USIM_ENABLED_
//#define HARBELL_WDT_DISABLE <<-- not used
#elif defined(SILICON_TTC)
#define _USIM_ENABLED_
#endif /* _HERMON_B0_SILICON_ */



#if defined(PHS_SW_DEMO_TTC)
#define _USIM_ENABLED_        
#endif /* PHS_SW_DEMO_TTC */



#if defined(NVM_INCLUDE)
#if !defined(INTEL_FDI) /* May be defined also in TTP */
#define INTEL_FDI
#endif
#endif
//
// Hermon project specific block
//
#if defined(_HERMON_B0_SILICON_)
// Optional HAL module enable/disable settings
/*******************************************************************************/
/* Hermon power management */
#define HERMON_POWER_MANAGEMENT

/* Allow MSA subsystem dynamic management (required MSA firmware support) */
#define ALLOW_MSASS_SLEEP

/* Report XPCR changes to iPAT */
#define MIPS_TEST_FOR_SLEEP

#endif

/*******************************************************************************
 *                      G e n e r a l   F l a g s
 ******************************************************************************/


/* Use this line to DISABLE the use of XIRQ working through SINTC (choose
 * to work directly to Primary INTC */
//#define XIRQ_VIA_SINTC_DISABLE

/* Enable this line to workaround the RTC "SHIFT after power-up" problem */
//#define RTC_SHIFT_WORKAROUND

/* Start camera demo mode by default */
//#define CAMERA_RUN_ON_START
/*******************************************************************************
 *                      O v e r r i d e   F l a g s
 ******************************************************************************/

/* Enable/Disable Secondary INTC */
#if defined(_HERMON_B0_SILICON_)
    #if !defined (INTEL_2CHIP_PLAT_BVD) && !defined(_TAVOR_BOERNE_)
        #define _S_INTC_
    #endif
#endif /*_HERMON_B0_SILICON_*/

#if defined(_QT_)
    #if defined RTC_SHIFT_WORKAROUND
    #undef RTC_SHIFT_WORKAROUND //QT do not have FDI therefore the workaround can not be used
    #endif /*RTC_SHIFT_WORKAROUND*/
#else
    #if (defined(RTC_SHIFT_WORKAROUND) && !defined(INTEL_FDI)) //the workaround requires FDI
    #error "Must use INTEL_FDI for RTC_SHIFT_WORKAROUND"
    #endif
#endif /*_QT_*/

//
// Special configuration:
// ####################### SA on A0 ##########################
// see plw.mak, TV containing SA and A0
//
#if defined(_WHITESAIL_)

#define TIMER_DSYTMR
#define I2C_ENABLED_
#define MEMC_ENABLED_
#define USB_ENABLED_
#define USIM_ENABLED_
//#define PMC_ENABLED_
#define LCD_ENABLED_ //Reference LCD code
#define KEYPAD_ENABLED_

#define WATCHDOG_OFF

#endif



// Drivers Validation Team(DVT)  Defines.
#if defined (PLATFORM_ONLY)
   //#if defined(_TAVOR_BOERNE_) || defined(_TAVOR_HARBELL_) || defined(SILICON_PV2)
   #if defined(_TAVOR_BOERNE_) || defined(SILICON_PV2)
      #define COMMON_DVT_ENABLE
      #define I2C_TEST_ENABLE
      #define UART_TEST_ENABLE
   #endif
   //#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
   #if defined(SILICON_PV2)
      #define INTC_TEST_DVT_ENABLE
      #define USIM_TEST_DVT_ENABLE
      #define DMA_TEST_ENABLE
      //#define FDI_TEST_ENABLE
      //#define POWER_TEST_DVT_ENABLE
      //#define PMCHIP_TEST_DVT_ENABLE
      #define LDMA_TEST_DVT_ENABLE
  #endif

#endif    //PLATFORM_ONLY


// Non-HARBELL platforms
//#define COMMON_DVT_ENABLE
//#define DIAG_TEST_ENABLE
//#define I2C_TEST_ENABLE
//#define DMA_TEST_ENABLE
//#define RTCC_TEST_ENABLE
//#define RTC_TEST_ENABLE
//#define FDI_TEST_ENABLE
//#define FAT12_TEST_ENABLE
//#define TIMER_TEST_ENABLE
//#define MMCSD_TEST_ENABLE
//#define WATCHDOG_TEST_ENABLE
//#define USIM_TEST_ENABLE
//#define UART_TEST_ENABLE
//#define LCDIF_TEST_ENABLE
//#define INTC_TEST_ENABLE
//#define SSP_TEST_ENABLE
//#define KEYPAD_SILICON_TEST_ENABLE
// Harbell
    //#define COMMON_DVT_ENABLE
    //#define I2C_TEST_ENABLE
    //#define UART_TEST_ENABLE
    //#define UART_PC_TEST_ENABLE
#ifdef UART_PC_TEST_ENABLE
    #define DVT_DOUBLE_BUFFER_CLASS
#endif
	//#define IPC_TEST_DVT_ENABLE
    //#define TIMER_TEST_DVT_ENABLE
	//#define INTC_TEST_DVT_ENABLE
	//#define LDMA_TEST_DVT_ENABLE
	//#define RM_TEST_DVT_ENABLE
	//#define SSP_TEST_DVT_ENABLE
    //#define USIM_TEST_DVT_ENABLE
	//#define FDI_TEST_ENABLE
#endif   //Miniplat
#endif	 // _HAL_CFG_
