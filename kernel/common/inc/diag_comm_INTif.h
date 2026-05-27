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

/*********************************************************************
*                      M O D U L E     B O D Y                       *
**********************************************************************
* Title: Diag                                                        *
*                                                                    *
* Filename: diag_comm_INTif.h                                        *
*                                                                    *
* Target, platform: Common Platform, SW platform                     *
*                                                                    *
* Authors: Yaeli Karni                                               *
*                                                                    *
* Description: handles the interaction with other core				 *
*				via MSL or Shared Memory                             *
*                                                                    *
* Notes:                                                             *
*	Yaeli Kanri	Aug 2006	enhance the code, add locking.Rx-task	 *
*	Yaeli Karni	Aug 2007	Use GPC APIs new/legacy. Mutli OS		 *
*   Yaeli KArni	Sep 2007	Add MSL-SAL interface. Main issues		 *
*							- the buffers are allocated by the       *
*                             gen_stub interface.					 *
*							- Usage of cookie in rx is different.	 *
*							  gen_stub gives a cookie with each rx   *
*							  buf. The cookie should be given when	 *
*							  returning the buffer.					 *
*	Yaeli Karni Jan 2008	creation of diag_comm_INTif.c/h and 	 *
*							specific implementation for MSL, Shared  *
*							memory									 *
*                                                                    *
*                                                                    *
*                                                                    *
*                                                                    *
*                                                                    *
************** General include definitions ***************************/

#if !defined (_DIAG_COMM_INTIF_H_)
#define _DIAG_COMM_INTIF_H_

#include "osa.h"
#include "diag_osif.h"
#include "global_types.h"

#include "ICAT_config.h"
#include "diag_config.h"
#include INCLUDE_ASSERT_OR_DUMMY
#include "diag_header_external.h"

#include "diag_comm.h"
#include "diag_init.h"

#if defined (DIAG_INT_IF)

#if defined(OSA_NUCLEUS) && !defined(DIAG_APPS)
#define DEBUG_MSL_ACK_ON_COMM_SIDE
#endif

// Defines 
#define DIAG_TX_CONF_FLAG_BIT                       0x1
#define DIAG_TX_FLAG_TIMER_EXPIRED_BIT              0x2
#define DIAG_TX_FLAG_MSG_IN_QUEUE_BIT               0x4
#define DIAG_TX_RESET_QUEUE_BIT                     0x8
#define DIAG_RX_CONF_FLAG_BIT						0x10

#if defined (OSA_WINCE)		   // stack	is not used by OSA in WinCE - set to small value
#define DIAG_INTIF_TX_TASK_PRIORITY     1	// solution for 'calibration issue' - thread prioiority was set to 1 by Israel Davidenko
#define DIAG_INTIF_RX_TASK_PRIORITY     4 // was 6
#define DIAG_INTIF_TX_TASK_STACK		  256
#define DIAG_INTIF_RX_TASK_STACK		  256
#else
#if defined (OSA_LINUX)		   
#define DIAG_INTIF_TX_TASK_PRIORITY     10 /* RT priority to make CP/MSL/INTIF tolerant to AP heavy load */
#define DIAG_INTIF_RX_TASK_PRIORITY     10 /* RT priority to make CP/MSL/INTIF tolerant to AP heavy load */
#else
#ifndef PHS_SW_DEMO_TTC
#define DIAG_INTIF_TX_TASK_PRIORITY     42
#else
#define DIAG_INTIF_TX_TASK_PRIORITY     46
#endif
#define DIAG_INTIF_RX_TASK_PRIORITY     DIAG_DEFUALT_PRIORITY
#endif
#define DIAG_INTIF_TX_TASK_STACK		  1024
#define DIAG_INTIF_RX_TASK_STACK		  768 /* DiagTx ack may be sent on the RX context and goes
                                               * very deep into proc-calls with 572 bytes*/
#endif

// align to 32 - work on UINT16...
#define ALIGN_TO_32_BIT(a)		{a = ((a & 0x0003) == 0) ? a : ((a&0xFFFC)+4);}

//ICAT EXPORTED STRUCT
typedef struct
{
    UINT8     MIN_BUFFERS_NUM;
    UINT8     TASK_TIMER_PERIOD;
    UINT8     task_priority;
} DiagIntIfTaskConfig;

typedef struct {
	UINT8 *	buf;		// pointer to data
	UINT32	len;		// len of data
	UINT32  cookie;		// cookie which goes with data
} IntIfRxBufData_S;

//lets put internal data in struct - so we can control the alignment of tx/rx buffers
typedef struct {
	OSAFlagRef  diag_comm_IntIf_tx_flag_ref;	//diag_comm_msl_tx_flag_ref;
	OSATaskRef	diag_IntIf_tx_task_ref;
	OSAMsgQRef	diag_comm_IntIf_rxQ_flag_ref; //for SAL
	OSATaskRef	diag_IntIf_rx_task_ref;

	// Tx variable
	UINT16  	diagNumOfSentMessages;
	UINT16		diagNumOfQQSentMessages;
	UINT16		diagQuickBuffSendInProgress;

	UINT32     _diag_IntIf_busy;
#ifdef PHS_SW_DEMO_TTC
	UINT32     _diag_IntIf_LowerWmhappen;
#endif
	// Diag over IntIf to Diag Rx data structure:
	DIAG_COM_RX_Packet_Info  diagRxPacketInfo;

} diagINTifData_S;


// variables  - data structure of the internal interface
extern diagINTifData_S intDataIntIf;

// msg-q for outgoing traces over internal interface
extern DiagReportItem_S	*_diagIntIfReportsList;
extern DiagQPtrData_S *_diagIntIfQPtrData;

// functions prototypes

// local functions for INT-IF
void DiagCommIntIfMultiTxTimerCallback (UINT32 param);
void diagIntIfBuffRelease (void);	// release the buffers from all Queues
void diagIntIfBuffReset (void);

#if defined(DEBUG_MSL_ACK_ON_COMM_SIDE)
void mslAckCallback(UINT32 param);
#endif

// APIs expected by the specific-internal-interface
BOOL diagIntIfIsTxAllowed (void);
// The main Tx notification handler:
void diagCommIntIfTxConf (void);
void diagCommMultiTransmitOverIntIf (UINT16 indexInReportsListArray, UINT16 numOfDiagMsgsInQ, UINT32 bQuickQ);
// Hendles the processing of recieved buffer
void diagCommIntIfRxConf (IntIfRxBufData_S *data);
void diagCommIntIfSpecificInit(void);


#endif	 //DIAG_INT_IF - general internal interface		//1234

#endif //_DIAG_COMM_INTIF_H_


