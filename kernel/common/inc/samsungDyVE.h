
#ifndef	SamsungDyVE_H
#define SamsungDyVE_H

/****************************************************************************************
 *  Dyve Mic Noise Level
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
//NOISE_LEVEL_M18DB		=	0xffffffee,//		-18,
NOISE_LEVEL_M18DB		=	-18,//		-18,
NOISE_LEVEL_M17DB		,              //		-17,
NOISE_LEVEL_M16DB		,              //		-16,
NOISE_LEVEL_M15DB		,              //		-15,
NOISE_LEVEL_M14DB		,              //		-14,
NOISE_LEVEL_M13DB		,              //		-13,
NOISE_LEVEL_M12DB		,              //		-12,
NOISE_LEVEL_M11DB		,              //		-11,
NOISE_LEVEL_M10DB		,              //		-10,
NOISE_LEVEL_M09DB		,              //		-9 ,
NOISE_LEVEL_M08DB		,              //		-8 ,
NOISE_LEVEL_M07DB		,              //		-7 ,
NOISE_LEVEL_M06DB		,              //		-6 ,
NOISE_LEVEL_M05DB		,              //		-5 ,
NOISE_LEVEL_M04DB		,              //		-4 ,
NOISE_LEVEL_M03DB		,              //		-3 ,
NOISE_LEVEL_M02DB		,              //		-2 ,
NOISE_LEVEL_M01DB		,              //		-1 ,
NOISE_LEVEL_0DB			=			0  ,
NOISE_LEVEL_01DB		=			1  ,
NOISE_LEVEL_02DB		=			2  ,
NOISE_LEVEL_03DB		=			3  ,

DIAMONDVOICE_NOISE_LEVEL_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 
}DiamondVoice_dyveMicGain;

/****************************************************************************************
 *  DyVE  Mic Sensitivity Level
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
MIC_SENSITIVITY_TEST	=			9,
MIC_SENSITIVITY_8		=			8,
MIC_SENSITIVITY_7		=			7,
MIC_SENSITIVITY_6		=			6,
MIC_SENSITIVITY_5		=			5,		// i9
MIC_SENSITIVITY_4		=			4,
MIC_SENSITIVITY_3		=			3,		// i7
MIC_SENSITIVITY_2		=			2,
MIC_SENSITIVITY_1		=			1,
MIC_SENSITIVITY_0		=			0,

DIAMONDVOICE_MIC_SENSITIVITY_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 
}DiamondVoice_dyveMicSensitivity;

/****************************************************************************************
 *  DyVE Max Volume Boost Level
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	   
MAXGAIN_0dB				=		     0,		//  0dB MAXGAIN
MAXGAIN_1dB				=		     1,		//  1dB MAXGAIN
MAXGAIN_2dB				=		     2,		//  2dB MAXGAIN
MAXGAIN_3dB				=		     3,		//  3dB MAXGAIN
MAXGAIN_4dB				=		     4,		//  4dB MAXGAIN
MAXGAIN_5dB				=		     5,		//  5dB MAXGAIN
MAXGAIN_6dB				=		     6,		//  6dB MAXGAIN
MAXGAIN_7dB				=		     7,		//  7dB MAXGAIN
MAXGAIN_8dB				=		     8,		//  8dB MAXGAIN
MAXGAIN_9dB				=		     9,		//  9dB MAXGAIN
MAXGAIN_10dB			=			10,		//  10dB MAXGAIN
MAXGAIN_11dB			=			11,		//  11dB MAXGAIN
MAXGAIN_12dB			=			12,		//  12dB MAXGAIN
MAXGAIN_13dB			=			13,		//  13dB MAXGAIN
MAXGAIN_14dB			=			14,		//  14dB MAXGAIN
MAXGAIN_15dB			=			15,		//  15dB MAXGAIN
MAXGAIN_16dB			=			16,		//  16dB MAXGAIN
MAXGAIN_17dB			=			17,		//  17dB MAXGAIN
MAXGAIN_18dB			=			18,		//  18dB MAXGAIN
MAXGAIN_19dB			=			19,		//  19dB MAXGAIN
MAXGAIN_20dB			=			20,		//  20dB MAXGAIN
MAXGAIN_21dB			=			21,		//  21dB MAXGAIN
MAXGAIN_22dB			=			22,		//  22dB MAXGAIN

DIAMONDVOICE_MAXGAIN_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 
}DiamondVoice_dyveMaxVolBoosLev;


/****************************************************************************************
 *  DyVE Clarity Level
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	  
CLARITY_LEVEL_OFF			=		0,		//  Clarity Level Off
CLARITY_LEVEL_LOW			=		4,		//  Clarity Level Low
CLARITY_LEVEL_MEDIUM		=		6,		//  Clarity Level Medium
CLARITY_LEVEL_HIGH			=	    9,		//  Clarity Level High
DIAMONDVOICE_CLARITY_MAXGAIN_ENUM_32_BIT	= 0x7FFFFFFF //32bit enum compiling enforcement 
}DiamondVoice_dyvePreemphasis;

#define	NUMVOLUME							11
#define 	GAINSTEP							19
#define 	COEFFNUM							5

#define	DyVEVOLUMESTEP					3

#define	SAMSUNGDyVE_TESTMODE			2
#define	SAMSUNGDyVE_ENABLE				1
#define	SAMSUNGDyVE_DISABLE				0

/******************************************************************************
 *  TYPEDEF STRUCT
 *****************************************************************************/
typedef struct{
short monitor1;				// Tx noise RMS
short monitor2;				// Rx Gain
short monitor3;				// Shift Value
short monitor4;				// Mic Sensitivity Level
short monitor5;				// Volume Step ( 0 ~ Max step)
} DyVEState_type;

//ICAT EXPORTED STRUCT
typedef struct{
short Step11Gain;
short Step10Gain;
short Step09Gain;
short Step08Gain;
short Step07Gain;
short Step06Gain;
short Step05Gain;
short Step04Gain;
short Step03Gain;
short Step02Gain;
short Step01Gain;
} DyVEStepGain_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungDyVEState(void);

#endif		// SamsungDyVE_H
