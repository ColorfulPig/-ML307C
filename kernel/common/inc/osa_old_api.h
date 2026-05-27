/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _OSA_OLD_API_H
#define _OSA_OLD_API_H

#if !defined (OSA_QUEUE_NAMES)
#define OSA_QUEUE_NAMES
#endif
/*****************************************************************************
 * OSA Data Types
 ****************************************************************************/

#include "osa.h"

#ifdef PLAT_USE_THREADX
#include <tx_hisr.h>
#include <tx_api.h>

#include <osa_tx.h>
#endif
typedef void*   OSATaskRef;
typedef void*   OSAHISRRef;
typedef void*   OSASemaRef;
typedef void*   OSAMutexRef;
typedef void*   OSAMsgQRef;
typedef void*   OSAMailboxQRef;
typedef void*   OSAPoolRef;
typedef void*   OSATimerRef;
typedef void*   OSAFlagRef;
typedef void*	OSAPartitionPoolRef;
/* Remain for backwards compatibility */
typedef void*   OSTaskRef;
typedef void*   OSSemaRef;
typedef void*   OSMutexRef;
typedef void*   OSMsgQRef;
typedef void*   OSMailboxQRef;
typedef void*   OSPoolRef;
typedef void*   OSTimerRef;
typedef void*   OSFlagRef;
typedef UINT8   OS_STATUS ;


/*****************************************************************************
 * OSA Function Prototypes
 ****************************************************************************/


/*========================================================================
 *  OSA Initialization:
 *
 *  Initializes OSA internal structures, tables, and OS specific services.
 *
 *========================================================================*/

/***********************************************************************
 *
 * Name:        OSAInitialize()
 *
 * Description: Initialize OS specific stuff.
 *
 * Parameters:  None
 *
 * Returns:
 *	OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAInitialize()     OsaInit(NULL)


/***********************************************************************
 *
 * Name:        OSARun()
 *
 * Description: Activates the OS.
 *
 * Parameters:  None
 *
 * Returns:     None
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSARun()            OsaRun(NULL)


/*========================================================================
 *  OSA Task Management:
 *========================================================================*/

