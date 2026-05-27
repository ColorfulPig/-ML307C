/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

/******************************************************************************************************************************
*              MODULE IMPLEMENTATION FILE
******************************************************************************************************************************
*  COPYRIGHT (C) 2001 Intel Corporation.
*
*  This file and the software in it is furnished under
*  license and may only be used or copied in accordance with the terms of the
*  license. The information in this file is furnished for informational use
*  only, is subject to change without notice, and should not be construed as
*  a commitment by Intel Corporation. Intel Corporation assumes no
*  responsibility or liability for any errors or inaccuracies that may appear
*  in this document or any software that may be provided in association with
*  this document.
*  Except as permitted by such license, no part of this document may be
*  reproduced, stored in a retrieval system, or transmitted in any form or by
*  any means without the express written consent of Intel Corporation.
*
*  Title:   UART Package UART API
*
*  Filename:    UART.h
*  Target, subsystem: Common Platform, HAL
*
*  Authors:	ILANA BEN-ARIE
*
*  Description: This file includes all the API that the UART Package support
*
*  Last Modified: <Initial> <date>
*
*  Notes:
*********************************************************************************************************************************/



#ifndef _UART_API_H_
#define _UART_API_H_



#include "global_types.h"
#include "pmu.h"

#define UART_INIT_DONE	            0xDEAFBEED
#define UART_INIT_DISABLE	        0xBEDBEDAA

//#define UART_DISABLE	0
#define UART_ENABLE		1

#ifdef LWIP_IPNETBUF_SUPPORT
#define UART_LOG_MAX_LENTH	200
#else
#define UART_LOG_MAX_LENTH	80
#endif

#define NUMBER_OF_SUPPORT_PORTS  4      /* The number of port that are support now from the 4 port which define  */

#define UART_TX_DELAY_TIME          200

#define UART_RX_SUPPORTTED
#define UART_LONG_LOG_MAX_LENTH	    768
#define MUX_HAL_RX_BUF_SIZE         (8192)
#define MUX_HAL_RX_DUAL_BUF_SIZE        (2048)
#define MUX_HAL_RX_DUAL_BUF_FLAG_CNT    (4)

 /* Activity reasons - use in UARTOn/UARTOff */
#define UART_ACTIVITY_RX            UART_RX_TIMER
#define UART_ACTIVITY_TX            UART_TX_TIMER
#define UART_ACTIVITY_EXT           UART_EXT_TIMER
#define UART_ACTIVITY_FORCE         0x80           /* No timer for this */
#define UART_ACTIVITY_TXON          0x40           /* No timer for this */
#define UART_ACTIVITY_USER1         0x20           /* No timer for this */
#define UART_ACTIVITIES             (UART_ACTIVITY_TX|UART_ACTIVITY_RX|UART_ACTIVITY_EXT) /*those timer may turn off*/


#define UART2_RX_DMA_DEVICE DMAC_UART2_RX
#define UART2_TX_DMA_DEVICE DMAC_UART2_TX

/* UART configure Type. */
typedef enum
{
    UART_DISABLE        = 0x00,
    UART_INT_DISABLE    = 0x01,
    UART_INT_ENABLE     = 0x02,
	UART_MAX_TYPE       = 0xFF
}UART_Config_Type;

/* UART BaudRate SelfAdapt state. */
typedef enum
{
    UART_BRSA_115200    = 0x00,
    UART_BRSA_57600     = 0x01,
    UART_BRSA_38400     = 0x02,
    UART_BRSA_19200     = 0x03,
    UART_BRSA_14400     = 0x04,
    UART_BRSA_9600      = 0x05,
    UART_BRSA_MAX       = 0xFF
}UART_BRSA_state;

/* UART BaudRate SelfAdapt parameter. */
typedef struct uart_brsa_parameter
{
	UART_BRSA_state state;
	UINT8 data;
} uart_brsa_parameter;

/* UART mode information. */
typedef struct uart_mode_info
{
	char string[7];
	UART_Config_Type Type[NUMBER_OF_SUPPORT_PORTS];
} uart_mode_info;


/***********************   define  MACROS for the basic intrface  ****************************************************/

 /* This MACRO read the Interrupt Identification Register.  */
