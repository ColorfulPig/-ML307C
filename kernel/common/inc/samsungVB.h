
#ifndef	SamsungVoiceBooster_H
#define SamsungVoiceBooster_H

/****************************************************************************************
 *  Max Level Table
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	  
VB_PCMMAXLEVEL_0DB		=	     0, 	// 100%
VB_PCMMAXLEVEL_M1DB		=	     1, 	// 89%
VB_PCMMAXLEVEL_M2DB		=	     2, 	// 79%
VB_PCMMAXLEVEL_M3DB		=	     3, 	// 71%
VB_PCMMAXLEVEL_M4DB		=	     4, 	// 63%
VB_PCMMAXLEVEL_M5DB		=	     5, 	// 56%
VB_PCMMAXLEVEL_M6DB		=	     6, 	// 50%
VB_PCMMAXLEVEL_M7DB		=	     7, 	// 45%
VB_PCMMAXLEVEL_M8DB		=	     8, 	// 40%
VB_PCMMAXLEVEL_M9DB		=	     9, 	// 35%
VB_PCMMAXLEVEL_M10DB	=		10, 	// 32%
VB_PCMMAXLEVEL_M11DB	=		11, 	// 28%
VB_PCMMAXLEVEL_M12DB	=		12, 	// 25%
VB_PCMMAXLEVEL_M13DB	=		13, 	// 25%
VB_PCMMAXLEVEL_M14DB	=		14, 	// 25%
VB_PCMMAXLEVEL_M15DB	=		15, 	// 25%
VB_PCMMAXLEVEL_M16DB	=		16, 	// 25%
VB_PCMMAXLEVEL_M17DB	=		17, 	// 25%
VB_PCMMAXLEVEL_M18DB	=		18, 	// 25%
VB_PCMMAXLEVEL_M19DB	=		19, 	// 25%
VB_PCMMAXLEVEL_M20DB	=		20, 	// 25%
VB_PCMMAXLEVEL_M21DB	=		21, 	// 25%
VB_PCMMAXLEVEL_M22DB	=		22, 	// 25%
VB_PCMMAXLEVEL_M23DB	=		23, 	// 25%
VB_PCMMAXLEVEL_M24DB	=		24, 	// 25%
VB_PCMMAXLEVEL_M25DB	=		25, 	// 25%
VB_PCMMAXLEVEL_M26DB	=		26, 	// 25%
VB_PCMMAXLEVEL_M27DB	=		27, 	// 25%
VB_PCMMAXLEVEL_M28DB	=		28, 	// 25%
VB_PCMMAXLEVEL_M29DB	=		29, 	// 25%
VB_PCMMAXLEVEL_M30DB	=		30, 	// 25%

DIAMONDVOICE_VB_PCMMAXLEVEL_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_vbLimiter;

/****************************************************************************************
 *  Gain Table
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
VB_GAIN_0DB			=			0 ,	//  0dB
VB_GAIN_1DB			=			1 ,	//  1 dB
VB_GAIN_2DB			=			2 ,	//  2 dB
VB_GAIN_3DB			=			3 ,	//  3 dB
VB_GAIN_4DB			=			4 ,	//  4 dB
VB_GAIN_5DB			=			5 ,	//  5 dB
VB_GAIN_6DB			=			6 ,	//  6 dB
VB_GAIN_7DB			=			7 ,	//  7 dB
VB_GAIN_8DB			=			8 ,	//  8 dB
VB_GAIN_9DB			=			9 ,	//  9 dB
VB_GAIN_10DB		=			10,	// 10 dB
VB_GAIN_11DB		=			11,	// 11 dB
VB_GAIN_12DB		=			12,	// 12 dB
VB_GAIN_13DB		=			13,	// 13 dB
VB_GAIN_14DB		=			14,	// 14 dB
VB_GAIN_15DB		=			15,	// 15 dB
VB_GAIN_16DB		=			16,	// 16 dB
VB_GAIN_17DB		=			17,	// 17 dB
VB_GAIN_18DB		=			18,	// 18 dB

DIAMONDVOICE_VB_GAIN_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_vbGain;

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
VB_HPFCUTOFF_BYPASS		=	0,	// Bypass
VB_HPFCUTOFF_100HZ		=	1,	// 100Hz
VB_HPFCUTOFF_200HZ		=	2,	// 200Hz
VB_HPFCUTOFF_300HZ		=	3,	// 300Hz
VB_HPFCUTOFF_400HZ		=	4,	// 400Hz
VB_HPFCUTOFF_500HZ		=	5,	// 500Hz
VB_HPFCUTOFF_600HZ		=	6,	// 600Hz
VB_HPFCUTOFF_700HZ		=	7,	// 700Hz
VB_HPFCUTOFF_800HZ		=	8,	// 800Hz

DIAMONDVOICE_VB_HPFCUTOFF_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_vbHpfCutOff;

//ICAT EXPORTED ENUM
typedef enum
{	 
VB_HARMONIC_OFF				=   0,
VB_HARMONIC_MINLEVEL		=	8192,	    // 25 %
VB_HARMONIC_MEDLEVEL		=	16384,		// 50 %
VB_HARMONIC_MAXLEVEL		=	32767,		// 100 %

DIAMONDVOICE_VB_HARMONIC_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_vbHarmonic;

#define VB_FRAME_LENGTH				320
#define VB_FIRFILTERNUM					13

/******************************************************************************
 *  STRUCTURES
 *****************************************************************************/
 typedef struct{
	short monitor1;
	short monitor2;
	short monitor3;
	short monitor4;
	short monitor5;
} VBState_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungVBState(void);

#endif		// SamsungVoiceBooster_H
