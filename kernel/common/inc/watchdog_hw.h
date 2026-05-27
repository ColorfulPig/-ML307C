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
* Title: Watch Dog Timer Driver Header
*
* Filename: watchdog_hw.h
*
* Target, platform: WhiteSail
*
* Authors:	Eilam Ben-Dror
*
* Description: This file is the WATCHDOG driver's hardware header file
*
* Last Updated:
*
* Notes:
*******************************************************************************/

#ifndef _WATCHDOG_HW_H_
#define _WATCHDOG_HW_H_

//Some include files defines its own SET/CLEAR bit
// But here set/clear requires also the access enabling
// So make sure to undefine
#undef  SET_BIT
#undef  CLR_BIT

extern UINT32 WDT_USE_CP_TIMER;

//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)

#define WATCHDOG_HW_BASE_ADDRESS     0xD4080000UL

#else
#define WATCHDOG_HW_BASE_ADDRESS     0xF00C0030L      // GB Timers base address
#endif
#else
#if defined (SILICON_TTC)
#define WATCHDOG_HW_BASE_ADDRESS     0xD4080000UL  //0xD403A000UL
#else /* Boerne/Hermon*/
#define WATCHDOG_HW_BASE_ADDRESS     0x40A00000L
#endif //_SILICON_TTC_
#endif //_TAVOR_HARBELL_
//Watchdog Registers Definitions:
typedef struct WATCHDOGControllerTAG
{
	volatile UINT32		 tccr;				   /* R/W			Timer Clock Control Reg */
	volatile UINT32      tmr[3][3];  /* R/W 		  3-match Reg per timer */
	volatile UINT32		 tcr[3];  /* Read Only	  Timer Count Reg */
	volatile UINT32		 tsr[3];  /* Read only	  Timer Status Register */
	volatile UINT32		 tier[3];  /* R/W 		  Timer Interrupt Enable Reg */
	volatile UINT32		 tplvr[3];  /* R/W 		  Timer Preload Value Reg */
	volatile UINT32		 tplcr[3]; /* R/W 		  Timer Preload Control Reg */
    UINT32      TWMER ;         // Watchdog Match Enable Register       R/W
    UINT32      TWMR  ;         // Watchdog Match Register              R/W
    UINT32      TWVR  ;         // Watchdog Value Register              R
    UINT32      TWSR  ;         // Watchdog Status Register             R/W
	volatile UINT32		 ticr[3];  /* Write only	  Timer Interrupt Clear Reg */
	UINT32      TWICR ;         // Watchdog Interrupt Clear Register    W
	volatile UINT32		 tcer;				   /* R/W			Timer Count Enable Reg */
	volatile UINT32		 tcmr;				   /* R/W			Timer Count Mode Reg */
	volatile UINT32		 tilr[3];  /* R/W 		  Timer Interrupt Mode (Level, Edge) */
    UINT32      TWCR  ;         // Watchdog Counter Reset Register      W
    UINT32      TWFAR ;         // Watchdog First Access Register       W
    UINT32      TWSAR ;         // Watchdog Second Access Register      W
	volatile UINT32		 tcvwr[3];  /* R/W 		  Timer value wr-rd request */
} WATCHDOGController;



//#define     WATCHDOGRegisters    (*(volatile WATCHDOGController *)WATCHDOG_HW_BASE_ADDRESS)
volatile WATCHDOGController *   pWATCHDOGRegisters = (volatile WATCHDOGController *)WATCHDOG_HW_BASE_ADDRESS;
#define  WATCHDOGRegisters    (*pWATCHDOGRegisters)

#define WATCHDOG_MAX_MATCH_TIME_MS  (WATCHDOG_MAX_MATCH_VALUE / WATCHDOG_CLOCK_RATE * WATCHDOG_CLOCK_FACTOR)

