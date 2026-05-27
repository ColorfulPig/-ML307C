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
/* Title: Arbel Resource Manager DB Header File                         */
/*                                                                      */
/* Filename: rm_db.h                                                    */
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


#ifndef _RM_DB_H_
#define _RM_DB_H_

#include "rm_def.h"

// Resources bit mask definitions (according to their order in the DB)
#define NO_ALLOCATED_RESOURCES      (0x0)
#define RSRC_DSSP3                  (0x1 << 0)
#define RSRC_SCK_FUNC               (0x1 << 1)
#define RSRC_TCU                    (0x1 << 2)
#define RSRC_MODEM_104MHZ           (0x1 << 3)
#define RSRC_MODEM_78MHZ            (0x1 << 4)
#define RSRC_I2C                    (0x1 << 5)
#define RSRC_GSSP2                  (0x1 << 6)
#define RSRC_MSL                    (0x1 << 7)
#define RSRC_USIM                   (0x1 << 8)
#define RSRC_DMA                    (0x1 << 9)
#define RSRC_DDR_HIGH_FREQ          (0x1 << 10)  //RSRC_IPC rafiagiv - 21/12
#define RSRC_DSSP_BUS               (0x1 << 11)
#define RSRC_VCTCXO                 (0x1 << 12)
#define RSRC_WB_SLEEP_MODULE        (0x1 << 13)
#define RSRC_TIMER0_13M             (0x1 << 14)
#define RSRC_TIMER1_13M             (0x1 << 15)
#define RSRC_TIMER2_13M_GB          (0x1 << 16)
#define RSRC_TIMER3_13M_GB          (0x1 << 17)
#define RSRC_WB_CIPHER_GB           (0x1 << 18)
#define RSRC_DSSP0_GB               (0x1 << 19)
#define RSRC_DSSP1_GB               (0x1 << 20)
#define RSRC_DSSP2_GB               (0x1 << 21)
#define RSRC_GSSP1_GB               (0x1 << 22)
#define	RSRC_CPA_DDR_HPerf		    (0x1 << 23)  
#define	RSRC_CGPIO			        (0x1 << 24)
#define	RSRC_FREE_3				    (0x1 << 25)  //RSRC_VITERBI_GB rafiagiv - 28/12
#define RSRC_FREE_4				    (0x1 << 26)  //RSRC_EQUALIZER_GB rafiagiv - 28/12
#define RSRC_TTPCOMM_GB             (0x1 << 27)
#define RSRC_DVFM                 	(0x1 << 28)
#define RSRC_MODEM_312MHZ_GB        (0x1 << 29)
#define RSRC_MODEM_208MHZ_GB        (0x1 << 30)

//notice that we cannot add any more peripherals here.
#define RSRC_UART        			(0x1u << 31)
#define RM_NUMBER_OF_RSRCS              32
#endif  /* _RM_DB_H_ */
