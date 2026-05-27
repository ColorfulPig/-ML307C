/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/************************************************************************/
/*                                                             							              */
/* Title: AntTunerConfig pacakge header file                                             */
/*                                                    								                     */
/* Filename:  AntTunerConfig.h                  				           			                            */
/*                                                          								              */
/* Authors:  liangyao							                                                 */
/*                                                                      								*/
/* Target, subsystem: Common Platform, HAL                              					*/
/************************************************************************/
#ifndef _ANT_TUNER_CONFIG_H_

#define _ANT_TUNER_CONFIG_H_

/*General  configuration*/

#define ANT_TUNER_NUM_OF_BAND_LTE_INTRA_MAX					20				// CRANE SERIES LTE BAND reduce to 20
#define ANT_TUNER_NUM_OF_BAND_LTE_INTER_CA_MAX				0				// no CA 
#define ANT_TUNER_NUM_OF_BAND_LTE_MAX						(ANT_TUNER_NUM_OF_BAND_LTE_INTRA_MAX+ANT_TUNER_NUM_OF_BAND_LTE_INTER_CA_MAX)

#define ANT_TUNER_NUM_OF_BAND_WCDMA_MAX						5
#define ANT_TUNER_NUM_OF_BAND_GSM_MAX						4

#define ANT_TUNER_INVALID_BAND_NUM							0xFFFF

#define ANT_TUNER_GPO_CFG_INVALID_VAL						0xFFFF

#define ANT_TUNER_GPO_NUM_MAX								0x8				// MAX GPO NUBER used

// MAX support pamameter group, 4
#define ANT_TUNER_NUM_OF_PARAM_GROUP_MAX					4

/*----------------------------------------------------------------------*/
/* Antenna Tuner Mode Selection*/

//ICAT EXPORTED ENUM
typedef enum
{
	ANT_TUNER_OFF_MODE = 0x0,				// not use ANT tuner or ANT tuner control from RFIC side, 
	ANT_TUNER_GPIO_ONLY_MODE = 0x1,			// only use GPO for antenna tuner, either for pri/div antenna or both
	ANT_TUNER_RFFE_ONLY_MODE = 0x2,			// only use RFFE antenna tuner, either for pri/div antenna or both, for tunable capacitor
	ANT_TUNER_RFFE_GPIO_MODE = 0x3,			// use RFFE plus GPO
	NUM_OF_ANT_TUNER_MODE,
} AntTunerModeSelEnum;

/* Antenna Tuner RAT Mode Selection*/
//ICAT EXPORTED ENUM
typedef enum
{
	ANT_TUNER_RAT_MODE_LTE = 0x0,
	ANT_TUNER_RAT_MODE_WCDMA = 0x1,		
	ANT_TUNER_RAT_MODE_GSM = 0x2, 	
	NUM_OF_ANT_TUNER_RAT_MODE,
} AntTunerRatModeSelEnum;

// sync with GsmBandTypeEnum
//ICAT EXPORTED ENUM
typedef enum {
	ANT_TUNER_GSM_BAND_INVALID = 0xFF,
	ANT_TUNER_GSM_EGSM900 = 0,		//  LTE BAND8
	ANT_TUNER_GSM_DCS1800,			//  LTE BAND3
	ANT_TUNER_GSM_GSM850,			//  LTE BAND5
	ANT_TUNER_GSM_PCS1900,          //  LTE BAND2
	NUM_OF_ANT_TUNER_GSM_BAND,
}AntTunerGsmBandTypeEnum;

// sync with GsmBandTypeEnum
//ICAT EXPORTED ENUM
typedef enum {
	ANT_TUNER_GPO_OUT_LOW = 0x0,
	ANT_TUNER_GPO_OUT_HIGH = 0x1,		//  
}AntTunerGpoOutEnum;

/* Antenna Tuner GPO ID*/
//ICAT EXPORTED ENUM
typedef enum
{
	ANT_TUNER_GPO_ID_INVALID = 0,
	ANT_TUNER_GPO_ID_0 = 0,
	ANT_TUNER_GPO_ID_12 = 12,
	ANT_TUNER_GPO_ID_13 = 13,		
} AntTunerGpoIDEnum;

// sync with GsmBandTypeEnum
//ICAT EXPORTED ENUM
typedef enum {
	ANT_TUNER_DEBUG_0FF = 0x0,
	ANT_TUNER_DEBUG_GPO_DIRECT_CFG = 0x1,
}AntTunerDebugTypeEnum;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16 bandNum;
	UINT16 cfgVal;						// MAX 8bit infor since MAX 8 GPO used for antenna tuner purpose
} AntTunerGpoCfgStruct;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16 bandNum;
	UINT16 cfgVal;						// MAX 8bit infor since MAX 8 GPO used for antenna tuner purpose
	UINT32 reserved;
} AntTunerRffeCfgStruct;


//ICAT EXPORTED STRUCT
typedef struct
{
	AntTunerGpoCfgStruct lteCfgInfor[ANT_TUNER_NUM_OF_BAND_LTE_MAX];  // LTE should consider
	AntTunerGpoCfgStruct wcdmaCfgInfor[ANT_TUNER_NUM_OF_BAND_WCDMA_MAX];
	AntTunerGpoCfgStruct gsmCfgInfor[ANT_TUNER_NUM_OF_BAND_GSM_MAX];
} AntTunerGpoValContentStruct;


//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16 antTunerGpoNum;         // GPO number actually used
	UINT16 reservd1;
	UINT16 antTunerGpoId[ANT_TUNER_GPO_NUM_MAX];   // order of GPO ID should sync with cfgVal value in LTE/WB/GSM cfg infor
	UINT16 antTunerDefaultVal;		// default valud for all band 
	UINT16 reservd2;
	AntTunerGpoValContentStruct antTunerGpoValContent[ANT_TUNER_NUM_OF_PARAM_GROUP_MAX];
	UINT16 reserved[40];	// 
} AntTunerGpoParamStruct;


//ICAT EXPORTED STRUCT
typedef struct
{
	AntTunerRffeCfgStruct lteCfgInfor[ANT_TUNER_NUM_OF_BAND_LTE_MAX];  // LTE should consider
	AntTunerRffeCfgStruct wcdmaCfgInfor[ANT_TUNER_NUM_OF_BAND_WCDMA_MAX];
	AntTunerRffeCfgStruct gsmCfgInfor[ANT_TUNER_NUM_OF_BAND_GSM_MAX];
} AntTunerRffeValContentStruct;


//ICAT EXPORTED STRUCT
typedef struct
{
	UINT16 reserved[8];
	AntTunerRffeValContentStruct antTunerRffeValContent[ANT_TUNER_NUM_OF_PARAM_GROUP_MAX];
	UINT16 reserved1[80];
} AntTunerRffeParamStruct;


/**************************** AntTunerConfig*****************************/

void AntTunerSendGpoOnlyParam(void);

void AntTunerConfig(void);

void AntTunerGpoMfprSet(UINT16 gpoIdx);

void AntTunerGpoMfprConfig(void);

#endif  



