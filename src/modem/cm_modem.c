#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "cm_sys.h"
#include "cm_virt_at.h"
#include "cm_modem_info.h"
#include "cm_modem.h"
#include "cm_modem_info.h"
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "cm_os.h"

int32_t cm_modem_get_cpin(void)
{
    int32_t cm_cpin = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CPIN?\r\n", rsp, &rsp_len, 32) == 0)
    {
		if(0 == strncmp((char *)rsp, "+CPIN: READY", 11))
        {
			cm_cpin = 0;
        }
    }
    return cm_cpin;
}

int32_t cm_modem_get_cgmr(char *cgmr)
{
    int32_t get_cgmr = -1;
    uint8_t rsp[32] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CGMR\r\n", rsp, &rsp_len, 32) == 0)
    {
        strcpy(cgmr, (char *)rsp);
        get_cgmr = 0;
    }
    return get_cgmr;
}

int32_t cm_modem_get_cgmm(char *cgmm)
{
    int32_t get_cgmm = -1;
    uint8_t rsp[8] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CGMM\r\n", rsp, &rsp_len, 32) == 0)
    {
        strcpy(cgmm, (char *)rsp);
        get_cgmm = 0;
    }
    return get_cgmm;
}

int32_t cm_modem_get_cgmi(char *cgmi)
{
    int32_t get_cgmi = -1;
    uint8_t rsp[6] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CGMI\r\n", rsp, &rsp_len, 32) == 0)
    {
        strcpy(cgmi, (char *)rsp);
        get_cgmi = 0;
    }
    return get_cgmi;
}

int32_t cm_modem_get_cops(cm_cops_info_t *cops)
{
    int32_t get_cops = -1;
    uint8_t rsp[30] = {0};
    int32_t rsp_len = 0; 
    if(cm_virt_at_send_sync((const uint8_t *)"AT+COPS?\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (strncmp((char *)rsp, "+COPS", 5) == 0)
        {
            // 解析 COPS 信息，数据格式: "+COPS: <mode>,<format>,<oper>,<act>"
            sscanf((char *)rsp, "+COPS: %hhu,%hhu,\"%17[^\"]\",%hhu", 
            &cops->mode, &cops->format, cops->oper, &cops->act);
            get_cops = 0;
            cops->oper[16] = '\0';
        }                        
    }
    return get_cops;
}

int32_t cm_modem_get_csq(char *rssi, char *ber)
{
    int32_t get_csq = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    char oc_rssi[8] = {0};
    char oc_ber[8] = {0};
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CSQ\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (strncmp((char *)rsp, "+CSQ", 4) == 0)
            {
                // 解析 CSQ 信息，数据格式: "+CSQ: <rssi>,<ber>"
                if (sscanf((char *)rsp, "+CSQ: %[^,],%[^,]", oc_rssi, oc_ber) == 2)
                {
                    // 成功读取了 rssi 和 ber 的值
                    strcpy(rssi, oc_rssi);
                    strcpy(ber, oc_ber);
                    get_csq = 0;
                }
            }
    }
    return get_csq;
}

int32_t cm_modem_get_radio_info(cm_radio_info_t *radio_info)
{
    int ret = -1; 
    cm_cops_info_t *cops = NULL;
    cops = malloc(sizeof(cm_cops_info_t));
    ret = cm_modem_get_cops(cops);
    if(cops != NULL)
    {
        radio_info->rat = cops->mode;
    }
    free(cops);
    cm_modem_info_radio(radio_info);
    return ret;
}

int32_t cm_modem_get_cell_info(cm_cell_info_t cell_info[], uint16_t cell_info_num)
{
    int ret = -1;
    ret = cm_modem_info_cell(cell_info,cell_info_num);
    return ret;
}

int32_t cm_modem_set_cfun(uint16_t fun)
{
    int32_t ret = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    char cfun_cmd[12] = {0};
    snprintf(cfun_cmd, sizeof(cfun_cmd), "AT+CFUN=%hd\r\n", fun);
    if(cm_virt_at_send_sync((const uint8_t *)cfun_cmd, rsp, &rsp_len, 32) == 0)
    {
        if(0 == strncmp((char *)rsp, "OK", 2))
        {
            ret = 0;
        }
    }
    return ret;
}

int32_t cm_modem_get_cfun(void)
{
    int32_t cm_cfun = -1;
    uint8_t rsp[10] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CFUN?\r\n", rsp, &rsp_len, 32) == 0)
    {       
        if (strncmp((char *)rsp, "+CFUN", 5) == 0)
        {
            // 解析 CFUN 信息，数据格式: "+CFUN: <cfun_value>"
            sscanf((char *)rsp, "+CFUN: %ld", &cm_cfun);
        }
    }
    return cm_cfun;
}

int32_t cm_modem_set_psm_cfg(const cm_psm_cfg_t *cfg)
{
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    int32_t set_cfg = -1;
    char cfg_cmd[50] = {0};
    char tau_tmp[9] = {0};
    char active_time_tmp[9] = {0};
    itoa((cfg->requested_periodic_tau), tau_tmp, 2);
    itoa((cfg->requested_active_time), active_time_tmp, 2);
	if( 0 == cfg->mode)
	{
		snprintf(cfg_cmd, sizeof(cfg_cmd), "AT+CPSMS=%d\r\n", cfg->mode);
	}
	else 
	{
   	    snprintf(cfg_cmd, sizeof(cfg_cmd), "AT+CPSMS=%d,,, %s, %s\r\n", cfg->mode, tau_tmp, active_time_tmp);
	}
	if(cm_virt_at_send_sync((const uint8_t *)cfg_cmd, rsp, &rsp_len, 32) == 0)
    {
        if(0 == strncmp((char *)rsp, "OK", 2))
        {
            set_cfg = 0;
        }
    }
    return set_cfg;
}