/***********************************************************************
 *
 * Name:        OSATaskCreate
 *
 * Description: Create Task.
 *
 * Parameters:
 *  OSATaskRef                 	*taskRef      [OUT]    OS task reference
 *  void		                  	*stackPtr      [IN]    pointer to start of task stack area
 *  UINT32 	                  	stackSize      [IN]    number of bytes in task stack area
 *  UINT8                	    	priority     [IN]    task priority 0 - 252
 *  CHAR                 		*taskName      [IN]    task name
 *  void                 			*taskStart(void*)      [IN]   pointer to task entry point
 *  void                 			*argv      [IN]    task entry argument pointer
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSATaskCreate(
    OSATaskRef* taskRef,
    void*       stackPtr,
    UINT32      stackSize,
    UINT8       priority,
    CHAR        *taskName,
    void        (*taskStart)(void*),
    void*       argv
 );


/***********************************************************************
 *
 * Name:        OSATaskCreateEx
 *
 * Description: Create Task.
 *
 * Parameters:
 *  OSATaskRef                 	*taskRef      [OUT]    OS task reference
 *  void		                  	*stackPtr      [IN]    pointer to start of task stack area
 *  UINT32 	                  	stackSize      [IN]    number of bytes in task stack area
 *  UINT8                	    	priority     [IN]    task priority 0 - 252
 *  CHAR                 		*taskName      [IN]    task name
 *  void                 			*taskStart(void*)      [IN]   pointer to task entry point
 *  void                 			*argv      [IN]    task entry argument pointer
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 * The task created doesn't start automatically.
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSATaskCreateEx(
    OSATaskRef* taskRef,
    void*       stackPtr,
    UINT32      stackSize,
    UINT8       priority,
    CHAR        *taskName,
    void        (*taskStart)(void*),
    void*       argv
 );

/***********************************************************************
 *
 * Name:        OSATaskDelete
 *
 * Description: Delete Task.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF      [IN]    Task Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskDelete(oSArEF)                       OsaTaskDelete(oSArEF,NULL)


/***********************************************************************
 *
 * Name:        OSATaskTerminate
 *
 * Description: Terminate Task.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF      [IN]    Task Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskTerminate(oSArEF)                      OsaTaskTerminate(oSArEF,NULL)



/***********************************************************************
 *
 * Name:        OSATaskSuspend
 *
 * Description: Suspend Task.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF      [IN]    Task Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskSuspend(oSArEF)                      OsaTaskSuspend(oSArEF,NULL)



/***********************************************************************
 *
 * Name:        OSATaskResume
 *
 * Description: Resume Task.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF      [IN]    Task Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskResume(oSArEF)                       OsaTaskResume(oSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSATaskSleep
 *
 * Description: Task sleep.
 *
 * Parameters:
 *  UINT32              tICKS           [IN]    Ticks to sleep.(1s=200ticks)
 *
 * Returns: None
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskSleep(tICKS)                         OsaTaskSleep(tICKS,NULL)

/***********************************************************************
 *
 * Name:        OSATaskChangePriority
 *
 * Description: Change task priority.
 *
 * Parameters:
 *  OSATaskRef          oSArEF          [IN]    Task Reference.
 *  UINT8                   nEW     [IN]    New Priority.
 *  UINT8                  *oLD    [OUT]    Old Priority.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskChangePriority(oSArEF,nEW,oLD)       OsaTaskChangePriority(oSArEF,nEW,oLD,NULL)


/***********************************************************************
 *
 * Name:        OSATaskGetPriority
 *
 * Description: Get task priority.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF          [IN]    Task Reference.
 *  UINT8                   *pRIORITY      [OUT]    Priority.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskGetPriority(oSArEF,pRIORITY)         OsaTaskGetPriority(oSArEF,pRIORITY,NULL)

/***********************************************************************
 *
 * Name:        OSAHISRGetPriority
 *
 * Description: Get HISR priority.
 *
 * Parameters:
 *  OSATaskRef                 oSArEF          [IN]    HISR Reference.
 *  UINT8                   *pRIORITY      [OUT]    Priority.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAHISRGetPriority(oSArEF,pRIORITY)         OsaHISRGetPriority(oSArEF,pRIORITY,NULL)

/***********************************************************************
 *
 * Name:        OSATaskYield
 *
 * Description: Allow other task to run.
 *
 * Parameters: None
 *
 * Returns:  None
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskYield()                              OsaTaskYield(NULL)

/***********************************************************************
 *
 * Name:        OSATaskGetCurrentRef
 *
 * Description: Get Task Ref.
 *
 * Parameters:
 *  OSATaskRef                 *PoSArEF    [OUT]    Task Reference.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATaskGetCurrentRef(PoSArEF)               OsaTaskGetCurrentRef(PoSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSAHISRGetCurrentRef
 *
 * Description: Get HISR Ref.
 *
 * Parameters:
 *  OSAHISRRef                 *PoSArEF    [OUT]    HISR Reference.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAHISRGetCurrentRef(PoSArEF)				OsaHISRGetCurrentRef(PoSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSAGetCurrentThreadRef
 *
 * Description: Get Current thread Ref.
 *
 * Parameters:
 *  OSATaskRef                 *PoSArEF    [OUT]    thread Reference.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAGetCurrentThreadRef(PoSArEF)				OsaGetCurrentThreadRef(PoSArEF,NULL)


/***********************************************************************
 *
 * Name:        OSAGetTaskInfo
 *
 * Description: Get the detailed information of specific thread
 *
 * Parameters:
 *  OSATaskRef                 oSArEF          [IN]    thread Reference.
 *  OSA_TASK                   *pTaskInfo      [OUT]    structure of thread info.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAGetTaskInfo(oSArEF, pTaskInfo)				OsaGetTaskInfo(oSArEF,pTaskInfo)

/***********************************************************************
 *
 * Name:        OSAGetThreadListHead
 *
 * Description: Get the head of created thread reference list
 *
 * Parameters:
 *  OSATaskRef                 *pListHead    [OUT]   list head.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAGetThreadListHead(pListHead)				OsaGetThreadListHead(pListHead,NULL)

/***********************************************************************
 *
 * Name:        OSAGetCreatedThreadCount
 *
 * Description: Get the count of created thread
 * Parameters:
 *  unsigned long                 *pCount    [OUT]   out.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAGetCreatedThreadCount(pCount)				OsaGetCreatedThreadCount(pCount,NULL)

/***********************************************************************
 *
 * Name:        OSAGetMaxThreadCount
 *
 * Description: Get the max number of thread that system can be created
 * Parameters:
 *  unsigned long                 *pCount    [OUT]   out.
 *
 * Returns:
 *      OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAGetMaxThreadCount(pCount)				OsaGetMaxThreadCount(pCount,NULL)


/***********************************************************************
 *
 * Name:        OSAThreadList
 *
 * Description: Get the reference list of created thread
 * Parameters:
 *  OSATaskRef                 *ptrList          [OUT]    thread Reference list.
 *  unsigned long               maximum_pointerst    [IN]   length of array
 *
 * Returns:
 *      unsigned long        Number of threads placed in list
 *
 * Notes:
 *
 ***********************************************************************/
#define		OSAThreadList(ptrList,maximum_pointers)				OsaThreadList(ptrList,maximum_pointers,NULL)


/*========================================================================
 *  OSA Semaphore Management
 *========================================================================*/

