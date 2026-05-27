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

/************************************************************************/
/*                                                                      */
/* Title: PMU Hardware header file                                      */
/*                                                                      */
/* Filename: pmu_hw.h                                                   */
/*                                                                      */
/* Author: Shuki Yabbo                                                  */
/*                                                                      */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/
#ifndef _PMU_HW_H_
#define _PMU_HW_H_

#ifdef MIPS_TEST_FOR_SLEEP
#include "cpu_mips_test.h"
#endif /* MIPS_TEST_FOR_SLEEP */

#define PMU_MAX_FUNCTIONAL_CLOCKS               32
#define PMU_MAX_APB_CLOCKS                      33
#define LOW_GROUP_OF_APB_PERIPHERALS            31            //according to APBPeripherals enum - the first 31 peripherals
#define HIGH_GROUP_OF_APB_PERIPHERALS_OFFSET    32            //according to APBPeripherals enum - peripherals that their APB clock number is above 31

#define USB_PLL_ON_MASK                         0x10000000L
#define USB_PLL_OFF_MASK                        0xEFFFFFFFL
#define USB_PLL_STABLE_MASK                     0x00800000L
#define USB_PLL_STABLE                          0x00800000L
#define USB_PLL_STATUS_BIT_OFFSET               28
#define PLL_MF_MASK                             0xFC000000L
#define PLL_MF_BITS_OFFSET                      26            //according to POSR bits 31-26
#define PLL_MF_COLUMN_AT_USB_PLL_TBL_OFFSET     4             //according to _PMUUsbPllTable

#define APB_SD_ALLOW_MASK                       0x04000000L   //APB clock domain shut down allow mask
#define APB_SD_NOT_ALLOW_MASK                   0xFBFFFFFFL   //APB clock domain shut down not allow mask
#define APB_CLOCK_ON_MASK                       0x00000001L   //peripheral APB clock on mask
#define APB_CLOCK_OFF_MASK                      0xFFFFFFFEL   //peripheral APB clock off mask
#define APB_RESET_WITH_CLOCK_ENABLE_MASK        0x00000003L   //peripheral APB reset + enable clock mask
#define APB_RESET_WITHOUT_CLOCK_ENABLE_MASK     0x00000002L   //peripheral APB reset without enabling the clock mask

#define PMU_CLOCKS_OFF                          0x00
#define PMU_ONLY_FUNC                           0x01
#define PMU_ONLY_APB                            0x02
#define PMU_CLOCKS_ON                           0x03

#define MSA_CLOCK_SD_ALLOW_MASK                 0x40000000L   //according to XPCR bit 30
#define MSA_CLK_SD_ALLOW                        0x40000000L   //according to XPCR bit 30
#define MSA_CLK_SD_NOT_ALLOW_MASK               0xBFFFFFFFL   //according to XPCR bit 30
#define MSA_CLK_ON_MASK                         0xBFFFFFFFL   //according to XPCR bit 30
#define MSA_RESET_ASSERT_MASK                   0x00000004L   //according to XPRR2 bit 2
#define MSA_RESET_NEGATE_MASK                   0xFFFFFFFBL   //according to XPRR2 bit 2

#define BB_CLOCK_SD_ALLOW_MASK                  0x02000000L   //according to XPCR bit 25
#define BB_CLK_SD_ALLOW                         0x02000000L   //according to XPCR bit 25
#define BB_CLOCK_SD_NOT_ALLOW_MASK              0xFDFFFFFFL   //according to XPCR bit 25
#define BB_CLK_ON_MASK                          0xFDFFFFFFL   //according to XPCR bit 25
#define BB_RESET_ASSERT_MASK                    0x00000008L   //according to XPRR2 bit 3
#define BB_RESET_NEGATE_MASK                    0xFFFFFFF7L   //according to XPRR2 bit 3

#define MEMC_CLK_SD_ALLOW_MASK                  0x08000000L   //according to XPCR bit 27
#define MEMC_CLK_SD_NOT_ALLOW_MASK              0xF7FFFFFFL   //according to XPCR bit 27

#define DROWSY_WKUP_PORT_BIT_OFFSET             23            //according to XPCR bit 23
#define PMU_MASK                                1
#define PMU_UN_MASK                             0

