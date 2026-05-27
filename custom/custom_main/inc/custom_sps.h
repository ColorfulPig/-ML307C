
#ifndef __CUSTOM_SPS_H__
#define __CUSTOM_SPS_H__ 

#include <stdio.h>

enum
{
	TPTC_R_FALSE = 0,		// 数据不被该协议支持
	TPTC_R_FRAME,			// 数据被该协议支持,已经识别一帧
	TPTC_R_CONTINUE,		// 数据暂时被认识
};

typedef int (* TPTC_PROC_CHARSCAN) (uint8_t ch);	
typedef int (* TPTC_PROC_FRAMESCAN)(void);	
typedef int (* TPTC_PROC_FINISHSCAN)(void);	
typedef int (* TPTC_PROC_BLOCKSCAN)(uint8_t *pBuf,uint32_t uLen);	

// 通信协议类
typedef struct 
{
	TPTC_PROC_CHARSCAN		inCharScan;		
	TPTC_PROC_FRAMESCAN		inFrameScan;	
	TPTC_PROC_FINISHSCAN	inFinishScan;	
	TPTC_PROC_BLOCKSCAN		inBlockScan;	
}TPTC_PROC;

void Sps_InUart0Scan(uint8_t *pv_Buffer,uint16_t v_Length);
void Sps_InUart1Scan(uint8_t *pv_Buffer,uint16_t v_Length);
void Sps_InUart2Scan(uint8_t *pv_Buffer,uint16_t v_Length);
void Sps_InUsbScan(uint8_t *pv_Buffer,uint16_t v_Length);
void Sps_InCloudScan(uint8_t *pv_Buffer,uint16_t v_Length);


#endif

