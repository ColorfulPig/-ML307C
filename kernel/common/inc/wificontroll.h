#ifndef WIFI_CONTROLL_H
#define WIFI_CONTROLL_H

#ifdef L1_WIFI_LOCATION
#include "pl_d_globs.h"
#include "global_types.h"
#include "global_def.h"
#include "pl_w_types.h"
#include "gbl_types.h"

//#include "rtc.h"

//#include "pl_memheaph.h"


#include "diag_API.h"


#include "utilities.h"

#include "pl_cscmd.h"
#include "WS_IPCComm.h"
#include "IPCComm.h"
#include "WS_IPCCommConfig.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "oss.h"
#include "diags.h"
#include "Csw_mem.h"

#include "pl_am_err_handler.h"
#include "pl_econst.h"

//#include "pl_am_spy.h"

//Added by xqshi for wifi search schedule module
#define OPTIMAZTION_WIFI_SEARCH       (1)
#define WIFI_LOW_TASK_FEATURE         (1)
#define SUPPORT_WIFI_FT_MODE          (1)
#define OPTIMAZTION_WIFI_REPORT       (1)
#define DEUBG_PRINT                   (1)
#define ENABLE_BG_PERIODIC_SEARCH     (1)
#define WIIF_SPY_FEATURE_SUPPORT      (1)
//CQ00143839
#define ALLOW_WIFI_FAILURE_REPORT     (1)


#ifdef ENABLE_WB_R99
#define WIFI_WB_MODE_FEATURE          (1)	// LWG
#else
#define WIFI_WB_MODE_FEATURE          (0) 	// LG
#endif

#define VOLTAGE_NEW_SCHEME             (1)
#define REDUCE_NVM_FEATURE             (1)

//#define  ENABLE_AUDIO_MUTEX          (0)

#define MAX_CHANNEL_NUM (14)
#define FAST_CHANNEL_NUM (3)
#define MAC_BSSID_NUM_4CHANNEL (6)
#define NVM_HEAD_SET_STRING(header,field,value) \
   strncpy(header.field,value,sizeof(header.field)-1)
#define WIFI_CFG_FILE	"WIFI_Cfg.nvm"
//#ifdef ENABLE_BG_PERIODIC_SEARCH 
#define  WIFI_BG_CFG_FILE	"WIFI_BG_Cfg.nvm"
//#endif

#define WIFI_RSSI_CORRECT_CFG_FILE "WIFI_RSSICORRECT_Cfg.nvm"



typedef unsigned char				uint8;
typedef unsigned short				uint16;
typedef unsigned int				uint32;


//ICAT EXPORTED ENUM
typedef enum {
	ACTION_TYPE_STOP_WIFI =0,
	ACTION_TYPE_START_WIFI
}actionType_te;

//ICAT EXPORTED ENUM
typedef enum  {
	COLLEECT_ALL_REPORT =0,
	IMMEDIATE_REPORT
}reportMode_te;

//ICAT EXPORTED ENUM
typedef enum {
	SCAN_TYPE_FULL =0,
	SCAN_TYPE_LIST
}scanType_te;


//ICAT EXPORTED STRUCT
typedef struct{
	actionType_te startStopInd;// start: 1; stop: 0;
	scanType_te scanType;// scan_list: 1; scan_full: 0;
	reportMode_te reportMode;// collect_all: 0;immediate 1;
	uint8 scanChannelCnt ;//If(scanType==scan_list) ,scanChannelCnt&&channelList  is valid
	uint8 channelList[MAX_CHANNEL_NUM];
	uint8 ScanTimelength;
	//modified by yiyuan project
	//uint8 pad;
	uint8 ScanMaxBssidNum; //from 4 to 10, and the default value is 5 
	//modified end 
}WFScanReq_ts;



//ICAT EXPORTED ENUM
typedef enum {
	Ps_High =0,
	Wifi_High
}ScanPriority_te;


