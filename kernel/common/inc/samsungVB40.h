#ifndef	SamsungVoiceBooster40_H
#define SamsungVoiceBooster40_H

/****************************************************************************************
 *  Compress & Limiter Threshold
 ****************************************************************************************/
 //ICAT EXPORTED ENUM
typedef enum
{
  VB40_PCMMAXLEVEL_0DB	=			0	,
  VB40_PCMMAXLEVEL_M1DB	=			1	,
  VB40_PCMMAXLEVEL_M2DB	=			2	,
  VB40_PCMMAXLEVEL_M3DB	=			3	,
  VB40_PCMMAXLEVEL_M4DB	=			4	,
  VB40_PCMMAXLEVEL_M5DB	=			5	,
  VB40_PCMMAXLEVEL_M6DB	=			6	,
  VB40_PCMMAXLEVEL_M7DB	=			7	,
  VB40_PCMMAXLEVEL_M8DB	=			8	,
  VB40_PCMMAXLEVEL_M9DB	=			9	,
  VB40_PCMMAXLEVEL_M10DB	=		10	,
  VB40_PCMMAXLEVEL_M11DB	=		11	,
  VB40_PCMMAXLEVEL_M12DB	=		12	,
  VB40_PCMMAXLEVEL_M13DB	=		13	,
  VB40_PCMMAXLEVEL_M14DB	=		14	,
  VB40_PCMMAXLEVEL_M15DB	=		15	,
  VB40_PCMMAXLEVEL_M16DB	=		16	,
  VB40_PCMMAXLEVEL_M17DB	=		17	,
  VB40_PCMMAXLEVEL_M18DB	=		18	,
  VB40_PCMMAXLEVEL_M19DB	=		19	,
  VB40_PCMMAXLEVEL_M20DB	=		20	,
  VB40_PCMMAXLEVEL_M21DB	=		21	,
  VB40_PCMMAXLEVEL_M22DB	=		22	,
  VB40_PCMMAXLEVEL_M23DB	=		23	,
  VB40_PCMMAXLEVEL_M24DB	=		24	,
  VB40_PCMMAXLEVEL_M25DB	=		25	,
  VB40_PCMMAXLEVEL_M26DB	=		26,
  VB40_PCMMAXLEVEL_M27DB	=		27	,
  VB40_PCMMAXLEVEL_M28DB	=		28	,
  VB40_PCMMAXLEVEL_M29DB	=		29	,
  VB40_PCMMAXLEVEL_M30DB	=		30	,
  VB40_PCMMAXLEVEL_M31DB	=		31	,
  VB40_PCMMAXLEVEL_M32DB	=		32	,
  VB40_PCMMAXLEVEL_M33DB	=		33	,
  VB40_PCMMAXLEVEL_M34DB	=		34	,
  VB40_PCMMAXLEVEL_M35DB	=		35	,
  VB40_PCMMAXLEVEL_M36DB	=		36	,
  VB40_PCMMAXLEVEL_M37DB	=		37	,
  VB40_PCMMAXLEVEL_M38DB	=		38	,
  VB40_PCMMAXLEVEL_M39DB	=		39	,
  VB40_PCMMAXLEVEL_M40DB	=		40	,
  DIAMONDVOICE_VB_40PCMMAXLEVEL_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_VB40_PCMMaxLevel;
/****************************************************************************************
 *  Gain Table
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{
  VB40_GAIN_0DB					=	0	,//  0dB
  VB40_GAIN_1DB					=	1	,//  1 dB
  VB40_GAIN_2DB					=	2,	//  2 dB
  VB40_GAIN_3DB					=	3,	//  3 dB
  VB40_GAIN_4DB					=	4,	//  4 dB
  VB40_GAIN_5DB					=	5,	//  5 dB
  VB40_GAIN_6DB					=	6,	//  6 dB
  VB40_GAIN_7DB					=	7,	//  7 dB
  VB40_GAIN_8DB					=	8,	//  8 dB
  VB40_GAIN_9DB					=	9,	//  9 dB
  VB40_GAIN_10DB				=	10,	// 10 dB
  VB40_GAIN_11DB				=	11	,// 11 dB
  VB40_GAIN_12DB				=	12,	// 12 dB
  VB40_GAIN_13DB				=	13	,// 13 dB
  VB40_GAIN_14DB				=	14,	// 14 dB
  VB40_GAIN_15DB				=	15,	// 15 dB
  VB40_GAIN_16DB				=	16,	// 16 dB
  VB40_GAIN_17DB				=	17,	// 17 dB
  VB40_GAIN_18DB				=	18,	// 18 dB
  DIAMONDVOICE_VB40_GAIN_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_VB40_Gain;
/******************************************************************************
 *  HPF Cut-Off Frequencies
 *****************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{
  VB40_HPFCUTOFF_BYPASS		=		0,	// Bypass
  VB40_HPFCUTOFF_100HZ		=		1,	// 100Hz
  VB40_HPFCUTOFF_200HZ		=		2,	// 200Hz
  VB40_HPFCUTOFF_300HZ		=		3,	// 300Hz
  VB40_HPFCUTOFF_400HZ		=		4,	// 400Hz
  VB40_HPFCUTOFF_500HZ		=		5,	// 500Hz
  VB40_HPFCUTOFF_600HZ		=		6,	// 600Hz
  VB40_HPFCUTOFF_700HZ		=		7,	// 700Hz
  VB40_HPFCUTOFF_800HZ		=		8,	// 800Hz
  DIAMONDVOICE_VB40_HPFCUTOFF_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DIAMONDVOICE_VB40_hpfCutOff;
/******************************************************************************
*  Excursion Frequencies
*****************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{
  VB40_EXCURSION_BYASS		=		0,	// Bypass
  VB40_EXCURSION_600HZ		=		1,	// 600Hz
  VB40_EXCURSION_800HZ		=		2,	// 800Hz
  VB40_EXCURSION_1000HZ		=		3,	// 1000Hz
  VB40_EXCURSION_1200HZ		=		4,	// 1200Hz
  VB40_EXCURSION_1400HZ		=		5,	// 1400Hz
  DIAMONDVOICE_VB40_EXCURSION_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DIAMONDVOICE_VB40_excursion;

#define  VB40_FRAME_LENGTH					320
#define  VB40_FIRFILTERNUM					13

/******************************************************************************
*  STRUCTURES
*****************************************************************************/
//ICAT EXPORTED STRUCT
typedef struct {
	short monitor1;
	short monitor2;
	short monitor3;
	short monitor4;
	short monitor5;
	short monitor6;
	short monitor7;
	short monitor8;
	short monitor9;
} VB40State_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungVB40State(void);

#endif		// SamsungVoiceBooster40_H
