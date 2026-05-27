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

/*********************************************************************
*                      M O D U L E     B O D Y                       *
**********************************************************************
*                                                                    *
* Title: GPIO (General Purpose Input Output) Header File             *
*                                                                    *
* Author: Yossi Gabay                                                *
*         Eilam Ben-Dror                                             *
*         Tal Rotbart	                                             *
*                                                                    *
* Target, subsystem: Common Platform, HAL                            *
*                                                                    *
* Yaeli Karni		Mar 2006, Enhance driver to support Boerne       *
*                                                                    *
*********************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "global_types.h"
#ifndef SILICON_PV2
#include "gpio_config.h"
#include "gpio_hw.h"
#else //_TAVOR_HARBELL_

#include "cgpio_HW.h"
#endif //_TAVOR_HARBELL_

/*-- Type definitions (T type) -----------------------------------------*/
//ICAT EXPORTED ENUM
typedef enum
{
 GPIO_PIN_NOT_ASSIGNED = -1,

 GPIO_PIN_0=0,GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,
 GPIO_PIN_8,  GPIO_PIN_9,  GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15,
 GPIO_PIN_16, GPIO_PIN_17, GPIO_PIN_18, GPIO_PIN_19, GPIO_PIN_20, GPIO_PIN_21, GPIO_PIN_22, GPIO_PIN_23,
 GPIO_PIN_24, GPIO_PIN_25, GPIO_PIN_26, GPIO_PIN_27, GPIO_PIN_28, GPIO_PIN_29, GPIO_PIN_30, GPIO_PIN_31,
 GPIO_PIN_32,
#ifndef SILICON_PV2
 GPIO_PIN_33, GPIO_PIN_34, GPIO_PIN_35, GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38, GPIO_PIN_39,
 GPIO_PIN_40, GPIO_PIN_41, GPIO_PIN_42, GPIO_PIN_43, GPIO_PIN_44, GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47,
 GPIO_PIN_48, GPIO_PIN_49, GPIO_PIN_50, GPIO_PIN_51, GPIO_PIN_52, GPIO_PIN_53, GPIO_PIN_54, GPIO_PIN_55,
 GPIO_PIN_56, GPIO_PIN_57, GPIO_PIN_58, GPIO_PIN_59, GPIO_PIN_60, GPIO_PIN_61, GPIO_PIN_62, GPIO_PIN_63,
#endif //_TAVOR_HARBELL_

#if defined(GPIO_128_ENTRIES)
 GPIO_PIN_64, GPIO_PIN_65, GPIO_PIN_66, GPIO_PIN_67, GPIO_PIN_68, GPIO_PIN_69, GPIO_PIN_70, GPIO_PIN_71,
 GPIO_PIN_72, GPIO_PIN_73, GPIO_PIN_74, GPIO_PIN_75, GPIO_PIN_76, GPIO_PIN_77, GPIO_PIN_78, GPIO_PIN_79,
 GPIO_PIN_80, GPIO_PIN_81, GPIO_PIN_82, GPIO_PIN_83, GPIO_PIN_84, GPIO_PIN_85, GPIO_PIN_86, GPIO_PIN_87,
 GPIO_PIN_88, GPIO_PIN_89, GPIO_PIN_90, GPIO_PIN_91, GPIO_PIN_92, GPIO_PIN_93, GPIO_PIN_94, GPIO_PIN_95,
 GPIO_PIN_96, GPIO_PIN_97, GPIO_PIN_98, GPIO_PIN_99, GPIO_PIN_100, GPIO_PIN_101, GPIO_PIN_102, GPIO_PIN_103,
 GPIO_PIN_104, GPIO_PIN_105, GPIO_PIN_106, GPIO_PIN_107, GPIO_PIN_108, GPIO_PIN_109, GPIO_PIN_110, GPIO_PIN_111,
 GPIO_PIN_112, GPIO_PIN_113, GPIO_PIN_114, GPIO_PIN_115, GPIO_PIN_116, GPIO_PIN_117, GPIO_PIN_118, GPIO_PIN_119,
 GPIO_PIN_120, GPIO_PIN_121, GPIO_PIN_122, GPIO_PIN_123, GPIO_PIN_124, GPIO_PIN_125, GPIO_PIN_126, GPIO_PIN_127,
#endif //INTEL_2CHIP_PLAT_BVD
 GPIO_MAX_AMOUNT_OF_PINS

#if defined(_EXTENDED_GPIO_)
 , EGPIO_PIN_0= GPIO_MAX_AMOUNT_OF_PINS,
 EGPIO_PIN_1,  EGPIO_PIN_2,  EGPIO_PIN_3,  EGPIO_PIN_4,  EGPIO_PIN_5,  EGPIO_PIN_6,  EGPIO_PIN_7,
 EGPIO_PIN_8,  EGPIO_PIN_9,  EGPIO_PIN_10, EGPIO_PIN_11, EGPIO_PIN_12, EGPIO_PIN_13, EGPIO_PIN_14,
 EGPIO_PIN_15, EGPIO_PIN_16, EGPIO_PIN_17, EGPIO_PIN_18, EGPIO_PIN_19, EGPIO_PIN_20, EGPIO_PIN_21,
 EGPIO_PIN_22, EGPIO_PIN_23, EGPIO_PIN_24, EGPIO_PIN_25, EGPIO_PIN_26, EGPIO_PIN_27, EGPIO_PIN_28,
 EGPIO_PIN_29, EGPIO_PIN_30, EGPIO_PIN_31, EGPIO_PIN_32, EGPIO_PIN_33, EGPIO_PIN_34, EGPIO_PIN_35,

 EGPIO_PIN_36, EGPIO_PIN_37, EGPIO_PIN_38, EGPIO_PIN_39, EGPIO_PIN_40, EGPIO_PIN_41, EGPIO_PIN_42,
 EGPIO_PIN_43, EGPIO_PIN_44, EGPIO_PIN_45, EGPIO_PIN_46, EGPIO_PIN_47, EGPIO_PIN_48, EGPIO_PIN_49,
 EGPIO_PIN_50, EGPIO_PIN_51, EGPIO_PIN_52, EGPIO_PIN_53, EGPIO_PIN_54, EGPIO_PIN_55, EGPIO_PIN_56,
 EGPIO_PIN_57, EGPIO_PIN_58, EGPIO_PIN_59, EGPIO_PIN_60, EGPIO_PIN_61, EGPIO_PIN_62, EGPIO_PIN_63,
 EGPIO_PIN_64, EGPIO_PIN_65, EGPIO_PIN_66, EGPIO_PIN_67, EGPIO_PIN_68, EGPIO_PIN_69, EGPIO_PIN_70, EGPIO_PIN_71,
 EGPIO_MAX_AMOUNT_OF_PINS
#endif /* _EXTENDED_GPIO_ */

}GPIO_PinNumbers;