#define MSL_CLK_DET_ENABLE_MASK                 0x00040000L   //according to XPCR bit 18
#define MSL_CLK_DET_DISABLE_MASK                0xFFFBFFFFL   //according to XPCR bit 18
#define MSL_CLK_DET_CLEAR_FIRST_STEP                            0x00020000L   //according to XPCR bit 17
#define MSL_CLK_DET_CLEAR_SECOND_STEP                           0xFFFDFFFFL   //according to XPCR bit 17

#define SYS_RESET_BITS_MASK                     0x00000007L   //according to XRSR bits 0..2

#define PMU_FORCE_ENABLE_MASK                   0x80000000L   //according to POCR bit 31
#define PMU_FORCE_DISABLE_MASK                  0x7FFFFFFFL   //according to POCR bit 31

#define RST_MEMC_DIV_BITS                       0xFFF3FFFFL   //according to CGCR bits 18..19
#define MEMC_DIV_DEFAULT                        0x0002        //according to MEMC div valid values
#define MEMC_VALID_DIV_VALUES_MASK              0x03          //according to MEMC div valid values (1 or 2)
#define MEMC_DIV_BITS_OFFSET                    18            //according to CGCR bits 18..19
#define RST_I2S_DIV_BITS                        0xFFFE00FFL   //according to CGCR bits 8..16
#define I2S_DIV_BITS_OFFSET                     8             //according to CGCR bits 8..16
#define PMU_I2S_MAX_DIV_VALUE                   0x0102        //according to CGCR bits 8..16
#define THREE_I2S_BIT_CLK_CYCLES                                30            //30 usec in the worst case of the I2S divider clock configuration
#define PMU_MSHC_MIN_DIV_VALUE                  4             //according to CGCR bits 2..7
#define PMU_MSHC_MAX_DIV_VALUE                  64            //according to CGCR bits 2..7
#define RST_MSHC_DIV_BITS                       0xFFFFFF03L   //according to CGCR bits 2..7
#define MSHC_DIV_BITS_OFFSET                    2             //according to CGCR bits 2..7

#define USIM_BIT_OFFSET                         4             //according to CGCR bit 25
#define ICP_BIT_OFFSET                          3             //according to CGCR bit 24
#define I2C_BIT_OFFSET                          1             //according to CGCR bit 22
#define MSL_BIT_OFFSET                          2             //according to CGCR bit 23
#define MMC_BIT_OFFSET                          0             //according to CGCR bit 21
#define USB_BIT_OFFSET                          5             //according to CGCR bit 26
#define CGCR_MUX_SEL_BITS_OFFSET                21            //according to CGCR bit 21

#define PMU_RESET_ALL_PERIPHERALS_FUNCTIONAL_SIDE				0xffffffffL   //according to XPRR1 register
#define PMU_NEGATE_ALL_PERIPHERALS_FUNCTIONAL_SIDE_RESET		0x00000000L   //according to XPRR1 register
#define PMU_ALL_FUNC_OFF                        0x00000000L   //according to XCGR register
#define PMU_ALL_FUNCTIONAL_CLOCKS_ON							0xffffffffL   //according to XCGR register
#define PMU_ALL_FUNC_CLK_OFF_EXCEPT_KEYPAD      0x00080000L   //according to XCGR register
#define PMU_ALL_APB_CLK_OFF_EXCEPT_KEYPAD_AND_INTC_LOW_GROUP    0x00080200L   //according to WPBCPU registers
#define PMU_ALL_APB_CLK_OFF_HIGH_GROUP          0x00          //according to WPBCPU registers

// HP02: TBD
#define PMU_WAKEUP_PERIPHERALS_MASK                             0xFCD4DF8F    //according to XCGR register
#define APB_WAKEUP_PERIPHERALS_MASK                             0x80D1FDFF    //according to WPBCPU registers
#define PMU_MFC_CONDITIONS                      0xC6000000L   //according to XPCR register
#define PMU_CORE_ONLY_IDLE_CONDITIONS           0x00L         //according to XPCR register
#define PMU_PX_IDLE_CONDITIONS                  0x80000000L   //according to XPCR register
#define PMU_IDLE_CONDITIONS                     0xC6000000L   //according to XPCR register  //
#define PMU_DROWSY_CONDITIONS                   0xE6000000L   //according to XPCR register
#define PMU_DROWSY_WITH_VCXOSD_CONDITIONS       0xE6080000L   //according to XPCR register
#define PMU_CLR_INT_MASK_1                      0x01000000L   //according to XPCR bit 24
#define PMU_CLR_INT_MASK_0                      0xFEFFFFFFL   //according to XPCR bit 24
#define PMU_MSA_SS_IDLE_ALLOW_MASK              0x400000000	  //according to XPCR bit 30

