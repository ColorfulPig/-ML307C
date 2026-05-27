/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

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
*********************************************************************/

#ifndef _GPIO_H_
#define _GPIO_H_

#include "global_types.h"
#include "hal_cfg.h"



/*-- Type definitions (T type) -----------------------------------------*/
typedef enum
{
 GPIO_PIN_0=0,GPIO_PIN_1,  GPIO_PIN_2,  GPIO_PIN_3,  GPIO_PIN_4,  GPIO_PIN_5,  GPIO_PIN_6,  GPIO_PIN_7,
 GPIO_PIN_8,  GPIO_PIN_9,  GPIO_PIN_10, GPIO_PIN_11, GPIO_PIN_12, GPIO_PIN_13, GPIO_PIN_14, GPIO_PIN_15,
 GPIO_PIN_16, GPIO_PIN_17, GPIO_PIN_18, GPIO_PIN_19, GPIO_PIN_20, GPIO_PIN_21, GPIO_PIN_22, GPIO_PIN_23,
 GPIO_PIN_24, GPIO_PIN_25, GPIO_PIN_26, GPIO_PIN_27, GPIO_PIN_28, GPIO_PIN_29, GPIO_PIN_30, GPIO_PIN_31,

 GPIO_PIN_32, GPIO_PIN_33, GPIO_PIN_34, GPIO_PIN_35, GPIO_PIN_36, GPIO_PIN_37, GPIO_PIN_38, GPIO_PIN_39,
 GPIO_PIN_40, GPIO_PIN_41, GPIO_PIN_42, GPIO_PIN_43, GPIO_PIN_44, GPIO_PIN_45, GPIO_PIN_46, GPIO_PIN_47,
 GPIO_PIN_48, GPIO_PIN_49, GPIO_PIN_50, GPIO_PIN_51, GPIO_PIN_52, GPIO_PIN_53, GPIO_PIN_54, GPIO_PIN_55,
 GPIO_PIN_56, GPIO_PIN_57, GPIO_PIN_58, GPIO_PIN_59, GPIO_PIN_60, GPIO_PIN_61, GPIO_PIN_62, GPIO_PIN_63,

 GPIO_MAX_AMOUNT_OF_PINS

#ifdef _EXTENDED_GPIO_
 , EGPIO_PIN_0= GPIO_MAX_AMOUNT_OF_PINS,
 EGPIO_PIN_1,  EGPIO_PIN_2,  EGPIO_PIN_3,  EGPIO_PIN_4,  EGPIO_PIN_5,  EGPIO_PIN_6,  EGPIO_PIN_7,
 EGPIO_PIN_8,  EGPIO_PIN_9,  EGPIO_PIN_10, EGPIO_PIN_11, EGPIO_PIN_12, EGPIO_PIN_13, EGPIO_PIN_14,
 EGPIO_PIN_15, EGPIO_PIN_16, EGPIO_PIN_17, EGPIO_PIN_18, EGPIO_PIN_19, EGPIO_PIN_20, EGPIO_PIN_21,
 EGPIO_PIN_22, EGPIO_PIN_23, EGPIO_PIN_24, EGPIO_PIN_25, EGPIO_PIN_26, EGPIO_PIN_27, EGPIO_PIN_28,
 EGPIO_PIN_29, EGPIO_PIN_30, EGPIO_PIN_31, EGPIO_PIN_32, EGPIO_PIN_33, EGPIO_PIN_34, EGPIO_PIN_35,

#if defined(_HERMON_B0_SILICON_)
 EGPIO_PIN_36, EGPIO_PIN_37, EGPIO_PIN_38, EGPIO_PIN_39, EGPIO_PIN_40, EGPIO_PIN_41, EGPIO_PIN_42,
 EGPIO_PIN_43, EGPIO_PIN_44, EGPIO_PIN_45, EGPIO_PIN_46, EGPIO_PIN_47, EGPIO_PIN_48, EGPIO_PIN_49,
 EGPIO_PIN_50, EGPIO_PIN_51, EGPIO_PIN_52, EGPIO_PIN_53, EGPIO_PIN_54, EGPIO_PIN_55, EGPIO_PIN_56,
 EGPIO_PIN_57, EGPIO_PIN_58, EGPIO_PIN_59, EGPIO_PIN_60, EGPIO_PIN_61, EGPIO_PIN_62, EGPIO_PIN_63,
 EGPIO_PIN_64, EGPIO_PIN_65, EGPIO_PIN_66, EGPIO_PIN_67, EGPIO_PIN_68, EGPIO_PIN_69, EGPIO_PIN_70, EGPIO_PIN_71,
#endif /* _HERMON_B0_SILICON_ */

 EGPIO_MAX_AMOUNT_OF_PINS

#endif /* _EXTENDED_GPIO_ */

}GPIO_PinNumbers;

// Return code of GPIO package operations
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
	GPIO_RC_INCORRECT_DEBOUNCE
}GPIO_ReturnCode;


typedef enum
{
    GPIO_INPUT_PIN = 1,// starting from 1 on purpose, to be compliant with Legacy code
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

typedef enum
{
    GPIO_PIN_FREE_FOR_USE = 0,
    GPIO_PIN_USE_IN_PORT,
    GPIO_PIN_USE_IN_INTERRUPT,
    GPIO_PIN_USE_IN_PORT_WITH_INTERRUPT,
    GPIO_PIN_LOCKED
}GPIO_PinUsage;

typedef struct
{
    GPIO_PinUsage           pinUsage;
    GPIO_PinDirection       direction;
}GPIO_PinStatus;


typedef enum
{
    GPIO_INITIAL_VALUE_NO_CHANGE = 0,
    GPIO_INITIAL_VALUE_LOW,
    GPIO_INITIAL_VALUE_HIGH
}GPIO_BitInitialValue;

typedef enum
{
    GPIO_PULL_UP_DOWN_DISABLE = 0,
    GPIO_PULL_UP_ENABLE,
    GPIO_PULL_DOWN_ENABLE
}GPIO_PullUpDown;

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

#endif  /* _GPIO_H_ */


