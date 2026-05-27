
#ifndef	DiamondVoiceTx_H
#define DiamondVoiceTx_H

/****************************************************************************************
 *  HEADER FILE
 ****************************************************************************************/
#include "samsungWiseVoice.h"


//ICAT EXPORTED ENUM
typedef enum
{	
	SAMSUNGWiseVoice_OFF  = 0, 
    SAMSUNGWiseVoice_ON   = 1,	

    SAMSUNGWISEVOICESWITCHER_ENUM_32_BIT	  = 0x7FFFFFFF //32bit enum compiling enforcement    
}SamsungWiseVoiceSwitcher;	


#define	SAMSUNGDIAMONDVOICE_ENABLE			1
#define	SAMSUNGDIAMONDVOICE_DISABLE		0


#define	SAMSUNGDIAMONDVOICEWIDEBAND_TX		320
#define	SAMSUNGDIAMONDVOICENARROWBAND_TX	160

/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
 //ICAT EXPORTED STRUCT
 typedef struct{	
 	SamsungWiseVoiceSwitcher txMode;
	SamsungWiseVoiceModes wisevoiceParam;
	SamsungWiseVoiceAdaptTime att_time;
	SamsungWiseVoiceAdaptTime rel_time;
	SamsungWiseVoiceCompressor_Threashold wisevoice_thd[4];
	SamsungWiseVoiceCompressor_Slope wisevoice_slp_high;
	SamsungWiseVoiceExpansion_Slope wisevoice_slp_low;
	SamsungWiseVoiceStaticGain wisevoice_gain;
	SamsungWiseVoiceWVVolume wisevoice_In_gain;
	SamsungWiseVoiceWVVolume wisevoice_Out_gain;
 } SamsungDiamondVoiceParamTx;


typedef struct{    
   short txMode;
   short wisevoiceParam;
   short att_time;
   short rel_time;
   short wisevoice_thd[4];
   short wisevoice_slp_high;
   short wisevoice_slp_low;
   short wisevoice_gain;
   short wisevoice_In_gain;
   short wisevoice_Out_gain;
} DiamondVoiceParamTx;



/******************************************************************************
 *  Monitor TYPES
 *****************************************************************************/
extern WiseVoiceState_type WiseVoiceMonitor_v;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

extern int SamsungDiamondVoiceEngineInitTx(short  bandwidth);

extern int SamsungDiamondVoiceEngineModeConfigTx(void *SamsungDiamondParam_varsTx);

extern int SamsungDiamondVoiceEngineWiseVoiceConfig(void *SamsungDiamondParam_varsTx);

extern int SamsungDiamondVoiceEngineExeTx(void *SamsungDiamondParam_varsTx, short *output, short *input);

#endif		// DiamondVoiceTx_H

