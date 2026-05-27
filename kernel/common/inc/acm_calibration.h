/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2019 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

/******************************************************************************
*               MODULE IMPLEMENTATION FILE
*******************************************************************************
* Title: Header for Audio calibration
*
* Filename: acm_calibration.h
*
* Authors: Jackie Fan
*
* Description: Header file for Audio calibration.
*
* Last Updated:
*
* Notes:
******************************************************************************/
#ifndef _ACMCALIBRATION_H_
#define _ACMCALIBRATION_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "acm_comm.h"
#include "AudioHAL.h"
#include "Biquad_IIR.h"


//For each VE parameter, fill a tag to mark the end
#define ASR_BOARD_VE_TAG 	0xBEEF


#define ACM_VOLUME_CNT   AUDIOHAL_SPK_VOL_QTY
#define ACM_RATE_CNT     2  	//voice enhancement rate number

#define ACM_DSP_RXSIDETONE_BIT  (0x1 << ACMDSP_CONTROL_BITS_RX_SIDETONE)
#define ACM_DSP_RXVOLUME_BIT  	(0x1 << ACMDSP_CONTROL_BITS_RX_VOLUME)
#define ACM_DSP_TXVOLUME_BIT  	(0x1 << ACMDSP_CONTROL_BITS_TX_VOLUME)

#define ACM_CODEC_MIC_BIT		(0x1 << ACMCODEC_CONTROL_BITS_MIC)
#define ACM_CODEC_SIDETONE_BIT	(0x1 << ACMCODEC_CONTROL_BITS_SIDETONE)

/********************************************************************************************************
	ACMCodec_GainT.CodecControl defines board specific configuration:
	bit 0 : mic selection
	bit 1 : codec sidetone selection
*********************************************************************************************************/
#define ACMCODEC_MIC1 					0
#define ACMCODEC_MIC2					1


#define ACMMEDIA_CONTROL_BITS_RX_EQ (0x0001)
#define ACMMEDIA_CONTROL_BITS_RX_DRC_AND_HFP (0x0002)
#define ACMMEDIA_CONTROL_BITS_RX_SPK_PRO (0x0004)

#define ASR_BOARD_MEDIA_VE_SETTING 							\
            ( (ACMMEDIA_CONTROL_BITS_RX_DRC_AND_HFP)        \
            )


//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_CONTROL_BITS_MIC 		=  0,	
	ACMCODEC_CONTROL_BITS_SIDETONE 	=  1,

	ACMCODEC_CONTROL_BITS_CNT 		= 16	//16 control bits
} ACMCODEC_CONTROL_BITS;


/********************************************************************************************************
	ACMDSP_VEParametersT.VoiceControl defines board specific configuration:
	bit 0 :  Tx EC
	bit 1 :  Tx Dual-Mic
	bit 2 :  Tx NS
	bit 3 :  TX EQ
	bit 4 :  TX AVC
	bit 5 :  TX Volume
	bit 6 :  reserved
	bit 7 :  reserved
	bit 8 :  Rx NS
	bit 9 :  Rx EQ
	bit 10: Rx Boost
	bit 11: Rx BiQuadIIR
	bit 12: Rx Rx Volume
	bit 13: Rx sidetone
	bit 14: reserved
	bit 15: reserved
*********************************************************************************************************/	

//ICAT EXPORTED ENUM
typedef enum
{
	ACMDSP_CONTROL_BITS_TX_EC		=  0,
	ACMDSP_CONTROL_BITS_TX_DUALMIC	=  1,
	ACMDSP_CONTROL_BITS_TX_NS		=  2,
	ACMDSP_CONTROL_BITS_TX_EQ		=  3,
	ACMDSP_CONTROL_BITS_TX_AVC		=  4,
	ACMDSP_CONTROL_BITS_TX_VOLUME	=  5,
	ACMDSP_CONTROL_BITS_RESERVED6	=  6,
	ACMDSP_CONTROL_BITS_RESERVED7	=  7,

	ACMDSP_CONTROL_BITS_RX_NS		=  8,
	ACMDSP_CONTROL_BITS_RX_EQ		=  9,
	ACMDSP_CONTROL_BITS_RX_BOOST	= 10,
	ACMDSP_CONTROL_BITS_RX_BIQUADIIR= 11,
	ACMDSP_CONTROL_BITS_RX_VOLUME	= 12,
	ACMDSP_CONTROL_BITS_RX_SIDETONE	= 13,
	ACMDSP_CONTROL_BITS_RX_SLOWVOICE= 14,
	ACMDSP_CONTROL_BITS_MP3_MODE	= 15,

	ACMDSP_CONTROL_BITS_CNT 		= 16	//16 control bits	
} ACMDSP_CONTROL_BITS;


