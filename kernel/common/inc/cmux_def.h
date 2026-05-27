#ifndef _CMUX_DEF_H_
#define _CMUX_DEF_H_
/*====*====*====*====*====*====*====*====*====*====*====*====*====*====*====*

                cmux_def.h


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

#include "utilities.h"

/*===========================================================================

                                LOCAL MACRO
===========================================================================*/

/*the service identifier definition*/
#define Channel_Control      (0)
#define DLCAT_1				 (1)
#define DLCAT_2				 (2)
#define DLCAT_3              (3)
#define DLCAT_4              (4)
#define DLCAT_5				 (5)
#define DLCAT_IND			 (6)
#define DLCAT_7				 (7)
#define DLCAT_8				 (8)

#define DLCATDATA_1              (9)
#define DLCATDATA_2              (10)
#define DLCATDATA_3              (11)
#define DLCATDATA_4              (12)
#define DLCATDATA_5              (13)
#define DLCATDATA_6              (14)
#define DLCATDATA_7              (15)
#define DLCATDATA_8              (16)
#define DLCATDATA_9              (17)
#define DLCATDATA_10              (18)
#define DLCATDATA_11			 (19)
#define DLCATDATA_12			 (20)
#define RELIABL_DATA		 (21)

#define Audio_Data 			 (33)	/*Marvell Audio control and data*/
#define NVM_Access  		 (34)	/*NVM access control*/
#define DIAG_DATA 				 (36)
#define TEST_DATA				 (37)
#define VTCSD_DATA				 (38)


#define RESERVED 				 (0xffff)

/*DLC channel feature*/
#define DLC_CONTROL_CHANNEL     (0)
#define DLC_AT_CHANNEL			(1)
#define DLC_AT_DATA_CHANNEL	    (2)
#define RAW_DATA_CHANNEL		(3)

#define customer_count (2)
#define default_mtu (1509)
#define max_retransmit_num (3)
#define datachannelmtu (2048)
#define AT_MODE 0      /*0:at mode;1:data mode*/
#define DATA_MODE 1       /*0:at mode;1:data mode*/
#define OTHER_MODE 3
/*service identifier define end*/
#define MUXDATAALIGN 				    (4)


/*state machine*/
#define CONNECTING 		                (0)
#define CONNECTED 		                (1)
#define FLOW_STOPPED 	                (2)
#define DISCONNECTING 	                (3)
#define DISCONNECTED 	                (4)

#define DLCOpenMaxWaitTime              (OSA_SUSPEND)      /*tick, or OSA_NO_SUSPEND,OSA_SUSPEND*/
#define DLCOpenRetrytime		        (3)

#define MUX_TX_ID                       0x44495854
#define MUX_DISC_ID                     0x43534944

#define frameheadlen			        (5)
#define frametaillen			        (2)
#define minbufsize                      16

#define MUX_HAL_MTU                     (2048)
#define MUX_HAL_FLOW_OFF_THRESHOLD      (1024)
#define MUX_HAL_FLOW_ON_THRESHOLD       (2048)
#define HALTXDONE                       (0x01)
#define HISR_PRIORITY                   HISR_PRIORITY_2

#define SHORT_CRC_CHECK                 3
#define LONG_CRC_CHECK                  4
#define SHORT_PAYLOAD_SIZE              127

#define EA                              1
#define FCS_SIZE                        1
#define FLAG_SIZE                       2

#define TS0710_MAX_HDR_SIZE             5
#define DEF_TS0710_MTU                  256

#define TS0710_BASIC_FLAG               0xF9

/* the control field */
#define SABM                            0x2f
#define SABM_SIZE                       4
#define UA                              0x63
#define UA_SIZE                         4
#define DM                              0x0f
#define DISC                            0x43
#define UIH                             0xef

