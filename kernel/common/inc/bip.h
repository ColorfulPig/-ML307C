#ifndef BIP_H
#define BIP_H

#include "sockets.h"

typedef unsigned long UINT32;
typedef unsigned short UINT16;
typedef unsigned char UINT8;
typedef unsigned char	BOOL;

#define BIP_PLATFORM_CRANE
#define OUT
#define IN

#define CR_MASK  0x7f
#define PROACTIVE_LENGTH_OFFSET  1
#define PROACTIVE_COMMAND_DETAILS_OFFSET  2
#define DEVICE_IDENTIFIER_BYTE_NUM 4
#define COMMAND_DETAILS_BYTE_NUM   5

#define TOA_INTERNATIONAL 0x91
#define TOA_UNKNOW        0x81

#define BIP_STK_CMD_PROACTIVE   11
#define BIP_STK_CMD_TERMINAL_RSP_IND 25

#define STK_MAX_TEXT_STR_SIZE       241
#define IPv4_ADDRESS_TYPE 0x21
#define IPv6_ADDRESS_TYPE 0x57

#define IPv4_ADDRESS_TYPE_LENGTH 4
#define IPv6_ADDRESS_TYPE_LENGTH 16

#define STK_MAX_SIM			2

#define STK_MAX_CHANNEL_NUM 2 // Must match Terminal Profile

#define STK_TCP_CHANNEL_BUFFER_SIZE 16384 // reserve 16k as Tx/Rx per
										   // Buffer for TCP
#define STK_UDP_CHANNEL_BUFFER_SIZE 1500 // Restrict UDP packet size to
										  // 1500 bytes due to MTU
										  // restriction


#define STK_BIP_PDP_LINK_ESTABLISHED (1 << 15)	/* Channel Status TS 11.14 12.56
												  *Coding of byte 3:
												  *bit 1 to 3:	Channel identifier : 1..7
												  *			Channel identifier 0 means "No channel available"
												  *bit 4 to 7:		RFU
												  *bit 8:	0 = Link not established or PDP context not activated
												  *		1 = Link established or PDP context activated */

#define STK_BIP_TCP_SERVER_LISTEN_STATE (1 << 14)
#define STK_BIP_TCP_SERVER_ACCEPTED_STATE (1 << 15)

#define STK_INVALID_PDP_CID 0xFF

#define STK_BIP_PDP_LINK_DROPED 5

#define	STK_BIP_TR_VOICE_CALL_BUSY 2

#define STK_DEFAULT_APN "CMNET"

#define STK_PDP_CONNECTED TRUE
#define STK_PDP_DISCONNECTED FALSE

#define STK_COMPREHENSION_TLV_BIT (1 << 7)

//#define NULL 0
#ifdef WIN32
#include "WinSock2.h"
#endif

typedef int SOCKET_DESC;

#define UNUSEDPARAM(param)
#define MTIL_MAX_DATA_STR     100
#define MTIL_MAX_IP_V4_SIZE		4
#define MTIL_MAX_IP_V6_SIZE		4
#define MTIL_MAX_PDP_CTX        15
#define BIP_START_CID			9

enum BIP_PDP_TYPE {
	BIP_IPTYPE_V4   = 1,
	BIP_IPTYPE_V6   = 2,
	BIP_IPTYPE_V4V6 = 3,
};

typedef struct _MTIL_PDP_ADDR {
	unsigned long	pdp_addr_type;			/* Holds PDP address type */
	unsigned long	ipv4_addr;			/* IPv4 address */
	unsigned long	ipv6_addr[4];			/* IPv6 address */
} MTIL_PDP_ADDR, *P_MTIL_PDP_ADDR;

typedef struct _MTIL_CGACT_INFO {
	unsigned long	state;			/* See Ref [3] sec 10.1.10<state> */
	unsigned long	cid;			/* See Ref [3] sec 10.1.10<cid> */
} MTIL_CGACT_INFO, *P_MTIL_CGACT_INFO;

typedef struct _MTIL_CGACT_STATUS_MSG {
	unsigned long	ret_code;			/* See Ref [3] sec 10.1.10 */
	unsigned long	num_cids;
	MTIL_CGACT_INFO	cid_info[MTIL_MAX_PDP_CTX];			/* See Ref [3] sec 10.1.10<cid>, <state> */
} MTIL_CGACT_STATUS_MSG, *P_MTIL_CGACT_STATUS_MSG;



typedef UINT32 MTILRequestHandle;

#define RESP(REQ_HANDLE, STRING, PREF, POSTF) printf("%s", STRING)
#define OSA_Win32TaskCreate(p1,p2,p3,p4,p5) (CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)p4,(LPVOID)p5,0,NULL)==NULL ? 1 : 0 )

