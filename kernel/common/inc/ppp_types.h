/*
 * ppp_types.h -- Network Pointer to Pointer Protocol header file
 *
 * (C) Copyright [2006-2008] Marvell International Ltd.
 * All Rights Reserved
 *
 */

#ifndef __PPP_TYPES_H__
#define __PPP_TYPES_H__

#if defined __cplusplus  /* __cplusplus */
extern "C" {
#endif

#include "ppp_platform.h"


#define PPP_FRAME_SIZE		0xC00     /*3072*/
#define PPP_LCP_MRU_SIZE	1500	/* 1500 bytes */
#define PPP_FRAME_ESCAPED_SIZE  1600 /* 1600 bytes */
#define PPP_LCP_RECV_MRU_SIZE	1500
#define PPP_LCP_RECV_ACCM 	0x00
#define LCP_MAX_OPTIONS		0x1E
#define PPP_MAX_CHAP_NAME_LENGTH        50
#define PPP_MAX_CHAP_CHALLENGE_LENGTH   50
#define PPP_MAX_CHAP_RESPONSE_LENGTH    50
#define PPP_MAX_AUTH_MESSAGE_LENGTH	200

#define CHAP_CHALLENGE_RESPONSE_TIMEOUT		6*1000
#define PPP_TERMINATE_TIMEOUT			15*1000

#define LCP_ECHO_REQ_INTERVAL_TIME		10*1000
#define LCP_ECHO_REQ_TIMEOUT			15*1000
#define LCP_ECHO_REQ_NUM_TRIES			10
#define PPP_OUT_FRAME_RESERVED_HAED_SIZE 0
#define PPP_OUT_FRAME_RESERVED_TIAL_SIZE 0
#define PPP_OUT_FRAME_RESERVED_SIZE (PPP_OUT_FRAME_RESERVED_HAED_SIZE + PPP_OUT_FRAME_RESERVED_TIAL_SIZE)
#define PPP_MEM_FREE_CHUNK_MAX  0

/*
 * Constants and structures defined by the internet system,
 * Per RFC 790, September 1981, and numerous additions.
 */

/*
 * The basic PPP frame.
 */
#define PPP_HDRLEN      4	/* octets for standard ppp header */
#define PPP_FCSLEN      2	/* octets for FCS */

#define LCP_MRU_LEN     4
#define LCP_ACCM_LEN    6
#define LCP_AUTH_LEN    4
#define LCP_MAGIC_LEN   6
#define LCP_PFC_LEN     2
#define LCP_ACFC_LEN    2

/*
 * Significant octet values.
 */

#define PPP_ALLSTATIONS 0xff	/* All-Stations broadcast address */
#define PPP_UI          0x03	/* Unnumbered Information */
#define PPP_FLAG        0x7e	/* Flag Sequence */
#define PPP_ESCAPE      0x7d	/* Asynchronous Control Escape */
#define PPP_TRANS       0x20	/* Asynchronous transparency modifier */

/*
 * Protocol field values.
 */
#define PPP_IP          0x21	/* Internet Protocol */
#define PPP_IPv6        0x57	/* Internet Protocol Version 6 */
#define PPP_AT          0x29	/* AppleTalk Protocol */
#define PPP_VJC_COMP    0x2d	/* VJ compressed TCP */
#define PPP_VJC_UNCOMP  0x2f	/* VJ uncompressed TCP */
#define PPP_COMP        0xfd	/* compressed packet */
#define PPP_IPCP        0x8021	/* IP Control Protocol */
#define PPP_ATCP        0x8029	/* AppleTalk Control Protocol */
#define PPP_CCP         0x80fd	/* Compression Control Protocol */
#define PPP_LCP         0xc021	/* Link Control Protocol */
#define PPP_PAP         0xc023	/* Password Authentication Protocol */
#define PPP_LQR         0xc025	/* Link Quality Report protocol */
#define PPP_CHAP        0xc223	/* Cryptographic Handshake Auth. Protocol */
#define PPP_CBCP        0xc029	/* Callback Control Protocol */
#define PPP_IPv6CP      0x8057  /* IPv6 Control Protocol*/

#define CHAP_MD5	0x05
#define DEFAILT_ACCM_MAP 0xFFFFFFFF

/*
 * Values for FCS calculations.
 */
#define PPP_INITFCS     0xffff	/* Initial FCS value */
#define PPP_GOODFCS     0xf0b8	/* Good final FCS value */
#define PPP_FCS(fcs, c) (fcs = (((fcs) >> 8) ^ fcstab[((fcs) ^ (c)) & 0xff]))

#define PPP_ESCAPE_C(d, s)	((d) = ((s)^0x20))

#define PPP_SET_BIT(b, s)	((b) |= (1<<(s)))
#define PPP_CLEAR_BIT(b, s)	((b) = (b)&(~(1<<(s))))
#define PPP_IS_BIT_SET(b, s) ((b)&(1<<(s)))

/*
 * Inline versions of get/put char/short/long.
 * Pointer is advanced; we assume that both arguments
 * are lvalues and will already be in registers.
 * cp MUST be U_CHAR *.
 */

#define GETCHAR(c, cp) { \
	(c) = *(cp)++; \
}
#define PUTCHAR(c, cp) { \
	*(cp)++ = (U_CHAR) (c); \
}

