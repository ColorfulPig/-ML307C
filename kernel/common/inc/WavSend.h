#ifndef _WAVSEND_H_
#define _WAVSEND_H_

#include "basop32.h"

#define Q14 16384

#define VOICE_RAMP_SIZE 32
#define LOG2_VOICE_RAMP_SIZE 5

#define Flag int

enum{
    DTMF_GEN_HIGH,
    DTMF_GEN_LOW
};

typedef struct{
    INT16   YMinus1;
    INT16   YMinus2;
} DtmfGenDelayType;

typedef struct{
    INT16   High;
    INT16   Low;
} DtmfGenCoef;

typedef struct{
	Flag	Enable;
	Flag	EnableLast;
	DtmfGenDelayType	Delay[2];		//{High(-1),High(-2),Low(-1),Low(-2)} 
} DtmfGenStateT;			//Data for RAM Code

typedef struct{
	DtmfGenCoef		Freq[2];			// {High, Low}
	DtmfGenCoef		Volume[2];
} DtmfGenConfigT;			//Data for ROM Code



enum{
	Control_Reset,			// 0 
	Control_Send,			// 1
};
enum{
	Status_Idle,			// 0 
	Status_Sending,			// 1
};



#define MAX_DATA_CNT		(20)	// Max 20 Bytes
#define CRC_BYTE			(1)		// 1Byte  8bits
#define SUB_REPEAT_CNT		(3)

typedef struct {
	INT16 On;
	INT16 Status;
	unsigned char  Symbol[1+(MAX_DATA_CNT+CRC_BYTE)*2];		// byte to be sent
 	INT16 SymbolNum;  
  	INT16 tick;
  	INT16 subtick;
} WavSendT;


void DtmfGen(INT16 *InPtr, INT16 *FreqCoeff, DtmfGenDelayType *DtmfDelay, int frame_size);
void DtmfGenRxUpdateParam(WavSendT *DTMFCmd);
void RampUp(INT16 *InPtr, INT16 length, INT16 log2length);
void RampDown(INT16 *InPtr, INT16 length, INT16 log2length);

void WavSendInit(void);
void AppSend(unsigned char *msg, int len);
void Trigger_WavSend(unsigned char* Msg_Buf, short SizeOfBytes,short Control);
int  WavSend_Shell(short* Pcm_Buffer);
unsigned char CRC_8(unsigned char *ptr, short len);


#endif //_CFW_DTMF_H_

