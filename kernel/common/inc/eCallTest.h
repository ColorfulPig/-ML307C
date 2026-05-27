/**************************************************************************//**
 *
 * \file        eCallTest.h
 *
 * \brief       Header file of the eCall unit test Module
 *
 *****************************************************************************/
/* Programmer   Avishai & Michael
*****************************************************************************/
#ifdef VOICE_ECALL_BUILD

#ifndef _ECALL_TEST_H_
#define _ECALL_TEST_H_

/******************************************************************************
 * Include Files
 *****************************************************************************/
#ifndef COMPILATION_TARGET
#include "SimulationDefines.h"
#endif

#include "cfw_typedef.h"

void ECallSaveRxTestData(CtrlPort *port);
void ECallNotifyTxTestEnded(void);
void ECallTestDataCmd(INT16 *cmdPtr);
void ECallTestControlCmd(INT16 *cmdPtr);
void EcallTestDataBufferPtrReset(void);
void EcallReadOneRxCtrlFrame(CtrlPort *port);

#endif //of #ifndef _ECALL_TEST_H_
#endif /*VOICE_ECALL_BUILD*/