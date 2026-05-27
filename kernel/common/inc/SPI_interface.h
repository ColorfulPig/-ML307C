/******************************************************************************
 *
 *  (C)Copyright 2005 - 2012 Marvell. All Rights Reserved.
 *
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE OF MARVELL.
 *  The copyright notice above does not evidence any actual or intended
 *  publication of such source code.
 *  This Module contains Proprietary Information of Marvell and should be
 *  treated as Confidential.
 *  The information in this file is provided for the exclusive use of the
 *  licensees of Marvell.
 *  Such users have the right to use, modify, and incorporate this code into
 *  products for purposes authorized by the license agreement provided they
 *  include this notice and the associated copyright notice with any such
 *  product.
 *  The information in this file is provided "AS IS" without warranty.
 *
 ******************************************************************************/
#ifndef __SPI_H__
#define __SPI_H__

#include "global_types.h"

#define SSP0_RX_CHANNEL XLLP_DMAC_CHANNEL_21
#define SSP0_TX_CHANNEL XLLP_DMAC_CHANNEL_22

#define SSP1_RX_CHANNEL XLLP_DMAC_CHANNEL_23
#define SSP1_TX_CHANNEL XLLP_DMAC_CHANNEL_24

#define SSP2_RX_CHANNEL XLLP_DMAC_CHANNEL_25
#define SSP2_TX_CHANNEL XLLP_DMAC_CHANNEL_26


//added on 20200104
#define SSP0_RX_DMA_DEVICE		        DMAC_SSP_0_RX
#define SSP0_TX_DMA_DEVICE		        DMAC_SSP_0_TX

#define SSP1_RX_DMA_DEVICE		        DMAC_SSP_1_RX
#define SSP1_TX_DMA_DEVICE		        DMAC_SSP_1_TX

#define SSP2_RX_DMA_DEVICE		        DMAC_SSP_2_RX
#define SSP2_TX_DMA_DEVICE		        DMAC_SSP_2_TX


#define SSP_FCR_DMA_READ					(SSP_FCR_RFT(8) | SSP_FCR_TFT(7) | SSP_FCR_RSRE | SSP_FCR_TSRE)
#define SSP_FCR_DMA_WRITE	        		(SSP_FCR_TFT(7) | SSP_FCR_RSRE | SSP_FCR_TSRE)
#define SSP_FCR_DMA_READ_FAST				(SSP_FCR_RFT(16) | SSP_FCR_TFT(15) | SSP_FCR_RSRE | SSP_FCR_TSRE)

#define ssp_reg_read8(x) 		(*(volatile UINT8 *)(x) & 0xff)
#define ssp_reg_write8(x,y) 	((*(volatile UINT8 *)(x)) = (y & 0xff) )
#define ssp_reg_read(reg)		*(volatile unsigned int *)(reg)
#define ssp_reg_write(reg, val) *(volatile unsigned int *)(reg) = (val); *(volatile unsigned int *)(reg);
#define ssp_reg_bit_set(reg, val) *(volatile unsigned int *)(reg) |= (val); *(volatile unsigned int *)(reg);
#define ssp_reg_bit_clr(reg, val) *(volatile unsigned int *)(reg) &= ~(val); *(volatile unsigned int *)(reg);

// Timer out error						    0x80 - 0x8F
//#define NoError                             0x00 //modify by tw 
#define SSPWaitTxEmptyTimeOutError			0x80
#define SSPRdStatusTimeOutError		        0x81
#define SSPWelWaitTxEmptyTimeOutError	    0x82
#define SSPWaitForWELTimeOutError		    0x83
#define SSPWaitCompleteTimeOutError         0x84
#define SSPRxChannelTimeOutError            0x85
#define SSPTxChannelTimeOutError            0x86

/*
 *
 *	THE BIT DEFINES
 *
 */
