
#include <stdlib.h>
#include <stdio.h>
#include "cm_sys.h"
#include "cm_sms.h"
#include "cm_common.h"
#ifndef __CM_DEMO_SMS_H__
#define __CM_DEMO_SMS_H__

void cm_virt_at_urc_cb_reg_test(unsigned char **cmd,int len);
void cm_virt_at_urc_cb_dereg_test(unsigned char **cmd,int len);
void cm_sendmsg_test_txt(unsigned char **cmd,int len);
void cm_sendmsg_test_pdu(unsigned char **cmd,int len);

#endif