#define GETSHORT(s, cp) {\
	(s) = *(cp); (cp)++; (s) <<= 8;\
	(s) |= *(cp); (cp)++;\
}
#define PUTSHORT(s, cp) { \
	*(cp)++ = (U_CHAR) ((s) >> 8); \
	*(cp)++ = (U_CHAR) (s & 0xff); \
}

#define GETLONG(l, cp) { \
	(l) = *(cp); (cp)++; (l) <<= 8; \
	(l) |= *(cp); (cp)++; (l) <<= 8; \
	(l) |= *(cp); (cp)++; (l) <<= 8; \
	(l) |= *(cp); (cp)++; \
}
#define PUTLONG(l, cp) { \
	*(cp)++ = (U_CHAR) ((l) >> 24); \
	*(cp)++ = (U_CHAR) ((l) >> 16); \
	*(cp)++ = (U_CHAR) ((l) >> 8); \
	*(cp)++ = (U_CHAR) (l); \
}

/*
 *	Same macros as above, but ptr is not incremented
 */

#define GETCHAR_NOINC(c, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	(c) = *(p); \
}
#define PUTCHAR_NOINC(c, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	*(p) = (U_CHAR) (c); \
}

#define GETSHORT_NOINC(s, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	(s) = *(p); (p)++; (s) <<= 8; \
	(s) |= *(p);  \
}
#define PUTSHORT_NOINC(s, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	*(p)++ = (U_CHAR) ((s) >> 8); \
	*(p) = (U_CHAR) (s & 0xff); \
}

#define GETLONG_NOINC(l, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	(l) = *(p); (p)++; (l) <<= 8; \
	(l) |= *(p); (p)++; (l) <<= 8; \
	(l) |= *(p); (p)++; (l) <<= 8; \
	(l) |= *(p);  \
}
#define PUTLONG_NOINC(l, cp) { \
	U_CHAR *p = (U_CHAR *)cp; \
	*(p)++ = (U_CHAR) ((l) >> 24); \
	*(p)++ = (U_CHAR) ((l) >> 16); \
	*(p)++ = (U_CHAR) ((l) >> 8); \
	*(p) = (U_CHAR) (l); \
}

/******* SET INIT LCP PARAMETERS **********/
#define PPP_LCP_INIT_MRU(x) { \
	x.Header.Type = LCP_MRU; \
	x.Header.Length = LCP_MRU_LEN; \
}

#define PPP_LCP_INIT_ACCM(x) { \
	x.Header.Type = LCP_ACCM; \
	x.Header.Length = LCP_ACCM_LEN; \
}

#define PPP_LCP_INIT_AUTH_PROTO(x) { \
	x.Header.Type = LCP_AUTH; \
	x.Header.Length = LCP_AUTH_LEN; \
}

#define PPP_LCP_INIT_MAGIC_NUM(x) { \
	x.Header.Type = LCP_MAGIC; \
	x.Header.Length = LCP_MAGIC_LEN; \
}

/********* SET LCP PARAMETERS *************/
#define PPP_LCP_SET_MRU(m, s) { \
	m.Mru.Mru = (s);\
}

#define PPP_LCP_SET_ACCM(a, v) { \
	a.Accm.Accm = (v);\
	PPP_SET_BIT(a.supportedParams, LCP_ACCM);\
}

#define PPP_LCP_SET_AUTH_PROTO(a, v) { \
	a.AuthProtocol.Proto = (v);\
	PPP_SET_BIT(a.supportedParams, LCP_AUTH);\
}

#define PPP_LCP_SET_MAGIC_NUM(m) { \
	U_INT	n;\
	n = PPP_RAND();\
	m.MagicNumber.Number = n;\
	PPP_SET_BIT(m.supportedParams, LCP_MAGIC);\
}

