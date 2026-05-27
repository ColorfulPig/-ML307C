#ifndef DCXO_CONTROLL_H
#define DCXO_CONTROLL_H

//#include "pl_d_globs.h"
//#include "global_types.h"
//#include "global_def.h"
//#include "pl_w_types.h"
//#include "gbl_types.h"


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

#ifdef PHS_SW_DEMO_TTC
#include "nvm_header.h"
#include "crossPlatformSW.h"

#endif

#include "TSXController.h"

#ifndef UINT16
#define UINT16 unsigned short
#endif // UINT16

#ifndef INT16
#define INT16 signed short
#endif // INT16

#ifndef UINT8
#define UINT8 unsigned char
#endif

//#ifndef Bool
//#define Bool unsigned char
//#endif

//#ifndef Boolean
//#define Boolean unsigned char
//#endif

//#ifndef BOOL
//#define BOOL unsigned char
//#endif


#ifndef SignedInt16
#define SignedInt16  signed short
#endif

#ifndef SignedInt32
#define SignedInt32  signed long
#endif


#define TRUE  1
#define FALSE 0


#ifndef NULL
    #define NULL    0
#endif 

#ifdef L1_DCXO_ENABLED
#define DCXO_CALIBRATION_NVM_FILE_NAME		"Dcxo_Calibration.nvm"
#define DCXO_CALIBRATION_NVM_VER			 	"00.00"  
#define DCXO_TEMPCOMPENSATION_NVM_FILE_NAME		"Dcxo_TempCompensation.nvm"
#define DCXO_TEMPCOMPENSATION_NVM_CUSTOMER_FILE_NAME		"Dcxo_TempCompensation_Customer.nvm"
#define DCXO_TEMPCOMPENSATION_NVM_VER			 	"00.00"  
#endif


void DcxoIpcDspMsgIpcHandler (UINT16 msgOpCode, UINT16 msgLength, UINT8 *msgData);

void plDcxoSetDspToDcxoMode(void);
void plDcxoSetRfTempretureReadMode(void);
void plDcxoSendRfCaliDataToPlp(void);
void plDcxoSendRfTemCompenLutToPlp(void);

extern UINT16 RfDcxoIsUsingFlgWb;


#ifdef L1_DCXO_ENABLED
#define MAX_SUPPORTED_DCXO_TEMP_IDX		       146
#define DCXO_DEFAULT_COARSE_VALUE		       (45)
#define DCXO_DEFAULT_CALIB_TEMPERATURE	       (27)
#define MAX_TEMPERATURE                        (105)
#define MIN_TEMPERATURE                        (-40)
#define MAX_SUPPORTED_DCXO_TEMP_IDX_CUSTOMER    50
//ICAT EXPORTED STRUCT
typedef struct
{
    UINT16 year;   //  Calendar year: e.g 2001
    UINT8 month;  //  Month of year: 1 - 12
    UINT8 day;		// date of month: 1~31
}DcxoCalander_ts;

//ICAT EXPORTED STRUCT
typedef struct
{
    //DcxoCalander_ts			timeStamp;
    INT16					dcxoAfcDacTempCompenVal;	/* the temperature comensation delta DAC value for the calibrated AFC DAC */
    //UINT16					updateCount;			 
	//Bool					updatedFlag;
	UINT16                  TemperatureDac;
}DcxoAfcDacTempCompCustomer_ts;
//ICAT EXPORTED STRUCT
typedef struct
{
    DcxoCalander_ts			timeStamp;
    INT16					dcxoAfcDacTempCompenVal;	/* the temperature comensation delta DAC value for the calibrated AFC DAC */
    UINT16					updateCount;			 
	unsigned char					updatedFlag;
	UINT16                  TemperatureDac;
}DcxoAfcDacTempComp_ts;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT8 Valid; //1-DB valid, otherwise invalid
	UINT8 Reserved;
	INT8 CaliTemp[2]; //Calibration temperature
	STRUCT_FREQOFFSET_INF FreqOffInf[2];// Calibration FreqOffset Info
	STRUCT_3RD_ORDER_POLY_PARAM PolyCoef;
}TsxCalibrationParam_ts;

//ICAT EXPORTED STRUCT
typedef struct DcxoCalibrationNvmTag
{
	UINT16                      useExternalTemp;        //0--internal, 1--external
	UINT16						dcxoCalibTemp;			/* the temperature in calibration*/	
	UINT16                      dcxoCalibTempDac;
	UINT16						dcxoCalibCoarseValue;		/* Coarse value */                     
	SignedInt32 				dcxoCalibMultiplier;
  	SignedInt32 				dcxoCalibDivisor;
  	SignedInt16     			dcxoCalibNominalDac;      /* DAC value */
  	SignedInt32    	 			dcxoCalibMinInputDac;     // DAC value /
  	SignedInt32     			dcxoCalibMaxInputDac;     // DAC value /
  	SignedInt16                 frqOffsetForcoarse64;     //0.1ppm
    DcxoAfcDacTempComp_ts		dcxoAfcDacTempCompenLut[126];
	UINT32                      Reserved;
	TsxCalibrationParam_ts      TsxCaliParam;
    SignedInt16				    dcxoMinCompenTemp;	/* MIN temperature value that compensated in LUT */
	SignedInt16				    dcxoMaxCompenTemp;	/* MAX temperature value that compensated in LUT */
	UINT8						dcxoTempCompenStep;		/* temperature compensation step in LUT, in degree C format*/
	unsigned char               dcxoAfcDacTempCompenLutisvalid;
} Dcxo_Calibration_Nvm_ts;

