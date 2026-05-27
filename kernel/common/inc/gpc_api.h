/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/***************************************************************************************************
 * INTEL CONFIDENTIAL
 * Copyright 2006 Intel Corporation All Rights Reserved.
 * The source code contained or described herein and all documents related to the source code
 * ("Material") are owned by Intel Corporation or its suppliers or licensors. Title to the
 * Material remains with Intel Corporation or its suppliers and licensors. The Material contains
 * trade secrets and proprietary and confidential information of Intel or its suppliers and
 * licensors. The Material is protected by worldwide copyright and trade secret laws and
 * treaty provisions. No part of the Material may be used, copied, reproduced, modified, published,
 * uploaded, posted, transmitted, distributed, or disclosed in any way without Intel's prior
 * express written permission.

 * No license under any patent, copyright, trade secret or other intellectual property right is
 * granted to or conferred upon you by disclosure or delivery of the Materials, either expressly,
 * by implication, inducement, estoppel or otherwise. Any license under such intellectual property
 * rights must be express and approved by Intel in writing.

 * Unless otherwise agreed by Intel in writing, you may not remove or alter this notice or any
 * other notice embedded in Materials by Intel or Intel's suppliers or licensors in any way.

 *      FILENAME:       gpc_api.h

 *      PURPOSE:        API Header File for GPC module

 **************************************************************************************************/
#ifndef __GPC_API_H__
#define __GPC_API_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "gbl_types.h"
#include "osa.h"
#include "msldl.h"
#include "msl_mem.h"

/* Default message size in bytes over GPC logical channels */
#define DEFAULT_GPC_SDU_SIZE MSL_MTU_SIZE

/* Maximum character string length for SSE names */
#define MAX_SSE_NAME_LEN 32

/* Maximum message size in bytes over GPC logical channels */
#define MAX_GPC_SDU_LEN MSL_MTU_SIZE

/* Maximum number of SSE provided receiving buffers */
#define MAX_RCV_BUF 16

typedef UINT32 GpcServiceDescriptor;
typedef UINT32 GpcChHandle;

typedef enum
{
    /* General status */
    GPC_SUCCESS = 0,    /* 0 -- Success */
    GPC_OP_REJECT,      /* 1 -- Operation is rejected */
    GPC_OP_TIMEOUT,     /* 2 -- Operation is time-out */
    GPC_ERROR,          /* 3 -- An error is detected */
    GPC_OUT_OF_MEM,     /* 4 -- Out of memory */
    GPC_INVALID_PARAM,  /* 5 -- API input parameter are invalid */
    GPC_NOT_SUPPORT,    /* 6 -- Features not support */

    /* Initializing status */
    GPC_INITIALIZED,    /* 7 -- Initialized */
    GPC_NOT_INITIALIZED,/* 8 -- Not initialized */
    GPC_INIT_PENDING,   /* 9 -- Control channel set up is in progress */

    /* SSE registration status */
    GPC_LOCAL_ONLY,         /* 10 -- SSE entity local registered only,
                                     no peer entity is found.*/
    GPC_REMOTE_FOUND,       /* 11 -- All peer SSEs are registered */
    GPC_REMOTE_UNREGISTERED,/* 12 -- Remote SSE unregistered */

    /* Channel status */
    GPC_CHAN_DISCONNECTED,  /* 13 -- Logical channel is disconnected */
    GPC_CHAN_PENDING,       /* 14 -- Channel establishing is in progress */
    GPC_CHAN_CONNECTED,     /* 15 -- Channel is established */
    GPC_CHAN_FAIL,          /* 16 -- Channel connect request is rejected */

    /* Data transmission status */
    GPC_TX_READY,   /* 17 -- Used in TX buffered mode. notify SSE
                             TX is ready for next transmission.*/
    GPC_TX_PENDING, /* 18 -- Indicate that TX is in progress */
    GPC_TX_CONFIRM, /* 19 -- Used in un-buffered mode, notify SSE
                             TX is ready for next transmission.*/
    GPC_TX_ERROR,   /* 20 -- Transmission error */
    GPC_TX_STOP,    /* 21 -- Tell sending side to stop transmit */
    GPC_TX_RESUME,  /* 22 -- Tell sending side to resume transmit */

    /* Data receive status */
    GPC_RX_SUCCESS, /* 23 -- Receiving succeed */
    GPC_RX_ERROR,   /* 24 -- Receiving error, such as received msg length is
                             larger than the buffer length SSE provided */

    /* Comm Error status */
    GPC_LINK_DOWN,  /* 25 -- GPC Link is down */
    GPC_LINK_UP,    /* 26 -- GPC Link is resumed */

    GPC_INVALID     /* 27 -- Invalid status */
} GpcStatus;