/***********************************************************************
 *
 * Name:        OSASemaphoreCreate
 *
 * Description: Create Semaphore.
 *
 * Parameters:
 *  OSASemaRef                 		*semaRef      [OUT]    OS semaphore reference
 *  UINT32		                  	initialCount      [IN]     initial count of the semaphore
 *  UINT8 	                  		waitingMode      [IN]    mode of tasks waiting (OS_FIFO, OS_PRIORITY)
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSASemaphoreCreate (
    OSASemaRef   *semaRef,
    UINT32      initialCount,
    UINT8       waitingMode
 );

/***********************************************************************
 *
 * Name:        OSASemaphoreCreateExt
 *
 * Description: Create Semaphore.
 *
 * Parameters:
 *  OSASemaRef                 		*semaRef      [OUT]    OS semaphore reference
 *  UINT32		                  	initialCount      [IN]     initial count of the semaphore
 *  UINT8 	                  		waitingMode      [IN]    mode of tasks waiting (OS_FIFO, OS_PRIORITY)
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSASemaphoreCreateExt(
    OSASemaRef   *semaRef,
    UINT32      initialCount,
    UINT32      maximumCount,
    UINT8       waitingMode
 );

/***********************************************************************
 *
 * Name:        OSASemaphoreAcquire
 *
 * Description: Acquire semaphore.
 *
 * Parameters:
 *  OSASemaRef                 	oSArEF          [IN]    Reference.
 *  UINT32                  		tIMEoUT         [IN]    Timeout in OS Ticks.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphoreAcquire(oSArEF,tIMEoUT)         OsaSemaphoreAcquire(oSArEF,tIMEoUT,NULL)

/***********************************************************************
 *
 * Name:        OSASemaphorePoll
 *
 * Description: Poll semaphore.
 *
 * Parameters:
 *  OSASemaRef                 	oSArEF          [IN]    Reference.
 *  UINT32                  		*PcOUNT         [OUT]    Current semaphore count.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphorePoll(oSArEF,PcOUNT)             OsaSemaphorePoll(oSArEF,PcOUNT,NULL)

/***********************************************************************
 *
 * Name:        OSASemaphoreRelease
 *
 * Description: Release semaphore.
 *
 * Parameters:
 *  OSASemaRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphoreRelease(oSArEF)                 OsaSemaphoreRelease(oSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSASemaphoreReleaseExt
 *
 * Description: Release semaphore.
 *
 * Parameters:
 *  OSASemaRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphoreReleaseExt(oSArEF)              OsaSemaphoreReleaseExt(oSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSASemaphoreDelete
 *
 * Description: Delete semaphore.
 *
 * Parameters:
 *  OSASemaRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphoreDelete(oSArEF)                  OsaSemaphoreDelete(oSArEF,NULL)

/***********************************************************************
 *
 * Name:        OsaSemaphoreCheck
 *
 * Description: Check whether semaphore is valid or not.
 *
 * Parameters:
 *  OsaRefT                 OsaRef          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSASemaphoreCheck(oSArEF)                 OsaSemaphoreCheck(oSArEF)

/*========================================================================
 *  OSA Mutex Management
 *========================================================================*/
IMPC extern OSA_STATUS OSAMutexCreate (
    OSMutexRef  *mutexRef,       /* OS mutex reference                         */
    UINT8       waitingMode      /* mode of tasks waiting OS_FIFO, OS_PRIORITY */
 );

/***********************************************************************
 *
 * Name:        OSAMutexLock
 *
 * Description:
 *
 * Parameters:
 *  OSAMutexRef                 oSArEF          [IN]    Reference.
 *  UINT32                  tIMEoUT         [IN]    Timeout in OS Ticks.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/

#define     OSAMutexLock(oSArEF,tIMEoUT)    OsaMutexLock(oSArEF,tIMEoUT,NULL)

/***********************************************************************
 *
 * Name:        OSAMutexUnlock
 *
 * Description:
 *
 * Parameters:
 *  OSAMutexRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMutexUnlock(oSArEF)          OsaMutexUnlock(oSArEF,NULL)

/***********************************************************************
 *
 * Name:        OSAMutexDelete
 *
 * Description:
 *
 * Parameters:
 *  OSAMutexRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMutexDelete(oSArEF)          OsaMutexDelete(oSArEF,NULL)


/*========================================================================
 *  OSA Interrupt Control
 *========================================================================*/


/***********************************************************************
 *
 * Name:        OSACreateHISR
 *
 * Description: Create ISR.
 *
 *  Parameters:
 *  OSAHISRRef                 	*oSArEF      [OUT]  oOS HISR  reference
 *  CHAR		                  	*name      [IN]    HISR name
  *  void                 		*hisr_entry(void)      [IN]   pointer to HISR entry point
 *  UINT8                	    	priority     [IN]    HISR priority 0 - 2
 *
 * Returns:
 *  None
 *
 * Notes:
 *
 ***********************************************************************/

#define OSACreateHISR(oSArEF, name, hisr_entry, priority)		Os_Create_HISR(hisr, name, hisr_entry, priority)


