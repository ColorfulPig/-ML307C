/*===========================================================================

    Dialer TASK Header File

DESCRIPTION
  This header file contains all the definitions necessary for
  clients to interface with the Dial module.

Copyright (c) 2011 by MARVELL INCORPORATED. All Rights Reserved.

Export of this technology or software is regulated by the U.S. Government.
Diversion contrary to U.S. law prohibited.

===========================================================================*/

/*===========================================================================

                      EDIT HISTORY FOR FILE

  This section contains comments describing changes made to the module.
  Notice that changes are listed in reverse chronological order.

when        who         what, where, why
--------   ------     -------------------------------------------------------
11/16/08   yhuang      Create dialer header file

===========================================================================*/

#ifndef __DIALER_TASK_H__
#define __DIALER_TASK_H__


#include <stdio.h>
#include "osa.h"
#include "UART.h"
#include "stdlib.h"
#include "string.h"
#include "ci_ps.h"
//#include "telatci.h"
//#include "telcontroller.h"
#include "teldef.h"
//#include "dial_macro.h"
#include "wan_applet.h"
#include "device_management_applet.h"
#include "pdpdef.h"
#include "telatparamdef.h"
//#include "telutl.h"

#ifndef PCAC_AT_CLIENT
#define PCAC_AT_CLIENT
#endif

#ifndef SMS_ENABLE
#define SMS_ENABLE
#endif

#ifdef PCAC_AT_CLIENT
/*===========================================================================

                         PUBLIC DATA DECLARATIONS

===========================================================================*/

#define DIALER_MSG_SIZE        384          //256
#ifdef SINGLE_SIM
#define DIALER_CMD_MSG_SIZE    128
#else
#define DIALER_CMD_MSG_SIZE    256
#endif
#define DIALER_TASK_STACK_SIZE (6*1024)
#define DIALER_TASK_PRIORITY   128

#define RE_DIALER_CMD_MSG_SIZE    64
#define RE_DIALER_TASK_STACK_SIZE  1024
#define RE_DIALER_TASK_PRIORITY   128

/* use 0\1\2 to denote ip type*/
/* note: this is not match lwip define, need turn when transfer to lwip*/
#undef IP_TYPE_IPV4V6
#define IP_TYPE_IPV4V6			0x0

#undef IP_TYPE_IPV4
#define IP_TYPE_IPV4			0x1

#undef IP_TYPE_IPV6
#define IP_TYPE_IPV6			0x2


#ifndef PDP_TYPE_DEFAULT
#define PDP_TYPE_DEFAULT        0x0
#endif
#ifndef PDP_TYPE_DEDICATED
#define PDP_TYPE_DEDICATED      0x1
#endif

//connect type
#define CM_CONTYPE_MANUAL			1
#define CM_CONTYPE_AUTO				0

#define MaxStringLen	20
//#define MAX_APN_INFO_LEN 54

/*---------------------------------------------------------------------------
  Macro for Judge lower or upper character.
---------------------------------------------------------------------------*/
#define ISLOWER(c)  (('a' <= c && c <= 'z') ? TRUE : FALSE)

/*---------------------------------------------------------------------------
  Macro for switch lower character to upper character.
---------------------------------------------------------------------------*/
#define TOUPPER(c)   (ISLOWER(c) ? c - ('a' - 'A') : c)

/*---------------------------------------------------------------------------
  Macro for DIAL Message debug.
---------------------------------------------------------------------------*/

#define dialmsg(fmt,args...) CPUartLogPrintf(fmt, ##args)
#define dialmsg_long(fmt,args...) CPUartLogPrintf(fmt, ##args)
#define dialmsg_level1(fmt,args...)

#define duster_free(p) free(p)
#define duster_malloc(p) malloc(p)
#define Duster_module_Printf CPUartLogPrintf
#define Dial_module_Printf CPUartLogPrintf

#define MaxMtPDPNum   10
#define MaxMoPDPNum   10

