#ifndef _WAVREC_H_
#define _WAVREC_H_

#include "basop32.h"




typedef struct IIR_StateT {
    INT16 x0;
    INT16 x1;
    INT16 y1;
    INT16 y2;
} IIR_StateT;

typedef struct IIR_ParamsT {
	 INT16 a[3];
	 INT16 b[3];
 } IIR_ParamsT;

typedef struct
{
	short Enable;
	short DecFreqNum;
	short FreqCoff[10];
	short N;
	short invertN;
} WavRecConfig;
typedef struct
{
	short Status;
	int  Sync_EAvg;					//Used to save Sync Energy Average
	int  Sync_T800;					//Used to save T[800Hz]
	int  SyncHist;					//Used to save Sync History Pattern
	int  ErrorHist;					//Used to save Error History Pattern
	short  DataCnt;					//Used to save and  compare 3 history Symbol
	unsigned char DataHist[3];		//Used to save and  compare 3 history Symbol
	short Symbol_Cnt;				//Used to save Symbol Output
	unsigned char Symbol[40];		//Used to save Symbol Output
	short Head_Index;				//Used to modify FIFO ptr for tone dectection
	short FIFO[768];				//Used to save history data  768 samples
	short Hilbert_Delay[2];			//For Hilbert Filter
	IIR_StateT BandPass_St[2];		//For IIR BandPass 2*2
} WavRecT;
enum{
	R_UnSync,		// 0
	R_SyncDone,		// 1
	R_Success,		// 2		return data
};
INT16 Find_Max_Index(INT32* Array,INT16 Len);
int goertzel_filter_sonic(short Koeff0, short N, short invertN, short *in);
unsigned char CRC_8(unsigned char *ptr, short len);
void Hilbert_Envelope(short* Input,const short* Filter_Coeff,short* DelayLine,short Len,short* Output);
INT16 Fine_Search(INT16* data_in, INT16 Len) ;
void Update_FIFO(INT16* FIFO,INT16 FIFO_Len,INT16* data_in, INT16 Data_Len) ;
void Check_Data_Hist(short tone);
void IIR_Filter(INT16* data, const IIR_ParamsT* params, IIR_StateT* state, INT16 frame_length) ;
void StartStopToneDection_Sonic(WavRecConfig *Cfg_Customer);
short ToneDectionFunc_Sonic(short *in, short size,WavRecT* St);
void ToneDectionShell_Sonic(short *in, short size);
void WavRec_State_Reset(void);
void Trigger_WavRec(void);
void WavRec_Data_Output(unsigned char** Ptr_Out,short* Len);
int  WavRec_Shell(short* in);

#endif 

