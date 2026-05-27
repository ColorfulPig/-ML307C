// (C) Copyright 2006 Marvell International Ltd.
// All Rights Reserved
//

/*------------------------------------------------------------------------------
 *  ----------------------------------------------------------------------------
 *
 *  Filename: MRD.h
 *
 *  Description: The APIs to handle MRD file
 *
 *  History:
 *   June, 2010 - Shuki Zanyovka
 *
 *  Notes:
 *
 ******************************************************************************/
#if !defined (MRD_HEADER_FILE_H)
#define MRD_HEADER_FILE_H

#include "global_types.h"
#include "ReliableData.h"
#include "mep.h"
#include "teldef.h"

#define IMEI_MRD_NAME       "IMEI.bin"
#define MEP_MRD_NAME        "MEP.bin"
#define MEP2_MRD_NAME       "MEP2.bin"
#define CDF_RDSK_NAME       "CDF.bin"
#define SN_MRD_NAME         "SN.bin"
// syns from 1826
#define MEP_CMD_CAT_NETWORK_STR           "PN"
#define MEP_CMD_CAT_NETWORK_SUBSET_STR    "PU"
#define MEP_CMD_CAT_SERVICE_PROVIDER_STR  "PP"
#define MEP_CMD_CAT_CORPORATE_STR         "PC"
#define MEP_CMD_CAT_SIM_STR               "PS"
#define MEP_CMD_CAT_PASSWORD_STR          "PSW"
#define MEP_CMD_CAT_COMMON_PART_STR       "CP"
#define MEP_CMD_CAT_RAW_STR               "RAW"
#ifdef SS_FEATURE
#define MEP_MAX_PASSWORD_LEN   32
#else
#define MEP_MAX_PASSWORD_LEN   16
#endif
#define MEP_CMD_PARAM_MAX_CNT             9
#define MEP_CMD_CAT_STR_MAX_LEN           3
#define MEP_MCC_MNC_ACT_MAX_LEN           4
#define MEP_NETWORK_SUBNET_MAX_LEN        2
#define MEP_SERVICE_PROVIDER_MAX_LEN      2
#define MEP_CORPORATE_MAX_LEN             2
#define MEP_IMSI_MAX_LEN                  15
#define MEP_ENABLE_ACTIVATE_FLAG_MAX_LEN  2
#define MEP_LOCKNETLIST_MAX_LEN           1024
//bt macro
#define BT_ID_NUMBER_SIZE                6

//CID MEID etc
#define MAX_TYPE_LEN	10
#define MAX_Data_LEN 16
#define MAX_ADC_Data_LEN	10
#define MAX_ADC_GROUP_CNT	10

//Type of Reliable Data Components
//0xC...  -->  (int)0xC...u to fix build
//warnning "enumeration value is out of "int" range
typedef enum
{
    MRD_IMEI_TYPE       =   (int)0xCAFE0010u,
    MRD_MEP_TYPE        =   (int)0xCAFE0015u,
    MRD_CDF_TYPE        =   (int)0xCAFE0020u,
    MRD_SN_TYPE         =   (int)0xCAFE0025u,
    MRD_GEN_DATA_TYPE   =   (int)0xCAFE0030u,
    MRD_BT_ID_TYPE     =   (int)0xCAFE0035u,
    MRD_MAC_TYPE        =   (int)0xCAFE0040u,
    MRD_SSID_TYPE       =   (int)0xCAFE0060u,
    MRD_WIFI_PASSWORD_TYPE = (int)0xCAFE0070u,
    MRD_PID_TYPE        =   (int)0xCAFE0080u,
    MRD_VID_TYPE        =   (int)0xCAFE0090u,
    MRD_SIMKEY_TYPE     =   (int)0xCAFE00A0u,
    MRD_ADF_TYPE        =   (int)0xAAAA0035u,
    MRD_USBMODE_TYPE    =   (int)0xCAFE00B0u,
    MRD_HWVER_TYPE      =   (int)0xCAFE00C0u,
    MRD_SWVER_TYPE      =   (int)0xCAFE00D0u,
    MRD_PTVER_TYPE      =   (int)0xCAFE00E0u,
    MRD_DIALMODE_TYPE   =   (int)0xCAFE00F0u,
    MRD_MANUFACTURE_TYPE = (int)0xCAFE0100u,
    MRD_MODEL_TYPE       = (int)0xCAFE0110u,
    MRD_WIFI_AUTHMODE_TYPE = (int)0xCAFE0120u,
    MRD_WIFI_CIPHER_TYPE = (int)0xCAFE0130u,
    MRD_DIALAPN_TYPE     = (int)0xCAFE0140u,
    MRD_DIALIPTYPE_TYPE  = (int)0xCAFE0150u,
    MASTER_KEY_TYPE      = (int)0xCAFE0160u,
    MRD_BOARD_TYPE       = (int)0xCAFE0170u,
    MRD_CID_TYPE         = (int)0xCAFE0180u,
    MRD_MEID_TYPE        = (int)0xCAFE0190u,
    MRD_BSN_TYPE         = (int)0xCAFE01A0u,
    MRD_QR_TYPE          = (int)0xCAFE01B0u,
    MRD_HC_TYPE          = (int)0xCAFE01C0u,
    MRD_MMI_FLAG_TYPE    = (int)0xCAFE01D0u,
    MRD_FAC_FLAG_TYPE    = (int)0xCAFE01E0u,
	MRD_UID16_1_TYPE=(int)0xCAFE01E1,
	MRD_UID16_2_TYPE=(int)0xCAFE01E2,

    MRD_TTS_ENCRYPT_TYPE=(int)0xCAFE0200u,     //cmiot add by cmiot3000,@20221103
    MRD_LBS_NVCONFIG_TYPE = (int)0xCAFE0210U,
    MRD_CMCUS_NVCONFIG_TYPE = (int)0xCAFE0220U,
} MRD_FILE_TYPES;

