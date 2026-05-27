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

#ifndef _GPS_API_H_
#define _GPS_API_H_

#include "gps.h"

int   asr_gnss_open(GpsCallbacks *gps_callbacks);
int   asr_gnss_initial(int start_mode);
int   asr_gnss_deinitial(void);
int   asr_gnss_download_agpsdata(char *agps_servname, int alm_flag);
int   asr_gnss_send_agpsdata(void);
int   asr_gnss_set_workmode(int sleep_wakeup);
int   asr_gnss_get_gps_info(ASRGpsSvStatus *param);
int   asr_gnss_conf_gps(char *conf_param, signed short len);
int   asr_gnss_update_pvt(void * buff);
void  GnssLocationProvider_inject_location(
	double latitude, double longitude, double accuracy);
int   asr_gnss_check_agpsdata(void);
int asr_gps_update_agpsdata_by_app(char *data,int len);
int   asr_gnss_close(void);

#endif /* _GPS_UART_H_ */