#define UARTInterruptIDRegisterRead(pn, s) \
                                 HW_UART_READ_IIR(_UARTHwAddresses[(pn)], s);

 /* This MACRO read the Line Status Register.  */
#define UARTLineStatusRead(pn, s) \
                                 HW_UART_READ_LSR(_UARTHwAddresses[(pn)], s);

 /* This MACRO enables the Tx interrupt in the UART Driver. */
#define UARTTxInterruptEnable(pn) \
                                ENABLE_UART_INT((pn),IER_TRN_HOLDING_EMPTY);

 /* This MACRO disables the Tx interrupt in the UART Driver  */
#define  UARTTxInterruptDisable(pn) \
                                DISABLE_UART_INT((pn),IER_TRN_HOLDING_EMPTY);

 /* This MACRO enables the Rx interrupt in the UART Driver.  */
#define  UARTRxInterruptEnable(pn) \
                                ENABLE_UART_INT((pn),IER_RCV_DATA_AVAILABLE);

 /*  This MACRO disables the Rx interrupt in the UART Driver.   */
#define  UARTRxInterruptDisable(pn) \
                                DISABLE_UART_INT((pn),IER_RCV_DATA_AVAILABLE );

 /*   This MACRO enables the Rx time out interrupt in the UART Driver.   */
#define  UARTRxTimeOutInterruptEnable(pn) \
                                ENABLE_UART_INT((pn), IER_TIME_OUT );

/* This MACRO disables the Rx time out interrupt in the UART Driver.  */
#define  UARTRxTimeOutInterruptDisable(pn) \
                                DISABLE_UART_INT((pn), IER_TIME_OUT );

 /*   This MACRO enables the Rx time out and Rx interrupts in the UART Driver.   */
#define  UARTRxTimeOutAndRxInterruptsEnable(pn) \
                                ENABLE_UART_INT((pn), (IER_TIME_OUT | IER_RCV_DATA_AVAILABLE));

/* This MACRO disables the Rx time out and Rx interrupts in the UART Driver.  */
#define  UARTRxTimeOutAndRxInterruptsDisable(pn) \
                                DISABLE_UART_INT((pn), (IER_TIME_OUT | IER_RCV_DATA_AVAILABLE));


/*   This MACRO enables the line status interrupt in the UART Driver.  */
#define  UARTErrInterruptEnable(pn) \
                                ENABLE_UART_INT((pn), IER_RCV_LINE_STATUS);

 /*   This MACRO disables the line status interrupt in the UART Driver. */
#define  UARTErrInterruptDisable(pn) \
                                DISABLE_UART_INT((pn), IER_RCV_LINE_STATUS);

 /*   This MACRO enables the Enable Modem Status interrupt in the UART Driver.   */
#define  UARTModemStatusInterruptEnable(pn) \
                                ENABLE_UART_INT((pn), IER_MODEM_STATUS );

 /*   This MACRO enables the Disable Modem Status interrupt in the UART Driver.   */
#define  UARTModemStatusInterruptDisable(pn) \
                                DISABLE_UART_INT((pn), IER_MODEM_STATUS );


/* This MACRO Write character to the FIFO  */
 #define UARTFIFOWriteCharacter(pn, b)  \
                                HW_UART_WRITE_THR((_UARTHwAddresses[(pn)]),(b));


#define UART_DEBUG_ENABLE          UARTTraceEnable