/***********************************************************************
 *
 * Name:        OSADeleteHISR
 *
 * Description: Delete ISR.
 *
 *  Parameters:
 *  OSAHISRRef                 *oSArEF      [IN]    reference
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSADeleteHISR(oSArEF)		Os_Delete_HISR(oSArEF)


/***********************************************************************
 *
 * Name:        OSAActivateHISR
 *
 * Description: Activate ISR.
 *
 *  Parameters:
 *  OSAHISRRef                 *oSArEF      [IN]    reference
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAActivateHISR(oSArEF)		Os_Activate_HISR(oSArEF)


/*===========================================================================
 *  OSA Real-Time Access:
 *=========================================================================*/
 /***********************************************************************
 *
 * Name:        OSAGetTicks
 *
 * Description: Number of ticks that passed since last reset.
 *
 * Parameters: None
 *
 * Returns:  UINT32   Number of ticks that passed since last reset
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAGetTicks()           OsaGetTicks(NULL)

/***********************************************************************
 *
 * Name:        OSAGetClockRate
 *
 * Description: Get the current system clock rate.
 *
 * Parameters: None
 *
 * Returns:     UNIT32  - current clock rate (ms / tick)
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAGetClockRate()       OsaGetClockRate(NULL)

/***********************************************************************
 *
 * Name:        OSATick
 *
 * Description: Tick the OS.
 *
 * Parameters: None
 *
 * Returns: None
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATick()               OsaTick(NULL)


/*========================================================================
 *  OSA Sys Context info
 *========================================================================*/

#define     OSAIsrEnable(mASK)                  OsaControlInterrupts(mASK,NULL)
#define     OSAIsrDisable(mASK)                 OsaControlInterrupts(mASK,NULL)

/***********************************************************************
 *
 * Name:        OSAContextLock
 *
 * Description: Lock context - No interrupts and no preemptions.
 *
 * Parameters:  None
 *
 * Returns:		OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAContextLock()                    OsaContextLock(NULL)


/***********************************************************************
 *
 * Name:       OSAContextUnlock
 *
 * Description: Restore the context.
 *
 * Parameters: None
 *
 * Returns:    OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAContextUnlock()                  OsaContextRestore(NULL)

 /***********************************************************************
  *
  * Name:		 OSAContextLockExt
  *
  * Description: Lock specific context-only interrtup and HISR preemptions allowed.
  *
  * Parameters:  None
  *
  * Returns:	 OS_SUCCESS/OS_FAIL
  *
  * Notes:
  *
  ***********************************************************************/
#define     OSAContextLockExt()                    OsaContextLockExt(NULL)


 /***********************************************************************
  *
  * Name:		OSAContextUnlockExt
  *
  * Description: Restore the specific context.
  *
  * Parameters: None
  *
  * Returns:	OS_SUCCESS/OS_FAIL
  *
  * Notes:
  *
  ***********************************************************************/
#define     OSAContextUnlockExt()                  OsaContextRestoreExt(NULL)


 /*========================================================================
  *  OSA Message Passing
  *========================================================================*/

/***********************************************************************
 *
 * Name:        OSAMsgQCreate
 *
 * Description: create message queue
 *
 * Parameters:
 *  OSMsgQRef                 	*msgQRef          [OUT]    OS message queue reference .
 *  char                  		*queueName       	 [IN]   name of message queue.
 *  UINT32                  		maxSize         [IN]    max message size the queue supports .
 *  UINT32                  		maxNumber         [IN]    max # of messages in the queue  .
 *  UINT32                  		waitingMode         [IN]    mode of tasks waiting OS_FIFO, OS_PRIORITY.

 * Returns:    OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMsgQCreate(
    OSMsgQRef   *msgQRef,
#if defined (OSA_QUEUE_NAMES)
    char        *queueName,
#endif
    UINT32      maxSize,
    UINT32      maxNumber,
    UINT8       waitingMode
 );


/***********************************************************************
 *
 * Name:		OSAMsgQSend
 *
 * Description: Send to message Q.
 *
 * Parameters:
 *  OSMsgQRef                 	msgQRef          [IN]    OS message queue reference .
 *  UINT32                  		size         [IN]    size of the message
 *  UINT8                  		*msgPtr         [IN]    start address of the data to be sent
 *  UINT32                  		timeout         [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout

 *
 * Returns:
 *	OSA_STATUS	OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMsgQSend (
    OSMsgQRef   msgQRef,
    UINT32      size,
    UINT8       *msgPtr,
    UINT32      timeout
 );


/***********************************************************************
 *
 * Name:		OSAMsgQRecv
 *
 * Description: Recieve from message Q..
 *
 * Parameters:
 *  OSMsgQRef                 	msgQRef          [IN]    OS message queue reference .
 *  UINT8                  		*recvMsg         OUT]     pointer to the message received
 *  UINT32                  		size         [IN]    size of the message
 *  UINT32                  		timeout         [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout

 *
 * Returns:
 *	OSA_STATUS	OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMsgQRecv (
    OSMsgQRef   msgQRef,
    UINT8       *recvMsg,
    UINT32      size,
    UINT32      timeout
 );

/***********************************************************************
 *
 * Name:        OSAMsgQPoll
 *
 * Description: Get the number of messages in queue.
 *
 * Parameters:
 *  OSAMsgQRef          oSArEF          [IN]    Reference.
 *  UINY32                  *cOUNT         [OT]    Number of messages in queue.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMsgQPoll(oSArEF,cOUNT)                           OsaMsgQPoll(oSArEF,cOUNT,NULL)


/***********************************************************************
 *
 * Name:        OSAMsgQDelete
 *
 * Description: Delete message Q.
 *
 * Parameters:
 *  OSAMsgQRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMsgQDelete(oSArEF)                               OsaMsgQDelete(oSArEF,NULL)


 /*========================================================================
  *  OSA Mailboxes
  *========================================================================*/

