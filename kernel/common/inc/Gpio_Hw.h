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
/* Title: GPIO (General Purpose Input Output) HW Header File            */
/*                                                                      */
/* Filename: Gpio_Hw.h                                                  */
/*                                                                      */
/* Author: Yossi Gabay                                                  */
/*         Eilam Ben-Dror                                               */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/*                                                                      */
/* Yaeli Karni		Mar 2006, Enhance driver to support Boerne          */
/*                                                                      */
/************************************************************************/
#ifndef _GPIO_HW_H_
#define _GPIO_HW_H_

#include "gpio_config.h"  // some configurations are shared as INTC and GPIO are coupled


#define GPIO_PINS_PER_REG           32
#define GPIO_MAX_PIN_NUMBER         (GPIO_PINS_PER_REG - 1)
#define GPIO_SHIFT_PINS_PER_REG     5
#define GPIO_SHIFT_PINS_MASK        (0x1F)

#define ALL_HW_BITS                 0xFFFFFFFFL

// The legacy register map has 3 (GPIO_LEGACY_REG_AMOUNT) registers of each type (sequential), which allows for up to 96 GPIOs; 
// For 128 GPIO's option the 4th is in offset 0x100 bytes from the first one (hence, 0x40 words from the first register).
// This is applicable for registers {GPLRx,GPDRx,GPSRx,GPCRx,GRERx,GFERx,GEDRx}
// For new Set/Clear registers {GSDR,GCDR,GSFER,GCFER,GSRER,GCRER,AP_MASK,CP_MASK} two options exist: 
// - GPIO_SC_REGS_EMBEDDED: the same approach, while these registers follow previous ones in the register map (TTC layout)
// - otherwise: 4 registers sequentially located in a separate space after all legacy registers             (Tavor layout)
#if !defined (GPIO_128_ENTRIES)
#define GPIO_HW_REG_AMOUNT          3
// next define to allign with changes needed for BVD/BOERNE (see below)
#define GPIO_REG_INDEX(iNDEX)		(iNDEX)	// in Hermon platform - registers are sequential
#define GPIO_SC_REG_INDEX(iNDEX)    (iNDEX)	// in Hermon platform - registers are sequential
#else
#define GPIO_HW_REG_AMOUNT          4
#define GPIO_REG_INDEX(iNDEX)		(((iNDEX)>2)? (0x40+(iNDEX-3)) : iNDEX)
#if defined(GPIO_SC_REGS_EMBEDDED)
#define GPIO_SC_REG_INDEX(iNDEX)    (((iNDEX)>2)? (0x40+(iNDEX-3)) : iNDEX)
#else
#define GPIO_SC_REG_INDEX(iNDEX)    (iNDEX)
#endif

#endif  // GPIO_128_ENTRIES

#define GPIO_LEGACY_REG_AMOUNT  3

typedef enum
{
    GPIO_HW_REG_0 = 0,
    GPIO_HW_REG_1,
#if defined (GPIO_128_ENTRIES)	// In bulverde and boerne we have 4 registers (128 GPIOs)
    GPIO_HW_REG_2,
    GPIO_HW_REG_3,
#endif
    GPIO_HW_MAX_REG_NUMBER
}GPIO_HwRegs;

