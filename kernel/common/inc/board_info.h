/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code ("Material") are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel's prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: BOARD information                               				*/
/*                                                                      */
/* Filename:  board_info.h                                              */
/*                                                                      */
/* Authors: Rony Hanina                                                 */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/

#ifndef _BOARD_INFO_H_
#define _BOARD_INFO_H_
/***************************************************************************/
#include "i2c_eeprom.h"

//#define __EPROM_OLD_VERSION__   1

/***************************************************************************/


#define EPROM_NUM_OF_CARD		15



//ICAT EXPORTED ENUM
typedef enum
{
    HERMON_PLATFORM 	= 0x01,
    TAVOR_PLATFORM 		= 0x10,
    BULVERDE_PLATFORM 	= 0x20,
    MONAHANS_PLATFORM 	= 0x30,
    TAVOR_PV_PLATFORM 	= 0x40,
    TAVOR_PV2_PLATFORM 	= 0x50,
    TAVOR_MMP_PLATFORM 	= 0x60,
    HERMON_2_PLATFORM 	= 0x70
}EpromPlatformName;


//ICAT EXPORTED ENUM
typedef enum
{
    EVB_TYPE 			= 0x01,
    PDK_TYPE 			= 0x10,
    PPV_TYPE 			= 0x20,
    SAAR_TYPE 			= 0x30,
    YARDEN_TYPE 		= 0x40
}EpromPlatformType;


//ICAT EXPORTED ENUM
typedef enum
{
    APPS_PROC_TYPE_BULVERDE_B0		= 0x01,
    APPS_PROC_TYPE_BULVERDE_MCP1	= 0x02,
    APPS_PROC_TYPE_BULVERDE_MCP2	= 0x03,
    APPS_PROC_TYPE_MONAHANS_A0		= 0x10,
    APPS_PROC_TYPE_MONAHANS_B0		= 0x11,
    APPS_PROC_TYPE_TAVOR_Z0			= 0x20,
    APPS_PROC_TYPE_TAVOR_A0			= 0x21,
    APPS_PROC_TYPE_TAVOR_A1			= 0x22,
    APPS_PROC_TYPE_TAVOR_B0			= 0x23,
    APPS_PROC_TYPE_TAVOR_B1			= 0x24,
    APPS_PROC_TYPE_TAVOR_B2			= 0x25,
    APPS_PROC_TYPE_TAVOR_PV			= 0x28,
    APPS_PROC_TYPE_TAVOR_PV2		= 0x2B,
    APPS_PROC_TYPE_TAVOR_MMP 		= 0x2D,
    APPS_PROC_TYPE_HERMON_2			= 0x30
}EpromAppsProcType;


//ICAT EXPORTED ENUM
typedef enum
{
    COMM_PROC_TYPE_HERMON_B0	= 0x01,
    COMM_PROC_TYPE_HERMON_B1	= 0x02,
    COMM_PROC_TYPE_HERMON_B2	= 0x03,
    COMM_PROC_TYPE_HERMON_B3	= 0x04,
    COMM_MCP1_TYPE_HERMON_B3	= 0x05,
    COMM_MCP2_TYPE_HERMON_B3	= 0x06,
    COMM_MCP2_TYPE_HERMON_LV	= 0x07,
    COMM_MCP2_TYPE_HERMON_L		= 0x08,
    COMM_PROC_TYPE_TAVOR_Z0		= 0x10,
    COMM_PROC_TYPE_TAVOR_A0		= 0x11,
    COMM_PROC_TYPE_TAVOR_A1		= 0x12,
    COMM_PROC_TYPE_TAVOR_B0		= 0x13,
    COMM_PROC_TYPE_TAVOR_B1		= 0x14,
    COMM_PROC_TYPE_TAVOR_B2		= 0x15,
    COMM_PROC_TYPE_TAVOR_PV		= 0x17,
    COMM_PROC_TYPE_TAVOR_PV2	= 0x1A,
    COMM_PROC_TYPE_TAVOR_MMP	= 0x20,
    COMM_PROC_TYPE_HERMON_2		= 0x30
}EpromCommProcType;


