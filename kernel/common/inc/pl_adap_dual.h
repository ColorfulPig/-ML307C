/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/**********************************************************************
*
* Filename: pl_adap_dual.h
*
* Programmers:  Marcelo Senter (ms)
*
* Description:  Internal Dual Mode APIs
*
**********************************************************************/

#ifndef PL_ADAP_DUAL_H
#define PL_ADAP_DUAL_H

#include "system.h"

#ifndef GPLC_ONLY

#if defined (ENABLE_CAT1_LG)   //crane_LG
void plAMsendGsmMeasureParams( void );
#endif

#if !defined INTEL_UPGRADE_GSM_CRL_IF
void wgiFillGsmMeasParams( UINT16* data, UINT16* numofwordwritten );
#else /* INTEL_UPGRADE_GSM_CRL_IF */
void wgiFillGsmMeasParams( INT16 SectionIndex, UINT16* data, UINT16* numofwordwritten );
#endif /* INTEL_UPGRADE_GSM_CRL_IF */
UINT8 wgiGetNumOfGainTables(void);
UINT16 wgiGetGainTablesSize(UINT8 numOfGsmGainTables);
VOID wgiCopyGainTables(UINT16 *dataDstPtr, UINT8 numOfGsmGainTables);
#if defined (UPGRADE_DSDS)
void wgiSaveGsmAFCValues(INT8 simID);
#else
void wgiSaveGsmAFCValues(void);
#endif
INT16 wgiGetSignedAfcDacVal(void);
VOID wgiGetGsmTimingParams(UINT16* pData);
#if defined (UPGRADE_DSDS)
void wgiSetResumeFlag(UINT16 val,UINT8 simID);
#else
void wgiSetResumeFlag(UINT16 val);
#endif
#if defined (UPGRADE_DSDS) 
UINT16 wgiGetResumeFlag(UINT8 simID);
#else
UINT16 wgiGetResumeFlag(void);
#endif
void ResetHoTo3GOnGoingFlag(void);
#if defined (UPGRADE_DSDS)
void wgiResetUtranMeas(UINT8 simID);
#else
void wgiResetUtranMeas(void);
#endif
void egiGetFujitsuAFCValues(Int16 Slope, Int16 NominalValue);

#endif /* GPLC_ONLY */

#if defined (UPGRADE_MULTI_RAT)
void ResetHoTo4GOnGoingFlag (void);
VOID GSMTerminateAfterDeactive(INT8 simID );
#endif

#if !defined (UPGRADE_DSDS)
VOID	GSMStartup( Boolean timed );
#else
void GSMStartup(Boolean timed, UINT8 simId);
#endif
#if !defined (UPGRADE_DSDS)
VOID    GSMTerminate( Boolean sendCnf);
#else
VOID    GSMTerminate( Boolean sendCnf, UINT8 simID );
#endif
#if !defined (UPGRADE_DSDS)
VOID    GSMPowerup(VOID);
#else
void GSMPowerup(UINT8 simId);
#endif
VOID GSMTerminateAfterDeactive(INT8 simID );  //xhzhao:need to check for DSDS

#endif /* PL_ADAP_DUAL_H */
