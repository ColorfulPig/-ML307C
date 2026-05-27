/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _DSP_DEBUG_CONFIG_NVM_H_
#define _DSP_DEBUG_CONFIG_NVM_H_


#define DSP_DEBUG_CONFIG_NVM_FILE_NAME	"DSPDebugConfig.nvm"
#define DSP_DBG_CFG_NVM_VER	"00.02" 


enum
{
	MONITOR_DPCH_RAKE = 0x1,
	MONITOR_HS_DSCH_INFO = 0x2,
	MONITOR_HS_SCCH_INFO = 0x4,
	MONITOR_DPCCH_INFO = 0x8,
	MONITOR_CQI_VALUES = 0x10
};


#define OutputForIml_Default OsUtil_Over_IML
#define HsdpaDebugCmd_Default MONITOR_HS_DSCH_INFO | MONITOR_DPCCH_INFO


//ICAT EXPORTED ENUM

typedef enum
{
	No_Debug=0,
	OsUtil_Over_IML,
	IPC_Over_IML,
	OsUtil_and_IPC_Over_IML	
}DSP_IML_CONFIG_te;

//ICAT EXPORTED ENUM

typedef enum
{
	AAAP_DISABLED=0,
	AAAP_ENABLED	
}AAAP_MODE_te;


//ICAT EXPORTED STRUCT

typedef struct
{
	BOOL				SendAudioDebug;
	UINT8				EnableGsmFTtool;	// 0-not active, 1-active except when PTM,  2-active all the time
	AAAP_MODE_te		AaapMode;	
} DSPDebugConfigNvm_ts;

//ICAT EXPORTED ENUM
typedef enum PmaxReductionFlagTypeTag{
	PMAX_REDUCTION_DISABLE = 0,
	PMAX_REDUCTION_ENABLE
}PmaxReductionFlagType;

//ICAT EXPORTED STRUCT
typedef struct RspDigRfGSMPACompenConfigTag
{
	INT16 gsmPACompenValueEGSM[2][4]; // dB in Q3
										//[2] is for modulation: 0: GMSK; 1: EPSK.
										//[4] is for number of Tx burst: 0: 1st Tx; 1: 2nd Tx; 2: 3rd Tx; 3: 4th Tx;
	INT16 gsmPACompenValueGSM850[2][4];
	INT16 gsmPACompenValueDCS[2][4];
	INT16 gsmPACompenValuePCS[2][4];
} RspDigRfGSMPACompenConfig_ts;


//ICAT EXPORTED STRUCT
typedef struct RspDigRfWBDBDCConfigTag
{
	UINT8 numOfBandCom;    //num of band combination supported
	UINT8 bandComList[16]; //the supported band combination, defined in 25.101:
						   //DBDCConfiguration Index | UL Band   |  DL Band A  |   DL Band B   |
						   //          0             | I or VIII |     I       |     VIII      |
						   //          1             | II or IV  |     II      |     IV        |
						   //          2             | I or V    |     I       |     V         |
						   //          3             | I or XI   |     I       |     XI        |
						   //          4             | II or V   |     II      |     V         |
} RspDigRfWBDBDCConfig_ts;

//ICAT EXPORTED STRUCT
typedef struct RspGSMPmaxReductionConfigTag
{
	PmaxReductionFlagType	pmaxReductionFlag;
	INT16 pmaxReductionValueEGSM[2][4]; // dB in Q3
										//[2] is for modulation: 0: GMSK; 1: EPSK.
										//[4] is for number of Tx burst: 0: 1Tx; 1: 2Tx; 2: 3Tx; 3: 4Tx;
	INT16 pmaxReductionValueGSM850[2][4];
	INT16 pmaxReductionValueDCS[2][4];
	INT16 pmaxReductionValuePCS[2][4];
} RspGSMPmaxReductionConfig_ts;

//ICAT EXPORTED STRUCT
typedef struct RspWbPmaxReductionConfigTag
{
	PmaxReductionFlagType	pmaxReductionFlag;
	INT16 pmaxReductionValueWBand1;
	INT16 pmaxReductionValueWBand2;
	INT16 pmaxReductionValueWBand4;
	INT16 pmaxReductionValueWBand5;
	INT16 pmaxReductionValueWBand8;
} RspWbPmaxReductionConfig_ts;

