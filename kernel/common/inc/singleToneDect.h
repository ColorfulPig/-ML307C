
#ifndef _SINGLE_TONE_DECT_H
#define _SINGLE_TONE_DECT_H

#ifdef __cplusplus
extern "C"{
#endif




typedef struct
{
	short Enable;
	short DecFreqNum;
	short FreqCoff[4];
	short N;
} ToneDecStateForCustomerStruct;

void StartStopToneDection(ToneDecStateForCustomerStruct * ToneDecStateForCustomer);
short ToneDectionShell(short *in, short size, short *DectedCnt);

#ifdef __cplusplus
}
#endif

#endif //_SINGLE_TONE_DECT_H

