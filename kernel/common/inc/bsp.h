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

//
// BSP API
//

#ifndef _BSP_H_
#define _BSP_H_

#include "gbl_types.h"
#include "intc.h"
#if !defined (SILICON_PV2)
#include "gpio.h"
#endif
#include "PMChip.h"

int   isDebugMode(void);
int   checkRAM(UINT32 addr);

// PIN-MUX AND BOARD BASE INITIALIZATION
void  bspInit(void);

// Tells if the chip is in the internal boot mode
int   bspIsInternalBoot(void);
// Tells if external memory interface is used
BOOL   bspIsExtMemUsed(void);

void bspXscToMsaIntSet(void);

// Tells if BSP is APPS+COMM (e.g. TwoChip) configuration
BOOL bspIsTwoChip(void);

// Initializations needed for specific packages
void bspIpcInit(void);
int  bspGetIpcInts(INTC_InterruptSources intArray[]);
void bspGetPlpFrameInt(	INTC_InterruptSources* frInt1,
						INTC_InterruptSources* frInt2);

// Initializations for PLP (MSA) interface excluding the IPC
void initPlpInterface(void);

#if !defined(SILICON_PV2) //_TAVOR_HARBELL_ /*This "#ifdef" not strongly required*/
                              /* But functions below are not implemented on HARBELL*/
void bspEnableFlashAccess(void);
void bspLcdifInit( UINT16   lcd_num );
GPIO_PinNumbers bspGetLcdResetPin ( UINT16   lcd_num );
void bspPWMInit ( UINT16    pwm_num );
void bspCamifInit( void );
GPIO_PinNumbers bspGetCameraSensorResetPin ( UINT16   sensor_num );
GPIO_PinNumbers bspGetCameraSensorPowerPin ( UINT16   sensor_num ) ;
void bspKeypadInit( void );
void bspGetKeypadSize ( UINT8   *num_of_raw , UINT8   *num_of_columns );
void bspGetColumnList ( UINT8   *keypad_column_list , UINT8    *keypad_num_of_columns);
void bspKeypadColumnSet ( UINT16     column_state );
void bspI2cInit( void );
void bspUART1Init( void );
void bspI2CToNAInit( void );
void bspUART1ClkInit( void );
void setMmcSdPins(void);
void bspIrdaInit( void );
GPIO_PinNumbers bspGetIrdaShdnPin ( void );
UINT16 bspGetLcdPwmDeviceNum( UINT16   lcd_num );
GPIO_PinNumbers bspGetCameraFlashStrobePin ( void );
GPIO_PinNumbers bspGetCameraFlashTriggerPin ( void );
GPIO_PinNumbers       UARTReturnDCDPinNumber(int portNumber);
GPIO_PinNumbers       UARTReturnRIPinNumber (int portNumber);
GPIO_PinNumbers bspGetRfcontPin ( void );
void bspSetRfcontGpio( void );
void bspSetInfineonRfcontGpio( void );
#endif//!_TAVOR_HARBELL_

void bspBTClockReqEnable(BOOL en);

#if defined(_TAVOR_BOERNE_)
void diagSendObmVersion ( void );
#endif

void setMuxWakeUpFFUart(void);
void setMuxWakeUpBTUart(void);
void setMuxWakeUpSTUart(void);
void setMuxGoToSleepFFUart(void);
void setMuxGoToSleepBTUart(void);
void setMuxGoToSleepSTUart(void);

INTC_InterruptSources UARTReturnRXISRSource(int portNumber);
INTC_InterruptSources UARTReturnRTSISRSource(int portNumber);//CTS!
INTC_InterruptSources UARTReturnDTRISRSource(int portNumber);
UINT32 GetTCRTimerCnt(void);
void bspRtcHwReset ( void );



UINT32 bspSspInit( UINT16   ssp_num );


void bspSetGSM(void);
void bspSetWCDMA(void);


/*========================== CHIP ID definition ==================*/
/* Definitions of the possible values of */
#define MDU_MONITOR_Z0	0x0UL
#define MDU_MONITOR_Z1	0x1UL
#define MDU_MONITOR_A0	0x2000UL

/* Definitions of the possible chip IDs , according to  MDU Monitor Register */
typedef enum
{
    TAVOR_SILICON_Z0 = 0,
    TAVOR_SILICON_Z1,
    TAVOR_SILICON_A0,
    TAVOR_SILICON_UNKNOWN
} Tavor_ChipID;