/*CQ00107636 start add*/
//ICAT EXPORTED STRUCT
typedef struct RspDigRfLtePmaxReductionConfigTag
{
	PmaxReductionFlagType	pmaxReductionFlag;
    /*CQ00107649 start del, no need 2\3G band*/
    #if 0
	INT16 pmaxReductionValueEGSM[2][4]; // dB in Q3
										//[2] is for modulation: 0: GMSK; 1: EPSK.
										//[4] is for number of Tx burst: 0: 1Tx; 1: 2Tx; 2: 3Tx; 3: 4Tx;
	INT16 pmaxReductionValueGSM850[2][4];
	INT16 pmaxReductionValueDCS[2][4];
	INT16 pmaxReductionValuePCS[2][4];
	INT16 pmaxReductionValueWBand1;
	INT16 pmaxReductionValueWBand2;
	INT16 pmaxReductionValueWBand4;
	INT16 pmaxReductionValueWBand5;
	INT16 pmaxReductionValueWBand8;
    #endif
    /*CQ00107649 end del*/
        //CQ00066262 add start,for LTE 44 bands
       INT16 pmaxReductionValueLTEBand1;
       INT16 pmaxReductionValueLTEBand2;
       INT16 pmaxReductionValueLTEBand3;
       INT16 pmaxReductionValueLTEBand4;
       INT16 pmaxReductionValueLTEBand5;
       INT16 pmaxReductionValueLTEBand6;
       INT16 pmaxReductionValueLTEBand7;
       INT16 pmaxReductionValueLTEBand8;
       INT16 pmaxReductionValueLTEBand9;
       INT16 pmaxReductionValueLTEBand10;
       INT16 pmaxReductionValueLTEBand11;
       INT16 pmaxReductionValueLTEBand12;
       INT16 pmaxReductionValueLTEBand13;
       INT16 pmaxReductionValueLTEBand14;
       INT16 pmaxReductionValueLTEBand15;
       INT16 pmaxReductionValueLTEBand16;
       INT16 pmaxReductionValueLTEBand17;
       INT16 pmaxReductionValueLTEBand18;
       INT16 pmaxReductionValueLTEBand19;
       INT16 pmaxReductionValueLTEBand20;
       INT16 pmaxReductionValueLTEBand21;
       INT16 pmaxReductionValueLTEBand22;
       INT16 pmaxReductionValueLTEBand23;
       INT16 pmaxReductionValueLTEBand24;
       INT16 pmaxReductionValueLTEBand25;
       INT16 pmaxReductionValueLTEBand26;
       INT16 pmaxReductionValueLTEBand27;
       INT16 pmaxReductionValueLTEBand28;
       INT16 pmaxReductionValueLTEBand29;
       INT16 pmaxReductionValueLTEBand30;
       INT16 pmaxReductionValueLTEBand31;
       INT16 pmaxReductionValueLTEBand32;
       INT16 pmaxReductionValueLTEBand33;
       INT16 pmaxReductionValueLTEBand34;
       INT16 pmaxReductionValueLTEBand35;
       INT16 pmaxReductionValueLTEBand36;
       INT16 pmaxReductionValueLTEBand37;
       INT16 pmaxReductionValueLTEBand38;
       INT16 pmaxReductionValueLTEBand39;
       INT16 pmaxReductionValueLTEBand40;
       INT16 pmaxReductionValueLTEBand41;
       INT16 pmaxReductionValueLTEBand42;
       INT16 pmaxReductionValueLTEBand43;
       INT16 pmaxReductionValueLTEBand44;
       //CQ00066262 add end,for LTE 44 bands
} 
RspDigRfLtePmaxReductionConfig_ts;
/*CQ00107636 end add*/

// Functions Prototypes
BOOL DSPDebugConfig_CreateNVMFile(void);
BOOL DSPDebugConfig_ReadNVMFile(DSPDebugConfigNvm_ts	*pDspDbgCfgNvm);


#endif // _DSP_DEBUG_CONFIG_NVM_H_
