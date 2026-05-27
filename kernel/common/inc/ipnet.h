#ifndef __IPNET_H__
#define __IPNET_H__

#include "osa.h"

struct IPNetIndexSection
{
	int (*GetIndex)(void);
	void (*GetIndexInc)(void);

	int (*PutIndex)(void);
	void (*PutIndexInc)(void);
};

typedef struct IPNetPktHdr
{
    UINT8 HighLength;
    UINT8 LowLength;

    UINT8 Offset;
    UINT8 Reserv1;
    
    UINT32 CId;

} __attribute__((packed)) S_IPNetPktHdr;

struct IPNetUSBNetHdr
{
    UINT8 HighLength;
    UINT8 LowLength;

    UINT8 Offset;
    UINT8 Reserv1;
};

typedef void (*tx_notify_t)(struct UM_PCB_STRUCT* um_buffer);
typedef void (*rx_notify_t)(struct UM_PCB_STRUCT* um_buffer);
typedef void (*resume_notify_t)(struct UM_PCB_STRUCT* um_buffer);

/* downlink API for IPNet */
void IPNetDlInit(void* hdr, void* data_pool, 
	UINT16 block_size, UINT16 block_num, 
	tx_notify_t tx_notify, resume_notify_t resume_notify);

void* IPNetGetDlBufferExtend(unsigned long size, UINT32 cid,UINT8* ExtHdPtr, UINT32 ExtHdLen);
void* IPNetGetDlBuffer(unsigned long size, unsigned int cid);
void  IPNetPutDlBuffer(void* ptr);
struct IPNetPktHdr* IPNetGetPktHdr(void* ptr);

UM_BLOCK* IPNetGetDlBlock(void);
void IPNetPutDlBlock(UM_BLOCK* block);

int IPNetGetDlBlockIndex(UM_BLOCK* block);
UM_BLOCK* IPNetGetDlBlockByIndex(int index);

struct IPNetPktHdr* IPNetGetPktHdr(void* ptr);

/* uplink API for IPNet */
void IPNetUlInit(void* hdr, void* data_pool, 
	UINT16 block_size, UINT16 block_num, 
	rx_notify_t rx_notify, resume_notify_t resume_notify);

#endif

