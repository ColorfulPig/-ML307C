#ifndef __TFT_Core_H__
#define __TFT_Core_H__
#include "global_types.h"
#ifdef LWIP_IPNETBUF_SUPPORT
#include "tftdef.h"
#include "ip4.h"
#include "ip6.h"
#include "ip.h"

#endif

#ifndef LWIP_IPNETBUF_SUPPORT
typedef struct {
        unsigned int  min;           //low limit
        unsigned int  max;          //max limit
} NumericRange;

typedef struct _TFTNode{
	struct _TFTNode *pre;
	struct _TFTNode *next;
}TFTNode;
typedef struct {
        unsigned int  direction;
		UINT8            pIdNextHdr;    		/**< Protocol number (ipv4) / next header (ipv6) */
		Bool        pIdNextHdrPresent; 	/**< TRUE: if present \sa CCI API Ref Manual */ //Michal
        NumericRange localPortRange;
        NumericRange RemotePortRange;
        Bool  localPortRangePresent;
        Bool  RemotePortRangePresent;
        unsigned char  SecondaryCID;
        unsigned char  PfIdx;        // Packet Filter ID
        unsigned char  EpIdx;       // Evaluation Precedence Index
        Bool  TosPresent;
        unsigned char  Tos;         // Type of Service
        unsigned char  TosMask;         // Type of Service Mask
        Bool  SpiPresent;
        Bool  FlowLablePresent;
        unsigned int SPI;
        unsigned int  FlowLable;
		unsigned char IPType;
        unsigned int  RemoteAddress[4];
		Bool  RemoteAddressPresent;
        unsigned int  SubnetMask[4];
		Bool  SubnetMaskPresent;
		void *next;
} PacketFilterInfo;
//used by TFT module
typedef struct { 
		unsigned char bearer_id;
		unsigned char cid;
		unsigned char Pcid;
		unsigned char bearer_type;
		PacketFilterInfo  packetinfo;      
} TftInfoList;

typedef struct _PacketFilterInfoNode{
	TFTNode header;
	TftInfoList info;
}PacketFilterInfoNode;

/** Layout of the IPv6 header (rfc 2460) */
typedef struct ip6_hdr {
    unsigned int ip6_vtf;        /**<  Version (4), Traffic Class (8), Flow Label 20 */
    unsigned short ip6_len;         /**<  Length              */
    unsigned char ip6_nextheader;   /**<  Next header         */
    unsigned char ip6_hoplimit;     /**<  Hop Limit           */
    unsigned int ip6_src[4]; /**<  Source address      */
    unsigned int ip6_dst[4]; /**<  Destination address */
}__attribute__((packed)) IP6Hdr_t; /**< @see struct ip6_hdr */

typedef struct ip6_frag_hdr {
  /* next header */
  unsigned char _nexth;
  /* reserved */
  unsigned char reserved;
  /* fragment offset */
  unsigned short _fragment_offset;
  /* fragmented packet identification */
  unsigned int _identification;
} __attribute__((packed)) IP6FragHdr_t;
/** Layout of TCP header (rfc 793) */
typedef struct 
{
    unsigned char        ip_pvhl;        /* IP protocol version and header length */
    unsigned char        ip_tos;         /* type of service */
    unsigned short 		 ip_len;         /* total length */
    unsigned short       ip_id;          /* identification */
    unsigned short       ip_off;         /* fragment offset field */
    unsigned char        ip_ttl;         /* time to live */
    unsigned char        ip_p;           /* protocol */
    unsigned short       ip_sum;         /* checksum */
    unsigned int       ip_src;         /* source IP address */
    unsigned int       ip_dst;         /* dest IP address */
}__attribute__((packed)) ip4_hdr;

#define	IP_OFFMASK 0x1fff	/* mask for fragmenting bits */

//IP TYPE 0-IPV4V6; 1---IPV4; 2-IPV6
#define IP_TYPE_IPV4V6			0
#define IP_TYPE_IPV4			1
#define IP_TYPE_IPV6			2

#undef ntohs
#define ntohs(x) (((((x) >> 8) & 0xFF) + ((x) << 8)) & 0xFFFF)


#define IPH_OFFSET(hdr) ((hdr)->ip_off)
#define IPH_ID(hdr) ((hdr)->ip_id)

