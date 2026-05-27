/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
**
** INTEL CONFIDENTIAL
** Copyright 2006 Intel Corporation All Rights Reserved. 
** The source code contained or described herein and all documents related to the source code ("Material") are owned 
** by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or 
** its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of 
** Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and 
** treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted, 
** transmitted, distributed, or disclosed in any way without Intel's prior express written permission.
** 
** No license under any patent, copyright, trade secret or other intellectual property right is granted to or 
** conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement, 
** estoppel or otherwise. Any license under such intellectual property rights must be express and approved by 
** Intel in writing.
** 
** Unless otherwise agreed by Intel in writing, you may not remove or alter this notice or any other notice embedded
** in Materials by Intel or Intel's suppliers or licensors in any way.
**
**  FILENAME:       bsp_util.h
**
**  Authors: 				Abraham Coifman	
**
**  PURPOSE:
**
******************************************************************************/

#include "global_types.h"
#include "mmap_phy.h"

#define NAND_BLOCK_SIZE 0x4000

#define DDR_LAST_ADDRESS_BOERNE	    0xBFFFFFFF  /* The address of the last byte of the DDR when accessed from Boerne */
#define DDR_LAST_ADDRESS_COMM       0xD0FFFFFF  /* The address of the last byte of the DDR when accessed from COMM */
#define BOERNE_ADDR(x)              (DDR_LAST_ADDRESS_BOERNE - (DDR_LAST_ADDRESS_COMM - x))  /* Macro translates a COMM address to its BOERNE value */

#define COMM_BLOCK_ARBEL_START		(3457)
#define COMM_BLOCK_ARBEL_LENGTH		(448)
#define COMM_BLOCK_ARBEL_END		(COMM_BLOCK_ARBEL_START + COMM_BLOCK_ARBEL_LENGTH - 1) //3904
	
/* MSA Image */
#define COMM_BLOCK_GB_START			(COMM_BLOCK_ARBEL_END+1)  //3905
#define COMM_BLOCK_GB_LENGTH		(64)
#define COMM_BLOCK_GB_END			(COMM_BLOCK_GB_START + COMM_BLOCK_GB_LENGTH - 1) //3968
	
#define RESET_ROUTINE_GB_ADDR		BOERNE_ADDR(0xD0FFFFC0) /* The address in which GB starts after COMM is released from reset */
#define RESET_ROUTINE_ARBEL_ADDR	BOERNE_ADDR(0xD0FFFFE0) /* The address in which Arbel  starts after COMM is released from reset */

#define RESET_ROUTINE_GB        { 0xE108, 0x8000 , 0xE148, 0xD0F8, 0x0050 }     /*  PO.L 0x8000; PO.H 0xD0F8 ; PC=PO, GB will jump to 0xd0f88000 */
#define RESET_ROUTINE_ARBEL     {0xE51FF004, ARBEL_BASE_ADDRESS, 0xA1A2A3A4, 0xA1A2A3A4}  /*  remember: LITTLE endian! 0xE51FF004 = MOV to PC the long next to me */

#define COMM_IMAGE_GB_NOR_START		0x13F00000 //0x10800000
#define COMM_IMAGE_GB_ADDR_DST 		BOERNE_ADDR(0xD0F00000)	// we will page GB image into DDR.
//Image word size
#define COMM_IMAGE_GB_SIZE_WORD		((COMM_BLOCK_GB_LENGTH*NAND_BLOCK_SIZE) / 4)

 //Image word size
#define COMM_IMAGE_ARBEL_NOR_START	 0x13800000 //0x10100000
#define COMM_IMAGE_ARBEL_SIZE_WORD	((COMM_BLOCK_ARBEL_LENGTH*NAND_BLOCK_SIZE) / 4 )
#define COMM_IMAGE_ARBEL_ADDR_DST	ARBEL_BASE_ADDRESS	// we will page ARBEL image into DDR.

typedef struct   arbelResetRoutineS
{
	unsigned long   Arbel_ResetRoutine[8];    /* LONGs - 32 bit */
} ARBEL_ResetRoutineS ;

typedef struct   GbResetRoutineS
{
	unsigned short	Gb_ResetRoutine[16];      /* Shorts */
} GB_ResetRoutineS ;

typedef enum ResetActionE_TAG
{
    RESET_ACT_LEVEL_INACTIVE = 0,       // could run
    RESET_ACT_LEVEL_ACTIVE = 1,         // in reset
    RESET_ACT_PULSE_INACT_ACT = 0xFF    // inactive  __active__ inactive
}ResetActionE;
/*
typedef enum ResetSiteE_TAG
{
    RESET_SITE_LEVEL_INACTIVE = 0,       // could run
    RESET_SITE_LEVEL_ACTIVE = 1,         // in reset
    RESET_SITE_PULSE_INACT_ACT = 0xFF    // inactive  __active__ inactive
}ResetSiteE;
*/
typedef enum bspUtil_ReturnCodeE_TAG
{
  	BSP_UTIL_RC_OK = 0,
  	BSP_UTIL_RC_NO_ACTION = -100
  	
} bspUtil_ReturnCodeE;


void tavorOBMalike(void);
bspUtil_ReturnCodeE mateSideReset(ResetActionE resetAction);
void copyCommSideImage (void);
void commSideResetExt(void *p);

void mateSideResetTest(void *);
void copyCommSideImageTest (void *);