#define PMU_MEMC_SHUTDOWN_CONDITIONS            0xA8000000L   //XPCR register: bits that cause MEMC shutdown
#define PMU_DROWSY_VOTES                        (/*MSA_CLK_SD_ALLOW|*/BB_CLK_SD_ALLOW) /*MSA on demand*/
#define MSA_CORE_STATUS_BIT						0x80000000	  //according to XPSR bit 31

#define PMU_78MHZ                               6             //according to POSR, pll = 13 * pllmf
#define PMU_104MHZ                              8             //according to POSR, pll = 13 * pllmf
#define PMU_117MHZ                              9             //according to POSR, pll = 13 * pllmf
#define PMU_156MHZ                              12            //according to POSR, pll = 13 * pllmf
#define PMU_208MHZ                              16            //according to POSR, pll = 13 * pllmf
#define PMU_234MHZ                              18            //according to POSR, pll = 13 * pllmf
#define PMU_312MHZ                              24            //according to POSR, pll = 13 * pllmf

#define XPCR_INITIAL_VALUE                      0x00000000L   //all shut down's and drowsy are not allowed, all wk up ports are not masked, USB PLL and MSL clk detctor are disabled

#define UCCR_WHEN_PLL_78                        0x1FBD1800L   //coresponds to PLL = 78 MHz
#define UCCR_WHEN_PLL_104                       0x1FBD1200L   //coresponds to PLL = 104 MHz
#define UCCR_WHEN_PLL_117                       0x1FBD1000L   //coresponds to PLL = 117 MHz
#define UCCR_WHEN_PLL_156                       0x1FBD0C00L   //coresponds to PLL = 156 MHz
#define UCCR_WHEN_PLL_208                       0x1FBD0900L   //coresponds to PLL = 208 MHz
#define UCCR_WHEN_PLL_234                       0x1FBD0800L   //coresponds to PLL = 234 MHz
#ifndef _QT_//regular use
#define UCCR_WHEN_PLL_312                       0x1FBD0600L   //coresponds to PLL = 312 MHz
#else//for QT:
#define UCCR_WHEN_PLL_312                       0x3F6F0600L   //coresponds to PLL = 260.5 KHz
#endif//_QT_
#define CGCR_INITIAL_VALUE_78                   0x1008003CL   //ALTUSBDIV=2, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_104                  0x1008003CL   //ALTUSBDIV=2, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_117                  0x1008003CL   //ALTUSBDIV=2, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_156                  0x1808003CL   //ALTUSBDIV=4, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_208                  0x1808003CL   //ALTUSBDIV=4, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_234                  0x1808003CL   //ALTUSBDIV=4, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16
#define CGCR_INITIAL_VALUE_312                  0x0008003CL   //ALTUSBDIV=6.5, all muxes select main pll, MEMC=1/2 * px clk, I2S=PLL, MSHC=pll/16

#define XPRR1_INITIAL_VALUE                     0x00000000L   //all peripheral's resets are negated

#define WPBCPU_INITIAL_VALUE                    0x00000000L   //reset is negated, peripheral APB clock is disabled
#define APB_WARPPER_FORCE_OFF                   0x00000000L
#define APB_WRAPPER_FORCE_ON                    0x00000001L
#define APB_WRAPPER_POWER_CONSUMPTION_OFF       0x00000000L
#define APB_WRAPPER_POWER_CONSUMPTION_ON        0x00000001L
#define APB_GCR_INVERT_BIT_MASK                 0x01          //according to WPBGCR bit 0
#define APB_FORCE_MODE_STATUS_CELL              0             //the 1st cell in _APBModesStatus array
#define APB_POWER_CONSUMPTION_MODE_STATUS_CELL  1             //the 2nd cell in _APBModesStatus array
#define PMU_APB_TCU_MASK                                        0x08000000L   //according to WPBCPU[27] register of the APB Wrapper module
#define PMU_APB_SIM_MASK                                        0x00800000L   //according to WPBCPU[23] register of the APB Wrapper module

#define PMU_VCXOST_MASK                                         0x00ff0000L   //according to POCR[VCXOST] register
#define PMU_VCXOST_OFFSET                                       16            //according to POCR[VCXOST] register

