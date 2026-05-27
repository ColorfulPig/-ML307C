/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

//Copyright 2005, Intel Corporation, All rights reserved.
/************************************************************************/
/*   Utils.h - 															*/
/* 																		*/
/************************************************************************/

#ifndef UTILS_H
#define UTILS_H

#include "global_types.h"
//should be used in BootLoader & Flasher ONLY
//#define LOW_LEVEL_ASSERTS_ONLY

#if defined (_TAVOR_HARBELL_)
// To Be Deleted...
#define Manitoba_Create_HISR    Os_Create_HISR   /*currently refered from diag_comm_SSP.c. Correct reference should be bsp_hisr.h*/
#endif


typedef enum
{
	CPU_HERMON_B0 = 0,
	CPU_HERMON_B1,
	CPU_HERMON_B2,
	CPU_HERMON_B3,
	CPU_HERMON_TCB874,
	CPU_HERMONL_A0,
	CPU_HERMONC_A0,
	CPU_HERMON_TCC874,
	CPU_HERMONEL_A0,
	CPU_MANITOBA_OTHER,
	CPU_BVD,
	CPU_TAVOR_A0,
	CPU_TAVOR_B0,
	CPU_TAVOR_B1,
	CPU_TAVOR_B2,
	CPU_TAVOR_PV_A0,
	CPU_TAVOR_PV_B0,
	CPU_TAVOR_PV_C0,
	CPU_TTC,
	CPU_OTHER
}CPU_Version;  //if this enum changed, update also the CPU_Version_str[]

// Returns the CPU version according to the above list
CPU_Version GetCpuVersion(void);

// Returns the original CPSR
// exp: old_level = disableInterrupts();
// old_level MUST be local automatic variable !!
unsigned long disableInterrupts(void);

// Returns the original CPSR
// exp: old_level = enableInterrupts();
// old_level MUST be local automatic variable !!
unsigned long enableInterrupts(void);

// Restores the IF bits in the CPSR to that value of the CPSR passed.
// The latter should be obtained from the enable/disable functions
// exp: restoreInterrupts(old_level);
// old_level MUST be local automatic variable !!
void restoreInterrupts(unsigned long ir);

// Count Leading Zeros
// Returns: 0 for 0x8xxxxxxx; 1 for 0x04xxxxxx; 31 for 0x00000001; 32 for 0
int _clz(unsigned long x);

// revert
// Returns: reverted endian value.change the order of bytes in the 32bit parameter from big to little endian and vice versa.
unsigned long _rev(unsigned long x);

// CP14 functions
void _xsFreqChange(void);

// Enter idle mode
void _xsGoIdle(void); //just idle the Xscale core
void setIdle(void);   //same as previous
void setIdleExt(UINT32 newXPCR, UINT32 oldXPCR); //idle the core with shutting down the MEMC and modifying PMU.XPCR

//
// General: soft-restart the image
//
void doRestart(void);

// Function analog of ASSERT
void fatalError(int condition);


// Assert macros
#ifndef LOW_LEVEL_ASSERTS_ONLY

extern void utilsAssertFail(const char      *cond,
                            const char      *file,
                            signed short    line,
                            unsigned char   allowDiag);

//regular ASSERTs
#if defined __MODULE__
 #define WARNING(cOND)       { if (!(cOND))  { utilsAssertFail(NULL , __MODULE__, __LINE__, 0xFF); }  }
 #define ASSERT(cOND)        { if (!(cOND))  { utilsAssertFail(#cOND, __MODULE__, __LINE__, 1);    }  }
#else
 #define WARNING(cOND)       { if (!(cOND))  { utilsAssertFail(NULL , __FILE__,   __LINE__, 0xFF); }  }
 #define ASSERT(cOND)        { if (!(cOND))  { utilsAssertFail(#cOND, __FILE__,   __LINE__, 1);    }  }
