/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*********************************************************************
*							INTEL Software
* --------------------------------------------------------------------
* Filename: 	DigRf3GPor_common.h
*
* Created By:  	Hui Li, 20, Oct, 2011
*
* Programmers: 	
*
* Module:  		Generic RF Module
*
*
* Notes:
*
***********************************************************************/

#ifndef DIGRF3GPOR_COMMON_H
#define DIGRF3GPOR_COMMON_H

#define SP8_NUM_OF_MESSAGE_IN_GROUP                       8
#define NUM_OF_GSM_MODULATION                             2
#define NUM_OF_PCL										  16
#define GSM_DEFAULT_LUT2_LENGTH                           32
#define GSM_DEFAULT_LUT1_LENGTH                           32
#define GSM_DEFAULT_LUT42_LENGTH                          16
#define GSM_RampProfile_LENGTH  		                  10


#define NUM_OF_WB_BAND									  11
//ICAT EXPORTED STRUCT
typedef struct Default_LUT1_DataTag
{
	UINT16 default_LUT1[GSM_DEFAULT_LUT1_LENGTH];
}Default_LUT1_Data_ts;
//ICAT EXPORTED STRUCT
typedef struct Default_LUT42_DataTag
{
	UINT16 default_LUT42[GSM_DEFAULT_LUT42_LENGTH];
}Default_LUT42_Data_ts;
//ICAT EXPORTED STRUCT
typedef struct Default_LUT2_DataTag
{
	UINT16 default_LUT2[GSM_DEFAULT_LUT2_LENGTH];
}Default_LUT2_Data_ts;
#if defined (ENABLE_CAT1_LG)
//ICAT EXPORTED STRUCT
typedef struct CalFileVersionFlagTag
{
	UINT16 calFileVersion;
}CalFileVersionFlag_ts;

//ICAT EXPORTED STRUCT
typedef struct RF_Config_returnSkylarkTag
{
	UINT16 rf_scheme_version;
	UINT16 band_combination_version;
}RF_Config_returnSkylark_ts;

//ICAT EXPORTED STRUCT
typedef struct 
{
	UINT16 	plRfDcxoIsUsingFlg;
}plRfDcxoIsUsingFlg_ts;
#endif
//#ifdef L1_DCXO_ENABLED
#if 0
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
	Bool					updatedFlag;
	UINT16                  TemperatureDac;
}DcxoAfcDacTempComp_ts;

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
    DcxoAfcDacTempComp_ts		dcxoAfcDacTempCompenLut[130];	
    SignedInt16				    dcxoMinCompenTemp;	/* MIN temperature value that compensated in LUT */
	SignedInt16				    dcxoMaxCompenTemp;	/* MAX temperature value that compensated in LUT */
	UINT8						dcxoTempCompenStep;		/* temperature compensation step in LUT, in degree C format*/
	Boolean                     dcxoAfcDacTempCompenLutisvalid;
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
	Boolean                     dacvalueisvalid;
	Boolean                     dcxoAfcDacTempCompenLutisvalid;
} Dcxo_Customer_TempCompen_Nvm_ts;
#endif
#endif  //DIGRF3GPOR_COMMON_H