#define BIT_0 (1 << 0)
#define BIT_1 (1 << 1)
#define BIT_2 (1 << 2)
#define BIT_3 (1 << 3)
#define BIT_4 (1 << 4)
#define BIT_5 (1 << 5)
#define BIT_6 (1 << 6)
#define BIT_7 (1 << 7)
#define BIT_8 (1 << 8)
#define BIT_9 (1 << 9)
#define BIT_10 (1 << 10)
#define BIT_11 (1 << 11)
#define BIT_12 (1 << 12)
#define BIT_13 (1 << 13)
#define BIT_14 (1 << 14)
#define BIT_15 (1 << 15)
#define BIT_16 (1 << 16)
#define BIT_17 (1 << 17)
#define BIT_18 (1 << 18)
#define BIT_19 (1 << 19)
#define BIT_20 (1 << 20)
#define BIT_21 (1 << 21)
#define BIT_22 (1 << 22)
#define BIT_23 (1 << 23)
#define BIT_24 (1 << 24)
#define BIT_25 (1 << 25)
#define BIT_26 (1 << 26)
#define BIT_27 (1 << 27)
#define BIT_28 (1 << 28)
#define BIT_29 (1 << 29)
#define BIT_30 (1 << 30)
#define BIT_31 ((unsigned)1 << 31)

#define SHIFT0(Val)  (Val)
#define SHIFT1(Val)  ((Val) << 1)
#define SHIFT2(Val)  ((Val) << 2)
#define SHIFT3(Val)  ((Val) << 3)
#define SHIFT4(Val)  ((Val) << 4)
#define SHIFT5(Val)  ((Val) << 5)
#define SHIFT6(Val)  ((Val) << 6)
#define SHIFT7(Val)  ((Val) << 7)
#define SHIFT8(Val)  ((Val) << 8)
#define SHIFT9(Val)  ((Val) << 9)
#define SHIFT10(Val) ((Val) << 10)
#define SHIFT11(Val) ((Val) << 11)
#define SHIFT12(Val) ((Val) << 12)
#define SHIFT13(Val) ((Val) << 13)
#define SHIFT14(Val) ((Val) << 14)
#define SHIFT15(Val) ((Val) << 15)
#define SHIFT16(Val) ((Val) << 16)
#define SHIFT17(Val) ((Val) << 17)
#define SHIFT18(Val) ((Val) << 18)
#define SHIFT19(Val) ((Val) << 19)
#define SHIFT20(Val) ((Val) << 20)
#define SHIFT21(Val) ((Val) << 21)
#define SHIFT22(Val) ((Val) << 22)
#define SHIFT23(Val) ((Val) << 23)
#define SHIFT24(Val) ((Val) << 24)
#define SHIFT25(Val) ((Val) << 25)
#define SHIFT26(Val) ((Val) << 26)
#define SHIFT27(Val) ((Val) << 27)
#define SHIFT28(Val) ((Val) << 28)
#define SHIFT29(Val) ((Val) << 29)
#define SHIFT30(Val) ((Val) << 30)
#define SHIFT31(Val) ((Val) << 31)


#define APBC_SSP_FNCLKSEL_6_5MHZ	(0x00<<4)
#define APBC_SSP_FNCLKSEL_13MHZ		(0x01<<4)
#define APBC_SSP_FNCLKSEL_26MHZ		(0x02<<4)
#define APBC_SSP_FNCLKSEL_52MHZ		(0x03<<4)
#define APBC_SSP_FNCLKSEL_3_25MHZ	(0x04<<4)
#define APBC_SSP_FNCLKSEL_1_625MHZ	(0x05<<4)
#define APBC_SSP_FNCLKSEL_812_5KHZ	(0x06<<4)
#define APBC_SSP_FNCLKSEL_I2S_CLK	(0x07<<4)

#define I2S_CLK_SEL_ENABLE			(1<<3)
#define APBC_SSP_RST				(1<<2)
#define APBC_SSP_FNCLK_ENABLE		(1<<1)
#define APBC_SSP_APBCLK_ENABLE		(1<<0)


#define APBC_GPIO_CLK_RST			((volatile UINT32 *)(0xD4015000 + 0x0008))

#define APBC_GPIO_RST				(1<<2)
#define APBC_GPIO_FNCLK_ENABLE		(1<<1)
#define APBC_GPIO_APBCLK_ENABLE		(0<<1)



#define	APBC_SSP0_CLK_RST  			((volatile UINT32 *)(0xD4015000 + 0x001C)) 
#define	APBC_SSP1_CLK_RST  			((volatile UINT32 *)(0xD4015000 + 0x0020)) 
#define APBC_SSP2_CLK_RST       	((volatile UINT32 *)(0xD4015000 + 0x004C)) 