#endif
#define ASSERT_NO_DIAG(cOND) { if (!(cOND))  { utilsAssertFail(#cOND, __FILE__,   __LINE__, 0);    }  }

#else//LOW_LEVEL_ASSERTS_ONLY

/*Make ALL ASSERTS as ASSERT_LOW_LEVEL*/
#define WARNING(cOND)           /*WARNING*/
#define ASSERT(cOND)            {ASSERT_LOW_LEVEL(cOND,0xAA)}
#define ASSERT_NO_DIAG(cOND)    {ASSERT_LOW_LEVEL(cOND,0xAA)}

#endif /*LOW_LEVEL_ASSERTS_ONLY*/

#define ASSERT_LOW_LEVEL(cOND,vALUE)    {\
                                            if (!(cOND)) \
                                            { \
                                                disableInterrupts(); \
                                                while (1);\
                                            } \
                                        }

#define fatalError(number)  ASSERT(FALSE)

#define  inbyte(addr)        (*((volatile UINT8  *)addr))
#define  inword(addr)        (*((volatile UINT16 *)addr))
#define  inlong(addr)        (*((volatile UINT32 *)addr))


#define  outbyte(addr,val)   (*((volatile UINT8  *)addr)=(UINT8)val)
#define  outword(addr,val)   (*((volatile UINT16 *)addr)=(UINT16)val)
#define  outlong(addr,val)   (*((volatile UINT32 *)addr)=(UINT32)val)


//
// CP14: Performance monitoring unit access
//

// Read/Set PMNC (Control Register, see Elkart core EAS chapter 8)
// Here are the bit definitions:
#define PMNC_ALL_ENABLE    1
#define PMNC_ALL_RESET     2
#define PMNC_CCNT_RESET    4
#define PMNC_CCNT_DIV64    8
#define PMNC_STOP_MASK	   0xFFFFFFF0 //Stop all counters
#define PMNC_RESTR_MASK    0xFFFFFFE7 //Enable all counters+reset CCNT and Performance regs
#define PMNC_START_MASK    0xFFFFFFE1 //Enable all clocks without reseting clk


#define PMNC_ID_MASK       0xFF000000  /* Returns 0x14 for Elkart */
void   cp14SetPMNC(UINT32 value);
UINT32 cp14ReadPMNC(void);

// Read the Clock Counter register (core clock or same/64 depending on the PMNC_CCNT_DIV64 bit - below)
UINT32 cp_ReadCCNT(void);  // NEW generic name. OLD & NEW are aliased
UINT32 cp14ReadCCNT(void); // OLD non-generic name, to be obsolete.
UINT32 cp14SetCCNT(UINT32 value);
UINT32  cp14ReadEVTSEL(void);
void  cp14SetEVTSEL(UINT32 value);

UINT32 getCpRateKHz(void); // returns CP-counter rate in kHz or 0-unknown/default. Depends upon Core frequency


//
// CP6: WS Primary INTC co-processor bus access
//

UINT32 cp6ReadICPR(void);
UINT32 cp6ReadICIP(void);
UINT32 cp6ReadICFP(void);
UINT32 cp6ReadICHP(void);
UINT32 cp6ReadICMR(void);
void cp6WriteICMR(UINT32 value);

#define TOGGLE_REG_SET_BITS_ON(r, b)      ((r) = ~(r)&(b))
#define TOGGLE_REG_SET_BITS_OFF(r, b)     ((r) &= (b))
#define ARE_BITS_ON(r,b)                  ((b) == ((r)&(b)))
#define ARE_BITS_OFF(r,b)                 (0x0 == ((r)&(b)))


