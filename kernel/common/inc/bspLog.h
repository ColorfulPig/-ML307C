/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/********************************************************************************

  Module:  bspLog

  Autor: Yan Markman

  Creation Date: 24.06.2008
------------------------------

 OTHER Logger and their constrains
 ----------------------------------
 For Target debugging we have
 - MIPS-RAM logger
 - DIAG ACAT traces

 The MIPS-RAM
  logs into RAM all OS-context switches and user-defined events.
  The only problem - there are a lot of events so buffer is enought for about 400 MSec.

 The DIAG
 - is not always present on Target (bring-up for example)
 - sends a lot of traces
 - requires a lot of resources and MIPS
 - sent to PC over USB/UART port

 REQUIREMENTS to the bspLog
 ----------------------------------
 Therefore the next LOGGER is required.
 1. Uses minimum resources and MIPS
 2. Always present in the basic Target package (in BSP)
 3. Keep very reduced number of events
 4. Trace enought for a long time but

 Additional requirement for Target having NVM
 A. Save the trace in a file
 B. The save should be reliable (no lost information)
 C. File is kept from restart to restart, new information is add to it
 D. File size is limited.

 The analysis tool is out of the scope.

===================================================================================
 DESCRIPTION
 -----------
 The logger has 2 parts
 - bspLogRecord     to meet requirements {1,2,3,4}
 - bspLogSave2file  to meet requirements {A,B,C}

 DESCRIPTION  bspLogRecord
 ------------------------
 All trace messages have fixed 32-byte format (refer BspLogRecord type)
     const char[20] __FILE__, const uint32 __LINE__,
     and 1 user-variable of uint32 and one TimeStamp variable
 The messages are recorded into bspLogBuf ring-buffer having 256 entries.
 The saving is done on the context of "caller". No other special contexts.

  NOTE:
   If Target is "multi-CPU" with shared-memory between CPUs
   the save-by-mate-CPU may be done instead of the direct "bspLogSave2file".
   In that case the buffer bspLogBuf[] must be placed into the shareable NON-CACHEABLE area!
 -----------------------------------------------------------------------------------------------

 DESCRIPTION  BspLogSave2file
 ----------------------------
 The save is registered for EE-Handler Postmortem process, so save buffer uppon assert is guarantied.
 The Rn-Time Save is optional. It is done by procedure bspLogSave2file()
 bspLogSave2file() has no its own task-context, but should be called by "somebody else".
 May be called "automatically" from a low-priority task or "manually" by ACAT-command (on a diag-context).

 !!! It is resposibility of the caller-task to call the bspLogSave2file() NOT FREQUENTLY

  The file-size should be limited but we have no possibility to write into it like into cyclic-Buffer.
  But we could use 2 files just like a double buffer:
    when one file is full, start to write into the other from beginning;
    and vice versa.

 The save logic
 --------------
  if number of  new_records ==0     then do nothing but exit
  The write to file is like cyclic-buffer.
  "End Marker" is used inside of file and points onto the next free.
  It is also used "between restarts" in the file.
  On restart the SW is looking for the "End Marker" and keep it in global variable.
  The global is used for the record save to file and incremented (with wrap upon the MAX file size.

*******************************************************************************/

#include "hal_cfg.h"
#include "utils.h"
#include "timer.h"

#if !defined (BSP_LOG_H)
#define BSP_LOG_H

//=={{{ ENABLE / DISABLE feature
//#if defined(_TAVOR_HARBELL_) || defined(_TAVOR_BOERNE_) || defined(SILICON_PV2)
#if defined(_TAVOR_BOERNE_) || defined(SILICON_PV2)
#define BSP_LOG_RECORD_ENABLE
#endif

#if defined(BSP_LOG_RECORD_ENABLE) && defined(INTEL_FDI)
#define BSP_LOG_SAVE2FILE_ENABLE
#endif
//==}}} ENABLE / DISABLE feature


#if defined(BSP_LOG_SAVE2FILE_ENABLE)
   void BspLogSave2file(void);
#else
#define BspLogSave2file()         /*nothing*/
#endif


#if !defined (BSP_LOG_RECORD_ENABLE)
//----------- GLOBAL APIs -----------------------------------------------------------------
#define PRINT_BSPLOG_FAST(pARAM1,pARAM2)     /* pARAM2=OldTimeStampFromRam - may be used in Interrupt */
#define PRINT_BSPLOG(pARAM1,pARAM2)          /* pARAM2=TimeStamp          CANNOT be used in interrupt */
#define PRINT_BSPLOG_TXT(TXT14c,pARAM1,pARAM2)  /*like PRINT_BSPLOG but with text instead File-Name*/