#define PMU_HW_ADDRESS                          0x40F00000L
#define DUMMY_ADD_OFFSET                        0x00000800L
#define PMU_DUMMY_READ_HW_ADDRESS               (PMU_HW_ADDRESS + DUMMY_ADD_OFFSET)
#define APBWRAPPER_HW_ADDRESS                   0x42000000L
#define SUM_OF_APB_RESERVED                     (((0x0200 - 0x0080)/(sizeof(UINT32))) - 1)
#define SUM_OF_WPBCPU_REGS                      33
#define MAX_APB_WRAPPER_REGS                    (SUM_OF_WPBCPU_REGS + 2)  /*WPBCPU's + WPBFCR + WPBGCR*/

#define DUMMY_VALUE                             0xA5A5A5A5L


struct PmuHWRegisters
    {
    UINT32       XPCR      ;     //  XSC power control register                         Read / Write
    UINT32       XPSR      ;     //  XSC power status register                          Read only
    UINT32       FCCR      ;     //  frequency change control register                  Read / Write
    UINT32       POCR      ;     //  PLL and oscillator control register                Read / Write
    UINT32       POSR      ;     //  PLL and oscillator status register                 Read only
    UINT32       UCCR      ;     //  UART clock control register                        Read / Write
    UINT32       CGCR      ;     //  clocks generation control register                 Read / Write
    UINT32       XPRR1     ;     //  XSC programmable reset register #1                 Read / Write
    UINT32       XPRR2     ;     //  XSC programmable reset register #2                 Read / Write
    UINT32       XCGR      ;     //  XSC clock gating register                          Read / Write
    UINT32       XRSR      ;     //  XSC reset status register                          Read only
    UINT32       XDCR      ;     //  XSC drowsy control register                        Read / Write
    UINT32       GPCR      ;     //  general purpose clock generation clock register    Read / Write
    UINT32       RESERVED1 ;     //  reserved address
    UINT32       RESERVED2 ;     //  reserved address
    UINT32       XDWR      ;     //  XSC dummy write register                           Write only
    } ;

struct APBWrapperHWRegisters
    {
    UINT32       WPBCPU[33];     //  CPU bus control registers                         Write only
    UINT32       FILLER[SUM_OF_APB_RESERVED];     //  reserved addresses
    UINT32       WPBFCR    ;     //  force peripherals clock control register          Write only
    UINT32       WPBGCR    ;     //  bus gate control registers                        Write only
    } ;


#define     Pmu                   (* (volatile struct PmuHWRegisters *) PMU_HW_ADDRESS)
#define     Pmu_Dummy_Read_Add    (* (volatile struct PmuHWRegisters *) PMU_DUMMY_READ_HW_ADDRESS)
#define     APBWrapper            (* (volatile struct APBWrapperHWRegisters *) APBWRAPPER_HW_ADDRESS)


typedef enum
{
#if defined (_HERMON_B0_SILICON_) && !defined (INTEL_2CHIP_PLAT_BVD)
    PMU_RST_PWM = 2,
#else
    PMU_RST_PWM2 = 0,
    PMU_RST_PWM1 = 2,
#endif
    PMU_RST_I2S = 7,
    PMU_RST_USIM = 8,
#ifndef _HERMON_B0_SILICON_
    PMU_RST_SCI3,
    PMU_RST_SCI2,
    PMU_RST_SCI1,
#endif
    PMU_RST_ICP = 12,
    PMU_RST_SSP = 15,
    PMU_RST_GPIO = 17,
    PMU_RST_I2C,
    PMU_RST_KEYPAD,
    PMU_RST_MSHC,
    PMU_RST_MSL = 22,
    PMU_RST_DSYTMR,
    PMU_RST_NDSYTMR,
    PMU_RST_MMC = 27,
    PMU_RST_USB,
    PMU_RST_UART3,
    PMU_RST_UART2,
    PMU_RST_UART1,
    PMU_RST_NONE
}PMUResetPeripherals;