typedef void (*STK_SendTRFormat)(char *pRespBuf, void *pparams);

typedef void (*STK_BIPListThread) (void *pchannel);

typedef void (*STK_SendEnvelopeFormat)(char *pRespBuf, void *pparams);

typedef enum EventList {
	/** MT Call */
	MT_CALL = 0x00,

	/** Call connected */
	CALL_CONNECTED = 0x01,

	/** Call disconnected */
	CALL_DISCONNECTED = 0x02,

	/** Location status */
	LOCATION_SATATUS = 0x03,

	/** User activity */
	USER_ACTIVITY = 0x04,

	/** Idle screen availabe */
	IDLE_SCREEN_AVAILABLE = 0x05,

	/** Card reader status  = if class "a" is supported) */
	CARD_READER_STATUS = 0x06,

	/** Language selection */
	LANGUAGE_SELECTION = 0x07,

	/** Brower Termination  = if class "c" is supported) */
	BROWER_TERMINATION = 0x08,

	/** Data available  = if class "e" is supported) */
	DATA_ABAILABLE = 0x09,

	/** Channel status  = if class "e" is supported) */
	CHANNEL_STATUS = 0x0A,

	//TS 102.223 section 8.25
	SINGLE_ACCESS_TECHNOLOGY_CHANGE = 0x0B,
	DISPLAY_PARAMETERS_CHANGED = 0x0C,
	LOCAL_CONNECTION = 0x0D,
	NETWORK_SEARCH_MODE_CHANGE = 0x0E,
	BROWSING_STATUS = 0x0F,
	FRAMES_INFORMATION_CHANGE = 0x10,
	//RESERVED_FOR_I_WLAN_ACCESS_STATUS = 0x11),
	//RESERVED_FOR_NETWORK_REJECTION = 0x12),
	HCI_CONNECTIVITY_EVENT = 0x13,
	MULTIPLE_ACCESS_TECHNOLOGY_CHANGE = 0x14,
// fix STK issue with SET_UP_EVENT_LIST
		//[TelephonyFeature.CONFIG_STK_MVL_FEATURE)
	REMOVE_EVENT = 0x15,
	EVENT_LIST_MAX_LENGTH,
} EVENT_LIST_STRUCT;

typedef enum _RESULT_CODE {

	/*
	 * Results '0X' and '1X' indicate that the command has been performed.
	 */

	/** Command performed successfully */
	OK = 0x00,

	/** Command performed with partial comprehension */
	PRFRMD_WITH_PARTIAL_COMPREHENSION = 0x01,

	/** Command performed, with missing information */
	PRFRMD_WITH_MISSING_INFO = 0x02,

	/** REFRESH performed with additional EFs read */
	PRFRMD_WITH_ADDITIONAL_EFS_READ = 0x03,

	/**
	 * Command performed successfully, but requested icon could not be
	 * displayed
	 */
	PRFRMD_ICON_NOT_DISPLAYED = 0x04,

	/** Command performed, but modified by call control by NAA */
	PRFRMD_MODIFIED_BY_NAA = 0x05,

	/** Command performed successfully, limited service */
	PRFRMD_LIMITED_SERVICE = 0x06,

	/** Command performed with modification */
	PRFRMD_WITH_MODIFICATION = 0x07,

	/** REFRESH performed but indicated NAA was not active */
	PRFRMD_NAA_NOT_ACTIVE = 0x08,

	/** Command performed successfully, tone not played */
	PRFRMD_TONE_NOT_PLAYED = 0x09,

	/** Proactive UICC session terminated by the user */
	UICC_SESSION_TERM_BY_USER = 0x10,

	/** Backward move in the proactive UICC session requested by the user */
	BACKWARD_MOVE_BY_USER = 0x11,

	/** No response from user */
	NO_RESPONSE_FROM_USER = 0x12,

	/** Help information required by the user */
	HELP_INFO_REQUIRED = 0x13,

	/** USSD or SS transaction terminated by the user */
	USSD_SS_SESSION_TERM_BY_USER = 0x14,


	/*
	 * Results '2X' indicate to the UICC that it may be worth re-trying the
	 * command at a later opportunity.
	 */

	/** Terminal currently unable to process command */
	TERMINAL_CRNTLY_UNABLE_TO_PROCESS = 0x20,

	/** Network currently unable to process command */
	NETWORK_CRNTLY_UNABLE_TO_PROCESS = 0x21,

	/** User did not accept the proactive command */
	USER_NOT_ACCEPT = 0x22,

	/** User cleared down call before connection or network release */
	USER_CLEAR_DOWN_CALL = 0x23,

	/** Action in contradiction with the current timer state */
	CONTRADICTION_WITH_TIMER = 0x24,

	/** Interaction with call control by NAA, temporary problem */
	NAA_CALL_CONTROL_TEMPORARY = 0x25,

	/** Launch browser generic error code */
	LAUNCH_BROWSER_ERROR = 0x26,

	/** MMS temporary problem. */
	MMS_TEMPORARY = 0x27,


	/*
	 * Results '3X' indicate that it is not worth the UICC re-trying with an
	 * identical command, as it will only get the same response. However, the
	 * decision to retry lies with the application.
	 */

	/** Command beyond terminal's capabilities */
	BEYOND_TERMINAL_CAPABILITY = 0x30,

	/** Command type not understood by terminal */
	CMD_TYPE_NOT_UNDERSTOOD = 0x31,

	/** Command data not understood by terminal */
	CMD_DATA_NOT_UNDERSTOOD = 0x32,

	/** Command number not known by terminal */
	CMD_NUM_NOT_KNOWN = 0x33,

	/** SS Return Error */
	SS_RETURN_ERROR = 0x34,

	/** SMS RP-ERROR */
	SMS_RP_ERROR = 0x35,

	/** Error, required values are missing */
	REQUIRED_VALUES_MISSING = 0x36,

	/** USSD Return Error */
	USSD_RETURN_ERROR = 0x37,

	/** MultipleCard commands error */
	MULTI_CARDS_CMD_ERROR = 0x38,

	/**
	 * Interaction with call control by USIM or MO short message control by
	 * USIM, permanent problem
	 */
	USIM_CALL_CONTROL_PERMANENT = 0x39,

	/** Bearer Independent Protocol error */
	BIP_ERROR = 0x3a,

	/** Access Technology unable to process command */
	ACCESS_TECH_UNABLE_TO_PROCESS = 0x3b,

	/** Frames error */
	FRAMES_ERROR = 0x3c,

	/** MMS Error */
	MMS_ERROR = 0x3d,
} RESULT_CODE;

