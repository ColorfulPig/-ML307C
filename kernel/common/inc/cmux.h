#ifndef _CMUX_H_
#define _CMUX_H_
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                cmux.h


GENERAL DESCRIPTION

    This file is for cmux.

EXTERNALIZED FUNCTIONS

INITIALIZATION AND SEQUENCING REQUIREMENTS

   Copyright (c) 2018 by ASR, Incorporated.  All Rights Reserved.
*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

*===========================================================================

                        EDIT HISTORY FOR MODULE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.


when         who        what, where, why
--------   ------     ----------------------------------------------------------
08/31/2018   zhoujin    Created module
===========================================================================*/


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "UART.h"
#include "cmux_def.h"

/*===========================================================================

                          Struct definition.

===========================================================================*/

//ICAT EXPORTED ENUM
typedef enum DLC_SVC_TYPE{
	DLC_SVC_TYPE1 = 0,  //APSE
	DLC_SVC_TYPE2       //INTEL
} DLC_SVC_TYPE;

//ICAT EXPORTED ENUM
typedef enum cmux_hal_type{
	MUX_USB = 0,
	MUX_UART,
	MUX_SPI,
	MUX_SDIO,
	MUX_HSI,
	MUX_ASI
}cmux_hal_type;

typedef enum cmux_mode_type{
    CMUX_DISABLE = 0,
    CMUX_USB_ENABLE,
    CMUX_UART_ENABLE,
    CMUX_MUX_TYPE
}cmux_mode_type;

typedef enum ts0710_events {
	CONNECT_IND,
	CONNECT_CFM,
	DISCONN_CFM
}ts0710_events;

typedef enum{
    DlcOpened=1,
    DlcClosed,
    DlcFlowBlockedPeer,
    DlcFlowUnblockedPeer,
    DlcFlowBlockedBufOver,
    DlcFlowUnblockedBufover
}DlcLinkStatus;

typedef struct CmuxRcvChanInfo
{
	UINT16 Apt_index;
	UINT8 cid;
	UINT8 datastop;
	UINT32 status;
	UINT32 AtStatus;   //0 at channel idle 1 at channel busy
}CmuxRcvChanInfo;

typedef struct flow_control_info{
	unsigned int send_stop_req_count;
	unsigned int send_start_req_count;
	unsigned int rec_stop_req_count;
	unsigned int rec_start_req_count;
}flow_control_info;

typedef struct cmux_data_node
{
    UINT16 length;
	UINT8 frametype;
	UINT8 frame_cr;
	void *data;
	void *start_data;
	struct cmux_data_node *next, *prev;
}cmux_data_node;

/* CMUX device information. */
typedef struct cmux_dev_info
{
	char string[7];
	cmux_hal_type type;
} cmux_dev_info;

typedef void (*tx_done_t)(unsigned int dlc,void* buffer, unsigned int length);
typedef int (*rx_ind_t)(unsigned int dlci);

typedef void (*DlcLinkStatusIndCB)(unsigned int dlci, DlcLinkStatus status);

typedef int (*mux_device_write_cb)(char *buf, int length);
typedef int (*mux_frame_write_cb)(cmux_data_node *frame_header, int length);

struct cmux_dlc
{
	UINT8 usingflag;			    /* open flag*/
	UINT8 id;			            /* channel idenfier*/
	UINT8 priority; 			    /* priority of this DLC[0 - 63]*/
	UINT8 flow_control;		        /* fc bit on opposite DLC */
	UINT8 state;				    /* channel state */
	UINT8 dlc_recv_state;   	    /* receiving, idle*/

	UINT16 n2;					    /* maximum number of re-transmissions*/
	UINT16 n1; 				        /*maximum frame size(MTU)*/
	UINT16 window_size;		        /* size of receipt window */
	UINT16 safe_window_size;	    /* less than this value unblocked dlc channel*/

	UINT16 rx_queue_count;
	UINT16 tx_queue_count;
	UINT16 max_tx_queue_count;
	UINT16 dlc_work_mode;           /*0:at mode;1:data mode*/
	UINT8  v24_signal;
	struct cmux_dlc	*next;		    /* next element in  priority array */
	struct cmux_dlc	*prev;		    /* previous element in  priority array */
	struct cmux_data_node *rx_queue_header, *rx_queue_tailer;
	struct cmux_data_node *tx_queue_header, *tx_queue_tailer;

