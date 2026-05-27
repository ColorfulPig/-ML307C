
#ifndef	SamsungWiseVoice_H
#define SamsungWiseVoice_H

/****************************************************************************************
  *  WiseVoice Mode
 ****************************************************************************************/

//ICAT EXPORTED ENUM
typedef enum
{	
	WiseVoice_EAR_MODE  = 0, 
    WiseVoice_SPK_MODE   = 1,	
    WiseVoice_BT_MODE = 2,

    SAMSUNGWISEVOICEMODES_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}SamsungWiseVoiceModes;

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
 typedef struct{
	short monitor1;			// Rx Volume
	short monitor2;			// pre_amp gain			
	short monitor3;			// post_amp gain
	short monitor4;			// compressor threshld
} WiseVoiceState_type;




 /****************************************************************************************
 *  WiseVoice Compressor Threshold
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{
    	COMPRESSOR_THRESHOLD_0DB	=	0,			// 0dB
    	COMPRESSOR_THRESHOLD_M01DB	=	1,			// -1dB
    	COMPRESSOR_THRESHOLD_M02DB	=	2,			// -2dB
    	COMPRESSOR_THRESHOLD_M03DB	=	3, 			// -3dB
    	COMPRESSOR_THRESHOLD_M04DB	=	4, 			// -4dB
    	COMPRESSOR_THRESHOLD_M05DB	=	5, 			// -5dB
    	COMPRESSOR_THRESHOLD_M06DB	=	6, 			// -6dB
    	COMPRESSOR_THRESHOLD_M07DB	=	7, 			// -7dB
    	COMPRESSOR_THRESHOLD_M08DB	=	8,			// -8dB
    	COMPRESSOR_THRESHOLD_M09DB	=	9,			// -9dB
    	COMPRESSOR_THRESHOLD_M10DB	=	10,			// -10dB
    	COMPRESSOR_THRESHOLD_M11DB	=	11,			// -11dB
    	COMPRESSOR_THRESHOLD_M12DB	=	12,			// -12dB
    	COMPRESSOR_THRESHOLD_M13DB	=	13,			// -13dB
    	COMPRESSOR_THRESHOLD_M14DB	=	14,			// -14dB
    	COMPRESSOR_THRESHOLD_M15DB	=	15,			// -15dB
    	COMPRESSOR_THRESHOLD_M16DB	=	16,			// -16dB
    	COMPRESSOR_THRESHOLD_M17DB	=	17,			// -17dB
    	COMPRESSOR_THRESHOLD_M18DB	=	18,			// -18dB
    	COMPRESSOR_THRESHOLD_M19DB	=	19,			// -19dB
    	COMPRESSOR_THRESHOLD_M20DB	=	20,			// -20dB
    	COMPRESSOR_THRESHOLD_M21DB	=	21,			// -21dB
    	COMPRESSOR_THRESHOLD_M22DB	=	22,			// -22dB
    	COMPRESSOR_THRESHOLD_M23DB	=	23,			// -23dB
    	COMPRESSOR_THRESHOLD_M24DB	=	24,			// -24dB
    	COMPRESSOR_THRESHOLD_M25DB	=	25,			// -25dB
    	COMPRESSOR_THRESHOLD_M26DB	=	26,			// -26dB
    	COMPRESSOR_THRESHOLD_M27DB	=	27,			// -27dB
    	COMPRESSOR_THRESHOLD_M28DB	=	28,			// -28dB
    	COMPRESSOR_THRESHOLD_M29DB	=	29,			// -29dB
    	COMPRESSOR_THRESHOLD_M30DB	=	30,			// -30dB
    	COMPRESSOR_THRESHOLD_M31DB	=	31,			// -31dB
    	COMPRESSOR_THRESHOLD_M32DB	=	32,			// -32dB
    	COMPRESSOR_THRESHOLD_M33DB	=	33,			// -33dB
    	COMPRESSOR_THRESHOLD_M34DB	=	34,			// -34dB	
    	COMPRESSOR_THRESHOLD_M35DB	=	35,			// -35dB
    	COMPRESSOR_THRESHOLD_M36DB	=	36,			// -36dB
    	COMPRESSOR_THRESHOLD_M37DB	=	37,			// -37dB
    	COMPRESSOR_THRESHOLD_M38DB	=	38,			// -38dB
    	COMPRESSOR_THRESHOLD_M39DB	=	39,			// -39dB
    	COMPRESSOR_THRESHOLD_M40DB	=	40,			// -40dB
    	COMPRESSOR_THRESHOLD_M41DB	=	41,			// -41dB
    	COMPRESSOR_THRESHOLD_M42DB	=	42,			// -42dB
    	COMPRESSOR_THRESHOLD_M43DB	=	43,			// -43dB
    	COMPRESSOR_THRESHOLD_M44DB	=	44,			// -44dB	
    	COMPRESSOR_THRESHOLD_M45DB	=	45,			// -45dB
    	COMPRESSOR_THRESHOLD_M46DB	=	46,			// -46dB
    	COMPRESSOR_THRESHOLD_M47DB	=	47,			// -47dB
    	COMPRESSOR_THRESHOLD_M48DB	=	48,			// -48dB
    	COMPRESSOR_THRESHOLD_M49DB	=	49,			// -49dB
    	COMPRESSOR_THRESHOLD_M50DB	=	50,			// -50dB
    	COMPRESSOR_THRESHOLD_M51DB	=	51,			// -51dB
    	COMPRESSOR_THRESHOLD_M52DB	=	52,			// -52dB
    	COMPRESSOR_THRESHOLD_M53DB	=	53,			// -53dB
    	COMPRESSOR_THRESHOLD_M54DB	=	54,			// -54dB	
    	COMPRESSOR_THRESHOLD_M55DB	=	55,			// -55dB
    	COMPRESSOR_THRESHOLD_M56DB	=	56,			// -56dB
    	COMPRESSOR_THRESHOLD_M57DB	=	57,			// -57dB
    	COMPRESSOR_THRESHOLD_M58DB	=	58,			// -58dB
    	COMPRESSOR_THRESHOLD_M59DB	=	59,			// -59dB
    	COMPRESSOR_THRESHOLD_M60DB	=	60,			// -60dB
    	COMPRESSOR_THRESHOLD_M61DB	=	61,			// -61dB
    	COMPRESSOR_THRESHOLD_M62DB	=	62,			// -62dB
    	COMPRESSOR_THRESHOLD_M63DB	=	63,			// -63dB
    	COMPRESSOR_THRESHOLD_M64DB	=	64,			// -64dB	
    	COMPRESSOR_THRESHOLD_M65DB	=	65,			// -65dB
    	COMPRESSOR_THRESHOLD_M66DB	=	66,			// -66dB
    	COMPRESSOR_THRESHOLD_M67DB	=	67,			// -67dB
    	COMPRESSOR_THRESHOLD_M68DB	=	68,			// -68dB
    	COMPRESSOR_THRESHOLD_M69DB	=	69,			// -69dB
    	COMPRESSOR_THRESHOLD_M70DB	=	70,			// -70dB
    	COMPRESSOR_THRESHOLD_M71DB	=	71,			// -71dB
    	COMPRESSOR_THRESHOLD_M72DB	=	72,			// -72dB
    	COMPRESSOR_THRESHOLD_M73DB	=	73,			// -73dB
    	COMPRESSOR_THRESHOLD_M74DB	=	74,			// -74dB	
    	COMPRESSOR_THRESHOLD_M75DB	=	75,			// -75dB
    	COMPRESSOR_THRESHOLD_M76DB	=	76,			// -76dB
    	COMPRESSOR_THRESHOLD_M77DB	=	77,			// -77dB
    	COMPRESSOR_THRESHOLD_M78DB	=	78,			// -78dB
    	COMPRESSOR_THRESHOLD_M79DB	=	79,			// -79dB
    	COMPRESSOR_THRESHOLD_M80DB	=	80,			// -80dB
    	COMPRESSOR_THRESHOLD_M81DB	=	81,			// -81dB
    	COMPRESSOR_THRESHOLD_M82DB	=	82,			// -82dB
    	COMPRESSOR_THRESHOLD_M83DB	=	83,			// -83dB
    	COMPRESSOR_THRESHOLD_M84DB	=	84,			// -84dB	
    	COMPRESSOR_THRESHOLD_M85DB	=	85,			// -85dB
    	COMPRESSOR_THRESHOLD_M86DB	=	86,			// -86dB
    	COMPRESSOR_THRESHOLD_M87DB	=	87,			// -87dB
    	COMPRESSOR_THRESHOLD_M88DB	=	88,			// -88dB
    	COMPRESSOR_THRESHOLD_M89DB	=	89,			// -89dB
    	COMPRESSOR_THRESHOLD_M90DB	=	90,			// -90dB
    	COMPRESSOR_THRESHOLD_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 

}SamsungWiseVoiceCompressor_Threashold;

//ICAT EXPORTED ENUM
typedef enum
{
		COMPRESSOR_SLOPE_1P0	=		10,			// 1.0
        COMPRESSOR_SLOPE_1P5	=		15,			// 1.5
        COMPRESSOR_SLOPE_2P0	=		20,			// 2.0
    	COMPRESSOR_SLOPE_2P5	=		25,			// 2.5
    	COMPRESSOR_SLOPE_3P0	=		30,			// 3.0
    	COMPRESSOR_SLOPE_3P5	=		35,			// 3.5
    	COMPRESSOR_SLOPE_4P0	=		40,			// 4.0
    	COMPRESSOR_SLOPE_4P5	=		45,			// 4.5
    	COMPRESSOR_SLOPE_5P0	=		50,			// 5.0
    	COMPRESSOR_SLOPE_5P5	=		55,			// 5.5
    	COMPRESSOR_SLOPE_6P0	=		60,			// 6.0
    	COMPRESSOR_SLOPE_6P5	=		65,			// 6.5
    	COMPRESSOR_SLOPE_7P0	=		70,			// 7.0
    	COMPRESSOR_SLOPE_7P5	=		75,			// 7.5
    	COMPRESSOR_SLOPE_8P0	=		80,			// 8.0
    	COMPRESSOR_SLOPE_8P5	=		85,			// 8.5		* Default Volue *
    	COMPRESSOR_SLOPE_9P0	=		90,			// 9.0
    	COMPRESSOR_SLOPE_9P5	=		95,			// 9.5
    	COMPRESSOR_SLOPE_10P0	=		100,			// 10.0
		COMPRESSOR_SLOPE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 
			
}SamsungWiseVoiceCompressor_Slope;


//ICAT EXPORTED ENUM
typedef enum
{
    	EXPANSION_SLOPE_1_1P0	=			10,			// 1/1.0
    	EXPANSION_SLOPE_1_1P5	=			15,			// 1/1.5
    	EXPANSION_SLOPE_1_2P0	=			20,			// 1/2.0
    	EXPANSION_SLOPE_1_2P5	=			25,			// 1/2.5
    	EXPANSION_SLOPE_1_3P0	=			30,			// 1/3.0
    	EXPANSION_SLOPE_1_3P5	=			35,			// 1/3.5
    	EXPANSION_SLOPE_1_4P0	=			40,			// 1/4.0
    	EXPANSION_SLOPE_1_4P5	=			45,			// 1/4.5
    	EXPANSION_SLOPE_1_5P0	=			50,			// 1/5.0
    	EXPANSION_SLOPE_1_5P5	=			55,			// 1/5.5
    	EXPANSION_SLOPE_1_6P0	=			60,			// 1/6.0
    	EXPANSION_SLOPE_1_6P5	=			65,			// 1/6.5
    	EXPANSION_SLOPE_1_7P0	=			70,			// 1/7.0
    	EXPANSION_SLOPE_1_7P5	=			75,			// 1/7.5
    	EXPANSION_SLOPE_1_8P0	=			80,			// 1/8.0
    	EXPANSION_SLOPE_1_8P5	=			85,			// 1/8.5
    	EXPANSION_SLOPE_1_9P0	=			90,			// 1/9.0
    	EXPANSION_SLOPE_1_9P5	=			95,			// 1/9.5
    	EXPANSION_SLOPE_1_10P0   =			100,			// 1/10.0

		EXPANSION_SLOPE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement 				
}SamsungWiseVoiceExpansion_Slope;

//ICAT EXPORTED ENUM
typedef enum
{
     STATIC_GAIN_0dB	=	0,
     STATIC_GAIN_1dB	=	1,
     STATIC_GAIN_2dB	=	2,
     STATIC_GAIN_3dB	=	3,
     STATIC_GAIN_4dB	=	4,
     STATIC_GAIN_5dB	=	5,
     STATIC_GAIN_6dB	=	6,
     STATIC_GAIN_7dB	=	7,
     STATIC_GAIN_8dB	=	8,
     STATIC_GAIN_9dB	=	9,
     STATIC_GAIN_10dB	=10,
     STATIC_GAIN_11dB	=11,
     STATIC_GAIN_12dB	=12,
     STATIC_GAIN_13dB	=13,
     STATIC_GAIN_14dB	=14,
     STATIC_GAIN_15dB	=15,
     STATIC_GAIN_16dB	=16,
     STATIC_GAIN_17dB	=17,
     STATIC_GAIN_18dB	=18,
     
	STATIC_GAIN_ENUM_32_BIT   = 0x7FFFFFFF //32bit enum compiling enforcement				 
}SamsungWiseVoiceStaticGain;

//ICAT EXPORTED ENUM
typedef enum
{	
	ADAPT_TIME_0msec  = 0, 
    ADAPT_TIME_10msec   = 10,	
    ADAPT_TIME_20msec = 20,
    ADAPT_TIME_30msec = 30,
	ADAPT_TIME_40msec  = 40, 
    ADAPT_TIME_50msec  = 50,	
    ADAPT_TIME_60msec = 60,
    ADAPT_TIME_70msec = 70,
	ADAPT_TIME_80msec  = 80, 
	ADAPT_TIME_90msec	= 90,	
	ADAPT_TIME_100msec = 100,
	ADAPT_TIME_110msec = 110,
	ADAPT_TIME_120msec  = 120, 
	ADAPT_TIME_130msec  = 130,	
	ADAPT_TIME_140msec = 140,
	ADAPT_TIME_150msec = 150,
    ADAPT_TIME_160msec = 160,
    ADAPT_TIME_170msec = 170, 
    ADAPT_TIME_180msec = 180,
    ADAPT_TIME_190msec = 190,
    ADAPT_TIME_200msec = 200,
    ADAPT_TIME_210msec = 210,
    ADAPT_TIME_220msec = 220, 
    ADAPT_TIME_230msec =  230,
    ADAPT_TIME_240msec  = 240, 
    ADAPT_TIME_250msec  = 250, 
    ADAPT_TIME_260msec  = 260, 
    ADAPT_TIME_270msec  = 270, 
    ADAPT_TIME_280msec  = 280, 
    ADAPT_TIME_290msec  = 290, 
    ADAPT_TIME_300msec  = 300 ,
    ADAPT_TIME_310msec  =  310 ,
    ADAPT_TIME_320msec  = 320 ,
    ADAPT_TIME_330msec  = 330 ,
    ADAPT_TIME_340msec  = 340 ,
    ADAPT_TIME_350msec  = 350 ,
    ADAPT_TIME_360msec  = 360 ,
    ADAPT_TIME_370msec  = 370 ,
    ADAPT_TIME_380msec  = 380 ,
    ADAPT_TIME_390msec  = 390 ,
    ADAPT_TIME_400msec  = 400 ,
    ADAPT_TIME_410msec  = 410 ,
    ADAPT_TIME_420msec  = 420 ,
    ADAPT_TIME_430msec  = 430 ,
    ADAPT_TIME_440msec  = 440 ,
    ADAPT_TIME_450msec  = 450 ,
    ADAPT_TIME_460msec  = 460 ,
    ADAPT_TIME_470msec  = 470 ,
    ADAPT_TIME_480msec  = 480 ,
    ADAPT_TIME_490msec  = 490 ,
    ADAPT_TIME_500msec  = 500 ,
    ADAPT_TIME_510msec  = 510 ,
    ADAPT_TIME_520msec  = 520 ,
    ADAPT_TIME_530msec  = 530 ,
    ADAPT_TIME_540msec  = 540 ,
    ADAPT_TIME_550msec  = 550 ,
    ADAPT_TIME_560msec  = 560 ,
    ADAPT_TIME_570msec  = 570 ,
    ADAPT_TIME_580msec  = 580 ,
    ADAPT_TIME_590msec  = 590 ,
    ADAPT_TIME_600msec  = 600 ,
    ADAPT_TIME_610msec  = 610 ,
     ADAPT_TIME_620msec  = 620 ,
     ADAPT_TIME_630msec  = 630 ,
     ADAPT_TIME_640msec  = 640 ,
     ADAPT_TIME_650msec  = 650 ,
     ADAPT_TIME_660msec  = 660 ,
     ADAPT_TIME_670msec  = 670 ,
     ADAPT_TIME_680msec  = 680 ,
     ADAPT_TIME_690msec  = 690 ,
     ADAPT_TIME_700msec  = 700 ,
     ADAPT_TIME_710msec  = 710 ,
     ADAPT_TIME_720msec  = 720 ,
     ADAPT_TIME_730msec  = 730 ,
     ADAPT_TIME_740msec  = 740 ,
     ADAPT_TIME_750msec  = 750 ,
     ADAPT_TIME_760msec  = 760 ,
     ADAPT_TIME_770msec  = 770 ,
     ADAPT_TIME_780msec  = 780 ,
     ADAPT_TIME_790msec  = 790 ,
     ADAPT_TIME_800msec  = 800 ,
     ADAPT_TIME_810msec  = 810 ,
     ADAPT_TIME_820msec  = 820 ,
     ADAPT_TIME_830msec  = 830 ,
     ADAPT_TIME_840msec  = 840 ,
     ADAPT_TIME_850msec  = 850 ,
     ADAPT_TIME_860msec  = 860 ,
     ADAPT_TIME_870msec  = 870 ,
     ADAPT_TIME_880msec  = 880 ,
     ADAPT_TIME_890msec  = 890 ,
     ADAPT_TIME_900msec  = 900 ,
     ADAPT_TIME_910msec  = 910 ,
     ADAPT_TIME_920msec  = 920 ,
     ADAPT_TIME_930msec  = 930 ,
     ADAPT_TIME_940msec  = 940 ,
     ADAPT_TIME_950msec  = 950 ,
     ADAPT_TIME_960msec  = 960 ,
     ADAPT_TIME_970msec  = 970 ,
     ADAPT_TIME_980msec  = 980 ,
     ADAPT_TIME_990msec  = 990 ,
     ADAPT_TIME_1000msec = 1000,
     ADAPT_TIME_1100msec = 1010,
     ADAPT_TIME_1200msec = 1020,
     ADAPT_TIME_1300msec = 1030,
     ADAPT_TIME_1400msec = 1040,
     ADAPT_TIME_1500msec = 1050,
     ADAPT_TIME_1600msec = 1060,
     ADAPT_TIME_1700msec = 1070,
     ADAPT_TIME_1800msec = 1080,
     ADAPT_TIME_1900msec = 1090,
     ADAPT_TIME_2000msec = 1100,
     ADAPT_TIME_2100msec = 1110,
     ADAPT_TIME_2200msec = 1120,
     ADAPT_TIME_2300msec = 1130,
     ADAPT_TIME_2400msec = 1140,
     ADAPT_TIME_2500msec = 1150,
     ADAPT_TIME_2600msec = 1160,
     ADAPT_TIME_2700msec = 1170,
     ADAPT_TIME_2800msec = 1180,
     ADAPT_TIME_2900msec = 1190,
     ADAPT_TIME_3000msec = 1200,
     ADAPT_TIME_3100msec = 1210,
     ADAPT_TIME_3200msec = 1220,
     ADAPT_TIME_3300msec = 1230,
     ADAPT_TIME_3400msec = 1240,
     ADAPT_TIME_3500msec = 1250,
     ADAPT_TIME_3600msec = 1260,
     ADAPT_TIME_3700msec = 1270,
     ADAPT_TIME_3800msec = 1280,
     ADAPT_TIME_3900msec = 1290,
     ADAPT_TIME_4000msec = 1300,

    SAMSUNGWISEVOICEADAPT_TIME_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}SamsungWiseVoiceAdaptTime;






/****************************************************************************************
 *  WiseVoice In/Out Gain
 ****************************************************************************************/