//ICAT EXPORTED STRUCT
typedef struct{
	
	uint8 ScanRoundNum;  //from 1 to 3. and the default value is 3 
	uint8 ScanMaxBssidNum; //from 4 to 10, and the default value is 5 
	uint8 ScanTimeout;  // uint:second and the default value is 25s
	ScanPriority_te ScanPriority; //the prioprity of PS and wifi
}MMWFScanStartReq_ts;


//ICAT EXPORTED ENUM
typedef enum
{
  A_WFScanReq_ts                  = PLP_WIFI_START_SCAN_OPCODE,
  
}wifiplpCommands_Strings;


//ICAT EXPORTED STRUCT
typedef struct{
	uint16 gaplength;
	uint16 paddding ;
}GW2WFGapStart_ts;

#if 0
//ICAT EXPORTED STRUCT
typedef struct{
  uint8 bssid[SIZE_MAC_ADDRESS];
  uint8 padding[2];
  uint32 rssi;
  uint32 channel_number; //
}channelCellInfo_ts;


//ICAT EXPORTED STRUCT
typedef struct{
	
    uint8              bssid_number;
    channelCellInfo_ts channelCellList[MAX_BSSID_NUM_TOTAL];
    //scanRestult_te scanRestult;
}WFScanInd_ts;

#endif
//ICAT EXPORTED ENUM
typedef enum {
    Abort_NOT_ACTIVE=0,
	Abort_by_MM =1,
	Abort_by_RRC=2,
	Abort_by_WB=4,
	#ifdef ENABLE_AUDIO_MUTEX
	Abort_by_AUDIO=8,
	#endif
}AbortTriggerSource_te;


//ICAT EXPORTED ENUM
typedef enum {
    Abort_NOT_ACTIVE_MASK=0,
	Abort_by_MM_MASK =1,
	Abort_by_RRC_MASK=2,
	Abort_by_WB_MASK=4,
	#ifdef ENABLE_AUDIO_MUTEX
	Abort_by_AUDIO_MASK=8,
	#endif
}AbortTriggerSourceMask_te;


//ICAT EXPORTED ENUM
typedef enum {
    Start_NOT_ACTIVE=0,
	Start_by_MM =1,
	Start_by_BG =2,
	Start_by_MM_BG =3,
}StartTriggerSource_te;


//ICAT EXPORTED ENUM
typedef enum {
   NOT_ACTIVE=0,
   MM_START_REQ,
   MM_STOP_REQ,
   BG_START_REQ,
   START_REQ_RRC,
   RRC_ABORT_REQ,
   REQ_GPLC_ABORT,
   GPLC_ABORT_CNF,
   RRC_START_NOTALLOWED_CNF,
   START_REQ_GPLC_WB,
   PHY_WIFI_SCAN_IND,
   PHY_WIFI_STOP_IND,
   RRC_START_ALLOWED_CNF,
   GPLC_GAP_IND,  
   LTE_GAP_IND_STATE, 
   PHY_GAP_FINISH_IND, 
  // REPORT_IND_2HOST,
   TIMER_2_REQ_AGAIN,
   TIMER_2_BG_WIFI,
   AUDIO_BUSY_NOT_ALLOWED,
   AUDIO_ABORT_REQ,

}WifiScanState_te;

#ifdef SUPPORT_WIFI_FT_MODE  

//ICAT EXPORTED ENUM
typedef enum {
  NORM_MODE = 0,
  FT_MODE,  // for factorty test mode 

}WifiScanMode_te;

#endif
//CQ00143839
#ifdef ALLOW_WIFI_FAILURE_REPORT
//ICAT EXPORTED STRUCT
typedef enum
{
  NULL_CAUSE =0,
  DUAL_SIM_CAMP,
  SERVICE_ONGOING,
  NOT_ENTER_IDLE,
}wifi_reject_cause_te;


#define  SetFailureCausebit(data, bit)     (data = (data | (1 << bit)))
#define  GetFailureCausebit(data, bit)     (data & (1 << bit))

#endif

