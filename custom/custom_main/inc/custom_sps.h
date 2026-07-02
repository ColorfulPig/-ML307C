
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

/* 扫描串口 0 输入数据并交给 BMS 协议解析器。 */
void Sps_InUart0Scan(uint8_t *pv_Buffer,uint16_t v_Length);
/* 扫描串口 1 输入数据并交给 GNSS 处理逻辑。 */
void Sps_InUart1Scan(uint8_t *pv_Buffer,uint16_t v_Length);
/* 扫描串口 2 输入数据并交给蓝牙处理逻辑。 */
void Sps_InUart2Scan(uint8_t *pv_Buffer,uint16_t v_Length);
/* 扫描 USB 输入数据并交给测试命令处理逻辑。 */
void Sps_InUsbScan(uint8_t *pv_Buffer,uint16_t v_Length);
/* 扫描云端下发数据并交给 Cloud 协议解析器。 */
void Sps_InCloudScan(uint8_t *pv_Buffer,uint16_t v_Length);


#endif

