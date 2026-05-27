#ifndef __MODEM_TFT_H__
#define __MODEM_TFT_H__

extern UINT32  Image$$DDR_HEAP_ENDMARK$$Base;
extern UINT32 Image$$DDR_HEAP_GUARD$$Base;

#define DDR_HEAP_END_MARK     ((UINT32)(&Image$$DDR_HEAP_ENDMARK$$Base))
#define DDR_HEAP_START_MARK     ((UINT32)(&Image$$DDR_HEAP_GUARD$$Base))
#define IsInVolteBuffer(address) (((UINT32)address<=DDR_HEAP_END_MARK)&&((UINT32)address>=DDR_HEAP_START_MARK))

int ModemVolteTxPacketPut(void *ptr);
void *ModemVolteTxPacketGet(UINT32 size, UINT32 cid);

int ModemVolteRxDataRelease(unsigned char *ptr);
void * ModemVolteRxDataPoll(void);
int ModemParserCidNeedToVolte(UINT8 cid);

#endif
