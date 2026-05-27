#ifndef __NGPIO_H__
#define __NGPIO_H__

typedef void (*GPIOCallback)(void);

typedef enum
{
	GPIORC_FALSE = 0,
	GPIORC_TRUE = 1,
	GPIORC_LOW = 0,
	GPIORC_HIGH = 1,

	GPIORC_OK = 0,
    GPIORC_INVALID_PORT_HANDLE = -100,
    GPIORC_NOT_OUTPUT_PORT,
    GPIORC_NO_TIMER,
    GPIORC_NO_FREE_HANDLE,
    GPIORC_AMOUNT_OUT_OF_RANGE,
    GPIORC_INCORRECT_PORT_SIZE,
    GPIORC_PORT_NOT_ON_ONE_REG,
    GPIORC_INVALID_PIN_NUM,
    GPIORC_PIN_USED_IN_PORT,
    GPIORC_PIN_NOT_FREE,
    GPIORC_PIN_NOT_LOCKED,
    GPIORC_NULL_POINTER,
    GPIORC_PULLED_AND_OUTPUT,
	GPIORC_INCORRECT_PORT_TYPE,
	GPIORC_INCORRECT_TRANSITION_TYPE,
	GPIORC_INCORRECT_DEBOUNCE,
	GPIORC_INCORRECT_DIRECTION,
	GPIORC_INCORRECT_PULL,
	GPIORC_INCORRECT_INIT_VALUE,
	GPIORC_WRITE_TO_INPUT
}GPIOReturnCode;

typedef enum
{
    GPIO_IN_PIN = 0,
    GPIO_OUT_PIN = 1
}GPIOPinDirection;

typedef enum
{
    GPIO_PULL_DISABLE = 0,
    GPIO_PULLUP_ENABLE,
    GPIO_PULLDN_ENABLE
}GPIOPullUpDown;

typedef enum
{
    GPIO_NO_EDGE = 0,
    GPIO_RISE_EDGE,
    GPIO_FALL_EDGE,
    GPIO_TWO_EDGE,
}GPIOTransitionType;


typedef struct
{
	GPIOPinDirection pinDir;
	UINT32			initLv;
	GPIOPullUpDown 	pinPull;
	GPIOTransitionType pinEd;
	GPIOCallback isr;
	GPIOCallback wu;
} GPIOConfiguration;

/***********************************************************************
* Function: GpioSetLevel                                              *
************************************************************************
* Description: Writes a value to a port					               *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				value - the value to be written						   *
*                                                                      *
* Return value: GPIORC_OK		                                       *
*				GPIORC_INVALID_PORT_HANDLE
*				GPIORC_INCORRECT_INIT_VALUE
*				GPIORC_WRITE_TO_INPUT								   *
*                                                                      *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioSetLevel(UINT32 portHandle, UINT32 value);

/***********************************************************************
* Function: GpioGetLevel                                              *
************************************************************************
* Description: Get current level of a port					           *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*										   							   *
*                                                                      *
* Return value: GPIORC_LOW
				GPIORC_HIGH  										   *
*				GPIORC_INVALID_PORT_HANDLE
*
*			                                            			   *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioGetLevel(UINT32 portHandle);

/***********************************************************************
* Function: GpioEnableEdgeDetection                                    *
************************************************************************
* Description: Enable edge detection function of a port		           *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				edge - the edge detection type						   *
*                                                                      *
* Return value: GPIORC_OK
				GPIORC_INVALID_PORT_HANDLE
				GPIORC_INCORRECT_TRANSITION_TYPE					   *
*				GPIORC_PIN_NOT_FREE
*
*			                                            			   *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioEnableEdgeDetection(UINT32 portHandle,GPIOTransitionType edge);

/***********************************************************************
* Function: GpioDisableEdgeDetection                                   *
************************************************************************
* Description: Disable edge detection function of a port		       *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				edge - the edge detection type						   *
*                                                                      *
* Return value: GPIORC_OK
				GPIORC_INVALID_PORT_HANDLE
				GPIORC_INCORRECT_TRANSITION_TYPE					   *
*
*
*			                                            			   *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioDisableEdgeDetection(UINT32 portHandle, GPIOTransitionType edge);

/***********************************************************************
* Function: GpioIsEdgeDetected                                   	   *
************************************************************************
* Description: check the pending bit of edge detection ofr a port      *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				                                                       *
* Return value: GPIORC_OK
				GPIORC_INVALID_PORT_HANDLE
																	   *
*			                                            			   *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioIsEdgeDetected(UINT32 portHandle);

/***********************************************************************
* Function: GpioClearEdgeDetection                                     *
************************************************************************
* Description: clear the pending bit of edge detection ofr a port      *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				                                                       *
* Return value: GPIORC_OK
				GPIORC_INVALID_PORT_HANDLE
																	   *
*			                                            			   *
* Notes:                                                               *
***********************************************************************/
GPIOReturnCode GpioClearEdgeDetection(UINT32 portHandle);