/*added by shoujunl for MEP*/
#define	MRV_MEP_ENABLED	 /*MEP Enabled*/

#ifdef WIFI_FUNCTION_SUPPORT
#define WIFI_SUPPORT
#endif

typedef void (*nwst_callback_fn)(int event);

/* --------------------------------------------------------------------
** Type declarations for DIALER TASK module.
** -------------------------------------------------------------------
*/
typedef enum E_Dialer_Cmd_Msg
{
    DIALER_AT_COMMAND = 0,
    DIALER_AT_IND,
    DIALER_START_DIAL,
    DIALER_DISCONNECT,
    DIALER_CGREG_FAIL_DIAL,
    DIALER_TEST
} _Dialer_Cmd_Msg;

typedef enum E_Dialer_Code
{
    CPIN_READY= 0,
    CPIN_NEED_PIN,
    CPIN_NEED_PUK,
    CPIN_PUK_EXHAUSTED,
    CPIN_UNKNOWN_ERROR,
    CGREG_REG,
    CGREG_NREG,
    CGREG_ERROR,
    CGDCONT_OK,
    CGDCONT_ERROR,
    CGACT_DIAL_CON,
    CGACT_DIAL_ERROR,
    CGDCONT_QUERY_OK,
    CGDCONT_QUERY_ERROR,
	/*added for MEP*/
	CPIN_PN_PIN = 0x30, /*Network Personalization*/
	CPIN_PN_PUK,
	CPIN_PU_PIN,		 /*Network SubSet Personalization*/
	CPIN_PU_PUK,
	CPIN_PP_PIN,		 /*Service Provider Personlization*/
	CPIN_PP_PUK,
	CPIN_PC_PIN,		 /*Corporate Personlization*/
	CPIN_PC_PUK,
	CPIN_SIM_PIN,		 /*SIM/USIM Personliazation*/
	CPIN_SIM_PUK
} _Dialer_Code ;

typedef enum E_Dialing_Code
{
	RD_INIT_STATE,
	RD_START_DIAL,
	RD_START_DISCON,
    RD_DIAL_OK,
    RD_DIAL_ERROR
} _Dialing_Code ;

typedef struct S_statisticsData
{
    /*list counter just record from power on*/
    /*no need to write to flash, only write used data*/
    unsigned long TX_Bytes; /*unit: MBytes, naming TX_Bytes not TX_MBytes to keep pre-compatible*/
	unsigned long TX_Bytes_Calc; /*unit: Byte*/
    unsigned long RX_Bytes; /*unit: MBytes*/
	unsigned long RX_Bytes_Calc; /*unit: Byte*/
	unsigned long error_Bytes; /*unit: MBytes, naming error_Bytes for rx to keep pre-compatible*/   
	unsigned long error_Bytes_Calc; /*unit: Byte, naming error_Bytes_Calc for rx to keep pre-compatible*/    
    unsigned long tx_error_Bytes;
    unsigned long tx_error_MBytes;    
    unsigned long TX_Packets;
    unsigned long RX_Packets;
	unsigned long error_Packets; /*naming error_Pakcets for rx to keep pre-compatible*/
    unsigned long tx_error_Packets;

    /*cutoff, remind, callback, will reconfig after power on*/
    /*so no need to write to flash also.*/
	unsigned long Cutoff_MBytes;	/*for set cutoff flow data*/
	unsigned long Reminder_MBytes;	/*for set remind flow data*/
	nwst_callback_fn callback;

	/*for statisticsData per Month, start*/
	unsigned long adjusted_value;	/*set accurate value from WebUI,adjusted_value using MBytes*/
	UINT16	 monthly_year;
	UINT8	monthly_month;
	UINT8	monthly_day;
	/*for statisticsData per Month, end*/
} statisticsData;

