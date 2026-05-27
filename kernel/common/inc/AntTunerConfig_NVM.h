/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _ANT_TUNER_CONFIG_NVM_H_
#define _ANT_TUNER_CONFIG_NVM_H_

#include "AntTunerConfig.h"

#define ANT_TUNER_CONFIG_NVM_FILE_NAME	"AntTunerConfig.nvm"
#define ANT_TUNER_CFG_NVM_VER	"00.01" 

//ICAT EXPORTED STRUCT
typedef struct
{
	AntTunerModeSelEnum antTunerMode;
	AntTunerGpoParamStruct antTunerGpoParam;
	//AntTunerRffeParamStruct antTunerRffeParam;     // memory saving	
} AntTunerConfigNvm_ts;


// Functions Prototypes
BOOL AntTunerConfig_CreateNVMFile(void);
BOOL AntTunerConfig_ReadNVMFile(AntTunerConfigNvm_ts	*pAntTunerCfgNvm);


#endif // _ANT_TUNER_CONFIG_NVM_H_
