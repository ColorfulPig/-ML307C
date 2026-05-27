/*----------------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------------------*/
/*                                                                      */
/*                                                                      */
/*  This file and the software in it is furnished under                 */
/*  license and may only be used or copied in accordance with the terms */
/*  of the license. The information in this file is furnished for       */
/*  informational use only, is subject to change without notice, and    */
/*  should not be construed as a commitment by Marvell Corporation.     */
/*  Marvell Corporation assumes no responsibility or liability for any  */
/*  errors or inaccuracies that may appear in this document or any      */
/*  software that may be provided in association with this document.    */
/*  Except as permitted by such license, no part of this document may   */
/*  be reproduced, stored in a retrieval system, or transmitted in any  */
/*  form or by any means without the express written consent of Marvell */
/*  Corporation.                                                        */
/*                                                                      */
/* Title: MAT module                                                    */
/*                                                                      */
/* Filename: mat_token.h                                                */
/*                                                                      */
/* Author:   Puxiang Xiong                                              */
/*                                                                      */
/* Project, Target, subsystem: Seagull Telephony                        */
/*                                                                      */
/* Remarks: -                                                           */
/*                                                                      */
/* Created: 10/15/2013                                                  */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/

/************************************************************************/

#ifndef __MAT_RESPONSE_H__
#define __MAT_RESPONSE_H__

#include "teldef.h"

#define LINE_SZ				(4096+512)

#define MAT_CHANNEL_MAX 14

typedef struct
{
	const char* response;
	int type;
}MATResponseType;

typedef struct
{
	char chn_buf[LINE_SZ * 2];
	int  chn_offset;
}MATChannelBuffer;

typedef enum
{
    MAT_RSP_PARSER_OK,
	MAT_RSP_PARSER_WAIT_FOR_TERMINAL,
    MAT_RSP_PARSER_ERROR,
    MAT_RSP_PARSER_NUM
}MATRspParseRet;


/*
 * This callback function is invoked when ATCmdSrv has response or indication.
 *
 * TelAtParserID	chnlId            -- [IN] the channle index;
 * const char 		*resp            -- [IN] the AT command response or indication;
 * int			size               -- [IN]  the length of AT response or indication;
 *
 */
typedef int (*MATConfIndCB)(TelAtParserID chnlId, const char* response, int size);



 /*
 * This function is used to register the call back function for specificed channel, which will be invoked when ATCmdSrv has response or indication.
 *
 * TelAtParserID	chnlId            -- [IN] the channle index;
 * MATConfIndCB 	cb           -- [IN] callabck interface
 *
 */
void MATSetConfIndCB(TelAtParserID channelId, MATConfIndCB cb);



/***********************************************************************
 *
 * Name:        MATHandleIndication
 *
 * Description:   used to parse a AT indicaiton of string type  to a structure: MATReturnPara
 *
 * Parameters:
 *  char                 		*response         -- [IN]   Pointer to the AT indication
 *  int                  		size       	 	   --	 [IN]    length of at indication
 * MATReturnPara		*ind			   -- [OUT] parse output result of AT indcation
 * Returns:
 *  MATRspParseRet
 *
 * Notes:
 *
 ***********************************************************************/
MATRspParseRet MATHandleIndication(const char* response, int size, MATReturnPara* ind);



/***********************************************************************
 *
 * Name:        MATHandleResponse
 *
 * Description:   used to parse a AT response of string type to a structure: MATReturnPara
 *
 * Parameters:
 *  TelAtParserID		chnlId            -- [IN] the channle index;
 *  char                 		*response_str         -- [IN]   Pointer to the AT response
 *  int                  		size       	 	   --	 [IN]    length of at indication
 * MATReturnPara		*resp			   -- [OUT] parse output result of AT response
 * Returns:
 *  MATRspParseRet
 *
 * Notes:
 *
 ***********************************************************************/
MATRspParseRet  MATHandleResponse(TelAtParserID chnlId, const char* response_str, int size,MATReturnPara *resp);



 /***********************************************************************
 *
 * Name:        MATConfIndFree
 *
 * Description:   used to free the memory used to parse AT indication/response
 *
 * Parameters:
 * MATReturnPara		*resp			   -- [IN] pointer to the structure to be freed
 * Returns:
 *  None
 *
 * Notes:
 *
 ***********************************************************************/
void MATConfIndFree(MATReturnPara* resp);

int MATConfIndCB_Dump(MATSimId sim_id, MATReturnPara *resp, MATRetType type);

void MMIConfInd_callback(TelAtParserID sAtpIndex, const char* response, int size);
void IMSConfInd_callback(TelAtParserID sAtpIndex, const char* response, int size, BOOL isInd);
void InterConfInd_callback(TelAtParserID sAtpIndex, const char* response, int size);

#endif

