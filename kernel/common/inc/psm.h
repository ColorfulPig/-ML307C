/* -*- linux-c -*- */
/*******************************************************************************
*               Copyright 2009, Marvell Technology Group Ltd.
*
* THIS CODE CONTAINS CONFIDENTIAL INFORMATION OF MARVELL. NO RIGHTS ARE GRANTED
* HEREIN UNDER ANY PATENT, MASK WORK RIGHT OR COPYRIGHT OF MARVELL OR ANY THIRD
* PARTY. MARVELL RESERVES THE RIGHT AT ITS SOLE DISCRETION TO REQUEST THAT THIS
* CODE BE IMMEDIATELY RETURNED TO MARVELL. THIS CODE IS PROVIDED "AS IS".
* MARVELL MAKES NO WARRANTIES, EXPRESS, IMPLIED OR OTHERWISE, REGARDING ITS
* ACCURACY, COMPLETENESS OR PERFORMANCE. MARVELL COMPRISES MARVELL TECHNOLOGY
* GROUP LTD. (MTGL) AND ITS SUBSIDIARIES, MARVELL INTERNATIONAL LTD. (MIL),
* MARVELL TECHNOLOGY, INC. (MTI), MARVELL SEMICONDUCTOR, INC. (MSI), MARVELL
* ASIA PTE LTD. (MAPL), MARVELL JAPAN K.K. (MJKK), GALILEO TECHNOLOGY LTD. (GTL)
* GALILEO TECHNOLOGY, INC. (GTI) AND RADLAN Computer Communications, LTD.
********************************************************************************
*/
/* psm -- Persistent Storage Manager */
#ifndef _PSM_H_
#define _PSM_H_
#include <stdlib.h>
#include <string.h>
#include "UART.h"
//#include "common_print.h"

#define PRODUCT_NAME "WMG88"
#define STR_MODVER "modver"

/** BIT map
* bit[0 - 1] - NEZHA
* bit[2 - 3] - CRANE
*/
#define PSM_PLATFORM_TYPE (0x04)
#ifdef PLAT_NEZHAC
#undef PSM_PLATFORM_TYPE
#define PSM_PLATFORM_TYPE (0x01)
#endif

#define PSM_PLATFORM_NEZHA_DEFINED  ((PSM_PLATFORM_TYPE & 0x03) ? TRUE: FALSE)
#define PSM_PLATFORM_CRANE_DEFINED  ((PSM_PLATFORM_TYPE & (0x03 << 2)) ? TRUE: FALSE)

//#define PSM_DEBUG

typedef unsigned long   UINT32;

extern void *DiagMemPoolAlignMalloc(UINT32 size);
extern void DiagMemPoolAlignFree(void* alignAddress);

/*
#ifdef  PSM_DEBUG
#define PSM_Printf(fmt,args...)  CPUartLogPrintf(fmt, ##args)
#define PSM_Long_Printf(fmt,args...)	LPUartLogPrintf(fmt, ##args)
#else
#define PSM_Printf(fmt,args...)
#define PSM_Long_Printf(fmt,args...)
#endif
*/

//#define PSM_Printf(fmt,args...)	 {PSM_module_Printf(2,fmt, ##args);}
//#define PSM_Long_Printf(fmt,args...)	 {PSM_module_Long_Printf(2,fmt, ##args);}

#define PSM_LOG_DEBUG 3
#define PSM_LOG_CRITIC 2
#define PSM_LOG_ERROR 1

#if (defined(LTEONLY_THIN) || defined(CRANEL_FP_8MRAM) || defined(LTEGSM_THIN))
#define	PSM_LOG_LEVEL 2
#else
#define	PSM_LOG_LEVEL 3
#endif

