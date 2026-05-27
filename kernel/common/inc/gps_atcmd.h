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

#ifndef __GPS_ATCMD_H__
#define __GPS_ATCMD_H__

#include "osa.h"
#include <stdbool.h>

typedef enum {
    GPS_AT_INIT         = 0,
    GPS_AT_DOWNLOAD,
    GPS_AT_SEND,
    GPS_AT_DEINIT,
    GPS_PVT_UPDATE,
    GPS_WORKMODE_BOOT = 5,
    GPS_WORKMODE_PM4,
    GPS_WORKMODE_PM5,
    GPS_UPDATE_INITC,
    GPS_SEND_IPC_MSG,
    GPS_RUNTEST_BOOT = 10,
    GPS_RUNTEST_PM4,
    GPS_CLEAR_PVT,
    GPS_GET_CELLID,
    GPS_GET_PLMN,
/***please add in order when need add new type.****/
}GPS_AT_MSG_E;

typedef struct {
    int alm_flag;
    char servname[64];
}GPS_AT_DL_MSG;

typedef struct {
    unsigned char *buff;
    unsigned int len;
}MCU_GNSS_IPC_MSG;

typedef void (*ASR_GPS_USER_CALLBACK)(USER_CB_STATE_T event, void *data, int daya_len);
void asr_gps_msgQ_send(GPS_AT_MSG_E event, void *arg);
unsigned char asr_gps_msgQ_send2(unsigned int event, void *arg);
void asr_gnss_reg_user_cb(ASR_GPS_USER_CALLBACK cb);
void asr_gps_create_atcmd_thread(void);
bool check_gps_atcmd_task_and_msgQ(void);
unsigned int asr_gps_get_packinfo_byname( const char* packinfo_name);
int asr_gps_download_agpsdata(char *agps_servname, int alm_flag);
void asr_aboot_dl_thread_delete( void );

#endif
