#ifndef __LPA_PLATFORM_H__
#define __LPA_PLATFORM_H__
#include "UART.h"

#define lpa_log(format, args...) CPUartLogPrintf("[LPA][%s]"format, __func__, ##args)

//#define ct_lpa_log(level, fmt, args...)	\
  //  {if (level >= 0) CPUartLogPrintf(fmt, ##args);}

typedef int (*https_post_cb)(char *, int, int, void *);
int https_request(char *url, char *contentType, int type, char *szPost, int szPostSize, https_post_cb cb_func,int bVerifyPeer, char *szCertPath, char *szCertFile);
int send_apduCom(char *buf , int len, char *rAPDU, int *rApduLen, int maxrApduLen);
int get_deviceIMEI(char *buf);
int set_cfun(int value);
void lpa_at_init(void);
int https_request_ctcc(unsigned char *url,unsigned char *szPost, void *header, unsigned char **postRsp);
int send_ccho_sync(unsigned char *dfname);
int send_cchc_sync(int session_id);
int send_ct_cgla_sync(int sessionid, unsigned char *tlvHexData, unsigned char *rspTlvData);
int lpa_sema_trywait(void);
int lpa_sema_wait(void);
void lpa_sema_post(void);
void ct_lpa_log(int level,const char *fmt, ...);

#endif