typedef struct _TLV
{
	UINT8 nTag;
	UINT32 nLength;
	int cr; // Comprehension required flag
	void *pValue;
} TLV;

typedef enum _BER_TAG {
	BER_UNKNOWN_TAG             = 0x00,
	BER_PROACTIVE_COMMAND_TAG   = 0xd0,
	BER_MENU_SELECTION_TAG      = 0xd3,
	BER_EVENT_DOWNLOAD_TAG      = 0xd6,
} BER_TAG;

typedef enum _COMMANDTYPE {
	DISPLAY_TEXT = 0x21,
	GET_INKEY = 0x22,
	GET_INPUT = 0x23,
	LAUNCH_BROWSER = 0x15,
	PLAY_TONE = 0x20,
	REFRESH = 0x01,
	SELECT_ITEM = 0x24,
	SEND_SS = 0x11,
	SEND_USSD = 0x12,
	SEND_SMS = 0x13,
	SEND_DTMF = 0x14,
	SET_UP_EVENT_LIST = 0x05,
	SET_UP_IDLE_MODE_TEXT = 0x28,
	SET_UP_MENU = 0x25,
	SET_UP_CALL = 0x10,
	PROVIDE_LOCAL_INFORMATION = 0x26,
	LANGUAGE_NOTIFICATION = 0x35,
	OPEN_CHANNEL = 0x40,
	CLOSE_CHANNEL= 0x41,
	RECEIVE_DATA = 0x42,
	SEND_DATA = 0x43,
	GET_CHANNEL_STATUS = 0x44,
} COMMANDTYPE;

typedef enum _COMPREHENSION_TLV_TAG {
	COMMAND_DETAILS = 0x01,
	DEVICE_IDENTITIES = 0x02,
	RESULT = 0x03,
	DURATION_TAG = 0x04,
	ALPHA_ID = 0x05,
	ADRESS_TAG = 0x06,
	CAPABILITY_CONFIG_PARAM_TAG = 0x07,
	SUB_ADDRESS_TAG = 0x08,
	SS_STRING = 0x09,
	USSD_STRING = 0x0a,
	SMS_TPDU_TAG = 0x0b,
	TEXT_STRING = 0x0d,
	TONE_TAG = 0x0e,
	ITEM = 0x0f,
	ITEM_ID = 0x10,
	RESPONSE_LENGTH = 0x11,
	FILE_LIST = 0x12,
	HELP_REQUEST = 0x15,
	DEFAULT_TEXT = 0x17,
	NEXT_ACTION_IND = 0x18,
	EVENT_LIST_TAG = 0x19,
	ICON_ID = 0x1e,
	ITEM_ICON_ID_LIST = 0x1f,
	IMMEDIATE_RESPONSE = 0x2b,
	DTMF_STRING_TAG = 0x2c,
	LANGUAGE_TAG = 0x2d,
	BROWSER_IDENTITY_TAG = 0x30,
	URL_TAG = 0x31,
	BROWSER_TERMINATION_CAUSE = 0x34,
	BEARER_DESC_TAG = 0x35,
	CHANNEL_DATA_TAG = 0x36,
	CHANNEL_DATA_LENGTH_TAG = 0x37,
	CHANNEL_STATUS_TAG = 0x38,
	BUFFER_SIZE_TAG = 0x39,
	IF_TRANS_LEVEL_TAG = 0x3c,
	OTHER_ADDRESS_TAG = 0x3e,
	NETWORK_ACCESS_NAME_TAG = 0x47,
	TEXT_ATTRIBUTE = 0x50,
	//3GPP 11.14
	LOCATION_INFORMATION = 0x13,
	IMEI = 0x14,
	NETWORK_MEASUREMENT_RESULT = 0x16,
	DEFAULT_TEXT_TAG = 0x17,
	DATA_TIME_TIME_ZONE = 0x26,
	TIMING_ADVANCE = 0x2E,
	TRANSACTION_ID = 0x9C,
	} COMPREHENSION_TLV_TAG;