//  HERMON and HARBEL have didicated watchdog timer working with verry similar logic.
//  But BOERNE watchdog is different and like to regular timer.
//  Let's treat boernie's macros differently
//---------------------------------------------------------------------------------------
#if defined (_TAVOR_BOERNE_)
//---------------------------------------------------------------------------------------

//Clock rate 3.25MHz and max match value
#define WATCHDOG_CLOCK_RATE           3250
#define WATCHDOG_CLOCK_FACTOR          1
#define WATCHDOG_MAX_MATCH_VALUE      0xFFFFFFFE
#define WATCHDOG_MIN_MATCH_VALUE    ((8/*mS*/*WATCHDOG_CLOCK_RATE) / WATCHDOG_CLOCK_FACTOR)

// Reset masks
#define WATCHDOG_RESET_MASK               0x1L
#define WATCHDOG_COUNT_MASK               WATCHDOG_RESET_MASK
#define WATCHDOG_WRITE_ACCESS_ENABLE

// Keep match value buffer. Do not use "static". Linker will check it is defined once only.
UINT32 watchdogMatchValue = WATCHDOG_MAX_MATCH_VALUE;
UINT32 watchdogResetMode  = 0;

// Enable WDT reset (No INT, only reset supported. Once started can't be stopped)
#define WATCHDOG_RESET_ENABLE         {watchdogResetMode |= WATCHDOG_RESET_MASK;}
#define WATCHDOG_INTERRUPT_ENABLE     {watchdogResetMode &=~WATCHDOG_RESET_MASK;}
#define WATCHDOG_MODE_GET             (watchdogResetMode == WATCHDOG_RESET_MASK)
                                     //Currently only RESET mode is supported for running
#define WATCHDOG_COUNT_START         {if(watchdogResetMode == WATCHDOG_RESET_MASK) \
                                       { WATCHDOGRegisters.OSMR3 = WATCHDOGRegisters.OSCR0 +watchdogMatchValue; \
                                       WATCHDOGRegisters.OWER |= WATCHDOG_COUNT_MASK; } }

#define WATCHDOG_COUNT_STOP            /*Can't be done for RESET. Do nothing*/

// Set match value:
#define WATCHDOG_MATCH_VALUE_SET(m)    { watchdogMatchValue= ( ((m) < WATCHDOG_MIN_MATCH_VALUE) ? WATCHDOG_MIN_MATCH_VALUE : (m) ); }

// Restart counter:
#define WATCHDOG_HW_KICK               { WATCHDOGRegisters.OSMR3 = WATCHDOGRegisters.OSCR0 +watchdogMatchValue; }

// Get match value:
#define WATCHDOG_MATCH_VALUE_GET(m)         ((m) = WATCHDOGRegisters.OSMR3)

// Get current counter value:
#define WATCHDOG_COUNT_VALUE_GET(v)         ((v) = WATCHDOGRegisters.OSCR0)

#define WATCHDOG_RESET_INDICATION_GET(i)    ((i) = 0)
#define WATCHDOG_RESET_INDICATION_CLEAR
/// INT not supported and the INTC_SRC_WDT may be irrelevant
#define WATCHDOG_INTERRUPT_CLEAR
//#define INTCBind(INTC_SRC_WDT,watchdogHandler)      INTC_RC_OK

//---------------------------------------------------------------------------------------
#else //_TAVOR_BOERNE_
//---------------------------------------------------------------------------------------

// Register Masks:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_RESET_MASK             0x2L
#define WATCHDOG_INTERRUPT_MASK         0x01L
#define WATCHDOG_COUNT_MASK             0x1L
#else
/* Mask for mode   reset/int  is WER[1]= 1/0
 * Mask for count Enable/Disable WER[0]= 1/0
*/
#define WATCHDOG_RESET_MASK             0x2L
#define WATCHDOG_COUNT_MASK             0x1L
#endif
#else
#define WATCHDOG_RESET_MASK             0x2L
#define WATCHDOG_INTERRUPT_MASK         0x01L
#define WATCHDOG_COUNT_MASK             0x1L
#endif