//ICAT EXPORTED STRUCT
typedef struct{
//added for yiyuan project
   uint8 ScanRoundNum;  //from 1 to 3. and the default value is 3 
   uint8 ScanMaxBssidNum; //from 4 to 10, and the default value is 5 
   uint16 ScanTimeout;	// uint:second and the default value is 25s , Scantimeout*1000
   
//Added end 
   AbortTriggerSource_te AbortTriggerSource;
   WifiScanState_te       WifiScanState;
   uint8   WifiScanCnt;
   #ifdef	OPTIMAZTION_WIFI_SEARCH
   uint8   WifiRetryReqCnt;
   uint32  WifiScanTimeout;
   #endif
   BOOL    IratfiScanGapStartFirstFlag;
   #ifdef SUPPORT_WIFI_FT_MODE
   uint8   WifiScanMode;
   ScanPriority_te ScanPriority; //the prioprity of PS and wifi
   uint8   pad[1];
   #else
   ScanPriority_te ScanPriority; //the prioprity of PS and wifi
   uint8   pad[2];
   #endif
   #ifdef ENABLE_BG_PERIODIC_SEARCH 
   //uint32  WifiScanReportTime;
   StartTriggerSource_te StartTriggerSource;
   uint8   pad1[3];
   #endif
   //CQ00143839
   #ifdef ALLOW_WIFI_FAILURE_REPORT
   wifi_failure_cause_te  wifi_failure_cause;
   uint8   pad2[3];
   #endif
}WFScanDb_ts;

#ifdef DEUBG_PRINT
//ICAT EXPORTED STRUCT
typedef struct{
   uint16   Wifi3ScanroundCnt;
   uint16   Wifi2ScanroundCnt;
   uint16   Wifi1ScanroundCnt;
   uint16   WifiMMCmdCnt;
   uint16   WifiBgCmdCnt;
   uint16   WifiBssidNumLess3Cnt;
   uint16   WifiBssidNum3Cnt;
   uint16   WifiBssidNum4Cnt;
   uint16   WifiBssidNum5Cnt;
   uint16   WifiBssidNum6Cnt;
   uint16   WifiBssidNum7Cnt;
   uint16   WifiBssidNum8Cnt;
   uint16   WifiBssidNum9Cnt;
   uint16   WifiBssidNum10Cnt;
   uint16   WifiLess400msCnt;
   uint16   WifiLess500msCnt;
   uint16   WifiLess600msCnt;
   uint16   WifiLess700msCnt;
   uint16   WifiLess800msCnt;
   uint16   WifiLess900msCnt;
   uint16   WifiLess1sCnt;
   uint16   WifiLess2sCnt;
   uint16   WifiLess3sCnt;
   uint16   WifiLess6sCnt;
   uint16   WifiLess9sCnt;
   uint16   WifiLess12sCnt;
   uint16   WifiLess15sCnt;
   uint16   WifiLess18sCnt;
   
}WFScanDbgDb_ts;
#endif

extern void gfiWifiScanCnf(uint8 reason);
extern void gfiScanGapStartInd(uint16 gaplength);
extern void gfiWifiScanAbortCnf(void) ; 
extern void plWIFIPRVTCreateFacilities(void);
extern void lfiScanGapStartInd(uint16 gaplength,UINT8 simId);
extern void FakesetScanTimer(void);
extern void MM2WifiStartScanReq(void);
extern void MM2WifiStopScanReq(void);
extern void getIratDsWifiAbortReq(UINT8 simId);
//CQ00143839
#ifdef ALLOW_WIFI_FAILURE_REPORT
extern void getIratDsWifiStartCnf(Bool allowed,int cause);
#else
void getIratDsWifiStartCnf(Bool allowed);
#endif



//Added for wifimainloop  task
/**********************************************************
*
* Enums for all WIFI memory pools
*
**********************************************************/
enum wifiPools
{

    POOL_PLP_TO_WIFI_SCAN_CNF=0,
  
    MAX_WIFI_POOLS
};

#define    SIZE_POOL_PLP_TO_WIFI_SCAN_CNF            (POOL_PLP_TO_WIFI_SCAN_CNF_PARTITION+OS_MEM_POOL_OVERHEAD)*L1WIFI_Q_DEPTH
//CQ00143530
#define    POOL_PLP_TO_WIFI_SCAN_CNF_PARTITION        sizeof(WFScanIndPLP_ts)
#define    SIZE_POOL_PLP_TO_WIFI_SCAN_CNF_32BYTE      SIZE_POOL_PLP_TO_WIFI_SCAN_CNF/4
#define    WIFI_TASK_STACK_SIZE                       8192

