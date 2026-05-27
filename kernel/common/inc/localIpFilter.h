#ifndef __LOCAL_IPFILTER_H__
#define __LOCAL_IPFILTER_H__

#define uint32_t    unsigned long
#define uint16_t    unsigned short
#define uint8_t     unsigned char
#define int32_t     long
#define int16_t     short
#define int8_t      char
typedef unsigned char Int8;
typedef unsigned short Int16;
typedef unsigned long Int32;
typedef Int8 Boolean;

struct IPNetPktHdr
{
    UINT8 HighLength;
    UINT8 LowLength;

    UINT8 Offset;
    UINT8 Reserv1;
    
    UINT32 CId;
};
typedef void (*volte_tx_t)(struct IPNetPktHdr* ptr,unsigned int length);

int32_t add_ipv4_filter_rule(uint8_t pCid,uint32_t srcIp,
		uint16_t srcPort,uint32_t dstIp,uint16_t dstport);
int32_t add_ipv6_filter_rule(uint8_t pCid,uint8_t *srcIp,uint16_t srcPort,
		uint8_t *dstIp,uint16_t dstPort);
int32_t remove_ip_filter_rule(int32_t id);
extern int VoLteTxProcessCallBackRegister(volte_tx_t txFunEentry);
extern int VoLteTxProcessDone(void* ptr);
//extern int VoLteUpLinkPacketProcess(unsigned char P_Cid,unsigned char * ptr,unsigned int length);
void* VoLteUpLinkPacketAlloc(unsigned char P_Cid,unsigned int length);
int VoLteUpLinkPacketFree(void* ptr,unsigned int length);

#endif
