#ifndef __ECALL_MSD_H__
#define __ECALL_MSD_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char EMSD_CHAR_T;
typedef char EMSD_INT8_T;
typedef unsigned char EMSD_UINT8_T;
typedef int EMSD_BOOL_T;
typedef long EMSD_INTERGER_T;
typedef unsigned long EMSD_UINTERGER_T;
typedef unsigned long EcallMsdContexHandle_t;

//#define ECALL_MSD_LOG_DUMP_TO_FILE (1)

#define ECALL_MSD_DB_ALL     1
#define ECALL_MSD_DB_INFO    2
#define ECALL_MSD_DB_EVENT   3
#define ECALL_MSD_DB_WARNING 4
#define ECALL_MSD_DB_ERROR   5
#define ECALL_MSD_DB_NONE    6
#define ECALL_MSD_DB_MEMORY  7
#define ECALL_MSD_DB_PRINT   (6)
#define ECALL_MSD_DB_ASSERT  0

/** A bit mask for the log level (DB_*) flag. */
#define ECALL_MSD_SYS_DB_MASK  0x00000F

#ifndef SYS_LOGLEVEL
#define SYS_LOGLEVEL ECALL_MSD_DB_ALL
#endif
#ifndef SYS_LOGMODULE
#define SYS_LOGMODULE ""
#endif
#define _SYS_LOGPREFIX(szMod) ((szMod)[0] == 0 ? "" : szMod ": ")
#define _SYS_LOGLEVEL ((SYS_LOGLEVEL) & ECALL_MSD_SYS_DB_MASK)

typedef EMSD_UINTERGER_T Sys_Logflags_t;

extern void EcallMsd_SysDebug_Print
    (
    Sys_Logflags_t Flags,
    const char * Str,
    ...
    );
