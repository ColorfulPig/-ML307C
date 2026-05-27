/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/


/* ===========================================================================
File        : DiamondVoiceTask.h
Description : 

Notes       


=========================================================================== */

#if !defined(_DIAMONDVOICE_TASK_H_)
#define _DIAMONDVOICE_TASK_H_
#include "oss.h"
#include "diags.h"
#include "csw_mem.h"
#include "IPCComm.h"
#include "samsungDiamondVoice.h"
#include "samsungDiamondVoiceTx.h"

/***********************************************************************************
    DiamondVoice Volume Step count
************************************************************************************/
#define DIAMONDVOICE_RX_VOLUME_STEP_CNT 6   //MRVL volume range is 6 step: 0, 20, 40, 60, 80,100. 



/***********************************************************************************
    NVM structures
************************************************************************************/

//ICAT EXPORTED ENUM
typedef enum
{	
	DIAMONDVOICE_OFF  = 0, 
    DIAMONDVOICE_ON   = 1,	

    DIAMONDVOICE_ENABLEDISABLE_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}DiamondVoice_EnableDisable;	


//ICAT EXPORTED STRUCT
typedef struct
{
	DiamondVoice_EnableDisable  SF;
	DiamondVoice_EnableDisable  VB;
	DiamondVoice_EnableDisable  ASF;
	DiamondVoice_EnableDisable  DYVE;
	DiamondVoice_EnableDisable  DYVE_TEST;
	DiamondVoice_EnableDisable  CNG;
	DiamondVoice_EnableDisable  VB40;
}DiamondVoice_Control;


//ICAT EXPORTED STRUCT
typedef struct
{   
	short	sfRxFilter[32];	
	short	sfComThd;	
	short 	sfComSlope; 	
	short   sfExpThd;	
	short   sfExpSlope;
}DiamondVoice_SFParam_type;

//ICAT EXPORTED STRUCT
typedef struct
{    
	short       SF_VOLUME_STEP[DIAMONDVOICE_RX_VOLUME_STEP_CNT];
	
	DiamondVoice_SFParam_type sfStepHigh;

	DiamondVoice_SFParam_type sfStepLow;

	short	    sfWNLevel;
	INT16       sfRxVolStepSelect;
}DiamondVoiceSFParam;


//ICAT EXPORTED STRUCT
typedef struct
{    
	short    	  vbRMSLimiter;
	
	short         vbBoosterGain;
	
	short         vbHpfCutoff;
	short     	  vbBandGain[3];
}DiamondVoiceVBParam;


//ICAT EXPORTED STRUCT
typedef struct
{    
	short vb40HpfCutoff;
	short vb40BoosterGain;
	short vb40PeakLimiter;
	short vb40Excur_f0;
	short vb40Excur_limit;
	short vb40RMSLimiter;
	short vb40BandGain[3];
}DiamondVoiceVB40Param;


//ICAT EXPORTED STRUCT
typedef struct
{    
	short      asfFilterMode;
	short      asfLimiter;
}DiamondVoiceASFParam;


//ICAT EXPORTED STRUCT
typedef struct
{    
	DyVEStepGain_type					dyveStepGain;
	short	dyveMicSensitivity;
	short	dyvePreemphasis;
	short	dyveMicGain;
	//DiamondVoice_dyveMaxVolBoosLev      DYVE_MAX_VOL_BOOST_LEV;
	short   dyveRxStep;
}DiamondVoiceDyVEParam;


//ICAT EXPORTED STRUCT
typedef struct
{    
	short		cngLevel;
}DiamondVoiceCNGParam;




//ICAT EXPORTED STRUCT
typedef struct
{
	DiamondVoice_Control        Control;    //On/Off setting
	DiamondVoiceSFParam             SF;         //sf
	DiamondVoiceVBParam             VB;         //VB
	DiamondVoiceASFParam            ASF;        //ASF
	DiamondVoiceDyVEParam           DYVE;       //DYVE
	DiamondVoiceCNGParam            CNG;        //CNG
    DiamondVoiceVB40Param           VB4; //VB4
}_DiamondVoiceParam;



extern  DiamondVoiceParamTx DiamondVoice_WiseVoiceConfig;
/***********************************************************************************
    APIs for ACM_COMM
************************************************************************************/
void    DiamondVoice_Init(void);
void    DiamondVoice_ParamSet(void);
void    WiseVoice_ParamSet(void);
void    DiamondVoice_ReloadNVM(void);
void    DiamondVoice_CreateDefaultNVM(void);
void    DiamondVoice_CreateDefaultNVM_IfNoExist(void);
void    DiamondVoice_ModeChangeConfig(UINT32 PCM_Size);
UINT32  DiamondVoice_IsEnabled(void);

void WiseVoice_ModeChangeConfig(UINT32 PCM_Size);


#endif /* _DIAMONDVOICE_TASK_H_ */