/* UART trace enable flag. */
#ifdef LWIP_IPNETBUF_SUPPORT
#define UART_TRACE(fmt,args...)                                             \
{                                                                           \
    if (UART_DEBUG_ENABLE)                                                  \
    {                                                                       \
        RTI_LOG(fmt, ##args);                                             \
    }                                                                       \
}
#else
#define UART_TRACE(fmt,args...)
#endif
/*-- Type definitions (T type) -----------------------------------------*/



typedef enum    /* The UART support several layer interface */
{
    UART_BASIC_INTERFACE,         /* Use for basic services only */
    NUMBER_OF_IF                  /* number of interface */
}UART_InterfaceType;

typedef enum   /* The number of ports in the UART  */
{
    UART_PORT_FFUART,
    UART_PORT_BTUART,
    UART_PORT_STUART, //UART0
    UART_PORT_4,
    NUMBER_OF_PORTS
}UART_Port;

typedef enum
{
 UART_ALWAYS = 0,       /* No clock on/off's while UART is open */
 ENABLE_SLEEP = 1,      /* Legacy sleep mode; taken if the sleepMode field is set to just TRUE */
 UART_ENABLE_SLEEP=ENABLE_SLEEP, /* correct style */
 /*------------------------------------------------*/
 UART_GPIO_WAKEUP = 2,
 /* Enable specific activities to be negated on timeout */
 UART_RX_TIMER = 4,
 UART_TX_TIMER = 8,
 UART_EXT_TIMER = 0x10,
 UART_TIMER_ON = (UART_RX_TIMER|UART_TX_TIMER|UART_EXT_TIMER) /* All possible activities can be negated on timeout */
}UARTSleepMode;

typedef UINT8 UART_Activity;
/* Activity reasons - use in UARTOn/UARTOff */
#define UART_ACTIVITY_RX UART_RX_TIMER
#define UART_ACTIVITY_TX UART_TX_TIMER
#define UART_ACTIVITY_EXT UART_EXT_TIMER
#define UART_ACTIVITY_FORCE 0x80           /* No timer for this */
#define UART_ACTIVITY_TXON  0x40           /* No timer for this */
#define UART_ACTIVITY_USER1 0x20           /* No timer for this */
#define UART_ACTIVITIES (UART_ACTIVITY_TX|UART_ACTIVITY_RX|UART_ACTIVITY_EXT) /*those timer may turn off*/

typedef enum   /* All the UART Baud Rate that the UART Package supplay */
{
#ifdef  _QT_
	/* This baud rate is not supported on any "real" platform.
	   It can only be used on QT.
	*/
	UART_BAUD_110       = 110,
#endif
    UART_BAUD_150       = 150,
    UART_BAUD_300       = 300,
    UART_BAUD_600       = 600,
    UART_BAUD_1200      = 1200,
    UART_BAUD_2400      = 2400,
    UART_BAUD_3600      = 3600,
    UART_BAUD_4800      = 4800,
    UART_BAUD_7200      = 7200,
    UART_BAUD_9600      = 9600,
    UART_BAUD_14400     = 14400,
    UART_BAUD_19200     = 19200,
    UART_BAUD_28800     = 28800,
    UART_BAUD_38400     = 38400,
    UART_BAUD_57600     = 57600,
    UART_BAUD_115200    = 115200,
    UART_BAUD_230400    = 230400,
    UART_BAUD_460800    = 460800,
    UART_BAUD_921600    = 921600,
    UART_BAUD_1000000   = 1000000,
    UART_BAUD_1125000   = 1125000,
    UART_BAUD_1800000   = 1800000,
    UART_BAUD_2100000   = 2100000,
    UART_BAUD_3600000   = 3600000,
    UART_MAX_NUM_BAUD
}UART_BaudRates;

typedef enum  /* The Word Len of the UART Frame Format  */
{
    UART_WORD_LEN_5,                /* set Word Lengto to 5 Bits         */
    UART_WORD_LEN_6,                /* set Word Lengto to 6 Bits         */
    UART_WORD_LEN_7,                /* set Word Lengto to 7 Bits         */
    UART_WORD_LEN_8                 /* set Word Lengto to 8 Bits         */
}UART_WordLen;

typedef enum  /* The Stop Bits of the UART Frame Format */
{
    UART_ONE_STOP_BIT,
    UART_ONE_HALF_OR_TWO_STOP_BITS
}UART_StopBits;

typedef enum  /* The Parity Bits of the UART Frame Format */

{
    UART_NO_PARITY_BITS,
    UART_EVEN_PARITY_SELECT,
    UART_ODD_PARITY_SELECT
}UART_ParityTBits;

typedef enum     /* The Operation Mode that the UART could work */
{
    UART_BYTE_CTL,              /* non fifo mode */
    UART_FIFO_CTL,              /*  fifo mode */
    UART_DMA_CTL                /* DMA mode */
}UART_OpMode;

typedef enum    /* The trigger level interrupt of the FIFO */
{
    UART_INT_TRIGGER_L1,              /* trigger level of 1 byte */
    UART_INT_TRIGGER_L8,              /* trigger level of 8 byte */
    UART_INT_TRIGGER_L16,             /* trigger level of 16 byte */
    UART_INT_TRIGGER_L32              /* trigger level of 32 byte */
}UART_TriggerLevel;
typedef struct   /* This is structure of the IrDA-SIR Configuration  */
{
    BOOL                     rxNegativeDataPolarity;	/* negative - TRUE, positive - FALSE  */
    BOOL                     txNegativeDataPolarity;	/* negative - TRUE, positive - FALSE  */
    BOOL                     lowPowerPulseMode;			/* 1.6 microsec - TRUE, 78 microsec (normal) - FALSE,   */
    BOOL                     rxSirEnable;				/* enable - TRUE, disable - FALSE  */
    BOOL                     txSirEnable;				/* enable - TRUE, disable - FALSE  */
}UART_SIRConfigure;

typedef enum    /* UART IrDA half-duplex mode: RX, TX, or NONE */
{
	UART_IRDA_RX,   /* Enable Irda RX operation (TX blocked) */
	UART_IRDA_TX,   /* Enable Irda TX operation (RX blocked) */
	UART_IRDA_NONE, /* Used to inform UART that it IS NOT in IRDA mode */
	UART_IRDA_ON    /* Used to inform UART that it IS in IRDA mode     */
}UARTIrdaMode;

#define UART_API_WITH_SLEEP
//ICAT EXPORTED STRUCT
typedef struct   /* This is structure of the UART Configuration  */
{
    UART_OpMode              opMode;             /* fifo mode, non fifo mode or DMA for basic interface*/
    UART_TriggerLevel        triggerLevel;       /* the trigger level interrupt on 1,8,16,32 */
    UART_BaudRates	     baudRate;			/* the rate of the transmit and the receive up to 111520 (default - 9600).*/
    UART_WordLen	     numDataBits;		/* 5, 6, 7, or 8 number of data bits in the UART data frame (default - 8). */
    UART_StopBits	     stopBits;			/* 1, 1.5 or 2 stop bits in the UART data frame (default - 1).   */
    UART_ParityTBits	     parityBitType;		/* Even, Odd or no-parity bit type in the UART data frame (default - Non). */
    UART_InterfaceType	     interfaceType;		/* number of interface that the UART driver supplies (default - UART_IF_TYPE_L2) */
    BOOL                     modemSignal;       /* enable operate modem - TRUE, disable modem - FALSE  */
    BOOL                    flowControl;       /* enable Auto flow Control - TRUE, disable Auto flow Control - FALSE  */
    UINT8                   sleepMode;         /* enable sleep mode - TRUE, more fine control - see UARTSleepMode enum */
    BOOL                    auto_baud;         /* enable auto_baud ,auto-baud-rate detection within the UART (default - FALSE) */
    UART_SIRConfigure       sirIrDA;
}UARTConfiguration;



typedef enum  /* The 4 Type of Notification on Interrupt  */
{
 UART_NotifyModemInterrupt,
 UART_NotifyTxInterrupt,
 UART_NotifyRxInterrupt,
 UART_NotifyErrInterrupt
}UART_ISRType;


 typedef struct  /* Struct which supplay all the status of the Line Ststus Register */
{
    BOOL    dataReady;
    BOOL    overrunError;
    BOOL    parityError;
    BOOL    framingError;
    BOOL    breakInterrupt;
    BOOL    transmitDataRequest;
    BOOL    transmitterEmpty;
    BOOL    fifoErrorStatus;
}UART_LineStatus;

 //ICAT EXPORTED ENUM
typedef enum	// change the order -1 to +
{
    UART_RC_OK                              =   1,      /* 1 - no errors                                            */

    UART_RC_PORT_NUM_ERROR                  =   -100,   /* -100 - Error in the UART port number                     */
    UART_RC_NO_DATA_TO_READ,                            /* -99 -  Eror no data to read from the FIFO UART           */
    UART_RC_ILLEGAL_BAUD_RATE,                          /* -98 -  Error in the UART Bayd Rate                       */
    UART_RC_UART_PARITY_BITS_ERROR,                     /* -97 - Error in parity bit                                */
    UART_RC_UART_ONE_STOP_BIT_ERROR,                    /* -96 - Error in one stop bit                              */
    UART_RC_ONE_HALF_OR_TWO_STOP_BIT_ERROR,             /* -95 - Error in two stop bit                              */
    UART_RC_BAD_INTERFACE_TYPE,                         /* -94 - Error in the Interface Type                        */
    UART_RC_UART_NOT_AVAILABLE,                         /* -93 - Error in try to open UART that is open             */
    UART_RC_NO_DATA_TO_WRITE,                           /* -92 - Error No data to writ the len = 0                  */
    UART_RC_NOT_ALL_BYTE_WRITTEN,                       /* -91 - Error Not all the Byte write to the UART FIFO      */
    UART_RC_ISR_ALREADY_BIND,                           /* -90 - Error try to bind ISR for Basic Interface          */
    UART_RC_WRONG_ISR_UNBIND,                           /* -89 - Error in the UnBind ISR for Basic Interface        */
    UART_RC_FIFO_NOT_EMPTY,                             /* -88 - Error, the UART FIFO not empty                     */
    UART_RC_UART_OPEN,                                  /* -87 - Error try chance the configurr when the UART open  */
    UART_RC_GPIO_ERR,                                   /* -86 - Error in the Configure of the GPIO                 */
    UART_RC_IRDA_CONFIG_ERR,                            /* -85 - Illegal IrDA configuration                         */
    UART_RC_TX_DMA_ERR                                  /* -84 - DMA TX Error                                       */
}UART_ReturnCode;

/* Log module type. */
typedef enum
{
    LOG_MODULE_BT           = 0x00,
    LOG_MODULE_LWIP         = 0x01,
    LOG_MODULE_LFS          = 0x02,
    LOG_MODULE_GNSS         = 0x03,
    LOG_MODULE_MAX          = 0xFF
}Log_Module_Type;

//ICAT EXPORTED ENUM
typedef enum
{
	LOG_DISABLE     = 0x0,
	UART_LOG_ENABLE = 0x1,
	ACAT_LOG_ENABLE = 0x2
} Log_ConfigE;

//ICAT EXPORTED ENUM
typedef enum
{
	MSG_DISABLE     = 0x0,
	ACAT_MSG_ENABLE = 0x1
} Msg_ConfigE;

//ICAT EXPORTED ENUM
typedef enum
{
	RTI_LOG_DISABLE = 0x0,
	RTI_DUMP_ENABLE = 0x1,
	RTI_TASK_ENABLE = 0x2,
	RTI_MIPS_ENABLE = 0x3
} RTI_ConfigE;

#if 0
//ICAT EXPORTED ENUM
typedef enum
{
	MASS_STORAGE_DISABLE = 0x0,
	MASS_STORAGE_ENABLE  = 0x1
} MassStorage_ConfigE;
#endif
//ICAT EXPORTED STRUCT
typedef struct {
	Log_ConfigE log_cfg;
	Msg_ConfigE msg_cfg;
	RTI_ConfigE rti_cfg;
} Log_ConfigS;


/***********   callback function    **************/
typedef void (*UARTNotifyInterrupt)(UART_Port);

typedef void (*UsbLogPrint_t) (const char * /*format*/, ...);

#ifdef LWIP_IPNETBUF_SUPPORT
/*===========================================================================

            EXTERN DECLARATIONS FOR MODULE

===========================================================================*/

/* UART initialized flag */
extern UINT32 uart_initialized;

/* log configuration. */
extern Log_ConfigS log_config;

/* UART trace flag */
extern BOOL UARTTraceEnable;

/*===========================================================================

                        EXTERN FUNCTION DECLARATIONS

===========================================================================*/

/*===========================================================================

                          INTERNAL FUNCTION DECLARATIONS

===========================================================================*/
/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      SysIsAssert                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get system assert flag.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      size                                Memory size                  */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
extern BOOL SysIsAssert(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      get_current_TStimer_tick                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get current TStimer tick.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
extern UINT32 get_current_TStimer_tick(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      get_current_TStimer_tick                                         */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function get current TStimer tick.                          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
extern BOOL getATRdyPhase1(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      RTI_LOG                                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void RTI_LOG(const char* fmt, ...);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      set_at_diag_mode                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function set UART AT or Diag mode.                           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
void set_at_diag_mode(UINT8 mode);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      IsProductionMode                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function get production mode.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
UINT8 IsProductionMode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      InProduction_Mode                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check production mode.                              */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
UINT8 InProduction_Mode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      IsUartDiagMode                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check whether it is UART diag mode or not.          */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
UINT8 IsUartDiagMode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      utilsAssertExt                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function Operates the "utilsAssertExt" as needed.           */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
extern void utilsAssertExt(const char      *cond,
                            const char      *file,
                            signed short    line,
                            const char*     fmt, ...);

#if (!defined(LTEONLY_THIN) && !defined(LTEONLY_8M8M) && !defined(LTEGSM_THIN))
/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      uart_printf                                                      */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function print UART log.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTConfiguration configurations an array of parameters that     */
/*      includes all ports configurations.                               */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_RC_OK  - no errors                                          */
/*      status - according the UART configre                             */
/*                                                                       */
/*************************************************************************/
int uart_printf(const char* fmt, ...);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      fatal_printf                                                     */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function print UART log.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTConfiguration configurations an array of parameters that     */
/*      includes all ports configurations.                               */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_RC_OK  - no errors                                          */
/*      status - according the UART configre                             */
/*                                                                       */
/*************************************************************************/
int fatal_printf(const char* fmt, ...);
#else
#define uart_printf(fmt,args...)
#define fatal_printf(fmt,args...)
#endif

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CPUartLogPrint                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function print UART log.                                    */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTConfiguration configurations an array of parameters that     */
/*      includes all ports configurations.                               */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_RC_OK  - no errors                                          */
/*      status - according the UART configre                             */
/*                                                                       */
/*************************************************************************/
void CPUartLogPrint(char* log, BOOL CRLF);


/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      ErrorLogPrintf_Extend                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void ErrorLogPrintf_Extend(const char *fmt, ...);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTLogPrintf_Extend                                             */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTLogPrintf_Extend(Log_Module_Type cat, const char *fmt, ...);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      ASSERT_EXT                                                       */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      ASSERT extend macro.                                             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define ASSERT_EXT(cOND, fmt, args...)                                  \
{                                                                       \
    if (!(cOND))                                                        \
    {                                                                   \
        utilsAssertExt(#cOND, __MODULE__, __LINE__, fmt, ##args);         \
    }                                                                   \
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      CPUartLogPrintf                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define CPUartLogPrintf(fmt,args...)                                                    \
{                                                                                       \
    if (log_config.log_cfg != LOG_DISABLE)      \
    {                                                                                   \
        UARTLogPrintf_Extend(LOG_MODULE_MAX, fmt, ##args);                              \
    }                                                                                   \
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      ErrorLogPrintf                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define ErrorLogPrintf(fmt,args...)                                                     \
{                                                                                       \
    if (log_config.log_cfg != LOG_DISABLE)                                             \
    {                                                                                   \
        ErrorLogPrintf_Extend("%08x "fmt"\r\n", get_current_TStimer_tick(), ##args);    \
    }                                                                                   \
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      Log_Print                                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define Log_Print(fmt,args...)           CPUartLogPrintf(fmt, ##args)


/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      LPUartLogPrintf                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define LPUartLogPrintf(fmt,args...)

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      MIFI_MSG_TRACE                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output MIFI message trace information.             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define MIFI_MSG_TRACE(cat1, cat2, cat3, fmt, args...)      \
{                                                           \
    if (log_config.msg_cfg == ACAT_MSG_ENABLE)              \
    {                                                       \
        DIAG_FILTER(cat1, cat2, cat3, DIAG_INFORMATION)     \
        diagPrintf(fmt, ##args);                            \
    }                                                       \
}

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      MIFI_LOG_TRACE                                                   */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output MIFI log information.                       */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
#define MIFI_LOG_TRACE(cat1, cat2, cat3, fmt, args...)      \
{                                                           \
    if (log_config.log_cfg != LOG_DISABLE)                  \
    {                                                       \
        DIAG_FILTER(cat1, cat2, cat3, DIAG_INFORMATION)     \
        diagPrintf(fmt, ##args);                            \
    }                                                       \
}
#else
#define CPUartLogPrintf
#define ErrorLogPrintf
#endif

/*===========================================================================

                          INTERNAL FUNCTION DECLARATIONS

===========================================================================*/

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      RTI_LOG                                                          */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      This function output trace information via uart port.            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      fmt                                 Format parameters            */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void RTI_LOG(const char* fmt, ...);


/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTIntEnable                                                    */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function check whether UART interrupt is enable or not.      */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
BOOL UARTIntEnable(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTGetTraceMode                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is used to get the UART trace mode.                 */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTGetTraceMode(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTTxTimerStart                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function start the UART Tx timer.                            */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTTxTimerStart(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTTxTimerStart                                                 */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function handle the UART TX timeout event.                   */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTTxTimeHandler(UINT32 arg);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTTxMutexLock                                                  */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function lock the UART Tx mutex.                             */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTTxMutexLock(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTTxMutexUnlock                                                */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function unlock UART Tx mutex.                               */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      None                                N/A                          */
/*                                                                       */
/*************************************************************************/
void UARTTxMutexUnlock(void);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTSendStringWithNoMutex                                        */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function send string to UART.                                */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
int UARTSendStringWithNoMutex(UINT8 *bufPtr);

/*************************************************************************/
/*                                                                       */
/* FUNCTION                                                              */
/*                                                                       */
/*      UARTBaudRateSelfAdapt                                            */
/*                                                                       */
/* DESCRIPTION                                                           */
/*                                                                       */
/*      The function is do UART baudrate self-adaption.                  */
/*                                                                       */
/* CALLED BY                                                             */
/*                                                                       */
/*      Application                                                      */
/*                                                                       */
/* CALLS                                                                 */
/*                                                                       */
/*      Application                         The application function     */
/*                                                                       */
/* INPUTS                                                                */
/*                                                                       */
/*      UARTPort portNumber                 UART port to be closed       */
/*                                                                       */
/* OUTPUTS                                                               */
/*                                                                       */
/*      UART_ReturnCode                     UART_RC_OK - no errors       */
/*                                                                       */
/*************************************************************************/
BOOL UARTBaudRateSelfAdapt(UINT8 *buf, int length, BOOL *newBaudRate);

/*===========================================================================
                        EXTERN FUNCTION DECLARATIONS

===========================================================================*/

UART_ReturnCode UARTPhase1Init(const UARTConfiguration configuration[]);
UART_ReturnCode UARTPhase2Init(void);
UART_ReturnCode UARTConfigurationGet(UART_Port portNumber, UARTConfiguration *configuration);
UART_ReturnCode UARTConfigure(UART_Port portNumber, const UARTConfiguration *configuration);
UART_ReturnCode UARTOpen(UART_Port portNumber);
UART_ReturnCode UARTClose(UART_Port portNumber);
void UARTLineStatusGet(UART_Port portNumber, UART_LineStatus *lineStatus);
UART_ReturnCode UARTISRBind(UART_Port portNumber, UART_ISRType isrType, UARTNotifyInterrupt callbackNotifyInterrupt);
UART_ReturnCode UARTISRUnbind(UART_Port portNumber, UART_ISRType isrType);
SwVersion UARTVertionGet(void);

void UARTHwInterruptEnable(UART_Port portNumber);
void UARTHwInterruptDisable(UART_Port portNumber);

void UARTWakeupConfigInit(UART_Port portNumber);

UART_ReturnCode UARTFIFOWrite(UART_Port portNumber, UINT8 *data, UINT16 *dataLength);
UART_ReturnCode UARTFIFORead(UART_Port portNumber, UINT8 *data, UINT16 lenToRead, UINT16 *dataLength);
UART_ReturnCode UARTCharacterWrite(UART_Port portNumber, UINT8 *character);
UART_ReturnCode UARTCharacterRead(UART_Port portNumber, UINT8 *character);
UART_ReturnCode UARTPrintString(UART_Port portNumber,char *data);
void UARTOn(UART_Port portNumber, UART_Activity reason);
void UARTOff(UART_Port portNumber, UART_Activity reason);

void UARTSetRI(void);
void UARTClearRI(void);
void UARTSetDCD(void);
void UARTClearDCD(void);
#ifdef LWIP_IPNETBUF_SUPPORT
void seagull_uart_init(void);
void seagull_uart_putc(const char ch);
#endif
void uart_init_restart(void);
int get_uart_baudrate(void);
int send_string_2uart(UINT8 *bufPtr);
void send_data_2uart(UINT8 *bufPtr, UINT32 length);

//
// Additional function APIs for UART interrupt control
// Macro's above are not good, since they require UART_HW.h and UART_CommonControl.h
void UARTInterruptControl(UART_Port p, UINT8 b, BOOL enable);


// Dedicated API for dynamic control of IrDA mode: RX, TX, or None
void UARTIrdaModeControl(UART_Port portNumber, UARTIrdaMode mode);
/*-------------------------------------------------------------------------------------------------------*/
// Debug and development mode options:

// Set "force awake" state as initial (on UARTOpen) per port
// Parameter: bit mask (3 lower bits assigned to 3 supported ports)
// 1: any port opened will have a "FORCE" vote set until it's cleared
// 0: normal mode
void pmUartInitForce(UINT8 force);



/******  This function is used to release the I2C bus while geting    *********/
/******  a reset in reading or writing sequence to the I2C bus.       *********/
void UARTSendI2CClocks ( void );

#ifdef PHS_SW_DEMO_TTC
/* 2008.04.25 PHS debug */
#define PHS_CP_UART_DEBUG

#ifdef PHS_CP_UART_DEBUG
void UartLogPrint(unsigned char * log);
void UartLogPrintf(const char *fmt, ...);
#else
#define UartLogPrint(x)
#define UartLogPrintf(fmt, ...)
#endif

// #define MEMLOG

#ifdef MEMLOG
void MemLogPrinf(const char *fmt, ...);
void MemLogList();
#else
#define MemLogPrinf(fmt, ...)
#define MemLogList()
#endif

/****/
#endif //#ifdef PHS_SW_DEMO_TTC

#define MFPRX_BASE_ADDR (0xD401E000)
#define MFPRX_GPIO_51                   (MFPRX_BASE_ADDR + 0x1A8)
#define MFPRX_GPIO_52                   (MFPRX_BASE_ADDR + 0x1AC)

#define write32(addr, val)                          (*(volatile unsigned int *)(addr) = (val))
#define read32(addr)                                (*(volatile unsigned int *)(addr))

#ifndef UART_NEW_VERSION
typedef enum{	
	RTS_NONE=-1,
	RTS_DEASSERT=0,
	RTS_ASSERT=1,
	RTS_AUTO=2,
}UartRtsControlType;

void UART_Set_Rts_Control(UART_Port portNumber, UartRtsControlType rts_control);
void UART_Set_Auto_FlowControl(UART_Port portNumber);
void UART_Clear_Auto_FlowControl(UART_Port portNumber);

#define UART_DMA_TX_ENABLE
#ifdef UART_DMA_TX_ENABLE
unsigned char GetUartPmLock(void);

typedef struct uartDmaRegister
{
    UINT32 DCSR;
} uartDmaRegisterT;

typedef struct uartDmaTxInfoStat
{
    UINT32 totalTxCnt;  // counter of tx
    UINT32 intrCnt;     // counter of intrrupt
    UINT32 toCnt;       // counter of timeout
    UINT32 lastLoopCnt;     // counter of loop after tx timeout at last
    UINT32 totalLoopCnt;     // counter of total loop after tx timeout
    UINT32 startTick;     // tick of start uart send
    UINT32 startOSTick;   // OS tick of start uart send
    UINT32 toTick;     // tick of uart send data timeout
    UINT32 toOSTick;    // OS tick of uart send data timeout
    struct uartDmaRegister reg;
} uartDmaTxInfoStatT;

typedef struct uartDmaInfoStat
{
	struct uartDmaTxInfoStat tx;
    //struct uartDmaRxInfoStat rx;
} uartDmaInfoStatT;

#endif

#define UART_DMA_RX_ENABLE
#endif //UART_NEW_VERSION
#endif  /*  _UART_API_H_   */
