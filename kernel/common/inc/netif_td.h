#ifndef NETIF_TD_HDR_H
#define NETIF_TD_HDR_H

#include "err.h"
#include "sys.h"
#include "sys_arch.h"
#include "netif.h"

#if LWIP_NETIF_LTE

/**************************************************************************
 *                          Define local macro                            *
 **************************************************************************/

#define PDP_FLAG_DEFAULT_BIT_POS    (0x1)   /*bit1: 0 denote default pdp; 1 denote dedicated pdp*/
#define PDP_FLAG_PPP_BIT_POS        (0x2)   /*bit2: if set to 1, denote ppp pdp config mode*/
#define PDP_FLAG_MBIM_BIT_POS       (0x4)   /*bit3: if set to 1, denote mbin pdp config mode*/
#define PDP_FLAG_APP_BIT_POS        (0x8)   /*bit4: if set to 1, denote app pdp config mode*/
#define PDP_FLAG_IMS_BIT_POS        (0x10)  /*bit5: if set to 1, denote ims pdp config mode*/
#define PDP_FLAG_PASS_BIT_POS       (0x20)  /*bit6: if set to 1, denote pass-through pdp config mode*/
#define PDP_FLAG_REMAP_BIT_POS      (0x40)  /*bit7: if set to 1, denote the config is remap pdn*/

extern unsigned char IS_LTE_W_PS;

/**************************************************************************
 *                          Define local struct                           *
 **************************************************************************/
struct td_info_t {
    u32_t ip_addr;
    u32_t dns_ip_1;
    u32_t dns_ip_2;
    u32_t gw_ip;
    u32_t mask;
};

struct td_speed_t {
    u32_t ticks;
    u32_t t1;
    u32_t t2;
    u32_t speed;
};

/*match to dialer_task.h
struct  Ipv4Info definition match lwip_ipv4_info;
struct  Ipv6Info definition match lwip_ipv6_info;
*/
typedef struct {
        int  IPAddr;
        int  PrimaryDNS;
        int  SecondaryDNS;
        int  GateWay;
        int  Mask;
} lwip_ipv4_info;

typedef struct {
        int  IPV6Addr[4];
        int  PrimaryDNS[4];
        int  SecondaryDNS[4];
        int  GateWay[4];
        int  Mask[4];
} lwip_ipv6_info;

typedef struct{
	char cid;          /* for PDP context: 
	                    (cid >> 7), sim id, 0 denote sim card 0, 1 denote sim card 1; 
	                    (cid & 0x3F), cid index config. 
	                      for tune netif context :
	                     cid, denote tune index config.
	                      */
	char status;       /* 0 for disconnect, 1 fot connect, 0xFF, denote delete directly */
	char pdp_flag;
	char ip_type;      /* 0 bit, set for IPv4; 1 bit, set for IPv6; value can:1\2\3 */
	lwip_ipv4_info *ip4info; /*if only v6, set to NULL */
	lwip_ipv6_info *ip6info; /*if only v4, set to NULL */
    sys_sem_t *sem;   /* This semaphore is posted when tcpip_thread process finish*/
    char remap_cid;    /*remap cid from cm, if PDP_FLAG_REMAP_BIT_POS set, need care about it; format as the same as 'char cid'*/
    u8_t u8_resvd;
    u16_t u16_resvd;
} lwip_ip_configinfo;

typedef struct {
    u16_t status;
    u8_t simid;
    u8_t cid;
    u32_t IPAddr;
    u32_t PrimaryDNS;
    u32_t SecondaryDNS;
    u32_t GateWay;
    u32_t NetMask;
} netif_ip4_info;

typedef struct {
    u16_t status;
    u8_t simid;
    u8_t cid;
    u32_t IPV6Addr[4];
    u32_t PrimaryDNS[4];
    u32_t SecondaryDNS[4];
} netif_ip6_info;

typedef struct {
    u32_t  local_ip6[4];
    u32_t  global_ip6[4];
    u32_t  gateway_ip6[4];
} lte_ip6_addr;

#if IS_LWIP_PLATFORM_1802_1802S
typedef struct _S_IPNetPktHdr_
{
    u8_t HighLength;
    u8_t LowLength;
    u8_t Offset;
    u8_t Reserv1;
}lwip_IPNetPktHdr;
#endif

#if IS_LWIP_PLATFORM_CRANE
#if LWIP_IPNET_TCPHEADER
typedef struct _tagCpHeader_
{
    u32_t dataLen:24;
    u32_t offset:8;
    u32_t m_cid:8;
    u32_t rbid:8;
    u32_t qfi:8;
    u32_t pdu:8;
}lwip_IPNetPktHdr;
#else
typedef struct _S_IPNetPktHdr_
{
    u8_t HighLength;
    u8_t LowLength;
    u8_t Offset;
    u8_t Reserv1;
    u32_t CId;
}lwip_IPNetPktHdr;
#endif
#endif

#endif /* LWIP_NETIF_TD */

#endif