//ICAT EXPORTED ENUM
typedef enum
{
    CEDER_CREEK_C0_TYPE		= 0x01,
    CEDER_CREEK_D0_TYPE 	= 0x02,
    GILON_A0_TYPE 			= 0x10,
    GILON_B0_TYPE 			= 0x11,
    GILON_C0_TYPE 			= 0x12
}EpromAfeType;


//ICAT EXPORTED ENUM
typedef enum
{
    ARAVA_REV_8_TYPE		= 0x01,
    ARAVA_REV_9_TYPE 		= 0x02,
    MICCO_A0_TYPE 			= 0x10,
    MICCO_B0_TYPE 			= 0x11,
    MICCO_C0_TYPE 			= 0x12,
    MICCO_D0_TYPE 			= 0x13
}EpromPmicType;


//ICAT EXPORTED ENUM
typedef enum
{
    SAMSUNG_K9K1216Q0C_NAND_TYPE			= 0x01,
    SAMSUNG_K9F1G08R0A_NAND_TYPE 			= 0x02,
    SAMSUNG_K9K1G08R0B_NAND_TYPE 			= 0x03,
    SAMSUNG_POP_2Gx16ONENAND_1Gx32DDR_TYPE 	= 0x04,
    MICRON_MT29F1G08ABA_NAND_TYPE 			= 0x10,
    MICRON_MT29F1G16ABA_NAND_TYPE 			= 0x11,
    MICRON_MT29F1G08ABB_NAND_TYPE 			= 0x12,
    MICRON_MT29F1G16ABB_NAND_TYPE 			= 0x13,
    SAMSUNG_ONENAND_KMF1G16Q2A_NAND_TYPE	= 0x20,
    MSYS_MDOC3_MD2534D2GV18XP_NAND_TYPE		= 0x30
}EpromNandFlashType;


//ICAT EXPORTED ENUM
typedef enum
{
    INTEL_TYAX_RD48F4440LVYBQO_NOR_TYPE		= 0x01,
    INTEL_TYAX_LZ48F4440LOYBPO_NOR_TYPE 	= 0x02,
    INTEL_SIBLEY_PF48F5000M0Y1B0_NOR_TYPE 	= 0x10,
    INTEL_SIBLEY_PF48F5000M0Y0C0_NOR_TYPE 	= 0x11
}EpromNorFlashType;


//ICAT EXPORTED ENUM
typedef enum
{
    SAMSUNG_SDRAM_K4S51323_TYPE			= 0x01,
    SAMSUNG_SDRAM_K4S56163_TYPE 		= 0x02,
    INFINEON_DDR_HYB18M512160_TYPE 		= 0x10,
    MICRON_DDR_MT46H16M32LF_TYPE 		= 0x20,
    MICRON_DDR_MT46H32M16LF_TYPE 		= 0x21,
    MICRON_DDR_1Gx32_TYPE 				= 0x22,
    MICRON_POP_1Gx16NAND_512x32DDR_TYPE	= 0x23,
    MICRON_POP_2Gx16NAND_1Gx32DDR_TYPE 	= 0x30
}EpromSdramType;


//ICAT EXPORTED ENUM
typedef enum
{
    NORMAL_OPERATION_MODE 	= 0x01,
    SYSTEM_DEBUG_MODE 		= 0x10,
    L1_DEBUG_MODE 			= 0x20
}EpromPlatformConfigMode;



//ICAT EXPORTED ENUM
typedef enum
{
    EPSON_L2F50282T00_MAIN 		= 0x01,
    JAMITECH_J220Q203_MAIN 		= 0x02,
    TOSHIBA_LTM020D550_MAIN 	= 0x03,
    TOSHIBA_LTD024D440_MAIN 	= 0x04,
    EPSON_L2F60012T00_MAIN 		= 0x10,
    EPSON_L2F50176T00_MAIN		= 0x11,
    EPSON_LTM022A97A_MAIN		= 0x12
}EpromMainLcdType;


//ICAT EXPORTED ENUM
typedef enum
{
    EPSON_L2F50282T00_SUB 		= 0x01,
    JAMITECH_J220Q203_SUB 		= 0x02,
    TOSHIBA_LTM020D550_SUB 		= 0x03,
    TOSHIBA_LTD024D440_SUB 		= 0x04,
    EPSON_L2F60012T00_SUB 		= 0x10,
    EPSON_L2F50176T00_SUB		= 0x11,
    EPSON_LTM022A97A_SUB		= 0x12
}EpromSubLcdType;




