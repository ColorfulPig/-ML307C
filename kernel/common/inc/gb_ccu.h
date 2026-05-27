/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************/
/*  COPYRIGHT (C) 2002 Intel Corporation.                               */
/*                                                                      */
/*  This file and the software in it is furnished under                 */
/*  license and may only be used or copied in accordance with the terms */
/*  of the license. The information in this file is furnished for       */
/*  informational use only, is subject to change without notice, and    */
/*  should not be construed as a commitment by Intel Corporation.       */
/*  Intel Corporation assumes no responsibility or liability for any    */
/*  errors or inaccuracies that may appear in this document or any      */
/*  software that may be provided in association with this document.    */
/*  Except as permitted by such license, no part of this document may   */
/*  be reproduced, stored in a retrieval system, or transmitted in any  */
/*  form or by any means without the express written consent of Intel   */
/*  Corporation.                                                        */
/*                                                                      */
/* Title: Grayback Clock Control Unit Header File                       */
/*                                                                      */
/* Filename: gb_ccu.h                                                   */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 4/11/2004                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/
#ifndef _GB_CCU_H_
#define _GB_CCU_H_

/*-- Type definitions (T type) -----------------------------------------*/
/*----------------------------------------------------------------------*/
/*-- Constants definitions (C type) ------------------------------------*/
/*----------------------------------------------------------------------*/
/*-- Global variables declarations (Public G type) ---------------------*/
/*----------------------------------------------------------------------*/
/*-- -----Public Functions Prototypes ----------------------------------*/
void CCUDefaultConfigSet(void);
void CCUTurnClocksOff(void);
void CCUSysPLL312MHzSet(void);
void CCUTurnModemClocksOn(void);
void CCUTurnModemClocksOff(void);
void CCUTurnModemClocksOn(void);
void CCUTurnModemClocksOff(void);
void CCUTurnUSIMClocksOn(void);
void CCUTurnUSIMClocksOff(void);
void CCUTurnMSLClocksOn(void);
void CCUTurnMSLClocksOff(void);
void CCUTurnAudioSSPClocksOn(void);
void CCUTurnAudioSSPClocksOff(void);
void CCUTurnSSPClocksOn(void);
void CCUTurnSSPClocksOff(void);
void CCUTurnI2CClocksOn(void);
void CCUTurnI2CClocksOff(void);

/*----------------------------------------------------------------------*/
/*-------Test Function--------------------------------------------------*/
/*----------------------------------------------------------------------*/

 #endif  /* _GB_CCU_H_ */


