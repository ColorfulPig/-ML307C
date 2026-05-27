/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code (“Material? are owned
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

/*********************************************************************
*                      M O D U L E     B O D Y                       *
**********************************************************************
* Title: Diag                                                        *
*                                                                    *
* Filename: diag_port.h                                        *
*                                                                    *
* Target, platform: Common Platform, SW platform                     *
*                                                                    *
* Authors:                                                           *
*                                                                    *
* Description: Handles the external interface (interaction with ACAT)*
*                                                                    *
* Notes:                                                             *
*                                                                    *
*                                                                    *
*                                                                    *
*                                                                    *
************** General include definitions ***************************/

#ifndef DIAG_PORT_H
#define DIAG_PORT_H

//lets put internal data in struct - so we can control the alignment of tx/rx buffers
#define DIAG_USB_INSERT_FLAG_BIT 0x80000000

#define DiagPort_Printf_Msg 	0x01
#define DiagPort_Connect_Msg 	0x02
#define DiagPort_Cmd_Msg		0x03

typedef struct{
	unsigned char ServiceId;
	void *pData;
	unsigned short Length;
}diagPortRxData_S;

/*
 External data of the External interface shared-memory
*/
typedef struct {
	// indicator if Tx is allowed
	UINT32	bDiagSHMtxAllowed;
	UINT32  txIndRecieved;
	UINT32  txIndQQRecieved;
	// for debug  data
	UINT32	congestionConditionOnIf;
	UINT32  nTx;
	UINT32	nTxAck;
	UINT32  nTxFailAck;
	UINT32  nRx;
	UINT32  busyActive;
	UINT32  busyRel;
	UINT32  nLinkUp;
	UINT32	nLinkDown;
	UINT32  Handshaking;
} extDataSHMif_S;


typedef struct {
	OSAFlagRef  diag_usb_insert_flag_ref;	//diag_comm_msl_tx_flag_ref;
	UINT32      diag_port_busy;

} diagportIfData_S;



void diagPortSpecificInit(void);
void diagPortTransmit(UINT8 *msgPtr, UINT16 msgLength);
void fix_command(UINT8* data, UINT32 len);

//fix build warnning
void DiagDisable(void);
void DiagEnable(void);
#endif