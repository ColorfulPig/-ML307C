#include "err.h"
#include "sys.h"
#include "stats.h"
#include "snmp.h"
#include "netif.h"
#include "sys_arch.h"
#include "etharp.h"
#include "dhcpd_api.h"

#ifndef __NET_BRIDGE__
#define __NET_BRIDGE__

#define BRIDGE_TMR_SEC  60  /* 60 seconds */
#define NETB_TTL_SIZE   (2) /* 2 times */

#define NETB_MAC_LEN    (ETHARP_HWADDR_LEN + ETH_VLANID_SIZE)

/**********************************/
/*define net_mac_rec_t flags definition*/
/**********************************/
/* can do bridge network Interface */
#define NETB_REC_FLAG_BRIDGE        0x0001
/* define this ip config is form dhcp*/
#define NETB_REC_FLAG_DHCP_IP       0x0002
/* define this ip config is form arp msg*/
#define NETB_REC_FLAG_ARP_IP        0x0004
/* define this net_mac keep persistant, like config marked by mac+apnif*/
#define NETB_REC_FLAG_PERSIST       0x0008

#define net_pool_is_bridge(net)            (((net)->flags & NETB_REC_FLAG_BRIDGE) ? 1 : 0)
#define net_pool_is_dhcpip(net)            (((net)->flags & NETB_REC_FLAG_DHCP_IP) ? 1 : 0)
#define net_pool_is_persistant(net)        (((net)->flags & NETB_REC_FLAG_PERSIST) ? 1 : 0)

struct _net_mac_rec_st{
    u16_t            ttl;
	u16_t            vlanid;
    u32_t            src_ip;
    u16_t            flags;
	u16_t            index; /*record net_mac_rec index*/
    struct netif *   netif_lc; /*record ethernet packet input netif.*/
    struct netif *   netif_nw;  /*record packet output APN netif, or internet netif.*/
    struct netif *   netif_nw6;  /*record packet output APN netif, or internet netif.*/
    dongle_config_info_s * dongle_info; /*use for config dhcp process, according to netif_nw*/

#if LWIP_ARP
    struct eth_addr  eth_addr;
    char 			 mac_addr [NETB_MAC_LEN];
#endif
};
typedef struct _net_mac_rec_st net_mac_rec_t;

net_mac_rec_t *net_pool_findall_by_mac(char *mac_addr);
net_mac_rec_t *net_pool_findall_by_ip(u32_t ip);
net_mac_rec_t *net_pool_findall_by_nwif(struct netif *nwif);
net_mac_rec_t *net_pool_findall_by_nwif6(struct netif *nwif);
net_mac_rec_t *net_pool_find_by_ip(u32_t ip);
struct netif * net_pool_get_nw_bymac(char *mac_addr);
struct netif * net_pool_get_nw6_bymac(char *mac_addr);
struct netif * net_pool_get_nw6_byif(struct netif * netif_lc);
struct netif * net_pool_get_if_bynw(struct netif * nwif);
struct netif * net_pool_get_if_bynw6(struct netif * nwif);
struct netif * net_pool_get_if_byIP(u32_t ip);

#if LWIP_NETIF_BRIDGE
net_mac_rec_t *net_pool_fetch_latest(void);
net_mac_rec_t *net_pool_fetch_head(void);
struct netif * net_pool_get_if_dongle(void);

int net_pool_fetch_size(void);
err_t net_pool_update_ip(u32_t ipaddr, char *mac_addr, u16_t flag);
err_t net_pool_remove_ip(u32_t ipaddr);

void net_bridge_tmr(void);
void net_bridge_init(void);
u8_t net_bridge_input(struct netif *netif, struct pbuf *p);
u8_t net_bridge_arp(struct netif *netif, struct pbuf *p);
u8_t net_bridge_input_ip6( struct netif *netif, struct pbuf *p);
err_t net_pre_routing(struct netif *netif, struct pbuf *p);
err_t net_pre_routing6(struct netif *netif, struct pbuf *p);

#if 0
struct netif * netif net_post_routing(struct netif *netif_in, struct pbuf *p);
#endif

#endif

#endif /* __NET_BRIDGE__ */
