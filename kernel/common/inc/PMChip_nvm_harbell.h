/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _PMC_NVM_H_
#define _PMC_NVM_H_

//#define NUM_OF_MAX_TEMPRATURE_ZONES  32
#define NUM_OF_MAX_TEMPRATURE_ZONES  166
#define NUM_OF_MIN_TEMPRATURE_ZONES  5
//
// PMC NVM Defaults
//

/****************************************************************************************
 PMC_TAVOR_TEMP_A0_QN 6 /PMC_TAVOR_TEMP_A1_QN 8  ( see bellow)
 * T = ( a1*x + a0)
 * with Q =
 * T = ( a1*x/2^8) + a0/2^6) = ((a1*x) + (a0*2^2))/2^8 =
 * final T = (a1*x + a0<<2)>>8
****************************************************************************************/

#define PMC_NVM_TAVOR_DEFAULT_TEMP_A0 0xF514
#define PMC_NVM_TAVOR_DEFAULT_TEMP_A1 0xDE
#define PMC_NVM_TAVOR_DEFAULT_TEMP_ADCVAL 0xFFFF

#define PMC_TAVOR_TEMP_A0_QN 7
#define PMC_TAVOR_TEMP_A1_QN 10
#define PMC_TAVOR_TEMP_QN 	 3

/***************************************************************************************/
/****************************************************************************************
   VBAT conversion
 * QN = 10.
  ORIGINAL: (2.65/256)*t+2.65 == 2714(t+2^8)/2^18 (no deviation)
 * NEW:   2714(t+2^8)/2^8  == a0*(t+a1)/2^QN  with 2% deviation for not working floating.
****************************************************************************************/
#define PMC_NVM_TAVOR_DEFAULT_VBAT_A0_MICCO  0xA9A
#define PMC_NVM_TAVOR_DEFAULT_VBAT_A1_MICCO  0x100
#define PMC_NVM_TAVOR_DEFAULT_VBAT_QN_MICCO  0x08
//Levante
#define PMC_NVM_TAVOR_DEFAULT_VBAT_A0_LVT  0x1518 /* 5400 = 1.8 * 1000 * 3 */
#define PMC_NVM_TAVOR_DEFAULT_VBAT_A1_LVT  0
#define PMC_NVM_TAVOR_DEFAULT_VBAT_QN_LVT  0x0C   /*12 bit ADC measure*/

/***************************************************************************************/

#include "nvm_header.h"
#include "global_types.h"

/*
 * MACROs
 */
#define NVM_HEAD_SET_STRING(header,field,value) \
   strncpy(header.field,value,sizeof(header.field)-1)

#if 0
//ICAT EXPORTED STRUCT
typedef struct
{
 	INT16  	a0;				// interpreted as signed
 	INT16  	a1;             // interpreted as signed     // t(x)=(a0-a1*x)/(2^QN)  , in 1/8C units
	UINT16  ADCval;         //for calculating the zone
}PMC_AdcTempCalib_ts;
#else
//ICAT EXPORTED STRUCT
typedef struct
{
 	INT32	a0;				// interpreted as signed
 	INT16  	a1;             // interpreted as signed     // t(x)=(a0-a1*x)/(2^QN)  , in 1/8C units
	UINT16  ADCval;         //for calculating the zone
}PMC_AdcTempCalib_ts;
#endif


//ICAT EXPORTED STRUCT
typedef struct
{
 	UINT16  a0;                      // interpreted as signed
 	UINT16  a1;                      // interpreted as signed
 	UINT8   QN;                      // = a0*(t+a1)/2^QN
 	UINT8  reserved;
} PMC_VbatCalib_ts;


//ICAT EXPORTED ENUM
typedef enum PmcAdcFixBehavior_tag
{
    ADC00_BEHAVIOR_RECOVER  = 0,   //default
    ADC00_BEHAVIOR_ASSERT_AT_ONCE,
    ADC00_BEHAVIOR_IGNORE,
    ADC00_BEHAVIOR_ASSERT_AFTER_SILENCE
}PmcAdcFixBehavior_e;


//ICAT EXPORTED STRUCT
typedef struct
{
 UINT8 NumOfZones;
 UINT16 Adc_max;
 PMC_AdcTempCalib_ts adcTempCalib[NUM_OF_MAX_TEMPRATURE_ZONES+1];
 PMC_VbatCalib_ts vbatCalib;
 PmcAdcFixBehavior_e    adcFixBehavior;
 UINT8                  reserved[63];
}PMC_Nvm_File_t;


#endif //_PMC_NVM_H_

