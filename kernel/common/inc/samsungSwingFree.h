
#ifndef	SamsungSwingFree_H
#define SamsungSwingFree_H

/****************************************************************************************
 *  SwingFree NS Rx Vol
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
//SF_VOLUME_M30DB		=		0xffffffe2, //-30,
SF_VOLUME_M30DB		=		-30, //-30,
SF_VOLUME_M29DB		, //-29,
SF_VOLUME_M28DB		, //-28,
SF_VOLUME_M27DB		, //-27,
SF_VOLUME_M26DB		, //-26,
SF_VOLUME_M25DB		, //-25,
SF_VOLUME_M24DB		, //-24,
SF_VOLUME_M23DB		, //-23,
SF_VOLUME_M22DB		, //-22,
SF_VOLUME_M21DB		, //-21,
SF_VOLUME_M20DB		, //-20,
SF_VOLUME_M19DB		, //-19,
SF_VOLUME_M18DB		, //-18,
SF_VOLUME_M17DB		, //-17,
SF_VOLUME_M16DB		, //-16,
SF_VOLUME_M15DB		, //-15,
SF_VOLUME_M14DB		, //-14,
SF_VOLUME_M13DB		, //-13,
SF_VOLUME_M12DB		, //-12,
SF_VOLUME_M11DB		, //-11,
SF_VOLUME_M10DB		, //-10,
SF_VOLUME_M09DB		, //-9 ,
SF_VOLUME_M08DB		, //-8 ,
SF_VOLUME_M07DB		, //-7 ,
SF_VOLUME_M06DB		, //-6 ,
SF_VOLUME_M05DB		, //-5 ,
SF_VOLUME_M04DB		, //-4 ,
SF_VOLUME_M03DB		, //-3 ,
SF_VOLUME_M02DB		, //-2 ,
SF_VOLUME_M01DB		, //-1 ,
SF_VOLUME_0DB		=		0  ,
SF_VOLUME_01DB		=		1  ,
SF_VOLUME_02DB		=		2  ,
SF_VOLUME_03DB		=		3  ,
SF_VOLUME_04DB		=		4  ,
SF_VOLUME_05DB		=		5  ,
SF_VOLUME_06DB		=		6  ,
SF_VOLUME_07DB		=		7  ,
SF_VOLUME_08DB		=		8  ,
SF_VOLUME_09DB		=		9  ,
SF_VOLUME_10DB		=		10 ,
SF_VOLUME_11DB		=		11 ,
SF_VOLUME_12DB		=		12 ,
SF_VOLUME_13DB		=		13 ,
SF_VOLUME_14DB		=		14 ,
SF_VOLUME_15DB		=		15 ,
SF_VOLUME_16DB		=		16 ,
SF_VOLUME_17DB		=		17 ,
SF_VOLUME_18DB		=		18 ,
SF_VOLUME_19DB		=		19 ,
SF_VOLUME_20DB		=		20 ,
SF_VOLUME_21DB		=		21 ,
SF_VOLUME_22DB		=		22 ,
SF_VOLUME_23DB		=		23 ,

DIAMONDVOICE_VOLUME_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
} DiamondVoice_sfVolume;

//ICAT EXPORTED ENUM 
typedef enum
{
SFRXVOLSTEPSELECT			=	3,
}DiamondVoice_swfree;

/****************************************************************************************
 *  SwingFree White Noise Suppression Level
 ****************************************************************************************/

//ICAT EXPORTED ENUM 
typedef enum
{	
SF_WNSLEVEL_1			=	1,		// white noise level (Default)
SF_WNSLEVEL_2			=	2,
DIAMONDVOICE_SF_WHITENOISE_LEVEL_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
} DiamondVoice_sfWNLevel;


