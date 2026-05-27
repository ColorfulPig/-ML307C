/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: XIRQ specific interface Header File                           */
/*                                                                      */
/* Filename: intc_xirq.h                                                */
/*                                                                      */
/* Author: Anton                                                        */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL/HOP                          */
/************************************************************************/
#ifndef _INTC_XIRQ_H_
#define _INTC_XIRQ_H_

#include "global_types.h"
#include "xirq_config.h"
#include "intc.h"

/*
 * The following functions are XIRQ-specific and do not belong to the generic INTC API
 * INTC_InterruptSources argument for these functions should be an XIRQ entry ID (not INTC enumeration)
 */
#if defined(SINTC_XIRQ) && !defined(SINTC_DOES_SRC_CONVERSION)
#define XIRQ_InterruptSources int
#else
#define XIRQ_InterruptSources INTC_InterruptSources
#endif

// Functions
INTC_ReturnCode INTCSwIntCrossAssert(XIRQ_InterruptSources isrSource);
void INTCXswiAssert(UINT16 prio);
INTC_ReturnCode INTCConfigureSwi(XIRQ_InterruptSources isrSource, INTC_FastNormal fastNormal,
                              INTC_ActivationType activationType,UINT8 priority);


INTC_ReturnCode INTCBindVirtual(UINT32 virtualIntAbsNum,INTC_ISR isr);
void INTCEdgeLevel(XIRQ_InterruptSources isrSource , BOOLEAN EdgeFl);
UINT32 get_int_status_0(void);
UINT32 get_int_status_1(void);
UINT32 get_int_status_2(void);
UINT32 get_int_status_3(void);
UINT32 INTCGetIntVirtualNum(UINT32 IntAbsNum);

/*
 * INTCIsInterruptPending.
 * Returns YES if any interrupts are pending and NO otherwise.
 */
#define IC_ACTIVE_IRQ_MASK_LOCAL 0x88UL /* The API h-file should not import all the HW definitions except for this */
#if 0
#define INTCIsInterruptPending() (((*(volatile UINT16*)(IRQCTRL_BASE_ADDR+IC_ACTIVE_IRQ_MASK_LOCAL))==0) ? NO : YES)
#endif
#endif // _INTC_XIRQ_H_