#define PPP_LCP_SET_PFC(a, v)	{ \
	(a).isPfcEnabled = (v); \
	PPP_SET_BIT(a.supportedParams, LCP_PFC); \
}

#define PPP_LCP_SET_ACFC(a, v)	{ \
	(a).isAcfcEnabled = (v); \
	PPP_SET_BIT(a.supportedParams, LCP_ACFC); \
}

/*********** APPEND CHAR/SHORT/LONG ***********/
#define PPP_APPEND(bUF, iNDEX, cHAR) { \
	*(bUF+iNDEX) = (cHAR); \
	iNDEX++; \
}

#define PPP_APPEND_SHORT(bUF, iNDEX, sHORT) { \
	U_SHORT t;\
	U_CHAR *p;\
	PUTSHORT_NOINC((sHORT), &t);\
	p = (U_CHAR *)&t;\
	PPP_APPEND(bUF, iNDEX, (*p)&0xFF);\
	p++;\
	PPP_APPEND(bUF, iNDEX, (*p)&0xFF);\
}

#define PPP_APPEND_LONG(bUF, iNDEX, lONG) { \
	U_INT t = (lONG);\
	PPP_APPEND(bUF, iNDEX, (t>>24)&0xFF);\
	PPP_APPEND(bUF, iNDEX, (t>>16)&0xFF);\
	PPP_APPEND(bUF, iNDEX, (t>>8)&0xFF);\
	PPP_APPEND(bUF, iNDEX, t&0xFF);\
}

/********* PPP STATE ***********/
typedef enum {
	PPP_STATE_NONE,
	PPP_STATE_LCP,
	PPP_STATE_PAP,
	PPP_STATE_IPCP,
	PPP_STATE_CONNECTED
} PppStateE;

typedef enum _QueueMessagesE {
	PPP_MESSAGE_FROM_TERM,
	PPP_MESSAGE_FROM_COMM,
	PPP_MESSAGE_DEINIT,

	PPP_LAST_Q_MSG = 0xFF
} QueueMessagesE;

typedef enum _MessageTypeE {
	CONFIG_REQ = 1,
	CONFIG_ACK,
	CONFIG_NAK,
	CONFIG_REJ,
	TERMINATE_REQ,
	TERMINATE_ACK,
	CODE_REJECT,
	PROTO_REJECT,
	ECHO_REQUEST,
	ECHO_REPLY,
	DISCARD_REQUEST,
	IDENTIFICATION,
	TIME_REMAINING,
	LAST_MESSAGE_TYPE = 0xFF
} MessageTypeE;

typedef enum _LcpOptionE {
	LCP_VENDOR,
	LCP_MRU,
	LCP_ACCM,/* Asynchronous-Control-Character-Map*/
	LCP_AUTH,
	LCP_QUALITY,
	LCP_MAGIC,
	LCP_RESERVED,
	LCP_PFC,/*Protocol-Field-Compression*/
	LCP_ACFC,/*Address-and-Control-Field-Compression*/
	LCP_FCS,
	LCP_MAXSUPPORTEDOPTION,
	/*
	   LCP_SDP,
	   LCP_NUMMODE,
	   LCP_MULTILINK,
	   LCP_CALLBACK,
	   LCP_CONNECTTIME,
	   LCP_COMPFRAME,
	   */
	LCP_OPTEND = 0Xff
} LcpOptionE;

typedef enum _PapOptionsE {
	PAP_AUTHENTICATE_REQ = 1,
	PAP_AUTHENTICATE_ACK,
	PAP_AUTHENTICATE_NAK,

	PAP_OPTION_LAST = 0xFF
} PapOptionsE;

typedef enum _ChapOptionsE {
	CHAP_AUTHENTICATE_CHALLENGE = 1,
	CHAP_AUTHENTICATE_RESPONSE,
	CHAP_AUTHENTICATE_SUCCESS,
	CHAP_AUTHENTICATE_FAIL,

	CHAP_OPTION_LAST = 0xFF
} ChapOptionsE;

typedef struct _PppHeaderS {
	U_CHAR Address;
	U_CHAR Control;
} PppHeaderS;

typedef struct _PppMessageS {
	U_SHORT Protocol;
	U_CHAR Data[1];
} PppMessageS;

typedef struct _PppFrameS {
	PppHeaderS Header;
	PppMessageS Message;
} PppFrameS;

typedef struct _MessageBufS {
	void *Buf;
	U_INT Length;
} MessageBufS;

typedef struct _LcpMessageS {
	U_CHAR Type;
	U_CHAR Id;
	U_SHORT Length;
	U_CHAR Data[1];
} LcpMessageS, PapMessageS, ChapMessageS, PppMessageHeaderS;