typedef enum {
    MRD_NO_ERROR,
    MRD_ERROR,
    MRD_INVALID_ADDRESS,
    MRD_FILE_NOT_FOUND_ERROR,
    MRD_FILE_ALREADY_EXISTS_ERROR,
    MRD_FILE_WRITE_ERROR,
    MRD_FILE_READ_ERROR,
    MRD_NOT_INITIALIZED_ERROR,
    MRD_NO_VALID_STAMP_ERROR,
    MRD_INVALID_HEADER_ERROR,
    MRD_INVALID_FILE_HEADER_ERROR,
    MRD_INVALID_CHECKSUM_ERROR,
    MRD_NO_MEMORY_ERROR,
    MRD_ALREADY_INITIALIZED_ERROR,
    MRD_FILENAME_TOO_LONG,
    MRD_NO_SPACE_ERROR,
    MRD_INVALID_END_OF_FILE_ERROR
} MRDErrorCodes;

typedef enum diagCommFsResultCode
{
    DIAG_COMM_FS_OK,
    DIAG_COMM_FS_DISABLE,
    DIAG_COMM_FS_NOT_INIT,
    DIAG_COMM_FS_INVALID_NAME,
    DIAG_COMM_FS_ALREADY_INIT,
    DIAG_COMM_FS_NULL_POINTER,
    DIAG_COMM_FS_NO_FREE_SPACE,
    DIAG_COMM_FS_INVALID_LENGTH,
    DIAG_COMM_FS_INVALID_BUFFER,
    DIAG_COMM_FS_INVALID_FILE,
    DIAG_COMM_FS_WRITE_ERROR,
    DIAG_COMM_FS_OTHER_ERROR
}diagCommFsResultCode;

#if defined(CRANEL_4MRAM) || defined(CRANEL_8MRAM) || defined(LTEGSM_THIN)
#define MRD_FACTORY_MIN_SIZE
#endif

#ifdef MRD_FACTORY_MIN_SIZE
#if defined(LTEGSM_THIN)
#define MRD_MAX_FILE_LEN               0xA000//(40*1024)
#elif defined(SINGLE_SIM)
#define MRD_MAX_FILE_LEN               0x6000//(24*1024)
#else
#define MRD_MAX_FILE_LEN               0x8000//(32*1024)
#endif
#else
#define MRD_MAX_FILE_LEN               0x10000//(64*1024)
#endif

#define FLASH_SECTOR_SIZE               0x1000
#define MRD_IMAGE_TOTAL_SIZE           (MRD_MAX_FILE_LEN - FLASH_SECTOR_SIZE) // (64*1024) //last 4k is reserved for adc cal data

struct DIAG_MRD_CaliDate
{
    char Name[128];
    char Date[32];
    char Time[32];
};

struct DIAG_MRD_MEP
{
    UINT32 version;
    UINT32 date;
    MEP_FIX mep_fix;
};


typedef struct {
    UINT8 digits[6];
} BtIDData;



typedef enum
{
	ADC_VBAT = 0x0,
	ADC_RTP = 0x1,
	ADC_MAX = 0x2
}PM803_ADC_TYPE;


//ICAT EXPORTED ENUM
typedef enum
{
    TEMP_NORMAL = 0,
    TEMP_LOW,
    TEMP_HIGH,
    TEMP_TYPE_MAX
}ADC_TEMP_t;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT8	volt[MAX_ADC_Data_LEN];
	UINT8	data[MAX_ADC_Data_LEN];
}ADCValue;

//ICAT EXPORTED STRUCT
typedef struct
{
	ADCValue	value[MAX_ADC_GROUP_CNT];
	UINT32	flag;
}ADCInfo;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT32	version;
	UINT32	timestamp;
	ADCInfo	info;
}MRDADCInfoS;