#define EcallMsdSysDebug(Flags, Str, ...)  EcallMsd_SysDebug_Print( Flags, Str, ## __VA_ARGS__)
#if 0
#define EcallMsdSysDebug(Flags, Str, ...)                                             \
    ((((Flags) & ECALL_MSD_SYS_DB_MASK) >= _SYS_LOGLEVEL) ? EcallMsd_SysDebug_Print(             \
    Flags, Str, ## __VA_ARGS__) : (void) 0)
#endif

#define EMSD_HANDLE_INVALID   ((EcallMsdContexHandle_t) 0)

#define EMSD_TRUE   ((EMSD_BOOL_T) 1)
#define EMSD_FALSE   ((EMSD_BOOL_T) 0)

#define ECALL_MSDR_OK   ((EMSD_INTERGER_T) 0)
#define ECALL_MSDR_FAIL ((EMSD_INTERGER_T) -1)
#define ECALL_MSDR_ENOMEM ((EMSD_INTERGER_T) -2)
#define ECALL_MSDR_INVALID_ARG ((EMSD_INTERGER_T)-3)
#define ECALL_MSDR_NOT_SUPPORT ((EMSD_INTERGER_T)-4)
#define ECALL_MSDR_MR_NO_READY ((EMSD_INTERGER_T)-5)
#define ECALL_MSDR_ENG_NOT_READY ((EMSD_INTERGER_T)-6)
#define ECALL_MSDR_EUNKNOWN ((EMSD_INTERGER_T)-7)

#define ECALL_MSD_BIN_MAX_LEN   ((EMSD_INTERGER_T) 140)
#define ECALL_MSD_XER_MAX_LEN   ((EMSD_INTERGER_T) 5120)

#define ECALL_MSD_VERSION2   ((EMSD_INTERGER_T) 2)
#define ECALL_MSD_CURRENT_MSD_VERSION   (ECALL_MSD_VERSION2)

/*each new ecall should start with 
  the ECALL_MSD_BASE_MESSAGE_IDENTIFIER
  */
#define ECALL_MSD_BASE_MESSAGE_IDENTIFIER     ((EMSD_INTERGER_T) 1)
#define ECALL_MSD_MESSAGE_IDENTIFIER_MIN     ((EMSD_INTERGER_T) 1)
#define ECALL_MSD_MESSAGE_IDENTIFIER_MAX     ((EMSD_INTERGER_T) 255)

/*automaticActivation*/
#define ECALL_MSD_AUTOMATIC_ACTIVATION_VALUE_TRUE    ((EMSD_BOOL_T) 1)
#define ECALL_MSD_AUTOMATIC_ACTIVATION_VALUE_FALSE   ((EMSD_BOOL_T) 0)
#define ECALL_MSD_AUTOMATIC_ACTIVATION_VALUE_INVALID ((EMSD_BOOL_T) 0XFF)

/*testCall*/
#define ECALL_MSD_TEST_CALL_VALUE_TRUE    ((EMSD_BOOL_T) 1)
#define ECALL_MSD_TEST_CALL_VALUE_FALSE   ((EMSD_BOOL_T) 0)
#define ECALL_MSD_TEST_CALL_VALUE_INVALID ((EMSD_BOOL_T) 0XFF)

/*positionCanBeTrusted*/
#define ECALL_MSD_POSITION_CAN_BE_TRUSTD_VALUE_TRUE    ((EMSD_BOOL_T) 1)
#define ECALL_MSD_POSITION_CAN_BE_TRUSTD_VALUE_FALSE   ((EMSD_BOOL_T) 0)
#define ECALL_MSD_POSITION_CAN_BE_TRUSTD_VALUE_INVALID ((EMSD_BOOL_T) 0XFF)

/*vehicleType*/
#define ECALL_MSD_VEHICLE_TYPE_VALUE_INVALID ((EMSD_INTERGER_T) 0XFF)

/*vehiclePropulsionStorageType*/
#define ECALL_MSD_VEHICLE_PROPULSION_STORAGE_TYPE_VALUE_DEFAULT ((EMSD_BOOL_T) 0)
#define ECALL_MSD_VEHICLE_PROPULSION_STORAGE_TYPE_VALUE_TRUE ((EMSD_BOOL_T) 1)
#define ECALL_MSD_VEHICLE_PROPULSION_STORAGE_TYPE_VALUE_FALSE ((EMSD_BOOL_T) 0)

/*TimeStamp*/
#define ECALL_MSD_TIME_STAMP_VALUE_DEFAULT ((EMSD_UINTERGER_T) 0)

/*positionLatitude*/
#define ECALL_MSD_POSTION_LATITUDE_VALUE_INVALID ((EMSD_INTERGER_T) 0)

/*positionLongitude*/
#define ECALL_MSD_POSTION_LONGITUDE_VALUE_MIN ((EMSD_INTERGER_T) -2147483648)
#define ECALL_MSD_POSTION_LONGITUDE_VALUE_MAX ((EMSD_INTERGER_T) 2147483647)
#define ECALL_MSD_POSTION_LONGITUDE_VALUE_INVALID ((EMSD_INTERGER_T) ECALL_MSD_POSTION_LONGITUDE_VALUE_MAX)

/*vehicleDirection*/
#define ECALL_MSD_VEHICLE_DIRECTION_VALUE_INVALID ((EMSD_INTERGER_T) 255)
#define ECALL_MSD_VEHICLE_DIRECTION_VALUE_MIN ((EMSD_INTERGER_T) 0)
#define ECALL_MSD_VEHICLE_DIRECTION_VALUE_MAX ((EMSD_INTERGER_T) 255)

/*latitudeDelta*/
#define ECALL_MSD_LATITUDE_DELTA_VALUE_INVALID ((EMSD_INTERGER_T) 1000)
#define ECALL_MSD_LATITUDE_DELTA_VALUE_MIN ((EMSD_INTERGER_T) -512)
#define ECALL_MSD_LATITUDE_DELTA_VALUE_MAX ((EMSD_INTERGER_T) 511)

/*numberOfPassengers*/
#define ECALL_MSD_NUMBER_OF_PASSENGERS_VALUE_INVALID ((EMSD_INTERGER_T) 1000)
#define ECALL_MSD_NUMBER_OF_PASSENGERS_VALUE_MIN ((EMSD_INTERGER_T) 0)
#define ECALL_MSD_NUMBER_OF_PASSENGERS_VALUE_MAX ((EMSD_INTERGER_T) 255)

/*ECALL_RU33464_DiagnosticResult*/
#define ECALL_RU33464_DIAGNOSTIC_RESULT_VALUE_INVALID ((EMSD_BOOL_T) 0xff)
#define ECALL_RU33464_DIAGNOSTIC_RESULT_BOOL_TRUE ((EMSD_BOOL_T) 1)
#define ECALL_RU33464_DIAGNOSTIC_RESULT_BOOL_FALSE ((EMSD_BOOL_T) 0)

/*ECALL_RU33464_CrashInfo*/
#define ECALL_RU33464_CRASH_INFO_VALUE_INVALID ((EMSD_BOOL_T) 0xff)
#define ECALL_RU33464_CRASH_INFO_BOOL_TRUE ((EMSD_BOOL_T) 1)
#define ECALL_RU33464_CRASH_INFO_BOOL_FALSE ((EMSD_BOOL_T) 0)

#define ECALL_RU33464_ERA_ADDITIONAL_DATA_VERSION2   ((EMSD_INTERGER_T) 2)
#define ECALL_CURRENT_RU33464_ERA_ADDITIONAL_DATA_VERSION   (ECALL_RU33464_ERA_ADDITIONAL_DATA_VERSION2)

/*coordinateSystemType*/
#define ECALL_RU33464_COORDINATE_SYSTEM_TYPE_VALUE_INVALID   ((EMSD_INTERGER_T) 0)
#define ECALL_RU33464_COORDINATE_SYSTEM_TYPE_VALUE_WGS84     ((EMSD_INTERGER_T) 1)
#define ECALL_RU33464_COORDINATE_SYSTEM_TYPE_VALUE_PZ90      ((EMSD_INTERGER_T) 2)

/*crashSeverityASI15*/
#define ECALL_RU33464_CRASH_SEVERITY_ASI15_INVALID_VALUE      ((EMSD_INTERGER_T) 0xffffffff)
#define ECALL_RU33464_CRASH_SEVERITY_ASI15_MIN_VALUE          ((EMSD_INTERGER_T) 0)
#define ECALL_RU33464_CRASH_SEVERITY_ASI15_MAX_VALUE          ((EMSD_INTERGER_T) 2074)

#define ECALL_RU33464_OP_ADDITIONAL_DATA_OID_V2_STR          "1.4.2"

/* DiagnosticResult */
typedef struct _ECALL_RU33464_V2_DiagnosticResult_t {
	EMSD_BOOL_T	micConnectionFailure	/* OPTIONAL */;
	EMSD_BOOL_T	micFailure	/* OPTIONAL */;
	EMSD_BOOL_T	rightSpeakerFailure	/* OPTIONAL */;
	EMSD_BOOL_T	leftSpeakerFailure	/* OPTIONAL */;
	EMSD_BOOL_T	speakersFailure	/* OPTIONAL */;
	EMSD_BOOL_T	ignitionLineFailure	/* OPTIONAL */;
	EMSD_BOOL_T	uimFailure	/* OPTIONAL */;
	EMSD_BOOL_T	statusIndicatorFailure	/* OPTIONAL */;
	EMSD_BOOL_T	batteryFailure	/* OPTIONAL */;
	EMSD_BOOL_T	batteryVoltageLow	/* OPTIONAL */;
	EMSD_BOOL_T	crashSensorFailure	/* OPTIONAL */;
	EMSD_BOOL_T	firmwareImageCorruption	/* OPTIONAL */;
	EMSD_BOOL_T	commModuleInterfaceFailure	/* OPTIONAL */;
	EMSD_BOOL_T	gnssReceiverFailure	/* OPTIONAL */;
	EMSD_BOOL_T	raimProblem	/* OPTIONAL */;
	EMSD_BOOL_T	gnssAntennaFailure	/* OPTIONAL */;
	EMSD_BOOL_T	commModuleFailure	/* OPTIONAL */;
	EMSD_BOOL_T	eventsMemoryOverflow	/* OPTIONAL */;
	EMSD_BOOL_T	crashProfileMemoryOverflow	/* OPTIONAL */;
	EMSD_BOOL_T	otherCriticalFailires	/* OPTIONAL */;
	EMSD_BOOL_T	otherNotCriticalFailures	/* OPTIONAL */;
} ECALL_RU33464_V2_DiagnosticResult_t;

/* CrashInfo */
typedef struct _ECALL_RU33464_V2_CrashInfo_t {
	EMSD_BOOL_T	crashFront	/* OPTIONAL */;
	EMSD_BOOL_T	crashLeft	/* OPTIONAL */;
	EMSD_BOOL_T	crashRight	/* OPTIONAL */;
	EMSD_BOOL_T	crashRear	/* OPTIONAL */;
	EMSD_BOOL_T	crashRollover	/* OPTIONAL */;
	EMSD_BOOL_T	crashSide	/* OPTIONAL */;
	EMSD_BOOL_T	crashFrontOrSide	/* OPTIONAL */;
	EMSD_BOOL_T	crashAnotherType	/* OPTIONAL */;
} ECALL_RU33464_V2_CrashInfo_t;

/* ERAAdditionalData */
typedef struct _ECALL_RU33464_V2_ERAAdditionalData_t {
    EMSD_INTERGER_T version;
	EMSD_INTERGER_T	crashSeverityASI15	/* OPTIONAL */;
	ECALL_RU33464_V2_DiagnosticResult_t stDiagnosticResult	/* OPTIONAL */;
	ECALL_RU33464_V2_CrashInfo_t	stCrashInfo	/* OPTIONAL */;
	EMSD_INTERGER_T	coordinateSystemType	/* DEFAULT 1 */;
}ECALL_RU33464_V2_ERAAdditionalData_t;

typedef struct _EallMsdBuff_t {
	EMSD_UINT8_T *buf;
	EMSD_INTERGER_T size;
} EallMsdBuff_t;

typedef struct _EallMsdAdditionalData_t{
    EallMsdBuff_t OID;
    EallMsdBuff_t Data;
} EallMsdAdditionalData_t;

typedef enum _EallMsdVehicleType{
    EallMsdVehicleTypeMaxNone = 0,
    passengerVehicleClassM1,
    busesAndCoachesClassM2,
    busesAndCoachesClassM3,
    lightCommercialVehiclesClassN1,
    heavyDutyVehiclesClassN2,
    heavyDutyVehiclesClassN3,
    motorcyclesClassL1e,
    motorcyclesClassL2e,
    motorcyclesClassL3e,
    motorcyclesClassL4e,
    motorcyclesClassL5e,
    motorcyclesClassL6e,
    motorcyclesClassL7e,
    EallMsdVehicleTypeMaxCount,
} EallMsdVehicleType;

typedef struct _EallMsdVehicleLocationDelta_t{
    EMSD_INTERGER_T latitudeDelta;
    EMSD_INTERGER_T longitudeDelta;
} EallMsdVehicleLocationDelta_t;

typedef struct _EallMsdVehicleLocation_{
    EMSD_INTERGER_T positionLatitude;
    EMSD_INTERGER_T positionLongitude;
} EallMsdVehicleLocation_t;

typedef struct _EallMsdVehiclePropulsionStorageType_t{
    EMSD_BOOL_T gasolineTankPresent;
    EMSD_BOOL_T dieselTankPresent;
    EMSD_BOOL_T compressedNaturalGas;
    EMSD_BOOL_T liquidPropaneGas;
    EMSD_BOOL_T electricEngergyStorage;
    EMSD_BOOL_T hydrogenStorage;
    EMSD_BOOL_T otherStorage;
} EallMsdVehiclePropulsionStorageType_t;

typedef struct _EallMsdVIN_t{
	EallMsdBuff_t	 isowmi;
	EallMsdBuff_t	 isovds;
	EallMsdBuff_t	 isovisModelyear;
	EallMsdBuff_t	 isovisSeqPlant;
} EallMsdVIN_t;

typedef struct _EallMsdControlType_t{
    EMSD_BOOL_T automaticActivation;
    EMSD_BOOL_T testCall;
    EMSD_BOOL_T positionCanBeTrusted;
    EallMsdVehicleType vehicleType;
} EallMsdControlType_t;

typedef struct _EallMsdMSDStructure_t{
	EMSD_INTERGER_T messageIdentifier;/*M*/
	EallMsdControlType_t controlType;/*M*/
	EallMsdVIN_t VIN;/*M*/
    EallMsdVehiclePropulsionStorageType_t vehiclePropulsionStorageType;/*M*/
    EMSD_UINTERGER_T timestamp;/*M*/
    EallMsdVehicleLocation_t vehicleLocation;/*M*/
    EMSD_INTERGER_T vehicleDirection;/*M*/
    EallMsdVehicleLocationDelta_t recentVehicleLocationN1Delta;/*O*/
    EallMsdVehicleLocationDelta_t recentVehicleLocationN2Delta;/*O*/
    EMSD_INTERGER_T numberOfPassengers;/*O*/

} EallMsdMSDStructure_t;

typedef struct _EallMsdMSDMessage_t{
	EallMsdMSDStructure_t msdStructure;
	EallMsdAdditionalData_t additionalData;
} EallMsdMSDMessage_t;

typedef struct _EallMsdInfo_t{
	EMSD_INTERGER_T msdVersion;/*M*/
	EallMsdMSDMessage_t msdMessage;
} EallMsdInfo_t;

/*_ECALL_RU33464_V2_ERAAdditionalData_t*/
typedef enum _EcallMsdElemType
{
    ECALL_MSD_ELEM_TYPE_NONE = 0,
    ECALL_MSD_ELEM_MESSAGE_IDENTIFIER,
    ECALL_MSD_ELEM_CONTROL_TYPE,
    ECALL_MSD_ELEM_VIN,
    ECALL_MSD_ELEM_VEHICLE_PROPULSION_STORAGE_TYPE,
    ECALL_MSD_ELEM_TIMESTAMP,
    ECALL_MSD_ELEM_VEHICLE_LOCATION,
    ECALL_MSD_ELEM_VEHICLE_DIRECTION,
    ECALL_MSD_ELEM_VEHICLE_RECENT_VEHICLE_LOCATION_N1_DELTA,
    ECALL_MSD_ELEM_VEHICLE_RECENT_VEHICLE_LOCATION_N2_DELTA,
    ECALL_MSD_ELEM_NUMBER_OF_PASSENGERS,
    ECALL_MSD_ELEM_ADDITIONAL_DATA,
    ECALL_MSD_ELEM_TYPE_MAX_COUNT,
}EcallMsdElemType;

typedef enum _EcallMsdCmdType
{
    ECALL_MSD_CMD_TYPE_NONE = 0,
    ECALL_MSD_CMD_SET_ELEM,
    ECALL_MSD_CMD_GET_ELEM,
    ECALL_MSD_CMD_RESET_ELEM,
    ECALL_MSD_CMD_GENERATE_MSD,
    ECALL_MSD_CMD_DECODE_MSD,
    ECALL_MSD_CMD_FREE_MSD_INFO,
    ECALL_MSD_CMD_GEN_ADDI_DATA,
    ECALL_MSD_CMD_DECODE_RU33464_ADDI_DATA,
    ECALL_MSD_CMD_TYPE_MAX_COUNT,
}EcallMsdCmdType;

typedef enum _EcallMsdAddiDataType
{
    ECALL_MSD_ADDI_DATA_TYPE_NONE = 0,
    ECALL_MSD_ADDI_DATA_TYPE_RU33464,
    ECALL_MSD_ADDI_DATA_TYPE_MAX_COUNT,
}EcallMsdAddiDataType;

typedef struct _EcallMsdCmdDataSetElem_t
{
    EcallMsdElemType eEcallMsdElemType;
    void *pstData;
}EcallMsdCmdDataSetElem_t;

typedef struct _EcallMsdCmdDataGetElem_t
{
    EcallMsdElemType eEcallMsdElemType;
    void *pstData;
}EcallMsdCmdDataGetElem_t;

typedef struct _EcallMsdCmdDataResetElem_t
{
    EcallMsdElemType eEcallMsdElemType;
}EcallMsdCmdDataResetElem_t;

typedef struct _EcallMsdCmdDataGenMSD_t
{
    EallMsdBuff_t stEallMsdBuff;
}EcallMsdCmdDataGenMSD_t;

typedef struct _EcallMsdCmdGenAddiData_t
{
    EcallMsdAddiDataType eType;
    union
    {
        ECALL_RU33464_V2_ERAAdditionalData_t stRU33464ERAAdditionalData;
    }
    uAddiDataRaw;
    EallMsdAdditionalData_t additionalData;
}EcallMsdCmdGenAddiData_t;

typedef struct _EcallMsdCmdDataDecodeMSD_t
{
    EallMsdBuff_t stEallMsdBuff;
    EallMsdInfo_t *pstEallMsdInfo;
}EcallMsdCmdDataDecodeMSD_t;

typedef struct _EcallMsdCmdFreeMSDInfo_t
{
    EallMsdInfo_t *pstEallMsdInfo;
    EMSD_BOOL_T   isContentOnly;
}EcallMsdCmdFreeMSDInfo_t;

EMSD_INTERGER_T EcallMsd_Init (void);
EMSD_INTERGER_T EcallMsd_DeInit (void);
EMSD_INTERGER_T EcallMsd_CreateContext (EcallMsdContexHandle_t *pstHandle);
EMSD_INTERGER_T EcallMsd_DestroyContext (EcallMsdContexHandle_t stHandle);
EMSD_INTERGER_T EcallMsd_DoCmd (EcallMsdContexHandle_t stHandle,
            EcallMsdCmdType eEcallMsdCmdType,
            void *pstTag);
EMSD_INTERGER_T EcallMsd_init_RU33464V2ERRAAdditionalData(ECALL_RU33464_V2_ERAAdditionalData_t *pstData);
EMSD_INTERGER_T EcallMsd_Free_EcallMSDInfo(EallMsdInfo_t *pstEallMsdInfo);
EMSD_INTERGER_T EcallMsd_Clean_EcallMSDInfo(EallMsdInfo_t *pstEallMsdInfo);
EMSD_INTERGER_T EcallMsd_init_EcallMSDInfo(EallMsdInfo_t *pstEallMsdInfo);
EMSD_BOOL_T EcallMsd_Is_NumberOfPassengersValid (EMSD_INTERGER_T numberOfPassengers);
EMSD_BOOL_T EcallMsd_Is_RvlDeltaValid (EallMsdVehicleLocationDelta_t *pstRvlDelta);
EMSD_BOOL_T EcallMsd_Is_AdditionalDataValid (EallMsdAdditionalData_t *pstEallMsdAdditionalData);
EMSD_INTERGER_T EcallMsd_Clean_EcallMsdBuff(EallMsdBuff_t *pstEallMsdBuff);
void EcallMsd_SetDebugMode(int flag);

#endif


