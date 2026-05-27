#ifndef __IP_FILTER_H__
#define __IP_FILTER_H__

#include "opt.h"
#include "pbuf.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _ip_info_t
{
    u16_t   src_port;       // src port,    0 - invalid
    u16_t   dst_port;       // dst port,    0 - invalid
    u32_t   src_ip;         // src ip,      0 - invalid
    u32_t   dst_ip;         // dst ip,      0 - invalid
}ip_info_t;

typedef struct _ip_elt_t
{
    u16_t   port_start;     // port start,  0 - invalid
    u16_t   port_end;       // port end,    0 - invalid
    u32_t   ip_addr;        // ip address,  0 - invalid
    u32_t   net_mask;       // ip net mask, 0 - invalid
}ip_elt_t;

typedef struct _ip_filter_t
{
    u16_t       flag;       // 0: not using; 1: using
    u8_t        type;       // 1 - tcp, 2 - udp, 3 - tcp and udp
    u8_t        action;     // 0: allow the packet; 1: drop the packet;

    ip_elt_t    src;        // source
    ip_elt_t    dst;        // destination
}ip_filter_t;

typedef struct _ip_filter_param_list
{
    unsigned char   protocol;           /*0: invalid; 1:TCP,2:UDP,3:BOTH TCP/UDP,4:ICMP */
    unsigned char   action;             //0: allow the packet; 1: drop the packet;
    unsigned char   net_mask1;          //0: invalid 24/32 mode
    unsigned char   net_mask2;          //0: invalid 24/32 mode

    unsigned int    src_ip;             //0: invalid
    unsigned int    des_ip;             //0: invalid

    unsigned short  src_port_start;     //0: invalid
    unsigned short  src_port_end;       //0: invalid

    unsigned short  des_port_start;     //0: invalid
    unsigned short  des_port_end;       //0: invalid

    struct _ip_filter_param_list *next;
}ip_filter_param_list;

typedef struct _dns_filter_param_list
{
    unsigned int   action;            //0: allow the packet; 1: drop the packet;
    char            domain [128];     //domain name
    unsigned int    src_ip_start;     //0: invalid
    unsigned int    src_ip_end;       //0: invalid

    struct _dns_filter_param_list *next;
}dns_filter_param_list;

typedef struct _dns_filter_t
{
    u16_t    flag;       // 0: not using; 1: using
    u8_t     action;     // 0: allow the packet; 1: drop the packet;
    u8_t     u8_align;   // 0: allow the packet; 1: drop the packet;
    char *   domain;     //domain name
    u32_t    src_ip_start; //0: invalid
    u32_t    src_ip_end;   //0: invalid
}dns_filter_t;

int ip_filter_ip_layer_icmp(struct pbuf *p);
int ip_filter_add_one_rule(ip_filter_param_list* elt);
int ip_filter_remove_one_rule(ip_filter_param_list* elt);
void ip_filter_set_deny_icmp_default(void);
void ip_filter_set_allow_icmp_default(void);
int ip_filter_ip_layer(struct pbuf *p);

/*return: 0, denote not filter, pass;1, denote filter;*/
int dns_filter_process(char * name_query, u32_t ipaddr);
int dns_filter_set_rules(dns_filter_param_list* list);
int dns_filter_get_en(void);

#ifdef __cplusplus
}
#endif

#endif /* __IP_FILTER_H__ */

