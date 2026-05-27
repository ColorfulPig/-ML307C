#ifndef __LPA_API_H__
#define __LPA_API_H__

int esim_init_eid_iccid(void);
void  reset_esim_card_iccid(void);
void  esim_card_download_profile(void);
void at_esim_send_ct_rsp_start(char *smdp_addr, char *match_id, int flag);
void at_esim_send_ct_notify_leab(char *iccid);
int lpa_sdk_sem_wait(void);
int lpa_sdk_sem_post(void);
int lpa_get_sdk_status(void);
int do_show_base_information(unsigned int atHandle, unsigned char *iccid);
void at_esim_send_ct_query_eid(unsigned int atHandle);
int at_esim_send_ct_query_iccid(unsigned int atHandle, char *iccid);
int at_esim_send_ct_act_profile(unsigned int atHandle, char *iccid);

#endif