// Return code of GPIO package operations
//ICAT EXPORTED ENUM
typedef enum
{
    GPIO_RC_OK = 1,

    GPIO_RC_INVALID_PORT_HANDLE = -100,
    GPIO_RC_NOT_OUTPUT_PORT,
    GPIO_RC_NO_TIMER,
    GPIO_RC_NO_FREE_HANDLE,
    GPIO_RC_AMOUNT_OUT_OF_RANGE,
    GPIO_RC_INCORRECT_PORT_SIZE,
    GPIO_RC_PORT_NOT_ON_ONE_REG,
    GPIO_RC_INVALID_PIN_NUM,
    GPIO_RC_PIN_USED_IN_PORT,
    GPIO_RC_PIN_NOT_FREE,
    GPIO_RC_PIN_NOT_LOCKED,
    GPIO_RC_NULL_POINTER,
    GPIO_RC_PULLED_AND_OUTPUT,
	GPIO_RC_INCORRECT_PORT_TYPE,
	GPIO_RC_INCORRECT_TRANSITION_TYPE,
	GPIO_RC_INCORRECT_DEBOUNCE,
	GPIO_RC_INCORRECT_DIRECTION,
	GPIO_RC_INCORRECT_INIT_VALUE
#ifdef SILICON_PV2
	, GPIO_RC_INTC_ERROR,
	GPIO_RC_PRM_ERROR
#endif //_TAVOR_HARBELL_
}GPIO_ReturnCode;

//ICAT EXPORTED ENUM
typedef enum
{
    GPIO_INPUT_PIN = 1,
    GPIO_OUTPUT_PIN
}GPIO_PinDirection;


typedef enum
{
    GPIO_DEBOUNCE_DISABLE = 0,
    GPIO_DEBOUNCE_ENABLE
}GPIO_Debounce;

typedef enum
{
    GPIO_NO_DETECTION = 0,
    GPIO_RISING_EDGE,
    GPIO_FALLING_EDGE,
    GPIO_BOTH_EDGE,
    GPIO_HIGH_LEVEL,
    GPIO_LOW_LEVEL,
    GPIO_BOTH_LEVEL
}GPIO_TransitionType;