typedef struct S_GlobalParameter_Duster2Dialer
{
	UINT32 DHCP_lease_time;
	UINT32 rssi;
	char DHCP_IP_first_section;
	char DHCP_IP_second_section;
	char DHCP_IP_third_section;
	char DHCP_IP_fourth_section;
	char DHCP_Start;
	char DHCP_End;
	char connectmode;
	char apn_mode;
	#ifdef WEBUI_SUPPORT
	char name[MaxStringLen];
	char password[MaxStringLen];
	char APN[MaxStringLen];
	char roaming;
	char ActiveWlanClientNumber;
	char gateway[18];
	char dns1[18];
	char dns2[18];
	char proto[10];
	char connect_disconnect[10];
	#endif
	#ifdef WIFI_SUPPORT
	char sec_mode[10];
	#endif
	DeviceNode PC;
	#ifdef WIFI_SUPPORT
	DeviceNode WlanClient[10];
	#endif
	#ifdef WEBUI_SUPPORT
	DeviceNode FixedIPDevice[30];
	char Lan_IP[12];
	#endif
	#ifdef WIFI_ENABLE
	char channel;
	#endif
	char dusterReadyFlag;
	char dhcpReadyFlag;
	char dhcpStatus;
	char NW_mode[2];
	char prefer_mode[2];
	#ifdef WIFI_SUPPORT
	char ssid[32];
	char secmode[20];
	char encmode[20];
	char netMode[4];
	char sub_mode[20];
	char sub_key[20];
	char sub_auth[12];
	#endif
}GlobalParameter_Duster2Dialer;

typedef struct S_GlobalParameter_Dialer2Duster
{
    statisticsData USB;
    statisticsData Cellular;
    statisticsData CellularSim1;
    #ifdef WEBUI_SUPPORT
    statisticsData Cellular_reset;
    statisticsData Cellular_single;
    statisticsData Cellular_history;
    statisticsData Cellular_month_history;
    statisticsData Cellular_month_reset;
    statisticsData Cellular_period_history;
    statisticsData Cellular_period_reset;
    statisticsData Cellular_unlimit_history;
    statisticsData Cellular_unlimit_reset;
    statisticsData Cellular_default_history;
    statisticsData Cellular_default_reset;
    #endif
    statisticsData WLAN;
     #ifdef WIFI_SUPPORT
    s_WlanStatistics *WlanStatList;
    #endif
    unsigned int ReturnCode;
	char pin_status;
	char sim_status;
	char pin_attempts;
	char puk_attempts;
	#ifdef WEBUI_SUPPORT
	UINT32 IPV4_remote_IP;
	UINT32 IPV4_DNS1;
	UINT32 IPV4_DNS2;
	UINT32 IPV4_GW;
	UINT32 IPV4_MASK;
	#endif
	UINT32 connected_tick;
	#ifdef WEBUI_SUPPORT
	UINT32 connected_tick1;
	UINT32 connected_tick2;
	UINT32 connected_tick3;
	UINT32 connected_tick4;
	UINT32 connected_tick5;
	UINT32 connected_tick6;
	#endif
	UINT32 total_connected_tick;
	#ifdef WEBUI_SUPPORT
	UINT32 total_connected_tick1;
	UINT32 total_connected_tick2;
	UINT32 total_connected_tick3;
	UINT32 total_connected_tick4;
	UINT32 total_connected_tick5;
	UINT32 total_connected_tick6;
	UINT32 unreadLocalSMSNum;
	char Oper[32];
	#endif
	#ifdef WIFI_SUPPORT
	UINT32 wifi_start_tim;
	UINT32 wifi_end_tim;
	#endif
	char  dialerReady;
	char LinkStatus[CI_PS_MAX_MO_AND_MT_PDP_CTX_NUM+1];
	#ifdef WEBUI_SUPPORT
	char SMSlist_new_Flag;
	char LocalSMSFull;
	char GetSIMInfoDone;
	char CheckSIMDone;
	/*add new items*/
	
    statisticsData Cellular_daily_history;
	statisticsData Cellular_daily_reset;
	#endif
}GlobalParameter_Dialer2Duster;

