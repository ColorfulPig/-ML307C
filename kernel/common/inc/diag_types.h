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


#ifndef _DIAG_TYPEDEFS_H_
#define _DIAG_TYPEDEFS_H_

#include "osa.h"

#define DIAG_STRUCT_REPORT 0xFF
#define DIAG_TEXT_REPORT   0xFE
#define DIAG_NVM_REPORT    0xFD

typedef struct ModuleMessageIDTag
{
	INT16	moduleID;	//Need to signed to support Comparison to INVALID_VALUE = (-1)
	UINT16	messageID;
}ModuleMessageID;

#define PASS_FILTER_BYTE(id) (((UINT16)(id))>>3)
#define PASS_FILTER_BIT(id)  ((UINT8)(id%8))
/* NOTE: negative logic: bit=0 means trace is sent, 1 - filtered out */
// also, if g_OverridePassFilter is set, indicate that there's no filter
#define PASS_FILTER(id) ((g_OverridePassFilter==1) || (((UINT8)(filterArray[PASS_FILTER_BYTE(id)])&(0x1<<((UINT8)(PASS_FILTER_BIT(id)))))==0))

#define DO_FOREVER  while (1)

typedef union
{
 struct {
   UINT8 icatNotReady; //BOOL
   UINT8 blockSendPDU; //BOOL
   UINT8 traceProhibited; //BOOL
 }flags;
 UINT32 all;
}DiagBlockReasons;

typedef enum
{
   DIAG_PEND_MSG_TX_OK=0,
   DIAG_PEND_MSG_TX_ERR
}DIAG_pendMsgSt;                  // status of pending messages in buffer


typedef enum
{
    DIAG_ERROR_CHANNEL_ALREADY_CLOSED = 0,
    DIAG_CLOSE_CHANNEL_OK
} DiagCloseChStatus;

typedef enum
{
    DIAG_CHAR = 0,
    DIAG_CHAR_BUFFER_SIZE_KNOWN,
    DIAG_CHAR_BUFFER_SIZE_UNKNOWN,
    DIAG_SHORT,
    DIAG_SHORT_BUFFER_SIZE_KNOWN,
    DIAG_SHORT_BUFFER_SIZE_UNKNOWN,
    DIAG_LONG,
    DIAG_LONG_BUFFER_SIZE_KNOWN,
    DIAG_LONG_BUFFER_SIZE_UNKNOWN,
    DIAG_STRING,
    DIAG_ENUM,
    DIAG_ENUM_SHORT,
    DIAG_ENUM_LONG,
    DIAG_ENUM_BUFFER_SIZE_KNOWN,
    DIAG_ENUM_BUFFER_SIZE_UNKNOWN,
    DIAG_ENUM_SHORT_BUFFER_SIZE_KNOWN,
    DIAG_ENUM_SHORT_BUFFER_SIZE_UNKNOWN,
    DIAG_ENUM_LONG_BUFFER_SIZE_KNOWN,
    DIAG_ENUM_LONG_BUFFER_SIZE_UNKNOWN
	//DIAG_ENUM_DUMMY_FOCE_4BYTE_VALUE=0x00FFFFFF
}ParameterType;

typedef struct{
    ParameterType   types;   //the parameter type
    UINT8          repititions;     //0 - no info about no. of repititions (open brackets), 1 or more - the no. of repititions
}ParameterInfo;

typedef struct{
    UINT16 reportId; 
    UINT8 moduleId;   
    UINT8 numOfParams;
    ParameterInfo parameters[1]; // variable length array
} DiagTraceDescriptor;

typedef struct{
    UINT16 reportId; 
    UINT8 moduleId;   
    UINT8 numOfParams;
    UINT8 miniInfo;
    ParameterInfo parameters[1]; // variable length array
} MiniDiagTraceDescriptor;


/*
typedef struct{
    ParameterType   types;   //the parameter type
    UINT16          repititions;     //0 - no info about no. of repititions (open brackets), 1 or more - the no. of repititions
}ParameterInfo;

typedef struct{
	UINT16 moduleId;
	UINT16 reportId;
	UINT16 numOfParams;
	ParameterInfo parameters[1]; // variable length array
} DiagTraceDescriptor;

*/


//typedef UINT32 * CommandAddress;	//Manitoba
typedef void (*CommandAddress)(void);	//Hermon

typedef char* CommandProto;
typedef const char * DiagDBVersion;

/* This enum defines the protocol types to be used when the diag logger */
/*	communicates with ICAT.Currently only PROTOCOL_TYPE_0 is supported	*/

// ICAT EXPORTED ENUM
typedef enum
{
	PROTOCOL_TYPE_0 = 0,
	MAX_PROTOCOL_TYPES
}ProtocolType;

//ICAT EXPORTED STRUCT
typedef struct
{
	BOOL bEnabled; //enable/disable the trace logging feature
	ProtocolType	eProtocolType;//protocol type for communication with ICAT, currently only protocol type 0 is supported
	UINT16			nMaxDataPerTrace;//for each trace, what is the maximum data length to accompany the trace, in protocol type 0, this is relevant only to DSP messages
}DiagLoggerDefs;

typedef struct {
//	UINT32 tm_id;
	UINT32 tm_uSec;
	UINT32 tm_sec;
	UINT32 tm_min;
	UINT32 tm_hour;
	UINT32 tm_mday;
	UINT32 tm_mon;
	UINT32 tm_year;
	UINT32 tm_wday;
	UINT32 tm_yday;
	UINT32 tm_isdst;
	UINT32 tm_zone;
}tm_t;


struct SUeCpInfo{
	INT32 i32Version;	//for the version of this structure.
	INT8 i8UeRatType;	//0=N/A, 1=LWG, 2=LTG, 3=WG, 4=TG
	INT8 i8SimType;		//0=N/A, 1=SSSS, 2=DSDS, 3=DSDL
	INT8 i8Reserved1;
	INT8 i8Reserved2;
	INT32 i32Reserved3;
	INT32 i32Reserved4;
	char szCpVersion[64];
	char szMsaVersion[64];
	char szBoardVersion[64]; //""=N/A, "MIFI", "Dongle", "LT02 R0.3"
	//v2 add those 3 fileds
	char szPlatformStream[32]; // ""= N/A, "HE_LWG_Dev", "HE_LWG_MOGG", "HE_LTG_DSDS_OT"
	char szProtocolStream[32]; // ""= N/A, "LTE_W_PS", "LTE_PS_DS_CSFB", "LTE_W_PS_MOGG"
	char szMsaStream[32]; // ""= N/A, "NeZha_5MD", "NeZha_5MD_MOGG", "Nezha_LTG"
	tm_t tmSystemTime;
	UINT32 SyncTimerTick;
};
// Filler entry value
#define PAD_ENTRY 0xffff

#endif
