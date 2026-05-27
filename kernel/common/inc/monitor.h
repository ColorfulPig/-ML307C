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

/*******************************************************************************
*               MODULE HEADER FILE
********************************************************************************
* Title:  			Monitor
*
* Filename: 		Monitor.h
*
* Target, platform: Common Platform, SW platform
*
* Authors: 			Oren Moravchik
*
* Description: 		Diag monitor for ICAT
*
* Last Updated:     09/02/2005
*
* Notes:
*******************************************************************************/


#ifndef _MONITOR_H_
    #define _MONITOR_H_


/*----------- Local include files --------------------------------------------*/
#include "global_types.h"
//#include "diags.h"


#define		DONT_MODIFY					0
#define		MODIFY_REQUEST				2
#define		ATT_MP_VALID				1
#define		ATT_MP_MODIFIABLE			2
/*----------- Global defines -------------------------------------------------*/


/*----------- Global macro definitions ---------------------------------------*/


/*----------- Global type definitions ----------------------------------------*/

/*-----------------1/1/2006 5:54PM------------------

  Defined in DiagAPI.c

#define MONITOR_CONNECT( Type , Description , Address , Modifiable , Function , Sub_Cat) \
{																				\
	extern int __MON_##Description(void);												\
	int paramID;																		\
	paramID = ( __MON_##Description());													\
	MonitorConnect(paramID, Address, Modifiable, Function);								\
}


#define MONITOR_DISCONNECT( Description ) \
{		\
	extern int __MON_##Description(void);												\
	int paramID;																		\
	paramID = ( __MON_##Description());													\
	MonitorDisconnect(paramID);								\
}																																			\
*/


//A single Monitored Param
typedef struct MonitorParameterTag
{
    // Actual parameters
	void*		m_pDataAddress;
	UINT8		m_iAttributes;
	void		(*m_pFunc)(void);
} MonitorParam;


typedef enum
{
   NOT_MODIFIABLE_BY_MONITOR,
   MODIFIABLE_BY_MONITOR
}  eModifiable;


/*----------- Extern definition ----------------------------------------------*/


/*----------- Global variable declarations -----------------------------------*/


/*----------- Global constant definitions ------------------------------------*/


/*----------- Global function prototypes -------------------------------------*/
Bool MonitorConnect( int i_nMPID , void* i_pAddress , eModifiable i_eModifiable ,
						void (*i_pFunc)(void) );


Bool MonitorDisconnect( int i_nMPID );

//void RequestMonitorParams( void* i_pData );

// void BuildAndSendRegistrationData( void* i_pData );

#endif



