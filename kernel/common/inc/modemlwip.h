#ifndef __MODEM_LWIP_H__
#define __MODEM_LWIP_H__

#include <inttypes.h>
#include "global_types.h"
#include "ipnetbuf.h"


#define LWIP_IPNET_MEM_RECORD_DEBUG


typedef struct _LwipPacketNode{

	struct _LwipPacketNode *next;
	#ifdef USBNET_HEAPBUF_SUPPORT
	UINT16 ulPacketRef;
	UINT16 ulFreeRef;
	UINT32 offset;
	#endif
	UINT8 *dataPtr;
}LwipPacketNode;

#define LWIP_PS_UL_BLOCK_SIZE	(IPNET_UP_BLOCK_SIZE)//(2048)
#define LWIP_PS_UL_BLOCK_COUNT 	(IPNET_UP_BLOCK_COUNT)//(160)

#define LWIP_PS_DL_BLOCK_SIZE        (2048)
#ifdef CRANE_MCU_DONGLE
#if (defined(LTEONLY_THIN) || defined(LTEGSM_THIN))
#define LWIP_PS_DL_BLOCK_NUM         (40) //(160)//(20)
#else
#define LWIP_PS_DL_BLOCK_NUM         (50) //(160)//(20)
#endif
#else
#define LWIP_PS_DL_BLOCK_NUM         (160)//(20)
#endif

#define LWIP_PS_UP_MEM_SIZE (LWIP_PS_UL_BLOCK_SIZE*LWIP_PS_UL_BLOCK_COUNT)
#define LWIP_PS_DL_MEM_SIZE (LWIP_PS_DL_BLOCK_SIZE*LWIP_PS_DL_BLOCK_NUM)
#define LWIP_PS_BUF_ADDR_ALIGN_LEN         8

#define PACKET_ALIGN(size, align) (((size) + (align - 1)) & ~(align - 1))


#define LWIP_PS_PACKET_MAX_LEN	2048
#define LWIP_DL_HASH_TABLE_LEN	(LWIP_PS_DL_MEM_SIZE/LWIP_PS_PACKET_MAX_LEN)
#define LWIP_UL_HASH_TABLE_LEN	(LWIP_PS_UP_MEM_SIZE/LWIP_PS_PACKET_MAX_LEN)


typedef struct _LwipMemNode{
	struct _LwipMemNode *next;
	UINT32 startAddr;
	UINT32 endAddr;
}LwipMemNode;


typedef struct _LwipHashNode{
	LwipMemNode *head;
	LwipMemNode *tail;
}LwipHashNode;

typedef struct _LwipDLMemManager
{
	UINT32 memBase;
    UINT32 memSize;
	UINT32 headerLen;
	UINT32 totalAllocCount;
	UINT32 totalSentCount;
	UINT32 totalAllocBytesSize;
	UINT32 totalSendBytesSize;

	BOOL  hwmReached;
	OSPoolRef lwipMemPoolRef;

	LwipHashNode lwipBufLinkArr[LWIP_DL_HASH_TABLE_LEN];
} LwipDLMemManager;

typedef struct _LwipULMemManager
{
	UINT32 memBase;
    UINT32 memSize;
	UINT32 headerLen;
	UINT32 totalAllocCount;
	UINT32 totalSentCount;
	UINT32 totalAllocBytesSize;
	UINT32 totalSendBytesSize;

	BOOL  hwmReached;
	OSPoolRef lwipMemPoolRef;

	LwipHashNode lwipBufLinkArr[LWIP_UL_HASH_TABLE_LEN];
} LwipULMemManager;


void* lwip_downlink_allocmem(UINT32 size,UINT32 cid);
void lwip_downlink_putmem(void *ptr);
void lwip_downlink_freemem(void *ptr);
UINT8* ipnet_uplink_allocmem(UINT32 siz, UINT8 isQosPacket);
void ipnet_uplink_freemem(void *ptr);
void ipnet_uplink_transfer_ind(UINT8 *data_ptr,UINT32 length);
void lwip_uplink_ps_packet_send(UINT8 *data_ptr, UINT32 len);
#endif
