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

//
// Hermon TOP level APB register map
//
#ifndef _HERMON_REGS_H_
#define _HERMON_REGS_H_

#include "gbl_types.h"

// Hermon APB memory map
#define HERMON_RTC_BASE  0x40900000
#ifndef PHS_SW_DEMO_TTC
#if defined(INTEL_2CHIP_PLAT_BVD)
#define HERMON_SSP0_BASE 0x41000000
#define HERMON_SSP1_BASE 0x41700000
#define HERMON_SSP2_BASE 0x41900000
#if defined (_TAVOR_BOERNE_)
#define HERMON_SSP3_BASE 0x41A00000
#endif
#else
#define HERMON_SSP0_BASE 0x42600000
#define HERMON_SSP1_BASE 0x42680000
#define HERMON_SSP2_BASE 0x42700000
#define HERMON_SSP3_BASE 0x42800000
#endif
#ifdef _HERMON_B0_SILICON_
#define HERMON_GSSP0_BASE 0x42C40000
#define HERMON_GSSP1_BASE 0x42C50000
#define HERMON_DRIVE_EXT_BASE 0x4290029C
#endif
#else
#define HERMON_SSP0_BASE 0xD401B000
#define HERMON_SSP1_BASE 0xD42A0C00
#define HERMON_SSP2_BASE 0xD401C000
#define HERMON_GSSP0_BASE 0xF020A000
//efine HERMON_SSP3_BASE HERMON_SSP1_BASE
//efine HERMON_GSSP1_BASE HERMON_SSP2_BASE
#endif
#define HERMON_GEN_BASE 0x42900000
#define HERMON_CIPHER_BASE 0x42A00100
#define HERMON_LCD0_BASE 0x42B00000
#define HERMON_LCD1_BASE 0x42B80000
#define HERMON_GSM_BASE 0x42C00000
#define HERMON_CAMERA_BASE 0x42D00000

#define HERMON_GREGS_BASE (HERMON_GEN_BASE+0x40)
#define HERMON_GREGS_BASE_EXT (HERMON_GEN_BASE+0x100)

// Imaged General Regs (mostly for pin Mux use)
typedef struct
{
  UINT32 AFSR[8];
  UINT32 SPSR;
  UINT32 ACR[2];
  UINT32 ADWR;
  UINT32 PCDCR;
  UINT32 PCSCR;
  UINT32 EXT_MSA_BOOT_ADDR;
  UINT32 SDRAM_DELAY;
  UINT32 EGSR[2];
  UINT32 GPSR[8];
#ifdef _HERMON_B0_SILICON_
  UINT32 regs_pad[2];
  UINT32 EGSR_ext[2];
#endif
}HermonGRegs_t;

