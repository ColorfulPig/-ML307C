
/*******************************************************************************
  In ACAT, in order to open AP files in CP folder,
  We must export the definitions in CP code.
********************************************************************************/

#ifndef _ACM_AP_H_
    #define _ACM_AP_H_

#ifdef __cplusplus
extern "C"{
#endif


//ICAT EXPORTED ENUM
typedef enum
{
	ON = 0,
	OFF,

	ON_OFF_ENUM_32_BIT = 0x7FFFFFFF  /* 32bit enum compiling enforcement */
} On_Off;

//ICAT EXPORTED ENUM
typedef enum
{
	SPEAKER_PHONE,
	HANDSET,
	HEADSET,
	CAR_KIT,
	BT_HEADSET,
	SPEAKER_PHONE_HP,       //Non voice call filter

	APPLIANCE_TYPE_ENUM_32_BIT = 0x7FFFFFFF  /* 32bit enum compiling enforcement */
} Appliance;

//ICAT EXPORTED ENUM
typedef enum
{
	MP3,
	AAC,
	OGG,
	MP4,
	AVI,

	CONTAINER_TYPE_ENUM_32_BIT = 0x7FFFFFFF  /* 32bit enum compiling enforcement */
} Container_Type;

//ICAT EXPORTED ENUM
typedef enum
{
	SYSTEM,
	RINGTONE,
	MUSIC,
	ALARM,

	STREAM_TYPE_ENUM_32_BIT = 0x7FFFFFFF  /* 32bit enum compiling enforcement */
} Stream_Type;


//ICAT EXPORTED STRUCT
typedef struct
{
	unsigned short noOfTaps;
	signed short   params[6];
	Appliance      appliance;
	On_Off         onOff;
	unsigned char  description[32];
} AUDIO_HiFi_EQ_Params;

//ICAT EXPORTED STRUCT
typedef struct
{
    Container_Type containerType;
    Stream_Type    streamType;
    unsigned short volumeOffset;
} AUDIO_HiFi_Volume_Params;

#ifdef __cplusplus
}
#endif
#endif  /* _ACM_AP_H_ */