#if defined(_HERMON_B0_SILICON_) && !defined (INTEL_2CHIP_PLAT_BVD)
typedef enum
{
    APB_RST_UART1 = 0,
    APB_RST_UART2,
    APB_RST_UART3,
    APB_RST_I2C,
    APB_RST_IPC,
    APB_RST_USB,
    APB_RST_ICP,
    APB_RST_EXT_PERIPHERAL_0,
    APB_RST_MMC,
    APB_RST_KEYPAD,
    APB_RST_USIM = 10,
    APB_RST_EXT_PERIPHERAL_5,
    APB_RST_PWM,
    APB_RST_RESERVED_0,
    APB_RST_EGPIO,
    APB_RST_RESERVED_1,
    APB_RST_MSL,
    APB_RST_RTC,
    APB_RST_GPIO,
    APB_RST_INTC,
    APB_RST_EXT_PERIPHERAL_1 = 20,
    APB_RST_NDSYTMR,
    APB_RST_ONE_WIRE,
    APB_RST_SSP_O_1,
    APB_RST_SSP_2,
    APB_RST_EXT_PERIPHERAL_2,
    APB_RST_EXT_PERIPHERAL_3,
    APB_RST_EXT_PERIPHERAL_4,
    APB_RST_LCD_IF,
    APB_RST_EXT_PERIPHERAL_6,
    APB_RST_EXT_PERIPHERAL_7 = 30,
    APB_RST_MSHC,
    APB_RST_CAM_BIT,
    APB_RST_NONE
}APBResetPeripherals;
#else//defined(_HERMON_B0_SILICON_) && !defined (INTEL_2CHIP_PLAT_BVD)
typedef enum
{
    APB_RST_UART1 = 0,
    APB_RST_UART2,
    APB_RST_UART3,
    APB_RST_I2C,
    APB_RST_IPC,
    APB_RST_USB,
    APB_RST_ICP,
    APB_RST_SSP,
    APB_RST_MMC,
    APB_RST_KEYPAD,
    APB_RST_USIM,
    APB_RST_LCD_IF,
    APB_RST_PWM,
    APB_RST_SCI1,
    APB_RST_SCI2,
    APB_RST_SCI3,
    APB_RST_MSL,
    APB_RST_RTC,
    APB_RST_GPIO,
    APB_RST_INTC,
    APB_RST_DSYTMR,
    APB_RST_NDSYTMR,
    APB_RST_ONE_WIRE,
    APB_RST_EXT_PERIPHERAL_0,
    APB_RST_EXT_PERIPHERAL_1,
    APB_RST_EXT_PERIPHERAL_2,
    APB_RST_EXT_PERIPHERAL_3,
    APB_RST_EXT_PERIPHERAL_4,
    APB_RST_EXT_PERIPHERAL_5,
    APB_RST_EXT_PERIPHERAL_6,
    APB_RST_EXT_PERIPHERAL_7,
    APB_RST_MSHC,
    APB_RST_CAM_BIT,
    APB_RST_NONE
}APBResetPeripherals;
#endif

#if defined(_MANITOBA_SILICON_) || defined (INTEL_2CHIP_PLAT_BVD)
#define APB_RST_SIM APB_RST_EXT_PERIPHERAL_0
#define APB_RST_ABTH APB_RST_EXT_PERIPHERAL_2
#define APB_RST_XCRG APB_RST_EXT_PERIPHERAL_3
#define APB_RST_TCU_SC APB_RST_EXT_PERIPHERAL_4
#endif

/*----------- macro definitions ----------------------------------------*/
#define PMU_WRITE_REG(reg,val)       \
    (Pmu.reg) = (val);

#define PMU_WRITE_TO_DUMMY_REG                \
    {                                         \
      PMU_WRITE_REG(XDWR, DUMMY_VALUE);       \
      PMU_WRITE_REG(XDWR, DUMMY_VALUE);       \
    }

#if defined (MIPS_TEST_FOR_SLEEP)
#define PMU_WRITE_REG_WITH_DUMMY(reg,val)       \
    {                                           \
      PMU_WRITE_TO_DUMMY_REG;                   \
      PMU_WRITE_REG(reg,val);                             \
	  if(&Pmu.reg==&Pmu.XPCR) {                           \
	         CPU_MIPS_TEST(0xD9);                         \
	         CPU_MIPS_TEST((UINT8)(val>>24));             \
	         CPU_MIPS_TEST( (UINT8)((UINT16)(val>>16)));  \
	  }                                                   \
    }
#else
#define PMU_WRITE_REG_WITH_DUMMY(reg,val)       \
    {                                           \
      PMU_WRITE_TO_DUMMY_REG;                   \
      PMU_WRITE_REG(reg,val);               \
    }
#endif

#define PMU_READ_REG(reg,val)                   \
    {                                           \
      (val) = (Pmu_Dummy_Read_Add.reg);         \
      (val) = (Pmu.reg);                        \
    }


#endif /* _PMU_HW_H_ */



