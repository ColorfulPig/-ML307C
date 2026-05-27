
#ifndef	DiamondVoice_H
#define DiamondVoice_H

/****************************************************************************************
 *  HEADER FILE
 ****************************************************************************************/
#include "samsungSwingFree.h"
#include "samsungVB.h"
#include "samsungVB40.h"
#include "samsungASF.h"
#include "samsungDyVE.h"
#include "samsungCNG.h"
#include "samsungDHA.h"

	/*   DiamondVoice Mode Config   */
#define	SAMSUNGSF_ON							(1<<0)
#define	SAMSUNGVB_ON							(1<<1)
#define	SAMSUNGASF_ON						(1<<2)
#define	SAMSUNGDYVE_ON						(1<<3)
#define	SAMSUNGDYVE_TEST						(1<<4)
#define	SAMSUNGCNG_ON						(1<<5)
#define	SAMSUNGVB40_ON						(1<<8)

#define	SAMSUNGSF_OFF							0x0
#define	SAMSUNGVB_OFF							0x0
#define	SAMSUNGASF_OFF						0x0
#define	SAMSUNGDYVE_OFF						0x0
#define	SAMSUNGCNG_OFF						0x0
#define	SAMSUNGVB40_OFF						0x0

#define	SAMSUNGDIAMONDVOICEWIDEBAND		320
#define	SAMSUNGDIAMONDVOICENARROWBAND	160

#define	SAMSUNGSFEQMODE_OFF					0
#define	SAMSUNGSFEQMODE_LOW					1
#define	SAMSUNGSFEQMODE_HIGH				2

#define	SAMSUNGDHAMODE_ON					1
#define	SAMSUNGDHAMODE_OFF					0

#define	SAMSUNGDIAMONDVOICE_ENABLE			1
#define	SAMSUNGDIAMONDVOICE_DISABLE		0
/******************************************************************************
 *  TYPEDEFS AND STRUCTURES
 *****************************************************************************/
typedef struct{
	short sfComThd;
	short sfComSlope;
	short sfExpThd;
	short sfExpSlope;
	short *sfRxFilter;
} SFParam_type;
typedef struct{
	short sfWNLevel;
	SFParam_type sfStepHigh;
	SFParam_type sfStepLow;
	short sfRxVolStepSelect;
	short sfRxVolume;
} SamsungDiamondVoiceSFParam;

typedef struct{
	short vbHpfCutoff;
	short vbBandGain[3];
	short vbBoosterGain;
	short vbRMSLimiter;
} SamsungDiamondVoiceVBParam;

 typedef struct{
	short vb40HpfCutoff;
	short vb40BoosterGain;
	short vb40PeakLimiter;
	short vb40Excur_f0;
	short vb40Excur_limit;
	short vb40RMSLimiter;
	short vb40BandGain[3];
 } SamsungDiamondVoiceVB40Param;

 typedef struct{
	short asfFilterMode;
	short asfLimiter;
} SamsungDiamondVoiceASFParam;

typedef struct{
	DyVEStepGain_type dyveStepGain;
	short dyveMicSensitivity;
	short dyvePreemphasis;
	short dyveMicGain;
	short dyveRxStep;
} SamsungDiamondVoiceDyVEParam;

typedef struct{
	short cngLevel;
} SamsungDiamondVoiceCNGParam;

typedef struct{
	short dhaMode;
	int dhaChFlag;
	short dhaHearingLevel[12];
} SamsungDiamondVoiceDHAParam;
typedef struct{
	short EQMode;
	short *EQFilter;
} SamsungDiamondVoiceEQParam;
 
/******************************************************************************
 *  Monitor TYPES
 *****************************************************************************/
extern SFState_type SFMonitor_v;
extern VBState_type VBMonitor_v;
extern VB40State_type VB40Monitor_v;
extern ASFState_type ASFMonitor_v;
extern DyVEState_type DyVEMonitor_v;
extern CNGState_type CNGMonitor_v;
extern DHAState_type DHAMonitor_v;

/******************************************************************************
 *  FUNCTION PROTOTYPES
 *****************************************************************************/

extern int SamsungDiamondVoiceEngineInit(short bandwidth);

extern int SamsungDiamondVoiceEngineMode(short mode);

extern int SamsungDiamondVoiceEngineInOutConfig(short *Speech, short *Noise, short *OutSignal);

extern int SamsungDiamondVoiceEngineConfig(SamsungDiamondVoiceSFParam *SamsungDiamondSFParam_vars, SamsungDiamondVoiceVBParam *SamsungDiamondVBParam_vars, SamsungDiamondVoiceVB40Param *SamsungDiamondVB40Param_vars, SamsungDiamondVoiceASFParam *SamsungDiamondASFParam_vars, SamsungDiamondVoiceDyVEParam *SamsungDiamondDyVEParam_vars, SamsungDiamondVoiceCNGParam *SamsungDiamondCNGParam_vars);

extern int SamsungDiamondVoiceEngineVolumeConfig(SamsungDiamondVoiceSFParam *SamsungDiamondSFParam_vars, SamsungDiamondVoiceDyVEParam *SamsungDiamondDyVEParam_vars);

extern int SamsungDiamondVoiceEngineEQConfig(SamsungDiamondVoiceEQParam *SamsungDiamondEQParam_vars);
	
extern int SamsungDiamondVoiceEngineDHAConfig(SamsungDiamondVoiceDHAParam *SamsungDiamondDHAParam_vars);

extern int SamsungDiamondVoiceEngineExe(void);

#endif		// DiamondVoice_H

