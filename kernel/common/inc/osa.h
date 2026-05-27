/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ===========================================================================
File        : osa.h
Description : This file defines the OS wrapper API and definitions for external application use.

              The OSA API allows applications to be developed independent of
              the target microkernel/hardware environment. It provides the
              facility to add support for multiple independent operating
              systems from different vendors.

Copyright (c) 2001 Intel CCD. All Rights Reserved
=========================================================================== */
#ifndef _OSA_H
#define _OSA_H

#ifdef PLAT_USE_THREADX
#include  <tx_thread.h>
#endif

#ifdef PLAT_USE_ALIOS
#include  <alios_type.h>
#endif


#ifndef IMPC
#define IMPC
#endif

#ifdef __cplusplus
  #ifdef ENV_SYMBIAN
    #include "e32def.h"
    #undef  IMPC
    #define IMPC  IMPORT_C
  #endif
extern "C" {
#endif

#include "gbl_types.h"


/*****************************************************************************
 * OSA Constants
 ****************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{
	OSA_TASK_READY,
	OSA_TASK_COMPLETED,
	OSA_TASK_TERMINATED,
	OSA_TASK_SUSPENDED,
	OSA_TASK_SLEEP,
	OSA_TASK_QUEUE_SUSP,
	OSA_TASK_SEMAPHORE_SUSP,
	OSA_TASK_EVENT_FLAG,
	OSA_TASK_BLOCK_MEMORY,
	OSA_TASK_MUTEX_SUSP,
	OSA_TASK_STATE_UNKNOWN,
}OSA_TASK_STATE;

//ICAT EXPORTED STRUCT
typedef struct OSA_TASK_STRUCT
{
    char                *task_name;                /* Pointer to thread's name     */
    unsigned int        task_priority;             /* Priority of thread (0-255)  */
    unsigned long        task_stack_def_val;             /* default vaule of  thread  */
    OSA_TASK_STATE      task_state;                /* Thread's execution state     */
    unsigned long       task_stack_ptr;           /* Thread's stack pointer   */
    unsigned long       task_stack_start;         /* Stack starting address   */
    unsigned long       task_stack_end;           /* Stack ending address     */
    unsigned long       task_stack_size;           /* Stack size               */
    unsigned long       task_run_count;            /* Thread's run counter     */

} OSA_TASK;

#define OSA_API_VERSION         51  /* this is the OSA API document version*/
#define OSA_NULL                NULL
#define OSA_TIMER_DEAD          0
#define OSA_TIMER_CREATED       1
#define OSA_TIMER_ACTIVE        2
#define OSA_TIMER_INACTIVE      3
#define OSA_ENABLED             2
#define OSA_DISABLED            3

#ifdef PLAT_USE_THREADX
#define OSA_LOWEST_PRIORITY     252
#define OSA_HIGHEST_PRIORITY    3
#elif defined(PLAT_USE_ALIOS)
#define OSA_LOWEST_PRIORITY     251
#define OSA_HIGHEST_PRIORITY    3
#else
#define OSA_LOWEST_PRIORITY     255
#define OSA_HIGHEST_PRIORITY    0
#endif

#define OSA_MIN_STACK_SIZE      256
#define OSA_ENABLE_INTERRUPTS   1
#define OSA_DISABLE_INTERRUPTS  2
#define OSA_SUSPEND             0xFFFFFFFF
#define OSA_NO_SUSPEND          0
#define OSA_FLAG_AND            5
#define OSA_FLAG_AND_CLEAR      6
#define OSA_FLAG_OR             7
#define OSA_FLAG_OR_CLEAR       8
#define OSA_FIXED               9
#define OSA_VARIABLE            10
#define OSA_FIFO                11
#define OSA_PRIORITY            12
#define OSA_GLOBAL              13
#define OSA_OS_INDEPENDENT      14

#define OSA_SISR_PRIORITY       1

