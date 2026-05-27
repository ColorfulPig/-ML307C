/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*
 * XIRQ table for TAVOR Harbell (coupled with the interrupt list, see intc_list.h).
 * THIS FILE CAN BE #include'd ONLY ONCE
 */
	{SW_INT_SRC_0  ,  15  },
	{SW_INT_SRC_1  ,  15  },
	{SW_INT_SRC_2  ,  15  },
	{SW_INT_SRC_3  ,  15  },
	{SW_INT_SRC_4  ,  15   },
	{SW_INT_SRC_5  ,  15  },
	{SW_INT_SRC_6  ,  15  },
	{SW_INT_SRC_7  ,  15  },
	{SW_INT_SRC_8  ,  15  },
	{SW_INT_SRC_9  ,  15  },
	{SW_INT_SRC_10 ,  15  },
	{SW_INT_SRC_11 ,  15  },
	{SW_INT_SRC_12 ,  15  },
	{SW_INT_SRC_13 ,  15  },
	{SW_INT_SRC_14 ,  15  },
	{SW_INT_SRC_15 ,  15  },
	/* Internal Interrupts (or register interrupts ) [16:23] , 8 possible sources*/
	{RI_INT_SRC_0  ,  0  },
	{RI_INT_SRC_1  ,  7  },
	{RI_INT_SRC_2  ,  5  },
	{RI_INT_SRC_3  ,  4  },
	{RI_INT_SRC_4  ,  5  },
	{RI_INT_SRC_5  ,  10 },
	{RI_INT_SRC_6  ,  15 },
	{RI_INT_SRC_7  ,  2  },
	{XSWI_INT_SRC_0,  0  },
	/* iNTERNAL EXC  7  },EPTION 3 INTERRUPTS*/
	{EIRQ_INT_0    ,  10 },
	{EIRQ_INT_1    ,  15 },
	{EIRQ_INT_2    ,  2  },
	/* HW interrupts - sourced by GB peripherals (including the Modem) 48 possible interrupts*/
	{INTC_HW_DMA_C3_0 ,  0  },
	{INTC_HW_DMA_C3_1 ,  7  },
	{INTC_HW_DMA_C3_2 ,  5  },
	{INTC_HW_DMA_C3_3 ,  4  },
	{INTC_HW_DMA_C3_4 ,  5  },
	{INTC_HW_DMA_C3_5 ,  10 },
	{INTC_HW_DMA_C3_6 ,  15 },
	{INTC_HW_DMA_C3_7 ,  2  },
	{INTC_HW_DMA_C2_0 ,  0  },

	{INTC_HW_DSPP_3   ,  4  },
	{INTC_HW_DMA_C2_2 ,  7  },
	{INTC_HW_DMA_C2_3 ,  5  },
	{INTC_HW_DMA_C2_4 ,  4  },
	{INTC_HW_DMA_C2_5 ,  5  },
	{INTC_HW_DMA_C2_6 ,  10 },
	{INTC_HW_DMA_C2_7 ,  15 },
	{INTC_HW_SLOW_CLK_SNAP ,  2  },
	{INTC_HW_EarlyWKUP ,7},
#if !defined (EDEN_1928) && !defined (NEZHA3_1826)
	{INTC_HW_GSSP_1   ,  0  },
	{INTC_HW_GSSP_2   ,  3  },
	{INTC_HW_RESERVED ,  7  },
#endif		
	{INTC_HW_MSL      ,  5  },
	{INTC_HW_EXTPAD_1 ,  4  },
	{INTC_HW_EXTPAD_2 ,  5  },
	{INTC_HW_USIM     ,  10 },
	{INTC_HW_I2C      ,  4 },

	{INTC_SRC_IPC_DATA_ACK ,  7  },
	{INTC_HW_IpcDatBuffAck ,  0 },
	{INTC_SRC_IPC_RD       ,  5  },
	{INTC_SRC_IPC_ACK      ,  4  },
	{INTC_SRC_IPC_GP       ,  5  },

#ifndef EDEN_1928
	{INTC_HW_CPMU     ,  4  },
#endif
	{INTC_HW_TCU_EXCP ,  7  },
	{INTC_HW_RTU_0    ,  4  },  //{INTC_HW_RTU_0    ,  5  }, //20090909, asked by MaXh
	{INTC_HW_RTU_1    ,  4  },
	{INTC_HW_RTU_2    ,  5  },
	{INTC_HW_Tmr1     ,  10 },
    {INTC_HW_Tmr2     ,  15 },
    {INTC_HW_Tmr3BkUp ,  10  },
    {INTC_HW_Tmr4BkUp ,  0  },
    {INTC_HW_TmrWDT   ,  3  },
	{INTC_HW_ECIPHER  ,  7  },
	{INTC_HW_TCU_GPOB ,  5  },
	{INTC_HW_ADPC     ,  4  },
	{INTC_HW_END_SLEEP_BKUP,5 },
    {INTC_HW_TCU_CLK_EN     , 10 },
    {INTC_HW_CL_GB_HINT , 15 },
    {INTC_SRC_CIPHER,14}
	,{INTC_BOERNE_CLK_CHG,	  0			},
	{INTC_GSM_WAKE_UP,		  1			},
	{INTC_WBCDMA_WAKE_UP,	  2			},
	{INTC_CRXD_32_28_EVENT,	  3			},
	{INTC_CRXD_27_0_EVENT,	  4			},
	{INTC_GSSP1_WAKE_UP,	  0			},
	{INTC_GSSP2_WAKE_UP,	  1			},
	{INTC_GSIM_UDET_UIO,	  2			},
	{INTC_CI2C_SCL_SDA_WAKE_UP, 3		},
	{INTC_COMM_UART,		  4			},
	{INTC_GB_DMA_C2,		  0			},
	{INTC_AC_IPC_0,			  1		    },
	{INTC_D2_MODEM_OK,		  2		    },
	{INTC_AC_IPC_1,			  3		    },
	{INTC_AC_IPC_2,			  4		    },
	{INTC_AC_IPC_3,			  0		    }
	// no setting for: INTC_CI2C_SCL_SDA_WAKE_UP_DUMMY_I2C_2_TEST - NOT A REAL HW INTERRUPT