// Not Imaged
typedef struct
{
#ifdef _HERMON_B0_SILICON_
 // General control registers
 UINT32 pad0[3];
 UINT32 EXT_APB_UNSCHED_WAKE;
 UINT32 WB_GSM_CHOSE;
 UINT32 SCK_INPUTS_SEL;
 UINT32 MAN_OR_3PT;
 UINT32 HER_OR_MAN_INT_SEL;
 UINT32 XSWI_X_INT;
 UINT32 pad1[(0x18C-0x124)/4];
 // MCU Clock control APB registers
 UINT32 WCU_CAM_INT_EN;
 UINT32 WCU_LCD0_EN;
 UINT32 WCU_LCD1_EN;
 UINT32 WCU_DENOM;
 UINT32 WCU_NOM;
 UINT32 WCU_DIV_LCD0;
 UINT32 WCU_DIV_LCD1;
 UINT32 WCU_32CIP_EN;
 UINT32 pad2[(0x1D4-0x1AC)/4];
 UINT32 LCD_MEMC_PH;
 UINT32 GSM_CPU_RESET_N;
 UINT32 pad3[(0x1E0-0x1DC)/4];
 UINT32 TETON_WRAPPER_ON;         //1E0
 UINT32 PLL2_DIV_CONTROL;
 UINT32 PLL2_ON;
 UINT32 PLL2_EN_OWNER;
 UINT32 DSP_CLK_SOURCE_SEL;
 UINT32 PLL2_FORCE_MODE;
 UINT32 I2S_NOM_DENOM_0;         //1F8
 UINT32 I2S_NOM_DENOM_1;
 UINT32 I2S_SYSCLK0_EN;
 UINT32 I2S_SYSCLK1_EN;
 UINT32 I2S_BITCLK1_EN;
 UINT32 I2S_SYSCLK0_312_SEL;
 UINT32 I2S_SYSCLK1_312_SEL;
 UINT32 I2S_BITCLK_DIV_468;
 UINT32 pad4;
 UINT32 CLK_CSSP1_EN;
 UINT32 CLK_CSSP2_EN;
 UINT32 RST_CSSP1_REGIS;   //synchronous reset
 UINT32 RST_CSSP2_REGIS;   //synchronous reset
 UINT32 APB_PLLCLK_ON_I2S;
 UINT32 APB_RESET_ASYNC;   //asynchronous reset
 UINT32 CLK_GSSP0_EN;
 UINT32 CLK_GSSP1_EN;
 UINT32 RST_GSSP0_REGIS;   //synchronous reset
 UINT32 RST_GSSP1_REGIS;   //synchronous reset
 UINT32 CLK_GSSP0_SRC_SEL;
 UINT32 CLK_GSSP1_SRC_SEL;
 UINT32 CLK_CSSP1_SRC_SEL;
 UINT32 CLK_CSSP2_SRC_SEL;
 UINT32 FPXG_LEGACY;
 UINT32 pad5[(0x268-0x258)/4];
 UINT32 DIV_ME_FUNC;
 UINT32 DIV_ME_SRAM;
 UINT32 ME_FUNC_EN;
 UINT32 ME_SRAM_EN;
 UINT32 CLK_52LP_EN;
 UINT32 ME_FUNC_RST;
 UINT32 ME_SRAM_RST;
 UINT32 USB_MASK_CLK;
 UINT32 USB_RST_SEL;
 UINT32 OTG_SYNC_RST;
 UINT32 OTG_ASYNC_RST;
 UINT32 pad6[2];
 UINT32 PCDC1;
 UINT32 PCSC1;
 UINT32 PAD_DRV_VAL[6];//TBD - #7 or
 UINT32 ACR2; //SDCAS_DELAY
 UINT32 DK_POL_REG;    //TBD   this one
 UINT32 CLK_32K_OUT_EN;
 UINT32 VCTCXO_REQ_EN;
 UINT32 VCTCXO_REQ_POL;
 UINT32 LLR_CLK_EN;          //2D0
 UINT32 RST_CSSP1_ASYNC;
 UINT32 RST_CSSP2_ASYNC;
 UINT32 RST_GSSP0_ASYNC;
 UINT32 RST_GSSP1_ASYNC;    //2E0
 UINT32 pad7;
 UINT32 pad8;
 UINT32 EMU_CONT_VALID_REG; //2EC
 UINT32 pad9;
 UINT32 CLKRES_MR_XR;
 UINT32 pad10[(0x3FC-0x2F8)/4];
 UINT32 VERSION;
#else    /* A0 */
 // General control registers
 UINT32 EXT_APB_CW_EN;
 UINT32 EXT_APB_CW_SHFDR;
 UINT32 SEL_DPCM1_CSSP2;
 UINT32 EXT_APB_UNSCHED_WAKE;
 UINT32 WB_GSM_CHOSE;
 UINT32 pad1[((0x18C-0x110)/4)-1];
 UINT32 WCU_CAM_INT_EN;
 UINT32 WCU_LCD0_EN;
 UINT32 WCU_LCD1_EN;
 UINT32 WCU_DENOM;
 UINT32 WCU_NOM;
 UINT32 WCU_DIV_LCD0;
 UINT32 WCU_DIV_LCD1;
 UINT32 WCU_32CIP_EN;
 UINT32 WCU_CSSP_RSTN;
 UINT32 WCU_CSSP_CLK_EN;
 UINT32 FORCE_CIP_CLK;
 UINT32 pad2[2];
 UINT32 FLSH_FF_SEL;
 UINT32 SRAM_MMR_APB_EN;
 UINT32 SRAM_SEL_WS_DAC;
 UINT32 SRAM_SEL_WS_EN;
 UINT32 SRAM_WAIT_PIPE_EN;
#endif  /* _HERMON_B0_SILICON_ */
}HermonGRegsExt_t;

// Dedicated registers
#define HERMON_WCIPHER_CLOCK_CONTROL_REG  ((volatile UINT32*)(HERMON_GEN_BASE+0x1a8))
#define HERMON_WCIPHER_CLOCK_FORCE_REG    ((volatile UINT32*)(HERMON_GEN_BASE+0x1b4))

//
// SPSR bits (Strapping)
// 1 is default (internal pull-up), strap adds an external pull-down
//                            // default = 1
#define strap_CLOCK_SEL 0x001 /* 13MHz */
#define strap_CHIP_MODE 0x002 /* ONE chip */
#define strap_DSP_BS    0x004 /* Normal */
#define strap_REDIR_MSA 0x008 /* XSC control on MSA reset */
#define strap_PJTAG_SEL 0x010 /* XSC */
#define strap_SJTAG_EN  0x020 /* Disabled */
#define strap_MSA_BOOT  0x040 /* Internal */
#define strap_XSC_BOOT  0x080 /* Internal */
#define strap_XSC_FREQ  0x100 /* 312MHz */
#if defined(_HERMON_B0_SILICON_)
#define strap_MEMC_WIDTH  0x200 /* 16bit */
#define strap_PX_FREQ     0x400 /* 156MHz */
#define strap_XSF_FREQ    0x800 /* PX frequency */
#define strap_MEMC_FREQ  0x1000 /* MEMC-PX frequency */
#endif