/***********************************************************************
 *
 * Name:        OSAMailboxQCreate
 *
 * Description: Create a mailbox Q.
 *              Each mailbox entry is 32 bit.
 *
 * Parameters:
 *  OSMailboxQRef                     *mboxQRef    [OUT]    OS mailbox queue reference.
 *  char                  			*queueName       	 [IN]   name of mailbox queue
 *  UINT32                  			maxNumber         [IN]    max # of messages in the queue  .
 *  UINT8                  			waitingMode         [IN]    mode of tasks waiting OS_FIFO, OS_PRIORITY.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMailboxQCreate (
    OSMailboxQRef   *mboxQRef,      /* OS mailbox queue reference              */
#if defined (OSA_QUEUE_NAMES)
    char            *queueName,     /* name of mailbox queue                   */
#endif
    UINT32          maxNumber,      /* max # of messages in the queue          */
    UINT8           waitingMode     /* mode of tasks waiting OS_FIFO, OS_PRIORITY */
 );


/***********************************************************************
 *
 * Name:        OSAMailboxQSend
 *
 * Description: Write data to mailbox Q.
 *
 * Parameters:
 *  OSAMailboxQRef                 	oSArEF          [IN]    Reference.
 *  UINT32                  			pTR       	 [IN]    Data to put in mailbox.
 *  UINT32                  			tIMEOUT         [IN]    timeout.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMailboxQSend(oSArEF,pTR,tIMEOUT)             OsaMailboxQSend(oSArEF,(UINT32)pTR,tIMEOUT,NULL)


/***********************************************************************
 *
 * Name:        OSAMailboxQRecv
 *
 * Description: Read from mailbox Q.
 *
 * Parameters:
 *  OSAMailboxQRef                 	oSArEF          [IN]    Reference.
 *  UINT32                 			*pTR        [OUT]    Data to receive from mailbox.
 *  UINT32                  			tIMEOUT         [IN]    timeout.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMailboxQRecv(oSArEF,pTR,tIMEOUT)             OsaMailboxQRecv(oSArEF,(UINT32 *)pTR,tIMEOUT,NULL)


/***********************************************************************
 *
 * Name:        OSAMailboxQPoll
 *
 * Description: Gen the number of messages in queue.
 *
 * Parameters:
 *  OSAMailboxQRef                 oSArEF          [IN]    Reference.
 *  UINY32                  		   *cOUNT         [OUT]    Number of messages in queue.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMailboxQPoll(oSArEF,cOUNT)                   OsaMailboxQPoll(oSArEF,cOUNT,NULL)


/***********************************************************************
 *
 * Name:        OSAMailboxQDelete
 *
 * Description: Delete mailbox Q.
 *
 * Parameters:
 *  OSAMailboxQRef                 oSArEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMailboxQDelete(oSArEF)                       OsaMailboxQDelete(oSArEF,NULL)


/*========================================================================
 *  OSA Event Management:
 *========================================================================*/

/***********************************************************************
 *
 * Name:        OsaFlagWait
 *
 * Description: Wait for event.
 *
 * Parameters:
 *  OSFlagRef                 flagRef          [IN]    OS reference to the flag.
 *  UINT32                  mask       	 	[IN]    flag mask to wait for .
 *  UINT32                  operation         [IN]    OSA_FLAG_AND, OSA_FLAG_AND_CLEAR,  OSA_FLAG_OR, OSA_FLAG_OR_CLEAR.
 *  UINT32                  *flags       	 	[OUT]    Current value of all flags
 *  UINT32                  timeout       	[IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAFlagWait(
    OSFlagRef   flagRef,
    UINT32      mask,
    UINT32      operation,
    UINT32      *flags,
    UINT32      timeout
 );
/***********************************************************************
 *
 * Name:        OSAFlagCreate
 *
 * Description: Create a flag event group.
 *
 * Parameters:
 *  OSFlagRef                     *rEF    [OT]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAFlagCreate(rEF)              OsaFlagCreate(rEF,NULL)

/***********************************************************************
 *
 * Name:        OSAFlagSet
 *
 * Description: Set an event.
 *
 * Parameters:
 *  OSFlagRef                 rEF          [IN]    Reference.
 *  UINT32                  mASK            [IN]    Flag mask.
 *  UINT32                  oP       [IN]    OSA_FLAG_AND, OSA_FLAG_OR.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAFlagSet(rEF,mASK,oP)         OsaFlagSet(rEF,mASK,oP,NULL)


/***********************************************************************
 *
 * Name:        OSAFlagDelete
 *
 * Description: Delete event group.
 *
 * Parameters:
 *  OSFlagRef                 rEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAFlagDelete(rEF)              OsaFlagDelete(rEF,NULL)


/*========================================================================
 *  OSA Timer Management:
 *========================================================================*/