enum
{
    DEFAULT_GPC_PRIORITY = 9
};

/* TODO in the future
 * To be removed
 * DO NOT use it anymore
 */
typedef struct  _GpcChConfiguration
{
    UINT8 priority;                    //the channel priority
    UINT32 maxSduSize;            //maximum SDU size expected by calling SSE entity. The value of maxSduSize shall not exceed MAX_GPC_SDU_SIZE
    BOOL nonblock;                //the executing mode of the logical channel
}GpcChConfiguration;

typedef enum
{
    GPC_ST_GPCSTATE,    //GPC module's state,
    GPC_ST_SDSTATE,     //The service descriptor's state
    GPC_ST_CHANSTATE,   //a logical channel's state
    GPC_ST_TXSTATUS,    //current or latest Tx status for a logical channel
    GPC_ST_RXSTATUS     //current or latest Rx status for a logical channel
}GpcStatusType;

typedef union _GpcStatusAttribute
{
    GpcServiceDescriptor s; //a service descriptor, it is used for status  type GPC_ST_SDSTATE
    GpcChHandle handle;     //handle of a logical channel, it is used for status types: GPC_ST_CHANSTATE, GPC_ST_TXSTATUS, GPC_ST_RXSTATUS.
}GpcStatusAttribute;

typedef enum _GpcEvents
{
    GPC_EV_GPCSTATE,    //GPC module's state is changed
    GPC_EV_SDSTATE,     //a service descriptor's state is changed
    GPC_EV_CHANSTATE    //a logical channel's state is changed
}GpcEvents;

typedef struct _GpcEventChState
{
    GpcStatus      status;
    UINT8          chNum;   //channel number
}GpcEventChState;

typedef struct _GpcEventState
{
    GpcStatus      status;
    UINT8          reserved;
}GpcEventState;

typedef union _GpcEventParms
{
    GpcEventState gpcState;     //for event GPC_EV_ GPCSTATE
    GpcEventState sdState;      //for event GPC_EV_ SDSTATE
    GpcEventChState chState;    //for event GPC_EV_CHANSTATE
}GpcEventParms;

typedef enum _GpcDataMode
{
    GPC_MODE_DEFAULT = 0xF0,/* 0xF0 -- The default data mode */

    GPC_MODE_TX_BUFFERED,   /* 0xF1 -- TX buffered mode */

    GPC_MODE_TX_UNBUFFERED, /* 0xF2 -- TX un-buffered mode */

    GPC_MODE_RX_NORMAL,     /* 0xF3 -- RX normal mode (SSE as receiver need to
                                       provides RX buffer(s), and notify GPC
                                       when it is ready to receive, see API
                                       document for details */

    GPC_MODE_RX_SIMPLE,     /* 0xF4 -- RX simple mode (GPC will use internally
                                       allocated default buffer to receive
                                       data, then pass to SSE and SSE need not
                                       notify GPC when it is ready to receive,
                                       see API document for details */

    /* Temp for improving SICAT throughput rate */
    GPC_MODE_DIRECT,        /* 0xF5 -- TX direct un-buffered mode */

    GPC_MODE_INVALID        /* 0xF6 -- Invalid data mode */
}GpcDataMode;