// Error:  #144: a value of type "char [22]" cannot be used to initialize an entity of type "char[20]"
//   means that your TEXT is too long. The max text size is 24 bytes not including end-zero.

#define BspLogPhase2Init()    /*void BspLogPhase2Init(void);*/
#define BspLogPhase3Init()    /*void BspLogPhase3Init(void);*/
//---------------------------------------------------
#else//BSP_LOG_RECORD_ENABLE
//---------------------------------------------------

typedef unsigned long   long   Ulong64;  //same as UINT64

typedef struct
{
    //--- CONST part ----- align 32
    UINT16  lineNo;
    char    idString[30];
}BspLogRecordCONST;

//ICAT EXPORTED STRUCT
typedef struct
{
    UINT16  lineNo;         // CONST
    char    idString[14];   // CONST = 16 bytes
    UINT32  timeStamp;      // var
    UINT32  param1;         // var
    UINT32  param2;         // var
    UINT32  param3;         // var
}BspLogRecord;              // ======= 32 bytes

typedef struct
{
    UINT8          wrIdx;     //BYTE-UINT8 index used for auto-wrap on 255++
    UINT8          rdIdx;
    // beginIdx=0, endBufIdx=0xFF
}BspLogDesc;


extern BspLogRecord   bspLogBuf[];
extern BspLogDesc     bspLogDesc;

extern UINT32* pTStimer_Last32kValue;/*&(Timers[TS_TIMER_ID].last32kValue);*/

void BspLogPhase2Init(void);
void BspLogPhase3Init(void);


#define BSPLOG_GET_PTR(wrPtr)                                           \
{                                                                       \
    UINT32 cpsr=disableInterrupts();                                    \
           wrPtr = &bspLogBuf[bspLogDesc.wrIdx++];                      \
    restoreInterrupts(cpsr);                                            \
}                                                                    /*endM*/
//         wrPtr = CACHED_TO_NON_CACHED(&bspLogBuf[bspLogDesc.wrIdx++]);

//======================================================================
#if 0
#define PRINT_BSPLOG4(LINENO,TXT14c,tIMEsTAMP,pARAM1,pARAM2,pARAM3)     \
{                                                                       \
  BspLogRecord*  wrPtr;                                                 \
  BSPLOG_GET_PTR(wrPtr);                                                \
  {                                                                     \
      /*Do NOT use "const" but "static" to avoid additional copy*/      \
      __align(8) static BspLogRecordCONST  bspLogRecord = {LINENO,TXT14c};         \
      Ulong64* src = (Ulong64*)&bspLogRecord;                           \
      Ulong64* dst = (Ulong64*)wrPtr;                                   \
      UINT32*  dst32;                                                   \
              *dst++ = *src++;                                          \
              *dst++ = *src;                                            \
            dst32 = (UINT32*)dst;                                       \
    *dst32++ /* wrPtr->timeStamp*/ = (UINT32)tIMEsTAMP;                         \
    *dst32++ /* wrPtr->param1   */ = (UINT32)pARAM1;                            \
    *dst32   /* wrPtr->param2   */ = (UINT32)pARAM2;                            \
  }                                                                     \
}                                                                    /*endM*/
#else
#define PRINT_BSPLOG4(LINENO,TXT14c,tIMEsTAMP,pARAM1,pARAM2,pARAM3) 
#endif
//======================================================================
//            GLOBAL APIs  IMPLEMENTATION
//======================================================================
#define PRINT_BSPLOG_FAST(pARAM1,pARAM2)        PRINT_BSPLOG4(__LINE__,__MODULE__,timerCountRead(TS_TIMER_ID),pARAM1,pARAM2,0)
#define PRINT_BSPLOG(pARAM1,pARAM2)             PRINT_BSPLOG4(__LINE__,__MODULE__, *pTStimer_Last32kValue    ,pARAM1,pARAM2,0)
#define PRINT_BSPLOG_TXT(TXT14c,pARAM1,pARAM2)  PRINT_BSPLOG4(__LINE__,  TXT14c  ,timerCountRead(TS_TIMER_ID),pARAM1,pARAM2,0)


#endif//BSP_LOG_RECORD_ENABLE
#endif//BSP_LOG_H

