/************************************************************************************
* 		CopyRight (C), Marvell Tech., LTD.( 2012 )
*************************************************************************************
* File Name: ECM.h
*
* Descripition:
*	Based on USB CDC ECM protel.
*
* Function List:
*
* Author    : Joshua Wang.
* Version   : 1.00.
* Data      : 06/19/2012.
*
* Histroy:
*
* 1. Data: @  06/19/2012
*     Author: Joshua Wang. 1.0.0
*     Modification: first drafted.
*
*************************************************************************************/
#ifndef _ECM_H_
#define _ECM_H_


/*===========================================================================

                     INCLUDE FILES FOR MODULE

===========================================================================*/

#include "osa.h"
#include "global_types.h"
#include "utils.h"
#include "usb_macro.h"
#include "usb_device.h"

/*--- Macros ----------------------------------------------------------------*/

/* ECM Transmit message */
#define ECM_TRANSMIT_MSG		                    0x4D434554


/****************************************************************************************
*                           Extern APIs
*****************************************************************************************/
/*******************************************************************************
* Function  : EcmBitrate
*
* Descritption:
*   Report the bitrate of ECM.
*
* Parameter :
*   Inputs  : N/A.
*   Ouputs  : N/A.
*
* Returns   :
*   N/A.
*
* Notes:
*   N/A.
******************************************************************************/
UINT32 EcmBitrate(void);

/*******************************************************************************
* Function    : ecm_free
*
* Description :
*   Free the buffer.
*
* Parameter   :
*   Inputs    :
*       buf => The address of buffer.
*   Outputs   :
*       N/A.
* Returns     :
*       N/A.
*
* Notes:
*       N/A.
******************************************************************************/
void ecm_free(void *buf);

/*******************************************************************************
* Function    : EcmParamInit
*
* Description :
*   Inilization of EcmParam struct.
*
* Parameter   :
*   Inputs    :
*       N/A.
*   Outputs   :
*       N/A.
* Returns     :
*       N/A.
*
* Notes:
*       N/A.
******************************************************************************/
void EcmParamInit(void);

/***********************************************************************
* Function: ECM_remove_hdr
*
* Description:
*   There is no header in ECM.
*
* Parameters:
*   Inputs:
*       date : pointer to message
*   Outputs:
*	    lenth: number of bytes in message
*
* Return value:
*   Always it is 0.
*
* Notes:
*   N/A.
***********************************************************************/
BOOL Ecm_remove_hdr(UINT8 **data, UINT32 *msgLen, UINT32 *dataLen);

/***********************************************************************
* Function: ECM_add_hdr
*
* Description:
*   There is no header in ECM.
*
* Parameters:
*   Inputs:
*       date : pointer to message
*	    lenth: number of bytes in message
*
* Return value:
*   N/A.
*
* Notes:
*   N/A.
***********************************************************************/
void Ecm_add_hdr (UINT8 *data, UINT32 length);

/*******************************************************************************
* Function    : Ecm_send_packet
*
* Description :
*    Perform the specific interface logic to tx a trace
*  on the external interface USB without not using DMA
*
* Parameter   :
*   Inputs    :
*       N/A.
*   Outputs   :
*       N/A.
* Returns     :
*       N/A.
*
* Notes:
*       N/A.
******************************************************************************/
void Ecm_send_packet(UINT8* data, UINT32 len);

/*******************************************************************************
* Function    : Ecm_getstrECMMACADDR
*
* Description :
*   Generate the string mac address used for usb protocal
*
* Parameter   :
*   Inputs    :
*       N/A.
*   Outputs   :
*       N/A.
* Returns     :
*       N/A.
*
* Notes:
*       N/A.
******************************************************************************/
void Ecm_getstrECMMACADDR(UINT8 * strMac, UINT32 len);


/*******************************************************************************
* Function    : Ecm_NetworkIndication
*
* Description :
*   ECM Network indication message
*
* Parameter   :
*   Inputs    :
*       N/A.
*   Outputs   :
*       N/A.
* Returns     :
*       N/A.
*
* Notes:
*       N/A.
******************************************************************************/
void Ecm_NetworkIndication(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      Ecm_indicate_status_msg                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send message to PC to indicate the current status.  */
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
void Ecm_indicate_status_msg(UINT8 *buf);

/*****************************************************************************************
* 						End of File
******************************************************************************************/
#endif
