/*************************************************************************/
/*                                                                       */
/*               Copyright Mentor Graphics Corporation 2012              */
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
/*      dm_extr.h                                      Nucleus PLUS 1.15 */
/*                                                                       */
/* COMPONENT                                                             */
/*                                                                       */
/*      UM - USB Ring Memory Management                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This file contains function prototypes of all functions          */
/*      accessible to other components.                                  */
/*                                                                       */
/* DATA STRUCTURES                                                       */
/*                                                                       */
/*      None                                                             */
/*                                                                       */
/* DEPENDENCIES                                                          */
/*                                                                       */
/*      um_defs.h                          USB Ring Management constants */
/*                                                                       */
/*************************************************************************/
#ifdef PLAT_USE_THREADX
#include        "osa_um_defs.h"        /* Include DM constants      */
#else
#include        "um_defs.h"        /* Include DM constants      */
#endif

/* Check to see if the file has been included already.  */

#ifndef UM_EXTR
#define UM_EXTR

#define			OSA_Data_Buffer_Init				UMC_Data_Buffer_Init     //add by zhangwl for UMC -start
#define			OSA_Data_UP_Buffer_Init				UMUPC_Data_Buffer_Init     //add by zhangwl for UMC -start
#define			OSA_Data_Buffer_Get					UMC_Data_Buffer_Get
#define			OSA_Data_UP_Buffer_Get				UMUPC_Data_Buffer_Get
#define			OSA_Data_Buffer_Put					UMC_Data_Buffer_Put
#define			OSA_Data_UP_Buffer_Put				UMUPC_Data_Buffer_Put
#define			OSA_Data_Buffer_Get_Block			UMC_Data_Buffer_Get_Block
#define			OSA_Data_Buffer_Put_Block			UMC_Data_Buffer_Put_Block
#define			OSA_Data_Buffer_Put_Block_nezha2    UMC_Data_Buffer_Put_Block_nezha2

#define			OSA_Data_UP_Buffer_Get_Block		UMUPC_Data_Buffer_Get_Block
#define			OSA_Data_UP_Buffer_Put_Block		UMUPC_Data_Buffer_Put_Block
#define	        OSA_Data_UP_Buffer_Block_By_APP  	UMUPC_Data_Buffer_Block_By_App
#define	        OSA_Data_UP_Buffer_UnBlock_By_APP  	UMUPC_Data_Buffer_Unblock_By_App
#define         OSA_Data_Buffer_Get_Block_Nezha2    UMC_Data_Buffer_Get_Block_Nezha2

#define			OSA_Data_Buffer_Is_Full				UMC_Data_Buffer_Is_Full //add by dong for UMC 

#define			OSA_Data_Buffer_Get_Size			UMC_Data_Buffer_Get_Size //add by zhangwl for UMC -end

#define	 		OSA_Downlink_buffer_Is_Reach_Hwm    Umc_Is_Reach_Hwm//add for L2 to check HWM of downlink buffer 

/*  Initialization functions.  */
STATUS UMC_Data_Buffer_Init(UM_PCB *buf, DATA_ELEMENT *hdr, DATA_ELEMENT *ptr, 
    					  UINT16 block_size, UINT16 block_num);

/* Core processing functions.  */
DATA_ELEMENT* UMC_Data_Buffer_Get(UM_PCB *buf, UINT16 size, OPTION newblock);
VOID UMC_Data_Buffer_Put(UM_PCB *buf, DATA_ELEMENT *ptr);

UM_BLOCK* UMC_Data_Buffer_Get_Block(UM_PCB *buf);
VOID UMC_Data_Buffer_Put_Block(UM_PCB *buf, UM_BLOCK *block);

/* Supplemental service routines */

/* Information retrieval functions.  */
UNSIGNED UMC_Data_Buffer_Get_Size(UM_PCB *buf);

STATUS UMUPC_Data_Buffer_Init(UMUP_PCB *buf, DATA_ELEMENT *hdr, DATA_ELEMENT *ptr, 
					UINT16 block_size, UINT16 block_num);

UMUP_BLOCK* UMUPC_Data_Buffer_Get_Block(UMUP_PCB *buf);

STATUS UMUPC_Data_Buffer_Put_Block(UMUP_PCB *buf, UMUP_BLOCK *block, UINT16 size);
DATA_ELEMENT *UMUPC_Data_Buffer_Get(UMUP_PCB *buf, UINT16* size);

VOID UMUPC_Data_Buffer_Put(UMUP_PCB *buf, DATA_ELEMENT *ptr);

BOOL Umc_Is_Reach_Hwm(UM_PCB *buf);


#endif