#define IPH_PROTO(hdr) ((hdr)->ip_p)
#define IPH_HL(hdr) (((hdr)->ip_pvhl) & 0x0f)
#define IPH_V(hdr)  ((hdr)->ip_pvhl >> 4)
#define IPH_SRC(hdr) ((hdr)->ip_src)
#define IPH_DEST(hdr) ((hdr)->ip_dst)

#define IPH6_SRC(hdr) ((hdr)->ip6_src)
#define IPH6_DEST(hdr) ((hdr)->ip6_dst)


#define IPH6_NH(hdr) ((hdr)->ip6_nextheader)

#else
#define IPH_SRC(hdr) ((hdr)->src.addr)
#define IPH_DEST(hdr) ((hdr)->dest.addr)

#define IPH6_SRC(hdr) ((hdr)->src.addr)
#define IPH6_DEST(hdr) ((hdr)->dest.addr)

#define IPH6_NH(hdr) ((hdr)->_nexth)

typedef struct ip_hdr ip4_hdr;
#endif


#define IP_PROTO_ICMP    1
#define IP_PROTO_IGMP    2
#define IP_PROTO_UDP     17
#define IP_PROTO_UDPLITE 136
#define IP_PROTO_TCP     6
#define IP_PROTO_GRE     47

#define IP_PROTO_DESTOPTS     60
#define IP_PROTO_ROUTING     43
#define IP_PROTO_FRAGMENT     44


#define IP4_DL_FILTER_COUNT		(8)
#define IP6_DL_FILTER_COUNT		(8)
#define STA_TFT_TABLE_SIZE   (32)
#define CACHE_SIZE 1024
#define CACHE_TIME 12 /* 50 sec */
#ifndef LWIP_IPNETBUF_SUPPORT
#define ENHASH
#endif
//#define DBG


struct tcp_hdr {
    unsigned short src;       /**< source port      */
    unsigned short dest;       /**< destination port */
    unsigned int  tcp_seq;            /**< sequence number  */
    unsigned int tcp_ack;            /**< ack number       */
    unsigned short tcp_offresflags;    /**< 4 bits hdr len +
                                   6 bits reserved +
                                   6 flags          */
#define TCP_LEN   0xf000U     /**< TCP header lenght bitmask  */
#define TCP_REX   0x0fc0U     /**< TCP reserved bits bitmask  */
#define TCP_FLAGS 0x003fU     /**< TCP flags bitmask          */
#define TCP_URG   0x0020U     /**< TCP urgent pointer bitmask */
#define TCP_ACK   0x0010U     /**< TCP flag ACK set           */
#define TCP_PSH   0x0008U     /**< TCP flag PSH set           */
#define TCP_RST   0x0004U     /**< TCP flag RST set           */
#define TCP_SYN   0x0002U     /**< TCP flag SYN set           */
#define TCP_FIN   0x0001U     /**< TCP flag FIN set           */
#define TCP_DEFAULTLEN 5      /**< Usual length of TCP header */

    unsigned short tcp_window;         /**< advertised window */
    unsigned short tcp_chksum;         /**< checksum          */
    unsigned short tcp_urgptr;         /**< urgent pointer    */

}__attribute__((packed)); /**< @see struct tcp_hdr */
/** Layout of UDP header (rfc 768) */
struct udp_hdr {
    unsigned short src;       /**< source port              */
    unsigned short dest;       /**< destination port         */
    unsigned short udp_len;            /**< UDP length               */
    unsigned short udp_chksum;         /**< UDP checksum             */
}__attribute__((packed)); /**< @see struct udp_hdr */
struct espid_ip6_info {
	unsigned int src_ip[4];
	unsigned int dest_ip[4];
	unsigned short int src_port;
	unsigned short int dest_port;
	unsigned int proto;
};

struct espid_ip4_info {
	unsigned int src_ip;
	unsigned int dest_ip;
	unsigned short int src_port;
	unsigned short int dest_port;
	unsigned int proto;
};



#define MAX_CID_COUNT_FOR_TFT   (16)

extern void TftAddNewItem(TftInfoList *, unsigned char, BOOL sim2Flag);
extern int tft_get_espid_by_ip6port(struct ip6_hdr *iphdr, void *tcpudp, int proto);
extern int tft_get_espid_by_ip4port(ip4_hdr *iphdr, void *tcpudp, int proto);

#endif