#define OSA_TICK_FREQ_IN_MILLISEC   5
#define RTI_THREAD_ARRAY_SIZE	256//128

 /*========================================================================
  *  OSA Return Error Codes
  *========================================================================*/

  enum
  {
    OS_SUCCESS = 0,        /* 0x0 -no errors                                        */
    OS_FAIL,               /* 0x1 -operation failed code                            */
    OS_TIMEOUT,            /* 0x2 -Timed out waiting for a resource                 */
    OS_NO_RESOURCES,       /* 0x3 -Internal OS resources expired                    */
    OS_INVALID_POINTER,    /* 0x4 -0 or out of range pointer value                  */
    OS_INVALID_REF,        /* 0x5 -invalid reference                                */
    OS_INVALID_DELETE,     /* 0x6 -deleting an unterminated task                    */
    OS_INVALID_PTR,        /* 0x7 -invalid memory pointer                           */
    OS_INVALID_MEMORY,     /* 0x8 -invalid memory pointer                           */
    OS_INVALID_SIZE,       /* 0x9 -out of range size argument                       */
    OS_INVALID_MODE,       /* 0xA, 10 -invalid mode                                 */
    OS_INVALID_PRIORITY,   /* 0xB, 11 -out of range task priority                   */
    OS_UNAVAILABLE,        /* 0xC, 12 -Service requested was unavailable or in use  */
    OS_POOL_EMPTY,         /* 0xD, 13 -no resources in resource pool                */
    OS_QUEUE_FULL,         /* 0xE, 14 -attempt to send to full messaging queue      */
    OS_QUEUE_EMPTY,        /* 0xF, 15 -no messages on the queue                     */
    OS_NO_MEMORY,          /* 0x10, 16 -no memory left                              */
    OS_DELETED,            /* 0x11, 17 -service was deleted                         */
    OS_SEM_DELETED,        /* 0x12, 18 -semaphore was deleted                       */
    OS_MUTEX_DELETED,      /* 0x13, 19 -mutex was deleted                           */
    OS_MSGQ_DELETED,       /* 0x14, 20 -msg Q was deleted                           */
    OS_MBOX_DELETED,       /* 0x15, 21 -mailbox Q was deleted                       */
    OS_FLAG_DELETED,       /* 0x16, 22 -flag was deleted                            */
    OS_INVALID_VECTOR,     /* 0x17, 23 -interrupt vector is invalid                 */
    OS_NO_TASKS,           /* 0x18, 24 -exceeded max # of tasks in the system       */
    OS_NO_FLAGS,           /* 0x19, 25 -exceeded max # of flags in the system       */
    OS_NO_SEMAPHORES,      /* 0x1A, 26 -exceeded max # of semaphores in the system  */
    OS_NO_MUTEXES,         /* 0x1B, 27 -exceeded max # of mutexes in the system     */
    OS_NO_QUEUES,          /* 0x1C, 28 -exceeded max # of msg queues in the system  */
    OS_NO_MBOXES,          /* 0x1D, 29 -exceeded max # of mbox queues in the system */
    OS_NO_TIMERS,          /* 0x1E, 30 -exceeded max # of timers in the system      */
    OS_NO_MEM_POOLS,       /* 0x1F, 31 -exceeded max # of mem pools in the system   */
    OS_NO_INTERRUPTS,      /* 0x20, 32 -exceeded max # of isr's in the system       */
    OS_FLAG_NOT_PRESENT,   /* 0x21, 33 -requested flag combination not present      */
    OS_UNSUPPORTED,        /* 0x22, 34 -service is not supported by the OS          */
    OS_NO_MEM_CELLS,       /* 0x23, 35 -no global memory cells                      */
    OS_DUPLICATE_NAME,     /* 0x24, 36 -duplicate global memory cell name           */
    OS_INVALID_PARM        /* 0x25, 37 -invalid parameter                           */
  };


/*****************************************************************************
 * OSA Data Types
 ****************************************************************************/

#define OsaApi
typedef void    *OsaRefT ;
typedef UINT8   OSA_STATUS;

/*========================================================================
 *  OSA Initialization:
 *
 *  Initializes OSA internal structures, tables, and OS specific services.
 *
 *========================================================================*/
typedef enum
{
    OSA_SISR_HIGH_PRIORITY = 0,
    OSA_SISR_MED_PRIORITY  = 1,
    OSA_SISR_LOW_PRIORITY  = 2,
    OSA_SISR_MAX_PRIORITY           //  Keep Last.
}
OsaSisrPriorityT ;

