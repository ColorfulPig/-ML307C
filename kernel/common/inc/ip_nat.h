/***************************************************************************
 * Copyright (c) 2001-2004 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * $Id: ip_nat.h,v 1.1 2010/05/05 19:34:23
 ***************************************************************************/

#ifndef __LWIP_NAT_H__
#define __LWIP_NAT_H__

#include "err.h"
#include "opt.h"
#include "sys_arch.h"
#include "ip_addr.h"
#include "tcp_impl.h"
#include "udp.h"
#include "inet_chksum.h"
#include "net_bridge.h"
#include "lwip_customer.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define LWIP_NAT_TTL_INFINITE            (-1)
#define LWIP_NAT_TTL_SECONDS             (60)
#define LWIP_NAT_TTL_SECONDS_APP         (60)
#define LWIP_NAT_TTL_SECONDS_ICMP        (20)
#define LWIP_NAT_TTL_SECONDS_TCP         (600)
#define LWIP_NAT_TTL_SECONDS_TRIGGER     (20)

/** Timer interval at which to call ip_nat_tmr() */
#define LWIP_NAT_TMR_INTERVAL_SEC        (10)

/* network order, port 21 is for ftp */
#define NAT_TCP_PORT_20                  (0x1400) /*21*/
#define NAT_TCP_PORT_21                  (0x1500) /*21*/
#define NAT_TCP_PORT_PPTP                (0xBB06) /*1723*/
#define NAT_TCP_PORT_DNS                 (0x3500) /*53*/
#define NAT_UDP_PORT_SIP                 (0xC413) /*5060*/
#define NAT_UDP_PORT_DNS                 (0x3500) /*53*/
#define NAT_UDP_PORT_DHCP_SERVER         (0x4300) /*67*/
#define NAT_UDP_PORT_DHCP_CLIENT         (0x4400) /*68*/

/* Return 1 if tcp port is a persist port */
#define _nat_tcp_port(port)              ((port) == NAT_TCP_PORT_21 \
                                          || (port) == NAT_TCP_PORT_PPTP)
#define nat_tcp_persist_port(port) (_nat_tcp_port(port) || lwip_hook_nat_persistant_port(port))

/*****define for nat_entry->flags settings ******/
/** indicates this ipnat table if do tcp dns transfor */
#define NAT_FLAG_DNS_TRANSFER  				0x0001
/** indicates this ipnat table if do ftp alg nat transfor */
#define NAT_FLAG_FTP_ALG_TRANSFER  		    0x0002
/** indicates this ipnat table need do tcp seq amend, for ftp alg */
#define NAT_FLAG_TCP_SEQ_AMEND  		    0x0004
/** indicates this ipnat table is ip nat entry for mifi */
#define NAT_FLAG_IP_NAT_ENTRY  		        0x0008
/** indicates this ipnat table is app nat entry */
#define NAT_FLAG_APP_NAT_ENTRY  		    0x0010
/** indicates this ipnat table created by dongle mode, for dongle fastpath */
#define NAT_FLAG_DONGLE_NAT_ENTRY  		    0x0020
/** indicates this ipnat table created by input trigger from nw for donglenat at */
#define NAT_FLAG_TRIGGER_NAT_ENTRY  		0x0040
/** indicates this ipnat table created by fwd rule */
#define NAT_FLAG_FWD_NAT_ENTRY  		    0x0080
/** indicates this ipnat table created by fragment nat */
#define NAT_FLAG_FRAG_NAT_ENTRY  		    0x0100