	rx_ind_t 	rx_ind;
	tx_done_t	tx_done;
	DlcLinkStatusIndCB link_status_ind;
};

/* Cmux Rx request message */
typedef struct CmuxTxReqMsg
{
	UINT32	id;
} CmuxTxReqMsg;

/* Cmux Rx request message */
typedef struct CmuxRxReqMsg
{
	UINT32	dlci;
} CmuxRxReqMsg;

typedef struct cmux_info
{
	UINT8 initiator;	            /* initiator:1 responder:0 */
	UINT8 openflag;                 /* Open flag */
	OSATimerRef timer;              /* for DLC0 */
	struct cmux_dlc dlc[NUM_DLCI];
}cmux_info;

typedef struct prio_array{
	struct cmux_dlc *head;
	struct cmux_dlc *tail;
}prio_array;

typedef struct{
    unsigned short service_id;
    unsigned short priority;
    unsigned short MTU;
    unsigned short feature;
}dlc_cfg_parameter;

//ICAT EXPORTED STRUCT
typedef struct {
	DLC_SVC_TYPE DlcSvcType;
	cmux_hal_type MuxHalType;
} MUX27010Cfg_t;

typedef struct{
	unsigned char type;			        /* custom type */
	DLC_SVC_TYPE MuxDlcSvcType;		    /* custom name */
	cmux_hal_type MuxHalType;
}CUSTOM_MUX_MAP_ITEM;

typedef struct {
	UINT8 ea:1;
	UINT8 cr:1;
	UINT8 server_chn:6;
} address_field;

typedef struct {
	UINT8 ea:1;
	UINT8 len:7;
}short_length;

typedef struct {
	UINT8 ea:1;
	UINT8 l_len:7;
	UINT8 h_len;
}long_length;

typedef struct {
	address_field addr;
	UINT8 control;
	short_length length;
}short_frame_head;

typedef struct {
	short_frame_head h;
	UINT8 data[0];
}short_frame;

typedef struct {
	address_field addr;
	UINT8 control;
	long_length length;
	UINT8 data[0];
} long_frame_head;

typedef struct {
	long_frame_head h;
	UINT8 data[0];
}long_frame;

/* Typedefinitions for structures used for the multiplexer commands */
typedef struct {
	UINT8 ea:1;
	UINT8 cr:1;
	UINT8 type:6;
}  mcc_type;

typedef struct {
	mcc_type type;
	short_length length;
	UINT8 value[0];
} mcc_short_frame_head;

typedef struct {
	mcc_short_frame_head h;
	UINT8 value[0];
} mcc_short_frame;

/* MSC-command */
typedef struct {
	UINT8 ea:1;
	UINT8 fc:1;
	UINT8 rtc:1;
	UINT8 rtr:1;
	UINT8 reserved:2;
	UINT8 ic:1;
	UINT8 dv:1;
}v24_sigs;

typedef struct {
	UINT8 ea:1;
	UINT8 b1:1;
	UINT8 b2:1;
	UINT8 b3:1;
	UINT8 len:4;
}brk_sigs;

typedef struct {
	address_field dlci;
	UINT8 v24_sigs;
}msc_t;

/* PN-command */
typedef struct {
	short_frame_head s_head;
	mcc_short_frame_head mcc_s_head;
	UINT8 dlci:6;
	UINT8 res1:2;
	UINT8 frame_type:4;
	UINT8 credit_flow:4;
	UINT8 prior:6;
	UINT8 res2:2;
	UINT8 ack_timer;
	UINT8 frame_sizel;
	UINT8 frame_sizeh;
	UINT8 max_nbrof_retrans;
	UINT8 credits;
	UINT8 fcs;
} pn_msg;

/* PN-command */
typedef struct {
	UINT8 dlci:6;
	UINT8 res1:2;
	UINT8 frame_type:4;
	UINT8 convergence_layer:4;
	UINT8 prior:6;
	UINT8 res2:2;
	UINT8 ack_timer;
	UINT8 frame_sizel;
	UINT8 frame_sizeh;
	UINT8 max_nbrof_retrans;
	UINT8 k;
//	UINT8 fcs;
} pn_t;

/* NSC-command */
typedef struct {
	short_frame_head s_head;
	mcc_short_frame_head mcc_s_head;
	mcc_type command_type;
	UINT8 fcs;
} nsc_msg;

