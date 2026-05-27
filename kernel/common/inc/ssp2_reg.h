

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * **
*                                           *                                     *
*     File name:         Ssp_Dma.h       *   ASR Proprietary Information       *
*     Programmer:     Xiaobing Li            *   (C) COPYRIGHT 2017 ASR  Com      *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*                                                                                 *
*       Create Date:  June 19, 2017                                               *
*                                                                                 *
*       Description:  OsRtos configuration file                                   *
*                                                                                 *
*       Notes:                                                                    *
*                                                                                 *
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef    _SSP2_H_

#define  _SSP2_H_
//#include "global_def.h"

#define SSP2_REG_BASE               0xD401C000
#define APB_SSP2_CLK_RST            0xD401504C

//SSP Register settings

//SSP_TOP_CTRL
#define TXD_TRI_LAST_PHASE         (0x1<<18)  //SSP_TOP_CTRL[18]        
#define TXD_TRI_STATE              (0x1<<17)  //SSP_TOP_CTRL[17]
#define SLAVE_CLK_FREE_RUN         (0x1<<16)  //SSP_TOP_CTRL[16]
#define INVERT_FRAME               (0x1<<15)  //SSP_TOP_CTRL[15]
#define FRAM_CLK_LOW_SET           (0x1<<14)  //SSP_TOP_CTRL[14]
#define TRAIL_PXA                  (0x0<<13)  //SSP_TOP_CTRL[13]
#define TRAIL_DMA                  (0x1<<13)  //SSP_TOP_CTRL[13]
#define LOOP_BACK                  (0x1<<12)  //SSP_TOP_CTRL[12]
#define SPH_1                      (0x1<<11)  //SSP_TOP_CTRL[11]
#define SPH_0                      (0x0<<11)  //SSP_TOP_CTRL[11]
#define SPO_1                      (0x1<<10)  //SSP_TOP_CTRL[10]
#define SPO_0                      (0x0<<10)  //SSP_TOP_CTRL[10]
#define DW_8BYTE                   (0x7<<5)   //SSP_TOP_CTRL[9:5]
#define DW_16BYTE                  (0xf<<5)   //SSP_TOP_CTRL[9:5] 
#define DW_18BYTE                  (0x11<<5)  //SSP_TOP_CTRL[9:5]
#define DW_32BYTE                  (0x1f<<5)  //SSP_TOP_CTRL[9:5]
#define SSPSFRM_M                  (0x0<<4)   //SSP_TOP_CTRL[4]
#define SSPSFRM_S                  (0x1<<4)   //SSP_TOP_CTRL[4]
#define SSPSCLK_M                  (0x0<<3)   //SSP_TOP_CTRL[3]
#define SSPSCLK_S                  (0x1<<3)   //SSP_TOP_CTRL[3]
#define FRF_SPI                    (0x0<<1)   //SSP_TOP_CTRL[2:1]
#define FRF_TI                     (0x1<<1)   //SSP_TOP_CTRL[2:1]
#define FRF_NMW                    (0x2<<1)   //SSP_TOP_CTRL[2:1]
#define FRF_PSP                    (0x3<<1)   //SSP_TOP_CTRL[2:1]
#define SSE_EN                     (0x1<<0)   //SSP_TOP_CTRL[0]

//SSP_FIFO_CTRL
#define RX_AUTO_FULL_CTRL_EN       (0x1<<17)  //SSP_FIFO_CTRL[17]
#define FIFO_UNPACKING             (0x0<<16)  //SSP_FIFO_CTRL[16]
#define FIFO_PACKING               (0x1<<16)  //SSP_FIFO_CTRL[16]
#define RX_REQ_EN                  (0x1<<11)  //SSP_FIFO_CTRL[11]
#define TX_REQ_EN                  (0x1<<10)  //SSP_FIFO_CTRL[10]
#define RX_THRES_15                (0xf<<5)   //SSP_FIFO_CTRL[9:5]
#define RX_THRES_7                 (0x7<<5)   //SSP_FIFO_CTRL[9:5]
#define TX_THRES_15                (0xf<<0)   //SSP_FIFO_CTRL[4:0]
#define TX_THRES_7                 (0x7<<0)   //SSP_FIFO_CTRL[4:0]

//SSP_INT_EN
#define BIT_CNT_ERROR_INT_EN       (0x1<<6)   //SSP_INT_EN[6]
#define TX_FIFO_UNDERRUN_INT_MASK  (0x1<<5)   //SSP_INT_EN[5]
#define RX_FIFO_OVERRUN_INT_MASK   (0x1<<4)   //SSP_INT_EN[4]
#define TX_FIFO_INT_EN             (0x1<<3)   //SSP_INT_EN[3]
#define RX_FIFO_INT_EN             (0x1<<2)   //SSP_INT_EN[2]
#define TIMEOUT_EN                 (0x1<<1)   //SSP_INT_EN[1]
#define PERI_TRAIL_EN              (0x1<<0)   //SSP_INT_EN[0]

//SSP_NET_WORK_CTRL
#define FRDC_7                     (0x7<<1)   //SSP_NET_WORK_CTRL[3:1]
#define FRDC_3                     (0x3<<1)   //SSP_NET_WORK_CTRL[3:1]
#define FRDC_1                     (0x1<<1)   //SSP_NET_WORK_CTRL[3:1]
#define NET_WORK_MODE_EN           (0x1<<0)   //SSP_NET_WORK_CTRL[0]

//SSP_PSP_CTL
//SSP_RWOT_CTRL


typedef struct SSP_REG 
{
	volatile UINT32	SSP_TOP_CTRL		  ;//	0x00 
	volatile UINT32	SSP_FIFO_CTRL		  ;//	0x04 
	volatile UINT32	SSP_INT_EN			  ;//	0x08 
	volatile UINT32	SSP_TO				  ;//	0x0C 
	volatile UINT32	SSP_DATAR			  ;//	0x10 
	volatile UINT32	SSP_STATUS			  ;//	0x14 
	volatile UINT32	SSP_PSP_CTRL		  ;//	0x18 
	volatile UINT32	SSP_NET_WORK_CTRL	  ;//	0x1C 
	volatile UINT32	SSP_NET_WORK_STATUS   ;//	0x20 
} Ssp_RegT;

#endif //_SSP2_H_