#if 0

#define    DST_WIFI 0x2F
#define    DST_MM   0x21

#define    SRC_WIFI 0x31
#define    SRC_DSP  0x30
#define    SRC_RRC 	0x10
#define    SRC_MM 	0x11
#define    SRC_TIMER 0x32
#define    SRC_GSM  0xD0
#endif


#define	SRC_RRC 	 0x10
#define	SRC_MM       0x20
#define	SRC_DSP      0x30
#define	SRC_TIMER    0x40
#define	SRC_GSM      0x50
#define	SRC_WIFI     0x60
#define SRC_WB       0x70
#define SRC_AUDIO    0x80

    /* External Event Destinations */
#define	 DST_RRC     0x01
#define  DST_MM      0x02
#define  DST_DSP     0x03

    
    /* Internal Event Destinations */
#define   DST_WIFI   0x04
#define   DST_GSM    0x05
#define   DST_L1A    0x06
#define   DST_AUDIO  0x07


#define    NULL_POOL  0


//ICAT EXPORTED STRUCT
typedef struct
{
  UINT32 wifiQDepth;
  UINT32 wifiStackSize;
  UINT32 wifiTaskPriority;
  UINT32  padding ;
}wifiConfigParams_ts;

#define L1C_WIFI_DEFAULT_DATA {L1WIFI_Q_DEPTH,WIFI_TASK_STACK_SIZE,4,0}
#ifdef WIFI_LOW_TASK_FEATURE 
#define L1C_WIFI_LOW_DEFAULT_DATA {L1WIFI_Q_DEPTH,WIFI_TASK_STACK_SIZE, 21,0}
#endif
#define L1WIFI_Q_DEPTH   6


#define TIMER_FOR5MS_TICK      (1)
#define TIMER_FOR1S_TICK       (200)
#define FAKE_STOP_TIMER_LENGTH  (200*4)  // 1 seconds when 200ticks is 1 sec
#define FAKE_STOP_START_TIMER_LENGTH  (200*1)  // 1 seconds when 200ticks is 1 sec

#define FAKE_SCAN_TIMER_LENGTH  (200*25)  // 1 seconds when 200ticks is 1 sec


#define GET_MM_FT_WIFI_START_REQ           (0x7120)

#define GET_MM_WIFI_START_REQ              (0x4120)
#define GTE_BG_TIMER_WIFI_START_REQ        (0x5120)
#define GET_TIMRE_TRY_WIFI_START_REQ       (0x6120)
#define GET_RRC_WIFI_START_CNF_ALLOWED     (0x1120)     
#define GET_RRC_WIFI_START_CNF_NOTALLOWED  (0x2120) 
#define GET_MM_WIFI_STOP_REQ               (0x4121)
#define GET_RRC_WIFI_ABORT_REQ             (0x2121) 
#define GET_AUDIO_WIFI_ABORT_REQ           (0x5121) 

#define GET_GFISCAN_GAPSTART_IND           (0x2122)
#define GET_WFISCAN_GAPSTART_IND           (0x3122)
#define GET_MM_WIFI_ABORT_REQ              (0x3121)
#ifdef WIFI_LOW_TASK_FEATURE 
#define REPORT_IND_2HOST_PROCESS           (0x3120)
#define CORE_FREQ_CHANGE_416_PROCESS       (0x3200)
#endif
#ifdef WIIF_SPY_FEATURE_SUPPORT
#define IRAT_DS_WIFI_START_REQ             (0x8120)   
#define IRAT_DS_WIFI_FINISH_IND            (0x8200)
#define IRAT_DS_WIFI_ABORT_CNF             (0x8201)
#define AUDIO_WIFI_ABORT_CNF               (0x9201)

#endif
#if(WIFI_WB_MODE_FEATURE == 1)
//#define GET_TO_REMOVE_WB_TASK_REQ          (0x1202)
#define SET_TO_SCHD_WIFI_TASK_START_REQ    (0x4122)
#endif