typedef struct _LcpOptionS {
	U_CHAR Type;
	U_CHAR Length;
	U_CHAR Data[1];
} LcpOptionS, PppOptionS;

typedef struct _LcpOptionHeaderS {
	U_CHAR Type;
	U_CHAR Length;
} LcpOptionHeaderS;

typedef struct _LcpMruS {
	LcpOptionHeaderS Header;
	U_SHORT Mru;
} LcpMruS;

typedef struct _LcpAuthProtoS {
	LcpOptionHeaderS Header;
	U_SHORT Proto;
} LcpAuthProtoS;

typedef struct _LcpMagicNumberS {
	LcpOptionHeaderS Header;
	U_INT Number;
} LcpMagicNumberS;

typedef struct _LcpAccmS {
	LcpOptionHeaderS Header;
	U_INT Accm;
} LcpAccmS;

typedef struct _LcpPfcS {
	LcpOptionHeaderS Header;
} LcpPfcS;

typedef struct _LcpAcfcS {
	LcpOptionHeaderS Header;
} LcpAcfcS;

typedef struct _LcpParamsS {
	LcpMruS Mru;
	LcpAuthProtoS AuthProtocol;	/* Authentication Protocol */
	LcpMagicNumberS MagicNumber;
	LcpAccmS Accm;

	BOOL isPfcEnabled;
	BOOL isAcfcEnabled;

	U_INT supportedParams;

} LcpParamsS;

typedef struct _PapAuthReqS {
	U_CHAR UserLen;
	U_CHAR *UseName;

	U_CHAR PassLen;
	U_CHAR *Password;

} PapAuthReqS;

typedef struct _ChapChallengeS {
	U_CHAR challengeValueLength;
	U_CHAR challengeValue[PPP_MAX_CHAP_CHALLENGE_LENGTH];
	U_CHAR challengeNameLength;
	U_CHAR challengeName[PPP_MAX_CHAP_NAME_LENGTH];
} ChapChallengeS;

typedef struct _ChapResponseS {
	U_CHAR responseValueLength;
	U_CHAR responseValue[PPP_MAX_CHAP_RESPONSE_LENGTH];
	U_CHAR responseNameLength;
	U_CHAR responseName[PPP_MAX_CHAP_NAME_LENGTH];
} ChapResponseS;

typedef struct _ChapSuccFailS {
	U_CHAR messageLength;
	U_CHAR message[PPP_MAX_AUTH_MESSAGE_LENGTH];
} ChapSuccFailS;

typedef struct {
	char Username[256];
	char Password[256];
} PapAuthenticationParamsS;

typedef struct {
	ChapChallengeS  *challenge;
	ChapResponseS *response;
	U_CHAR id;
}ChapAuthenticationParamsS;

typedef struct {
	U_SHORT auth_type;
	union {
		PapAuthenticationParamsS *PapAuthenticationParams;
		ChapAuthenticationParamsS ChapAuthenticationParams;
	};
} PppAuthenticationParamsS;

/************************************************************************/
/* IPCP                                                                 */
/************************************************************************/
/* Defined by RFC 1332, there are total of 10 IPCP options */
/* This implementation supports only 4, as listed in IpcpOptionsE */
#define IPCP_NUM_AVAILABLE_OPTIONS	10
#define IPCP_DUMMY_ADDRESS			0x10000001	/* 10.0.0.1 */
#define IPCP_DUMMY_DNS              0xFFFFFF00	/* 255.255.255.0 */

typedef enum _IpcpOptionsE {
	IPCP_OPT_IP_ADDRESSES = 1, /*has been replaced by IPCP_OPT_IP_ADDRESS*/
	IPCP_OPT_IP_COMPRESSION_PROTOCOL = 2,
	IPCP_OPT_IP_ADDRESS = 3,
	IPCP_OPT_PRIMARY_DNS_ADDRESS = 0x81,
	IPCP_OPT_SECONDARY_DNS_ADDRESS = 0x83
} IpcpOptionsE;

typedef enum _Ipv6cpOptionsE {
	IPV6CP_OPT_INTERFACE_IDENTIFIER = 1,
	IPV6CP_OPT_IP_COMPRESSION_PROTOCOL = 2
} Ipv6cpOptionsE;

typedef enum _HdlcFlagStateE {
	HDLC_FLAG_NONE,
	HDLC_FLAG_FIRST,
	HDLC_FLAG_LAST
} HdlcFlagStateE;