/***********************************************************************
 *
 * Name:        OSATimerStart
 *
 * Description: Create a Timer if needed and activate it.
 *
 * Parameters:
 *  OSTimerRef                 	timerRef          [IN]    OS supplied timer reference.
 *  UINT32                  		initialTime       	 	[IN]     initial expiration time in OS tick (5ms each)
 *  UINT32                 	 	rescheduleTime         [IN]    timer period after initial expiration
 *  void                  		(*callBackRoutine)(UINT32)       	 	[IN]    timer call-back routine
 *  UINT32                  		timerArgc       	[IN]    argument to be passed to call-back on expiration
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSATimerStart(
    OSTimerRef  timerRef,
    UINT32      initialTime,
    UINT32      rescheduleTime,
    void        (*callBackRoutine)(UINT32),
    UINT32      timerArgc
 );

typedef OsaTimerStatusParamsT OSATimerStatus ;
/***********************************************************************
 *
 * Name:        OSATimerCreate
 *
 * Description: Create a Timer, if no input params the timer will be created on OsaTimerStart.
 *
 * Parameters:
 *  OSTimerRef                     *rEF    [OUT]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATimerCreate(rEF)             OsaTimerCreate(rEF,NULL)

/***********************************************************************
 *
 * Name:        OSATimerDelete
 *
 * Description: Delete timer.
 *
 * Parameters:
 *  OSTimerRef                 rEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATimerDelete(rEF)             OsaTimerDelete(rEF,NULL)


/***********************************************************************
 *
 * Name:        OSATimerStop
 *
 * Description: Stop timer.
 *
 * Parameters:
 *  OSTimerRef                 rEF          [IN]    Reference.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATimerStop(rEF)               OsaTimerStop(rEF,NULL)

/***********************************************************************
 *
 * Name:        OSATimerGetStatus
 *
 * Description: Get timer status.
 *
 * Parameters:
 *  OSTimerRef                 rEF          [IN]    Reference.
 *  OSATimerStatus                *sTATUS          [OT]    timer status.
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSATimerGetStatus(rEF,sTATUS)   OsaTimerStatus(rEF,sTATUS)


/*========================================================================
 *  OSA Memory Heap Access
 *
 *  Allocating Memory -
 *
 *  Deallocating Memory -
 *
 *========================================================================*/


/***********************************************************************
 *
 * Name:        OSAMemPoolCreate
 *
 * Description: Create a memory pool
 *
 * Parameters:
 *  OSPoolRef                 	*poolRef          [OUT]    OS assigned reference to the pool.
 *  UINT32                  		poolType       	 	[IN]    OSA_FIXED or OS_VARIABLE
 *  UINT8                 	 	*poolBase         [IN]    pointer to start of pool memory
 *  UINT32                  		poolSize      	 	[IN]    number of bytes in the memory pool
 *  UINT32                  		partitionSize       	[IN]     size of partitions in fixed pools
 *  UINT8                 	 	waitingMode         [IN]    mode of tasks waiting OS_FIFO, OS_PRIORITY
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMemPoolCreate (
    OSPoolRef *poolRef,
    UINT32    poolType,
    UINT8*    poolBase,
    UINT32    poolSize,
    UINT32    partitionSize,
    UINT8     waitingMode
 );

/***********************************************************************
 *
 * Name:        OSAMemPoolCreateExt
 *
 * Description: Create a independent memory pool
 *
 * Parameters:
 *  OSPoolRef                 	*poolRef          [OUT]    OS assigned reference to the pool.
 *  UINT32                  		poolType       	 	[IN]    OSA_FIXED or OS_VARIABLE
 *  UINT8                 	 	*poolBase         [IN]    pointer to start of pool memory
 *  UINT32                  		poolSize      	 	[IN]    number of bytes in the memory pool
 *  UINT32                  		partitionSize       	[IN]     size of partitions in fixed pools
 *  UINT8                 	 	waitingMode         [IN]    mode of tasks waiting OS_FIFO, OS_PRIORITY
 *  char                 		*poolName      [IN]    pool name
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMemPoolCreateExt (
    OSPoolRef *poolRef,
    UINT32    poolType,
    UINT8*    poolBase,
    UINT32    poolSize,
    UINT32    partitionSize,
    UINT8     waitingMode,
    char *poolName
 );

/***********************************************************************
 *
 * Name:        OSAMemPoolAlloc
 *
 * Description:  allocation from memory pool
 *
 * Parameters:
 *  OSPoolRef                 	poolRef          [IN]    OS assigned reference to the pool.
 *  UINT32                  		size       	 	[IN]    number of bytes to be allocated
 *  void                 	 		**mem         [OUT]    pointer to start of allocated memory
 *  UINT32                  		timeout      	 	[IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMemPoolAlloc (
    OSPoolRef poolRef,
    UINT32    size,
    void**    mem,
    UINT32    timeout
 );


/***********************************************************************
 *
 * Name:        OSAMemPoolFree
 *
 * Description:  free memory
 *
 * Parameters:
 *  OSPoolRef                 	poolRef          [IN]    OS assigned reference to the pool. Unused, can be NULL
 *  void                 	 		*mem           [IN]     pointer to start of memory to be freed
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
IMPC extern OSA_STATUS OSAMemPoolFree (
    OSPoolRef poolRef,
    void*     mem
 );

/***********************************************************************
 *
 * Name:        OSAMemPoolDelete
 *
 * Description:  delete memory pool
 *
 * Parameters:
 *  OSPoolRef                 	rEF          [IN]    OS assigned reference to the pool. Unused, can be NULL
 *
 * Returns:
 *  OS_SUCCESS
 *
 * Notes:
 *
 ***********************************************************************/
