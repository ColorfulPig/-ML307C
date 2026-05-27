/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
* DSPC Proprietary Information, (C) COPYRIGHT 2000 DSPC, an Intel Company
* File name:    cfw_ipc.h
* Programmer:   Evgeny M.
* Create Date:  Jan, 2002
* Description:
*   IPC (Inter Processor Communication) header file.
*
* Notes:
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#ifndef _CFW_IPC_H_
#define _CFW_IPC_H_ 1
#include "cfw_typedef.h"      // Common FW generic types

typedef struct {
    UINT16	SubOpcode;
    UINT16	CmdMsgLength;
    UINT32 *	CmdMsgPtr;
    UINT8		UrgentFlag;
} IpcSetParamsType;

typedef enum {
    IPC_SETID_VOICE = 1,
} IpcSetIDType;

enum {
    VOICE_ECALL_MSG = 0x37,    //0x0077
};


void IpcSendMessage(int SET, IpcSetParamsType* Msg);           //todo list !!!!

#endif // _CFW_IPC_H_

