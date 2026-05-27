/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************
*
*  Filename: bspUartManager.h
*
* This is an additional BSP layer  between UART-driver and a UART's "customer".
* UART customers may be:
*   - Platform's message print, HeartBeat...
*   - ACAT DIAG
*   - 3G Protocol Stack
* Calling for this is not must but upon customer decision only,
* e.g. customer may call directly uart-driver function.
*
* In some cases an alternating of behavior or routing may be required in run-time.
* But sometimes the alternating is required on early start-up stage and
* so MUST be done statically upon SW define flags.
* 
* Global API functions:
*  bspUartPhase1Init()
*  bspUartPhase2Init()
*  bspUartPlatUseAltFunc()
*  bspUartApplUseAltFunc()
*  bspUartNumOfUartsGet()
*  bspUartGetIoUart()
*
* Global API variables may be used for "fast decision"
*  bspUartPlatUseAltFuncVar
*  bspUartApplUseAltFuncVar
* 
*******************************************************************************/
#ifndef BSP_UART_MANAGER_H
#define BSP_UART_MANAGER_H

#include "global_types.h"
#include "platform_nvm.h"


extern platformUART_route_ts  bspUartPlatUseAltFuncVar;
extern int                    bspUartApplUseAltFuncVar;
extern int                    bspUartNumOfUartsVar;

void bspUartPhase1Init(void);
void bspUartPhase2Init(void);
void bspUartPhase3Init(void);

platformUART_route_ts  bspUartPlatUseAltFunc(void);
int                    bspUartApplUseAltFunc(void);
void bspUartUseForErrorIO(void);
int  bspUartNumOfUartsGet(void);
int  bspUartGetIoUart    (void);

#endif// BSP_UART_MANAGER_H