#if defined (_TAVOR_HARBELL_)
//ON-CHIP trace buffer is not supported on TAVOR A0 but on B0 only with JTAG protocol
// Let's put macro-stubs meanwhile
#define EnableTraceBuffer(vOID)     /**/
#define DisableTraceBuffer(vOID)    /**/
#define ReadTraceBuffer(pBUFFER)    pBUFFER = NULL
//#define ReadTBREG(x)                0
//#define ReadCHKPT1(x)               0
//#define ReadCHKPT0(x)               0
#else
/* XSCALEs*/
// Add Tavor_Boerne interrupts Coprocessor access - it has 52 interrupts (and not only 32)
UINT32 cp6ReadICPR2(void);
UINT32 cp6ReadICIP2(void);
UINT32 cp6ReadICFP2(void);
UINT32 cp6ReadICMR2(void);
void cp6WriteICMR2(UINT32 value);
void EnableTraceBuffer(void);
void DisableTraceBuffer(void);
void ReadTraceBuffer(UINT32 *Buffer);

UINT8  ReadTBREG(void);
UINT32 ReadCHKPT1(void);
UINT32 ReadCHKPT0(void);
#endif//_TAVOR_HARBELL_  /* XSCALE only */

// CPSR mode
#define CPSR_MODE_BITS 0x1F
#define CPSR_MODE_USER 0x10
#define CPSR_MODE_FIQ  0x11
#define CPSR_MODE_IRQ  0x12
#define CPSR_MODE_SVC  0x13
#define CPSR_MODE_ABT  0x17
#define CPSR_MODE_UND  0x1B
#define CPSR_MODE_SYS  0x1F
#define CPSR_T_BIT     0x20 //Thumb
#define CPSR_IRQ_BIT   0x80 // IRQ enable
#define CPSR_FIQ_BIT   0x40 // IRQ enable


UINT32 ReadSP(void);
UINT32 Read_SPSR(void);
UINT32 ReadCPSR(void);
UINT32 ReadMode_R13(UINT32 mode);
UINT32 ReadMode_R14(UINT32 mode);

// Set SP for the CPU mode specified by CPSR
void SetMode_R13(UINT32 mode, UINT32 sp);

// Set SP and SL (v7) for the current CPU mode
void SetSystemStack(UINT32 sp, UINT32 limit);

// Reads the r0-r14,pc,cpsr values into the given buffer (see EE_RegInfo_Data_t)
void   ReadRegisterContext(UINT32* pBuffer);

// Restores r0-r13,pc,cpsr values from the given buffer (see EE_RegInfo_Data_t)
// LR is not restored!
// Jumps to pBuffer->PC
void   RestoreRegisterContext(UINT32* pBuffer);

// Restores r0-r12 values from the given buffer (see EE_RegInfo_Data_t)
// r13, LR, CPSR are not restored!
// Returns from exception mode and jumps to pBuffer->PC
void   RestoreRegisterContextEx(UINT32* pBuffer, UINT32 setExcModeSP);

#if !defined (_TAVOR_HARBELL_) /* XSCALE only */
//read ICAM/OCAM register
//register_index  what ICAMs to read READ: CRR[0..2]:
//returns ICAM/OCAM register value
UINT32 mcpComRead(UINT32 register_index);

//
// DEBUG CAPABILITIES ACCESS
//
void  dbgSetIBCR0(UINT32 value);
UINT32 dbgReadIBCR0(void);
void  dbgSetIBCR1(UINT32 value);
UINT32 dbgReadIBCR1(void);
void  dbgSetDBR0(UINT32 value);
UINT32 dbgReadDBR0(void);
void  dbgSetDBR1(UINT32 value);
UINT32 dbgReadDBR1(void);
void  dbgSetDBCON(UINT32 value);
UINT32 dbgReadDBCON(void);
void  dbgSetDCSR(UINT32 value);
UINT32 dbgReadDCSR(void);
#endif//_TAVOR_HARBELL_  /* XSCALE only */

void	doTurboFrequencyChange(UINT32 fBit,UINT32 tBit);
UINT32	GetTurboFrequencyChangeCfgBits(void *pRegAddress);
UINT32	RunOperationUnderSpecificStack_ASM(void *pFuncAddress,void *pStackAddress, UINT32 funcParam1);

#endif // UTILS_H