typedef enum _StkTransportProtocol {
	STK_TP_RESERVED, /*TP - transport protocol*/
	STK_TP_UDP_CLIENT_REMOTE,
	STK_TP_TCP_CLIENT_REMOTE,
	STK_TP_TCP_SERVER,
	STK_TP_UDP_CLIENT_LOCAL,
	STK_TP_TCP_CLIENT_LOCAL,
	STK_TP_END,
	STK_TP_TRANSPORT_PROTOCOL_MAX = 0xFF,
} StkTransportProtocol;

typedef enum _STKBearerType {
	STK_BT_MOBILE_CSD = 0x01, /*BT - bearer type*/
	STK_BT_MOBILE_PS,
	STK_BT_DEFAULT_BEARER,
	STK_BT_LOCAL_LINK,
	STK_BT_BLUETOOTH,
	STK_BT_IRDA,
	STK_BT_RS232,
	STK_BT_MOBILE_PS_EXTENDED_QOS = 0x09,
	STK_BT_I_WLAN = 0x0A,
	STK_BT_MOBILE_EUTRAN,
	STK_BT_USB = 0x10,
} STKBearerType;

typedef enum _STKBearerParameters {
	STK_BP_PRECEDENCE_CLASS, /*BP - bearer parameters*/
	STK_BP_DELAY_CLASS,
	STK_BP_RELIABILITY_CLASS,
	STK_BP_PEAK_THROUGHPUT_CLASS,
	STK_BP_MEAN_THROUGHPUT_CLASS,
	STK_BP_PCKT_DATA_PRTCL_TYPE,
	STK_BP_LENGTH,
	STK_BP_MAX = 0xFF,
} STKBearerParameters;

typedef enum _STKBearerLTEParameters {
	STK_BP_QCI, //*QCI
	STK_BP_MAX_BIT_RATE_UPLINK,//MAX Up link
	STK_BP_MAX_BIT_RATE_DOWNLINK,//Max downlink
	STK_BP_GUAR_BIT_RATE_UPLINK,//G Up link
	STK_BP_GUAR_BIT_RATE_DOWNLINK,//G downlink
	STK_BP_MAX_BIT_RATE_EXT_UPLINK,//M Ext Up LINK
	STK_BP_MAX_BIT_RATE_EXT_DOWNLINK,//M Ext Down LINK
	STK_BP_GUAR_BIT_RATE_EXT_UPLINK,//M G Ext Up LINK
	STK_BP_GUAR_BIT_RATE_EXT_DOWNLINK,
	STK_BP_LTE_PCKT_DATA_PRTCL_TYPE,
	STK_BP_LTE_LENGTH,
	STK_BP_LTE_MAX = 0xFF,
} STKBearerLTEParameters;

typedef enum _STK_BIP_TR_ADDITIONAL_VALUES {
	STK_BIP_TR_ADD_VAL_NO_CAUSE,           //No specific cause can be given;
	STK_BIP_TR_ADD_VAL_NO_CHNNL_AVAIL,     //No channel available;
	STK_BIP_TR_ADD_VAL_CHNNL_CLSD,         // Channel closed;
	STK_BIP_TR_ADD_VAL_CHNNL_ID_NOT_VALID, //Channel identifier not valid;
	STK_BIP_TR_ADD_VAL_BUF_SZ_NOT_AVAIL,   //Requested buffer size not available;
	STK_BIP_TR_ADD_VAL_SECUR_ERROR,        // Security error (unsuccessful authentication);
	STK_BIP_TR_ADD_VAL_ITL_NOT_AVAIL,      //Requested SIM/ME interface transport level not available.
	STK_BIP_TR_ADD_VAL_NO_VALUE = 0xFF,
}STK_BIP_TR_ADDITIONAL_VALUES;

