/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/*--------------------------------------------------------------------------------------------------------------------
INTEL CONFIDENTIAL
Copyright 2006 Intel Corporation All Rights Reserved.
The source code contained or described herein and all documents related to the source code (“Material? are owned
by Intel Corporation or its suppliers or licensors. Title to the Material remains with Intel Corporation or
its suppliers and licensors. The Material contains trade secrets and proprietary and confidential information of
Intel or its suppliers and licensors. The Material is protected by worldwide copyright and trade secret laws and
treaty provisions. No part of the Material may be used, copied, reproduced, modified, published, uploaded, posted,
transmitted, distributed, or disclosed in any way without Intel’s prior express written permission.

No license under any patent, copyright, trade secret or other intellectual property right is granted to or
conferred upon you by disclosure or delivery of the Materials, either expressly, by implication, inducement,
estoppel or otherwise. Any license under such intellectual property rights must be express and approved by
Intel in writing.
-------------------------------------------------------------------------------------------------------------------*/

/************************************************************************/
/*                                                                      */
/* Title: PMC pacakge header file                                       */
/*                                                                      */
/* Filename:  PMChip.h                                                    */
/*                                                                      */
/* Authors: Adi Dayan & Yossi Hanin                                     */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/
#ifndef _PMChip_H_

  #define _PMChip_H_
/*----------------------------------------------------------------------*/

#include "global_types.h"
//#include "PMChip_config.h"

/******** Types ****************/
typedef enum
{
	MAN_ADC_VBAT = 0x00,
	MAN_ADC_ICH,
	MAN_ADC_VCH,
	MAN_ADC_TBAT,
	MAN_ADC_ADC_IN4,
	MAN_ADC_ADC_IN5,
	MAN_ADC_ADC_IN6,
	MAN_ADC_ADC_IN7,
	MAN_ADC_EMPTY = 0xFF
} PMC_adc_reg_t;

typedef void (*PMCNotifyEventFunc)(UINT64 eventRegs);
//for the status register.
typedef void (*PMCGetStatusNotifyFunc)(UINT16 status);
//call back function for the I2C read API
typedef void (*PMCReadCallback)(UINT8 *dataBuffPtr, UINT16 dataSize,UINT16 userId);
//call back function for the I2C write API
typedef void (*PMCWriteCallback)(UINT16 dataBuffPtr);
//for the ADC manual readings   (gil)
typedef void (*PMCGetGPADCValueNotifyFunc)(PMC_adc_reg_t reg,UINT16 value);
typedef void (* ReadingCallback)(int);
typedef void (* LTETempReadingCallback)(unsigned short, unsigned short);

typedef void (* ReadingCallbackBoth)(BOOL,int,int);


#define PMCAdcChannelNumber PMC_adc_reg_t

typedef struct {
	  UINT8 eventA;
	  UINT8 eventB;
	  UINT8 eventC;
	  UINT8 status;
	  UINT8 eventD;
	  UINT8 statusB;
}Registers_ts;

typedef enum
{
    PMC_STATUS_USB,
    PMC_STATUS_ALL
} PMC_Status_t;

typedef enum
{
    PMC_EVENT_USB,
    PMC_EVENT_ONKEY,
    PMC_EVENT_ALL
} PMC_Event_t;

 typedef union
 {
    UINT8   autoControl;
    UINT8   autoControl2;
    UINT8   manControl;
 }adcModeCntrl_t;

// the upper UINT8 is the B event and lower is the A event reg.
typedef union
{
	UINT64 all;
	Registers_ts regs;
} PMCEvents;

typedef struct
{
   UINT8 maskA;
   UINT8 maskB;
   UINT8 maskC;
   UINT8 maskD;
   PMCNotifyEventFunc taskBindFun;
}PMCNotifyEventBindArray_t;

typedef enum
{
	PMC_OK = 0,
	PMC_I2C_ERR
}PMC_ERR_CODE;
/************ PM Chip ID *****************************/
typedef enum             //The names MICCO_B0, PMC_MICCO_B0 are already used with defines.
{
    PMC_TYPE_CCR,
    PMC_TYPE_MICCO_B0,
    PMC_TYPE_MICCO_C0,
	PMC_TYPE_MICCO_C1_DB,
	PMC_TYPE_MICCO_C1_EA,
	PMC_TYPE_MICCO_C1_EB,
    PMC_TYPE_LEVANTE_A0,
    PMC_TYPE_LEVANTE_A1,
    PMC_TYPE_LEVANTE_B0,
    PMC_TYPE_UNKNOWN,  //add new types before this line
    PMC_TYPE_MAX = PMC_TYPE_UNKNOWN
}PMC_TYPE;

typedef enum
{
  MICCO_ID_A0 		= 0x00,
  MICCO_ID_B0       = 0x10,     //The names MICCO_B0, PMC_MICCO_B0 are already used with defines.
  MICCO_ID_C0 		= 0x20,
  MICCO_ID_C1_DB 	= 0x21,
  MICCO_ID_C1_EA 	= 0x30,
  MICCO_ID_C1_EB 	= 0x31,
  LEVANTE_ID_A0 	= 0x40,
  LEVANTE_ID_A1 	= 0x41,
  LEVANTE_ID_B0 	= 0x48
} PMIC_ID;

