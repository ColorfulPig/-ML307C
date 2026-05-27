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
/* Title: I2C EEPROM package                                            */
/*                                                                      */
/* Filename: I2C_EEPOM.h                                                */
/*                                                                      */
/* Author: Rony Hanina                                                  */
/*                                                                      */
/* Target, subsystem: Common Platform, HAL                              */
/************************************************************************/
#ifndef _I2C_EEPROM__H_
#define _I2C_EEPROM__H_
/***************************************************************************/
#include "I2C.h"
#include "board_info.h"
/***************************************************************************/


typedef enum
{
    EEPROM_RC_NOT_OK 			= 1,
    EEPROM_RC_OK 				= 0,
    EEPROM_TO_MANY_DATA 		= -100,
    EEPROM_NO_VALID_DATA 		= -101,
    EEPROM_ILLEGAL_CARD_NUM		= -102,
    EEPROM_CARD_NOT_EXIST		= -103
} EEPROM_ReturnCode;




/***************************************************************************/
/***************************************************************************/

typedef void (*EEPROMReadCallback)(UINT8 *dataBuffPtr, UINT16 dataSize,UINT16 userId);
typedef void (*EEPROMWriteCallback)(UINT16);

/***************************************************************************/
/***************************************************************************/

void EPROMi2cCheckCardEcn ( void );
EPROM_DATA *EPROMi2cBoardInfoGet ( void );
EEPROM_ReturnCode EPROMi2cRfCardTypeGet ( EEPROM_RFCard    *rf_card_type );
EEPROM_ReturnCode EPROMi2cBoardTypeGet ( EpromPlatformType    *board_type );
EEPROM_ReturnCode EEPROM_ReadDataFromMemory ( UINT16   start_memory_address , UINT8    *mem_buf , UINT16    size , EEPROMReadCallback   callBackFun );
EEPROM_ReturnCode EEPROM_ReadCurrentDataFromMemory ( UINT8    *mem_buf , EEPROMReadCallback   callBackFun );
EEPROM_ReturnCode EEPROM_ReadDataFromMemoryBlocking ( UINT16   start_memory_address , UINT8    *mem_buf , UINT16    size );
EEPROM_ReturnCode EEPROM_ReadCurrentDataFromMemoryBlocking ( UINT8    *mem_buf );
EEPROM_ReturnCode EEPROM_WriteDataToMemory ( UINT16   				start_memory_address,
											 UINT8     			   *mem_data,
											 UINT16     			size,
											 EEPROMWriteCallback   	callBackFun );
EEPROM_ReturnCode EEPROM_WriteDataToMemoryBlocking ( UINT16   		   	start_memory_address,
											 		 UINT8     		   *mem_data,
													 UINT16     		size );

/***************************************************************************/
/***************************************************************************/
#endif /* _I2C_EEPROM__H_ */
