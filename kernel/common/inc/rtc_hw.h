/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2006, 2007 Marvell DSPC Ltd. All Rights Reserved.
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*                MODULE HEADER FILE
*******************************************************************************
*  COPYRIGHT (C) 2001 Intel Corporation.
*
*  This file and the software in it is furnished under
*  license and may only be used or copied in accordance with the terms of the
*  license. The information in this file is furnished for informational use
*  only, is subject to change without notice, and should not be construed as
*  a commitment by Intel Corporation. Intel Corporation assumes no
*  responsibility or liability for any errors or inaccuracies that may appear
*  in this document or any software that may be provided in association with
*  this document.
*  Except as permitted by such license, no part of this document may be
*  reproduced, stored in a retrieval system, or transmitted in any form or by
*  any means without the express written consent of Intel Corporation.
*
*  Title: rtc
*
*  Filename: rtc_hw.h
*
*  Target, subsystem: Common Platform, HAL
*
*  Authors:  Avi Erami, Eilam Ben-Dror
*
*  Description:  rtc hardware header file.
*
*  Last Modified: 24.07.2001
*
*  Notes:
******************************************************************************/

#ifndef _RTC_HW_H_
#define _RTC_HW_H_
#include "rtc.h"

#if 0
#ifdef _MANITOBA_SILICON_
    #define RTC_HW_ADDRESS          0x42900000L
    #define RTC_HW_RESET_ADDRESS    0x42A00C00L

#else
#ifdef SILICON_TTC
	#define RTC_HW_ADDRESS          0xD4010000L
    #define RTC_HW_RESET_ADDRESS    0xD4015024L
#else
    /* _HERMON_ and TAVOR*/
    #define RTC_HW_ADDRESS          0x40900000L
    #define RTC_HW_RESET_ADDRESS    0x42900064L				/* ACR0 */
#endif /* _TTC_ */
#endif /* _MANITOBA_ */
#endif

#define RTC_HW_ADDRESS			0xD4010000L
#define RTC_HW_RESET_ADDRESS	0xD4015028L
#define RTC_SCS_HW_ADDRESS	0xD40C0000L



#define RTC_DEFAULT_TRIM 0x00007FFFL



// Defines the RTC Controller HW-Structure
struct RtcController
{
    UINT32      RCNR;	//count register			Read/Write
    UINT32      RTAR;	//alarm register			Read/Write
    UINT32      RTSR;	//status register			Read/Write
    UINT32      RTTR;	//trim register				Read/Write
    UINT32      RTPR;	//Pwr Island ctrl register		Read/Write
    UINT32      RTBR0;	//Backup register				Read/Write
    UINT32      RTBR1;	//Backup register				Read/Write
    UINT32      RTBR2;	//Backup register				Read/Write
    UINT32      RTBR3;	//Backup register				Read/Write
    UINT32      RTBR4;	//Backup register				Read/Write
};

struct RtcScsController
{
    UINT32      RCNR;	//count register			Read/Write
    UINT32      RTAR;	//alarm register			Read/Write
    UINT32      RTTR;	//trim register				Read/Write
    UINT32      CNTL;	//ctrl register				Read/Write
                                //[2] RTC alarm polar
                                //	0 = RTC alarmwakeup signal high active
                                //	1 = RTC alarmwakeup signal low active
                                //[1] RTC alarm clear
                                //	Write 1 = clear RTC alarmwakeup signal
                                //[0] RTC alarm enable
                                //	0 = enable
                                //	1 = disable

    UINT32      IDAC;	//IDAC code to RF module 		Read/Write
    UINT32      RSYN;	//sync shadow registers control		Read/Write
				// [10:8] Read Only, FSM state (just for debug)
				// [7] Read Only, register synchronized done (just for debug)
				// [6] clear RTC_sync_done interrupt (Write 1 = clear RTC_sync_done interrupt)
				// [5] synchronize idac_code register
				// [4] synchronize control register
				// [3] synchronize trim register
				// [2] synchronize alarm register
				// [1] synchronize time counter register
				// [0] trigger synchronizationfrom shadow registers to real registers in AON power domain

    UINT32      RGAT;	//auto clock gating PCLK in DCS mode	Read/Write
    UINT32      DCSM;	//dcs mode detect from PAD		Read

