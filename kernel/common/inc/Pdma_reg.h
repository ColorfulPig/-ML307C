

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
#ifndef    _PDMA_REG_H_

#define  _PDMA_REG_H_
//include "global_def.h"
#define   AXI_PDMA_BASE            0xD4000000
#define PDMA_INT_NUM                 47

#define SSP2_RX_DMA_CHN            29      
#define SSP2_TX_DMA_CHN            30      

#define AXI_DMA_CSR_CH(X)            ( AXI_PDMA_BASE + X*4)
#define AXI_DMA_INT                       ( AXI_PDMA_BASE + 0xF0)

#define AXI_DMA_DADR(X)              ( AXI_PDMA_BASE + 0x200 + X*16)         // DMA Descriptor Address Registers
#define AXI_DMA_SADR(X)              ( AXI_PDMA_BASE + 0x204 + X*16)
#define AXI_DMA_TADR(X)              ( AXI_PDMA_BASE + 0x208 + X*16)
#define AXI_DMA_CMD(X)               ( AXI_PDMA_BASE + 0x20C + X*16)

#define SSP2_RX_DMA_REQ            (AXI_PDMA_BASE + 0x100+ 60*4)
#define SSP2_TX_DMA_REQ            (AXI_PDMA_BASE + 0x100+ 61*4)

 #define  APBC_AIB_CLK_RST        (0xD4015000+0x3C) 
#define  MFPR_SSP2_SCLK          (0xD401E000+67*4) //GPIO_12, func1 
#define  MFPR_SSP2_SFRM          (0xD401E000+68*4) //GPIO_13
#define  MFPR_SSP2_TXD           (0xD401E000+70*4) //GPIO_15
#define  MFPR_SSP2_RXDIN         (0xD401E000+69*4) //GPIO_14




#endif //_PDMA_REG_H_