typedef enum _STK_BIP_PDP_CMND_TYPE_ACIVATION {
	STK_BIP_PDP_CGACT,
	STK_BIP_PDP_CGDATA,
	STK_BIP_PDP_CMND_MAX,
} STK_BIP_PDP_CMND_TYPE_ACIVATION;


typedef struct _DEVICE_IDENTITIES_STRUCT {
	UINT8 srcId;
	UINT8 destId;
} DEVICE_IDENTITIES_STRUCT;

typedef struct _COMMAND_DETAILS_STRUCT {
	int compRequired;
	int commandNumber;
	int typeOfCommand;
	int commandQualifier;
} COMMAND_DETAILS_STRUCT;

typedef struct _STK_TEXT_STRING_DATA{
	char *pText;
	int codingScheme;
	int length;
} STK_TEXT_STRING_DATA;

typedef struct _STK_BEARER_DESCRIPTION_STRUCT {
	STKBearerType type;
	UINT8 parameters[STK_BP_LTE_LENGTH];
} STK_BD;

typedef struct _STK_INTERFACE_TRANSPORT_LEVEL_STRUCT {
	UINT32 port;
	UINT8 protocol; /* Values from  StkTransportProtocol*/
} STK_ITL;

typedef struct _STK_OTHER_ADDRESS_STRUCT {
	UINT8 version; /* ipv4 ipv6 */
	UINT8 dest_ipv6addr[IPv6_ADDRESS_TYPE_LENGTH];
	UINT32 dest_ipv4addr;
} STK_OTHER_ADDRESS;

typedef struct _STK_OPEN_CHANNEL_DATA {
	STK_OTHER_ADDRESS destinationAddress;
	STK_BD bearerDescription;
	STK_ITL itl;
	UINT32 bufSize;
	STK_TEXT_STRING_DATA alpha;
	STK_TEXT_STRING_DATA networkAccessName;
	STK_TEXT_STRING_DATA userLogin;
	STK_TEXT_STRING_DATA userPassword;
	unsigned int pckgLength;
} STK_OPEN_CHANNEL_DATA, *P_STK_OPEN_CHANNEL_DATA;

typedef struct _STK_BIP_DATA_SETTINGS {
	UINT8 channel;
	UINT32 length;
	char* pdata;
} STK_BIP_DATA_SETTINGS, *P_STK_BIP_DATA_SETTINGS;

typedef struct _STK_BIP_CONNECTION_DATA {
	SOCKET_DESC sd;
	SOCKET_DESC cld;
	struct sockaddr_in addr;
    struct sockaddr_in6 v6addr;
	UINT32 bufSize;
	char *pRxBuf;
	char *pTxBuf;
	UINT32 RxPos;
	UINT32 RxLen;
	UINT32 TxPos;
	UINT8 availRx;
	BOOL bindChannel;
} STK_BIP_CONNECTION_DATA, *P_STK_BIP_CONNECTION_DATA;

typedef struct _STK_BIP_CHANNEL {
	RESULT_CODE (*bipchannelopen) (IN UINT8 ipVersion, IN UINT32 port, IN UINT32 dest_ipv4addr, IN UINT8 *pdestAddrIPv6,
			 OUT SOCKET_DESC *pSd, OUT struct sockaddr_in *pAddr);
	RESULT_CODE (*bipchannelclose) (struct _STK_BIP_CHANNEL* pBipChannel);
	RESULT_CODE (*bipchannelsend) (STK_BIP_DATA_SETTINGS Data, BOOL sendImmid);
	RESULT_CODE (*bipchannelreceive) (UINT8 len, UINT8 chnnl);
	void (*threadFunc) (void *pchannel);
	void *BipCommTaskStack;
	OSTaskRef BipCommTaskRef;
	STK_BIP_CONNECTION_DATA bipConnectData;
	STK_OPEN_CHANNEL_DATA pdpChnlData;
	UINT16 channelStatus;
	UINT8 channel;
	UINT8 cid;
	UINT8 recvthreadstate;
	UINT8 cidCreateByBip;
} STK_BIP_CHANNEL, *P_STK_BIP_CHANNEL;

typedef struct _STK_CLOSE_CHANNEL_DATA {
	STK_TEXT_STRING_DATA alpha;
	UINT8 channelToBeClosed;
} STK_CLOSE_CHANNEL_DATA, *P_STK_CLOSE_CHANNEL_DATA;

typedef struct _STK_SEND_DATA {
	STK_BIP_DATA_SETTINGS data;
	STK_TEXT_STRING_DATA alpha;
	BOOL sendImmed;
} STK_SEND_DATA, *P_STK_SEND_DATA;