typedef struct S_DialCmdMessage
{
    _Dialer_Cmd_Msg MsgId; //enum type
    void *MsgData;
} DialCmdMessage;

typedef struct S_DialRespMessage
{
    void *MsgData;
} DialRespMessage ;


typedef enum{
	//for webUI message
	ConnectionCreate = 0,
	ConnectionModify,
	ConnectionDestroy,
	ConnectionDeleteCtx,
	ConnectionClearEitf,
	UpdateTft,
	//for dialer message
	DialerStart,
	DialerStop,
	DialerSetTFT,
	DialerResponse,
	DialerDeleteContext,
	DialerPSServiceOption,
	DialerSetRadioState,
	DialerSetImsState,
	DialerClearEITFlag,
	//for AT command server
	AtcmdNOK,
	AtcmdCgdcont,
	DialerTFTResponse,
	DialerTFTResponse_1, /* TFT response for SIM1*/
	DialerMTStart,
	RoamStart,
	NWReady,
	RAUChange,
	AtcmdSimRemoved,
	//for Reject cause network
	DialerReStart,
	setInitialAttach,
	setupDataCall,
	deactiveDataCall,
	MTUChange,
	CMMsgMaxType = 0x1FFFFFF
}CMMsgType;

typedef struct{
	CMMsgType MsgID;
	void *MsgData;
	unsigned int tick;
}CMMsg;

#if 0
typedef struct {
        int  IPAddr;
        int  PrimaryDNS;
        int  SecondaryDNS;
        int  GateWay;
        int  Mask;
} Ipv4Info;

typedef struct {
        int  IPV6Addr[4];
        int  PrimaryDNS[4];
        int  SecondaryDNS[4];
        int  GateWay[4];
        int  Mask[4];
} Ipv6Info;

typedef struct {
        unsigned int connected_tick;
        unsigned int total_connected_tick;
        char  PDP_Type;                    /** 1-Primary, 0--secondary */
        char  IP_Type;                     /** 0-IPV4V6; 1---IPV4; 2-IPV6 */
        char  IsDefaultConnection;
        char  PrimaryCID;
        char  SecondaryCID;
        char  QCI;
		char  BearerID;
		char  iptype_dialer;	/*modified by dialer 0 --Disable Auto APN; 1 -- IPV4V6; 2 -- IPV4; 3 -- IPV6*/
		char  iptype_to_dialer; /*tell dialer the iptype to dial 0 --Disable Auto APN; 1 -- IPV4V6; 2 -- IPV4; 3 -- IPV6*/
        char  APN[MAX_APN_INFO_LEN + 1];
		char  LteAPN[MAX_APN_INFO_LEN + 1];
		/*added by shoujunl 131127 start*/
		char  Usr2G3G[MAX_APN_INFO_LEN + 1];
		char  PASWD2G3G[MAX_APN_INFO_LEN + 1];
		char  Authtype2G3G[20];
		char  Usr4G[MAX_APN_INFO_LEN + 1];
		char  PASWD4G[MAX_APN_INFO_LEN + 1];
		char  Authtype4G[20];
		/*added by shoujunl 131127 end*/
		int  redial_times;
		int  waiting_tickets;
} PdpInfo;


typedef struct {
        int  direction;
		Bool directionPresent;
        NumericRange localPortRange;
        NumericRange RemotePortRange;
        Bool  localPortRangePresent;
        Bool  RemotePortRangePresent;
        char  SecondaryCID;
        char  PfIdx;        // Packet Filter ID
        char  EpIdx;       // Evaluation Precedence Index
        Bool  pnNextHdrPresent;
        char  pnNextHdr;      // Protocol Number (IPV4) or Next Header(IPV6)
        Bool  TosPresent;
        char  Tos;         // Type of Service
        char  TosMask;         // Type of Service Mask
        Bool  SpiPresent;
        Bool  FlowLablePresent;
        int   SPI;
        int   FlowLable;
		char  IPType;
        char  RemoteAddress[16];
		Bool  RemoteAddressPresent;
        char  SubnetMask[16];
		Bool  SubnetMaskPresent;
		unsigned int	maxULRate;			/**<UL_MBR Max Bit Rate, Uplink, in kbps */
		unsigned int	maxDLRate;			/**<DL_MBR Max Bit Rate, Downlink, in kbps (MAX 256000kbps) */
		unsigned int	guaranteedULRate;	/**<UL_GBR Guaranteed Bit Rate, Uplink, in kbps */
		unsigned int	guaranteedDLRate;	/**<DL_GBR Guaranteed Bit Rate, Downlink, in kbps */
		void *next;
} PacketFilterInfo;

