/************************************************************************
*
*               Copyright Mentor Graphics Corporation 2005
*                           All Rights Reserved.
*
*   THIS WORK CONTAINS TRADE SECRET AND PROPRIETARY INFORMATION WHICH IS
*   THE PROPERTY OF MENTOR GRAPHICS CORPORATION OR ITS LICENSORS AND IS
*   SUBJECT TO LICENSE TERMS.
*
*************************************************************************

*************************************************************************
*
* FILE NAME                                 VERSION
*
*       nucleus.h               Nucleus PLUS\IQ80310\RVCT 1.15.3
*
* COMPONENT
*
*       System Constants
*
* DESCRIPTION
*
*       This file contains system constants common to both the
*       application and the actual Nucleus PLUS components.  This file
*       also contains data structure definitions that hide internal
*       information from the application.
*
* DATA STRUCTURES
*
*       None
*
* DEPENDENCIES
*
*       None
*
************************************************************************/
/* Check to see if this file has been included already.  */

#ifndef         NUCLEUS_SPECIFIC
#define         NUCLEUS_SPECIFIC

#define NU_PLUS_RELEASE_STRING "Copyright MGC 2005 - Nucleus PLUS - IQ80310 RVCT v. 1.15"

#define         NU_SUPERV_USER_MODE     0
#define         NU_MODULE_SUPPORT       0
#define         NU_MMU_MODE             0

/* MMU mode requires the module support */
#if (NU_MMU_MODE == 1)

#undef          NU_MODULE_SUPPORT
#define         NU_MODULE_SUPPORT       1

#endif

/* Module support requires mode switching */
#if (NU_MODULE_SUPPORT == 1)

#undef          NU_SUPERV_USER_MODE
#define         NU_SUPERV_USER_MODE     1

#endif

/* Describe the static base register, needed by DDL, for calling 
** services. This also avoids allocating SB (=v6) to any other purpose. */
__global_reg(6) void *sb;

/* Define the clock frequency that is used by the timer control unit (must be defined in hertz */
#define NU_System_Clock_Frequency       33000000

/* Define the interval between successive PLUS ticks (must be stated in units of microseconds) */
#define NU_PLUS_Tick_Rate               10000

/* Define the number of Nucleus PLUS ticks that will occur every second */
#define NU_PLUS_Ticks_Per_Second        (1000000 / NU_PLUS_Tick_Rate)

/* Define the number of timer ticks that will occur every second.  This is
   usually the frequency of the timer used by the Nucleus clock.  It may
   be modified by a prescalar.  See the INT assembler file for more info */
#define NU_HW_Ticks_Per_Second          NU_System_Clock_Frequency

/* The number of timer ticks between Nucleus timer interrupts that increment
   TMD_System_Clock.  See timer initialization code for details about how to
   determine this number. */
#define NU_HW_Ticks_Per_SW_Tick         (NU_HW_Ticks_Per_Second / NU_PLUS_Ticks_Per_Second)

/* Define NU_COUNT_DOWN if the system timer counts down to 0.
   This macro is used to switch between formulas to calculate
   the number of ticks since the systems started in NU_Get_Clock */
#define NU_COUNT_DOWN

/* These macros returns the value of the Nucleus timer (not the system clock)
   Due to the method of reading the timer value on the IQ80310 being a little
   more complicated then reading a single register value on other boards,
   we needed multiple macros that called its sub macros to return the 
   timer value */

#define NU_TIMER_COUNT_MASK 0x5F /* 6 bits of timer data with the MSB in bit 6 not bit 5 */
#define NU_TIMER_LA0_REG_ADDR (volatile unsigned char *)0xFE880000
#define NU_TIMER_LA1_REG_ADDR (volatile unsigned char *)0xFE890000
#define NU_TIMER_LA2_REG_ADDR (volatile unsigned char *)0xFE8A0000
#define NU_TIMER_LA3_REG_ADDR (volatile unsigned char *)0xFE8B0000
#define	NU_Timer_Cnt0  (*NU_TIMER_LA0_REG_ADDR & NU_TIMER_COUNT_MASK)						
#define	NU_Timer_Cnt1  (*NU_TIMER_LA1_REG_ADDR & NU_TIMER_COUNT_MASK)						
#define	NU_Timer_Cnt2  (*NU_TIMER_LA2_REG_ADDR & NU_TIMER_COUNT_MASK)						
#define	NU_Timer_Cnt3  (*NU_TIMER_LA3_REG_ADDR & 0xF)/* only 4 bits in most sig. */		
#define	NU_Timer_Byte0 (((NU_Timer_Cnt0 & 0x20) >> 1) | (NU_Timer_Cnt0 & 0x1F))
#define	NU_Timer_Byte1 (((NU_Timer_Cnt1 & 0x20) >> 1) | (NU_Timer_Cnt1 & 0x1F))
#define	NU_Timer_Byte2 (((NU_Timer_Cnt2 & 0x20) >> 1) | (NU_Timer_Cnt2 & 0x1F))
#define NU_Timer_Value ((NU_Timer_Cnt3 << 18) | (NU_Timer_Byte2 << 12) | (NU_Timer_Byte1 << 6) | NU_Timer_Byte0)																
#define NU_Retrieve_Hardware_Clock(hw_tick) hw_tick = NU_Timer_Value

/* Define the number of accesses required to read or write a pointer data
   type.  This value is used to make optimizations in some ports of Nucleus
   PLUS.  */
   
#define         NU_POINTER_ACCESS       1

/* Define the padding required for usage of the DATA_ELEMENT type within 
   Nucleus PLUS structures.  These values insure proper alignment for the
   next structure member.  */
   
#define         PAD_1                   3
#define         PAD_2                   2
#define         PAD_3                   1

/* Define constants that are target dependent and/or are used for internal
   purposes.  */

#define         NU_MIN_STACK_SIZE       240
#define         NU_MAX_NAME             8
#define         NU_MAX_VECTORS          20
#define         NU_MAX_LISRS            8

/* Define constants for the number of UNSIGNED words in each of the basic
   system data structures.  */

#if ((NU_SUPERV_USER_MODE == 1)||(NU_MODULE_SUPPORT == 1))

#define         NU_TASK_SIZE            44
#define         NU_HISR_SIZE            24

#else

#define         NU_TASK_SIZE            42
#define         NU_HISR_SIZE            22

#endif

#define         NU_MAILBOX_SIZE         13
#define         NU_QUEUE_SIZE           18
#define         NU_PIPE_SIZE            18
#define         NU_SEMAPHORE_SIZE       10
#define         NU_EVENT_GROUP_SIZE     9
#define         NU_PARTITION_POOL_SIZE  15
#define         NU_MEMORY_POOL_SIZE     17
#define         NU_TIMER_SIZE           17
#define         NU_PROTECT_SIZE         2

/* Define interrupt lockout and enable constants.  */

#define         NU_DISABLE_INTERRUPTS   0xC0
#define         NU_ENABLE_INTERRUPTS    0x00

#endif  /* !NUCLEUS */

