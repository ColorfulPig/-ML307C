/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: Interrupt Controller Sources (including GPIO)                 */
/*                                                                      */
/* Filename: Intc_list.h                                                */
/*                                                                      */
/* Author: Alexander Rivman                                              */
/*         Eilam Ben-Dror                                               */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/
#ifndef _INTC_LIST_XIRQ_H_
#define _INTC_LIST_XIRQ_H_

////////////////////////////// 20080823 ///////////////////
#if 0
#include "xirq_config.h"  // don't #include "arbelirq.h" - HW specifics
typedef enum
{
	/* S/W interrupts , initiated by TCU Module , 16 interrupts  */
	SW_INT_SRC_0 = SWI0_ID,
	SW_INT_SRC_1    ,
	SW_INT_SRC_2    ,
	SW_INT_SRC_3    ,
	SW_INT_SRC_4    ,
	SW_INT_SRC_5    ,
	SW_INT_SRC_6    ,
	SW_INT_SRC_7    ,
	SW_INT_SRC_8    ,
	SW_INT_SRC_9    ,
	SW_INT_SRC_10    ,
	SW_INT_SRC_11    ,
	SW_INT_SRC_12    ,
	SW_INT_SRC_13    ,
	SW_INT_SRC_14    ,
	SW_INT_SRC_15    ,
	/* Internal Interrupts (or register interrupts ) [16:23] , 8 possible sources*/
	FirstRI_INT = RI0_ID,
	RI_INT_SRC_0  = FirstRI_INT ,
	RI_INT_SRC_1     ,
	RI_INT_SRC_2     ,
	RI_INT_SRC_3     ,
	RI_INT_SRC_4     ,
	RI_INT_SRC_5     ,
	RI_INT_SRC_6     ,
	RI_INT_SRC_7     ,
	/* Cross-connection interrupt*/
	XSWI_INT_SRC_0,
	/* iNTERNAL EXCEPTION 3 INTERRUPTS*/
	EIRQ_INT_0  ,
	EIRQ_INT_1  ,
	EIRQ_INT_2  ,
	/* HW interrupts - sourced by GB peripherals (including the Modem) 48 possible interrupts*/
	FIRST_HW_INT = HWI0_ID ,
	INTC_HW_DMA_C3_0 = FIRST_HW_INT,    /*  DMA C3 [0] - HW IRQ [0] */
	INTC_HW_DMA_C3_1 ,    /*  DMA C3 [1] - HW IRQ [1] */
	INTC_HW_DMA_C3_2 ,	  /*  DMA C3 [2] - HW IRQ [2] */
	INTC_HW_DMA_C3_3 ,	  /*  DMA C3 [3] - HW IRQ [3] */
	INTC_HW_DMA_C3_4 ,	  /*  DMA C3 [4] - HW IRQ [4] */
	INTC_HW_DMA_C3_5 ,	  /*  DMA C3 [5] - HW IRQ [5] */
	INTC_HW_DMA_C3_6 ,	  /*  DMA C3 [6] - HW IRQ [6] */
	INTC_HW_DMA_C3_7 ,	  /*  DMA C3 [7] - HW IRQ [7] */
	INTC_HW_DMA_C2_0 ,	  /*  DMA C2 [0] - HW IRQ [8] */

	INTC_HW_DSPP_3 ,	  /*  DMA C2 [0] - HW IRQ [9] */

	INTC_HW_DMA_C2_2 ,	  /*  DMA C2 [2] - HW IRQ [10] */
	INTC_HW_DMA_C2_3 ,	  /*  DMA C2 [3] - HW IRQ [11] */
	INTC_HW_DMA_C2_4 ,	  /*  DMA C2 [4] - HW IRQ [12] */
	INTC_HW_DMA_C2_5 ,	  /*  DMA C2 [5] - HW IRQ [13] */
	INTC_HW_DMA_C2_6 ,	  /*  DMA C2 [6] - HW IRQ [14] */
	INTC_HW_DMA_C2_7 ,	  /*  DMA C2 [7] - HW IRQ [15] */
	INTC_HW_SLOW_CLK_SNAP ,    /*  Slow clock interrupt  HW IRQ [16] */
	INTC_HW_EarlyWKUP,    /*  Early wakeup interrupt From GSM slow clock HW IRQ [17]*/
	INTC_HW_GSSP_1   ,    /*  GSSP1      - HW IRQ [18] */
	INTC_HW_GSSP_2   ,    /*  GSSP1      - HW IRQ [19] */
	INTC_HW_RESERVED ,    /*   HW IRQ [20] */
	INTC_HW_MSL      ,    /*  MSL        - HW IRQ [21]*/
	INTC_HW_EXTPAD_1 ,    /*  External 1 from PAD  - HW IRQ [22]*/
	INTC_HW_EXTPAD_2 ,    /*  External 2 from PAD  - HW IRQ [23]*/
	INTC_HW_USIM     ,    /*  USIM      - HW IRQ [24]*/
	INTC_SRC_USIM=INTC_HW_USIM, /*alias*/
	INTC_HW_I2C      ,    /* I2C       - HW IRQ [25]*/
	INTC_SRC_I2C=INTC_HW_I2C, /*alias*/

    INTC_SRC_IPC_DATA_ACK,          /* IPC 0  IPC_CPU_DATA_Buffer_ACK (or of 13 intrpts)  - HW IRQ [26] */
//    INTC_HW_IpcChnlAck    ,      /* IPC 1  IPC_CPU_DATA_Channel_ACK(or of 13 intrpts)  - HW IRQ [26]*/
//    INTC_HW_IpcRd         ,      /* IPC 2  IPC CPU_RD_INT                              - HW IRQ [28]*/
//    INTC_HW_IpcCPUAck     ,      /* IPC 3  IPC CPU_ACK_INT                             - HW IRQ [29]*/
//    INTC_HW_IpcGP         ,      /* IPC 4  IPC CPU_GP_INT                              - HW IRQ [30]*/

    INTC_HW_IpcDatBuffAck ,       /* IPC 1  IPC_CPU_DATA_Channel_ACK(or of 13 intrpts)  - HW IRQ [27]*/
    INTC_SRC_IPC_RD,              /* IPC 2  IPC CPU_RD_INT                              - HW IRQ [28]*/
    INTC_SRC_IPC_ACK,             /* IPC 3  IPC CPU_ACK_INT                             - HW IRQ [29]*/
    INTC_SRC_IPC_GP,              /* IPC 4  IPC CPU_GP_INT                              - HW IRQ [30]*/




	INTC_HW_CPMU     ,    /* CPMU            - HW IRQ [31] */
	INTC_HW_TCU_EXCP ,    /* TCU exception   - HW IRQ [32] */
	INTC_HW_RTU_0    ,    /* RTU 0 - HW IRQ [33]*/
	INTC_HW_RTU_1    ,    /* RTU 1 - HW IRQ [34]*/
	INTC_HW_RTU_2    ,    /* RTU 2 - HW IRQ [35]*/

	INTC_HW_Tmr1     ,    /* HW IRQ [36] Timer1 */
    INTC_HW_Tmr2     ,    /* HW IRQ [37] Timer2 */
    INTC_HW_Tmr3BkUp ,    /* HW IRQ [38] Timer3 Backup */
    INTC_HW_Tmr4BkUp ,    /* HW IRQ [39] Timer4 Backup */
    INTC_HW_TmrWDT   ,    /* HW IRQ [40] Timer Watch Dog */
	INTC_HW_ECIPHER  ,    /* HW IRQ [41] Ecipher */
	INTC_HW_TCU_GPOB ,    /* HW IRQ [42] TCU GPOB[1] Future development */
	INTC_HW_ADPC     ,    /* HW IRQ [43] ADPC */
	INTC_HW_END_SLEEP_BKUP ,    /*HW IRQ [44] End Sleep Backup from WB sleep module.*/
    INTC_HW_TCU_CLK_EN   ,     /*HW IRQ [45] APBT_TCU_clk_en_int From APBT to signal that ARM can write to TCU FIFO after D2 exit.*/
    INTC_HW_CL_GB_HINT ,  /*HW IRQ [46] c1_gb_hint Write to illegal memory in ARM.*/
    INTC_SRC_CIPHER ,   /* HW IRQ [47] WB-Cipher Needs sticky bit*/
	INTC_BOERNE_CLK_CHG,			/* HW [48] Boerne MSL clock change*/
	INTC_GSM_WAKE_UP,				/* HW [49] GSM wakeup interrupt */
	INTC_WBCDMA_WAKE_UP,			/* HW [50] WBCDMA wakeup interrupt*/
	INTC_CRXD_32_28_EVENT,			/* HW [51] “CRXD?+ “CGPIO_REG[32:28]?wakeup event*/
	INTC_CRXD_27_0_EVENT,			/* HW [52] “CRXD?+ “CGPIO_REG[32:28]?wakeup event*/
	INTC_GSSP1_WAKE_UP,				/* HW [53] “GSSP1_CLK, GSSP1_FRM,GSSP1_RX?wakeup event*/
	INTC_GSSP2_WAKE_UP,				/* HW [54] “GSSP2_CLK, GSSP1_FRM,GSSP2_RX?wakeup event*/
	INTC_GSIM_UDET_UIO,				/* HW [55] “GSIM_UDET, GSIM_UIO”wakeup event*/
	INTC_CI2C_SCL_SDA_WAKE_UP,		/* HW [56] CI2C_SCL, CI2C_SDA*/
	INTC_COMM_UART,					/* HW [57] Comm UART Interrupt*/
	INTC_GB_DMA_C2,					/* HW [58] GB DMA C2 [1]*/
	INTC_AC_IPC_0,					/* HW [59] AC - IPC [0]*/
	INTC_D2_MODEM_OK,				/* HW [60] D2 - modem entry OK*/
	INTC_AC_IPC_1,					/* HW [61] AC - IPC [1]*/
	INTC_AC_IPC_2,					/* HW [62] AC - IPC [2]*/
	INTC_AC_IPC_3,					/* HW [63] AC - IPC [2]*/
	INTC_CI2C_SCL_SDA_WAKE_UP_DUMMY_I2C_2_TEST,

	INTC_MAX_INTERRUPT_SOURCES
}INTC_InterruptSources_xirq;

// First XIRQ interrupt in the INTC_InterruptSources enum
#define INTC_SRC_S_START SW_INT_SRC_0

#endif
////////////////////////////////////////////////////////////
#endif                                  // _INTC_LIST_H_
