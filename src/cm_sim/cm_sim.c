#include <stdlib.h>
#include <stdio.h>
#include "cm_sys.h"
#include "cm_virt_at.h"
#include "cm_sim.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "cm_sys.h"
#include "cm_os.h"


/*
 * 获取ICCID函数
 * 通过AT指令获取SIM卡的ICCID号
 * 参数:
 *   iccid: 用于存储ICCID号的缓冲区
 * 返回值:
 *   成功返回0，失败返回-1
 */
int32_t cm_sim_get_iccid(char* iccid)
{
    char iccid_str[21] = {0};
    int32_t get_iccid = -1;
    uint8_t rsp[64] = {0};
    int32_t rsp_len = sizeof(rsp);
    if(cm_virt_at_send_sync((const uint8_t *)"AT*ICCID?\r\n", rsp, &rsp_len, 200) == 0)
    {
        sscanf((const char *)rsp, "%*[^\"]\"%[^\"]\"", iccid_str); 
        memcpy(iccid, iccid_str, strlen(iccid_str));
        get_iccid = 0;
    }
    return get_iccid;
}