    //shadow registers
    UINT32      SCNR;	//shadow count register			Read/Write
    UINT32      STAR;	//shadow alarm register			Read/Write
    UINT32      STTR;	//shadow trim register			Read/Write
    UINT32      STPR;	//shadow Pwr Island ctrl register	Read/Write
};

#define SCS_BIT_CNTL_ALARM_POLA              (0x1<<2)
#define SCS_BIT_CNTL_ALARM_CLEAR             (0x1<<1)
#define SCS_BIT_CNTL_ALARM_EN                (0x1<<0)

#define SCS_BIT_SYNC_RSYN_CLEAR_SYNC_INTC    (0x1<<6)
#define SCS_BIT_SYNC_RSYN_IDAC               (0x1<<5)
#define SCS_BIT_SYNC_RSYN_CONTROL            (0x1<<4)
#define SCS_BIT_SYNC_RSYN_TRIM               (0x1<<3)
#define SCS_BIT_SYNC_RSYN_ALARM              (0x1<<2)
#define SCS_BIT_SYNC_RSYN_COUNTER            (0x1<<1)
#define SCS_BIT_SYNC_RSYN_TRIGGER            (0x1<<0)
#define SCS_BIT_SYNC_RSYN_MASK               (0x3F) //bits able to be config

#define     RtcHwRegisters    (* (volatile struct RtcController *) RTC_HW_ADDRESS)
#define     RtcScsHwRegisters (* (volatile struct RtcScsController *) RTC_SCS_HW_ADDRESS)
#define     RTCResetRegister  (* (volatile UINT32 *) RTC_HW_RESET_ADDRESS)

#ifdef SILICON_TTC
	#define RTC_RESET_MASK                  0x4L       // There is only one RST bit in TTC
#else
    #define RTC_RESET_MASK                  0xCL
    #define RTC_TRIM_RESET_MASK             0x2L
#endif

#define RTC_CTL_STOP		(RTCResetRegister=0x80L) // stop RTC counter
#define RTC_CTL_START		(RTCResetRegister=0x83L) // release RTC counter to keep counting
#define RTC_CTL_ZERO_STOP	(RTCResetRegister=0x04L) // reset RTC counter to 0 ,and stop counter

#if 0
#if defined (_TAVORB0_AccessMethodToRcnrRdcrRycrRttrRegistersChanged)
/* in Tavor B0 writing to RCNR(and also RDCR, RYCR and RTTR) changed according to
Tavor-P-B0_ChangesList.PDF chapter 5.22*/

#define PSBR_ADD (0x40F500A0)
#define PSBR_WRITE_DELAY_IN_USEC 80 /*required 2 cycles of 32K, 80u to be of the safe side*/

#define RTC_RCNR_WRITE(val)\
{\
	*(volatile UINT32*)PSBR_ADD|=0x1;\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	RtcHwRegisters.RCNR=(val);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	*(volatile UINT32*)PSBR_ADD&=(~0x1);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
}

#define RTC_RDCR_WRITE(val)\
{\
	*(volatile UINT32*)PSBR_ADD|=0x1;\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	RtcHwRegisters.RDCR=(val);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	*(volatile UINT32*)PSBR_ADD&=(~0x1);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
}

#define RTC_RYCR_WRITE(val)\
{\
	*(volatile UINT32*)PSBR_ADD|=0x1;\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	RtcHwRegisters.RYCR=(val);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	*(volatile UINT32*)PSBR_ADD&=(~0x1);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
}

#define RTC_RTTR_WRITE(val)\
{\
	*(volatile UINT32*)PSBR_ADD|=0x1;\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	RtcHwRegisters.RTTR=(val);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
	*(volatile UINT32*)PSBR_ADD&=(~0x1);\
	timerDelay(PSBR_WRITE_DELAY_IN_USEC);\
}
#endif

#ifdef _MANITOBA_SILICON_
    #define RTC_ALARM_INTERRUPT_SOURCE      INTC_SRC_GPIO_59
    #define RTC_ALARM_GPIO_SOURCE           GPIO_PIN_59

#else  /* Hermon  */

    #define RTC_ALARM_INTERRUPT_SOURCE      INTC_SRC_RTC_ALARM
    #define RTC_ALARM_INTERRUPT_POS         (INTC_SRC_RTC_ALARM - INTC_MAX_PRIMARY_INTERRUPT_SOURCES)

#endif /* _MANITOBA_ */
#endif
#endif /* _RTC_HW_H_ */