int32_t cm_modem_get_psm_cfg(cm_psm_cfg_t *cfg)
{
    int32_t get_cfg = -1;
    uint8_t tau_tmp = 0;
    uint8_t active_time_tmp = 0;
    int8_t periodic_tau[10] = {0};
    int8_t active_time[10] = {0};
    uint8_t rsp[40] = {0};
    int32_t rsp_len = 0;
    char mode = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CPSMS?\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (sscanf((char *)rsp, "+CPSMS: %hhu,%*[^,],%*[^,],\"%[^\"]\",\"%[^\"]", &mode, periodic_tau, active_time) == 3)
        {
            // 成功读取了所有参数
            tau_tmp = (unsigned char) strtol((char *)periodic_tau, NULL, 2);
            active_time_tmp = (unsigned char) strtol((char *)active_time, NULL, 2);
            cfg->mode = (unsigned char)mode;
            cfg->requested_active_time = active_time_tmp;
            cfg->requested_periodic_tau = tau_tmp; 
            get_cfg = 0;
        }
    }
    return get_cfg;
}

int32_t cm_modem_get_cereg_state(cm_cereg_state_t *cereg)
{
    int32_t get_cereg_state = -1;
    uint8_t rsp[40] = {0};
    uint32_t ci_num = 0;
    uint16_t lac_num = 0;
    char n = 0;
    char state[5]  = {0};
    char lac[7]  = {0};
    char ci[11]  = {0};
    char act[5]  = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CEREG=5;+CEREG?\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (strncmp((char *)rsp, "+CEREG", 6) == 0)
        {
            // 解析 CEREG 信息，数据格式: "+CEREG: <n>,<state>,<lac>,<ci>,<act>"
            if (sscanf((char *)rsp, "+CEREG: %hhu,%[^,],%[^,],%[^,],%s", &n, state, lac, ci, act) == 5)
            {
                // 成功读取了所有参数
                for(int32_t i = 8;i > 0;i--)
                {
                    if(ci[i] >= 'A')
                    {
                        ci_num = ci_num + pow(16, (8 - i))*(ci[i] - 55);
                    }
                    else
                    {
                        ci_num = ci_num + pow(16, (8 - i))*(ci[i] - 48);
                    }
                }
                for(int32_t i = 4; i > 0; i--)
                {
                    lac_num = lac_num + pow(10, (4 - i))*(lac[i] - 48);
                }    
                cereg->n = (uint8_t)n;
                cereg->state = (uint8_t)atoi(state);
                cereg->lac = lac_num;
                cereg->ci = ci_num;
                cereg->act =  (uint8_t)atoi(act);
                get_cereg_state = 0;
            }
        } 
    }
    return get_cereg_state;
}

int32_t cm_modem_get_cscon(void)
{
    int32_t cm_cscon = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CSCON?\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (sscanf((char *)rsp, "+CSCON: %*[^,],%*[^,],%ld", &cm_cscon) == 1)
        {
            // 成功读取了 cm_cscon 的值
            // 这里可以添加对 cm_cscon 的有效性检查
        }
    }
    return cm_cscon;
}

int32_t cm_modem_activate_pdp(uint16_t cid)
{
    int32_t pdp_r = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    char at_cmd[20] ={0};
    char cid_cmd[5] = {0};
    itoa((uint16_t)cid, cid_cmd, 10);
    snprintf(at_cmd, sizeof(at_cmd), "AT+CGACT=1, %s\r\n", cid_cmd);
    if(cm_virt_at_send_sync((const uint8_t *)at_cmd, rsp, &rsp_len, 32) == 0)
    {
        if(0 == strncmp((char *)rsp, "OK", 2))
        {
            pdp_r = 0;
        }
    }
    return pdp_r;
}

int32_t cm_modem_deactivate_pdp(uint16_t cid)
{
    int32_t pdp_r = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    char at_cmd[20] ={0};
    char cid_cmd[5] = {0};
    itoa((uint16_t)cid, cid_cmd, 10);
    snprintf(at_cmd, sizeof(at_cmd), "AT+CGACT=0, %s\r\n", cid_cmd);
    if(cm_virt_at_send_sync((const uint8_t *)at_cmd, rsp, &rsp_len, 32) == 0)
    {
        if(0 == strncmp((char *)rsp, "OK", 2))
        {
            pdp_r = 0;
        }
    }
    return pdp_r;
}

int32_t cm_modem_get_pdp_state(uint16_t cid)
{
    int32_t pdp_state = -1;
    uint8_t rsp[16] = {0};
    int32_t rsp_len = 0;
    if(cm_virt_at_send_sync((const uint8_t *)"AT+CGACT?\r\n", rsp, &rsp_len, 32) == 0)
    {
        if (strncmp((char *)rsp, "+CGACT", 6) == 0) 
            {
                // 解析 CGACT 信息，数据格式: "+CGACT: <cid>,<state>"
                char *ptr = (char *)rsp;
                while (1)
                {
                    // 跳过以 "+" 开头的部分
                    ptr = strstr(ptr, "+CGACT:");
                    if (ptr == NULL) break; // 没有找到下一组数据，退出循环
                    
                    // 解析字段
                    unsigned short cid_read = 0;
                    int state_read = 0;

                    // 使用 sscanf 读取 cid 和 state
                    if (sscanf(ptr, "+CGACT: %hu,%d", &cid_read, &state_read) == 2)
                    {
                        if (cid == cid_read)
                        {
                            pdp_state = state_read;
                            break; // 找到目标，退出循环
                        }
                    }

                    // 移动到下一个可能的 "+CGACT:" 子串
                    ptr += strlen("+CGACT:");
                }
            }
    }
    return pdp_state;
}