//#ifdef CRANEL_8MRAM
#if defined(CRANEL_4MRAM) || defined(CRANEL_8MRAM) || defined(CRANEGT_DDR) || defined(CRANELS_SUPPORT)

//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_PGA_STAGE1_M6dB = 0,	
	ACMCODEC_PGA_STAGE1_M3dB,
	ACMCODEC_PGA_STAGE1_0dB,
	ACMCODEC_PGA_STAGE1_3dB,
	ACMCODEC_PGA_STAGE1_6dB,
	ACMCODEC_PGA_STAGE1_9dB,
	ACMCODEC_PGA_STAGE1_12dB,
	ACMCODEC_PGA_STAGE1_15dB,
	ACMCODEC_PGA_STAGE1_18dB,
	ACMCODEC_PGA_STAGE1_21dB,

	ACMCODEC_PGA_STAGE1_CNT,

	ACMCODEC_PGA_STAGE1_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_PGA_STAGE1;
    
#else 


//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_PGA_STAGE1_0dB = 0,	
	ACMCODEC_PGA_STAGE1_6dB,
	ACMCODEC_PGA_STAGE1_12dB,
	ACMCODEC_PGA_STAGE1_18dB,
	ACMCODEC_PGA_STAGE1_EXT_24dB = 7,/*0b111, only for cranG(3603) and craneM(1603)*/

	ACMCODEC_PGA_STAGE1_CNT,

	ACMCODEC_PGA_STAGE1_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_PGA_STAGE1;
#endif 



//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_PGA_STAGE2_M6dB = 0,	
	ACMCODEC_PGA_STAGE2_M3dB,
	ACMCODEC_PGA_STAGE2_0dB,
	ACMCODEC_PGA_STAGE2_3dB,
	ACMCODEC_PGA_STAGE2_6dB,

	ACMCODEC_PGA_STAGE2_CNT,

	ACMCODEC_PGA_STAGE2_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_PGA_STAGE2;