/* IPCP Option 0x02 */
typedef struct _IpcpIpCompressionS {
	U_CHAR Type;
	U_CHAR Length;
	U_SHORT Protocol;
	U_CHAR Data[1];
} IpcpIpCompressionS;

/* IPCP Option 0x03 */
typedef struct _IpcpIpAddressS {
	U_CHAR Type;
	U_CHAR Length;
	U_INT Address;
} IpcpIpAddressS;

/* IPCP Option 0x81,0x83 */
typedef U_INT IpcpPrimaryDns;
typedef U_INT IpcpSecondaryDns;

typedef struct _IpcpConnectionParamsS {
	U_INT IpAddress;
	U_INT PrimaryDns;
	U_INT SecondaryDns;
	U_CHAR Ipv6InterfaceId[8];
} IpcpConnectionParamsS;

typedef struct _IpcpParamsS {
	IpcpIpCompressionS IpCompression;
	IpcpIpAddressS IpRemoteAddress;
	IpcpIpAddressS IpMobileAddress;
	U_INT IpAddress;
	U_INT PrimaryDns;
	U_INT SecondaryDns;
} IpcpParamsS;

typedef enum _IpVersionE {
	IPV4,
	IPV6
} IpVersionE;

/************************************************************************/
/* Message Queue                                                        */
/************************************************************************/
typedef struct _QueueMessageS {
	void *Ptr;
	//U_INT Size;/*No Use*/
	U_CHAR Type;
} QueueMessageS;

/************************************************************************/
/* PPP Main                                                             */
/************************************************************************/

typedef struct _PppControlS {
	PPP_TASK_t ppp_handler_ref;
	PPP_MsgQ_t MsgQRef;

	/* how many incoming packets are allowed in queue
	 * before start dropping */
	U_INT QueuedDataInPacketsCount;
	PppStateE PppState;
	BOOL isConfigReqSent;
	BOOL isConnectIndSent;

	/* PdpDown:
	 * TRUE:  PPP & PC link break down
	 * FALSE: PPP & CP link not break down */
	BOOL PdpDown;

	IpVersionE ipVersion;

	U_CHAR Cid;

	/* incomming PPP frame, excluding START/END flag(0x7e)
	 * and escape chars(0x7d) */
	MessageBufS *InFrameContainer;
	MessageBufS *InFramePlaceholder;
	/* transmitted PPP frame, including START/END flag
	 *  and escape chars */
	MessageBufS OutFrameContainer;
	/* transmitted PPP message before encoding */
	MessageBufS OutMessageContainer;
	PppMessageS *InMessage;

	BOOL IsRxFragmented;
	BOOL InFramePlaceholderFreed;
	BOOL IsEscapeFound;
	HdlcFlagStateE FlagState;
	U_SHORT IncomingFcs;

	U_CHAR LastRecvId;
	U_CHAR LastXmitId;

	/* points to rejected option in message */
	LcpOptionS *lcpOptionRej;

	LcpParamsS lcpRecvParams;
	LcpParamsS lcpXmitParams;

	U_CHAR lcpEchoReqTimeoutCount;
	BOOL isLcpEchoRequestEnabled;

	ChapChallengeS chapChallenge;
	ChapResponseS chapResponse;
	ChapSuccFailS chapSuccFail;
	U_CHAR chapChallengeTryCount;

	PapAuthenticationParamsS PapAuthenticationParams;

	PppAuthenticationParamsS PppAuthenticationParams;
	/* user space timer */
	TIMER_t pppTerminateTimeoutTimer;
	TIMER_t lcpEchoReqTimeoutTimer;
	TIMER_t lcpEchoReqIntervalTimer;
	TIMER_t chapChallengeTimeoutTimer;

	IpcpParamsS ipcpRecvParams;
	IpcpParamsS ipcpXmitParams;
	/* Other peer ack'ed our Config Request */
	BOOL ipcpRecvAck;
	BOOL ipv6cpRecvAck;

	/* This peer sent ack to other peer's config req */
	BOOL ipcpXmitAck;
	BOOL ipv6cpXmitAck;

	U_INT CPOOMDrops;
	U_INT APOOMDrops;
	U_INT QUEFULLDrops;
	U_CHAR ipv6cp_recv_interface_id[8];
	U_CHAR ipv6cp_xmit_interface_id[8];
	BOOL ipcpXmitUpdated;
	SEM_t task_exit;
	U_LLONG receivedBytes;
	U_LLONG processedBytes;
} PppControlS;

#if defined __cplusplus
}
#endif /* __cplusplus */

#endif /*  __PPP_TYPES_H__ */