//ICAT EXPORTED STRUCT
typedef struct DcxoTempCompenNvmTag
{
    DcxoAfcDacTempComp_ts		dcxoAfcDacTempCompenLut[MAX_SUPPORTED_DCXO_TEMP_IDX];	
    SignedInt16					dcxoMinCompenTemp;	/* MIN temperature value that compensated in LUT */
	SignedInt16					dcxoMaxCompenTemp;	/* MAX temperature value that compensated in LUT */
	UINT8						dcxoTempCompenStep;		/* temperature compensation step in LUT, in degree C format*/
} Dcxo_TempCompen_Nvm_ts;
//ICAT EXPORTED STRUCT
typedef struct DcxoCustomerTempCompenNvmTag
{
    DcxoAfcDacTempCompCustomer_ts		dcxoAfcDacTempCompenLut[MAX_SUPPORTED_DCXO_TEMP_IDX_CUSTOMER];	
    SignedInt16					dcxoMinCompenTemp;	/* MIN temperature value that compensated in LUT */
	SignedInt16					dcxoMaxCompenTemp;	/* MAX temperature value that compensated in LUT */
	UINT8						dcxoTempCompenStep;		/* temperature compensation step in LUT, in degree C format*/
	UINT16                      useExternalTemp;        //0--internal, 1--external
	unsigned char                     dacvalueisvalid;
	unsigned char                     dcxoAfcDacTempCompenLutisvalid;
} Dcxo_Customer_TempCompen_Nvm_ts;
#endif

#if defined (UPGRADE_LTE_ONLY)

#define	CF_NVM_GPLC_NVM_PARAMS_FILE_NAME		"gplcNVMParams.nvm"
#define CURRENT_GPLC_NVM_PARAMS_VERSION		"8.0"

typedef unsigned short Arfcn;


//ICAT EXPORTED ENUM
typedef enum SaicModeTag
/* @ENUM_DESC@ SAIC mode of work */
{
   NORMAL_OPERATION_MODE,       /* @ENUM_VAL_DESC@ SAIC enabled - Normal Operation (FW decides if to use SAIC or not) */
   CONVENTIONAL_MODE,			/* @ENUM_VAL_DESC@ Force Conventional Mode on FW. */
   SAIC_MODE,					/* @ENUM_VAL_DESC@ NOT supported (Force SaiclMode on FW) */
   SAIC_MODE_NUM_OF_ELEMENTS	/* @ENUM_VAL_DESC@ NOT-a-VALUE (Number of elements in SaicMode ENUM) */
}
SaicMode;


//ICAT EXPORTED ENUM
typedef enum BandTag
{
    /* RR uses these to index cell information from an array dimensioned to NUM_BANDS. Therefore
    ** they must be kept in the same order */
    GSM_BAND        = 0,
    DCS_BAND        = 1,
    PCS_BAND        = 2,
    EGSM_BAND       = 3,
#if defined (UPGRADE_MORE_BANDS)
    GSM_450_BAND    = 4,
    GSM_480_BAND    = 5,
    GSM_850_BAND    = 6,



#endif
    NUM_BANDS, /**< The number of bands supported by Layer 1 and RR */
    			
#if defined (UPGRADE_3G)
		/* used by MM */
		UMTS_BAND		= 0xFE,
#endif
    INVALID_BAND    = 0xFF
}
Band;


//ICAT EXPORTED STRUCT
typedef struct GPLCGeneralParamsTag	
{
	UINT32  	support850Band;

	UINT16		statisticsResolution; 
	unsigned char 	reducedScEstimatorRateEnabled;
	unsigned char 	singleSlotDecodingEnabled;

	unsigned char 	partialDecodingEnabled;
	unsigned char 	multislotPowerProfileEnabled;
	unsigned char 	bepEnabled;
	SaicMode	saicOrConventional;

	SignedInt16 initSyncBsicRssiThr;	
	unsigned char 	cipherEngineOddIntEnable;
	UINT8		GplcMipsStatsEnable;

	unsigned char 	BandLockEnable;
	Band		LockedBand;
	unsigned char 	CellLockEnable;
	UINT8		reserved_not_used; 	// for alignment purposes - new ACAT does not open it if misaligned!

	Arfcn		LockedCell;
	UINT8		reserved_not_used2[2]; 	// for alignment purposes - new ACAT does not open it if misaligned!
	#ifdef L1_DCXO_ENABLED
		UINT16		RfDcxoIsUsingFlg;
	#endif

}GPLCGeneralParams;



//ICAT EXPORTED STRUCT
typedef struct _GPLCParamsNVMStruct
{
	NVM_Header_ts nvmHeader;
	GPLCGeneralParams gplcGeneralParams;
} GPLCParamsNVMStruct;
#endif

//extern int DcxoTempCompensationCustomer_CreateNVMFile(void);
extern unsigned char DcxoCalibration_ReadNVMFile(Dcxo_Calibration_Nvm_ts *pRfDcxoCalibrationNvm);
extern unsigned char DcxoTempCompensation_ReadNVMFile(Dcxo_TempCompen_Nvm_ts *pRfDcxoTempCompenData);
extern int DcxoTempCompensation_PrintData(void);
int DcxoTempCompensation_CreateNVMFile(void);
//void polyfitforcomp(INT32 coef[], INT16 inputX[],INT16 inputY[],int length);
void plDcxoProcessModeReport(UINT8 simID);

extern void DcxoIpcDspMsgRegister(void);
extern void Dcxoinit(void);

#if defined (UPGRADE_LTE_ONLY)
void L1GetRfDcxoIsUsingFlag(void);
#endif

void SetRfDcxoIsUsingFlgWithGPIO (UINT8 DCXOEnable);

#endif

