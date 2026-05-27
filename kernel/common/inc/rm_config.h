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
/* Title: Arbel Resource Manager Configurations Header File             */
/*                                                                      */
/* Filename: rm_config.h                                                */
/*                                                                      */
/* Author:   Raviv Levi                                                 */
/*                                                                      */
/* Project, Target, subsystem: Tavor, Arbel, HOP     					*/
/*																		*/
/* Remarks: -                                                           */
/*    													                */
/* Created: 11/4/2005                                                   */
/*                                                                      */
/* Modified:                                                            */
/************************************************************************/


#ifndef _RM_CONFIG_H_
#define _RM_CONFIG_H_

// The cyclic log size
#define RM_LOG_SIZE                 32

// Maximum multi-user-clocks per service
#define RM_MAX_MUC_PER_SRVC          2   // IMPORTANT: This is not easy to re-configure as it requires additions to the LUT.

// ICAT display related configurable constants
#define ADJACENT_BITS                4 // where to put the space... (if changed, change BINARY_STRING_BUFF_SIZE as well)
#define BINARY_STRING_BUFF_SIZE     (RM_NUMBER_OF_RSRCS + (RM_NUMBER_OF_RSRCS/ADJACENT_BITS) + 1) //1chars (resources) + 6spaces + 1\n.

#endif  /* _RM_CONFIG_H_ */
