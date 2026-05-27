#ifndef _NTP_H_
#define _NTP_H_

//#define NTP

//#ifdef NTP
#include <time.h>
#define NTP_FLAG_BIT 0x1

void NTP_Init(void);
void SyncNTP(void);
time_t GetNtpResult(void);
int GetNTPStatus(void);


/** add by cmiot start */
#define CM_NTP_CUSTOM
typedef unsigned long       UINT32;
typedef void (*ntp_rsp_cb)(UINT32 handle, const char *time, int err);
typedef enum
{
    CM_NTP_TASK_IDLE = 0,
    CM_NTP_TASK_BUSY
} cm_ntp_status_e;

typedef enum
{
    CM_NTP_ERR_OK = 0,
    CM_NTP_ERR_PDP_DEACT = 572,
    CM_NTP_ERR_DNS_FAILED = 552,
    CM_NTP_ERR_TIMEOUT = 2,
    CM_NTP_ERR = 3
} cm_ntp_errcode_e;

typedef struct
{
    UINT32 handle;
    char server[255];
    char port[8];
    unsigned char sync_local;
    int  timeout;
    ntp_rsp_cb pfunc;
} cm_ntp_cfg_t;

int cm_sync_ntp_time(cm_ntp_cfg_t *info);

/** add by cmiot end */

//#endif

#endif