//ICAT EXPORTED ENUM
typedef enum
{
    OV9640_MAIN		= 0x01,
    OV9650_MAIN		= 0x02,
    OV9653_MAIN		= 0x03,
    OV9655_MAIN		= 0x04,
    OV2630_MAIN		= 0x05,
    OV2640_MAIN		= 0x06,
	OV3630_MAIN     = 0x07,
    OV7660_MAIN		= 0x08,
    OV6680_MAIN		= 0x09,
	OV3640_MAIN     = 0x0A
}EpromMainCameraType;


//ICAT EXPORTED ENUM
typedef enum
{
    OV9640_SUB 		= 0x01,
    OV9650_SUB 		= 0x02,
    OV9653_SUB 		= 0x03,
    OV9655_SUB 		= 0x04,
    OV2630_SUB 		= 0x05,
    OV2640_SUB 		= 0x06,
	OV3630_SUB      = 0x07,
    OV7660_SUB 		= 0x08,
    OV6680_SUB 		= 0x09,
	OV3640_SUB      = 0x0A
}EpromSubCameraType;


//ICAT EXPORTED ENUM
typedef enum
{
    HSDL_3002 		= 0x01
}EpromIrdaType;


//ICAT EXPORTED ENUM
typedef enum
{
    KEYPAD_5_5_NO_ROTARY_NO_DIRECT 			= 0x01,
    KEYPAD_5_5_WITH_ROTARY_NO_DIRECT 		= 0x02,
    KEYPAD_5_5_WITH_ROTARY_WITH_DIRECT 		= 0x03,
    KEYPAD_8_8_NO_ROTARY_NO_DIRECT 			= 0x04,
    KEYPAD_8_8_WITH_ROTARY_NO_DIRECT 		= 0x10,
    KEYPAD_8_8_WITH_ROTARY_WITH_DIRECT 		= 0x11,
    KEYPAD_6_6_NO_ROTARY_NO_DIRECT 			= 0x12,
    KEYPAD_6_6_WITH_ROTARY_NO_DIRECT 		= 0x13,
    KEYPAD_6_6_WITH_ROTARY_WITH_DIRECT 		= 0x20,
}EpromKeypadType;



//ICAT EXPORTED ENUM
typedef enum
{
    LCD_PARALLEL_USING_PMIC 	= 0x01,
    LCD_PARALLEL_USING_PWM 		= 0x02,
    LCD_SERIAL_USING_PMIC 		= 0x10,
    LCD_SERIAL_USING_PWM 		= 0x11
}EpromMainLcdBlType;



//ICAT EXPORTED ENUM
typedef enum
{
    SUB_LCD_PARALLEL_USING_PMIC 	= 0x01,
    SUB_PARALLEL_USING_PWM 			= 0x02,
    SUB_SERIAL_USING_PMIC 			= 0x10,
    SUB_SERIAL_USING_PWM 			= 0x11
}EpromSubLcdBlType;



//ICAT EXPORTED ENUM
typedef enum
{
    KEYPAD_PARALLEL_USING_PMIC 		= 0x01,
    KEYPAD_PARALLEL_USING_PWM 		= 0x02,
    KEYPAD_SERIAL_USING_PMIC 		= 0x10,
    KEYPAD_SERIAL_USING_PWM 		= 0x11
}EpromKeypadBlType;


//ICAT EXPORTED ENUM
typedef enum
{
    CAM_PARALLEL_USING_PMIC 	= 0x01,
    CAM_PARALLEL_USING_GPIO		= 0x02,
    CAM_SERIAL_USING_PMIC 		= 0x10,
    CAM_SERIAL_USING_GPIO 		= 0x11
}EpromCamStrobeType;


//ICAT EXPORTED ENUM
typedef enum
{
	PHILIPS_TEA5764        = 0x01,
	PHILIPS_TEA5761        = 0x02,
	SILICON_LAB_SI4700     = 0x03
}EpromFmRadioType;



