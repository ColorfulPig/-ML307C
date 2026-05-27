#ifndef __DATA_BUF_H__
#define __DATA_BUF_H__

#include <inttypes.h>
typedef unsigned char Boolean;


#ifdef LWIP_IPNETBUF_SUPPORT
#define IPNET_UPLINK_HEADER_LENGTH_CPONLY	8
#else
#define IPNET_UPLINK_HEADER_LENGTH_CPONLY	4
#endif

#define IPNET_UPLINK_HEADER_LENGTH_SHM	8

#define MAX_CID_COUNT (8)
#define IPNET_EVENT_RX     	(0x08)

#define IPNET_UP_BLOCK_SIZE	(2048)

#if (defined(LTEONLY_THIN) || defined(LTEGSM_THIN))
#ifndef SINGLE_SIM
	#define IPNET_UP_BLOCK_COUNT (65) //(160 + 64 + 64)
#else
    #if defined(TPT_UL)
    #define IPNET_UP_BLOCK_COUNT (40 + 32) //(160 + 64 + 64)
    #else
    #define IPNET_UP_BLOCK_COUNT (40) //(160 + 64 + 64)
    #endif
#endif	
#else
#define IPNET_UP_BLOCK_COUNT (125) //(160 + 64 + 64)
#endif


typedef struct EVB_IP_HEADER_TAG{
  uint8_t lenhigh;
  uint8_t lenlow;
  uint16_t offset;
}EVB_IP_HEADER;

typedef struct TWOCHIP_IP_HEADER_TAG{
	uint16_t length;
	uint16_t reserved;
	uint8_t CID;
	uint8_t reserved_0;
	uint8_t reserved_1;
	uint8_t reserved_2;

}TWOCHIP_IP_HEADER;

typedef struct ip_channel_hdr
{
	uint16_t length;
	uint16_t offset;
}CPONLY_IP_HEADER;

typedef struct DnLinkPsPacketList_Tag{
	uint8_t *pdu;
	uint16_t length;
	uint16_t CID;
	struct DnLinkPsPacketList_Tag *next;
	uint32_t reserved;
}DnLinkPsPacketList;

typedef enum{
	PS_PACKET_UNKNOWN,
	PS_PACKET_DEFAULT,
    PS_PACKET_IMS_DEFAULT,		
    PS_PACKET_IMS_AUDIO,
    PS_PACKET_IMS_VIDEO, 		
    PS_PACKET_NUM
} PS_PACKET_TYPE;

typedef struct pduTag
{
    UINT16   length;       /*Number of user data bytes in NPDU*/
    UINT16   dataOffset;   /*Offset from npduData that user data starts*/
    UINT16   totalLength;  /*Total size of buffer, set only when allocated*/
    UINT8   *npduData;     /*The start of the buffer */
	Boolean  isMMS;
	UINT32   timeStamp;
	PS_PACKET_TYPE packetType;
}pdu;

typedef struct SnUlSduListNodeTag
{
    INT8                     epsBearerId;
    pdu                     npdu;
    struct SnUlSduListNodeTag *next;
}SnUlSduListNode;

#if 0
typedef struct SnUlSduListTag{
	UINT32 length;
	UINT8* Pdu;
	struct SnUlSduListTag *next;
	UINT32 reserved[3];
}SnUlSduListNode;
#endif
typedef struct UpLinkPsPacketListTag{
	UINT16 AckPkgNum;
	UINT16 AckPkgTotalLen;
	UINT16 DataPkgNum;
	UINT16 DataPkgTotalLen;
	UINT32 CID;
	SnUlSduListNode *AckIpListHead;
	SnUlSduListNode *AckIpListTail;
	SnUlSduListNode *DataIpListHead;
	SnUlSduListNode *DataIpListTail;
}UpLinkPsPacketList;

typedef struct UpLinkPsPacketReqTag{
	UINT16 ElementCnt;
	UpLinkPsPacketList SubReqlist[MAX_CID_COUNT];

}UpLinkPsPacketReq;

//ICAT EXPORTED STRUCT
typedef struct {
	UINT32 using_dhcp;
	char apn_name[32];
} ipnet_setting;

//DownStream API between PS and platform ,invoked by PS 
uint8_t *DnLinkAllocateBuffer(uint16_t length);
void DnLinkPsPacketInd(DnLinkPsPacketList * list);

//Downstream flowcontrol API based on DL pool wartermask, invoked by PS 
Bool DnLinkPsTransferIsBlock(void);

//Upstream API for release node and pdu buffer, invoked by PS 
void UpLinkReleasePsPacket(SnUlSduListNode * node);

//UpStream API to transfer IP packet list to PS, invoked by Platform and implemented by PS
extern void UpLinkPsPacketListReq(UpLinkPsPacketReq* UpPsTransferReq);






#endif