/****************************************************************************************
 *  SwingFree AGC Compressor Threshold
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	
SF_COMPRESSOR_THRESHOLD_0DB	=  0,	    // 0dB
SF_COMPRESSOR_THRESHOLD_M01DB   =  1,	    // -1dB 
SF_COMPRESSOR_THRESHOLD_M02DB   =  2,	    // -2dB  
SF_COMPRESSOR_THRESHOLD_M03DB   =  3,		// -3dB 
SF_COMPRESSOR_THRESHOLD_M04DB   =  4,		// -4dB 
SF_COMPRESSOR_THRESHOLD_M05DB   =  5,		// -5dB 
SF_COMPRESSOR_THRESHOLD_M06DB   =  6,		// -6dB 
SF_COMPRESSOR_THRESHOLD_M07DB   =  7,		// -7dB 
SF_COMPRESSOR_THRESHOLD_M08DB   =  8,		// -8dB 
SF_COMPRESSOR_THRESHOLD_M09DB   =  9,		// -9dB 
SF_COMPRESSOR_THRESHOLD_M10DB   =  10,		// -10dB 
SF_COMPRESSOR_THRESHOLD_M11DB   =  11,		// -11dB 
SF_COMPRESSOR_THRESHOLD_M12DB   =  12,		// -12dB 
SF_COMPRESSOR_THRESHOLD_M13DB   =  13,		// -13dB 		* Default Volue *
SF_COMPRESSOR_THRESHOLD_M14DB   =  14,		// -14dB 
SF_COMPRESSOR_THRESHOLD_M15DB   =  15,		// -15dB 
SF_COMPRESSOR_THRESHOLD_M16DB   =  16,		// -16dB 
SF_COMPRESSOR_THRESHOLD_M17DB   =  17,		// -17dB 
SF_COMPRESSOR_THRESHOLD_M18DB   =  18,		// -18dB 
SF_COMPRESSOR_THRESHOLD_M19DB   =  19,		// -19dB 
SF_COMPRESSOR_THRESHOLD_M20DB	=  20,		// -20dB
SF_COMPRESSOR_THRESHOLD_M21DB	=  21,		// -21dB
SF_COMPRESSOR_THRESHOLD_M22DB	=  22,		// -22dB
SF_COMPRESSOR_THRESHOLD_M23DB	=  23,		// -23dB
SF_COMPRESSOR_THRESHOLD_M24DB	=  24,		// -24dB		
SF_COMPRESSOR_THRESHOLD_M25DB	=  25,		// -25dB
SF_COMPRESSOR_THRESHOLD_M26DB	=  26,		// -26dB
SF_COMPRESSOR_THRESHOLD_M27DB	=  27,		// -27dB
SF_COMPRESSOR_THRESHOLD_M28DB	=  28,		// -28dB
SF_COMPRESSOR_THRESHOLD_M29DB	=  29,		// -29dB

DIAMONDVOICE_SF_COMPRESSOR_THRESHOLD_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
} DiamondVoice_sfComThd;

/****************************************************************************************
 *  SwingFree AGC Compressor Slope
****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	
SF_COMPRESSOR_SLOPE_1P0		=	10	,		// 1.0
SF_COMPRESSOR_SLOPE_1P5		=	15	,		// 1.5
SF_COMPRESSOR_SLOPE_2P0		=	20	,		// 2.0
SF_COMPRESSOR_SLOPE_2P5		=	25	,		// 2.5
SF_COMPRESSOR_SLOPE_3P0		=	30	,		// 3.0
SF_COMPRESSOR_SLOPE_3P5		=	35	,		// 3.5
SF_COMPRESSOR_SLOPE_4P0		=	40	,		// 4.0
SF_COMPRESSOR_SLOPE_4P5		=	45	,		// 4.5
SF_COMPRESSOR_SLOPE_5P0		=	50	,		// 5.0
SF_COMPRESSOR_SLOPE_5P5		=	55	,		// 5.5
SF_COMPRESSOR_SLOPE_6P0		=	60	,		// 6.0
SF_COMPRESSOR_SLOPE_6P5		=	65	,		// 6.5
SF_COMPRESSOR_SLOPE_7P0		=	70	,		// 7.0
SF_COMPRESSOR_SLOPE_7P5		=	75	,		// 7.5
SF_COMPRESSOR_SLOPE_8P0		=	80	,		// 8.0
SF_COMPRESSOR_SLOPE_8P5		=	85	,		// 8.5		* Default Volue *
SF_COMPRESSOR_SLOPE_9P0		=	90	,		// 9.0
SF_COMPRESSOR_SLOPE_9P5		=	95	,		// 9.5
SF_COMPRESSOR_SLOPE_10P0	=	100	,		// 10.0

DIAMONDVOICE_SF_COMPRESSOR_SLOPE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}DiamondVoice_sfComSlope;
       
/****************************************************************************************
  *  SwingFree AGC Expansion Threshold
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 

SF_EXPANSION_THRESHOLD_M55DB	=    55,		// -55dB
SF_EXPANSION_THRESHOLD_M56DB	=    56,		// -56dB
SF_EXPANSION_THRESHOLD_M57DB	=    57,		// -57dB
SF_EXPANSION_THRESHOLD_M58DB	=    58,		// -58dB
SF_EXPANSION_THRESHOLD_M59DB	=    59,		// -59dB
SF_EXPANSION_THRESHOLD_M60DB	=    60,		// -60dB		* Default Volue *
SF_EXPANSION_THRESHOLD_M61DB	=    61,		// -61dB
SF_EXPANSION_THRESHOLD_M62DB	=    62,		// -62dB
SF_EXPANSION_THRESHOLD_M63DB	=    63,		// -63dB
SF_EXPANSION_THRESHOLD_M64DB	=    64,		// -64dB
SF_EXPANSION_THRESHOLD_M65DB	=    65,		// -65dB
SF_EXPANSION_THRESHOLD_M66DB	=    66,		// -66dB
SF_EXPANSION_THRESHOLD_M67DB	=    67,		// -67dB
SF_EXPANSION_THRESHOLD_M68DB	=    68,		// -68dB
SF_EXPANSION_THRESHOLD_M69DB	=    69,		// -69dB
SF_EXPANSION_THRESHOLD_M70DB	=    70,		// -70dB		
SF_EXPANSION_THRESHOLD_M71DB	=    71,		// -71dB
SF_EXPANSION_THRESHOLD_M72DB	=    72,		// -72dB
SF_EXPANSION_THRESHOLD_M73DB	=    73,		// -73dB
SF_EXPANSION_THRESHOLD_M74DB	=    74,		// -74dB
SF_EXPANSION_THRESHOLD_M75DB	=    75,		// -75dB

DIAMONDVOICE_SF_EXPANSION_THRESHOLD_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}DiamondVoice_sfExpThres;

/****************************************************************************************
 *  SwingFree AGC Expansion Slope
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
SF_EXPANSION_SLOPE_1_1P0	=	10,		// 1/1.0
SF_EXPANSION_SLOPE_1_1P5	=	15,		// 1/1.5
SF_EXPANSION_SLOPE_1_2P0	=	20,		// 1/2.0
SF_EXPANSION_SLOPE_1_2P5	=	25,		// 1/2.5
SF_EXPANSION_SLOPE_1_3P0	=	30,		// 1/3.0
SF_EXPANSION_SLOPE_1_3P5	=	35,		// 1/3.5
SF_EXPANSION_SLOPE_1_4P0	=	40,		// 1/4.0		* Default Volue *
SF_EXPANSION_SLOPE_1_4P5	=	45,		// 1/4.5
SF_EXPANSION_SLOPE_1_5P0	=	50,		// 1/5.0
SF_EXPANSION_SLOPE_1_5P5	=	55,		// 1/5.5
SF_EXPANSION_SLOPE_1_6P0	=	60,		// 1/6.0
SF_EXPANSION_SLOPE_1_6P5	=	65,		// 1/6.5
SF_EXPANSION_SLOPE_1_7P0	=	70,		// 1/7.0
SF_EXPANSION_SLOPE_1_7P5	=	75,		// 1/7.5
SF_EXPANSION_SLOPE_1_8P0	=	80,		// 1/8.0
SF_EXPANSION_SLOPE_1_8P5	=	85,		// 1/8.5
SF_EXPANSION_SLOPE_1_9P0	=	90,		// 1/9.0
SF_EXPANSION_SLOPE_1_9P5	=	95,		// 1/9.5
SF_EXPANSION_SLOPE_1_10P0	=	100,		// 1/10.0

DIAMONDVOICE_SF_EXPANSION_SLOPE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement  
}DiamondVoice_sfExpSlope;

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
 typedef struct{
	short monitor1;			// Rx Volume
	short monitor2;			// pre_amp gain			
	short monitor3;			// post_amp gain
	short monitor4;			// compressor threshld
	short monitor5;			// compressor slope
	short monitor6;			// expansion threshold
	short monitor7;			// expansion slope
	short monitor8;			// eq mode
} SFState_type;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/
extern int SamsungSwingFreeState(void);
#endif		// SamsungSwingFree_H

