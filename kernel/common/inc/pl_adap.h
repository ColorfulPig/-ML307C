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

/**********************************************************************
*
* Filename: pl_adap.h
*
* Programmers:  Marcelo Senter (ms)
*
* Description:  Definitions for Adaptation Layer
*
* --------------------------------------------------------------------
* Revision History
*
* Date         Who        Version      Description
* --------------------------------------------------------------------
* 17-Dec-2002  ms
**********************************************************************/
#ifndef PL_ADAP_H
#define PL_ADAP_H

#include "pl_adap_cb.h"
#include <GplcSpy.h>

/* This function should be called from KiOsSendSignal and KiOsSendIntSignal in L1GKI. It returns whether
   KiOsSendSignal and KiOsSendIntSignal should send the signal. If not the signal should be
   destroyed (KiOsDestroySignal or KiOsDestroyIntSignal). */
extern Bool plgAdapNeeded (TaskId destTask, SignalStructure *block, SignalBuffer *signal, Int8 simId);

#if defined (INTEL_UPGRADE_L1_BUILD)
extern void L1TraceSignal(void *signalSructure);
#define TRACE_SIGNAL
#define TRACE_SIGNAL_IMAGE          L1TraceSignal((void *)block);
#define TRACE_TEST_SIGNAL           L1TraceSignal((void *)block);
#define TRACE_TEST_SIGNAL_IMAGE     L1TraceSignal((void *)block);
#else
extern void TraceSignal(void *signalRecord);
#define TRACE_SIGNAL
#define TRACE_SIGNAL_IMAGE
#define TRACE_TEST_SIGNAL             TraceSignal((void *)(&(block->record)));
#define TRACE_TEST_SIGNAL_IMAGE       TraceSignal((void *)(&(block->record)));
#endif /* INTEL_UPGRADE_L1_BUILD */



/* define to call callback function */
#define PLG_CALLBACK(func, data)					\
    GW_DIAG_SEND_DATA_PLG(func, data)				\
	if (func##_ptr){  								\
        TRACE_SIGNAL                				\
		func##_ptr(data);							\
    }                               				\
    else{                            				\
        TRACE_SIGNAL_IMAGE          				\
	}												\
	return (FALSE);

#define PLG_CALLBACK_DS(func, data, simId)					\
    GW_DIAG_SEND_DATA_PLG(func, data)				\
	if (func##_ptr){  								\
        TRACE_SIGNAL                				\
		func##_ptr(data, simId);							\
    }                               				\
    else{                            				\
        TRACE_SIGNAL_IMAGE          				\
	}												\
	return (FALSE);
#define PLG_EMPTY_CALLBACK(func)			\
    GW_DIAG_SEND_EMPTY_PLG(func)			\
	if (func##_ptr){						\
        TRACE_SIGNAL                		\
		func##_ptr(); 						\
    }                               		\
    else{                            		\
        TRACE_SIGNAL_IMAGE          		\
	}										\
	return (FALSE);

#define PLG_EMPTY_CALLBACK_DS(func, simId)			\
    GW_DIAG_SEND_EMPTY_PLG(func)			\
	if (func##_ptr){						\
        TRACE_SIGNAL                		\
		func##_ptr(simId); 						\
    }                               		\
    else{                            		\
        TRACE_SIGNAL_IMAGE          		\
	}										\
	return (FALSE);


#define PLG_TEST_CALLBACK(func, data)				\
    GW_DIAG_SEND_DATA_PLG(func, data)				\
	if (func##_ptr){								\
        TRACE_TEST_SIGNAL                          	\
		func##_ptr((plgTaskId_te)destTask, data);	\
    }                                               \
    else{                                           \
        TRACE_TEST_SIGNAL_IMAGE                     \
	}												\
	return (FALSE);

#define PLG_TEST_EMPTY_CALLBACK(func)				\
    GW_DIAG_SEND_EMPTY_PLG(func)   	   				\
	if (func##_ptr){								\
        TRACE_TEST_SIGNAL                        	\
		func##_ptr((plgTaskId_te)destTask);			\
    }                                       		\
    else{                                    		\
        TRACE_TEST_SIGNAL_IMAGE                  	\
	}												\
	return (FALSE);

#endif /* PL_ADAP_H */