//ICAT EXPORTED ENUM
typedef enum
{
#ifdef __EPROM_OLD_VERSION__
    EPROM_LCD_CAM_V1 					= 0x40,
    EPROM_LCD_CAM_V2 					= 0x41,
    EPROM_KEYPAD 						= 0x51,
    EPROM_MISC_V1 						= 0x60,
    EPROM_MISC_V2 						= 0x61,
    EPROM_MAIN_BOARD_V1					= 0x100,
    EPROM_MAIN_BOARD_V2   				= 0x101,
    EPROM_DC_14 						= 0x110,
    EPROM_DC_35 						= 0x111,
    EPROM_DC_40 						= 0x112,
    EPROM_MEMORY_V1 					= 0x120,
    EPROM_MEMORY_V2 					= 0x121,
    EPROM_RF_GILON_V1 					= 0x150,
    EPROM_RF_GILON_V2 					= 0x151,
    EPROM_MICCO_B_V1 					= 0x171,
    EPROM_MICCO_B_V2 					= 0x172,
    EPROM_RF_POLEG_V1 					= 0x190,
    EPROM_RF_POLEG_V2 					= 0x191,
    EPROM_RF_POLEG_Z0_ADAPTOR			= 0x192,
	EPROM_RF_POLARIS_2_V8				= 0x1a0,
	EPROM_RF_POLARIS_2_V9				= 0x1a1,
	EPROM_RF_POLARIS_2_Z0_AVB_ADAPTOR 	= 0x1a5,
	EPROM_RF_MAXIM						= 0x1b0,
	EPROM_TEC							= 0x1d0
#else
    TAVOR_MATHIS_Z0					= 0x01,
    TAVOR_MATHIS_A0					= 0x02,
    TAVOR_MATHIS_B0					= 0x03,
    HERMON_MATHIS_SOLDERED			= 0x08,
    HERMON_MATHIS_SOCKET			= 0x09,
    HERMON_MATHIS_SOCKET_PPV		= 0x0a,
    HERMON_MATHIS_FOR_POLEG			= 0x0b,
    HERMON_L_MATHIS					= 0x0c,
    HERMON_MATHIS_2_CHIP_CC			= 0x0f,
    MATHIS_MAIN_BOARD				= 0x11,
    LCD_CAM_TAVOR					= 0x13,
    LCD_CAM_HERMON					= 0x14,
    KEYPAD_2_CHIP					= 0x17,
    KEYPAD_TAVOR					= 0x18,
    MISC_TAVOR						= 0x1a,
    MISC_HERMON						= 0x1d,
    BT								= 0x1f,
    GPS								= 0x21,
    WLAN							= 0x23,
    WLAN_AND_BT						= 0x25,
    TAVOR_EVB2_MAIN_BOARD		 	= 0x27,
    TAVOR_A0_EVB2_DC_14 			= 0x29,
    TAVOR_A0_EVB2_DC_35 			= 0x2a,
    TAVOR_A0_EVB2_DC_PLBGA			= 0x2b,
    TAVOR_B0_EVB2_DC_14 			= 0x2d,
    TAVOR_B0_EVB2_DC_35 			= 0x2e,
    TAVOR_B0_EVB2_DC_PLBGA			= 0x2f,
    TAVOR_EVB_MEMORY				= 0x31,
    TAVOR_EVB2_NAND_AND_NOR_MEM		= 0x32,
    TAVOR_EVB2_DUAL_NAND_MEM		= 0x33,
    TAVOR_EVB1_MAIN_BOARD		 	= 0x37,
    TAVOR_EVB1_DC_14 				= 0x39,
    TAVOR_EVB1_DC_35 				= 0x3a,
    TAVOR_EVB1_DC_PLBGA				= 0x3b,
    AFE_GILON_DC 					= 0x3d,
    AFE_CCR_DC_V3					= 0x3f,
    MICCO_A 						= 0x41,
    MICCO_B 						= 0x42,
    MICCO_C 						= 0x43,
    LDO_CARD						= 0x46,
    LDO_CARD_FOR_TAVOR				= 0x47,
    TAVOR_EVB_LDO_ADAPTOR			= 0x48,
    RF_POLEG	 					= 0x4a,
    RF_POLEG_Z0_ADAPTOR				= 0x4b,
    RF_SION	 						= 0x4d,
    RF_AYALON	 					= 0x4F,
    RF_YARKON	 					= 0x51,
    RF_DOLEV	 					= 0x53,
    RF_ALMOG	 					= 0x55,
	RF_MAXIM						= 0x57,
	RF_POLARIS						= 0x59,
	L1_TOOLS_CARD					= 0x5b,
	CLOCK_ONLY						= 0x5e
#endif
} EpromCardType;