typedef enum {
    PMC_USIM_1_8V,
    PMC_USIM_3V
} PMC_UsimV_TYPE;

#include "levante_hw.h"
#include "levante.h"

 /******** Prototypes ****************/

#if (defined SILICON_TTC)       //  Temperary until we have PMChip.c for TTC
    #define     PMCGetTemperatureReadings(cALLbACK)         cALLbACK(25)
#else   //  SILICON_TTC
	BOOL    PMCGetTemperatureReadings( ReadingCallback callBackvoid );
#endif  //  SILICON_TTC

void        PMCPhase1Init(void);
void        PMCPhase2Init(void);
void        PMCNvmInit(void);
void        PMCPowerDown(void);
UINT8       PMCGetChipID(void);
UINT8       PMCWriteRegister(UINT8 regToWrite, UINT8 value, UINT16 userID);
UINT8       PMCWriteRegisterBlocking(UINT8 regToWrite, UINT8* writeBuffer, UINT8 size, PMCReadCallback callBackFun, UINT16 userID);
UINT8       PMCWriteRegisterInPageMode(UINT8 regToWrite, UINT8* writeBuffer, UINT8 size, PMCWriteCallback callBackFun, UINT16 userID);
UINT8       PMCReadRegisterNonBlocking(UINT8 regToRead, UINT8* regValue, PMCReadCallback callBackFun, UINT16 userID);
UINT8       PMCReadRegisterBlocking(UINT8 regToRead, UINT8* regValue, UINT16 userID);
UINT8       PMCReadRegisterInPageMode(UINT8 regToRead,UINT8* readBuffer, UINT8 size, PMCReadCallback callBackFun, UINT16 userID);
PMC_TYPE    PMC_GetType(void);
PMC_TYPE    PMC_GetRealType(void);
void        PMC_GetType_ACAT(void);

// PMC notification event (PMIC interrupt) binding
// The mask value given is written into IRQ_MASK_A,B,C (A is lsbyte, then B, and C).
// Mask bits equal to 1 mask the interrupt reason, 0 enable
// Only one bound callback is currently allowed
void        PMCNotifyEventBind (UINT32 mask, PMCNotifyEventFunc pmcBindFunc);
void        PMCSetIrqMask  (UINT32 RegToChange ,UINT8 Function);       // New by Gil 9/12/04

#if (defined SILICON_TTC)       //  Temperary until we have PMChip.c for TTC
    #define     PMCGetTempAndVbatReadings(bOTH,cALLbACK)        cALLbACK(bOTH,25,0)
#else   //  ! SILICON_TTC
    #if defined (FLAVOR_COM)
	BOOL PMCGetVbatReadings( ReadingCallback callBackvoid );
	BOOL PMCGetTemperatureReadingsForLte (LTETempReadingCallback callBack);
	BOOL PMCGetTempAndVbatReadings (BOOL both,ReadingCallbackBoth callBack);
	#else
	void PMCReadRegisters(void);

    void PMCIntLISR(void);
	#endif
#endif  //  SILICON_TTC

/*******************************************************************/
/*                     System control services                     */
/*******************************************************************/

// Shutdown Hermon
void    PMCShutDown(void);
// Reset Hermon
void 	PMCHwReset(void);
// Enable HW reset via On-Key
void    PMCHWResetEnable(void);
// Disable HW reset via On-Key
void    PMCHWResetDisable(void);

/******************/
/* general PMC API */
/******************/

/*indications & monitors*/
BOOL 	PMCGetStatusNonBlocking( PMCGetStatusNotifyFunc pmcBindFunc);
BOOL 	PMCGetGPADCValue(PMC_adc_reg_t reg,PMCGetGPADCValueNotifyFunc bindFunc);
INT32   PMCAdcTempConvert(UINT16 adcVal);
BOOL 	PMCGetDevice	( void );
BOOL    PMCUSBDevicePresent (UINT64 eventRegs);
BOOL    PMCUsbDetection( PMCNotifyEventFunc pmcBindFunc );
BOOL    PMCConfigUsimV(PMC_UsimV_TYPE voltage);
void    PMCEnableUsimV(void);
void    PMCDisableUsimV(void);

//*************************************************************************
//             PMIC_ADC_OFF_ON_WORKAROUND
//*************************************************************************
void  pmicAdcFixStateMachine(void);
#define EVENT_A_VBATLOW   0x00000000
#define EVENT_A_BATTEMP   0x00000000
#define EVENT_A_ONKEY     0x00000000
#define STATUS_ONKEY_VAL  0x00000000

#ifndef LTEONLY_THIN_SINGLE_SIM_2MFLASH
#define PMIC_TASK_STACK_SIZE         2048
#else
#define PMIC_TASK_STACK_SIZE         612
#endif
#define PMIC_TASK_PRIORITY           200
#define PMIC_READ_VBAT_TEMPER        0x1
#define PMIC_READ_VBAT               0x2
#define PMIC_READ_TEMPER             0x4


#endif  /* _PMChip_H_ */