#define     OSAMemPoolDelete(rEF)                                       OS_SUCCESS
#if 0
#define     OSAMemPoolFixedFree(PmEM)                                   OSAMemPoolFree(NULL,PmEM)
#define     OSAMemPoolCreateGlobal(rEF,nAME,bASE,pOOLsIZE, pARTsIZE)    OSAMemPoolCreate(rEF,OSA_GLOBAL,bASE,pOOLsIZE, pARTsIZE,OS_PRIORITY)
#endif

/***********************************************************************
 *
 * Name:        OsaPartitionPoolCreate
 *
 * Description: Create Partition Pool.
 *
 * Parameters:
 *  OSAPartitionPoolRef         *rEF       	 	[OUT]    OS assigned reference to the pool
 *  char                 	 		*nAME         [IN]    pool name
 *  void                  		*sTARTaDDR      	 	[IN]     pointer to start of pool memory
 *  UINT32                  		pOOLsIZE       	[IN]      number of bytes in the memory pool
 *  UINT32                  		pARTsIZE       	[IN]     size of partitions in fixed pools
 *  UINT8                 	 	sUSPEND         [IN]    mode of tasks waiting OS_FIFO, OS_PRIORITY
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAPartitionPoolCreate(rEF, nAME, sTARTaDDR, pOOLsIZE, pARTsIZE, sUSPEND )		OsaPartitionPoolCreate(rEF, nAME, sTARTaDDR, pOOLsIZE, pARTsIZE, sUSPEND)

/***********************************************************************
 *
 * Name:        OSAPartitionAllocate
 *
 * Description: allocation a block from Partition Pool.
 *
 * Parameters:
 *  OSAPartitionPoolRef         *rEF       	 	[IN]    OS assigned reference to the pool
 *  void                  		**pBLOCK       	[OUT]      pointer to start of allocated block
 *  UINT8                 	 	sUSPEND         [IN]    OS_SUSPEND, OS_NO_SUSPEND, or timeout
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAPartitionAllocate(rEF, pBLOCK, sUSPEND)										OsaPartitionAllocate(rEF, pBLOCK, sUSPEND)


/***********************************************************************
 *
 * Name:        OSAPartitionFree
 *
 * Description: free the block
 *
 * Parameters:
 *  void                  		*pBLOCK       	[IN]      the block to be freedl
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAPartitionFree(pBLOCK)														OsaPartitionFree(pBLOCK)


/***********************************************************************
 *
 * Name:        OSAListAllCreatedTasks
 *
 * Description: list all created tasks and HISRs
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedTasks()						OsaListAllCreatedTasks()


/***********************************************************************
 *
 * Name:        OSAListAllCreatedTimers
 *
 * Description: list all created timers
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedTimers()						OsaListAllCreatedTimers()


/***********************************************************************
 *
 * Name:        OSAListAllCreatedEventFlags
 *
 * Description: list all created event flags
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedEventFlags()					OsaListAllCreatedEventFlags()


/***********************************************************************
 *
 * Name:        OSAListAllCreatedMemBlockPools
 *
 * Description: list all created memory block pools
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedMemBlockPools()					OsaListAllCreatedMemBlockPools()

/***********************************************************************
 *
 * Name:        OSAListAllCreatedMsgQs
 *
 * Description: list all created message queues
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedMsgQs()						OsaListAllCreatedMsgQs()


/***********************************************************************
 *
 * Name:        OSAListAllCreatedSemaphores
 *
 * Description: list all created semaphores
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedSemaphores()					OsaListAllCreatedSemaphores()



/***********************************************************************
 *
 * Name:        OSAListAllCreatedMutexs
 *
 * Description: list all created mutexs
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedMutexs()					OsaListAllCreatedMutexs()


/***********************************************************************
 *
 * Name:        OSAListAllCreatedMemBytesPoolsStatus
 *
 * Description: list all created memory bytes pools status
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  OSA_STATUS  OSA Complition Code.
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAListAllCreatedMemBytesPoolsStatus()					OsaListAllCreatedMemBytesPoolsStatus()

/***********************************************************************
 *
 * Name:        OSAGetDefaultMemPoolFreeSize
 *
 * Description: get the avaliable bytes of default heap pool
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  UINT32 the unused bytes of default heap pool
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAGetDefaultMemPoolFreeSize()					OsaGetDefaultMemPoolFreeSize()


/***********************************************************************
 *
 * Name:        OSAGetMemPoolFreeSize
 *
 * Description: get the avaliable bytes of specificed memory pool
 *
 * Parameters:
 *  OSPoolRef      rEF       	 	[IN]    memory pool reference
 *
 * Returns:
 *  UINT32 the unused bytes of specificed memory pool
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAGetMemPoolFreeSize(poolRef)					OsaGetMemPoolFreeSize(poolRef)

/***********************************************************************
 *
 * Name:        OSAGetVersion
 *
 * Description: get the current OS verison
 *
 * Parameters:
 *  None
 *
 * Returns:
 *  UINT8 *   OS verison string
 *
 * Notes:
 *
 ***********************************************************************/