typedef struct _GpcChannelConfig
{
    /*
     * Maximum SDU size expected by calling SSE entity.
     * The value of maxSduSize shall not exceed MAX_GPC_SDU_SIZE
     */
    UINT32 maxSduSize;      /* Default: DEFAULT_GPC_SDU_SIZE */

    /* The channel priority */
    UINT8 priority;         /* Default: DEFAULT_GPC_PRIORITY */

    /* The transmit/receive API mode of the logical channel */
    BOOL isNonblockAPI;     /* Default: TRUE*/

    /* The data mode of transmit */
    GpcDataMode transmitMode;   /* Default: GPC_MODE_TX_UNBUFFERED */

    /* The data mode of receiving */
    GpcDataMode receiveMode;    /* Default: GPC_MODE_RX_NORMAL */

    /* Receive buffer size when in receive normal mode*/
    UINT16 rxBufferSize;    /* Default: DEFAULT_GPC_SDU_SIZE */
} GpcChannelConfig;

/* Timeout configuration */
#define GPC_NON_BLOCK       OSA_NO_SUSPEND

#define GPC_BLOCK_SUSPEND   OSA_SUSPEND


/* call back function typedef */
typedef void (*GpcEventNotify)(
    GpcEvents event,
    GpcEventParms parms);

typedef void (*GpcTxConfNotify)(
    UINT8 chNum,
    UINT8 *buf,
    UINT32 len,
    GpcStatus status,
    UINT32 cookie);

typedef void (*GpcRxIndNotify)(
    UINT8 chNum,
    UINT8 *buf,
    UINT32 len,
    GpcStatus status,
    UINT32 cookie);

/******************************************************************************
 * Function: GPC_RegInitNotify
 *
 * Description:
 *  Register GPC with init notification callback
 *
 * Parameters:
 *  GpcEventNotify      Callback function for init status notification
 *
 * Return value:
 *  GPC_INITIALIZED     GPC is initialized successfully and control channel
 *                      is setup
 *  GPC_ERROR           Register Init Notification failed.
 *  GPC_SUCCESS         Register Init Notification succeed.
 *
 */
GpcStatus GPC_RegInitNotify(GpcEventNotify cbEventNotify);

/******************************************************************************
 * Function: GPC_Init
 *
 * Description:
 *  Initialize GPC, this API should be called by system instead of SSE -
 *  the GPC user.
 *
 * Parameters:
 *  None.
 *
 * Return value:
 *  GPC_INITIALIZED     GPC is initialized successfully and control channel
 *                      is setup
 *  GPC_INIT_PENDING    GPC initialization is in progress.
 *
 * Notes:
 *  This API should be called by system instead of GPC user.
 */
GpcStatus GPC_Init(void);

/******************************************************************************
 * Function: GPC_DeInit
 * Description:
 *  This function shuts down the MSL GPC facility, and returns all occupied
 *  resources back to the system. All registered SSE are removed from GPC
 *  module. After the MSL GPC facility is shutdown, any other GPC API calls,
 *  excepted GPC_Init() function call, will return GPC_NOT_INITIALIZED.
 *
 *  This API should be called by system instead of SSE - the GPC user.
 *
 * Parameters:
 *  None.
 *
 * Return value:
 *  GPC_SUCCESS             GPC is de-initialized successfully
 *  GPC_NOT_INITIALIZED     Otherwise.
 *
 * Notes:
 *  This API should be called by system instead of GPC user.
 */
GpcStatus GPC_DeInit(void);

/******************************************************************************
 * Function: GPC_getStatus
 *
 * Description:
 *  Query the status of GPC.
 *
 * Parameters:
 *  GpcStatusType           The status type to be queried.
 *
 *  GpcStatusAttribute*     The detailed infomation as the query input.
 *
 * Return value:
 *  GpcStatus       The result of this query.
 *
 * Notes:
 *   None.
 */
GpcStatus GPC_getStatus(GpcStatusType type, GpcStatusAttribute *attr);