typedef struct
{
    UINT32      *SisrStackPtr[(int)OSA_SISR_MAX_PRIORITY] ;     //  [OP,IN] SISR Stack Pointers.
    UINT32      SisrStackSize[(int)OSA_SISR_MAX_PRIORITY] ;     //  [IN]    SISR Stack Size.
}
OsaInitParamsT ;


IMPC extern OSA_STATUS   OsaInit(
    OsaInitParamsT          *pParams            //  [OP,IN] Initialization parameters.
    ) ;

IMPC extern void        OsaRun( void *pForFutureUse );

IMPC extern char        *OsaGetVersion( void );

/*========================================================================
 *  OSA Task Management:
 *========================================================================*/
typedef struct
{
    void                (*entry)(void *) ;      //  Task entry function.
    UINT32              stackSize ;             //  Size of stack.
    UINT32              *stackPtr ;             //  [OP]    Memory area for the stack.
    void                *argv ;                 //  [OP]    A pointer that is passed to the task.
    char                *name ;                 //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;         //  [OP]    TRUE - The object can be accessed from all processes.
    UINT8               priority ;              //  Task priority.
}
OsaTaskCreateParamsT ;

IMPC extern OSA_STATUS  OsaTaskCreateEx(
    OsaRefT                 *pOsaRef,           //  [OT]    Reference.
    OsaTaskCreateParamsT    *pParams            //  [IN]    Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaTaskCreate(
    OsaRefT                 *pOsaRef,           //  [OT]    Reference.
    OsaTaskCreateParamsT    *pParams            //  [IN]    Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaTaskDelete(
    OsaRefT                 OsaRef,             //  [IN]    Reference.
    void                    *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaTaskSuspend(
    OsaRefT                 OsaRef,             //  [IN]    Reference.
    void                    *pForFutureUse
    ) ;

IMPC extern OSA_STATUS OsaTaskResume(
    OsaRefT                 OsaRef,             //  [IN]    Reference.
    void                    *pForFutureUse
    ) ;

IMPC extern void        OsaTaskSleep(
    UINT32                  ticks,              //  [IN]    Time to sleep in ticks.
    void                    *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaTaskChangePriority(
    OsaRefT                 OsaRef,             //  [IN]    Reference.
    UINT8                   newPriority,        //  [IN]    New task priority.
    UINT8                   *oldPriority,       //  [OT]    Old task priority.
    void                    *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaTaskGetPriority(
    OsaRefT                 OsaRef,             //  [IN]    Reference.
    UINT8                   *pPriority,         //  [OT]    Task priority.
    void                    *pForFutureUse
    ) ;
IMPC extern OSA_STATUS OsaHisrCreate(
    OsaRefT* OsaRef,
    CHAR* name,
    VOID (*hisr_entry)(VOID),
    unsigned char priority
    );
IMPC extern OSA_STATUS OsaHisrActivate(OsaRefT *hisr);
IMPC extern OSA_STATUS OsaHisrDel(OsaRefT *hisr);
IMPC extern OSA_STATUS OsaSemaphoreCheck( OsaRefT OsaRef);
IMPC extern OSA_STATUS OsaHISRGetPriority(
	OsaRefT 				OsaRef, 			//  [IN]    Reference.
	UINT8 					*pPriority, 		//  [OT]    Task priority.
	void 					*pForFutureUse
	) ;

IMPC extern void        OsaTaskYield(  void *pForFutureUse  ) ;

IMPC extern OsaRefT     OsaCurrentThreadRef(void);

IMPC extern OSA_STATUS  OsaTaskGetCurrentRef(
    OsaRefT                 *pOsaRef,           //  [OT]    Reference.
    void                    *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaHISRGetCurrentRef(
    OsaRefT                 *pOsaRef,           //  [OT]    Reference.
    void                    *pForFutureUse
    ) ;
IMPC extern OSA_STATUS OsaTaskTerminate( OsaRefT OsaRef, void *pForFutureUse );
IMPC extern OSA_STATUS OsaGetTaskInfo(OsaRefT OsaRef, OSA_TASK *task_info);

IMPC extern OSA_STATUS OsaGetCurrentThreadRef(
	OsaRefT                 *pOsaRef,           //  [OT]    Reference.
    void                    *pForFutureUse
  );
IMPC extern OSA_STATUS OsaGetThreadListHead(
	OsaRefT *pListHead, //  [OT]    Reference.
	void *pForFutureUse
);

IMPC extern OSA_STATUS OsaGetCreatedThreadCount(
	unsigned long *count, //  [OT]    count
	void *pForFutureUse
);

IMPC extern OSA_STATUS OsaGetMaxThreadCount(
	unsigned long *maxCnt, //  [OT]	  count
	void *pForFutureUse
);
IMPC extern unsigned long OsaThreadList(OsaRefT *ptrList, unsigned long maximum_pointers,void *pForFutureUse);

/*========================================================================
 *  OSA Semaphore Management
 *========================================================================*/
typedef struct
{
    UINT32              initialCount ;              //  [OP]    Initial count of the semaphore (0 = Lock), (Default = 1).
    UINT32              maximumCount ;              //  [OP]    Maximum tasks that can "pass" through the semaphore, (Default = max(1,initialCount)).
    UINT8               waitingMode ;               //  [OP]    OS_FIFO, OS_PRIORITY, (Default = OS_PRIORITY).
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
}
OsaSemaphoreCreateParamsT ;

IMPC extern OSA_STATUS  OsaSemaphoreCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaSemaphoreCreateParamsT   *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    , UINT32 callerAddress) ;

IMPC extern OSA_STATUS  OsaSemaphoreAcquire(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      timeout,            //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaSemaphorePoll(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      *pCount,            //  [OT]    Current semaphore count.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaSemaphoreRelease(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS OsaSemaphoreReleaseExt(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaSemaphoreDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*========================================================================
 *  OSA Mutex Management
 *========================================================================*/
typedef struct
{
    UINT8               waitingMode ;               //  [OP]    OS_FIFO, OS_PRIORITY, (Default = OS_PRIORITY).
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
    BOOL				inherit;
}
OsaMutexCreateParamsT ;

IMPC extern OSA_STATUS  OsaMutexCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaMutexCreateParamsT       *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    , UINT32 callerAddress) ;

IMPC extern OSA_STATUS  OsaMutexLock(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      timeout,            //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMutexUnlock(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMutexDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*========================================================================
 *  OSA Interrupt Control
 *========================================================================*/
typedef struct
{
    UINT32              intSource ;                 //          Interrupt number (need to set to OSA_NULL_INT_SOURCE if not used).
    void                (*fisrRoutine)(UINT32) ;    //  [OP]    First level ISR to be called.
    void                (*sisrRoutine)(void) ;      //  [OP]    Second level ISR routine to be called.
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    UINT32              stackSize ;                 //  [OP]    Size of stack.
    OsaSisrPriorityT    priority ;                  //          SISR priority.
}
OsaIsrCreateParamsT ;

#define     OSA_NULL_INT_SOURCE         ((UINT32)-1)

IMPC extern OSA_STATUS  OsaIsrCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaIsrCreateParamsT         *pParams            //  [IN]    Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaIsrNotify(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaIsrDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*===========================================================================
 *  OSA Real-Time Access:
 *=========================================================================*/

IMPC extern UINT32  OsaGetTicks( void *pForFutureUse ) ;

IMPC extern UINT32  OsaGetClockRate( void *pForFutureUse ) ;

IMPC extern void    OsaTick( void *pForFutureUse ) ;

/*========================================================================
 *  OSA Sys Context info
 *========================================================================*/
typedef struct
{
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
}
OsaCriticalSectionCreateParamsT ;

IMPC extern OsaRefT     OsaCriticalSectionCreate(
    OsaCriticalSectionCreateParamsT     *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    ) ;

IMPC extern OsaRefT     OsaCriticalSectionEnter(
    OsaRefT                             OsaRef,             //  [IN]    Reference from the Create Function.
    void                                *pForFutureUse
    ) ;

IMPC extern void        OsaCriticalSectionExit(
    OsaRefT                             OsaRef,             //  [IN]    Reference from the Enter Function.
    void                                *pForFutureUse
    ) ;

IMPC extern void        OsaCriticalSectionDelete(
    OsaRefT                             OsaRef,             //  [IN]    Reference from the Create Function.
    void                                *pForFutureUse
    ) ;

IMPC extern UINT32      OsaControlInterrupts(
    UINT32                              mask,               //  [IN]    New interrupt mask.
    void                                *pForFutureUse
    ) ;

IMPC extern UINT32      OsaContextLock( void *pForFutureUse ) ;

IMPC extern UINT32      OsaContextRestore( void *pForFutureUse ) ;

IMPC extern OSA_STATUS OsaContextLockExt( void *pForFutureUse );

IMPC extern OSA_STATUS OsaContextRestoreExt( void *pForFutureUse );

 /*========================================================================
  *  OSA Message Passing
  *========================================================================*/
typedef struct
{
    UINT32              maxSize ;                   //  Max message size the queue supports.
    UINT32              maxMsg ;                    //  Max # of messages in the queue.
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
    UINT8               waitingMode ;               //  OS_FIFO, OS_PRIORITY.
}
OsaMsgQCreateParamsT ;

typedef struct
{
    void                *msgPtr ;                   //  [IN]    Start address of the data.
    UINT32              size ;                      //  [IN]    Size of the message.
    UINT32              timeout ;                   //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
}
OsaMsgQSendParamsT ;

typedef struct
{
    void                *msgPtr ;                   //  [IN]    Start address of the data.
    UINT32              size ;                      //  [IN]    Size of the message.
    UINT32              timeout ;                   //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    UINT32              actualSize ;                //  [OT]    Actual bytes read from queue.
}
OsaMsgQRecvParamsT ;

IMPC extern OSA_STATUS  OsaMsgQCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaMsgQCreateParamsT        *pParams            //  [IN]    Input Parameters (see datatype for details).
    ,UINT32 callerAddress) ;

IMPC extern OSA_STATUS  OsaMsgQSend(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    OsaMsgQSendParamsT          *pParams            //  [IN]    See datatype.
    ) ;

IMPC extern OSA_STATUS  OsaMsgQRecv(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    OsaMsgQRecvParamsT          *pParams            //  [IN]    See datatype.
    ) ;

IMPC extern OSA_STATUS  OsaMsgQPoll(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      *pCount,            //  [OT]    Number of messages in Q.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMsgQFreeRate(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      *pFreeRate          //  [OT]    free Rate of Q.

    ) ;

IMPC extern OSA_STATUS  OsaMsgQDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

 /*========================================================================
  *  OSA Mailboxes
  *========================================================================*/
typedef struct
{
    UINT32              maxMsg ;                    //  Max # of messages in the queue.
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
    UINT8               waitingMode ;               //  OS_FIFO, OS_PRIORITY.
}
OsaMailboxQCreateParamsT ;

IMPC extern OSA_STATUS  OsaMailboxQCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaMailboxQCreateParamsT    *pParams            //  [IN]    Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaMailboxQSend(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      mboxData,           //  [IN]    Data to put in mailbox.
    UINT32                      timeout,            //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMailboxQRecv(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      *mboxData,          //  [OT]    Data read from mailbox.
    UINT32                      timeout,            //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMailboxQPoll(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      *pCount,            //  [OT]    Number of messages in Q.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaMailboxQDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*========================================================================
 *  OSA Event Management:
 *========================================================================*/
typedef struct
{
    char                *name ;                     //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;             //  [OP]    TRUE - The object can be accessed from all processes.
}
OsaFlagCreateParamsT ;

typedef struct
{
    UINT32              mask ;                      //  [IN]    Flag mask.
    UINT32              operation ;                 //  [IN]    OSA_FLAG_AND, OSA_FLAG_AND_CLEAR, OSA_FLAG_OR, OSA_FLAG_OR_CLEAR.
    UINT32              timeout ;                   //  [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout in OS ticks.
    UINT32              *flags ;                    //  [OT]    Current value of the flag.
}
OsaFlagWaitParamsT ;

IMPC extern OSA_STATUS  OsaFlagCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaFlagCreateParamsT        *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaFlagSet(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    UINT32                      mask,               //  [IN]    Flag mask.
    UINT32                      operation,          //  [IN]    OSA_FLAG_AND, OSA_FLAG_OR.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaFlagWait(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    OsaFlagWaitParamsT          *pParams            //  [IN]    Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaFlagDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*========================================================================
 *  OSA Timer Management:
 *========================================================================*/
typedef struct
{
    UINT32              initialTime ;                   //  [IN]    Initial expiration time in clock ticks.
    UINT32              rescheduleTime ;                //  [IN]    Periodic expiration time in clock ticks. 0=One shot timer.
    void                (*callBackRoutine)(UINT32) ;    //  [IN]    Routine to call when timer expiers.
    UINT32              timerArgc ;                     //  [IN]    Argument to be passed to the callBackRoutine
    char                *name ;                         //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;                 //  [OP]    TRUE - The object can be accessed from all processes.
}
OsaTimerParamsT ;

typedef struct
{
    UINT32              status  ;                       //  [O]     Timer status OS_ENABLED, OS_DISABLED.
}
OsaTimerStatusParamsT ;

IMPC extern OSA_STATUS  OsaTimerCreate(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaTimerParamsT             *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaTimerStart(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    OsaTimerParamsT             *pParams            //  [OP,IN] Input Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaTimerStop(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

IMPC extern OSA_STATUS  OsaTimerStatus(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    OsaTimerStatusParamsT       *pParams            //  [OT]    Output Parameters (see datatype for details).
    ) ;

IMPC extern OSA_STATUS  OsaTimerDelete(
    OsaRefT                     OsaRef,             //  [IN]    Reference.
    void                        *pForFutureUse
    ) ;

/*============================================================================
 * Special memory handling that doesn't use the OS memory services.
 *============================================================================*/
typedef struct
{
    void                *poolBase ;                     //          Pointer to start of pool memory.
    UINT32              poolSize ;                      //          size of the pool.
    char                *name ;                         //  [OP]    Pointer to a NULL terminated string.
    BOOL                bSharedForIpc ;                 //  [OP]    TRUE - The object can be accessed from all processes.
    UINT32              LowWaterMark ;                  //  [OP]    Number of bytes left in pool tp trigger the LowWaterMarkCbFunc.
    void                (*LowWaterMarkCbFunc)(UINT32) ; //  [OP]    Routine to call when the LowWaterMark is triggered.
}
OsaMemCreateParamsT ;

IMPC extern OSA_STATUS  OsaMemAddMemoryToPool(
    OsaRefT                     OsaRef,             //  [OP,IN] Pool Reference, if NULL, the default pool is used.
    void                        *memBase,           //  [IN]    Pointer to the memory to be added.
    UINT32                      memSize,            //  [IN]    Size of the memory to be added.
    void                        *pForFutureUse
    ) ;

IMPC extern void *      OsaMemAlloc(
    OsaRefT                     OsaRef,             //  [OP,IN] Pool Reference, if NULL, the default pool is used.
    UINT32                      Size                //  [IN]    Number of bytes to be allocated.
    ) ;

IMPC extern void *      OsaMemAllocExt(
    OsaRefT                     poolRef,             //  [OP,IN] Pool Reference, if NULL, the default pool is used.
    UINT32                      Size,                //  [IN]    Number of bytes to be allocated.
    BOOL                        align32
    ) ;

IMPC extern BOOL        OsaMemAllocAgain(
    void                        *pMem               //  [IN]    Pointer to the memory to be allocated again.
    ) ;

IMPC extern OSA_STATUS  OsaMemCreatePool(
    OsaRefT                     *pOsaRef,           //  [OT]    Reference.
    OsaMemCreateParamsT         *pParams            //  [IN] Input Parameters (see datatype for details).
    ) ;

IMPC extern void        OsaMemFree(
    void                        *pMem               //  [IN]    Pointer to the memory to be freed.
    ) ;

IMPC extern UINT32      OsaMemGetAllocSize(
    void                        *pMem               //  [IN]    Pointer to the memory.
    ) ;

IMPC extern OsaRefT     OsaMemGetDefaultPoolRef( void ) ;

IMPC extern OsaRefT     OsaMemGetPoolRef(
    char                        *poolName,          //  [OP,IN] Pool's name - can be NULL.
    void                        *pMem,              //  [OP,IN] A memory address we need the poolRef for it - can be NULL.
    void                        *pForFutureUse
    ) ;

IMPC extern UINT32      OsaMemGetUserParam(
    void                        *pMem               //  [IN]    Pointer to the allocated memory.
    ) ;

IMPC extern BOOL        OsaMemResizeAlloc(
    void                        *pMem,              //  [IN]    Pointer to the memory to be re-sized.
    UINT32                      NewSize             //  [IN]    New Size.
    ) ;

IMPC extern void        OsaMemSetDefaultPool(
    OsaRefT                     OsaRef              //  [IN]    Pool Reference.
     ) ;

IMPC extern OSA_STATUS  OsaMemSetUserParam(
    void                        *pMem,              //  [IN]    Pointer to the allocated memory.
    UINT32                      Param               //  [IN]    User's parameter.
     ) ;

IMPC extern OSA_STATUS	OsaPartitionAllocate(
	OsaRefT * 					pOsaRef,
	void **						return_pointer,
	UINT32 						suspend
	);

IMPC extern OSA_STATUS	OsaPartitionFree(
	OsaRefT 					OsaRef
	);

IMPC extern OSA_STATUS OsaPartitionPoolCreate(
	OsaRefT *					pOsaRef,
	char *						pool_name,
	void *						start_address,
	UINT32 						pool_size,
	UINT32 						partition_size,
	UINT32 						suspend_type
	);

IMPC extern UINT32 Osa_TimeoutValue(
	UINT32 						timeout
	);

IMPC extern OSA_STATUS OsaSystemProtectInit(void);

IMPC extern OSA_STATUS OsaSystemProtect(void);

IMPC extern OSA_STATUS OsaSystemUnProtect(void);

IMPC extern UINT32 OsaGetInterruptCount(void);

IMPC extern OSA_STATUS OsaListAllCreatedTasks( void );

IMPC extern OSA_STATUS OsaListAllCreatedTimers( void );

IMPC extern OSA_STATUS OsaListAllCreatedEventFlags( void );

IMPC extern OSA_STATUS OsaListAllCreatedMemBlockPools( void );

IMPC extern OSA_STATUS OsaListAllCreatedMsgQs( void );

IMPC extern OSA_STATUS OsaListAllCreatedSemaphores( void );

IMPC extern OSA_STATUS OsaListAllCreatedMutexs( void );


IMPC extern OSA_STATUS OsaListAllCreatedMemBytesPoolsStatus(void);

/*=============================================================================
 * Remap old names to new ones to remain backwards compatibility with old names.
 *=============================================================================*/
#include    "osa_old_api.h"
#include    "osa_utils.h"
#include    <bsp_hisr.h>
#include	"nucleus.h"
#include 	"osa_internals.h"


#define OS_INTERRUTPT_COUNT				((UINT32)OsaGetInterruptCount())


#ifdef PLAT_USE_THREADX

 #include   <osa_tx.h>
 #include   <tx_hisr.h>
 #include   "tx_api.h"
 #include   "tx_initialize.h"
 #include 	"osa_um_extr.h"

extern volatile unsigned long	_tx_thread_system_state;

//#define OS_INTERRUTPT_COUNT				(_tx_thread_system_state)
#define OS_MAX_NAME						TX_MAX_NAME
#define OS_POOL_PARTITION_OVERHEAD      (sizeof(unsigned char *))
#define OS_PARTITION_MEM_OVERHEAD		(0)

#elif defined(PLAT_USE_ALIOS)

#define OS_MAX_NAME						9
#define OS_POOL_PARTITION_OVERHEAD      (sizeof(unsigned char *))
#define OS_PARTITION_MEM_OVERHEAD		(0)

#include  <k_api.h>
#include  <osa_ali.h>
#include  "osa_um_extr.h"

#else

#include   "um_defs.h"
#include   "tc_defs.h"
#include	"nucleus.h"


//extern INT	TCD_Interrupt_Count;

//#define OS_INTERRUTPT_COUNT				(TCD_Interrupt_Count)
#define OS_MAX_NAME						NU_MAX_NAME
#define OS_POOL_PARTITION_OVERHEAD      (PM_OVERHEAD)
#define OS_PARTITION_MEM_OVERHEAD		(0)
#endif


#ifdef __cplusplus
}
#endif
#endif /* _OSA_H */

