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
/* Title: Arbel Resource Manager Definitions Header File                */
/*                                                                      */
/* Filename: rm_def.h                                                   */
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


#if !defined _RM_DEF_H_
    #define _RM_DEF_H_

    #include "global_types.h"
    #include "commccu.h"
    #include "mccu.h"
    #include "rm_lut.h"
    #include "rm_db.h"

    #include "rm_internal.h"

    //#if ((defined PM_DEBUG_MODE_ENABLED) && (!defined RM_DEBUG_MODE_ENABLED))
	#if 0
//        #define RM_DEBUG_MODE_ENABLED	 // not automatic - consume to much TCM 
    #endif

    // Constant definition

    //Used for ICAT display
    #define LSB             0x00000001
    #define MSB             0x80000000
    #define END_OF_STRING   '\0'

    // Macro definitions

    /* defining maximal users number for the multi user clocks */
    #define RM_MAX_MUC_USERS            13

    #define IS_USER_LAST(mUC,ALLOCfREE)                                             \
        ((RM_RSRC_FREE == ALLOCfREE) && (1 == _rmVars.multiUserCounters[mUC]))

    #define IS_USER_FIRST(mUC,ALLOCfREE)                                            \
        ((RM_RSRC_ALLOC == ALLOCfREE) && (0 == _rmVars.multiUserCounters[mUC]))

    #define RM_UPDATE_DB(rESOURCE,sTATUS)                                           \
                    {                                                               \
                        if (RM_RSRC_ALLOC == (sTATUS))                              \
                        {                                                           \
                        TURN_BIT_ON(_rmVars.currentResourcesStatus,(rESOURCE));   \
                        }                                                           \
                        else /* == RM_RSRC_FREE */                                  \
                        {                                                           \
                        TURN_BIT_OFF(_rmVars.currentResourcesStatus,(rESOURCE));  \
                        }                                                           \
                    }

#ifdef PHS_SW_DEMO_TTC_PM
#define RM_UPDATE_EXT_DB(rESOURCE,sTATUS)											\
				{																\
					if (RM_RSRC_ALLOC == (sTATUS))								\
					{															\
					TURN_BIT_ON(_rmVars.ext_currentResourcesStatus,(rESOURCE));   \
					}															\
					else /* == RM_RSRC_FREE */									\
					{															\
					TURN_BIT_OFF(_rmVars.ext_currentResourcesStatus,(rESOURCE));  \
					}															\
				}

#endif

    #define MAP_TO_MCCU_STATE(aLLOCfREE)                                            \
        ((RM_RSRC_ALLOC == (aLLOCfREE)) ? MCCU_CLK_ENABLE : MCCU_CLK_DISABLE)

    #define MAP_TO_COMMCCU_STATE(aLLOCfREE)                                         \
        ((RM_RSRC_ALLOC == (aLLOCfREE)) ? COMMCCU_CLK_ENABLE : COMMCCU_CLK_DISABLE)

    #define NO_USERS(mULTIuSERcLOCK)                                                \
        (0 == _rmVars.multiUserCounters[mULTIuSERcLOCK])

    #define GET_DB_RESOURCE_STATUS(rESOURCEbITmASK)                                 \
        (((rESOURCEbITmASK) & (_rmVars.currentResourcesStatus)) ?                   \
                                                    RM_RSRC_ALLOC : RM_RSRC_FREE)

    #define GET_MULTI_USER_CLK_STATE_FROM_DB(mULTIuSERcLOCK)                        \
        (GET_DB_RESOURCE_STATUS(MULTI_USER_CLK_TO_RESOURCE(mULTIuSERcLOCK)))

    // LUT related macros
    #define MULTI_USER_CLOCK_NEEDED(SERVICEiD,MUCiND)                               \
    (rmLookUpTables.serviceToMultiUserClocks[(SERVICEiD)][(MUCiND)] !=             \
                                                                NO_MULTI_USER_CLOCK)

    #define GET_MULTI_USER_CLOCK(SERVICEiD,MUCiND)                                  \
    (rmLookUpTables.serviceToMultiUserClocks[(SERVICEiD)][(MUCiND)])

    #define COMMCCU_CLOCK_NEEDED(SERVICEiD)                                         \
    (rmLookUpTables.serviceToCommCCUClock[(SERVICEiD)] != NO_COMMCCU_CLK)


    #define GET_COMMCCU_CLOCK(SERVICEiD)                                            \
    (rmLookUpTables.serviceToCommCCUClock[(SERVICEiD)])

    #define MCCU_CLOCK_NEEDED(SERVICEiD)                                            \
        (rmLookUpTables.serviceToMCCUClock[(SERVICEiD)] != NO_MCCU_CLK)

    #define GET_MCCU_CLOCK(SERVICEiD)                                               \
        (rmLookUpTables.serviceToMCCUClock[(SERVICEiD)])

    #define GET_MAIN_RESOURCE(SERVICEiD)                                            \
        (rmLookUpTables.serviceToMainResourceBitVector[(SERVICEiD)])