//ICAT EXPORTED ENUM
typedef enum
{	 
     WV_VOLUME_M30DB		=			-30,
     WV_VOLUME_M29DB		=			-29,
     WV_VOLUME_M28DB		=			-28,
     WV_VOLUME_M27DB		=			-27,
     WV_VOLUME_M26DB		=			-26,
     WV_VOLUME_M25DB		=			-25,
     WV_VOLUME_M24DB		=			-24,
     WV_VOLUME_M23DB		=			-23,
     WV_VOLUME_M22DB		=			-22,
     WV_VOLUME_M21DB		=			-21,
     WV_VOLUME_M20DB		=			-20,
     WV_VOLUME_M19DB		=			-19,
     WV_VOLUME_M18DB		=			-18,
     WV_VOLUME_M17DB		=			-17,
     WV_VOLUME_M16DB		=			-16,
     WV_VOLUME_M15DB		=			-15,
     WV_VOLUME_M14DB		=			-14,
     WV_VOLUME_M13DB		=			-13,
     WV_VOLUME_M12DB		=			-12,
     WV_VOLUME_M11DB		=			-11,
     WV_VOLUME_M10DB		=			-10,
     WV_VOLUME_M09DB		=			-9,
     WV_VOLUME_M08DB		=			-8,
     WV_VOLUME_M07DB		=			-7,
     WV_VOLUME_M06DB		=			-6,
     WV_VOLUME_M05DB		=			-5,
     WV_VOLUME_M04DB		=			-4,
     WV_VOLUME_M03DB		=			-3,
     WV_VOLUME_M02DB		=			-2,
     WV_VOLUME_M01DB		=			-1,
     WV_VOLUME_0DB			=			0,
     WV_VOLUME_01DB			=			1,
     WV_VOLUME_02DB			=			2,
     WV_VOLUME_03DB			=			3,
     WV_VOLUME_04DB			=			4,
     WV_VOLUME_05DB			=			5,
     WV_VOLUME_06DB			=			6,
     WV_VOLUME_07DB			=			7,
     WV_VOLUME_08DB			=			8,
     WV_VOLUME_09DB			=			9,
     WV_VOLUME_10DB			=			10,
     WV_VOLUME_11DB			=			11,
     WV_VOLUME_12DB			=			12,
     WV_VOLUME_13DB			=			13,
     WV_VOLUME_14DB			=			14,
     WV_VOLUME_15DB			=			15,
     WV_VOLUME_16DB			=			16,
     WV_VOLUME_17DB			=			17,
     WV_VOLUME_18DB			=			18,
     WV_VOLUME_19DB			=			19,
     WV_VOLUME_20DB			=			20,
     WV_VOLUME_21DB			=			21,
     WV_VOLUME_22DB			=			22,
     WV_VOLUME_23DB			=			23,
     
     SAMSUNGWISEVOICEWVVOLUME_TIME_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}SamsungWiseVoiceWVVolume;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

extern int DiamondVoiceEngineWiseVoiceINIT(short  bandwidth);

extern int SamsungWiseVoiceInOutConfig(short *Speech, short *OutSignal);

extern int SamsungWiseVoiceConfig(short mode);

extern int SamsungWiseVoiceAdaptTimeConfig(short att, short rel);

extern int SamsungWiseVoiceThresholdConfig(short *thd);

extern int SamsungWiseVoiceSlopConfig(short *slp);

extern int SamsungWiseVoiceGainConfig(short gain);

extern int SamsungWiseVoiceInOutGainConfig(short ingain, short outgain);

extern int SamsungWiseVoiceExe(void);
  
#endif		// SamsungWiseVoice_H