typedef struct _STK_RECEIVE_DATA {
	UINT8 channel;
	STK_TEXT_STRING_DATA alpha;
	UINT8 len;
} STK_RECEIVE_DATA, *P_STK_RECEIVE_DATA;

typedef struct _STK_RECEIVE_TERM_RESP {
	UINT8 avail;
	UINT8 len; //length of pRespData
	char *pRespData;
} STK_RECEIVE_TERM_RESP, *P_STK_RECEIVE_TERM_RESP;

typedef struct _STK_GET_CHNNL_STATUS_TERM_RESP {
	UINT16 ChnnlStatusBuf[STK_MAX_CHANNEL_NUM];
	UINT8 chnnlsNum;
} STK_GET_CHNNL_STATUS_TERM_RESP, *P_STK_GET_CHNNL_STATUS_TERM_RESP;

typedef struct _STK_ENVLP_DATA_AVAIL {
	UINT16 chnnlStatus;
	UINT8 Datalen;
} STK_ENVLP_DATA_AVAIL, *P_STK_ENVLP_DATA_AVAIL;

typedef struct _STK_BIP_PDP_CID_ADDR {
	char apn[MTIL_MAX_DATA_STR];
	MTIL_PDP_ADDR pdp_addr;
	MTIL_CGACT_INFO	cid_info;
} STK_BIP_PDP_CID_ADDR, *P_STK_BIP_PDP_CID_ADDR;
typedef struct _STK_BIP_PDP_INFO {
	unsigned long	num_cids;
	STK_BIP_PDP_CID_ADDR pdp_info[MTIL_MAX_PDP_CTX+1];
} STK_BIP_PDP_INFO, *P_STK_BIP_PDP_INFO;

typedef struct _STK_SEND_TR_PARAM {
	MTILRequestHandle reqHandle;
	RESULT_CODE resValue;
	UINT32 addResValue;
	STK_SendTRFormat pSTK_SendTRFormat;
	void *param;
} STK_SEND_TR_PARAM, *P_STK_SEND_TR_PARAM;


/* RESULT_CODE decodeRelevantTlv(TLV *ptlv, int expectedTag, int isMandatory)
 *
 * Decodes Mandatory or optional TLV
 * 1. Decoding mandatory TLV
 * 		Looks for tag and length. If expected tag matches parsed function fills in pTlv struct and returns OK.
 * 		ERRORS:
 * 		1. CMN_DATA_NOT_UNDERSTOOD:
 * 			-getLength function failed.
 * 			-attempt to access out of array value.
 * 		2. REQUIRED_VALUE_MISSING:
 * 			-expected tag does not match parsed.
 * 2. Decoding optional TLV
 * 		Looks for tag and length. If expected tag matches parsed function fills in pTlv struct and returns OK.
 * 		If not pTlv.tag is set to UNEXPECTED_TAG value. smplTlvsLen ValueIndex are set to values that were before decodeRelevantTlv is called.
 * 		ERRORS:
 * 		1. CMND_DATA_NOT_UNDERSTOOD:
 * 			-getLength function failed.
 * 			-attempt to access out of array value.
 *
 * */

RESULT_CODE decodeRelevantTlv(TLV *ptlv, int expectedTag, BOOL isMandatory);

RESULT_CODE getTag(UINT8 *ptag);

RESULT_CODE getLength(OUT unsigned int *pLength);

RESULT_CODE decodeBerTlv(OUT TLV *pBerTlv);

RESULT_CODE decodeIrrelevantTlv(int tag, int isMandatory);

void returnValueIndex(TLV tlv);
UINT8 GetBIPEnableFlag(void);
RESULT_CODE decodeAlpha(unsigned char *pRawAlphaData, int length, STK_TEXT_STRING_DATA *pAlphaData);

RESULT_CODE retrieveBearerDescription(UINT32 length, STK_BD *pBearerDesc);

RESULT_CODE retrieveBufferSize(UINT32 length, UINT32 *pBufSize);

/* !!! length - IPv4Len or IPv6Len !!! */
RESULT_CODE retrieveOtherAddress(UINT32 length, STK_OTHER_ADDRESS *pAddress);

RESULT_CODE retrieveInterfaceTransportLevel(UINT32 length, STK_ITL *pItl);

RESULT_CODE retrieveNetworkAccessName(UINT32 length, STK_TEXT_STRING_DATA *pNetwrkAccessName);

RESULT_CODE retrieveDeviceIdenties(UINT32 length, DEVICE_IDENTITIES_STRUCT *pDevIdent);

RESULT_CODE retrieveChannelData(UINT32 length, P_STK_BIP_DATA_SETTINGS pBipData);