/***********************************************************************
* Function: GpioSetDirection                                           *
************************************************************************
* Description: Controls the direction of a given port                  *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				dir - the required direction						   *
*                                                                      *
* Return value: GPIORC_OK		                                       *
*				GPIORC_INVALID_PORT_HANDLE						       *
*               GPIORC_INCORRECT_DIRECTION
				GPIORC_PIN_USED_IN_PORT							   *
*                                                                      *
* Notes:                                                               *
* 																	   *
*																	   *
* 																	   *
* If the port is with interrupt, it can not be changed to output with *
* this API																*
*************************************************************************/
GPIOReturnCode GpioSetDirection(UINT32 portHandle, GPIOPinDirection dir);

/***********************************************************************
* Function: GpioGetDirection                                           *
************************************************************************
* Description: Get the direction of a given port                  	   *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*                                                                      *
* Return value: GPIO_IN_PIN
				GPIO_OUT_PIN										   *
*				GPIORC_INVALID_PORT_HANDLE						       *
*
*                                                                      *
* Notes:                                                               *
* 																	   *
**************************************************************************/
GPIOPinDirection GpioGetDirection(UINT32 portHandle);

/***********************************************************************
* Function: GpioSetPullControl                                           *
************************************************************************
* Description: Controls the pull up and pull down state of port        *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*				pull - pull up/pull down/not pull					   *
*                                                                      *
* Return value: GPIORC_OK		                                       *
*				GPIORC_INVALID_PORT_HANDLE						       *
*               GPIORC_INCORRECT_PULL
				                                                       *
* Notes:                                                               *
* 																	   *
*																	   *
* 																	   *
* If the port is with interrupt, it can not be changed to output with *
* this API																*
*************************************************************************/
GPIOReturnCode GpioSetPullControl(UINT32 portHandle, GPIOPullUpDown pull);

/***********************************************************************
* Function: GpioBindWakeupCallback                          		   *
************************************************************************
* Description: Bind a callback function to a GPIO's wakeup event       *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*                                                                      *
* Return value: GPIORC_OK
				GPIO_NULL_POINTER 									   *
*				GPIORC_INVALID_PORT_HANDLE			                   *
* Notes:                                                               *
* 																	   *
**************************************************************************/
GPIOReturnCode GpioInitConfiguration(UINT32 portHandle, GPIOConfiguration config);

/***********************************************************************
* Function: GpioBindInterruptCallback                          		   *
************************************************************************
* Description: Bind a callback function to a GPIO's interrupt	       *
*                                                                      *
* Parameters:	portHandle - the port handle                           *
*                                                                      *
* Return value: GPIORC_OK
				GPIO_NULL_POINTER 									   *
*				GPIORC_INVALID_PORT_HANDLE			                   *
* Notes:                                                               *
* 																	   *
**************************************************************************/
GPIOReturnCode GpioBindInterruptCallback(UINT32 portHandle, GPIOCallback func);

GPIOReturnCode GpioPhase1Init(void);
GPIOReturnCode GpioPhase2Init(void);

GPIOReturnCode GpioEnableEdgeDetectionBit(UINT32 portHandle, 	GPIOTransitionType edge);
GPIOReturnCode GpioDisableEdgeDetectionBit(UINT32 portHandle, 	GPIOTransitionType edge);
GPIOReturnCode GpioIsEdgeDetected_wakeup(UINT32 portHandle);
GPIOReturnCode GpioClearEdgeDetection_wakeup(UINT32 portHandle);

UINT32 GPIO_MFPR_ADDR(UINT32 n);

#endif