#define TIMER_FORCHANGEFREQ     (1) // 1ms
#define WIFI_SCAN_TIMEOUT       (25*1000) //25s
#define MAX_VALUE_NUMBER          (0x418937) //0x418937=0xFFFFFFFF/1000

#define WIFI_ONESCAN_TIME         (2*1000) //2s
#define WIFI_SCAN_REQ_TIME        (1*1000)  //1s
#define WIFI_SCAN_REQ_TIMER       (1*200) //1s for timer tick
#define WIFI_ONESCAN_TIMER        (400) //2s for timer tick
#define WIFI_ONESCAN_TIMER_CHECK  (1000) //5s for timer tick
#define WIFI_BG_10S_TIMER         (2000)
#define WIFI_BG_1MIN_TIMER        (12000)        //60s 1s for timer tick
#define WIFI_BG_3MIN_TIMER        (3*WIFI_BG_1MIN_TIMER)        //60s 1s for timer tick
#define WIFI_BG_5MIN_TIMER        (5*WIFI_BG_1MIN_TIMER)        //60s 1s for timer tick




//Definition of task ID type.
typedef UINT8  SourceID_type;
typedef UINT8  DestinationID_type;
typedef UINT16 MessageID_type;

//ICAT EXPORTED STRUCT
typedef struct
{
    MessageID_type          messageID;
    UINT16                  messageLength;
    SourceID_type           sourceID;
    DestinationID_type      destinationID;
    UINT8                   messageControlFlags; //for future use
    VOID                    *messageBody;
    OSPoolRef               poolRef;
} wifiMsgHeader;


#define STACK_WATCH_POINT   100
//ICAT EXPORTED ENUM
typedef enum {
	_WIFI_SCAN_DISABLE = 0,
	_WIFI_START_SCAN_ENABLE,
	_WIFI_STOP_SCAN_ENABLE,
} WIFI_TEST_TYPE;

//ICAT EXPORTED ENUM
typedef enum {
	_WIFI_SCAN_FULL_BAND = 0,
	_WIFI_SCAN_FULL_LIST = 1,
	_WIFI_SCAN_FAST_THREE =2,
	_WIFI_SCAN_FAST_ONCE =3,
} WIFI_MODE_TYPE;

//ICAT EXPORTED STRUCT
typedef struct {
	WIFI_TEST_TYPE     WifiCfgVal;
	WIFI_MODE_TYPE     WifiModeCfgVal;
	UINT16		StartscanInterval;//in seconds
	UINT16      StopscanInterval;
} WifiCfgDataS;

//ICAT EXPORTED STRUCT
typedef struct {
   INT32   WifiRssiCorrect[3];
}WifiRssiCorrectS;
//#ifdef ENABLE_BG_PERIODIC_SEARCH 
//ICAT EXPORTED ENUM
typedef enum {
	_WIFI_BG_NOT_ACTIVE= 0,
	_WIFI_BG_ONCE_1MIN = 1,
	_WIFI_BG_ONCE_3MIN =2,
	_WIFI_BG_ONCE_5MIN =3,
	_WIFI_BG_ONCE_10S  =4,
} WIFI_BG_PERIODIC_TYPE;

//ICAT EXPORTED ENUM
typedef enum {
   _WIFI_BG_FAST_ONCE =0,
   _WIFI_BG_FAST_THREE =1,
} WIFI_BG_MODE_TYPE;

//ICAT EXPORTED STRUCT
typedef struct {
	WIFI_BG_PERIODIC_TYPE     WifiBgCfgVal;
	WIFI_BG_MODE_TYPE         WifiBgModeCfgVal;
	uint8                     WifiNearTimeRequire;          //second
	uint8                     WifiBgMinBssidNum;  //min num
} WifiBgCfgDataS;

#define IS_IDX_SET(mask,idx)    ((1<<(idx))&(mask))
#define RESET_IDX(mask,idx)     ((mask)&=(~(1<<(idx))))


//#endif


//Added end 
#endif
#endif