typedef enum
{
	CRANEG_Z1,
	CRANEG_Z2,
	CRANEG_A0,
	FALCON_Z1,
	EAGLE_Z1,
	NUM_OF_UMTS_Chip
}UMTS_Chip_Version;

typedef enum
{
   BOARD_COTULLA,
   BOARD_THELON_DVK,
   BOARD_HERMON_EVB,
   BOARD_HERMON_DVK,    //SDK for B0
   BOARD_MANITOBA,
   BOARD_MATHIS,
   BOARD_TAVOR,
   BOARD_TAVOR_Z0,
   BOARD_TAVOR_Z1,
   BOARD_TAVOR_A0,
   BOARD_TAVOR_B0,
   BOARD_UNKNOWN
} BspBoardType;



BspBoardType bspBoardType(void);

#ifdef PHS_SW_DEMO_TTC
typedef enum
{
	ASR_EVB,
	CUSTOM_NOKIA,
	CUSTOM_EVB3,
	TIGX_MIFI,
	CUSTOM_3,
	CUSTOM_4,
	CUSTOM_FF_P1,
	CUSTOM_1920,
	CUSTOM_7,
	CUSTOM_SAARN,
	CUSTOM_9,
	CUSTOM_10,
	CUSTOM_11,
	CUSTOM_12,
	CUSTOM_SIM2,
	BOARD_1,
	BOARD_2,
	BOARD_3,
	BOARD_4,
	BOARD_5,
	BOARD_6,
	BOARD_7,
	BOARD_8,
	BOARD_9,
	BOARD_10,
	BOARD_11,
	BOARD_12,

}BspCustomType;





/*typedef enum
{
	CUSTOM_DialUp,
	CUSTOM_TwoChip,
	CUSTOM_ShareMemory,
}BspCustomType;*/

BspCustomType bspGetBoardType(void);
BOOL bspBoardIsDKB(void);
const char* bspGetBoardName(void);
typedef enum
{
	AC_SHM,
	AC_HSIC,
	AC_HSI,
	AC_SPI,
	AC_NONE,
}AcLinkType;

const char* GetBuildVersion(void);
AcLinkType bspGetAcLinkType(void);
unsigned int GetChipID(void);
unsigned int GetLongChipID(void);
unsigned long GetPlatformType(void);

extern BOOL IsChipCrane(void);
extern BOOL IsChipCrane_A0(void);
extern BOOL IsChipCrane_A1(void);
extern BOOL IsChipCraneG(void);
extern BOOL IsChipCraneG_Z1(void);
extern BOOL IsChipCraneG_Z2(void);
extern BOOL IsChipCraneG_A0(void);
extern BOOL IsChipCraneG_A1(void);
extern BOOL IsChipCraneG_AX(void);
extern BOOL IsChipCraneM(void);
extern BOOL IsChipCraneM_Z1(void);
extern BOOL IsChipCraneM_Z2(void);
extern BOOL IsChipCraneM_A0(void);
extern BOOL IsChipCraneM_A1(void);
extern BOOL IsChipCraneL(void);
extern BOOL IsChipCraneL_Z1(void);
extern BOOL IsChipCraneL_A0(void);
extern BOOL IsChipCraneLS(void);
extern BOOL IsChipCraneLS_Z1(void);
extern BOOL IsChipCraneLS_A0(void);
extern BOOL IsChipCraneGT(void);
extern BOOL IsChipCraneGT_Z1(void);
extern BOOL IsChipCraneLR(void);
extern BOOL IsChipCraneLG(void);

extern BOOL IsChip_Crane_A0_or_Above(void);
extern BOOL IsChip_CraneG_A0_or_Above(void);
extern BOOL IsChip_CraneM_A0_or_Above(void);
extern BOOL IsChip_CraneM_A7_or_Above(void);

//for AT modem port config , DUAL_AT_MODEM:
//[TRUE  ]:dual usb at port , dual usb at channel
//[FALSE ]:single usb at port , but could use cmd "AT&M1" or "AT&M2" to switch
#define DUAL_AT_MODEM TRUE

#define LONG_CHIP_ID GetLongChipID()