#define nat_is_dns_transfer(entry)                  (((entry)->flags & NAT_FLAG_DNS_TRANSFER) ? 1 : 0)
#define nat_set_dns_transfer(entry)                 ((entry)->flags |= NAT_FLAG_DNS_TRANSFER)
#define nat_is_ftpalg_transfer(entry)               (((entry)->flags & NAT_FLAG_FTP_ALG_TRANSFER) ? 1 : 0)
#define nat_set_ftpalg_transfer(entry)              ((entry)->flags |= NAT_FLAG_FTP_ALG_TRANSFER)
#define nat_is_tcpseq_amend(entry)                  (((entry)->flags & NAT_FLAG_TCP_SEQ_AMEND) ? 1 : 0)
#define nat_set_tcpseq_amend(entry)                 ((entry)->flags |= NAT_FLAG_TCP_SEQ_AMEND)
#define nat_is_ipnat_entry(entry)                   (((entry)->flags & NAT_FLAG_IP_NAT_ENTRY) ? 1 : 0)
#define nat_set_ipnat_entry(entry)                  ((entry)->flags |= NAT_FLAG_IP_NAT_ENTRY)
#define nat_is_appnat_entry(entry)                  (((entry)->flags & NAT_FLAG_APP_NAT_ENTRY) ? 1 : 0)
#define nat_set_appnat_entry(entry)                 ((entry)->flags |= NAT_FLAG_APP_NAT_ENTRY)
#define nat_is_donglenat_entry(entry)               (((entry)->flags & NAT_FLAG_DONGLE_NAT_ENTRY) ? 1 : 0)
#define nat_set_donglenat_entry(entry)              ((entry)->flags |= NAT_FLAG_DONGLE_NAT_ENTRY)
#define nat_is_trigger_entry(entry)                 (((entry)->flags & NAT_FLAG_TRIGGER_NAT_ENTRY) ? 1 : 0)
#define nat_set_trigger_entry(entry)                ((entry)->flags |= NAT_FLAG_TRIGGER_NAT_ENTRY)
#define nat_is_fwd_entry(entry)                     (((entry)->flags & NAT_FLAG_FWD_NAT_ENTRY) ? 1 : 0)
#define nat_set_fwd_entry(entry)                    ((entry)->flags |= NAT_FLAG_FWD_NAT_ENTRY)
#define nat_is_frag_entry(entry)                    (((entry)->flags & NAT_FLAG_FRAG_NAT_ENTRY) ? 1 : 0)
#define nat_set_frag_entry(entry)                   ((entry)->flags |= NAT_FLAG_FRAG_NAT_ENTRY)

#define nat_is_infinite_entry(entry)                (((entry)->ttl == LWIP_NAT_TTL_INFINITE) ? 1 : 0)
#define nat_is_stable_flow_entry(entry)             (((entry)->status == IP_NAT_STATUS_TCP_ESTABLISH) ? 1 : 0)
#define nat_is_close_flow_entry(entry)              (((entry)->status >= IP_NAT_STATUS_TCP_CLOSING) ? 1 : 0)
#define nat_is_tcp_stable_entry(entry)              (((entry)->proto == IP_PROTO_TCP) \
                                                     && (((entry)->status >= IP_NAT_STATUS_TCP_ESTABLISH) \
                                                     && ((entry)->status < IP_NAT_STATUS_TCP_CLOSING)) ? 1 : 0)


#define ip_nat_chksum_adjust_len2(chksum, optr, nptr)   inet_chksum_adjust_2n((chksum), (optr), (nptr), 1)
#define ip_nat_chksum_adjust_len4(chksum, optr, nptr)   inet_chksum_adjust_2n((chksum), (optr), (nptr), 2)

typedef enum {
    IP_NAT_STATUS_INIT = 0,
    IP_NAT_STATUS_TCP_SYN,
    IP_NAT_STATUS_TCP_ESTABLISH,
    IP_NAT_STATUS_TCPSEQ_AMEND_START,
    IP_NAT_STATUS_TCPSEQ_AMEND_IN_STABLE,
    IP_NAT_STATUS_TCPSEQ_AMEND_OUT_STABLE,
    IP_NAT_STATUS_TCP_CLOSING,
    IP_NAT_STATUS_END
} ip_nat_status;

typedef enum {
    IP_NAT_UDP  = 0x1UL,
    IP_NAT_TCP  = 0x2UL,
    IP_NAT_ICMP = 0x3UL,    
    IP_NAT_GRE  = 0x4UL
} ip_nat_type;

typedef struct _ip_nat_icmp_t {
    u16_t                 id_new;
    u16_t                 id;
    u16_t                 seqno;
    u16_t                 seqno_new;
} ip_nat_icmp_t;

typedef struct _ip_nat_tcp_t {
    u16_t                 nport;
    u16_t                 sport;
    u16_t                 dport;
    s16_t                 seq_diff; /*tcp seqno diff value for one ftp alg process now*/
    s32_t                 seq_amend; /*tcp seqno amend value for multi ftp alg one ftp connection*/
    u32_t                 seq_next; /*tcp origin seq_next marker value for one ftp alg process*/
} ip_nat_tcp_t;

typedef struct _ip_nat_udp_t {
    u16_t                 nport;
    u16_t                 sport;
    u16_t                 dport;
} ip_nat_udp_t;