//used by TFT module
typedef struct {
		char bearer_id;
		PacketFilterInfo * packetinfo;
        void  *next;
} TftInfoList;

//used by webUI module
typedef struct {
	char connection_num;
	PacketFilterInfo * packetinfo;
	void * next;
} TftInfoList_webUI;

typedef struct {
        void  *next;
        PacketFilterInfo PF;
} TftList;

typedef struct {
        Ipv4Info *IPV4;
        Ipv6Info *IPV6;
        PacketFilterInfo *PF;
        PdpInfo Pdp_Info;
        char  ConnectionNumber;
} PdpContext;

typedef struct {
        void *next;
        PdpContext PDP;
} PdpContextList;
#endif

typedef enum {
        DialerOK = 0,
        Dialerinit,
        DialerDisconnectOK,
        DialerDisconnectFail,
        DialerCgregFailure,
        DialerCgdcontFailure,
        DialerCgdscontFailure,
        DialerCgeqosFailure,
        DialerCgtftFailure,
        DialerCgdataFailure,
        DialerCgdcontQueryFailure = 10,
        DialerCgdscontQueryFailure,
        DialerCgtftQueryFailure,
        DialerDefaultConnectionPresent,
        DialerNWReject,
        DialerNWRejectOnlyV4,
        DialerNWRejectOnlyV6,
        DialerNWRejectSingleV4V6,
        DialerAUTHREQFaile,
        DialerIPV4V6Stop,
        DialerNWRejectAuthFail = 20,
        DialerNWRejectNotSubscribed,
        //use to handle cause 17
        DialerNWRejectNotRedial,
        DialerRadioPowerOff,
        DialerPSOptionOK,
        DialerPSOptionNOK,
        DialerSetRadioStateOK,
        DialerSetRadioStateNOK,
        DialerSetImsStateOK,
        DialerSetImsStateNOK,
        DialerNWSearching,
        DialerMsgMax
} DialerReasonCode;

typedef struct {
        unsigned int connected_tick;
        unsigned int total_connected_tick;
        char  IsMTPdp;
        char  CID;
        char  ConnectionNumber;
        char  IP_Type;
        int  IPAddr;
        int  IPV6Addr[4];
	    TIME dial_start_time;
} PdpLinkTimeInfo;

/*
typedef enum {
        DialerStart = 0,
        DialerStop,
        DialerUpdateTFT,
        DialerResponce,
        DialerMsgNum
} DialerMessageType;

typedef enum {
        AtcmdNOK = DialerMsgNum,
        AtcmdCgdcont,
        AtcmdMsgNum
} AtcmdsrvMessageType;

typedef enum {
        DusterCelluarAvailble = AtcmdMsgNum,
        DusterManualConnect,
        DusterManualDisconnect,
        DusterUpdateTft,
        DusterMsgNum
} DusterMessageType;
*/

/* IPV4 content struct */
typedef struct S_IPV4_DIAL_CONTENT
{
	UINT32 IPV4_GW;
	UINT32 IPV4_IP;
	UINT32 IPV4_DNS1;
	UINT32 IPV4_DNS2;
	UINT32 IPV4_MASK;
}IPV4_Dial_Content;

typedef enum {
    G_primary_PDP= 0,
    G_secondary_PDP,
    LTE_primary_PDP,
    LTE_secondary_PDP
} DialerATType;


