
#ifndef	SamsungASF_H
#define SamsungASF_H

/****************************************************************************************
 *  Max Level Table
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	
ASF_PCMMAXLEVEL_0DB		=	    0 ,	// 100%
ASF_PCMMAXLEVEL_M1DB	=		1 ,	// 89%
ASF_PCMMAXLEVEL_M2DB	=		2 ,	// 79%
ASF_PCMMAXLEVEL_M3DB	=		3 ,	// 71%
ASF_PCMMAXLEVEL_M4DB	=		4 ,	// 63%
ASF_PCMMAXLEVEL_M5DB	=		5 ,	// 56%
ASF_PCMMAXLEVEL_M6DB	=		6 ,	// 50%
ASF_PCMMAXLEVEL_M7DB	=		7 ,	// 45%
ASF_PCMMAXLEVEL_M8DB	=		8 ,	// 40%
ASF_PCMMAXLEVEL_M9DB	=		9 ,	// 35%
ASF_PCMMAXLEVEL_M10DB	=		10,	// 32%
ASF_PCMMAXLEVEL_M11DB	=		11,	// 28%
ASF_PCMMAXLEVEL_M12DB	=		12,	// 25%
ASF_PCMMAXLEVEL_M13DB	=		13,	// 25%
ASF_PCMMAXLEVEL_M14DB	=		14,	// 25%
ASF_PCMMAXLEVEL_M15DB	=		15,	// 25%
ASF_PCMMAXLEVEL_M16DB	=		16,	// 25%
ASF_PCMMAXLEVEL_M17DB	=		17,	// 25%
ASF_PCMMAXLEVEL_M18DB	=		18,	// 25%
ASF_PCMMAXLEVEL_M19DB	=		19,	// 25%
ASF_PCMMAXLEVEL_M20DB	=		20,	// 25%
ASF_PCMMAXLEVEL_M21DB	=		21,	// 25%
ASF_PCMMAXLEVEL_M22DB	=		22,	// 25%
ASF_PCMMAXLEVEL_M23DB	=		23,	// 25%
ASF_PCMMAXLEVEL_M24DB	=		24,	// 25%
ASF_PCMMAXLEVEL_M25DB	=		25,	// 25%
ASF_PCMMAXLEVEL_M26DB	=		26,	// 25%
ASF_PCMMAXLEVEL_M27DB	=		27,	// 25%
ASF_PCMMAXLEVEL_M28DB	=		28,	// 25%
ASF_PCMMAXLEVEL_M29DB	=		29,	// 25%
ASF_PCMMAXLEVEL_M30DB	=		30,	// 25%

DIAMONDVOICE_ASF_PCMMAXLEVEL_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_asfLimiter;

/******************************************************************************
 *  FILTER MODE
 *****************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{ 
ASF_FILTER_BYPASS		=		0,	// Bypass
ASF_FILTER_MODE1		=		1,	// A-weigth
ASF_FILTER_MODE2		=		2,	// mode2
ASF_FILTER_MODE3		=		3,	// mode3
ASF_FILTER_MODE4		=		4,	// reserved
ASF_FILTER_MODE5		=		5,	// reserved
ASF_FILTER_MODE6		=		6,	// reserved
ASF_FILTER_MODE7		=		7,	// reserved
ASF_FILTER_MODE8		=		8,	// reserved

DIAMONDVOICE_ASF_FILTER_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_asfFilterMode;         

/******************************************************************************
 *  STRUCTURES
 *****************************************************************************/
typedef struct{
	short monitor1;			// ASF Filter Mode
	short monitor2;			// ASF Limit Level
	short monitor3;
	short monitor4;
	short monitor5;
} ASFState_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungASFState(void);

#endif		// SamsungASF_H
