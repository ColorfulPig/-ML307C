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

#ifndef __MAT_TOKEN_H__
#define __MAT_TOKEN_H__

typedef enum {
    MAT_TOKEN_UNKNOWN,
    MAT_TOKEN_PREFIX_ID,
    MAT_TOKEN_INTEGER,      /* integer data */
    MAT_TOKEN_STR,          /* string data  */
    MAT_TOKEN_DEFAULT,      /* default data */
	MAT_TOKEN_SET,			/* set data     */
    MAT_TOKEN_EOF,          /* end of token */
	MAT_TOKEN_TERMINAL,		/* token terminal */
}MAT_TOKEN_TYPE;

typedef struct
{
    int position;
	int first_pos;
    int token;              /* current token */

    const char* line;
    int line_size;

    /* the value */
    char* str_value;
    int int_value;
}MATToken;

int MATTokenInit(MATToken *token, const char* line, int size);
int MATTokenDeinit(MATToken *token);
int MATTokenReset(MATToken *token);

int MATNextToken(MATToken *token);

int MATTokenInteger(MATToken *token);
int MATNextToken_CMT(MATToken *token);

const char* MATTokenString(MATToken *token);

#endif

