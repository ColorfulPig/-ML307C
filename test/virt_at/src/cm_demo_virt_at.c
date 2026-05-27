#include "cm_os.h"
#include "cm_mem.h"
#include "cm_common.h"
#include "cm_sys.h"
#include "cm_demo_uart.h"
#include "cm_virt_at.h"
#include "cm_demo_virt_at.h"

void oc_resp_cb(cm_virt_at_param_t *param)
{
    if(param->event == CM_VIRT_AT_OK)
    {
        cm_demo_printf("oc_resp_cb:event=%d rsp=%s rsp_len=%d user_param=%s\n", param->event, param->rsp, param->rsp_len, param->user_param);
    }
    else
    {
        cm_demo_printf("error\n");
    }
}

void cm_virt_at_test_sync(unsigned char **cmd,int len)
{
    char operation[64] = {0};
    sprintf(operation, "%s\r\n", cmd[2]);
    uint8_t rsp[128] = {0};
    int32_t rsp_len = 0;

    if(cm_virt_at_send_sync((const uint8_t *)operation, rsp, &rsp_len, 10) == 0)
    {
        cm_demo_printf("rsp=%s rsp_len=%d\n", rsp, rsp_len);
    }
    else
    {
        cm_demo_printf("ret != 0\n");
    }

}

void cm_virt_at_test_async(unsigned char **cmd,int len)
{
    char operation[64] = {0};
    sprintf(operation, "%s\r\n", cmd[2]);
    if(cm_virt_at_send_async((const uint8_t *)operation, oc_resp_cb, "operation") != 0)
    {
        cm_demo_printf("cm_virt_at_send_async(, oc_resp_cb, operation) != 0");
    }

}
