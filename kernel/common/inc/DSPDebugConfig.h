/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/**********************************************************************
 *
 * Filename: DSPDebugConfig.h
 *
 * File description:
 *			DSP Debug Configuration header file
 *
 **********************************************************************/

#ifndef _DSP_DEBUG_CONFIG_H_
#define _DSP_DEBUG_CONFIG_H_


#define PLP_AUDIO_DBG_CMD 				(0x46)
#define PLP_AUDIO_DBG_CMD_LENGTH		(1)

#define PLP_GSM_FT_TOOL_ENABLE_CMD			(0xF9)
#define PLP_GSM_FT_TOOL_ENABLE_CMD_LENGTH	(5)

#define PLP_OUTPUT_FOR_IML_CMD_LENGTH	(2)


// Functions Prototypes
#ifdef MACRO_FOR_LWG
void DSPDebugConfig( BOOL inPldReset );
#endif
#ifdef MACRO_FOR_LTG
void DSPDebugConfig( void );
#endif

#endif // _DSP_DEBUG_CONFIG_H_

