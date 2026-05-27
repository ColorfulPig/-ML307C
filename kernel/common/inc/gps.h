/*
 * Copyright (C) 2010 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ANDROID_INCLUDE_HARDWARE_GPS_H
#define ANDROID_INCLUDE_HARDWARE_GPS_H

#include <stdint.h>
#include <hardware.h>

/**
 * The id of this module
 */
#define GPS_HARDWARE_MODULE_ID "gps"


/** Milliseconds since January 1, 1970 */
typedef int64_t GpsUtcTime;

/** Maximum number of SVs for gps_sv_status_callback(). */
#define GPS_MAX_SVS 32

/** GPS status event values. */
typedef uint16_t GpsStatusValue;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GPS status unknown. */
#define GPS_STATUS_NONE             0
/** GPS has begun navigating. */
#define GPS_STATUS_SESSION_BEGIN    1
/** GPS has stopped navigating. */
#define GPS_STATUS_SESSION_END      2
/** GPS has powered on but is not navigating. */
#define GPS_STATUS_ENGINE_ON        3
/** GPS is powered off. */
#define GPS_STATUS_ENGINE_OFF       4

/** Flags to indicate which values are valid in a GpsLocation. */
typedef uint16_t GpsLocationFlags;
// IMPORTANT: Note that the following values must match
// constants in GpsLocationProvider.java.
/** GpsLocation has valid latitude and longitude. */
#define GPS_LOCATION_HAS_LAT_LONG   0x0001
/** GpsLocation has valid altitude. */
#define GPS_LOCATION_HAS_ALTITUDE   0x0002
/** GpsLocation has valid speed. */
#define GPS_LOCATION_HAS_SPEED      0x0004
/** GpsLocation has valid bearing. */
#define GPS_LOCATION_HAS_BEARING    0x0008
/** GpsLocation has valid accuracy. */
#define GPS_LOCATION_HAS_ACCURACY   0x0010


/** Represents a location. */
typedef struct {
    /** set to sizeof(GpsLocation) */
    size_t          size;
    /** Contains GpsLocationFlags bits. */
    uint16_t        flags;
    /** Represents latitude in degrees. */
    double          latitude;
    /** Represents longitude in degrees. */
    double          longitude;
    /** Represents altitude in meters above the WGS 84 reference
     * ellipsoid. */
    double          altitude;
    /** Represents speed in meters per second. */
    float           speed;
    /** Represents heading in degrees. */
    float           bearing;
    /** Represents expected accuracy in meters. */
    float           accuracy;
    /** Timestamp for the location fix. */
    GpsUtcTime      timestamp;
} GpsLocation;

/** Represents the status. */
typedef struct {
    /** set to sizeof(GpsStatus) */
    size_t          size;
    GpsStatusValue status;
} GpsStatus;

/** Represents SV information. */
typedef struct {
    /** set to sizeof(GpsSvInfo) */
    size_t          size;
    /** Pseudo-random number for the SV. */
    int     prn;
    /** Signal to noise ratio. */
    int   snr;
    /** Elevation of SV in degrees. */
    int   elevation;
    /** Azimuth of SV in degrees. */
    int   azimuth;
} GpsSvInfo;

/** Represents SV status. */
typedef struct {
    /** set to sizeof(GpsSvStatus) */
    size_t          size;

    /** Number of SVs currently visible. */
    int         num_svs;

    /** Contains an array of SV information. */
    GpsSvInfo   sv_list[GPS_MAX_SVS];

    /** Represents a bit mask indicating which SVs
     * have ephemeris data.
     */
    uint32_t    ephemeris_mask;

    /** Represents a bit mask indicating which SVs
     * have almanac data.
     */
    uint32_t    almanac_mask;

    /**
     * Represents a bit mask indicating which SVs
     * were used for computing the most recent position fix.
     */
    uint32_t    used_in_fix_mask;
} GpsSvStatus;



/** ASR SV Report status. */
#define ASR_MAX_SVS      (6u)
typedef struct {
    int     svid;
    int     snr;
} ASRGpsSvInfo;