//ACR0 bits
#define ACR0_GSPE  0x01
#define ACR0_RRC0  0x02
#define ACR0_RRC1  0x04
#define ACR0_RRC2  0x08
#define ACR0_SAIE  0x10
#define ACR0_KPDAH 0x20
#define ACR0_DKAH  0x40
#define ACR0_SME   0x80
#define ACR0_13M   0x100
#define ACR0_DPCM1           0x10000

#define ACR0_RFCONT_SEL_0    0x00020000
#define ACR0_RFCONT_SEL_1    0x00040000
#define ACR0_RFCONT_SEL_2    0x00080000
#define ACR0_RFCONT_SEL_3    0x00100000
#define ACR0_RFCONT_SEL_4    0x00200000
#define ACR0_RFCONT_SEL_5    0x00400000
#define ACR0_RFCONT_SEL_6    0x00800000
#define ACR0_RFCONT_SEL_7    0x01000000
#define ACR0_RFCONT_SEL_8    0x02000000
#define ACR0_RFCONT_SEL_9    0x04000000

#ifdef _HERMON_B0_SILICON_
#define ACR0_FORCE_INT_EN    0x08000000
#define ACR0_SET_INT_MASK    0x10000000
#define ACR0_INT_IRQ_R_SEL   0x20000000

#define GCRAPB1_ADDRESS         0x42C32000 /*General Control Register on APB*/
#define HER_OR_MAN_INT_SEL_ADDRESS  0x4290011c
#define MAN_OR_3PT_ADDRESS      0x42900118

#define MAN_SELECT              0x01
#define HER_SELECT              0x00

#define ACR1_ADDRESS			0x42900068
#define ENABLE_IN_OUT			0x10000
#define GSM_WB_MUXING_ADDRESS	0x42900064
#define BIT_21_SELECT_TCU       0x80000


/*  Sim Reset Register  */
#define GSM_SIM_RST_ADDRESS 		0x4290025C
#define SIM_RESET_RELEASE          0x1
#define SIM_RESET                  0x0

/*  Slow Clock Control Regster */
#define SCCNTR_ADDRESS				0x42c12000
#define SCSTATE_CONTROL_ADDRESS     0x42C11020
#define NIRQ						0x01
#define NACTIVE						0x02
#define STICKY_BIT_RESET			0x04

/*  TCU Control Register  */
#define TCUCNTR_ADDRESS				0x42c02000
#define TCU_CONTROL_REG_ADDRESS     0x42C01010
#define TCU_HALT 					0x01 /* in Manitobe Mode this bit connect to the halt  */

/*  GSM DSSP1 Clock En DPB domain */
#define DSSP1_CLK_EN_ADDRESS		0xC0A01624
#define DSSP1_ACTIVE_CLK			0x01

/*  GSM OTHELLO Mode DPB domain */
#define OTHELLO_MODE_ADDRESS		  0xC0A0164C

#define DSSP5_GPIO_OUTPUT_EN          0x80000   // DSSP0
#define DSSP4_GPIO_OUTPUT_EN          0x40000   // DSSP1
#define DSSP2_GPIO_OUTPUT_EN          0x20000   // DSSP2
#define DSSP1_GPIO_OUTPUT_EN          0x10000   // DSSP3
#define FRAME_STRIPPER_RESET_RELEASE  0x00
#define FRAME_STRIPPER_RESET          0x40
#define FRAME_SYNC_STRIPPER_ON        0x20
#define FRAME_SYNC_STRIPPER_OFF       0x00
#define FRAME_STRIPPER_POLARITY       0x00
#define SELECT_DSSP1				  0x00 // - SELECT_DSSP0
#define SELECT_DSSP2                  0x00 // - SELECT_DSSP1
#define SELECT_DSSP4                  0x00 // - SELECT_DSSP2
#define SELECT_DSSP5                  0x00 // - SELECT_DSSP3

#endif



#define READ_FROM_REG_32(address,val)  \
        ((val) = (*((volatile UINT32 *)(address))))

#define READ_FROM_REG_32_U(address)  \
        (*((volatile UINT32 *)(address)))

#define WRITE_TO_REG_32(address,val)  \
      (*((volatile UINT32 *)(address)) = (val))


// ACR1 bits
#ifdef _HERMON_B0_SILICON_
#define ACR1_RFAFE_SAIE      0x00010000       // RF&AFE pins forced to input
#define ACR1_MSL_SAIE        (1<<23) //0x00800000       // MSL MASK
#endif

#include "bsp_mux.h"
extern volatile HermonGRegs_t* const hermonGR;
extern volatile HermonGRegsExt_t* const hermonGRE;

#endif // _HERMON_REGS_H_
