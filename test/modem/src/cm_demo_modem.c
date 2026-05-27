#include "cm_os.h"
#include "cm_mem.h"
#include "cm_common.h"
#include "cm_sys.h"
#include "cm_demo_uart.h"
#include "cm_virt_at.h"
#include "cm_modem.h"

void cm_test_modem(unsigned char **cmd,int len)
{
    int cm_get_pin = cm_modem_get_cpin();
    cm_demo_printf("cm_get_pin:%d\n",cm_get_pin);

    char cgmr[32] = {0};
    cm_modem_get_cgmr(cgmr);
    cm_demo_printf("cgmr:%s\n",cgmr);

    char cgmm[10] = {0};
    cm_modem_get_cgmm(cgmm);
    cm_demo_printf("cgmm:%s\n",cgmm);

    char cgmi[10] = {0};
    cm_modem_get_cgmi(cgmi);
    cm_demo_printf("cgmi:%s\n",cgmi);

    cm_cops_info_t *cops = NULL;
    cops = cm_malloc(sizeof(cm_cops_info_t));
    memset(cops,0,sizeof(cm_cops_info_t));
    cm_modem_get_cops(cops);
    cm_demo_printf("cops->mode:%d\n", cops->mode);
    if(cops->mode)
    {
        cm_demo_printf("cops->act:%d cops->format:%d, cops->oper:%s\n",cops->act, cops->format, cops->oper);
    } 
    cm_free(cops);

    char rssi[10] = {0};
    char ber[10] = {0};
    cm_modem_get_csq(rssi,ber);
    cm_demo_printf("rssi:%s,ber:%s\n",rssi, ber);


    cm_radio_info_t *radio_info = NULL;
    radio_info = cm_malloc(sizeof(cm_radio_info_t));
    memset(radio_info,0,sizeof(cm_radio_info_t));
    cm_modem_get_radio_info(radio_info);
    cm_demo_printf("radio_info->last_cellid:%d,radio_info->last_earfcn:%d,radio_info->last_ecl:%d,radio_info->last_pci:%d,radio_info->last_snr:%d,radio_info->rat:%d,radio_info->rsrp:%d,radio_info->rsrq:%d,radio_info->rssi:%d,radio_info->rx_time:%d,radio_info->rxlev:%d,radio_info->tx_power:%d\n",
                    radio_info->last_cellid,   radio_info->last_earfcn,   radio_info->last_ecl,   radio_info->last_pci,   radio_info->last_snr,   radio_info->rat,   radio_info->rsrp,   radio_info->rsrq,   radio_info->rssi,   radio_info->rx_time,   radio_info->rxlev,   radio_info->tx_power);
    cm_free(radio_info);


    cm_cell_info_t cell_info[1];
    cm_modem_get_cell_info(cell_info,1);
    cm_demo_printf("cell_info[0].bandwidth:%d,cell_info[0].cid:%.8x,cell_info[0].earfcn:%d,cell_info[0].earfcn_offset:%d,cell_info[0].mcc:%s,cell_info[0].mnc:%s,cell_info[0].pci:%d,cell_info[0].primary_cell:%d,cell_info[0].rsrp:%d,cell_info[0].rsrq:%d,cell_info[0].rssi:%d,cell_info[0].snr:%d,cell_info[0].tac:%d\n",
                    cell_info[0].bandwidth,   cell_info[0].cid,   cell_info[0].earfcn,   cell_info[0].earfcn_offset,   cell_info[0].mcc,   cell_info[0].mnc,   cell_info[0].pci,   cell_info[0].primary_cell,   cell_info[0].rsrp,   cell_info[0].rsrq,   cell_info[0].rssi,   cell_info[0].snr,   cell_info[0].tac);

    int get_fun = cm_modem_get_cfun();
    cm_demo_printf("get_fun:%d\n",get_fun);

    cm_psm_cfg_t *psm_cfg = NULL;
    psm_cfg = cm_malloc(sizeof(cm_psm_cfg_t));
    memset(psm_cfg,0,sizeof(cm_psm_cfg_t));
    cm_modem_get_psm_cfg(psm_cfg);
    cm_demo_printf("psm_cfg->mode:%d, psm_cfg->requested_active_time:%d, psm_cfg->requested_periodic_tau:%d\n",psm_cfg->mode, psm_cfg->requested_active_time, psm_cfg->requested_periodic_tau);
    cm_free(psm_cfg);


    cm_cereg_state_t *cereg;
    cereg = cm_malloc(sizeof(cm_cereg_state_t));
    memset(cereg,0,sizeof(cm_cereg_state_t));
    cm_modem_get_cereg_state(cereg);
    cm_demo_printf("cereg->act:%d, cereg->active_time:%d, cereg->cause_type:%d,cereg->ci:%.8x, cereg->lac:%d,cereg->n:%d,cereg->periodic_tau:%d,cereg->rac:%d,cereg->reject_cause:%d,cereg->state:%d\n",
                    cereg->act,    cereg->active_time,    cereg->cause_type,   cereg->ci,    cereg->lac,cereg->n,      cereg->periodic_tau,   cereg->rac,   cereg->reject_cause,   cereg->state);
    cm_free(cereg);

    int get_cscon = cm_modem_get_cscon();
    cm_demo_printf("get_cscon:%d\n",get_cscon);

    int pdp_state = cm_modem_get_pdp_state(1);
    cm_demo_printf("pdp_state:%d\n",pdp_state);
}
