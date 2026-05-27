/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ===========================================================================
File        : osa_utils.h
Description :

Copyright (c) 2001 Intel CCD. All Rights Reserved
=========================================================================== */

#ifndef _OSA_UTILS_H
#define _OSA_UTILS_H

typedef void*   OSATaskRef;
typedef void*   OSAHISRRef;

typedef void*   OSAMsgQRef;
typedef void*   OSAMailboxQRef;
typedef void*	OSAPartitionPoolRef;
typedef UINT8   OS_STATUS ;
IMPC extern UINT32 OSATaskList(OSATaskRef *ptrList,UINT32 maximum_pointers);
IMPC extern UINT32 OSAHISRList(OSAHISRRef  *ptrList,UINT32 maximum_pointers);
IMPC extern VOID* 	OSATaskGetStackStart(OSATaskRef taskRef);
IMPC extern UINT32		OSATaskGetStackSize(OSATaskRef taskRef);
IMPC extern void 		(*OSATaskGetEntry(OSATaskRef taskRef))(void *);
IMPC extern UINT32		OSATaskGetEntryParam(OSATaskRef hisrRef);
IMPC extern char*		OSATaskGetName(OSATaskRef taskRef);


IMPC extern UINT32 		OSATaskGetSysParam1(OSATaskRef taskRef);
IMPC extern void 		OSATaskSetSysParam1(OSATaskRef taskRef, UINT32 value);
IMPC extern UINT32 		OSATaskGetSysParam3(OSATaskRef taskRef);
IMPC extern void 		OSATaskSetSysParam3(OSATaskRef taskRef, UINT32 value);
#define					OSATaskGetIndex(a)			OSATaskGetSysParam3(a)
#define					OSATaskSetIndex(a, b)		OSATaskSetSysParam3(a, b)
IMPC extern VOID*		OSAHISRGetStackStart(OSATaskRef hisrRef);
IMPC extern UINT32		OSAHISRGetStackSize(OSATaskRef hisrRef);
IMPC extern void 		(*OSAHISRGetEntry(OSATaskRef hisrRef))(void);
IMPC extern char*		OSAHISRGetName(OSATaskRef hisrRef);

IMPC extern UINT32 		OSAHISRGetSysParam1(OSATaskRef hisrRef);
IMPC extern void 		OSAHISRSetSysParam1(OSATaskRef hisrRef, UINT32 value);
IMPC extern UINT32 		OSAHISRGetAppParam1(OSATaskRef hisrRef);
IMPC extern void 		OSAHISRSetAppParam1(OSATaskRef hisrRef, UINT32 value);

IMPC extern UINT32			OSAPartitionPoolGetAllocated(OSAPartitionPoolRef pool_ptr);
IMPC extern char*			OSAPartitionPoolGetName(OSAPartitionPoolRef PartitionPoolRef);
IMPC extern UINT32			OSAPartitionPoolGetAvailble(OSAPartitionPoolRef PartitionPoolRef);
IMPC extern UINT32			OSAPartitionPoolGetPartitionSize(OSAPartitionPoolRef PartitionPoolRef);
IMPC extern BOOL			OSAPartitionInUse( void* BlockPtr, void* PoolPtr );
IMPC extern OSAPartitionPoolRef 	OSAPartitionGetPoolPtr( void* BlockPtr );
IMPC extern OSA_STATUS 		OSAMsgQFrontSend(OSAMsgQRef queue_ptr, void *message,
                                        UINT32 size, UINT32 suspend);
#ifdef PLAT_USE_ALIOS
IMPC extern UINT64 Osa_TimeoutValueEx( UINT32 timeout );
IMPC extern UINT32 OsaGetThreadEntry(OsaRefT osaRef);
IMPC extern void OsaSetDbgDisOpt(UINT32 val);
IMPC extern UINT32 OsaGetDbgDisOpt(void);
#endif
#endif