//ICAT EXPORTED ENUM
typedef enum
{//0~31, RSNUM = 4
	ACMCODEC_ADC_DIGGAIN_BYPASS =  0,		//0
	ACMCODEC_ADC_DIGGAIN_M24dB	=  1,
	ACMCODEC_ADC_DIGGAIN_M18dB	=  2,
	ACMCODEC_ADC_DIGGAIN_M14dB	=  3,
	ACMCODEC_ADC_DIGGAIN_M12dB	=  4,
	ACMCODEC_ADC_DIGGAIN_M10dB	=  5,
	ACMCODEC_ADC_DIGGAIN_M8dB	=  6,
	ACMCODEC_ADC_DIGGAIN_M7dB	=  7,
	ACMCODEC_ADC_DIGGAIN_M6dB	=  8,
	ACMCODEC_ADC_DIGGAIN_M5dB	=  9,
	ACMCODEC_ADC_DIGGAIN_M4dB	= 10,
	ACMCODEC_ADC_DIGGAIN_M3dB	= 11,
	ACMCODEC_ADC_DIGGAIN_M2dB	= 12,
	ACMCODEC_ADC_DIGGAIN_M1dB 	= 14,
	ACMCODEC_ADC_DIGGAIN_0dB	= 16,
	ACMCODEC_ADC_DIGGAIN_1dB	= 18,
	ACMCODEC_ADC_DIGGAIN_2dB	= 20,
	ACMCODEC_ADC_DIGGAIN_3dB	= 23,
	ACMCODEC_ADC_DIGGAIN_4dB	= 25,
	ACMCODEC_ADC_DIGGAIN_5dB	= 28,
	ACMCODEC_ADC_DIGGAIN_6dB	= 31,
	
	ACMCODEC_ADC_DIGGAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_ADC_DIGGAIN;


//ICAT EXPORTED ENUM
typedef enum
{//0~255, RSNUM = 4
	ACMCODEC_DAC_DIGGAIN_BYPASS =   0,		//0
	ACMCODEC_DAC_DIGGAIN_M24dB	=   1,
	ACMCODEC_DAC_DIGGAIN_M18dB	=   2,
	ACMCODEC_DAC_DIGGAIN_M14dB	=   3,
	ACMCODEC_DAC_DIGGAIN_M12dB	=   4,
	ACMCODEC_DAC_DIGGAIN_M10dB	=   5,
	ACMCODEC_DAC_DIGGAIN_M8dB	=   6,
	ACMCODEC_DAC_DIGGAIN_M7dB	=   7,
	ACMCODEC_DAC_DIGGAIN_M6dB	=   8,
	ACMCODEC_DAC_DIGGAIN_M5dB	=   9,
	ACMCODEC_DAC_DIGGAIN_M4dB	=  10,
	ACMCODEC_DAC_DIGGAIN_M3dB	=  11,
	ACMCODEC_DAC_DIGGAIN_M2dB	=  12,
	ACMCODEC_DAC_DIGGAIN_M1dB 	=  14,
	ACMCODEC_DAC_DIGGAIN_0dB	=  16,
	ACMCODEC_DAC_DIGGAIN_1dB	=  18,
	ACMCODEC_DAC_DIGGAIN_2dB	=  20,
	ACMCODEC_DAC_DIGGAIN_3dB	=  23,
	ACMCODEC_DAC_DIGGAIN_4dB	=  25,
	ACMCODEC_DAC_DIGGAIN_5dB	=  28,
	ACMCODEC_DAC_DIGGAIN_6dB	=  32,
	ACMCODEC_DAC_DIGGAIN_7dB	=  36,
	ACMCODEC_DAC_DIGGAIN_8dB	=  40,
	ACMCODEC_DAC_DIGGAIN_9dB	=  45,
	ACMCODEC_DAC_DIGGAIN_10dB	=  51,
	ACMCODEC_DAC_DIGGAIN_11dB	=  57,
	ACMCODEC_DAC_DIGGAIN_12dB	=  64,
	ACMCODEC_DAC_DIGGAIN_13dB	=  71,
	ACMCODEC_DAC_DIGGAIN_14dB	=  80,
	ACMCODEC_DAC_DIGGAIN_15dB	=  90,
	ACMCODEC_DAC_DIGGAIN_16dB	= 101,
	ACMCODEC_DAC_DIGGAIN_17dB	= 113,
	ACMCODEC_DAC_DIGGAIN_18dB	= 127,
	ACMCODEC_DAC_DIGGAIN_19dB	= 143,
	ACMCODEC_DAC_DIGGAIN_20dB	= 160,
	ACMCODEC_DAC_DIGGAIN_21dB	= 180,
	ACMCODEC_DAC_DIGGAIN_22dB	= 201,
	ACMCODEC_DAC_DIGGAIN_23dB	= 226,	
	ACMCODEC_DAC_DIGGAIN_24dB	= 254,
	
	ACMCODEC_DAC_DIGGAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_DAC_DIGGAIN;


//ICAT EXPORTED ENUM
typedef enum
{//0~255, RSNUM = 8
	ACMCODEC_SIDETONE_GAIN_BYPASS 	=   0,		//0
	ACMCODEC_SIDETONE_GAIN_M48dB	=   1,
	ACMCODEC_SIDETONE_GAIN_M42dB	=   2,
	ACMCODEC_SIDETONE_GAIN_M38dB	=   3,
	ACMCODEC_SIDETONE_GAIN_M36dB	=   4,
	ACMCODEC_SIDETONE_GAIN_M34dB	=   5,
	ACMCODEC_SIDETONE_GAIN_M32dB	=   6,
	ACMCODEC_SIDETONE_GAIN_M30dB	=   8,
	ACMCODEC_SIDETONE_GAIN_M28dB	=  10,
	ACMCODEC_SIDETONE_GAIN_M26dB	=  13,
	ACMCODEC_SIDETONE_GAIN_M24dB	=  16,
	ACMCODEC_SIDETONE_GAIN_M22dB	=  20,
	ACMCODEC_SIDETONE_GAIN_M20dB	=  26,
	ACMCODEC_SIDETONE_GAIN_M18dB 	=  32,
	ACMCODEC_SIDETONE_GAIN_M16dB	=  41,
	ACMCODEC_SIDETONE_GAIN_M14dB	=  51,
	ACMCODEC_SIDETONE_GAIN_M12dB	=  64,
	ACMCODEC_SIDETONE_GAIN_M10dB	=  81,
	ACMCODEC_SIDETONE_GAIN_M9dB		=  91,
	ACMCODEC_SIDETONE_GAIN_M8dB		= 102,
	ACMCODEC_SIDETONE_GAIN_M7dB		= 114,	
	ACMCODEC_SIDETONE_GAIN_M6dB		= 128,
	ACMCODEC_SIDETONE_GAIN_M5dB		= 144,
	ACMCODEC_SIDETONE_GAIN_M4dB		= 161,
	ACMCODEC_SIDETONE_GAIN_M3dB		= 181,
	ACMCODEC_SIDETONE_GAIN_M2dB		= 203,
	ACMCODEC_SIDETONE_GAIN_M1dB		= 228,
	ACMCODEC_SIDETONE_GAIN_0dB		= 255,
	
	ACMCODEC_SIDETONE_GAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_SIDETONE_GAIN;


//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_DAC_GAIN_6dB 	= 0,	
	ACMCODEC_DAC_GAIN_3dB,
	ACMCODEC_DAC_GAIN_0dB,
	ACMCODEC_DAC_GAIN_M3dB,
	ACMCODEC_DAC_GAIN_M6dB,

	ACMCODEC_DAC_GAIN_CNT,

	ACMCODEC_DAC_GAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_DAC_GAIN;


//ICAT EXPORTED ENUM
typedef enum
{
	//ACMCODEC_RCV_GAIN_NOTUSED  	= 0,
	//ACMCODEC_RCV_GAIN_M6dB 		= 1,	
	ACMCODEC_RCV_GAIN_M6dB 		= 0,	
	ACMCODEC_RCV_GAIN_M3dB,
	ACMCODEC_RCV_GAIN_0dB,
	ACMCODEC_RCV_GAIN_3dB,
	ACMCODEC_RCV_GAIN_6dB,

	ACMCODEC_RCV_GAIN_CNT,

	ACMCODEC_RCV_GAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_RCV_GAIN;


//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_CLASSG_MODE_0_9V = 0,
	ACMCODEC_CLASSG_MODE_1_8V,

	ACMCODEC_CLASSG_MODE_CNT,

	ACMCODEC_CLASSG_MODE_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_CLASSG_MODE;


//ICAT EXPORTED ENUM
typedef enum
{
	ACMCODEC_CLASSD_GAIN_1_5VV = 0,
	ACMCODEC_CLASSD_GAIN_2_0VV,
	ACMCODEC_CLASSD_GAIN_2_3VV,

	ACMCODEC_CLASSD_GAIN_CNT,

	ACMCODEC_CLASSD_GAIN_ENUM_16_BIT = 0x7FFF //16bit enum compiling enforcement    
} ACMCODEC_CLASSD_GAIN;



//ICAT EXPORTED STRUCT
typedef struct
{
    ACMCODEC_PGA_STAGE1    	PGA_Stage1;        	//00:0dB;01:6dB;10:12dB;11:18dB
    ACMCODEC_PGA_STAGE2    	PGA_Stage2;        	//000:-6dB;001:-2.5dB;010:0dB;011:3.5dB;100:6dB
    ACMCODEC_ADC_DIGGAIN	ADC_DigGain;       	//AdcDigGain[4:0]:1~31
    ACMCODEC_SIDETONE_GAIN	SideTone_Gain;  	//SideToneGain[8:0] :[-256,255]
    signed short    		Reserved1;  		//Reserved1
    signed short    		Reserved2;  		//Reserved2
} ACMCodec_GainInT;

//ICAT EXPORTED STRUCT
typedef struct
{
    ACMCODEC_DAC_DIGGAIN    DAC_DigGain;   		//DacDigGain[7:0]:1~255
    ACMCODEC_DAC_GAIN    	DAC_Gain;          	//000:6dB;001:3.5dB;010:0dB;011:-2.5dB;100:-6dB
    ACMCODEC_RCV_GAIN    	RCV_Gain;          	//001:-6dB;010:-2.5dB;011:0dB;100:3.5dB;101:6dB
    ACMCODEC_CLASSG_MODE    ClassG_Mode;		//0=+/-0.9V; 1=+/-1.8V
	ACMCODEC_CLASSD_GAIN    ClassD_Gain;		//0=1.5V/V;1=2V/V;2=2.33V/V;3=0
    signed short    		Reserved1;  		//Reserved1
} ACMCodec_GainOutT;


//ICAT EXPORTED STRUCT
typedef struct
{
    ACM_PROFILE_ID    Profile;
	unsigned short	  CodecControl;
    ACMCodec_GainInT  Tx_CodecGain;	    //tx has same gain for every volume
    signed short      Tx_DSPGain; 	    //dB, tx has same gain for every volume
    ACMCodec_GainOutT Rx_CodecGain[ACM_VOLUME_CNT];
    signed short	  Rx_DSPGain[ACM_VOLUME_CNT];	//dB
    signed short      Rx_DSPSideToneGain;			//dB
} ACMCodec_GainT;

//ICAT EXPORTED STRUCT
typedef struct
{
    signed short    SampleDelay; 		// reference delay to echo
    signed short    NumOfTaps;			// Fir filter length
    signed short    ConvergenceSpeed;   // slowdown
    signed short    Ref2EchoPowerRatio;	// ratio of ref to echo, [-12 ~30]db, decimal number
    signed short    DC_factor;          //[14000 ~16350]  but default should be 16200
    signed short    reserved0;	
} ACMDSP_ECParamsT;

//ICAT EXPORTED STRUCT
typedef struct
{
	signed short    BoostMode;          // 0 , 1, 2, 3
	signed short    BoostGainDb; 
	signed short    PeakGainDb;
	signed short    BoostNoiseGainDb;
	signed short    NoiseLevel;
	signed short    BoostAttach;
	signed short    BoostRelease;
	signed short    reserved;
	signed short    TargetLevel;
	signed short    AgcMaxGainDb;
	signed short    AgcMaxRMS;         
	signed short    AgcEnvStep;         //expert parameters
	signed short    RMStrackStep;       //expert parameters
} ACMDSP_AGCParamsT;

//ICAT EXPORTED STRUCT
typedef struct 
{
    signed short a[3];
    signed short b[3];
} ACMDSP_BiQuadCoeffsT;


//ICAT EXPORTED STRUCT
typedef struct 
{
    unsigned short 	PeriodDelta;
	unsigned short 	ExpandRate;
	unsigned short 	CorrectionThreshold;
    signed short 	reserved;
} ACMDSP_SlowVoiceParamsT;


//ICAT EXPORTED STRUCT
typedef struct {
	 signed short   MaxSuppressDb;	    // [-40,0)db.
	 signed short   PeakNoiseDb;	    // [-70,-12)db.
	 signed short   NoiseMatrix;	    // following is expert parameters
	 signed short   FrameSmoothFactor;
	 signed short   reserved;
} ACMDSP_NSParamsT; //noise suppression para, for both TX&RX

//ICAT EXPORTED STRUCT
typedef struct {// res parameters
	 signed short   ResMode;
	 signed short   DtGainDb;
	 signed short   FestGainDb;
	 signed short   NoiseFloorDbov;
	 signed short   CnGaindb;   
	 signed short   DtThresh;
	 signed short   Gamma;
	 //following is expert paramters
	 signed short   AuditoryMasking;
	 signed short   NonlinearModel;
	 signed short   WinitCntThresh;
	 signed short   EstResMatrix;
	 signed short   VadhangRef;
	 signed short   VadhangNear;
	 signed short   reserved;
} ACMDSP_NSResParamsT;


//ICAT EXPORTED STRUCT
typedef struct {// reserved  parameters for dual_mic
	 signed short  TapsNum; 
	 signed short  Oversuppression;
	 signed short  OverAuditoryMasking;
	 signed short  DualMicVmThresh;
	 signed short  DualMicVmOffset;
	 signed short  DualMicNoiseThresh;
	 signed short  DualMicVadHang;
	 signed short  DualMicVmRelease;
	 signed short  NonStationaryLeftDb;
} ACMDSP_NSDmParamsT;


//ICAT EXPORTED STRUCT
typedef struct {
	 ACMDSP_NSParamsT	 NsParams;
	 ACMDSP_NSResParamsT ResParams;
	 ACMDSP_NSDmParamsT  DmParams;
} ACMDSP_NSTxParamsT;       //TX only

//CATStudio could not support union definition which has bit filed and short types
#if 0
//ICAT EXPORTED STRUCT
typedef struct {
    BOOL Tx_EC_On:1;		    //bit 0
    BOOL Tx_DualMic_On:1;	//bit 1
    BOOL Tx_NS_On:1;		    //bit 2
    BOOL Tx_EQ_On:1;		    //bit 3
    BOOL Tx_AVC_On:1;		//bit 4
    BOOL Tx_Volume_On:1;	    //bit 5
    BOOL Reserved6:1;		//bit 6
    BOOL Reserved7:1;		//bit 7

    BOOL Rx_NS_On:1;		    //bit 8
    BOOL Rx_EQ_On:1;		    //bit 9
    BOOL Rx_Boost_On:1;	    //bit 10
    BOOL Rx_BiQuadIIR_On:1;	//bit 11
    BOOL Rx_Volume_On:1;	    //bit 12
    BOOL Rx_Sidetone_On:1;	//bit 13
    BOOL Reserved14:1;		//bit 14
    BOOL Reserved15:1;		//bit 15   
} ACMDSP_VCBitsT;


//ICAT EXPORTED UNION
typedef union {
  	ACMDSP_VCBitsT Bits;
  	unsigned short VC;
} ACMDSP_VoiceControlT; //TX only
#endif



//ICAT EXPORTED STRUCT
typedef struct
{    
    ACM_PROFILE_ID		    Profile;
    signed short          	Version;
    unsigned short 			VoiceControl;

//TX path
    signed short		    ExpertParam1[ACM_RATE_CNT];
    ACMDSP_ECParamsT        EcParams[ACM_RATE_CNT];
    ACMDSP_NSTxParamsT      NsParamsTx[ACM_RATE_CNT];
    signed short		    TxFreq_eqCoeffArray[ACM_RATE_CNT][24];
    ACMDSP_AGCParamsT       TxAgcParams[ACM_RATE_CNT];

//RX path
    signed short 		    ExpertParam2[ACM_RATE_CNT];
    ACMDSP_NSParamsT        NsParamsRx[ACM_RATE_CNT];
    signed short	        RxFreq_eqCoeffArray[ACM_RATE_CNT][24];
    ACMDSP_AGCParamsT 	    RxAgcParams[ACM_RATE_CNT];
    ACMDSP_BiQuadCoeffsT	BiQuadIIR[ACM_RATE_CNT]; //can be used as hpf/lpf or notch filter
    ACMDSP_SlowVoiceParamsT SlowVoiceParam;

//Reserved    
    unsigned short	        Reserved[6];
	unsigned short			Tag;		//Fixed as 0xBEEF
} ACMDSP_VEParametersT;


//ICAT EXPORTED STRUCT
typedef struct
{    
    unsigned short  drcGain;
    unsigned short  cutOffFreqIdx;
    unsigned short  resv1;
    unsigned short  resv2;
    
} ACMCP_MediaDRCParaT;

//ICAT EXPORTED STRUCT
typedef struct
{    
    unsigned short  maxRms;
    unsigned short  Alpha;
    unsigned short  N2cos;
    unsigned short resv;
    
} ACMCP_SPKProParaT;

//ICAT EXPORTED STRUCT
typedef struct
{    
    unsigned int    index;
    unsigned int    onoffCtl; // bit0:eq,  bit1: drc, bit2: spkPro
    EqParamIn       eqParam;
    ACMCP_MediaDRCParaT drcParam;
    ACMCP_SPKProParaT spkProParam;
    unsigned short	        Reserved[7];
	unsigned short			Tag;		//Fixed as 0xBEEF
} ACMCP_MediaVEarametersT;

int ACMCalibration_GetVEParam(unsigned short profile, ACMDSP_VEParametersT *out_params);
int ACMCalibration_GetMediaVEParam(unsigned int index,  ACMCP_MediaVEarametersT* out_param);
int ACMCalibration_GetCodecGainParam(unsigned short profile, ACMCodec_GainT *out_params);
void  ACMCalibration_ConfigVE(short *pData);
void  ACMCalibration_ConfigMediaVE(short *pData);
void  ACMCalibration_ConfigCodec(short *pData);


#ifdef __cplusplus
}
#endif

#endif //_ACMCALIBRATION_H_