#define	SPI_SSP0_BASE			 	0xD401B000
#define	SPI_SSP1_BASE			 	0xD401B800
#define	SPI_SSP2_BASE			 	0xD401C000   


#define TCR		       		(0x0000)	/* 32 bit	SSP TOP Control Register 0 */
#define FCR		       		(0x0004)	/* 32 bit	SSP FIFO Control Register 1  */
#define IER		       		(0x0008)	/* 32 bit	SSP Interrupt Enable Register  */
#define TOR					(0x000C)	/* 32 bit	SSP Time Out Register  */
#define DR		       		(0x0010)	/* 32 bit	SSP Data Register */
#define SR		       		(0x0014)	/* 32 bit	SSP Status Register */
#define PCR		       		(0x0018)

#define SSP_CSR_STOP_INT_EN			(1<<29)

#define SSP_IER_EBCEI				(1<<6)
//#define SSP_IER_TIM					(1<<5)
//#define SSP_IER_RIM					(1<<4)
//#define SSP_IER_TIE					(1<<3)
//#define SSP_IER_RIE					(1<<2)
#define SSP_IER_TINTE				(1<<1)
#define SSP_IER_PINTE				(1<<0)

#define SSP_SR_TX_FIFO_NOT_FULL		(1<<6)
#define SSP_SR_RX_FIFO_NOT_EMPTY	(1<<14)

#define SSP_SPI_MODE0			0x00
#define SSP_SPI_MODE1			SSP_TCR_SPH
#define SSP_SPI_MODE2			SSP_TCR_SPO
#define SSP_SPI_MODE3			(SSP_TCR_SPO | SSP_TCR_SPH)

/* Transmit FIFO Level: BIT 11:7, when it's 0x0, TXFIFO is emptry or full. */
#define SSP_SR_TFL	            0xF80

/* Transmit FIFO Not Full: 0 = TXFIFO is ful, 1 = TXFIFO is not full */
#define	SSP_SR_TF_NF		    (1<<6)

/* SSP Busy: 0 = SSPx port is idle or disabled, 1 = SSPx port is currently transmitting or receiving framed data */
#define	SSP_SR_BSY	            (1<<0)

//#define SSP0_TCR_SSE              	BIT0

#define SSP_TCR_INITIAL 	       	(SSP_TCR_TTELP | SSP_TCR_TTE | SSP_TCR_TRIAL | SSP_TCR_DSS8)   // default set data size 8bits
#define SSP_FCR_INITIAL 	       	(0)
#define SSP_IER_INITIAL 	       	(SSP_IER_RIM|SSP_IER_TIM)


#define	SPI0_CLK_1			((volatile UINT32 *)(0xD401E000 + 0x160))		//GPIO33, 0xD401_E160 /SSP0_CLK, AF1
#define	SPI0_FRM_1		    ((volatile UINT32 *)(0xD401E000 + 0x164))	    //GPIO34, 0xD401_E164 /SSP0_FRM, AF1
#define	SPI0_RXD_1		    ((volatile UINT32 *)(0xD401E000 + 0x168))		//GPIO35, 0xD401_E168 /SSP0_TX,  AF1
#define	SPI0_TXD_1		    ((volatile UINT32 *)(0xD401E000 + 0x16C))		//GPIO36, 0xD401_E16C /SSP0_TX,  AF1

#define	SPI1_CLK_0			((volatile UINT32 *)(0xD401E000 + 0x0EC))		//GPIO4, 0xD401_E160 /SSP1_CLK, 1603:AF1   1606:AF7  1602:AF2
#define	SPI1_FRM_0		    ((volatile UINT32 *)(0xD401E000 + 0x0F0))	    //GPIO5, 0xD401_E164 /SSP1_FRM, 1603:AF1   1606:AF7  1602:AF2
#define	SPI1_RXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x0F4))		//GPIO6, 0xD401_E168 /SSP1_TX,  1603:AF1   1606:AF7  1602:AF2
#define	SPI1_TXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x0F8))		//GPIO7, 0xD401_E16C /SSP1_TX,  1603:AF1   1606:AF7  1602:AF2

