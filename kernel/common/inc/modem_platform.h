/*
 * modem_platform.h -- modem platform specific header file
 *
 * (C) Copyright [2006-2008] Marvell International Ltd.
 * All Rights Reserved
 *
 */

#ifndef __MODEM_PLATFORM_H__
#define __MODEM_PLATFORM_H__

#if defined __cplusplus  /* __cplusplus */
extern "C" {
#endif

#include "ppp_platform.h"
#include "ppp_types.h"
#include "utils.h"
//#include "mvUsbModem.h"
//#include "UART.h"

#define PPP_PLATFORM_CRANE
#define PROPERTY_VALUE_MAX 16
#define PPP_APN_STR_MAX_LEN 99

#define SUPPORT_RDP
#define perror(args...) do { } while (0)

#define PPP_SEM_WAIT
#define PPP_SEM_POST

#define PPP_AT_CMD_ATP_MODEM TEL_AT_CMD_ATP_MODEM

#define AT_CMD_BUF_SIZE 3*1024
#define AT_RESP_BUF_SIZE 1024
#define MAX_DUMP_BUF_SIZE 128
#define MAX_STR_SIZE 128

#define INVALID_CID 0xFF
#define TIMEOUT_CGACT_DEACT	20
#define TIMEOUT_CGDATA		60
#define TIMEOUT_CGGETIP		10
#define TIMEOUT_AUTHReq		10
#define TIMEOUT_CGDCONT       10
#define TIMEOUT_CGDFLT		10


#define PROP_PPP_CID "cid"

#define DATABITS	CS8
#define BAUD		B115200
#define STOPBITS	0
#define PARITYON	0
#define PARITY		0

#define MODEM_MESSAGE_Q_SIZE 16

#undef IP_TYPE_IPV4V6
#define IP_TYPE_IPV4V6			0x3

#undef IP_TYPE_IPV4
#define IP_TYPE_IPV4			0x1

#undef IP_TYPE_IPV6
#define IP_TYPE_IPV6			0x2
#ifdef PPP_WITHOUT_ATSRV
enum modem_state {
	STATE_OFFLINE,
	STATE_CRASH_RESET,
	STATE_CRASH_EXIT,
	STATE_BOOTING,
	STATE_ONLINE,
	STATE_NV_REBUILDING,
	STATE_LOADER_DONE,
	STATE_SIM_ATTACH,
	STATE_SIM_DETACH,
};
#else
#define MODEM_ATCMD_SOCKET "/tmp/atcmdmni"
#endif

 typedef struct {
        int  inIpAddress[4];
        int  inPrimaryDNS[4];
        int  inSecondaryDNS[4];
        int  inDefaultGateway[4];
        int  inSubnetMask[4];
} ipv6config_t;

typedef struct direct_ipconfig {
  int dwContextId;
  int dwProtocol;
  struct
      {
        int inIPAddress;
        int inPrimaryDNS;
        int inSecondaryDNS;
        int inDefaultGateway;
        int inSubnetMask;
      } ipv4;

  ipv6config_t ipv6;
}direct_ipconfig_t;

typedef struct _modem_atcmd_interface {
	int atcmd_result;
	int ip_dns_has_update;
	int ipv6_dns_has_update;
	int atcmd_no_suspend;
	BOOL pdp_deactive_send;
	int ip_type; // 1: v4, 2:v4, 3:v4v6
	direct_ipconfig_t ipParams;
	//unsigned char ipv6_addr[16];
	OSSemaRef atcmd_response;
} modem_atcmd_interface;

typedef struct _pdp_params {
	char apn[PPP_APN_STR_MAX_LEN + 1];
	char ip_type;
	char auth_type;
	char username[64];
	char password[64];
}pdp_params_t;

typedef struct _ppp_statistics {
	unsigned long long tx_byte;
	unsigned long long rx_byte;
} ppp_statistics_t;

typedef enum _msg_from
{
	MSG_FROM_MODEM,
	MSG_FROM_AT,
	MSG_FROM_COMM
} msg_from_e;

typedef struct _modem_msg {
	msg_from_e  from;
	int atp_index;
	int  len;
	char *data;
} modem_msg_t;

/* sync with MODEM_STATE in telatci.h */
#define ACM_DATA_MODE 1
#define ACM_CONTROL_MODE 0
#define MODEM_ECHO_MODE 1
#define MODEM_NORMAL_MODE 0
typedef struct _modem_control_interface {
	PPP_TASK_t modem_router_ref;
	ppp_cb_funcs ppp_cbs;
	OSSemaRef router_exit;
	unsigned int modem_state;
	unsigned char modem_current_cid;
	unsigned int echo_mode;
	unsigned int atp_index;
	modem_atcmd_interface atcmd_if;
	BOOL get_ip ;
	BOOL ccinet_down;
	UINT8 cgreg_flag;
	pdp_params_t pdp_params;
	pdp_params_t pdp_params_sim1;
	OSMsgQRef  msg_ref;
} modem_control_interface;

extern modem_control_interface modem_if;
extern unsigned int old_connected_ppp_cid;

/***************************** Declarations ************************************/
/* Send PACKET to MODEM */
extern void ppp_tx_modem(const U_CHAR *packet, U_INT len, BOOL isIP);
/* Send PACKET to COMM */
extern int ppp_tx_comm(const U_CHAR *packet, U_INT len);
/* Send ATCMD to ATCMDSRV */
extern int modem_ppp_terminate(U_CHAR cid, BOOL suspend);
extern int modem_ppp_connect(U_CHAR cid, BOOL suspend);
extern int modem_ppp_authenticate(U_CHAR cid, PppAuthenticationParamsS *authentication_params);
extern int modem_ppp_cgdcont(U_CHAR cid);
extern int modem_ppp_get_ip(U_CHAR cid);
extern int modem_ppp_cgdflt(void);
/* Recv AT Response from ATCMDSRV */
extern void modem_parse_atcmd(U_CHAR *packet, int size);
/* Debug AT Functions */
extern void ppp_dump_string(const char *name, char *buf, int len);
extern void ppp_dump_buffer(const char *name, char *buf, int len);
extern void ppp_get_statistics(unsigned long long *ret_tx, unsigned long long *ret_rx);
extern void ppp_get_instant_statistics(unsigned long long *ret_tx, unsigned long long *ret_rx);
extern void modem_clear_ip_update_flag(void);
extern UINT8 get_ppp_iptype(void);
extern char *get_ppp_apn(void);
void ppp_store_auth_params(U_CHAR *line);
int modem_is_ip_update(void);
int modem_is_ipv6_update(void);
void ppp_set_netif(UINT8 status, UINT8 ip_type);


#if defined __cplusplus
}
#endif /* __cplusplus */

#endif