/******************************************************************************
 * Function: GPC_registerSSE
 *
 * Description:
 *  allows an upper layer entity (SSE) to register itself or GPC services
 *
 * Parameters:
 *  char*                   Name of the calling SSE entity,
 *                          should not exceed MAX_SSE_NAME_LEN
 *
 *  UINT32                  Timeout for blocking interface in OS clock ticks
 *                          Valid values:
 *                          1) 1 - 4,294,967,293 (0xFFFFFFFD)
 *                          2) GPC_BLOCK_INFINITE(0xFFFFFFFE)
 *
 *  GpcEventNotify          Callback function for event and status notification
 *
 *  GpcServiceDescriptor*   GPC service descriptor for the calling SSE entity
 *                          Note: it is a return value.
 *
 * Return value:
 *  GPC_SUCCESS         Both local and remote SSE entity is registered
 *  GPC_OP_TIMEOUT      Registering time-out
 *  GPC_NOT_INITIALIZED GPC not initialized
 *  GPC_INVALID_PARAM   Invaid input parameter(s)
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_ERROR           Error happenned
 *
 * Notes:
 *   It is a synchronous API
 */
GpcStatus GPC_registerSSE(
    const char *sseName,
    UINT32 timeout,
    GpcEventNotify cbEventNotify,
    GpcServiceDescriptor *s);

/******************************************************************************
 * Function: GPC_UnRegister
 *
 * Description:
 *  Removes the SSE entity specified with service descriptor from the
 *  GPC communication services. After the GPC_UnRegister() is called,
 *  all active GPC logical channels associated with the specified SSE
 *  will be released, and the service descriptor is invalid.
 *
 * Parameters:
 *  GpcServiceDescriptor    The service descriptor to be unregistered.
 *
 * Return value:
 *  GPC_SUCCESS         Unregister successfully
 *  GPC_INVALID_PARAM   Invalid input parameter(s)
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_NOT_INITIALIZED GPC module not initialized
 *  GPC_ERROR           Error happenned
 *
 * Notes:
 *  None.
 */
GpcStatus GPC_UnRegister(GpcServiceDescriptor s);

/******************************************************************************
 * Function: GPC_connectChannel
 *
 * Description:
 *  This function creates an endpoint of a new GPC logical channel, and
 *  start to establish the connection between the remote SSE entity and
 *  the local SSE entity specified by s.
 *
 * Parameters:
 *  GpcServiceDescriptor    Handle of the Gpc service
 *
 *  UINT8                   Channel number, valid in 0~MAX_CHNUM_PER_SSE-1
 *
 *  GpcChannelConfig*       Channel configuration, if NULL, use default config
 *
 *  UINT32                  Timeout for blocking interface in OS clock ticks
 *                          Valid value
 *                          1) 1 - 4,294,967,293 (0xFFFFFFFD)
 *                          2) GPC_BLOCK_INFINITE(0xFFFFFFFE)
 *                          3) GPC_NON_BLOCK (0x0)
 *
 *  GpcTxConfNotify         Tx confirmation call back
 *
 *  GpcRxIndNotify          Rx indication call back
 *
 *  GpcChHandle*            Channel handle
 *                          Note: This is a return value
 *
 * Return value:
 *  GPC_CHAN_CONNECTED  The channel is established
 *  GPC_OP_TIMEOUT      Registering time-out when synchoronous mode
 *  GPC_CHAN_PENDING    The channel establishment procedure is ongoing when
 *                      asynchronous mode.
 *  GPC_INVALID_PARAM   Invalid input parameter(s)
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_ERROR           Error happenned
 *
 * Notes:
 *  This is synchoronous/asynchronous API depending on the timeout parameter.
 */
GpcStatus GPC_connectChannel(
    GpcServiceDescriptor s,
    UINT8 chNum,
    GpcChannelConfig *config,
    UINT32 timeout,
    GpcTxConfNotify cbTxConfNotify,
    GpcRxIndNotify cbRxIndNotify,
    GpcChHandle *handle);

