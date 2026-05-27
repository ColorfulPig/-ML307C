/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#if !defined(XIRQ_CONFIG_H)
#define       XIRQ_CONFIG_H

#define NUM_SWI               16
#define NUM_RI                8
#define NUM_XSWI              1
#define NUM_EIRQ              3
/*Fix coverity[overrun-call]*/
#if defined (EDEN_1928) || defined (NEZHA3_1826)
#define MAX_NUM_HWI           77
#else
#define MAX_NUM_HWI           64
#endif
#define NUM_HWI_REGS          ((MAX_NUM_HWI + 15)/16)

#define SWI0_ID               0
#define RI0_ID                (SWI0_ID + NUM_SWI)
#define XSWI_ID               (RI0_ID + NUM_RI)
#define EIRQ0_ID              (XSWI_ID + NUM_XSWI)
#define SWI_EIRQ_ID           (EIRQ0_ID)
#define RI_EIRQ_ID            (EIRQ0_ID + 1)
#define XSWI_EIRQ_ID          (EIRQ0_ID + 2)
#define HWI0_ID               (EIRQ0_ID + NUM_EIRQ)

#define MAX_NUM_IRQ           (NUM_SWI + NUM_RI + NUM_XSWI + NUM_EIRQ + MAX_NUM_HWI)

//#if defined(_TAVOR_HARBELL_) || defined (SILICON_PV2)
#if defined (SILICON_PV2)
#ifdef PHS_SW_DEMO_TTC  //20080828
#include "intc_config.h"
/* Dummy base address for simulated interrupt controller. */
#if defined (EDEN_1928) || defined (NEZHA3_1826)
#define IRQCTRL_BASE_ADDR     0xD4041000//0xD0231000////pei changed
#else
gg
#define IRQCTRL_BASE_ADDR     0xD4031000
#endif
#define INTC_CONFIGURE_WITH_3_ARGUMENTS // Matches the SINTC definition, see sintc.h
#define XIRQ_STICKY_OFFSET 0x8f8
#define XIRQ_EDGELEVEL_OFFSET 0x8f0
#else
/* Dummy base address for simulated interrupt controller. */
#define IRQCTRL_BASE_ADDR     0xD4000800
#define INTC_CONFIGURE_WITH_3_ARGUMENTS // to be used for API mismatch identification until resolved
#define XIRQ_D2_SPECIFIC
#define XIRQ_STICKY_OFFSET 0x500
#define XIRQ_EDGELEVEL_OFFSET 0x508
#endif   //PHS_SW_DEMO_TTC 
#endif

#if defined(SILICON_TTC)
#include "intc_config.h"
/* Dummy base address for simulated interrupt controller. */
#define IRQCTRL_BASE_ADDR     0xD4031000
#define INTC_CONFIGURE_WITH_3_ARGUMENTS // Matches the SINTC definition, see sintc.h
#define XIRQ_STICKY_OFFSET 0x8f8
#define XIRQ_EDGELEVEL_OFFSET 0x8f0
#endif

#endif // XIRQ_CONFIG_H