//ICAT EXPORTED ENUM
typedef enum
{
    GPIO_PIN_FREE_FOR_USE = 0,
    GPIO_PIN_USE_IN_PORT,
    GPIO_PIN_USE_IN_INTERRUPT,
    GPIO_PIN_USE_IN_PORT_WITH_INTERRUPT,
    GPIO_PIN_LOCKED
}GPIO_PinUsage;

//ICAT EXPORTED STRUCT
typedef struct
{
    GPIO_PinUsage           pinUsage;
    GPIO_PinDirection       direction;
}GPIO_PinStatus;

//ICAT EXPORTED ENUM
typedef enum
{
    GPIO_INITIAL_VALUE_NO_CHANGE = 0,
    GPIO_INITIAL_VALUE_LOW,
    GPIO_INITIAL_VALUE_HIGH
}GPIO_BitInitialValue;

//ICAT EXPORTED ENUM
typedef enum
{
    GPIO_PULL_UP_DOWN_DISABLE = 0,
    GPIO_PULL_UP_ENABLE,
    GPIO_PULL_DOWN_ENABLE
}GPIO_PullUpDown;

//ICAT EXPORTED STRUCT
typedef struct
{
    GPIO_PinNumbers             pinNumber;
    GPIO_PinDirection           direction;
    GPIO_TransitionType         transitionType;
    GPIO_Debounce               debounce;
    GPIO_PullUpDown             pullUpDown;
    GPIO_BitInitialValue        initialValue;
}GPIO_PinConfiguration;

typedef UINT8        GPIO_PortHandle;

#ifdef SILICON_PV2
typedef struct
{
	//this is reserved for passing more parameters if needed when HW changes.
	UINT32	reserved1;
}GPIO_IntcInfo;

typedef void (*GPIO_ISR)(void);
#endif //_TAVOR_HARBELL_
/*----------------------------------------------------------------------*/


/*-- Constants definitions (C type) ------------------------------------*/

/*----------------------------------------------------------------------*/


/*-- Global variables declarations (Public G type) ---------------------*/


/*---Definitions for backward compatability ----------------------------*/

/*-- Public Functions Prototypes ---------------------------------------*/
GPIO_ReturnCode GPIOPhase1Init(UINT8 amountPins, GPIO_PinConfiguration config[],
                               GPIO_PinNumbers *pinNum);
void            GPIOPhase2Init(void);

GPIO_ReturnCode GPIOConfigure(UINT8 amountPins, GPIO_PinConfiguration config[],
                         GPIO_PinNumbers *pinNum);
GPIO_ReturnCode GPIOConfigurationGet(UINT8 amountPins, GPIO_PinNumbers pinsArray[],
                                     GPIO_PinConfiguration config[]);
GPIO_ReturnCode GPIOPortControl(GPIO_PortHandle portHandle, GPIO_PinDirection dir);

GPIO_ReturnCode GPIOPortOpen(UINT8 portSize, GPIO_PinNumbers firstPin,
                             GPIO_PinDirection dir, UINT32 portInitValue, GPIO_PortHandle *portHandle);
GPIO_ReturnCode GPIOPortWithInterruptOpen(UINT8 portSize, GPIO_PinNumbers firstPin,
                                          GPIO_PinDirection dir, UINT32 portInitValue, GPIO_PortHandle *portHandle);

GPIO_ReturnCode GPIOPortClose(GPIO_PortHandle portHandle);

GPIO_ReturnCode GPIOStatusGet(GPIO_PinNumbers pinNumber, GPIO_PinStatus *status);


GPIO_ReturnCode GPIOPortWrite(GPIO_PortHandle portHandle, UINT32 value);
GPIO_ReturnCode GPIOPortRead(GPIO_PortHandle portHandle, UINT32 *value);

GPIO_ReturnCode GPIOPinLock(GPIO_PinNumbers pinNum);
GPIO_ReturnCode GPIOPinUnlock(GPIO_PinNumbers pinNum);

SwVersion       GPIOVersionGet(void);

#ifdef SILICON_PV2
GPIO_ReturnCode GPIOPortBind(GPIO_PortHandle portHandle, GPIO_ISR isr, GPIO_IntcInfo *config); //config is reserved. pass it as NULL.
GPIO_ReturnCode GPIOPortIntcDisable(GPIO_PortHandle portHandle);
GPIO_ReturnCode GPIOPortIntcEnable(GPIO_PortHandle portHandle);
GPIO_ReturnCode GPIOPortIntcClear(GPIO_PortHandle portHandle);
#endif   //_TAVOR_HARBELL_

#endif  /* _GPIO_H_ */