typedef struct {
    /** Number of SVs currently visible. */
    unsigned int    fix_state;
    unsigned int    module_state;
    double          latitude;
    double          longitude;
    double          altitude;
    int                  svs_num;
    int                 buf[GPS_MAX_SVS];           
    /** Contains an array of SV information. */
    ASRGpsSvInfo   asr_sv_list[ASR_MAX_SVS];
} ASRGpsSvStatus;



/** Callback with location information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (* gps_location_callback)(GpsLocation* location);

/** Callback with status information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (* gps_status_callback)(GpsStatus* status);

/** Callback with SV status information.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (* gps_sv_status_callback)(GpsSvStatus* sv_info);

/** Callback for reporting NMEA sentences.
 *  Can only be called from a thread created by create_thread_cb.
 */
typedef void (* gps_nmea_callback)(GpsUtcTime timestamp, const char* nmea, int length);


/** GPS callback structure. */
typedef struct {
    /** set to sizeof(GpsCallbacks) */
    size_t      size;
    gps_location_callback location_cb;
    gps_status_callback status_cb;
    gps_sv_status_callback sv_status_cb;
    gps_nmea_callback nmea_cb;
} GpsCallbacks;


/** Represents the standard GPS interface. */
typedef struct {
    /** set to sizeof(GpsInterface) */
    size_t          size;
    /**
     * Opens the interface and provides the callback routines
     * to the implemenation of this interface.
     */
    int   (*init)( GpsCallbacks* callbacks );

    /** Starts navigating. */
    int   (*start)( void );

    /** Stops navigating. */
    int   (*stop)( void );

    /** Closes the interface. */
    void  (*cleanup)( void );

    /** Injects current location from another location provider
     *  (typically cell ID).
     *  latitude and longitude are measured in degrees
     *  expected accuracy is measured in meters
     */
    int  (*inject_location)(double latitude, double longitude, double accuracy);


    /**
    *   Description: hardware poweroff 
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*gps_deinitial)(void);

    /**
    *   Description: Close GNSS Forcedly. 
    *       New API is added for UI Team.
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*close)(void);
    
    /**
    *   Description: recv assist data from agps server.
    *   
    *   param[0]: agps_servname is agps server name.
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*download_agpsdata)(char *agps_servname, int alm_flag);

    /**
    *   Description: send assist data to gps firmware.
    *   
    *   param[0]: NULL.
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*send_agpsdata)(void);

    /**
    *   Description: set gps hardware mode, sleep or wakeup.
    *   
    *   param[0]: 0 -> sleep, 1 -> wakeup.
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*set_workmode)(int sleep_wakeup);

    /**
    *   Description: get gps hardware status.
    *   
    *   param[0]: fix status, 0 -> not fix, 1 -> fix
    *   param[1]: longitude
    *   param[2]: latitude
    *   param[3]: altitude
    *   param[4]~param[15]:CNO and CNO_val
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*get_gps_info)(ASRGpsSvStatus *param);

    /**
    *   Description: Send Config Cmd to GPS Module.
    *   
    *   param[0]: conf_param, config gps.
    *   
    *   Returns:
    *   0 -> success
    *   1 -> failed
    */
    int (*conf_gps)(char *conf_param, signed short len);

    int (*check_agpsdata_isvalid)(void);
} GpsInterface;


struct gps_device_t {
    struct hw_device_t common;

    /**
     * Set the provided lights to the provided values.
     *
     * Returns: 0 on succes, error code on failure.
     */
    const GpsInterface* (*get_gps_interface)(struct gps_device_t* dev);
};


typedef enum {
ASR_GPS_INITIAL_SUCCESS             = 0,
ASR_GPS_INITIAL_FAILED              = 1,
ASR_GPS_INITIALED                   = 2,
ASR_GPS_DOWNLOAD_SUCCESS            = 3,
ASR_GPS_DOWNLOAD_FAIL               = 4,
ASR_GPS_SEND_DATA_SUCCESS           = 5,
ASR_GPS_SEND_DATA_FAIL              = 6,
ASR_GPS_DEINIT_SUCCESS,
ASR_GPS_DEINIT_FAIL,
}USER_CB_STATE_T;


#define  ASR_UART_FIFO_SIZE                (6400)

#endif /* ANDROID_INCLUDE_HARDWARE_GPS_H */