typedef enum
{
    EEPROM_MAIN_BOARD  	= 0,
    EEPROM_LCD_CAM  	= 1,
    EEPROM_MICCO  		= 2,
    EEPROM_35_DC  		= 3,
    EEPROM_14_DC  		= 4,
    EEPROM_40_DC  		= 5,
    EEPROM_MISC  		= 6,
    EEPROM_KEYPAD  		= 7,
    EEPROM_TEC  		= 8,
	EEPROM_MEMORY  		= 9,
    EEPROM_MAXIM  		= 10,
	EEPROM_POLARIS		= 11,
	EEPROM_POLEG		= 12,
    EEPROM_GILON  		= 13,
	EEPROM_MAX_CARD
} EEPROM_Card;


//ICAT EXPORTED ENUM
typedef enum
{
	EEPROM_RF_MAXIM_POLARIS_2_V9		= 0x1,
    EEPROM_RF_POLEG_V1 					= 0x2,
    EEPROM_RF_POLEG_V2 					= 0x3,
    EEPROM_RF_POLEG_V3 					= 0x4,
    EEPROM_RF_POLEG_V4 					= 0x5,
    EEPROM_RF_POLEG_V5 					= 0x6,
    EEPROM_RF_POLEG_V6 					= 0x7,
    EEPROM_RF_POLEG_V7 					= 0x8,
    EEPROM_RF_POLEG_V8 					= 0x9,
    EEPROM_RF_POLEG_V9 					= 0xA,
    EEPROM_RF_POLEG_V10 				= 0xB,
    EEPROM_RF_POLEG_V11 				= 0xC,
    EEPROM_RF_POLEG_V12 				= 0xD,
    EEPROM_RF_POLEG_V13 				= 0xE,
    EEPROM_RF_POLEG_V14 				= 0xF,
    EEPROM_RF_SION_V1 					= 0x10,
    EEPROM_RF_SION_V2 					= 0x11,
    EEPROM_RF_SION_V3 					= 0x12,
    EEPROM_RF_SION_V4 					= 0x13,
    EEPROM_RF_AYALON_V1 				= 0x20,
    EEPROM_RF_AYALON_V2 				= 0x21,
    EEPROM_RF_AYALON_V3 				= 0x22,
    EEPROM_RF_AYALON_V4 				= 0x23,
    EEPROM_RF_YARKON_V1 				= 0x30,
    EEPROM_RF_YARKON_V2 				= 0x31,
    EEPROM_RF_YARKON_V3 				= 0x32,
    EEPROM_RF_YARKON_V4 				= 0x33,
    EEPROM_RF_ALMOG_V1 					= 0x40,
    EEPROM_RF_ALMOG_V2 					= 0x41,
    EEPROM_RF_ALMOG_V3 					= 0x42,
    EEPROM_RF_ALMOG_V4 					= 0x43,
    EEPROM_RF_DOLEV_V1 					= 0x50,
    EEPROM_RF_DOLEV_V2 					= 0x51,
    EEPROM_RF_DOLEV_V3 					= 0x52,
    EEPROM_RF_DOLEV_V4 					= 0x53,
    EEPROM_RF_POLEG_GV1 				= 0x54,
    EEPROM_RF_POLEG_GV2 				= 0x55,
    EEPROM_RF_POLEG_GV3 				= 0x56,
    EEPROM_RF_POLEG_GV4 				= 0x57,
    EEPROM_RF_POLEG_GV5 				= 0x58,
    EEPROM_RF_POLEG_GV6 				= 0x59,
    EEPROM_RF_POLEG_GV7 				= 0x5A,
    EEPROM_RF_POLEG_GV8 				= 0x5B,

    EEPROM_RF_POLEG_Z0_ADAPTOR			= 0xd0,
	EEPROM_RF_MAXIM_POLARIS_2_V8		= 0xd1,
	EEPROM_RF_POLARIS_2_Z0_AVB_ADAPTOR 	= 0xd2,
	EEPROM_RF_MAXIM						= 0xd3
} EEPROM_RFCard;