typedef struct _ip_nat_table_t {
    s16_t           ttl;    // must be s16_t, -1 for not timer
    u16_t           index;  // record table index
    u32_t           key;    // hash table key, also keep same with UL flowid for mifi, and flowid for dongle
    ip_addr_t       source;
    ip_addr_t       dest;
    struct netif*   netif_nw;  //record internet netif, netif_cid**
    struct netif*   netif_lc;  //record local netif, netif_pc, netif_wifi
#if LWIP_ARP
    net_mac_rec_t*  netmac_rec; //record net_mac_rec for coming packet which is from
#endif
    void *          pf_info;  //record packetfilter_info to nat table, using for nat cache using
    u8_t            epsid;  //record epsid to nat table, using for nat cache using
    u8_t            qfi;
    u8_t            psi;
    u8_t            proto;

	u8_t 		    wndsc_dl; //record remote window scale from dl packet
	u8_t 		    wndsc_ul; //record local window scale from ul packet
    u16_t           u16resvd;

    u16_t           flags; //using flags to extend ip_nat_table
    u16_t           status; //using to recorder tcp status
    u32_t           ctick; //record create or update tick, for delete oldest check
    u32_t           packets; //record how many packets match this nat_entry for stable flow    
    u32_t           flowid;  //record flow id for DL direction, for nat_entry->key record UL direction for mifi

    union {
        ip_nat_icmp_t icmp;
        ip_nat_tcp_t  tcp;
        ip_nat_udp_t  udp;
    } u;

} ip_nat_table_t;

/* port fwd flag, bitmap settings */
#define PORT_FWD_INVALID     (0x00)
#define PORT_FWD_TCP         (0x01)
#define PORT_FWD_UDP         (0x02)

#define PORT_FWD_CONFIG      (0x10) /*denote this table is used to do port forward*/
#define PORT_FWD_MAPPING     (0x20) /*denote this table is used to do port mapping*/
#define PORT_FWD_FTP_ALG     (0x40)  /*denote this table is used to do ftp alg*/

/*port_fwd_item used to for port fwd function,*/
/*this struct used in functio process.*/
typedef struct _port_fwd_item // 12 bytes
{
    /*flag of hotbit map, refer to port fwd flag, bitmap settings*/
    u16_t flag;         // 1 tcp, 2 udp, 3 both, 0 invalid
    u16_t index;

    /*local machine ipaddr*/
    u32_t ipaddr;       // network order

    /*when table used for port forward, port_start and port_end denote port range*/
    /*when table used for port mapping, */
    /* --port_start denote external port,*/
    /* --port_end denote internal port,*/
    u16_t port_start;   // host order
    u16_t port_end;     // host order

    ip_nat_table_t *nat_entry; /*record malloc nat_entry for fastpath*/
}port_fwd_item;

/*port_fwd_list used to config port fwd function,*/
/*this struct opened for api call.*/
/*the meaning of data come from port_fwd_item*/
typedef struct _port_fwd_list
{
    u32_t flag;         // 1 tcp, 2 udp, 3 both, 0 invalid
    u32_t ipaddr;       // network order
    u16_t port_start;   // host order
    u16_t port_end;     // host order
    struct _port_fwd_list *next;
}port_fwd_list;


static inline u32_t nat_make_tcp_hash_key(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr, int dlul)
{
    if (LWIP_DL_DIRECTION == dlul) {
        return MAKE_HASH_KEY_NEW((iphdr->dest.addr) ^ (iphdr->src.addr), ntohs(tcphdr->dest) << 8UL, (((ntohs(tcphdr->src)) ^ (tcphdr->src)) & 0xF) << 4UL, IP_NAT_TCP);
    } else {
        return MAKE_HASH_KEY_NEW((iphdr->dest.addr) ^ (iphdr->src.addr), ntohs(tcphdr->src) << 8UL, (((ntohs(tcphdr->dest)) ^ (tcphdr->dest)) & 0xF) << 4UL, IP_NAT_TCP);
    }
}

#if 0
static inline u32_t nat_make_udp_hash_key(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr, int dlul)
{
    if (LWIP_DL_DIRECTION == dlul) {
        return MAKE_HASH_KEY_NEW((iphdr->dest.addr) ^ (iphdr->src.addr), ntohs(udphdr->dest) << 8UL, (((ntohs(udphdr->src)) ^ (udphdr->src)) & 0xF) << 4UL, IP_NAT_UDP);
    } else {
        return MAKE_HASH_KEY_NEW((iphdr->dest.addr) ^ (iphdr->src.addr), ntohs(udphdr->src) << 8UL, (((ntohs(udphdr->dest)) ^ (udphdr->dest)) & 0xF) << 4UL, IP_NAT_UDP);
    }
}
#endif
static inline u32_t nat_make_udp_hash_key(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr, int dlul)
{
    struct pseudo_ip_hdr pseudo_iphdr;
    int len = PSEUDO_IP_HDR_HASH_LEN;
    int hashkey = 0;

    if (LWIP_DL_DIRECTION == dlul) { //for dl, need turn dest to src
        pseudo_iphdr.src.addr = iphdr->dest.addr;
        pseudo_iphdr.dest.addr = iphdr->src.addr;
        pseudo_iphdr.src_port = udphdr->dest;
        pseudo_iphdr.dest_port = udphdr->src;
        pseudo_iphdr.ip_proto = IPH_PROTO(iphdr);
    } else { //ul as base, src as src, dest as dest
        pseudo_iphdr.src.addr = iphdr->src.addr;
        pseudo_iphdr.dest.addr = iphdr->dest.addr;
        pseudo_iphdr.src_port = udphdr->src;
        pseudo_iphdr.dest_port = udphdr->dest;
        pseudo_iphdr.ip_proto = IPH_PROTO(iphdr);
    }

    hashkey = lwip_strhash_gen((char*)&pseudo_iphdr, len);
    return (u32_t)hashkey;
}