typedef struct CmuxCallBackFuncS
{
   rx_ind_t RxIndCB;                    //  Data indication (Pointer to the data & its length).
   DlcLinkStatusIndCB LinkStatusIndCB;  //  Link status (TRUE=Up, FALSE=Down).
   tx_done_t TxDoneCnfCB;               //  Tx complete (Pointer to buffer) - can free the buffer.
} CmuxCallBackFuncS ;

typedef struct muxparameter
{
    unsigned short int mode;
    unsigned short int subset;
    unsigned int port_speed;
    unsigned short int max_frame_size;
    unsigned short int ack_time;
    unsigned char max_retx_num;
    unsigned char response_time;
    unsigned char wakeup_response_time;
    unsigned char window_size;
}muxparameter;

/*===========================================================================

            EXTERN DECLARATIONS FOR MODULE

===========================================================================*/

/*===========================================================================

                        EXTERN FUNCTION DECLARATIONS

===========================================================================*/
/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      mvUsbModemSendData                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      Send modem packet data to PC via USB.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void mvUsbModemSendData(UINT8 sATP,UINT8 *bufPtr, UINT32 length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      mux_get_trace_mode                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the mux trace mode.                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void mux_get_trace_mode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_dlc_priority                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the priority of cmux channel.        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT8 cmux_get_dlc_priority(unsigned char ch);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_dlc_MTU                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the MTU of cmux channel.             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT32 cmux_get_dlc_MTU(unsigned char ch);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_dlc_retx_num                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the max retransmit num of channel.   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT32 cmux_get_dlc_retx_num(unsigned char ch);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_init_tx                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function intializes mux tx array                             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_init_tx(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_match_index_service                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function search the DLC cfg table to find the match item.    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_match_index_service(int service_id);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_hal_init                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function intializes the Cmux Hal device.                     */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_hal_init(cmux_hal_type PhyInterface);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_next_msg_length                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get the next message length.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_get_next_msg_length(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_serial_rcv_msg                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function receive cmux hal message.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_serial_rcv_msg(char *buf, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_hal_rcv_frame                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function receive cmux hal message.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_hal_rcv_frame(char *buf, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc0_send_uih                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send UIH frame via dlc 0 channel.                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_dlc0_send_uih(UINT8 cr, UINT8 type, UINT8 *data, int len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_msc_msg                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send CMUX MSC message.                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_msc_msg( UINT8 value, UINT8 cr, UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_fcoff_msg                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control message.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_fcoff_msg(UINT8 cr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_fcon_msg                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control on message.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_fcon_msg(UINT8 cr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc_flow_control_off                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control off message.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_dlc_flow_control_off(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc_flow_control_on                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control on message.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_dlc_flow_control_on(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_process_mcc                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process CMUX Multiplexer command cr.                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_process_mcc(short_frame *data, UINT32 len, int longpkt);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      dlc0_handler                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function handler the DLC 0 frame.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void dlc0_handler(short_frame *frame, UINT16 size);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      process_uih                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process the UIH frame.                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int process_uih( short_frame *data, UINT32 len, UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_ua                                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send UA response.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_ua(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_dm                                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send DM response.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_dm(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_sabm                                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send sabm message.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_sabm(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_disc                                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Disconnect (DISC) command message.             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_disc(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_pn_msg                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Parameter Negotiation message.                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_pn_msg( UINT8 prior, UINT32 frame_size, UINT8 convergence_layer, UINT8 dlci, UINT8 cr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_nsc_msg                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Non Support Command message.                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void send_nsc_msg(mcc_type cmd, UINT8 cr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_add_to_rx_queue                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function add cmux message to RX queue.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_add_to_rx_queue(UINT8 *data, UINT16 size);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      composite_timer_callback                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is composite timer callback.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void composite_timer_callback (UINT32 source_call);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_process_frame                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process the RX CMUX frame.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_process_frame(UINT8 * data,UINT16 size);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_tx_task                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is cmux TX task function.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_tx_task(void *argv);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_rx_task                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is cmux RX task function.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_rx_task(void *argv);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_process_cmux_cmd                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process cmux command.                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_process_cmux_cmd(char *buf, int frame_length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_hal_process_rx                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process cmux hal RX message.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_hal_process_rx(int freelen);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_wait_dlc_open                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function wait cmux dlc open.                                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_wait_dlc_open(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_startup                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to start up cmux mode.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_startup(muxparameter * par);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_startup                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to start up cmux mode.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_startup_internal(muxparameter * par);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc_open                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function open CMUX dlc channel.                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_dlc_open(int service_id, CmuxCallBackFuncS *Func);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_transmit_cmd_data_to_app                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function transmit CMUX comand or data to app.                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_transmit_cmd_data_to_app(int channel);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_default_tx_complete_callback                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is default CMUX TX complete callback function.      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_default_tx_complete_callback(unsigned int dlc, void *buffer, unsigned int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_init_internal                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function intializes cmux related variables and task.         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_init_internal(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_write                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is cmux write function.                             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_write(int dlci, char *packet, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_add_tx_queue                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function add send packet to TX queue.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_add_tx_queue(int dlci, char* packet, int length, int type);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_hal_change_to_frame_mode                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function change CMUX to be frame mode.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_hal_change_to_frame_mode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_next_frame_length                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get the next frame length.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_get_next_frame_length(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_hal_rx_msg                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function process cmux hal RX message.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_hal_rx_msg(unsigned char *mesg, unsigned int mesglength);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_flow_control_on                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control on message.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_flow_control_on(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_flow_control_off                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send flow control off message.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_flow_control_off(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_channel_block                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function control cmux channel block.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_channel_block(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_channel_resume                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function control cmux channel resume.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_channel_resume(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_channel_get_status                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get cmux channel status.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_channel_get_status(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_rx_count                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get cmux RX count.                                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_get_rx_count(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_dlc_status                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get cmux dlc status.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_get_dlc_status(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CmuxDlcDataRxInd                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is Cmux Dlc data rx indication function.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      int                                 return status                */
/*                                                                       */
/*************************************************************************/
int CmuxDlcDataRxInd(unsigned int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CmuxLinkStatusIndCB                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is Cmux link state change indication function.      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void CmuxLinkStatusIndCB(unsigned int dlci,DlcLinkStatus status);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CmuxDlcTxCompleteCallback                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is Cmux Dlc Tx Complete Callback function.          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void CmuxDlcTxCompleteCallback(unsigned int dlci, void* buffer, unsigned int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_insert_Qos_queue                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function insert the node to Qos TX queue.                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_insert_Qos_queue(int dlci, cmux_data_node* node);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_frame_send_init                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function intializes the cmux frame_header and frame_tail     */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_frame_send_init(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_init_dev_type                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function intializes the cmux device type.                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_init_dev_type(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      send_control_frame                                               */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send the cmux control frame.                        */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int send_control_frame(UINT8 dlci, UINT8 frame_cr, UINT8 frametype, char *data_node, int len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_send_frame                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send cmux frame.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_send_frame(UINT8 dlci, cmux_data_node * data_node);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_select_dlc_by_prio                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function select the cmux dlc_by priority.                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_select_dlc_by_prio(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dequeue_Qos_queue                                           */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function dequeue node from tx queue.                         */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
cmux_data_node * cmux_dequeue_Qos_queue(int dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_tx_fcs_compute                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function compute the TX FCS data.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT8 cmux_tx_fcs_compute(UINT8 * payload, int len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_rx_fcs_check                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check the RX FCS.                                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT8 cmux_rx_fcs_check(char *payload, int len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc_tx_queue_unschedule                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*     The function remove dlc node from priority array to stop schedule.*/
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_dlc_tx_queue_unschedule(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_dlc_tx_queue_schedule                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*     The function add dlc node to priority array to start schedule.    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_dlc_tx_queue_schedule(UINT8 dlci);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_fcs_compute                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function compute the FCS data.                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT8 cmux_fcs_compute(const UINT8 payload[], int len);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_usb_frame_send                                              */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Cmux frame by usb device.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_usb_frame_send(cmux_data_node *frame_header, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_uart_send                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Cmux data by uart device.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_uart_send(char * buf, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_uart_frame_send                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Cmux frame by uart device.                     */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_uart_frame_send(cmux_data_node *frame_header, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_usb_send                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send Cmux data by usb device.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int cmux_usb_send(char * buf, int length);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      get_cmux_mode                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get cmux mode.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
int get_cmux_mode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_quit                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to quit cmux mode.                      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void cmux_quit(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      cmux_get_dlc_feature                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the feature of cmux channel.             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
UINT32 cmux_get_dlc_feature(unsigned char ch);

#endif
