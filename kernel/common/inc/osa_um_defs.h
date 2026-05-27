/*************************************************************************/
/*                                                                       */
/*               Copyright Mentor Graphics Corporation 2012             */
/*                         All Rights Reserved.                          */
/*                                                                       */
/* THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS  */
/* THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS   */
/* SUBJECT TO LICENSE TERMS.                                             */
/*                                                                       */
/*************************************************************************/

/*************************************************************************/
/*                                                                       */
/* FILE NAME                                               VERSION       */
/*                                                                       */
/*      um_defs.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      UM - USB Ring Memory Management                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains data structure definitions and constants for  */
/*      the USB Ring Memory component.                                    */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      DM_PCB                              Dynamic Pool control block   */
/*      DM_HEADER                           Header of each memory block  */
/*      DM_SUSPEND                          Memory suspension block      */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      cs_defs.h                           Common service definitions   */
/*      tc_defs.h                           Thread Control definitions   */
/*                                                                       */
/*************************************************************************/
#if (!defined PLAT_USE_THREADX) && (!defined PLAT_USE_ALIOS)
#include        "cs_defs.h"        /* Common service constants  */
#include        "tc_defs.h"        /* Thread control constants  */
#endif

/* Check to see if the file has been included already.  */
#ifndef UM_DEFS
#define UM_DEFS

#define UM_EVT_GET_BUF	0x00
#define UM_EVT_PUT_BUF	0x01

/* Define the Dynamic Pool Control Block data type.  */
typedef struct UM_PCB_STRUCT
{
    volatile struct UM_BLOCK_HDR_STRUCT* um_hdr;
    UINT16 um_block_size;
    UINT16 um_block_num;

    UINT16 um_waiting_lwm;
    UINT16 um_lwm;

    DATA_ELEMENT *um_data_ptr;

    VOID (*um_tx_notify)(struct UM_PCB_STRUCT* um_buffer);
    VOID (*um_resume_notify)(struct UM_PCB_STRUCT* um_buffer);
	VOID (*um_event_handler)(struct UM_PCB_STRUCT* um_buffer, int evt);
} UM_PCB;

/* Define the header structure that is in front of each memory block.  */
typedef struct UM_BLOCK_HDR_STRUCT
{
    volatile UINT16 um_put_index;
    volatile UINT16 um_get_index;

    DATA_ELEMENT um_padding[32 - 4];
}UM_BLOCK_HDR;

/* Define the block structure that describes each memory block.  */
typedef struct UM_BLOCK_STRUCT
{
    volatile UINT16 um_length;
    volatile UINT16 um_ref;
}UM_BLOCK;

/* Define the header structure that is in front of each memory block.  */
typedef struct UMUP_BLOCK_HDR_STRUCT
{
    volatile UINT16 um_block_put_index;
    volatile UINT16 um_block_get_index;
	volatile UINT16 um_buffer_put_index;
	volatile UINT16 um_buffer_get_index;
    DATA_ELEMENT um_padding[32 - 8];
}UMUP_BLOCK_HDR;
/* Define the block structure that describes each memory block.  */
typedef struct UMUP_BLOCK_STRUCT
{
    UINT16 um_length;
    UINT16 um_ref;
	UINT16 umup_ref;

    DATA_ELEMENT um_padding[32 - 6];
}UMUP_BLOCK;

/* Define the Dynamic Pool Control Block data type for UP direction.  */
typedef struct UMUP_PCB_STRUCT
{
    volatile UMUP_BLOCK_HDR* um_hdr;
    UINT16 um_block_size;
    UINT16 um_block_num;
	

    UINT16 um_waiting_lwm;
    UINT16 um_lwm;

	UINT16 um_packet_headersize;
	UINT16 um_block_by_application;
	
    DATA_ELEMENT *um_data_ptr;

    VOID (*um_rx_notify)(struct UMUP_PCB_STRUCT* um_buffer);
    VOID (*um_resume_notify)(struct UMUP_PCB_STRUCT* um_buffer);
} UMUP_PCB;


#endif 