// First access key:
#define WATCHDOG_1ST_ACCESS_KEY         0xBABA

// Second access key:
#define WATCHDOG_2ND_ACCESS_KEY         0xEB10

// WDT clock rate:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)

//extern UINT32 WDT_USE_CP_TIMER

#define WATCHDOG_CLOCK_RATE      (WDT_USE_CP_TIMER ? 32768 : 256)                  // Hz

#define WATCHDOG_CLOCK_FACTOR           1000        // Hz -> KHz

#else
#define WATCHDOG_CLOCK_RATE             8           // Hz        => 128mS
#define WATCHDOG_CLOCK_FACTOR           1000        // Hz -> KHz
#endif
#else
#define WATCHDOG_CLOCK_RATE             256         // Hz
#define WATCHDOG_CLOCK_FACTOR           1000        // Hz -> KHz
#endif

#define WATCHDOG_MAX_MATCH_VALUE        (WDT_USE_CP_TIMER ? 0xFFFFFFFF : 0x0000FFFF)

// Macro Definitions:

//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
// Read WER register always disables write access to it.
// So the bit operation like  WER |= bitX  cannot execute writing.
// Use this with Reade->BitOp->WrAccessEna->Write
#define SET_BIT(rEG,mASK)   {UINT32 v=rEG; v|= mASK;  WATCHDOG_WRITE_ACCESS_ENABLE;  rEG=v; }
#define CLR_BIT(rEG,mASK)   {UINT32 v=rEG; v&=~mASK;  WATCHDOG_WRITE_ACCESS_ENABLE;  rEG=v; }
#endif


#define WATCHDOG_INITIALIZATION             { WATCHDOGRegisters.tccr &= ~(0x3 << 5); \
			                                  WATCHDOGRegisters.tccr |= (0x2 << 5); \
   			                                  WATCHDOGRegisters.tplcr[2] = 0; \
			                                  WATCHDOGRegisters.tplvr[2] = 0; \
			                                  WATCHDOGRegisters.tcmr |= (1<<2); }


// Enable write access to WDT registers:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)

#define WATCHDOG_WRITE_ACCESS_ENABLE {  if(!WDT_USE_CP_TIMER)                                   \
										{                                                      \
											WATCHDOGRegisters.TWFAR = WATCHDOG_1ST_ACCESS_KEY; \
											WATCHDOGRegisters.TWSAR = WATCHDOG_2ND_ACCESS_KEY; \
										}													   \
}
#else
#define WATCHDOG_WRITE_ACCESS_ENABLE        {WATCHDOGRegisters.WFAR = WATCHDOG_1ST_ACCESS_KEY; \
                                             WATCHDOGRegisters.WSAR = WATCHDOG_2ND_ACCESS_KEY;}
#endif
#else
#define WATCHDOG_WRITE_ACCESS_ENABLE        {WATCHDOGRegisters.TWFAR = WATCHDOG_1ST_ACCESS_KEY; \
                                             WATCHDOGRegisters.TWSAR = WATCHDOG_2ND_ACCESS_KEY;}
#endif

// Enable WDT reset
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_RESET_ENABLE       { if(WDT_USE_CP_TIMER)                              \
										{	                                             \
											;     \
										}\
										else                                             \
										{   WATCHDOG_WRITE_ACCESS_ENABLE;   \
											WATCHDOGRegisters.TWMER |= WATCHDOG_RESET_MASK; \
										}\
}
#else
#define WATCHDOG_RESET_ENABLE	     SET_BIT(WATCHDOGRegisters.WER , WATCHDOG_RESET_MASK)
#endif
#else
#define WATCHDOG_RESET_ENABLE               {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWMER |= WATCHDOG_RESET_MASK;}
#endif

