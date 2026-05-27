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
* Title: ICAT config
*
* Filename: ICAT_config.h
*
* Target, platform:
*
* Authors: Yan M, Yaeli Karni
*
* Description:
*
* Last Updated:
*
* Notes:
*******************************************************************************/

#ifndef _ICAT_CONFIG_H_
    #define _ICAT_CONFIG_H_

/*----------- Global defines -------------------------------------------------*/
/*
  commStatusHndGetDevice in softutil\trasnport\src\commStatusHnd.c returns for
    PORT_ONE    - UART_PORT_FFUART
    PORT_TWO    - UART_PORT_BTUART
    PORT_THREE  - UART_PORT_STUART
    and is called from commL4OpenChannel in softutil\trasnport\src\commStatusHnd.c
  which is called by
  diagRxTask in diag/diag_logic/src/diag_rx.c and
  diagCloseChNotification diag/diag_logic/src/diag_init.c
*/
	#include "osa.h"

#ifdef PHS_SW_DEMO_TTC
	/* Added by gwl */
#ifndef _DIAG_USE_COMMSTACK_
#define _DIAG_USE_COMMSTACK_
#endif
	
#ifndef _DIAG_ENABLED_
#define _DIAG_ENABLED_
#endif
	
#ifndef _TAVOR_DIAG_
#define _TAVOR_DIAG_
#endif		
#endif //#ifdef PHS_SW_DEMO_TTC	

	#if defined OSA_NUCLEUS
		#include "bsp.h"

		#define DIAG_SIG_TRACE_DISABLE       // DISABLE GKI SIGNAL TRACE TO ICAT by default: use ICAT commands for run-time control

		#if !defined (FULL_SYSTEM)
			#define DIAG_L1GKI                   // To use L1 GKI for Signal Tracing
		#endif

		#if defined(NVM_INCLUDE)
			#define DIAG_NVM_FILTERS
		#endif
		// These two UART port definitions are relevant for _DIAG_USE_COMMSTACK_ case only
			#if defined(_HERMON_B0_SILICON_) //SDK    for Hermon & Boerne
				#define ICAT_PORT           ((bspBoardType()!=BOARD_HERMON_DVK)?PORT_ONE:PORT_THREE)
				#define ICAT_UART_PORT      ((bspBoardType()!=BOARD_HERMON_DVK)?UART_PORT_FFUART:UART_PORT_STUART)
	     	#else  //Setting for UART on HRBL & TTC
			    #define ICAT_PORT           (PORT_THREE)
			    #define ICAT_UART_PORT      (UART_PORT_STUART)
		    #endif  // _HERMON_B0_SILICON_
		#endif  //OSA_NUCLEUS

//#ifdef  PHS_SW_DEMO_TTC
#if 0
#define DIAG_USB_TX_ENDPOINT			 USB_DEVICE_ENDPOINT_7
#define DIAG_USB_RX_ENDPOINT			 USB_DEVICE_ENDPOINT_8
#endif
	// is this USB definition?
	#if !defined (_DIAG_USE_SSP_)
		#if !defined (_DIAG_USE_COMMSTACK_)
		// USB is used for DIAG-ICAT communication
			#define DIAG_USB_TX_ENDPOINT             USB_DEVICE_ENDPOINT_7
			#define DIAG_USB_RX_ENDPOINT             USB_DEVICE_ENDPOINT_8
			#define DIAG_TX_USB_ENDPOINT_LISR        INTC_USB_ENDPOINT_1
			#define DIAG_RX_USB_ENDPOINT_LISR        INTC_USB_ENDPOINT_2

			#if defined OSA_NUCLEUS
				#define DIAG_TX_USB_DMA //uncomment to use DMA for TX
 			#endif  //OSA_NUCLEUS
			//#define DIAG_RX_USB_DMA //uncomment to use DMA for RX - not supported!

			//assign DMA channels:
			//the following file must define DIAG_USB_DMA_TX_CH_NUM and DIAG_USB_DMA_RX_CH_NUM
			// if DIAG_TX_USB_DMA and / or DIAG_RX_USB_DMA respectively are enabled
			#if defined OSA_NUCLEUS
				#include "dma_channel.h"
			#endif  //OSA_NUCLEUS
		#endif   //_DIAG_USE_COMMSTACK_
	#endif  //_DIAG_USE_SSP_

	#if defined(_QT_)
		#define ICAT_MINIMAL /* Minimize resources used for DIAG/ICAT */
	#endif

	// Define the following for non-GKI build. Optional with ARM tools, mandatory with other tools (Intel SDT).
	// Affect: DiagStubxxxxxxx.c
	// 1. ARM tools: ARM linker follows the input file order so the stubs are taken only when no real GKI is present
	// 2. WIth other tools, need to define DIAG_NO_GKI, otherwise stubs can be taken instead of
	// real GKI implementation
	//  The best place for this flag definition is the TARGETxx.MAK
	#if defined(PLATFORM_ONLY) && !defined(DIAG_NO_GKI)
		#define DIAG_NO_GKI
	#endif

	// GKI signals configuration (should be aligned with GKI used)
	// TASKID size (default is 2-byte)
	//#define DIAG_1_BYTE_TASK_IDS
	//#define DIAG_4_BYTE_TASK_IDS
	// Signal-ID size (default is 2-byte)
	#define DIAG_4_BYTE_SIGNAL_IDS


/*----------- Global macro definitions ---------------------------------------*/

/*----------- Global type definitions ----------------------------------------*/

/*----------- Extern definition ----------------------------------------------*/

/*----------- Global variable declarations -----------------------------------*/

/*----------- Global constant definitions ------------------------------------*/

/*----------- Global function prototypes -------------------------------------*/

#undef EXTERN
#endif  /* _ICAT_CONFIG_H_ */