#define OSAGetVersion()					OsaGetVersion()

/* Remap old names to new ones to remain backwards compatibility with old names.
 */
 #define OS_NULL                OSA_NULL
 #define OS_TIMER_DEAD          OSA_TIMER_DEAD
 #define OS_TIMER_CREATED       OSA_TIMER_CREATED
 #define OS_TIMER_ACTIVE        OSA_TIMER_ACTIVE
 #define OS_TIMER_INACTIVE      OSA_TIMER_INACTIVE
 #define OS_ENABLED             OSA_ENABLED
 #define OS_DISABLED            OSA_DISABLED
 #define OS_SUSPEND             OSA_SUSPEND
 #define OS_NO_SUSPEND          OSA_NO_SUSPEND
 #define OS_FLAG_AND            OSA_FLAG_AND
 #define OS_FLAG_AND_CLEAR      OSA_FLAG_AND_CLEAR
 #define OS_FLAG_OR             OSA_FLAG_OR
 #define OS_FLAG_OR_CLEAR       OSA_FLAG_OR_CLEAR
 #define OS_LOWEST_PRIORITY     OSA_LOWEST_PRIORITY
 #define OS_HIGHEST_PRIORITY    OSA_HIGHEST_PRIORITY
 #define OS_POOL_MEM_OVERHEAD   OSA_POOL_MEM_OVERHEAD

 #define OS_FIXED               OSA_FIXED
 #define OS_VARIABLE            OSA_VARIABLE
 #define OS_FIFO                OSA_FIFO
 #define OS_PRIORITY            OSA_PRIORITY

#define OSInitialize            OSAInitialize
#define OSRun                   OSARun
#define OSTaskCreate            OSATaskCreate
#define OSTaskDelete            OSATaskDelete
#define OSTaskPriority          OSATaskChangePriority
#define OSTaskGetPriority       OSATaskGetPriority
#define OSTaskSuspend           OSATaskSuspend
#define OSTaskResume            OSATaskResume
#define OSTaskSleep             OSATaskSleep
#define OSTaskYield             OSATaskYield
#define OSFlagCreate            OSAFlagCreate
#define OSFlagDelete            OSAFlagDelete
#define OSFlagSet               OSAFlagSet
#define OSFlagWait              OSAFlagWait
#define OSFlagPeek              OSAFlagPeek
#define OSTimerCreate           OSATimerCreate
#define OSTimerDelete           OSATimerDelete
#define OSTimerStart            OSATimerStart
#define OSTimerStop             OSATimerStop
#define OSTimerGetStatus        OSATimerGetStatus
#define OSMsgQCreate            OSAMsgQCreate
#define OSMsgQDelete            OSAMsgQDelete
#define OSMsgQSend              OSAMsgQSend
#define OSMsgQRecv              OSAMsgQRecv
#define OSMsgQPoll              OSAMsgQPoll
#define OSMailboxQCreate        OSAMailboxQCreate
#define OSMailboxQDelete        OSAMailboxQDelete
#define OSMailboxQSend          OSAMailboxQSend
#define OSMailboxQRecv          OSAMailboxQRecv
#define OSMailboxQPoll          OSAMailboxQPoll
#define OSSemaphoreCreate       OSASemaphoreCreate
#define OSSemaphoreDelete       OSASemaphoreDelete
#define OSSemaphoreAcquire      OSASemaphoreAcquire
#define OSSemaphoreRelease      OSASemaphoreRelease
#define OSSemaphorePoll         OSASemaphorePoll
#define OSMutexCreate           OSAMutexCreate
#define OSMutexDelete           OSAMutexDelete
#define OSMutexLock             OSAMutexLock
#define OSMutexUnlock           OSAMutexUnlock
#define OSIsrCreate             OSAIsrCreate
#define OSIsrDelete             OSAIsrDelete
#define OSIsrEnable             OSAIsrEnable
#define OSIsrDisable            OSAIsrDisable
#define OSIsrNotify             OSAIsrNotify
#define OSGetTicks              OSAGetTicks
#define OSGetClockRate          OSAGetClockRate
#define OSTick                  OSATick
#define OSMemPoolCreate         OSAMemPoolCreate
#define OSMemPoolDelete         OSAMemPoolDelete
#define OSMemPoolAlloc          OSAMemPoolAlloc
#define OSMemPoolFree           OSAMemPoolFree
#define OSContextLock           OSAContextLock
#define OSContextUnlock         OSAContextUnlock

#endif /* _OSA_OLD_API_H */