// Disable WDT reset, enable interrupt
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_INTERRUPT_ENABLE           {  if(WDT_USE_CP_TIMER)\
												{\
													WATCHDOGRegisters.tier[2] = 0x1;\
												}\
												else\
												{\
													WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             		WATCHDOGRegisters.TWMER &= ~WATCHDOG_INTERRUPT_MASK;\
												}\
											}

#define WATCHDOG_INTERRUPT_DISABLE          {  if(WDT_USE_CP_TIMER)\
												{\
													WATCHDOGRegisters.tier[2] = 0x0;\
												}\
											}

#else
#define WATCHDOG_INTERRUPT_ENABLE    CLR_BIT(WATCHDOGRegisters.WER , WATCHDOG_RESET_MASK)
#endif
#else
#define WATCHDOG_INTERRUPT_ENABLE           {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWMER &= WATCHDOG_INTERRUPT_MASK;}
#endif

// Retrieve the mode: reset / interrupt
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined(PHS_SW_DEMO_TTC)
#define WATCHDOG_MODE_GET                  (WDT_USE_CP_TIMER ? 0 : ((WATCHDOGRegisters.TWMER & WATCHDOG_RESET_MASK)>>1))       //WATCHDOG_INTERRUPT_MODE
#else
#define WATCHDOG_MODE_GET		            ((WATCHDOGRegisters.WER   & WATCHDOG_RESET_MASK)>>1)
#endif
#else
#define WATCHDOG_MODE_GET                   ((WATCHDOGRegisters.TWMER & WATCHDOG_RESET_MASK)>>1)
#endif


//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined(PHS_SW_DEMO_TTC)
#define WATCHDOG_COUNT_STATUS_GET(v)                   { if(WDT_USE_CP_TIMER)\
														  {\
															(v) = ((WATCHDOGRegisters.tcer >> 2) & 0x1);\
														  }\
														  else\
														  {\
														  	(v) = (WATCHDOGRegisters.TWMER & 0x1);\
														  }\
}
#else
#define WATCHDOG_COUNT_STATUS_GET(v)		            ((v) = (WATCHDOGRegisters.WER   & 0x1))
#endif
#else
#define WATCHDOG_COUNT_STATUS_GET(v)                   ((v) = (WATCHDOGRegisters.TWMER & 0x1))
#endif


// Start counter:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_COUNT_START                {             \
\
if(WDT_USE_CP_TIMER)\
{\
	WATCHDOGRegisters.tcer |= (1<<2);\
}\
else\
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;     \
    WATCHDOGRegisters.TWMER |= WATCHDOG_COUNT_MASK;\
}\
}
#else
#define WATCHDOG_COUNT_START         SET_BIT(WATCHDOGRegisters.WER , WATCHDOG_COUNT_MASK)
#endif
#else
#define WATCHDOG_COUNT_START                {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWMER |= WATCHDOG_COUNT_MASK;}
#endif

// Stop counter
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_COUNT_STOP                 {\
if(WDT_USE_CP_TIMER)  \
{\
	WATCHDOGRegisters.tcer &= ~(1<<2);\
}\
else  \
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;	  \
	WATCHDOGRegisters.TWMER &= ~WATCHDOG_COUNT_MASK;\
}\
}
#else
#define WATCHDOG_COUNT_STOP          CLR_BIT(WATCHDOGRegisters.WER , WATCHDOG_COUNT_MASK)
#endif
#else
#define WATCHDOG_COUNT_STOP                 {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWMER &= ~WATCHDOG_COUNT_MASK;}
#endif

// Restart counter:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_HW_KICK                {\
if(!WDT_USE_CP_TIMER)\
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;     \
    WATCHDOGRegisters.TWCR = 1;       \
}\
}

#else
//#define WATCHDOG_HW_KICK_HARBELL    {WATCHDOG_WRITE_ACCESS_ENABLE; WATCHDOGRegisters.WCRR = 1;}
#define WATCHDOG_HW_KICK                     /*do nothing*/
#endif
#else
#define WATCHDOG_HW_KICK                    {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWCR = 1;}
#endif

