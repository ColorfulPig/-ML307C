/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
**
**  COPYRIGHT (C) 2001, 2002 Intel Corporation.
**
**  This software as well as the software described in it is furnished under 
**  license and may only be used or copied in accordance with the terms of the 
**  license. The information in this file is furnished for informational use 
**  only, is subject to change without notice, and should not be construed as 
**  a commitment by Intel Corporation. Intel Corporation assumes no 
**  responsibility or liability for any errors or inaccuracies that may appear 
**  in this document or any software that may be provided in association with 
**  this document. 
**  Except as permitted by such license, no part of this document may be 
**  reproduced, stored in a retrieval system, or transmitted in any form or by 
**  any means without the express written consent of Intel Corporation. 
**
**  FILENAME:       audio_stub.h
**
**  PURPOSE:       contains functions for the audio stub functions
**          
**  NOTES:      The following funcs are implemented in this file                
**                  
**
******************************************************************************/
#ifndef _AUDIO_STUB_H_
#define _AUDIO_STUB_H_

#include "gbl_types.h"
#include "osa.h"
#include "acipc_data.h"

#include "audio_stub_config.h"



#define AUDIO_TEST_TRACE(fmt, ...)
//#define AUDIO_TEST_TRACE		uart_printf	//UartLogPrintf


#define AUDIO_STUB_PORT  	ACIPCD_AUDIO_SID   
#define AUDIO_CLIENT_SHAKE_REQ 	1
#define AUDIO_CLIENT_SHAKE_CNF 	2
#define AUDIO_CLIENT_DATA_REQ 	3
#define AUDIO_CLIENT_DATA_CNF 	4
#define AUDIO_SERVER_DATA_REQ 	5
#define AUDIO_SERVER_DATA_CNF 	6

/* maximun allow number of bytes transfer */
#define MAX_SPEECH_DATA_FRAME_SIZE  160 //for PCM streaming
#define MAX_CODED_SPEECH_PACKET     19  //for vocoder streaming
#define MAX_TONE_PACKET             7   //for tone streaming
#define UP_LINK_NUMBER_OF_BUFFERS   2
#define DOWN_LINK_NUMBER_OF_BUFFERS 2
#define DEFAULT_STREAMING_TICK_FREQ 1


#define AP_MSG_NO_CONTENT       (1<<0)
#define AP_MSG_WITH_CONTENT     (1<<1)
#define AP_SET_MODEM_MASTER     (1<<2)
#define AP_SUPPORT_WBAMR        (1<<3)
#define AP_SUPPORT_SLIC         (1<<4)

#define AP_INIT_FLAGS_MASK      (AP_MSG_NO_CONTENT|AP_MSG_WITH_CONTENT|AP_SET_MODEM_MASTER|AP_SUPPORT_WBAMR|AP_SUPPORT_SLIC)



typedef struct _ShmApiMsg
{
	//ShmServiceId svcId;
	int svcId;
	int procId;
	int msglen;
	//unsigned char msgData[64];
}ShmApiMsg;


// data structure used for messaging between audio client driver API and stub task
typedef struct _AudioStubClientSendReq { 
  UINT8   prodId;   // procedure ID
  void    *pData;   // pointer to a memory that contains
              // data to be transmitted 
  UINT16    size;   // size of the data pointed to by pData
} AudioStubClientSendReq;

// data structure used for messaging between audio server manager API and stub task
typedef struct _AudioStubServerSendReq { 
  UINT32  cbId;     // callback ID
  void  *pData;     // pointer to a memory that contains
              // data to be transmitted 
  UINT16  size;     // size of the data pointed to by pData
} AudioStubServerSendReq;


//Message ID for audio stub queue
enum
{
    AudioStbcDataInd_MSG=0,
    AudioDataSendReq_MSG
};


// procedure IDs
enum
{
  AUDIO_END_ID=99
};

// golbal variables.
// gobal variables sahared between init APis and the stub tasks
extern OSAMsgQRef audioServerStubPCMMsgQ;


#endif