#define AC_IS_SHM			(bspGetAcLinkType() == AC_SHM)
#define AC_IS_2CHIP_SPI		(bspGetAcLinkType() == AC_SPI)
#define AC_IS_2CHIP_HSI		(bspGetAcLinkType() == AC_HSI)
#define AC_IS_2CHIP_HSIC 	(bspGetAcLinkType() == AC_HSIC)
#define AC_IS_2CHIP			(AC_IS_2CHIP_SPI || AC_IS_2CHIP_HSI || AC_IS_2CHIP_HSIC)
#define AC_USED_MUX			(bspGetAcLinkType() == AC_SPI)
#define AC_IS_CPONLY		(bspGetAcLinkType() == AC_NONE)

#define CHIP_IS_CRANE		(IsChipCrane())
#define CHIP_IS_CRANE_A0	(IsChipCrane_A0())
#define CHIP_IS_CRANE_A1	(IsChipCrane_A1())

#define CHIP_IS_CRANEG		(IsChipCraneG())
#define CHIP_IS_CRANEG_Z1	(IsChipCraneG_Z1())
#define CHIP_IS_CRANEG_Z2	(IsChipCraneG_Z2())
#define CHIP_IS_CRANEG_A0	(IsChipCraneG_A0())
#define CHIP_IS_CRANEG_A1	(IsChipCraneG_A1())
#define CHIP_IS_CRANEG_AX	(IsChipCraneG_AX())

#define CHIP_IS_CRANEM		(IsChipCraneM())
#define CHIP_IS_CRANEM_Z1	(IsChipCraneM_Z1())
#define CHIP_IS_CRANEM_Z2	(IsChipCraneM_Z2())
#define CHIP_IS_CRANEM_A0	(IsChipCraneM_A0())
#define CHIP_IS_CRANEM_A1	(IsChipCraneM_A1())

#define CHIP_IS_CRANEL		(IsChipCraneL())
#define CHIP_IS_CRANEL_Z1	(IsChipCraneL_Z1())
#define CHIP_IS_CRANEL_A0	(IsChipCraneL_A0())

#define CHIP_IS_CRANELS		(IsChipCraneLS())
#define CHIP_IS_CRANELS_Z1	(IsChipCraneLS_Z1())
#define CHIP_IS_CRANELS_A0	(IsChipCraneLS_A0())

#define CHIP_IS_CRANELR		(IsChipCraneLR())
#define CHIP_IS_CRANEGT		(FALSE)
#define CHIP_IS_CRANELG		(IsChipCraneLG())

//embeded dsp : 1606/3602/1609/1602
#define CHIP_IS_EMBEDED_DSP (CHIP_IS_CRANEL || CHIP_IS_CRANELS || CHIP_IS_CRANELR || CHIP_IS_CRANELG)


#define PLATFORM_IS_ASIC    (GetPlatformType() == 0x0)
#define PLATFORM_IS_FPGA    (GetPlatformType() == 0x1)
#define PLATFORM_IS_ZEBU    (GetPlatformType() == 0x2) /* include Z1 */

#endif

typedef enum
{
	MEMC_CLOCK_156,
	MEMC_CLOCK_104,
	MEMC_CLOCK_78,
	MEMC_CLOCK_52
} MemcClockRate;

MemcClockRate bspGetMemcClockRate(void);

typedef enum
{
	I2S_TRI_STATE,
	PCM_TRI_STATE,
	SSP_TRI_STATE
}TriStateIF;


#if !defined(SILICON_PV2) //_TAVOR_HARBELL_
typedef enum
{
	EXT_FLASH_16_BIT_WIDTH = 16,
	EXT_FLASH_32_BIT_WIDTH = 32
}ExtFlashBitWidth;

BOOL isFlashSpeed78( void );  //Tells if flash speed is 78MHz according to runtime configuration.

ExtFlashBitWidth bspGetExFlashBusWidth( void );

// USB client activation (with cable detection)
void bspUsbActivate(void);
// USB detect phase1 charge of PMIC interrupt binding. must be could after PMICphase1.
void bspUsbDetectPhase1( void );

#endif//!_TAVOR_HARBELL_

void bspAudioMux (TriStateIF Interface, BOOL Active);

void bspGetFlashParams(void);


typedef struct
{
    UINT32 totalCapacity;      // Total number of bytes
    UINT16 widthCode;          // 8bit, 16bit, 32bit
    UINT16 writeBufferSizeLog; // Write buffer maximal size (in bytes) = 2**writeBufferSizeLog
    UINT16 nRegions;
    UINT16 nBlocks1;
    UINT32 blockSize1;         // in bytes
    UINT16 nBlocks2;
    UINT32 blockSize2;         // in bytes
} FlashParameters;