#define PSM_Printf(fmt,args...) 	\
        {if(PSM_LOG_LEVEL >= PSM_LOG_DEBUG) CPUartLogPrintf("[psm]"fmt, ##args);}

#define PSM_Printf_Critic(fmt,args...) 	\
        {if(PSM_LOG_LEVEL >= PSM_LOG_CRITIC) CPUartLogPrintf("[psm]"fmt, ##args);}

#define PSM_Printf_Error(fmt,args...) 	\
        {if(PSM_LOG_LEVEL >= PSM_LOG_ERROR) CPUartLogPrintf("[psm]"fmt, ##args);}


/*
static inline void * psm_malloc(unsigned int size)
{
	return calloc(1, size);
	//return DiagMemPoolAlignMalloc(size);
}

#define psm_free(ptr) free((ptr))
*/

typedef struct qstr {
	unsigned int len;      /* length without the trailing ';' */
	unsigned char *name;
} qstr_t;

#define SUBSPACELEN 100
typedef struct psm_handle {
	char  *psmh_mod_name;   /* The module name */
	qstr_t psmh_env;       /* Structure to point to the allocated environment */
	void  *psmh_backend_info; /* The backend configuration */
	int    psmh_semid;        /* Semaphore id */
	short  psmh_dirty;        /* Buffer Dirty? */
	char   psmh_subspace[SUBSPACELEN];  /* The subspace */
	short  psmh_subspacelen;                 /* The active subspace len */
} psm_handle_t;

psm_handle_t * __psm_open(char *module_name, void *backend_hint, char handleIndex);
void psm_close(psm_handle_t *handle, char handleIndex);



#define PSM_CTL_CMD_ENCRYPT    1
#define PSM_CTL_CMD_COMPRESS   2

/* The set/get calls will take the module name and construct a proper
 * namespace before writing the variables to the disk
 */
int psm_set(psm_handle_t *handle, qstr_t *variable, qstr_t *value);
int psm_safe_set(psm_handle_t *handle, qstr_t *variable, qstr_t *value);
int psm_set_int(psm_handle_t *handle, qstr_t *variable, int value);


char *psm_get(psm_handle_t *handle, qstr_t *variable);
unsigned char *psm_getall(psm_handle_t *handle);
int psm_get_int(psm_handle_t *handle, qstr_t *variable);


int psm_eraseall(psm_handle_t *handle, void *backend_hint, char handleIndex);

int psm_commit(psm_handle_t *handle, char handleIndex);

int psm_push_subspace(psm_handle_t *handle, char *subspace);
int psm_pop_subspace(psm_handle_t *handle);

/* Returns a string modname.subspace.subspace.... */
char *psm_get_namespace(psm_handle_t *handle);




/**** Functions not exposed to the user ****/

int backend_read_env(struct qstr *env, void **backend_info, char *backend_hint, char handleIndex); /* backend_info is allocated in this function */
int backend_write_env(struct qstr *env, void *backend_info, char handleIndex);
void backend_free_env(struct qstr *env, void *backend_info); /* backend_info is destroyed in this function */
int backend_eraseall(struct qstr *env, void *backend_info, void *backend_hint, char handleIndex);
int backend_eraseall_recover(void *backend_hint, char handleIndex);

/***** Utility functions *****/
char *util_search_var(unsigned char *env, int size, qstr_t *variable);
int util_del_var(unsigned char *environment, int size, qstr_t *variable, int *delete_len);
#define PSM_FLAG_SAFE_SET    1   /* Do not allow NEW variable definitions */
int util_set_var(unsigned char *environment, int size, qstr_t *variable, qstr_t *value, short flags);
unsigned long util_get_actual_env_size(char *env);
void psm_test(void);
int util_del_var_value(unsigned char *environment, int size, qstr_t *variable, qstr_t *value,int *delete_len);



/****** Locking Functions ****/
/*
#ifdef PSM_USE_LOCKS
int psm_lock(int semid);
int psm_unlock(int semid);
int psm_lock_init();
#else // PSM_USE_SHM
#define psm_lock(x)             do { } while(0)
#define psm_unlock(x)           do { } while(0)
#define psm_lock_init()        -1
#endif // PSM_USE_SHM
*/

#endif /* ! _PSM_H_ */