/* the type field in a multiplexer command packet */
#define TEST                            0x8
#define FCON                            0x28
#define FCOFF                           0x18
#define MSC                             0x38
#define RPN                             0x24
#define RLS                             0x14
#define PN                              0x20
#define CLD								0x30
#define NSC                             0x4

/* V.24 modem control signals */
#define FC                              0x2
#define RTC                             0x4
#define RTR                             0x8
#define IC                              0x40
#define DV                              0x80

/* The control channel is defined as DLCI 0 */
#define CTRL_CHAN                       0

/* Multiplexer command cr */
#define MCC_CMD                         1

/* Multiplexer response cr */
#define MCC_RSP                         0
#define MCC_READY_REC                   0x0d

/*Error kind define end*/
#define parametererr                    (1)
#define timeoutererr                    (2)
#define dlcclosederr                    (3)
#define muxclosederr                    (4)
#define DlcTxQueueOverflow              (5)
#define DlcTxFlowControlOn              (6)
#define MuxTxFlowControlOn              (7)
#define Nomemory                        (8)

#define MUX_EA			                1
#define MUX_BASIC_FLAG_SEQ	            0xf9
#define MUX_ADVANCED_FLAG_SEQ	        0x7e
#define MUX_CONTROL_ESCAPE	            0x7d

#define NOMUXFRAME	                    (0xdd)
#define	MUX_SABM	                    (0x2f)
#define	MUX_UA		                    (0x63)
#define	MUX_DM		                    (0x0f)
#define	MUX_DISC	                    (0x43)
#define	MUX_UIH		                    (0xef)
#define	MUX_UI		                    (0x03)
#define MUX_PN		                    (0x20)

#define MUXUSBTxBufSize                 (4096)

#ifdef CMUX_DLCI_EXT
#define NUM_DLCI		                64
#else
#define NUM_DLCI		                5
#endif

#define CMUX_RX_TASK_PRIO               (120)
#define CMUX_RX_TASK_STACK_SIZE         (1024)

#define CMUX_TX_TASK_PRIO               (80)
#define CMUX_TX_TASK_STACK_SIZE         (1024)

/* Cmux RX maxmium queue count  */
#define  CMUX_RX_MSG_Q_MAX	            512

/* Cmux RX request queue size */
#define  CMUX_RX_MSG_Q_SIZE	            4

/* Cmux TX maxmium queue count  */
#define  CMUX_TX_MSG_Q_MAX	            1024

/* Cmux TX request queue size */
#define  CMUX_TX_MSG_Q_SIZE	            4

/*state machine end*/
#define min(a,b)    (((a) < (b)) ? (a) : (b))

/* Mux trace enable flag. */
#define MUX_LOG_ENABLE                  MuxTraceEnable

/* Mux trace enable flag. */
#define MUX_TRACE(fmt,args...)                                           \
{                                                                        \
    if (MUX_LOG_ENABLE)                                                  \
    {                                                                    \
        CPUartLogPrintf(fmt, ##args);                                  \
    }                                                                    \
}

#define SET_PF(ctr)                     ((ctr) | (1 << 4))
#define CLR_PF(ctr)                     ((ctr) & 0xef)
#define GET_PF(ctr)                     (((ctr) >> 4) & 0x1)

#define GET_PN_MSG_FRAME_SIZE(pn)       ( ((pn)->frame_sizeh << 8) | ((pn)->frame_sizel))
#define SET_PN_MSG_FRAME_SIZE(pn, size) ({ (pn)->frame_sizel = (size) & 0xff; \
                                           (pn)->frame_sizeh = (size) >> 8; })

#define GET_LONG_LENGTH(a)              ( ((a).h_len << 7) | ((a).l_len) )
#define SET_LONG_LENGTH(a, length)      ({ (a).ea = 0; \
                                           (a).l_len = length & 0x7F; \
                                           (a).h_len = (length >> 7) & 0xFF; })

/*===========================================================================

            EXTERN DECLARATIONS FOR MODULE

===========================================================================*/

extern BOOL MuxTraceEnable;
#endif