RESULT_CODE retrieveChannelDataLength(UINT32 length, UINT8 *pDataLen);

RESULT_CODE retrieveEventList(UINT32 length);

RESULT_CODE handleOpenChannelRequest(STK_OPEN_CHANNEL_DATA openChannelData);

RESULT_CODE handleCloseChannelRequest(STK_CLOSE_CHANNEL_DATA CloseChannelData);

RESULT_CODE handleSendDataRequest(STK_SEND_DATA SendData);

RESULT_CODE handleReceiveDataRequest(UINT8 channel, UINT8 len);

RESULT_CODE handleGetChannelStatus(void);

void STK_removeChannel(P_STK_BIP_CHANNEL pBipChannel);

void STK_closeAllBipChannels(int simid);

BOOL STK_isEventInList(UINT8 event);

BOOL STK_setEventListMask(UINT8 event);

RESULT_CODE STK_ActivateBIPChannel(IN OUT P_STK_BIP_CHANNEL pBipChannel);

RESULT_CODE STK_InitTcpClient(IN UINT8 ipVersion, IN UINT32 port, IN UINT32 dest_ipv4addr, IN UINT8 *pdestAddrIPv6,
			 OUT SOCKET_DESC *pSd, OUT struct sockaddr_in *pAddr);

RESULT_CODE STK_InitUdpClient(IN UINT8 ipVersion, IN UINT32 port, IN UINT32 dest_ipv4addr, IN UINT8 *pdestAddrIPv6,
			 OUT SOCKET_DESC *pSd, OUT struct sockaddr_in *pAddr);

RESULT_CODE STK_InitTcpServer(IN UINT8 ipVersion, IN UINT32 port, IN UINT32 dest_ipv4addr, IN UINT8 *pdestAddrIPv6,
			 OUT SOCKET_DESC *pSd, OUT struct sockaddr_in *pAddr);

RESULT_CODE STK_InitSocketFunctions(UINT8 protocol, P_STK_BIP_CHANNEL pBipChannel);

RESULT_CODE STK_ManagePDPConnection(STKBearerType type, P_STK_BIP_CHANNEL pBipChannel);

P_STK_BIP_CHANNEL STK_CreateBipChannel(void);

BOOL STK_RxBufMemCpy(UINT8 channel, char *src, UINT32 len);

BOOL STK_getRxLen(UINT8 channel, int *rxlen);

BOOL STK_SetRxLen(UINT8 channel, int rxlen);

RESULT_CODE STK_CloseClientSocket(P_STK_BIP_CHANNEL pBipChannel);

RESULT_CODE STK_CloseTcpServer(P_STK_BIP_CHANNEL pBipChannel);

RESULT_CODE STK_SendDataTcpClient(STK_BIP_DATA_SETTINGS Data, BOOL sendImmid);

RESULT_CODE STK_SendDataUdpClient(STK_BIP_DATA_SETTINGS Data, BOOL sendImmid);

RESULT_CODE STK_SendDataTcpServer(STK_BIP_DATA_SETTINGS Data, BOOL sendImmid);

RESULT_CODE STK_ReceiveData(UINT8 len, UINT8 channel);

int STK_BindBIPChannel(SOCKET_DESC sd, UINT8 cid);

void STK_ReadLoop(UINT16 channelStatus, SOCKET_DESC sd, P_STK_BIP_CONNECTION_DATA BipConnectData);

RESULT_CODE STK_sendBIPData(BOOL sendImmid, UINT32 data_len, char* pdata,
							  struct sockaddr * to, SOCKET_DESC sd,
							  P_STK_BIP_CONNECTION_DATA BipConnectData);

//void STK_TcpClientThreadFunc(UINT8 *pchannel);

void STK_UdpClientThreadFunc(void *pchannel);

void STK_TcpServerThreadFunc(void *pchannel);

RESULT_CODE STK_checkBipRxTxBuffSize(StkTransportProtocol protocol, UINT32 *pSize);

RESULT_CODE STK_allocBipBuff(StkTransportProtocol protocol, UINT32 size, char **Buf);

void STK_CleanupBIPChannel(int simid, int channel);

void STK_CloseSocket(IN OUT SOCKET_DESC * sock);

void STK_teardownDataConnection(UINT8 channel);

BOOL STK_sameAPN(char *apn,int cid);

BOOL STK_specifiePDPcontext(int cid, char *apn);

BOOL STK_setStateOfPDPcontext(UINT32 cid, UINT8 state, UINT8 CmndType);

BOOL STK_setLTEQoService(int cid, STK_BD bearerDescription);

BOOL STK_isTargetPDPActive(int *cid);

int findPdpByState(BOOL is_active);

UINT32 STK_isCidValid(UINT32 cid);

