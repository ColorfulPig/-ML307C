/**
 * @file        dmp.c
 * @brief       DMP入库功能认证，示例仅供参考
 * @copyright   Copyright © 2024 China Mobile IOT. All rights reserved.
 * @author      by shimingrui
 */

#include "cm_dmp.h"
#include "dmp.h"
#include "cm_fs.h"
#include "cm_sys.h"
#include "cm_pm.h"
#include "cm_modem.h"
#include "cm_os.h"

#define min(a, b) (((a) < (b)) ? (a) : (b))

/*
    appkey 和 pswd 填入自己申请到的账号密码或者联系技术支持获取测试值
*/
static cmdmp_nv_cfg_t s_cmdmp_nv_cfg = {
    .enable = 1,
    .interval = 1,
    .appkey = "",
    .pswd = "",
#ifdef DM40_SUPPORT	
	.tmplID = "TY000123",	
#endif
    .host = "shipei.fxltsbl.com",
    .port = 5683
};

static void cm_mobile_dm_init(void)
{	
    /*判断是否启用DM功能*/
    if(s_cmdmp_nv_cfg.enable == 0)
    {
        cm_log_printf(0, "dm func disabled");
        return;
    }

    int appkey_len = strlen(s_cmdmp_nv_cfg.appkey);
    int pswd_len = strlen(s_cmdmp_nv_cfg.pswd);
    int host_len = strlen(s_cmdmp_nv_cfg.host);

    /*参数有效性检测*/
    if(appkey_len == 0 ||
       pswd_len == 0 ||
       host_len == 0 ||
       s_cmdmp_nv_cfg.interval < 1 || s_cmdmp_nv_cfg.interval > 1440)
    {
        cm_log_printf(0, "dm param err");
        return;
    }

   int appinfo_len = strlen(s_cmdmp_nv_cfg.appinfo);
   int mac_len = strlen(s_cmdmp_nv_cfg.mac);
   int rom_len = strlen(s_cmdmp_nv_cfg.rom);
   int ram_len = strlen(s_cmdmp_nv_cfg.ram);
   int cpu_len = strlen(s_cmdmp_nv_cfg.cpu);
   int osver_len = strlen(s_cmdmp_nv_cfg.osver);
   int swver_len = strlen(s_cmdmp_nv_cfg.swver);
   int swname_len = strlen(s_cmdmp_nv_cfg.swname);
   int volte_len = strlen(s_cmdmp_nv_cfg.volte);
   int nettype_len = strlen(s_cmdmp_nv_cfg.nettype);
   int account_len = strlen(s_cmdmp_nv_cfg.account);
   int phone_len = strlen(s_cmdmp_nv_cfg.phonenum);
   int location_len = strlen(s_cmdmp_nv_cfg.location);

#ifdef DM40_SUPPORT	
	int brand_len = strlen(s_cmdmp_nv_cfg.brand);
	int model_len = strlen(s_cmdmp_nv_cfg.model);
	int tmplID_len = strlen(s_cmdmp_nv_cfg.tmplID);
	int BatCap_len = strlen(s_cmdmp_nv_cfg.BatCap);
	int scrnSz_len = strlen(s_cmdmp_nv_cfg.scrnSz);
	int rtMac_len = strlen(s_cmdmp_nv_cfg.rtMac);
	int btMac_len = strlen(s_cmdmp_nv_cfg.btMac);
	int gpu_len = strlen(s_cmdmp_nv_cfg.gpu);
	int board_len = strlen(s_cmdmp_nv_cfg.board);
	int RES_len = strlen(s_cmdmp_nv_cfg.RES);
#endif

    /*参数设置*/
    cm_mobile_dm_set_option(CM_MOBILE_DM_APPKEY, appkey_len == 0 ? NULL : s_cmdmp_nv_cfg.appkey, appkey_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_PASSWORD, pswd_len == 0 ? NULL : s_cmdmp_nv_cfg.pswd, pswd_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_HOST, host_len == 0 ? NULL : s_cmdmp_nv_cfg.host, host_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_APPINFO, appinfo_len == 0 ? NULL : s_cmdmp_nv_cfg.appinfo, appinfo_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_MAC, mac_len == 0 ? NULL : s_cmdmp_nv_cfg.mac, mac_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_ROM, rom_len == 0 ? NULL : s_cmdmp_nv_cfg.rom, rom_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_RAM, ram_len == 0 ? NULL : s_cmdmp_nv_cfg.ram, ram_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_CPU, cpu_len == 0 ? NULL : s_cmdmp_nv_cfg.cpu, cpu_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_OSVER, osver_len == 0 ? NULL : s_cmdmp_nv_cfg.osver, osver_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_SWVER, swver_len == 0 ? NULL : s_cmdmp_nv_cfg.swver, swver_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_SWNAME, swname_len == 0 ? NULL : s_cmdmp_nv_cfg.swname, swname_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_VOLTE, volte_len == 0 ? NULL : s_cmdmp_nv_cfg.volte, volte_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_NETTYPE, nettype_len == 0 ? NULL : s_cmdmp_nv_cfg.nettype, nettype_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_ACCOUNT, account_len == 0 ? NULL : s_cmdmp_nv_cfg.account, account_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_PHONENUM, phone_len == 0 ? NULL : s_cmdmp_nv_cfg.phonenum, phone_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_LOCATION, location_len == 0 ? NULL : s_cmdmp_nv_cfg.location, location_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_INTERVAL, &(s_cmdmp_nv_cfg.interval), sizeof(s_cmdmp_nv_cfg.interval));
    cm_mobile_dm_set_option(CM_MOBILE_DM_PORT, &(s_cmdmp_nv_cfg.port), sizeof(s_cmdmp_nv_cfg.port));
    cm_mobile_dm_set_option(CM_MOBILE_DM_TMLTYPE, &(s_cmdmp_nv_cfg.tmltype), sizeof(s_cmdmp_nv_cfg.tmltype));

#ifdef DM40_SUPPORT	
    cm_mobile_dm_set_option(CM_MOBILE_DM_BRAND, brand_len == 0 ? NULL : s_cmdmp_nv_cfg.brand, brand_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_MODEL, model_len == 0 ? NULL : s_cmdmp_nv_cfg.model, model_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_TMPLID, tmplID_len == 0 ? NULL : s_cmdmp_nv_cfg.tmplID, tmplID_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_BATCAP, BatCap_len == 0 ? NULL : s_cmdmp_nv_cfg.BatCap, BatCap_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_SRCNSZ, scrnSz_len == 0 ? NULL : s_cmdmp_nv_cfg.scrnSz, scrnSz_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_RTMAC, rtMac_len == 0 ? NULL : s_cmdmp_nv_cfg.rtMac, rtMac_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_BTMAC, btMac_len == 0 ? NULL : s_cmdmp_nv_cfg.btMac, btMac_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_GPU, gpu_len == 0 ? NULL : s_cmdmp_nv_cfg.gpu, gpu_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_BOARD, board_len == 0 ? NULL : s_cmdmp_nv_cfg.board, board_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_RES, RES_len == 0 ? NULL : s_cmdmp_nv_cfg.RES, RES_len);
    cm_mobile_dm_set_option(CM_MOBILE_DM_WEARING, &(s_cmdmp_nv_cfg.wearing), sizeof(s_cmdmp_nv_cfg.wearing));
    cm_mobile_dm_set_option(CM_MOBILE_DM_ENABLE, &(s_cmdmp_nv_cfg.enable), sizeof(s_cmdmp_nv_cfg.enable));	
#endif

    /*启动*/
    cm_mobile_dm_start();
}

static void cm_dm40_test(void)
{
    /* 等待模组PDP激活 */
    int32_t pdp_time_out = 0;
    
    while(1)
    {
        if(pdp_time_out > 20)
        {
            cm_log_printf(0, "network timeout\n");
            cm_pm_reboot();
        }
        if(cm_modem_get_pdp_state(1) == 1)
        {
            cm_log_printf(0, "network ready\n");
            break;
        }
        osDelay(200);
        pdp_time_out++;
    }

    /* 适当等待（模组PDP激活到能进行数据通信需要适当等待，或者通过PING操作确定模组数据通信是否可用） */
    osDelay(1000);
    
    cm_mobile_dm_init();
}

/** opencpu程序主入口，禁止任何阻塞操作*/
int cm_opencpu_entry(void *arg)
{
    (void)arg;
    osThreadAttr_t app_task_attr = {0};
    app_task_attr.name  = "main_task";
    app_task_attr.stack_size = 4096 * 2;
    app_task_attr.priority = osPriorityNormal;

    osThreadNew((osThreadFunc_t)cm_dm40_test, 0, &app_task_attr);
    return 0;
}