// Defines the GPIO Controller HW-Structure
struct GpioController
{
    UINT32      GPLR[GPIO_LEGACY_REG_AMOUNT];   //pin level register                    Read (*status)
    UINT32      GPDR[GPIO_LEGACY_REG_AMOUNT];	//pin direction register				Read/Write
    UINT32      GPSR[GPIO_LEGACY_REG_AMOUNT];	//pin output set register				Write
    UINT32      GPCR[GPIO_LEGACY_REG_AMOUNT];	//pin output clear register				Write
#if !defined (GPIO_APP_LAYOUT)  // layouts are different starting at offset 0x30
    UINT32      GSTR[GPIO_LEGACY_REG_AMOUNT];   //source type register (edge/level)     Read/Write
    UINT32      GRHR[GPIO_LEGACY_REG_AMOUNT];   //rising edge/high level detect enable  Read/Write
    UINT32      GFLR[GPIO_LEGACY_REG_AMOUNT];   //falling edge/low level detect enable  Read/Write
    UINT32      GRER[GPIO_LEGACY_REG_AMOUNT];   //rising edge detect register           Read (*status)
    UINT32      GFER[GPIO_LEGACY_REG_AMOUNT];   //falling edge detect register          Read (*status)
    UINT32      GDBR[GPIO_LEGACY_REG_AMOUNT];   //De-Bounce enable register             Read/Write
    UINT32      GDCR;                           //De-Bounce clock control register      Read/Write
    UINT32      GIMR[GPIO_LEGACY_REG_AMOUNT];   //interrupt mask register               Read/Write
    UINT32      GIRR[GPIO_LEGACY_REG_AMOUNT];   //interrupt request register            Read (*status)
    UINT32      GICR[GPIO_LEGACY_REG_AMOUNT];   //interrupt clear register              Write
    UINT32      GSLR;                           //status latch register (dummy)         Write
    UINT32      GPER[GPIO_LEGACY_REG_AMOUNT];   //pull up/down enable register          Write
    UINT32      GUDR[GPIO_LEGACY_REG_AMOUNT];   //pull up/down select register          Write
#else	
	UINT32      GRER[GPIO_LEGACY_REG_AMOUNT];   //rising edge detect register           Read (*status)
    UINT32      GFER[GPIO_LEGACY_REG_AMOUNT];   //falling edge detect register          Read (*status)
	UINT32      GEDR[GPIO_LEGACY_REG_AMOUNT];
#endif

#if !defined(GPIO_SC_REGS_EMBEDDED)
	UINT32		GAFR_FILLER[(0x100-0x54)/4];  // Filler up to offset 0x100
#else
	// Bit-wise set/clear type registers for direction and edge detection configuration
	UINT32		GSDR[GPIO_LEGACY_REG_AMOUNT];	// BitWise set of GPIO direction reg
	UINT32		GCDR[GPIO_LEGACY_REG_AMOUNT];	// BitWise clear of GPIO direction reg
	UINT32		GSRER[GPIO_LEGACY_REG_AMOUNT];	// BitWise set of GPIO rising edge detect reg
	UINT32		GCRER[GPIO_LEGACY_REG_AMOUNT];	// BitWise clear of GPIO rising edge detect reg
	UINT32		GSFER[GPIO_LEGACY_REG_AMOUNT];	// BitWise set of GPIO falling edge detect reg
	UINT32		GCFER[GPIO_LEGACY_REG_AMOUNT];	// BitWise clear of GPIO falling edge detect reg
#if defined(GPIO_CORE_INT_MASK_AP)              // Offset 0x9C
    UINT32      INT_MASK_REG[GPIO_LEGACY_REG_AMOUNT]; // The actual core mask register
#else
    UINT32      INT_MASK_RESERVED_AP[GPIO_LEGACY_REG_AMOUNT];
#endif
#if defined(GPIO_CORE_INT_MASK_CP)              // Offset 0xA8
    UINT32      INT_MASK_REG[GPIO_LEGACY_REG_AMOUNT]; // The actual core mask register
#else
    UINT32      INT_MASK_RESERVED_CP[GPIO_LEGACY_REG_AMOUNT];
#endif
	UINT32		GAFR_FILLER[(0x100-0xb4)/4];  // Filler up to offset 0x100
#endif
	// More registers exist - of 2 types:
	// 1. extra 4th register for each of the above types GPLR/GPDR/GPSR/ ... in offest 0x100 of the reg0 in the type
	//	  this is covered by the macro GPIO_REG_INDEX  (in Bulverde and in Boerne)
	// 2. special registers to set/clear of specific pins without affecting the other pins:
	//    (GSDR, GCDR, GSRER, GCRER, GSFER, GCFER) (BOERNE only)
#if (GPIO_HW_REG_AMOUNT>3)	// Additional registers 
	UINT32		GPLR3;			// pin level register #4 - accessed via the macro GPIO_REG_INDEX from GPLR
	UINT32		GPLR_FILLER[2];	// reserved registers
	UINT32		GPDR3;			// pin direction register #4 - accessed via the macro GPIO_REG_INDEX from GPDR
	UINT32		GPDR_FILLER[2];	// reserved registers
	UINT32		GPSR3;			// pin output set register #4 - accessed via the macro GPIO_REG_INDEX from GPSR
	UINT32		GPSR_FILLER[2];	// reserved registers
	UINT32		GPCR3;			// pin output clear register #4 - accessed via the macro GPIO_REG_INDEX from GPCR
	UINT32		GPCR_FILLER[2];	// reserved registers
	UINT32		GRER3;			// rising edge detect register #4 - accessed via the macro GPIO_REG_INDEX from GRER
	UINT32		GRER_FILLER[2];	// reserved registers
	UINT32		GFER3;			// falling edge detect register #4 - accessed via the macro GPIO_REG_INDEX from GFER
	UINT32		GFER_FILLER[2];	// reserved registers
	UINT32		GEDR3;			// Edge Detect status register #4 - accessed via the macro GPIO_REG_INDEX from GEDR
	UINT32		GEDR_FILLER[2];	// reserved registers
#endif

#if defined(GPIO_SETCLEAR_ACCESS) && !defined(GPIO_SC_REGS_EMBEDDED)
	// Bit-wise set/clear type registers for direction and edge detection configuration
	UINT32		FILLER[(0x400-0x154)/4];		// reserved (0x40e00154 -> 0x40e00400)
	UINT32		GSDR[GPIO_HW_REG_AMOUNT];	// BitWise set of GPIO direction reg
	UINT32		GSDR_FILLER[4];	// reserved registers
	UINT32		GCDR[GPIO_HW_REG_AMOUNT];	// BitWise clear of GPIO direction reg
	UINT32		GCDR_FILLER[4];	// reserved registers
	UINT32		GSRER[GPIO_HW_REG_AMOUNT];	// BitWise set of GPIO rising edge detect reg
	UINT32		GSRER_FILLER[4];	// reserved registers
	UINT32		GCRER[GPIO_HW_REG_AMOUNT];	// BitWise clear of GPIO rising edge detect reg
	UINT32		GCRER_FILLER[4];	// reserved registers
	UINT32		GSFER[GPIO_HW_REG_AMOUNT];	// BitWise set of GPIO falling edge detect reg
	UINT32		GSFER_FILLER[4];	// reserved registers
	UINT32		GCFER[GPIO_HW_REG_AMOUNT];	// BitWise clear of GPIO falling edge detect reg
	UINT32		GCFER_FILLER[4];	// reserved registers
#endif
};