//ICAT EXPORTED STRUCT
typedef struct
{
#ifdef __EPROM_OLD_VERSION__
	UINT8						cardTypeHi;
	UINT8						cardTypeLo;
    UINT8 			  			ecnLo;
    UINT8 			  			ecnHi;
    UINT8 			  			cpldVer;
    UINT8 			  			cardSNum1;
    UINT8 			  			cardSNum2;
    UINT8 			  			cardSNum3;
    UINT8 			  			cardSNum4;
    UINT8 			  			spare_1;
    UINT8 			  			spare_2;
    UINT8 			  			userData_1;
    UINT8 			  			userData_2;
#else
	EpromCardType				cardType;
	UINT8						cardVersion;
    UINT8 			  			ecnLo;
    UINT8 			  			ecnHi;
    UINT8 			  			cpldVer;
    UINT8 			  			cardSNum1;
    UINT8 			  			cardSNum2;
    UINT8 			  			cardSNum3;
    UINT8 			  			cardSNum4;
    UINT8 			  			spare_1;
    UINT8 			  			spare_2;
    UINT8 			  			userData_1;
    UINT8 			  			userData_2;
#endif
} EpromCard;


//ICAT EXPORTED STRUCT
typedef struct
{
    UINT8 			  			platformSNum1;
    UINT8 			  			platformSNum2;
    UINT8 			  			platformSNum3;
    UINT8 			  			platformSNum4;
    UINT8 			  			platformSNum5;
    UINT8 			  			platformSNum6;
    UINT8 			  			platformSNum7;
    UINT8 			  			platformSNum8;
    UINT8 			  			spare_3;
    UINT8 			  			spare_4;
    UINT8 			  			spare_5;
    UINT8 			  			spare_6;
    UINT8 			  			spare_7;
    UINT8 			  			spare_8;
    UINT8 			  			crc_1;
    UINT8 			  			crc_2;
} EpromPlatformEpilogue;


//ICAT EXPORTED STRUCT
typedef struct
{
    UINT8 			  			header;
    UINT8 			  			epromSize;
	EpromPlatformName           platformName;
	EpromPlatformType           platformType;
    UINT8 			  			platformVer;
    UINT8 			  			numOfCard;
	EpromAppsProcType           appsProcType;
	EpromCommProcType           commProcType;
	EpromAfeType                afeType;
	EpromPmicType               pmicType;
	EpromNandFlashType          nandFlashType;
	EpromNorFlashType           norFlashType;
	EpromSdramType              SdramType;
	EpromPlatformConfigMode     PlatformConfigMode;
    UINT8 			  			spare_1;
#ifdef __EPROM_OLD_VERSION__
    UINT8 			  			spare_2;
#else
    EEPROM_RFCard 			  	rfType;
#endif
	EpromMainLcdType            mainLcdType;
	EpromSubLcdType             subLcdType;
	EpromMainCameraType         mainCameraType;
	EpromSubCameraType          subCameraType;
	EpromIrdaType               irdaType;
	EpromKeypadType             keypadType;
	UINT8        				rotaryEncoderSupport;
	EpromMainLcdBlType          mainLcdBlType;
	EpromSubLcdBlType           subLcdBlType;
	EpromKeypadBlType           keypadBlType;
	EpromCamStrobeType          camStrobeType;
	EpromFmRadioType            fmRadioType;
	UINT8       				sdmmcSupportType;
	UINT8        				usimSupportType;
    UINT8 			  			spare_3;
    UINT8 			  			spare_4;
    UINT8 			  			irda;
    UINT8 			  			gps;
    UINT8 			  			platformTester;
    UINT8 			  			bt;
    UINT8 			  			atCommand;
    UINT8 			  			spare_5;
    UINT8 			  			spare_6;
    UINT8 			  			iml;
    UINT8 			  			secJtag;
    UINT8 			  			ethernet;
    UINT8 			  			spare_7;
    UINT8 			  			spare_8;
    UINT8 			  			spare_9;
	EpromCard                   card[EPROM_NUM_OF_CARD];
	EpromPlatformEpilogue       platformEpilogue;
}  EPROM_DATA;


#endif
