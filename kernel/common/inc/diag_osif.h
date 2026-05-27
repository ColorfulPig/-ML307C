/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _DIAG_OSIF_H_
#define _DIAG_OSIF_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "osa.h"
#ifdef __cplusplus
}
#endif

#define DIAG_UNUSED_PARAM(param) (void)param;

// specific OS defines
#if defined (OSA_NUCLEUS)
#include "asserts.h"
#include "timer.h"
#include "timer_config.h"
#endif

#if defined (OSA_LINUX)
#include "assert.h"
#endif//defined (OSA_LINUX)

// specific OS (OSA_NUCLEUS) interface defines
#if defined (OSA_NUCLEUS)
// Diag has special define for usage of UART (_DIAG_USE_COMMSTACK_), SSP (_DIAG_USE_SSP_), Virtio (_DIAG_USE_VIRTIO_ (assumes SSP)), MSL (DIAG_OVER_MSL). USB is the default.
// Not all cores with diag, has all APIs of each type of interface, thus we need to set a define 
// for the USB , so coede of the 'EXTif' will compile on different platforms/cores

#endif

#if defined (OSA_WINCE)
	#define DIAG_WM_CONSOLE_PRN(X) NKDbgPrintfW X
#else
	#define DIAG_WM_CONSOLE_PRN(X)
#endif

#if !defined (DIAG_DEFUALT_PRIORITY)
	#if 0//defined (OSA_NUCLEUS)
	/*-----------------3/16/2005 17:50------------------
	 * Boaz Sommer:
	 * The following code is "ugly" and should be replaced
	 * with code that defines DIAG task priority in a system
	 * file.
	 * --------------------------------------------------*/
		#if	OSA_LOWEST_PRIORITY > 254
			#define DIAG_DEFUALT_PRIORITY			42	// When OS lowesr priority is larger than 254  --> Hermon C7
		#else
			#define DIAG_DEFUALT_PRIORITY			25	// When OS lowesr priority is 31
		#endif
	#else	//NUCLEUS
		#define DIAG_DEFUALT_PRIORITY			(42)
	#endif	//OSA_NUCLEUS
#endif	//DIAG_DEFUALT_PRIORITY

// align code on byte boundary for nbytes , ASSERT define etc.
/*
The macro IS_DIAG_BLOCKED translates to checking of global variable in Nucleus, to have 
less impact on runtime when ACAT is not connected (operational, production, field testing)
In WinCE (and Linux?), the usage of global is a bit difficult due to the required import/export
definition needed from different dll's. Currently this is left with a function call (more instructions
are performed before finding out no trace should be sent)
*/

#if defined (OSA_NUCLEUS)
	#define DIAG_ALIGN(nbytes)	__align(nbytes)
	#define INCLUDE_ASSERT_OR_DUMMY	"asserts.h"
	#define DIAG_ASSERT(condition)	ASSERT(condition)
	#define IS_DIAG_BLOCKED	diagBlocked.all
#endif

// Time stamp for traces
#if defined (OSA_NUCLEUS)
//This should be set to the slow clock timer: (32KHz)
	#define GET_TIMESTAMP 	diag_get_32k_tick()
//	#define GET_TIMESTAMP 	timerCountRead(TS_TIMER_ID)
#endif


/* The DIAG-PRINT is using system calls
 * and therefore should never be called from interrupt context.
 * The wrong using may be found only at run-time.
**/
#define DIAG_CHECK_CONTEXT_IS_NOT_INT_ENABLE

#ifdef  DIAG_CHECK_CONTEXT_IS_NOT_INT_ENABLE
	#ifdef OSA_NUCLEUS
		#ifdef PLAT_TEST
		#define DIAG_CHECK_CONTEXT_IS_NOT_INT	 {DIAG_ASSERT(OS_INTERRUTPT_COUNT==0);}
		#else
		#define DIAG_CHECK_CONTEXT_IS_NOT_INT    {extern volatile UINT32 TCD_Interrupt_Count;  DIAG_ASSERT(TCD_Interrupt_Count==0);}
		#endif
	#else
		#define DIAG_CHECK_CONTEXT_IS_NOT_INT
	#endif//OSA_NUCLEUS
#else
	#define DIAG_CHECK_CONTEXT_IS_NOT_INT
#endif



////////////////////  Extern definitions //////////////////

	#ifdef __cplusplus
		#define DIAG_EXPORT extern "C"
		#define DIAG_IMPORT extern "C"
	#else
		#define DIAG_EXPORT extern
		#define DIAG_IMPORT extern
	#endif //__cplusplus


//Macro for trace declaration (WinCE taken from diagdb.h)
#if (defined (OSA_NUCLEUS) || defined (OSA_LINUX))
	#define DIAG_FILTER_EXTERN(cat1, cat2, cat3) extern int __trace_##cat1##__##cat2##__##cat3(void)
#endif



////////////////////////////////////////
//Diag BSP Configuration
////////////////////////////////////////



typedef enum
{
    DEFAULT_EXT_IF_TX=0,
	MULTI_EXT_IF_TX,
	SINGLE_EXT_IF_TX,
	
	//Keeping old enum for backward compatibility. Changing the names to specify it's the USB interface.
	USB_EXT_IF_TX_DEFAULT=DEFAULT_EXT_IF_TX,
	USB_EXT_IF_TX_MULTI  =MULTI_EXT_IF_TX,
	USB_EXT_IF_TX_SINGLE =SINGLE_EXT_IF_TX,

	DUMMY_EXT_IF_TX=0x1FFFFFFF //To enforce UINT32 size
} e_extIfTxMulti;

typedef struct
{
	//Don't change the already used parameters
	e_extIfTxMulti extIfTxMulti;
	//You can always extend this structure. Zero stands for default configuration.
	//...
} s_diagBSPconfig;

extern UINT32 diagBSPconfig(s_diagBSPconfig *cfg);
////////////////////////////////////////
////////////////////////////////////////

//function prototypes
DIAG_EXPORT UINT32	diag_lock(UINT32 *cpsr_ptr);
DIAG_EXPORT void	diag_unlock(UINT32 cpsr_val);
DIAG_EXPORT void	diag_lock_G(void);
DIAG_EXPORT void	diag_unlock_G(void);
DIAG_EXPORT UINT32	osTimeClockRateGet(void);
extern void diagOSspecificPhase1Init(void);
extern void diagOSspecificPhase2Init(void);
#endif	 // _DIAG_OSIF_H_