#define     GpioHWRegisters    (* (volatile struct GpioController *) GPIO_HW_ADDRESS)

/* To be used by INTC package only */
// A dummy write should be done before reading any status register: GPLR, GRER, GFER, GIRR

#define GPIOReadPinLevelRegister(p)             (GPIODummyWrite \
                                                 GpioHWRegisters.GPLR[GPIO_REG_INDEX(p)])

#define GPIOReadRisingEdgeDetectRegister(p)     (GPIODummyWrite \
                                                 GpioHWRegisters.GRER[GPIO_REG_INDEX(p)])

#define GPIOReadFallingEdgeDetectRegister(p)    (GPIODummyWrite \
                                                 GpioHWRegisters.GFER[GPIO_REG_INDEX(p)])

#if defined(GPIO_INT_MASK_REQ_REGS)
#define GPIOReadRequestRegister(p)    (GPIODummyWrite \
                                                 GpioHWRegisters.GIRR[p])
                                                 
#define GPIOReadMaskRegister(p)    (GPIODummyWrite \
                                                 GpioHWRegisters.GIMR[p])
#endif

#if !defined (GPIO_APP_LAYOUT)     /*---------------------------------*/
#define GPIODummyWrite                           GpioHWRegisters.GSLR = 0,

#define GPIOReadInterruptStatusReg(p)           (GPIODummyWrite \
                                                 GpioHWRegisters.GIRR[p])
// Micha, Name should Changed to GPIOClearInterruptReg, the additional dummy write is for able to see correct info via Debugger
//#define GPIOClearEdgeDetectionStatusReg(p,b)    (GpioHWRegisters.GICR[p] = b, GPIODummyWrite)
// REMOVED: Dummy Write after a write is not according to spec (obviously used once for debug as explained above)
// This case also complicated the macro-definition (problem with comma).
#define GPIOClearEdgeDetectionStatusReg(p,b)    (GpioHWRegisters.GICR[p] = b)

#else/*INTEL_2CHIP_PLAT_BVD-------------------------------------------*/
#define GPIODummyWrite                          

#define GPIOReadInterruptStatusReg(p)           (GpioHWRegisters.GEDR[GPIO_REG_INDEX(p)])

#define GPIOClearEdgeDetectionStatusReg(p,b)    (GpioHWRegisters.GEDR[GPIO_REG_INDEX(p)] = (b))
#endif/*INTEL_2CHIP_PLAT_BVD-------------------------------------------*/


/* Clear Interrupt flag of GPIO Pin - GPIO Pin parameter is
   a GPIO pin from GPIO list */
#define GPIOInterruptClear(gpioPin)     (GPIOClearEdgeDetectionStatusReg((gpioPin)>> GPIO_SHIFT_PINS_PER_REG, \
                                                                         (0x01UL  << ((gpioPin) & GPIO_SHIFT_PINS_MASK))))


/* GPIO-INTC non-public interface: moved to GPIO_intc.h file due to compilation issues
GPIO_ReturnCode GPIOEdgeDetectionEnable(GPIO_PinNumbers pinNum);
GPIO_ReturnCode GPIOEdgeDetectionDisable(GPIO_PinNumbers pinNum);
BOOL            GPIOSetUsedForInterrupt(GPIO_PinNumbers pinNum);
void            GPIOClearUsedForInterrupt(GPIO_PinNumbers pinNum);
*/


#endif /* _GPIO_HW_H_ */