//
// Shutdown Request
// Handled by the low priority event handling task
// Allows the FDI to complete the write/erase operations and then shuts down the power via PMIC
//
// The given <type> is SHD_POWER_DOWN the system is shutdown via PMIC
//
// In all other cases the system is reset and the given <type> value is available
// at the next startup as swResetType in the StartInfo_ts structure below.

//ICAT EXPORTED ENUM
typedef enum
{
  SHD_POWER_DOWN,
  SHD_RESET,
  SHD_GHOST,
  SHD_SW_ERROR /* EEHandler triggered the reset */
} ShutDownType_te;

void bspShutDown(ShutDownType_te type);

//
// Start Up reasons and startup sequence control
//

//ICAT EXPORTED ENUM
typedef enum
{
    RR_NORMAL_POWER_ON       = 0x00,    // default, not combined with others
    RR_WATCH_DOG_TIMEOUT     = 0x01,
    RR_SOFTWARE_GENERATED    = 0x02,
    RR_CHARGING_BATTERY      = 0x04,
    RR_LOW_BATTERY           = 0x08,
    RR_ALARM_POWER_ON        = 0x10,
    RR_EXT_POWER_ON          = 0x20
}
StartupReason_te;

//ICAT EXPORTED ENUM
typedef enum
{
  RE_RTC_ALARM               = 0x01
} StartupExtInd_te;


typedef struct
{
  UINT8            type;        // StartupReason_te values OR'ed
  ShutDownType_te  swResetType; // any 8-bit value
  //Other inidications
  UINT8            extState;            // StartupExtIndications_te values OR'ed
  //Arava initial state captured
  UINT8            pmcFaultLog; // Arava FAULT_LOG
  // pmcEvents follow the layout of PMCEvents (PMChip.h) to allow 32-bit representation
  union
  {
	UINT32 all;
    Registers_ts regs;
  } pmcEvents;
} StartupInfo_ts;


typedef enum
{
    CCR_USING_I2C     = 0x0002,      //MODES NOT COBINED
    CCR_USING_SSP     = 0x0003
}CcrComMode_te;

typedef struct
{
    UINT8	TxClkDelay;					// Moves Tx clock forward in 3ns steps. Range: 0x00- 0x07.
	UINT8 	RxEarlyClkDelay;        	// Moves 30.72Mhz DBB sampling clock backwards in 3ns step. Range: 0x00- 0x07
	UINT8	InternSamplingInversion;	// Swaps IQ samples. 0=No inversion. 1=Inversion.
	UINT8	NumOfSamplePerChip;			// '0': 4 samples per chip (15.36 MHz). '1': 2 samples per chip (7.68 MHz)
	UINT8	RxClkPolarity2SampPerChip;	// Inverts RX clock polarity when DP mode is set to 2 samples per chip. 0=Normal. 1=Invert.
	UINT8	RxClkPolarity4SampPerChip;	// Inverts RX clock polarity when DP mode is set to 4 samples per chip. 0=Normal. 1=Invert.
    UINT8 	TxClockPolarity;			// Inverts TX clock polarity. 0=Normal. 1=Invert.
} CcrDpParams_ts;

CcrComMode_te  bspCCRcomMode(void);

void bspCcrDpParams(CcrDpParams_ts *DpParams_p);

// Startup reason query service
// Retrieves the StartupInfo applicable for the recent system startup (and recent shutdown when relevant)
void bspGetStartupReason(StartupInfo_ts* info);

void bspSwResetReasonSet(UINT8 rr);
UINT8 bspSwResetReasonGetClear(void);

//Initialization stage state machine
typedef enum
{
	BSP_INIT_PROCESS_START=0,
	BSP_INIT_CINIT1_PASSED,
	BSP_INIT_CINIT2_PASSED,
	BSP_INIT_PHASE1_PASSED,
	BSP_INIT_PHASE2_BASIC_PASSED,
	BSP_INIT_PHASE2_PASSED,
	BSP_INIT_TASK_FINISH /*this must be the last state*/
}BSP_InitStateE;

extern /*BSP_InitStateE*/UINT32 initState;

UMTS_Chip_Version GetUmtsChipVersion(void);

#endif //_BSP_H_