typedef struct {
    CMMsgType MessageID;
    void *MessageData;
} DialerRequestMessage;

typedef struct {
    	DialerReasonCode RC;
        void *MessageData;
} DialerResponseData;

typedef struct {
    CMMsgType MessageID;
    void *MessageData;
} AtcmdsrvIndMessage;

typedef enum {
	NW_STATUS_23G = 0,
	NW_STATUS_LTE,
	NW_STATUS_NOT_REG = 3,
	NW_STATUS_CS_ONLY = 4
} DialerNWStatus;

enum 
{
    DIALER_PDP_DISMATCH = 0,
    DIALER_PDP_MATCHED = 1,
    DIALER_PDP_MATCHED_IMS = 2,
    DIALER_PDP_MATCHED_NO_DNS = 3,
    DIALER_PDP_NOT_ALLOW = 4
};

#if 0
typedef struct _NumericRange
{
	int min;     /* lower limit */
	int max;     /* upper limit */
}NumericRange;

typedef struct {
    int  direction;
    Bool directionPresent;
    NumericRange localPortRange;
    NumericRange RemotePortRange;
    Bool  localPortRangePresent;
    Bool  RemotePortRangePresent;
    char  SecondaryCID;
    char  PfIdx;        // Packet Filter ID
    char  EpIdx;       // Evaluation Precedence Index
    Bool  pnNextHdrPresent;
    char  pnNextHdr;      // Protocol Number (IPV4) or Next Header(IPV6)
    Bool  TosPresent;
    char  Tos;         // Type of Service
    char  TosMask;         // Type of Service Mask
    Bool  SpiPresent;
    Bool  FlowLablePresent;
    int   SPI;
    int   FlowLable;
    char  IPType;
    char  RemoteAddress[16];
    Bool  RemoteAddressPresent;
    char  SubnetMask[16];
    Bool  SubnetMaskPresent;
    unsigned int	maxULRate;			/**<UL_MBR Max Bit Rate, Uplink, in kbps */
    unsigned int	maxDLRate;			/**<DL_MBR Max Bit Rate, Downlink, in kbps (MAX 256000kbps) */
    unsigned int	guaranteedULRate;	/**<UL_GBR Guaranteed Bit Rate, Uplink, in kbps */
    unsigned int	guaranteedDLRate;	/**<DL_GBR Guaranteed Bit Rate, Downlink, in kbps */
    void *next;
} PacketFilterInfo;
#endif
/*
typedef struct {
        CMMsgType MessageID;
        void *MessageData;
} DusterRequestMessage;
*/
extern char *strcat(char *dest, const char *src);
extern BOOL IsGsmRatForMIFI(void);
int send_AT_wait_resp(int sATPInd,char *in_str, int timeout, char *ok_fmt, int ok_flag,char *err_fmt, char *out_str);
void dialer_task_init(void);
void set_dialer_connmode(char value);
char get_dialer_linkstas(char cid);
void set_in_dialer_mt_proc(int flag);
int get_in_dialer_mt_proc(void);
UINT32 getPdpTimeTotalTicks(UINT8 cid);

/*
UINT32 get_currentconn_tick();
UINT32 get_totalconn_tick();
*/
int get_auto_apn_iptype(int simID);
int get_auto_apn_mtu(int simID);
char *get_auto_apn_name(int simID);
void updatePdpTimeNok(UINT8 cid);

/* 
*@flag  1: allow deactive default pdp if exist more than 1 pdp 
*       0: not allow to deactive default pdp
**/
void dialer_set_deact_default_pdp_flag(BOOL flag);
BOOL dialer_get_dns_present_flag(void);
void dialer_set_dns_present_flag(BOOL flag);

/* 
*@force  1: foce dialer switchoff attach if in 2/3G
**/
void set_force_apn_reattach(BOOL force);
void setInvalidSimEMMCause(UINT32 atHandle, UINT32 cause);

#endif // PCAC_AT_CLIENT

#endif