/******************************************************************************
 * Function: GPC_Disconnect
 *
 * Description:
 *  The GPC_Disconnect() function call disconnects a logical channel specified
 *  by handle. All the pending Tx/Rx user data messages are discarded.
 *  The allocated computing resources return to system. The GPC is responsible
 *  for notifying the remote channel endpoint for releasing the channel.
 *
 * Parameters:
 *  GpcChHandle     The handle of channel to be disconnected
 *
 * Return value:
 *  GPC_SUCCESS         Channel closed successfully
 *  GPC_INVALID_PARAM   Invalid input parameter(s)
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_ERROR           Error happenned, the channel's behavior is undefined
 *                      after error.
 *
 * Notes:
 *  None.
 */
GpcStatus GPC_Disconnect(GpcChHandle handle);

/******************************************************************************
 * Function: GPC_TransmitRequest
 *
 * Description:
 *  Transmits data to the peer SSE entity
 *
 * Parameters:
 *  GpcChHandle Chnanel handle
 *  UINT8*      Pointer to a data buffer containing a Tx message
 *
 *  UINT32      Payload data len in bytes
 *
 *  UINT32      User specific, the same value is given back to user with
 *              the callback function call.
 *
 * Return value:
 *  GPC_SUCCESS         Transmit request has been processed successfully
 *  GPC_OP_REJECT       This transmit is rejected, SSE need to try it again
 *                      after at least one previous transmitting is finished.
 *  GPC_INVALID_PARAM   Invalid input parameter
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_ERROR           Error happenned
 *
 * Notes:
 *  For un-buffered transmission mode, buf needs to reserve 4 bytes in the
 *  beginning, and needs 32 bytes aligned for DMA transmission requirement.
 *  len is the data payload in bytes excluding the reserved 4 bytes
 *
 *  For buffered transmission mode, len is the data payload excluding
 *  the 4 reserved bytes and there is no restriction to buf.
 */
GpcStatus GPC_TransmitRequest(
    GpcChHandle handle,
    UINT8 *buf,
    UINT32 len,
    UINT32 cookie);

/******************************************************************************
 * Function: GPC_dataReceiveReady
 *
 * Description:
 *  Provide a Rx buffer to GPC for a specific logical channel.
 *
 * Parameters:
 *  GpcChHandle channel handle
 *
 *  UINT8   Pointer to a Rx data buffer ready to store the received message
 *
 *  UINT8   Number of buffers
 *
 *  UINT32  User specific, the same value is given back to user with
 *          the callback function call.
 *
 * Return value:
 *  GPC_SUCCESS         If receive request has been enqueued successfully
 *  GPC_INVALID_PARAM   Invalid input parameter
 *  GPC_NOT_SUPPORT     Specified opeartion is not supported so far
 *  GPC_OUT_OF_MEM      Run out of memory
 *  GPC_ERROR           Error happenned
 *
 * Notes:
 *  Multiple Rx buffer at one time is not supported yet.
 */
GpcStatus GPC_dataReceiveReady(
    GpcChHandle handle,
    UINT8 *buf,
    UINT8 buf_num,
    UINT32 cookie);

/******************************************************************************
 * APIs to be supported.
 *
 * Do NOT use them now.
 */
UINT32 GPC_getRemoteRxBufferSize(GpcChHandle handle);

UINT32 GPC_getMaxBurstTxRequestNumber(GpcChHandle handle);

/******************************************************************************
 * Obsoleted APIs, will be removed soon.
 *
 * Do NOT use them any more.
 */
GpcStatus GPC_Register(const char *sseName, GpcServiceDescriptor *s, GpcEventNotify cbEventNotify);

GpcStatus GPC_Connect(GpcServiceDescriptor s,  UINT8 chNum, GpcChHandle *handle, GpcChConfiguration *config, BOOL buffered, GpcTxConfNotify cbTxConfNotify, GpcRxIndNotify cbRxIndNotify);

GpcStatus GPC_ReceiveReady(GpcChHandle handle, UINT8 *buf, UINT32 len, UINT32 cookie);

INT32 GPC_Send(GpcChHandle handle, UINT8 *buf, UINT32 len);

INT32 GPC_Recv(GpcChHandle handle, UINT8 *buf, UINT32 len);

GpcStatus GPC_Status(GpcStatusType type, GpcStatusAttribute *attr);


#ifdef __cplusplus
}
#endif //__cplusplus

#endif
