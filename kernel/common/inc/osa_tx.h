/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ===========================================================================
File        : osa_tx.h
Description : Definition of OSA Software Layer data types specific to the
              ThreadX OS.

Notes       :

=========================================================================== */


#ifndef _OSA_TX_H
#define _OSA_TX_H

#include <osa.h>
#include <tx_thread.h>
#include <tx_port.h>
#include <tx_hisr.h>
#include <tx_timer.h>
#include <tx_api.h>
#include "tx_initialize.h"
#include <bsp_hisr.h>


#define 	TX_MAX_NAME		9
/*
 * Data Types.
 */
typedef struct
{
    TX_SEMAPHORE    TxRef ;
    UINT          maximumCount ;
	UINT 			waitingMode;
	UINT			reserved;
}
OsaSemaphoreT ;

typedef struct
{
    TX_HISR     TxRef ;
    UINT      intSource ;
    void        (*fisrRoutine)(UINT32) ;
    void        (*sisrRoutine)(void) ;
	UINT		reserved;
}
OsaIsrT ;

typedef struct
{
	TX_QUEUE 	TxRef ;
	UINT 		waitingMode;
	UINT		reserved;
}
OsaMsgQT ;

typedef struct
{
    TX_MUTEX    TxRef ;
	UINT 			waitingMode;
	UINT			reserved;
}
OsaMutexT ;
/*
 * Defines.
 */
#define     OSA_DUMMY_CRITICAL_SECTION_HANDLE       ((OsaRefT)(~0))
#define     OSA_MSGQ_MSG_SIZE(sIZE)                 ((UNSIGNED)(sIZE + sizeof(UNSIGNED) - 1) / sizeof(UNSIGNED))    /*  Size is in UNSIGNED not bytes. */
#define     OSA_UNINITIALIZED_TIMER                 (~TX_TIMER_ID)

/*
 * Macros.
 */

/*
 * Data.
 */

/*
 * Functions.
 */
BOOL Osa_TranslateErrorCode( char *callerFuncName, UINT ErrorCode, OSA_STATUS *pOsaStatus ) ;


#endif

