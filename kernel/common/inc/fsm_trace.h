/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/* ===========================================================================
File        : fsm_trace.h
Description : Trace interface mapping for the 
              genlib/fsm package.

Notes       : Adapt macro definitions to desried trace implementation.

Copyright (c) 2001 Intel CCD. All Rights Reserved
=========================================================================== */

#if !defined(_FSM_TRACE_H_)
#define _FSM_TRACE_H_

/************************
Trace Macros 
*************************/

#ifndef FSM_TRACING

/* No tracing... macros undefined */

#define FSM_TRACE(CAT1, CAT2, CAT3, rl, fmt)
#define FSM_TRACE1(CAT1, CAT2, CAT3, rl, fmt, val1)
#define FSM_TRACE2(CAT1, CAT2, CAT3, rl, fmt, val1, val2)
#define FSM_TRACE3(CAT1, CAT2, CAT3, rl, fmt, val1, val2, val3)

#define FSM_ASSERT(cond)

#else

/* Implement trace macros as desired... */


#ifdef ICAT_PRESENT
/* Intel ICAT: Bring in diag.h, map the macros! */

#include "diag.h"

#define FSM_TRACE(CAT1, CAT2, CAT3, rl, fmt) \
          DIAG_FILTER(CAT1, CAT2, CAT3, rl) \
          diagPrintf(fmt)

#define FSM_TRACE1(CAT1, CAT2, CAT3, rl, fmt, val1) \
          DIAG_FILTER(CAT1, CAT2, CAT3, rl) \
          diagPrintf(fmt, val1)

#define FSM_TRACE2(CAT1, CAT2, CAT3, rl, fmt, val1, val2) \
          DIAG_FILTER(CAT1, CAT2, CAT3, rl) \
          diagPrintf(fmt, val1, val2)

#define FSM_TRACE3(CAT1, CAT2, CAT3, rl, fmt, val1, val2, val3) \
          DIAG_FILTER(CAT1, CAT2, CAT3, rl) \
          diagPrintf(fmt, val1, val2, val3)

#define FSM_ASSERT(cond) \
          ASSERT(cond)


#else  /* Assume printf and assert available! */

#include <stdio.h>
#include <assert.h>

#define FSM_TRACE(CAT1, CAT2, CAT3, rl, fmt)                     \
          printf("%s %s %s: RL = %s:", #CAT1, #CAT2, #CAT3, #rl); \
          printf(fmt);                                           \
          printf("\n")

#define FSM_TRACE1(CAT1, CAT2, CAT3, rl, fmt, val1)              \
          printf("%s %s %s: RL = %s:", #CAT1, #CAT2, #CAT3, #rl); \
          printf(fmt, val1);                                     \
          printf("\n")

#define FSM_TRACE2(CAT1, CAT2, CAT3, rl, fmt, val1, val2)        \
          printf("%s %s %s: RL = %s:", #CAT1, #CAT2, #CAT3, #rl); \
          printf(fmt, val1, val2);                               \
          printf("\n")

#define FSM_TRACE3(CAT1, CAT2, CAT3, rl, fmt, val1, val2, val3)  \
          printf("%s %s %s: RL = %s:", #CAT1, #CAT2, #CAT3, #rl); \
          printf(fmt, val1, val2, val3);                         \
          printf("\n")
                                                               
#define FSM_ASSERT(cond)                                         \
          assert(cond)


#endif /* ICAT_PRESENT */


#endif /* FSM_TRACING */

#endif /* _FSM_TRACE_H_ */


/*                      end of fsm_trace.h
--------------------------------------------------------------------------- */