static inline u32_t nat_make_icmp_hash_key(const struct ip_hdr *iphdr, const struct icmp_echo_hdr *icmphdr, int dlul)
{
    return MAKE_HASH_KEY_NEW(((iphdr->dest.addr) ^ (iphdr->src.addr)), (ntohs(icmphdr->seqno) & 0xFF) << 8UL, (ntohs(icmphdr->id) & 0xF) << 4UL, IP_NAT_ICMP);
}

static inline u32_t nat_make_gre_hash_key(const struct ip_hdr *iphdr, const void *grehdr, int dlul)
{
    return MAKE_HASH_KEY_NEW(((iphdr->dest.addr) ^ (iphdr->src.addr)), (ntohs(IPH_PROTO(iphdr)) & 0xFF) << 8UL, (IPH_PROTO(iphdr) & 0xF) << 4UL, IP_NAT_GRE);
}

extern int lwip_nat_ttl_tcp_set;
static inline void update_nat_entry_ttl(ip_nat_table_t * nat_entry, s32_t ttl)
{
	if (LWIP_NAT_TTL_INFINITE == ttl) {
		nat_entry->ttl = LWIP_NAT_TTL_INFINITE;
	} else {
		nat_entry->ttl = nat_is_tcp_stable_entry(nat_entry) ? lwip_nat_ttl_tcp_set : ttl;
	}
    nat_entry->ctick = OSAGetTicks();
}

#if IP_NAT
void  ip_nat_init(void);
void  ip_nat_tmr(void);
int  ip_nat_input(struct netif *netif, struct pbuf **pbuf);
int  ip_nat_out(struct netif *netif, struct pbuf *p);
int app_nat_input(struct netif *netif, struct pbuf *p);
int app_nat_output(struct netif *netif, struct pbuf *p);
void nat_tcp_status_update(ip_nat_table_t *nat_entry, struct tcp_hdr *tcphdr);
ip_nat_table_t *ip_nat_tcp_input(struct ip_hdr *iphdr, struct tcp_hdr *tcphdr);
ip_nat_table_t *ip_nat_tcp_output(struct ip_hdr *iphdr, struct tcp_hdr *tcphdr);
ip_nat_table_t *ip_nat_udp_input(struct ip_hdr *iphdr, struct udp_hdr *udphdr);
ip_nat_table_t *ip_nat_udp_output(struct ip_hdr *iphdr, struct udp_hdr *udphdr);
ip_nat_table_t * ip_nat_tcp_output_hit_cache(struct pbuf *p);
ip_nat_table_t * ip_nat_udp_output_hit_cache(struct pbuf *p);
port_fwd_item *ip_fwd_output(struct pbuf* p);
void ip_nat_tcp_mangle_window(struct netif *netif_in, ip_nat_table_t * nat_entry, struct tcp_hdr *tcphdr);
void ip_nat_mangle_ftp(struct netif *netif, struct pbuf ** ppbuf);
int app_hash_cache_get_in_tcp(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr, int nat_flag);
int app_hash_cache_get_out_tcp(const struct ip_hdr *iphdr, const struct tcp_hdr *tcphdr, int nat_flag);
int app_hash_cache_get_in_udp(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr, int nat_flag);
int app_hash_cache_get_out_udp(const struct ip_hdr *iphdr, const struct udp_hdr *udphdr, int nat_flag);
ip_nat_table_t * app_nat_key_hit_cache(const struct ip_hdr *iphdr, 
                                    const struct tcp_hdr *tcphdr,
                                    const struct udp_hdr *udphdr,
                                    u32_t flowid,
                                    int nat_flag);

void lwip_set_proxy_func(ip_addr_t * addr, u16_t port);
void lwip_set_proxy_flag(int flag);
void lwip_dump_proxy_set(char *buf, int *size_set);
void lwip_uplink_proxy_proc(ip_nat_table_t *nat_entry, struct ip_hdr *iphdr, struct tcp_hdr *tcphdr);
void lwip_downlink_proxy_proc(ip_nat_table_t *nat_entry, struct ip_hdr *iphdr, struct tcp_hdr *tcphdr);

int ip_port_fwd_rules(port_fwd_list *list, int type);
int ip_port_fwd_rule_add(port_fwd_list *list);
void ip_port_fwd_tmr(void);

#endif /* IP_NAT */

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __LWIP_NAT_H__ */
