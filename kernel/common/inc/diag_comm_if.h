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

#if !defined (_DIAG_COMM_IF_H_)
#define _DIAG_COMM_IF_H_

#include "diag_buff.h" //TBDIY for extern DiagReportItem_S *_diagIntIfReportsList and extern DiagQPtrData_S *_diagIntIfQPtrData

//Definitions for Transmit Task
#define DIAG_TX_DONE_FLAG_BIT		              0x1
#define DIAG_TX_FLAG_TIMER_EXPIRED_BIT            0x2
#define DIAG_TX_FLAG_QUEUE_ADD_BIT                0x4
#define DIAG_SINGLE_TX_DONE						  0x8
#define DIAG_TX_DISCONNECT_BIT                    0x10
#define DIAG_TX_CONNECT_BIT                       0x20
#define DIAG_TX_RESET_QUEUE						  0x40
#define DIAG_TX_TIMEOUT						      2000

typedef struct
{
 BOOL       bIsIfBusy;
 UINT32	    NBuffTxNotify;	// keeps number of tx-notifications bufferes till handled in task
 DIAG_COM_RX_Packet_Info  *RxPacket;
 OSFlagRef  TxFlgRef;
 OSATaskRef TxTaskRef;
 OSFlagRef  RxFlgRef;
 OSATaskRef RxTaskRef;
} GlobalIfData;

// functions called from specific OS implementation
extern UINT32 diagCommSetChunk(DIAG_COM_RX_Packet_Info  *RxPacket,CHAR *msgData, UINT32 length, UINT32 bRemoveHeader);//TBDIY moved from diag_comm_EXTif.h
extern void diagBuffRelease(DiagReportItem_S *RepList, DiagQPtrData_S *QPtrData, GlobalIfData *dataIf);
extern void diagBuffReset(DiagReportItem_S *RepList, DiagQPtrData_S *QPtrData, GlobalIfData *dataIf);

#endif //_DIAG_COMM_IF_H_