#ifdef PHS_SW_DEMO_TTC_PM
	#define SERVICE_IS_FREE(SERVICEiD)              \
				(NO_ALLOCATED_RESOURCES == (((SERVICEiD < 32) ?							\
				_rmVars.currentResourcesStatus : _rmVars.ext_currentResourcesStatus) &	 \
				(1 << (SERVICEiD%32))))
				
	#define SERVICE_IS_BUSY(SERVICEiD)												\
			(NO_ALLOCATED_RESOURCES != (((SERVICEiD < 32) ?							\
			_rmVars.currentResourcesStatus : _rmVars.ext_currentResourcesStatus) &	 \
			(1 << (SERVICEiD%32))))
#else
	#define SERVICE_IS_FREE(SERVICEiD)												\
			(NO_ALLOCATED_RESOURCES ==												  \
					(_rmVars.currentResourcesStatus & GET_MAIN_RESOURCE(SERVICEiD)))
	
	#define SERVICE_IS_BUSY(SERVICEiD)												\
			(NO_ALLOCATED_RESOURCES !=													\
					(_rmVars.currentResourcesStatus & GET_MAIN_RESOURCE(SERVICEiD)))
#endif



    #define NO_CLOCK_NEEDED(SERVICEiD)                                              \
        ((NO_COMMCCU_CLK == rmLookUpTables.serviceToCommCCUClock[(SERVICEiD)]) &&   \
        (NO_MCCU_CLK    == rmLookUpTables.serviceToMCCUClock[(SERVICEiD)]))

    #define GET_MCCU_FREQ(RMfREQ)                                                   \
        (rmLookUpTables.rmFreqtoMCCUFreq[(RMfREQ)])

    #define MULTI_USER_CLK_TO_RESOURCE(MULTIuSERcLOCK)                              \
        (rmLookUpTables.multiUserClkToResourceBitVector[(MULTIuSERcLOCK)])

    #define MULTI_USER_TO_COMMCCU(MULTIuSERcLOCK)                                   \
        (rmLookUpTables.multiUserToCommCCU[(MULTIuSERcLOCK)])

    #define RM_CBK_SERVICE_TO_COMMPM_WAKEUP(CBKsERVICEiD)                           \
        (rmLookUpTables.cbkServiceToCommPMWakeup[CBKsERVICEiD])

    /* Consider using a LUT here if the SW reset will be more widely supported */
    #define SW_RESET_NOT_SUPPORTED(sERVICEiD)                                       \
        ( ((sERVICEiD) != RM_SRVC_GSSP2)    &&                                      \
        ((sERVICEiD) != RM_SRVC_GSSP1_GB) )
	 #define MULTI_USER_DIRECTLY_CALLED(sERVICEiD)                                       \
        ( ((sERVICEiD) == RM_SRVC_VCTCXO)    ||                                      \
        ((sERVICEiD) == RM_SRVC_DDR_HIGH_FREQ) )

	  /* adding support for uart callback, not supported in Z0 */
    /* Not using LUT here because this macro isn't expected to be used very often */
    #define WAKEUP_NOT_SUPPORTED(CBKsERVICEiD)                                      \
        ((CBKsERVICEiD != RM_CALLBACK_SRVC_SSP)   &&                                \
        (CBKsERVICEiD != RM_CALLBACK_SRVC_USIM)   &&                                \
        (CBKsERVICEiD != RM_CALLBACK_SRVC_TIMER)  &&                                \
        (CBKsERVICEiD != RM_CALLBACK_SRVC_I2C)    &&                                \
		(CBKsERVICEiD != RM_CALLBACK_SRVC_UART)   &&		                        \
		(CBKsERVICEiD != RM_CALLBACK_SRVC_SCK)    &&                                \
        (CBKsERVICEiD != RM_CALLBACK_SRVC_WB_SLEEP_MODULE) &&                       \
        (CBKsERVICEiD != RM_CALLBACK_SRVC_AC_IPC) &&                       			\
        (CBKsERVICEiD != RM_CALLBACK_SRVC_MSL)    &&                                \
		(CBKsERVICEiD != RM_CALLBACK_SRVC_CGPIO)    )  

    // RM Service --> CPMU Peripheral mapping
    #define RM_SRVC_TO_CPMU_PERIPHERAL(rmsERVICE)                                   \
        (rmLookUpTables.rmServiceToCPMUPeripheral[(rmsERVICE)])

    // PBRCR definitions. (due to a silicon problem)
	//(used to config the GPB for infinite retries with 256 cycles between retries)
	#define RM_PBRCR_ADDRESS	(0xFFD07000UL)
	#define RM_PBRCR_REG		(*((volatile UINT32*) RM_PBRCR_ADDRESS))
	#define RM_PBRCR_VALUE		((UINT32)0x8000000F)
	#define RM_PBRCR_CONFIGURE	(RM_PBRCR_REG = RM_PBRCR_VALUE)


    // Debug mode definitions
    //#define RM_DEBUG_MODE_ENABLED - Defined in the make file !!!

    // The below definitions are meant to creat one point of reference for the boolean arguments.
    #define RM_MCCU_RETURNED_OK(MCCUrC)     ((MCCUrC) != MCCU_RC_OK)

    #if defined RM_DEBUG_MODE_ENABLED
        #define HANDLE_MCCU_RC(MCCUrC)      {if (RM_MCCU_RETURNED_OK(MCCUrC)) ASSERT(0);}
        #define HANDLE_RM_FREQ_INPUT_ERROR  ASSERT(0)
        #define CHECK_NULL_POINTER(pTR)     ASSERT(pTR)
        #define RM_UPDATE_LOG               RMLogUpdate()
        #define RM_CHECK_COUNTER(cOUNTER)   {if ((0 > (cOUNTER)||(RM_MAX_MUC_USERS < (cOUNTER)))) ASSERT(0);}
        #define IS_ENTRY_VALID(eNTRY)       ((_rmVars.rmDbgVars.resourcesStatusLog.resourcesArr[eNTRY] & MSB) != 0x0)
        #define MAKE_ENTRY_VALID(eNTRY)     TURN_BIT_ON(_rmVars.rmDbgVars.resourcesStatusLog.resourcesArr[eNTRY], MSB)
        #define HANDLE_UNSUPPORTED_RESET    ASSERT(FALSE)
        #define HANDLE_CPMU_RC(CPMUrC)      ASSERT(CPMU_RC_OK == (CPMUrC))
        #define HANDLE_WAKEUP_NOT_SUPPORTED ASSERT(FALSE)
    #else
        #define HANDLE_MCCU_RC(MCCUrC)      {if (RM_MCCU_RETURNED_OK(MCCUrC)) {return (MCCU_RC_CLOCK_ERR == (MCCUrC)) ? RM_RC_ERR_CLOCK : RM_RC_ERR_FREQ;}}
        #define HANDLE_RM_FREQ_INPUT_ERROR  {return RM_RC_ERR_CLOCK;}
        #define CHECK_NULL_POINTER(pTR)     {if (NULL == (pTR)) return RM_RC_ERR_NULL_POINTER;}
        #define RM_UPDATE_LOG
        #define RM_CHECK_COUNTER(cOUNTER)
        #define HANDLE_UNSUPPORTED_RESET    {return RM_RC_RESET_NOT_SUPPORTED;}
        #define HANDLE_CPMU_RC(CPMUrC)      {if (CPMU_RC_OK != (CPMUrC)) return RM_RC_ERR_CPMU;}
        #define HANDLE_WAKEUP_NOT_SUPPORTED {return;}
        // if was not void, should have returned:  RM_RC_WAKEUP_NOT_SUPPORTED;}
    #endif

#endif  /* _RM_DEF_H_ */
