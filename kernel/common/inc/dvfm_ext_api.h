/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef DVFM_EXT_API_H
#define DVFM_EXT_API_H
// for the definition of UNIT8 (PV2) taken from platform
#include "gbl_types.h"

typedef UINT8 mode_t;

#define NO_MODE 0xFF

//ICAT EXPORTED ENUM
typedef enum{
	WB_OFF_MODE,
    WB_START_UP_MODE,
    WB_INIT_MODE,
    WB_OOS_MODE,
	WB_DRX_SLEEP_MODE,
	WB_DRX_MODE,
	WB_PCCPCH_MODE,
	WB_PCCPCH_DRX_MODE,
	WB_SCCPCH_MODE,
	WB_PCCPCH_SCCPCH_MODE,
	WB_DCH_MODE,
	WB_DCH_HSDPA_MODE,
	WB_DCH_HSUPA_MODE,
	WB_DCH_HSDPA_HSUPA_MODE,
	WB_ECF_HSDPA_MODE,
	WB_PCCPCH_ECF_HSDPA_MODE,
	WB_EDRX_MODE,
	WB_EDRX_DRX_MODE,
	WB_PCCPCH_EDRX_MODE,
	WB_PCCPCH_EDRX_DRX_MODE,
	WB_FFT_SCAN_MODE,
	WB_START_UP_FFT_SCAN_MODE,
	WB_DRX_SLEEP_FFT_SCAN_MODE,
	WB_DRX_FFT_SCAN_MODE,
	WB_ECF_DRX_SLEEP_MODE,
	WB_EDRX_ECF_DRX_SLEEP_MODE,
	WB_ECF_DRX_SLEEP_FFT_SCAN_MODE,	
#if defined (L1_UPGRADE_R8)
	WB_DCH_HSDPA_HSUPA_AICH_MODE,
	WB_DCH_HSDPA_HSUPA_AICH_PCCPCH_MODE,	
#endif		

      //added by xqshi for pccpchReq in cell dch state
    WB_DCH_PCCPCH_MODE,
#ifndef  L1_WB_R99_ONLY

    WB_DCH_HSDPA_PCCPCH_MODE,
    WB_DCH_HSDPA_HSUPA_PCCPCH_MODE,
#endif
    //added end
	NUM_OF_WB_MODES,

	INVALID_WB_MODE,
	MAX_NUM_OF_WB_MODES

}wb_mode_vals_te;

//ICAT EXPORTED ENUM
typedef enum
{
    GSM_STATE_NULL = 0,
    GSM_STATE_MEASURE,
    GSM_STATE_SYNC,
    GSM_STATE_IDLE,
    GSM_STATE_DEDICATED,
    GSM_STATE_GPRS_PTM,
    GSM_STATE_TEST,

/* NOTE: if any new states are sdded in l1frint.c then the corresponding states
         must be added here */

    GSM_NUM_L1_STATES
}
gsm_mode_vals_te;


//ICAT EXPORTED ENUM
typedef enum{
    GSM_CLIENT,
	WB_CLIENT,
	NUM_OF_CLIENTS
}dvfmClientId_vals_te;

typedef UINT8 dvfmClientId_t;

//ICAT EXPORTED ENUM
typedef enum{
    REQUEST_HANDLED,
    REQUEST_PENDING_MS_TRIGGER
}dvfmToMsRequestStatus_vals_te;

typedef UINT8 dvfmToMsRequestStatus_t;

//ICAT EXPORTED ENUM
typedef enum{
    MODE_CHANGE_REQUEST,
	TRIGGER_FOR_ALREADY_IN_NEW_MODE
}msModeChangePurpose_vals_te;

typedef UINT8 msModeChangePurpose_t;

// hold an internal enumeration for the PPs because the platform enumeration holds values which are irrelevant to comm
// (COMMPM_PP_NOTSET) and here we want to hold arrays which use the pp number as index (valuesToSendToMsa)
//ICAT EXPORTED ENUM
typedef enum{
    PP1,
	PP2,
	PP3,
	NUM_OF_PPS
}commDvfmPP_vals_te;

typedef UINT8 commDvfmPP_t;

typedef void (*dvfmToMsAckCallbackFunc_t)(dvfmToMsRequestStatus_t reqStatus, mode_t ackMode);

void commDvfmMsToDvfmRegister(dvfmClientId_t clientId, dvfmToMsAckCallbackFunc_t callbackFunc);
void commDvfmMsToDvfmModeChangeReq(dvfmClientId_t clientId, mode_t newMode, msModeChangePurpose_t purpose);
commDvfmPP_t commDvfmModeToPP(dvfmClientId_t clientId, mode_t modeToMap);
void commDvfmManagerInit(void);
void commDvfmInitDspToInitialPP(void);

#endif //DVFM_EXT_API_H