// Set match value:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_MATCH_VALUE_SET(m)         {   \
if(WDT_USE_CP_TIMER)\
{\
	WATCHDOGRegisters.tmr[2][0] = (m);\
}\
else\
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;	  \
	WATCHDOGRegisters.TWMR = (m);    \
}\
}
#else
#define WATCHDOG_MATCH_VALUE_SET(m)         {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.WTPR = (m);}
#endif
#else
#define WATCHDOG_MATCH_VALUE_SET(m)         {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWMR = (m);}
#endif

// Get match value:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_MATCH_VALUE_GET(m)         {\
if(WDT_USE_CP_TIMER)\
{\
	(m) = WATCHDOGRegisters.tmr[2][0];\
}\
else\
{\
	(m) = WATCHDOGRegisters.TWMR;\
}\
}

#else
#define WATCHDOG_MATCH_VALUE_GET(m)         ((m) = WATCHDOGRegisters.WTPR)
#endif
#else
#define WATCHDOG_MATCH_VALUE_GET(m)         ((m) = WATCHDOGRegisters.TWMR)
#endif

// Get current counter value:
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_COUNT_VALUE_GET(v)         {   \
if(WDT_USE_CP_TIMER)\
{\
	volatile unsigned long tmp;                     \
	for(tmp = WATCHDOGRegisters.tcr[2]; ((v) = WATCHDOGRegisters.tcr[2]) != tmp; tmp = (v));\
}\
else\
{	\
	(v) = WATCHDOGRegisters.TWVR;\
}\
}

#else
#define WATCHDOG_COUNT_VALUE_GET(v)	        ((v) = WATCHDOGRegisters.WCR)
#endif
#else
#define WATCHDOG_COUNT_VALUE_GET(v)         ((v) = WATCHDOGRegisters.TWVR)
#endif


// Get WDT reset indication
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_RESET_INDICATION_GET(i)    (\
if(WDT_USE_CP_TIMER)\
{\
	(i) = (BOOL)0x0);\
}\
else\
{\
	(i) = (BOOL)WATCHDOGRegisters.TWSR;\
}\
}
#else
// CPMUGetLastResetReason()....
#endif
#else
#define WATCHDOG_RESET_INDICATION_GET(i)    ((i) = (BOOL)WATCHDOGRegisters.TWSR)
#endif

// Clear WDT reset indication
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_RESET_INDICATION_CLEAR   {\
if(!WDT_USE_CP_TIMER)\
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;	   \
	WATCHDOGRegisters.TWSR = 0; \
}\
}
#else
//#define WATCHDOG_RESET_INDICATION_CLEAR   CPMUClearLastResetReason()
#endif
#else
#define WATCHDOG_RESET_INDICATION_CLEAR     {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWSR = 0;}
#endif


// Clear WDT interrupt
//#if defined (_TAVOR_HARBELL_) || defined(SILICON_PV2)
#if defined(SILICON_PV2)
#if defined (PHS_SW_DEMO_TTC)
#define WATCHDOG_INTERRUPT_CLEAR            {\
if(WDT_USE_CP_TIMER)\
{\
	WATCHDOGRegisters.ticr[2] = 0x7;\
}\
else\
{\
	WATCHDOG_WRITE_ACCESS_ENABLE;     \
    WATCHDOGRegisters.TWICR = 1; \
}\
}
#else
#define WATCHDOG_INTERRUPT_CLEAR            INTCClrSrc(INTC_HW_TmrWDT); WATCHDOG_COUNT_STOP;
#endif
#else
#define WATCHDOG_INTERRUPT_CLEAR            {WATCHDOG_WRITE_ACCESS_ENABLE;     \
                                             WATCHDOGRegisters.TWICR = 1;}
#endif

#endif//_TAVOR_BOERNE_

#if !defined(SILICON_TTC)
#define PRMReleaseWdgFromReset()
#endif
#endif     //_WATCHDOG_HW_H_
