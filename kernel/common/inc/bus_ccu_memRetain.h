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
/* Title: Comm. sub-system Clock Control Unit Header File               */
/*                                                                      */
/* Filename: bus_ccu_mem_retain.h                                       */
/*                                                                      */
/* Author:   Chen Anati                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor PV2, Seagull, HOP     				*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: Septembet 2009                                              */
/*                                                                      */
/*                                                                      */
/************************************************************************/


#ifndef _BUS_CCU_MEM_RET_H_
#define _BUS_CCU_MEM_RET_H_


/*-- -----Public Functions Prototypes ----------------------------------*/
void   BusCCUShadowVoteSet(BusCCU_BusTypeE bus_type, BusCCU_ClockStateE OnOfffBUS);
UINT32 BusCCUShadowVoteGet(void);



#endif /*_BUS_CCU_MEM_RET_H_*/