#define	SPI2_CLK_0			((volatile UINT32 *)(0xD401E000 + 0x10C))		//GPIO12, 0xD401_E160 /SSP2_CLK, AF1
#define	SPI2_FRM_0		    ((volatile UINT32 *)(0xD401E000 + 0x110))	    //GPIO13, 0xD401_E164 /SSP2_FRM, AF1
#define	SPI2_RXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x114))		//GPIO14, 0xD401_E168 /SSP2_RX,  AF1
#define	SPI2_TXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x118))		//GPIO15, 0xD401_E16C /SSP2_TX,  AF1

#define	SPI0_CLK_0			((volatile UINT32 *)(0xD401E000 + 0x11C))		//GPIO16,  /SSP0_CLK, AF2
#define	SPI0_FRM_0		    ((volatile UINT32 *)(0xD401E000 + 0x120))	    //GPIO17,  /SSP0_FRM, AF2
#define	SPI0_RXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x124))		//GPIO18,  /SSP0_RX,  AF2
#define	SPI0_TXD_0		    ((volatile UINT32 *)(0xD401E000 + 0x128))		//GPIO19,  /SSP0_TX,  AF2

#define	SPI2_CLK_1			((volatile UINT32 *)(0xD401E000 + 0x160))		//GPIO33, 0xD401_E160 /SSP2_CLK, AF2
#define	SPI2_FRM_1		    ((volatile UINT32 *)(0xD401E000 + 0x164))	    //GPIO34, 0xD401_E164 /SSP2_FRM, AF2
#define	SPI2_RXD_1		    ((volatile UINT32 *)(0xD401E000 + 0x168))		//GPIO35, 0xD401_E168 /SSP2_RX,  AF2
#define	SPI2_TXD_1		    ((volatile UINT32 *)(0xD401E000 + 0x16C))		//GPIO36, 0xD401_E16C /SSP2_TX,  AF2


#define SSP0_TOR_TIMEOUT	0x300
#define SSP1_TOR_TIMEOUT	0x300 
#define SSP2_TOR_TIMEOUT	0x300 
#define DEFAULT_TIMEOUT   	3000

typedef enum{
	SPI_MODE0,
	SPI_MODE1,
	SPI_MODE2,
	SPI_MODE3,
	SPI_MODE_MAX,
}SPI_MODE_T;

typedef enum {
	SPI_CLK_812_5KHZ,
	SPI_CLK_1_625MHZ,
	SPI_CLK_3_25MHZ,
	SPI_CLK_6_5MHZ,
	SPI_CLK_13MHZ,
	SPI_CLK_26MHZ,
	SPI_CLK_52MHZ,
	SPI_CLK_100KHZ,
	SPI_CLK_200KHZ,
	SPI_CLK_300KHZ,
	SPI_CLK_400KHZ,
	SPI_CLK_500KHZ,
	SPI_CLK_600KHZ,
	SPI_CLK_700KHZ,
	SPI_CLK_800KHZ,
	SPI_CLK_900KHZ,
	SPI_CLK_1MHZ,
	SPI_CLK_MAX,
}SPI_CLK_T;

//init 
void SPI_Initialize(int port, SPI_MODE_T spimode, SPI_CLK_T spiclk,int DataSize, BOOL gpiocs); //modify by tw,增加设置位宽参数
//add by tw
void SPI_DeInitialize(int port);

//dma write and read
UINT32 SPI_DMA_DataTransfer(int port, unsigned char *inbuf, unsigned char *outbuf, unsigned int len);
UINT32 SPI_DMA_Write(int port, unsigned char *wbuf, unsigned int len);
UINT32 SPI_DMA_Read(int port, unsigned char *rbuf, unsigned int len);

//PIO write and read
UINT32 SPI_PIO_Write(int port, unsigned char *wbuf, unsigned int len);
UINT32 SPI_PIO_Read(int port, unsigned char *rbuf, unsigned int len, unsigned char mosi_status);
UINT32 SPI_PIO_Write_Read(int port,unsigned char *inbuf, unsigned char *outbuf, unsigned int len);

#endif //__SPI_H__