void STK_setPdpBipState(UINT32 cid, UINT32 state);

void STK_setNumbersOfCid(UINT32 numberOfCids);

UINT32 STK_getPdpBipState(UINT32 cid);

UINT32 STK_getNumbersOfCid(void);

void STK_setPdpIPv4Addr(UINT32 cid, UINT32 ipv4_addr);

void STK_setPdpAPN(UINT32 cid, char *apn);

UINT32 STK_getPdpIPv4Addr(UINT32 cid);

void STK_getPdpAPN(UINT32 cid, char *apn);

RESULT_CODE processCommandDetails(IN TLV *pBerTlv, OUT COMMAND_DETAILS_STRUCT *pCmdDet);

RESULT_CODE processOpenChannel(OUT TLV *pOpenChannel, COMMAND_DETAILS_STRUCT cmdDet);

RESULT_CODE processCloseChannel(TLV *pCloseChannel, COMMAND_DETAILS_STRUCT cmdDet,UINT32 *add_result);

RESULT_CODE processSendData(OUT TLV *pSendData, COMMAND_DETAILS_STRUCT cmdDet,UINT32 *add_result);

RESULT_CODE processReceiveData(OUT TLV *pReceiveData, COMMAND_DETAILS_STRUCT cmdDet);

RESULT_CODE processGetChannelStatus(OUT TLV *pReceiveData, COMMAND_DETAILS_STRUCT cmdDet);

RESULT_CODE processSetUpEventList(OUT TLV *pSetUpEvntLst, COMMAND_DETAILS_STRUCT cmdDet);

RESULT_CODE decodeSimpleTlv(OUT TLV *pTlv); //common decoding fnct getLength getTag getCr.

RESULT_CODE retrieveFileList_string(TLV Refresh);

int parseProactiveCommand(char *pdata);

void sendTerminalResponse_BIP(MTILRequestHandle reqHandle, RESULT_CODE resValue, UINT32 addResValue, STK_SendTRFormat pSTK_SendTRFormat, void *pChannel);

void STK_SendTRFormat_OpenChannel(char *pRespBuf, STK_BIP_CHANNEL *Channel);

void STK_SendTRFormat_SendData(char *pRespBuf, STK_BIP_CHANNEL *Channel);

void STK_SendTRFormat_ReceiveData(char *pRespBuf, STK_RECEIVE_TERM_RESP *pReceiveTR);

void STK_SendTRFormat_GetChannelStatus(char *pRespBuf, P_STK_GET_CHNNL_STATUS_TERM_RESP pStatus);


void sendEnvelope_BIP(MTILRequestHandle reqHandle, UINT8 event, UINT8 sourceId, UINT8 destId, STK_SendEnvelopeFormat pSTK_EnvelopeFormat, void *pparams);
void sendEnvelope_SMS(char *data, int pduLen, int scalen);

void STK_EnvelopeFormat_DataAvail(char *pRespBuf, P_STK_ENVLP_DATA_AVAIL Channel);

void STK_EnvelopeFormat_ChnnlStatus(char *pRespBuf, UINT16 *pStatus);

void STK_UpdateBipPDPInfo(MTIL_CGACT_STATUS_MSG * cgactStatusMsg);

int findNextAvailableChId(void);

UINT8 STK_getChannelidByStatus(UINT16 channelStatus);

P_STK_BIP_CHANNEL allocateBipChannel(void);

P_STK_BIP_CHANNEL findBipChannelByCID(UINT8 cid, UINT8 offset, UINT8 *pId);

P_STK_BIP_CHANNEL findBipChannelByBearerType(STKBearerType type, UINT8 offset, UINT8 *pId);

P_STK_BIP_CHANNEL STK_findBipChannelByDownCID(UINT8 cid);

void STK_setBipChannelState(UINT8 channel, UINT16 state, UINT16 *pchannelStatus);

void STK_startBipThread(STK_BIPListThread threadFunc, P_STK_BIP_CHANNEL pBipChannel);

void hex2str(UINT32 len, char *psrc, char *pdest);

void checkPSRet(UINT32 result);

static char *get_pdu_from_send_sms(char *data, char *smsc, int packing);
static int querySmsc(char *outBuf, int size);
static int encodeSmsc(char * smsc, char *outbuf, int includeLength);
static unsigned char getValueFromHexStr( char *data_str , unsigned long index );

void STK_setPdpIPv6Addr(UINT32 cid, UINT32* ipv6_addr);
UINT32* STK_getPdpIPv6Addr(UINT32 cid);
int STK_BindBIPChannel_IPv6(SOCKET_DESC sd, UINT8 cid);
void STK_setPdpIPtype(UINT32 cid, int type);
int STK_getPdpIPtype(UINT32 cid);

#endif //BIP_H