//ICAT EXPORTED STRUCT
typedef struct
{
	ADCValue	value[TEMP_TYPE_MAX-1][MAX_ADC_GROUP_CNT];
	UINT8 temp[4];
	UINT32	flag;
}ADCInfo2;

//ICAT EXPORTED STRUCT
typedef struct
{
	UINT32	version;
	UINT32	timestamp;
	ADCInfo2	info;
}MRDADCInfoS2;

enum mep_cmd_category {
    MEP_CMD_CAT_NETWORK,
    MEP_CMD_CAT_NETWORK_SUBSET,
    MEP_CMD_CAT_SERVICE_PROVIDER,
    MEP_CMD_CAT_CORPORATE,
    MEP_CMD_CAT_SIM,
    MEP_CMD_CAT_PASSWORD,
    MEP_CMD_CAT_COMMON_PART,
    MEP_CMD_CAT_RAW,
    MEP_CMD_CAT_MAX
};

struct mep_cat_info
{
    unsigned short param_cnt;
    unsigned short max_len;
};

MRDErrorCodes MRDInit(unsigned char* mrd_area);
MRDErrorCodes MRDParseHeader(unsigned char *MRD_file_buffer);
MRDErrorCodes MRDOpen(void);
MRDErrorCodes MRDClose(void);
MRDErrorCodes MRDFreeFileBuffer(void);
MRDErrorCodes MRDFileAdd(char *MRD_filename,
                         UINT32 flags,
                         UINT32 mrd_file_type,
                         UINT32 file_format_version,
                         UINT32 timestamp,
                         char *input_filename);
MRDErrorCodes MRDFileRead(char *MRD_filename,
                          UINT32 mrd_file_type,
                          UINT32 *flags,
                          UINT32 *file_format_version,
                          UINT32 *timestamp,
                          char *output_filename);
MRDErrorCodes MRDFileExist(char *MRD_filename, UINT32 mrd_file_type);
MRDErrorCodes MRDFileRemove(char *MRD_filename, UINT32 MRD_file_type);
MRDErrorCodes MRDFileDirFirst(char *MRD_filename,
                              UINT32 *flags,
                              UINT32 *file_format_version,
                              UINT32 *timestamp);
MRDErrorCodes MRDFileDirNext(char *MRD_filename,
                             UINT32 *flags,
                             UINT32 *file_format_version,
                             UINT32 *timestamp);
void DiagCommCopyFileToMRD(char *FileName);
void DiagCommDelMRDFile(char *FileName);
void DiagCommMRDFileExist(char *FileName);
void DiagCommMRDUpdateMEP(struct DIAG_MRD_MEP *diag_mep, int length);
void DiagCommMRDDelMEP(void);
void DiagCommMRDDumpMEP(void);
void DiagCommMRDCopyMEPToNVM(void);
void DiagCommATCFUN(int* value);

RETURNCODE_T  AtMrdCdf(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
RETURNCODE_T  AtMrdImei(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
#ifndef REMOVE_MEP
RETURNCODE_T  AtMrdMep(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
#endif
RETURNCODE_T  AtCalInfo(const utlAtParameterOp_T op, const char *command_name_p,const utlAtParameterValue_P2c   parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void  *arg_p);
RETURNCODE_T  AtMrdSn(const utlAtParameterOp_T  op, const char *command_name_p,const utlAtParameterValue_P2c   parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
RETURNCODE_T  AtMrdItem(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
RETURNCODE_T  ATMRDBACKUP(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
#if (!defined(LTEONLY_THIN) && !defined(LTEGSM_THIN))
RETURNCODE_T  AtMrdWiFiMAC(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
#endif
void BtIDRead(BtIDData *btID);
void BtIDInit(unsigned long *ddrBuffaddr);
#ifdef BT_SUPPORT
RETURNCODE_T AtMrdBtMAC(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
#endif
RETURNCODE_T  AtMrdADC(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
RETURNCODE_T  AtMrdRtpADC(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);
RETURNCODE_T ciStarAVSIM(const utlAtParameterOp_T op, const char *command_name_p, const utlAtParameterValue_P2c parameter_values_p, const size_t num_parameters, const char *info_text_p, unsigned int *xid_p, void *arg_p);

void RDSaveADCFileToMRD(PM803_ADC_TYPE type);
int MRDItemGet(const char *itemType, char *Data);

int MRDAdcValueGet(UINT32 index, UINT32 *AdcValue, UINT32 *VoltageValue, PM803_ADC_TYPE type, ADC_TEMP_t tempType);
int MRDAdcFlagGet(PM803_ADC_TYPE type, ADC_TEMP_t tempType);
int MRDAdcCntGet(PM803_ADC_TYPE type, ADC_TEMP_t tempType);
UINT32 GetMRDFlashAddress(void);
UINT32 GetMRDBakFlashAddress(void);
void set_mrd_production_mode(UINT32 val);

void vbat_pm803_init(void);


#define MAX_DEVID_SIZE		512

#endif //MRD_HEADER_FILE_H
