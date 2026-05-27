/************************************************************************
*
*  File Name:  gps_uart.h
*
*  Description:  uart for gps
*
*  Rev  Author		 Date		Changes
*  ---  ----------------------	------------------
*  1.0  joshua     12/31/2020	Initial draft
*************************************************************************/


#ifndef _GPS_HW_CONFIG_H_
#define _GPS_HW_CONFIG_H_

/*========macro defined for customer control start========*/
#ifndef  PROJ_ASR_APP_001
/*========修改为只能手动更新，不能自动更新-zyf/20231127========*/
//#define AGPS_UPDATE_DEFAULT
#define AGPS_UPDATE_ONESHOT
//#define CUSTOMER_AGPS_UPDATE_ENABLE
#else
/* MMI */
//#define AGPS_UPDATE_ONESHOT
//#define AGPS_UPDATE_ONESHOT
#define CUSTOMER_AGPS_UPDATE_ENABLE
#endif
/*========macro defined for customer control end========*/

/*
 * Pls Check The Platform which is used.
 */
#ifdef CRANE_MCU_DONGLE

#if defined CRANELR_SUPPORT
#define JACANA_ASR1602_DKB_V10_ASR
#elif (defined(CRANELS_SUPPORT) || defined(CRANELG_SUPPORT))
#define JACANA_ASR1609_DKB_V10_ASR
#elif defined(CRANEL_4MRAM)|| defined(CRANEL_8MRAM)
#define JACANA_ASR1606L_DKB_V10_ASR
#elif defined  CRANEM_GOOME_4G03N
#define JACANA_ASR1603_4G03N_V10_GOOME
#else
#define JACANA_ASR1606L_DKB_V10_ASR
#endif

/* MCU -> CranG Platform.*/
#else
#if defined CRANEL_FP_8MRAM
#define JACANA_ASR3602_DKB_V10_ASR
#else
#define JACANA_ASR3603_DKB_V10_ASR
#endif
#endif

#if defined(JACANA_ASR1603_4G03N_V10_GOOME) || defined(JACANA_ASR1602_DKB_V10_ASR)
#define VRCR_MODE_VCXO_REQ
#else
#define VRCR_MODE_MN_CLK_REQ
#endif

#ifdef  JACANA_ASR1603_DKB_V00_ASR
#define HOST_RESET_GPS_GPIO		(12)
#define HOST_UART_RX_GPIO                    (51)
#define HOST_UART_TX_GPIO                    (52)
#define GPS_WAKE_HOST_GPIO		(20)
#define HOST_WAKE_GPS_GPIO		(31)

#define GPS_UART_PORT                       (UART_PORT_1)

#elif defined  JACANA_ASR1603_DKB_V11_ASR
#define HOST_RESET_GPS_GPIO		(15)
#define HOST_UART_RX_GPIO                    (0)
#define HOST_UART_TX_GPIO                    (1)
#define GPS_WAKE_HOST_GPIO		(121)
#define HOST_WAKE_GPS_GPIO		(14)

#define GPS_UART_PORT                       (UART_PORT_3)

#elif defined  JACANA_ASR1603_4G03N_V10_GOOME

#define HOST_RESET_GPS_GPIO		(14)
#define HOST_UART_RX_GPIO                    (4)
#define HOST_UART_TX_GPIO                    (5)
#define GPS_WAKE_HOST_GPIO		(13)
#define HOST_WAKE_GPS_GPIO		(12)

#define GPS_UART_PORT                       (UART_PORT_3)

#elif defined  JACANA_ASR1606L_DKB_V10_ASR

#define HOST_RESET_GPS_GPIO		(37)
#define HOST_UART_RX_GPIO                    (35)
#define HOST_UART_TX_GPIO                    (36)
#define GPS_WAKE_HOST_GPIO		(34)
#define HOST_WAKE_GPS_GPIO		(33)

#define GPS_UART_PORT                       (UART_PORT_2)

#elif defined  JACANA_ASR3603_DKB_V10_ASR

#define HOST_RESET_GPS_GPIO		(34)
#define HOST_UART_RX_GPIO       (51)
#define HOST_UART_TX_GPIO       (52)
#define GPS_WAKE_HOST_GPIO		(75)
#define HOST_WAKE_GPS_GPIO		(31)

#define GPS_UART_PORT                       (UART_PORT_1)

#elif defined  JACANA_ASR3602_DKB_V10_ASR

#define HOST_RESET_GPS_GPIO		(37)
#define HOST_UART_RX_GPIO                    (35)
#define HOST_UART_TX_GPIO                    (36)
#define GPS_WAKE_HOST_GPIO		(34)
#define HOST_WAKE_GPS_GPIO		(33)

#define GPS_UART_PORT                       (UART_PORT_2)

#elif defined  JACANA_ASR1602_DKB_V10_ASR

#define HOST_RESET_GPS_GPIO		(68)
#define HOST_UART_RX_GPIO                    (14)
#define HOST_UART_TX_GPIO                    (15)
#define GPS_WAKE_HOST_GPIO		(73)
#define HOST_WAKE_GPS_GPIO		(12)

#define GPS_UART_PORT                       (UART_PORT_2)

#endif


#ifdef  VRCR_MODE_MN_CLK_REQ
#define  VRCR_ADDR	(0xD4050018)
#define  CLK_REQ               (0xD401E0CC)
#endif

#ifdef VRCR_MODE_VCXO_REQ
#define VRCR_ADDR	(0xD4050018)
#define CLK_REQ           (0xD401E0D4)
#endif

#define VCXO_OUT       (0xD401E0D8) 

#if (defined(CRANELS_SUPPORT) || defined(CRANELG_SUPPORT))
#define GPS_CMD_TIMER    0
#define GPS_FWDL_TIMER  0 
#define GPS_SWDT_TIMER  1 
#else
#define GPS_CMD_TIMER    1
#define GPS_FWDL_TIMER  1 
#define GPS_SWDT_TIMER  1 

#endif

#if (defined(CRANELS_SUPPORT) || defined(CRANELG_SUPPORT))
#define GPS_WAKELOCK_UART_RECV          (1 << 0)
#define GPS_WAKELOCK_IPC          		        (1 <<1)
#else
#define GPS_WAKELOCK_UART_RX_DMA        (1 << 0)
#define GPS_WAKELOCK_UART_RECV               (1 << 1)
#define GPS_WAKELOCK_UART_TX_DMA        (1 << 2)
#endif


#define ASR_GPS_WORKMODE_WAKEUP                1   
#define ASR_GPS_WORKMODE_POWEROFF              2
#define ASR_GPS_WORKMODE_SLEEP                 0


#define ASR_GPS_STATE_POWEROFF                 0
#define ASR_GPS_STATE_ACTIVE                   1
#define ASR_GPS_STATE_SLEEP                    2
#define ASR_GPS_STATE_SLEEPING                 3
#define ASR_GPS_STATE_POWEROFFING              4

/*Global Settings for Debug.    */
#define ABOOT_DOWNLOAD_DEBUG         1

#define JACANA_ASR_PVT_DATA_LEN          (4096u)
#define JACANA_NMEA_PACK_FIFO_SIZE     	 (2048u)
#define JACANA_PVT_PACK_FIFO_SIZE        (5120u)

#if (!defined(CRANELS_SUPPORT) && !defined(CRANELG_SUPPORT))
void    gps_uart_init(void);
size_t  gps_uart_get_rxbuffer2(unsigned char *msg);
void    gps_uart_cfg(UINT32 baudrate);

void    gps_wake_host_gpio_init(void);

void    set_callback_execute_endflag(const BOOL flag);
#endif

/*Global Flag.  */
void    set_gpsfw_download_done(void);
void    reset_gpsfw_download_state(void);

/* GPS Module Wakelock  */
void clear_gps_pm_lock(unsigned char lock_type);
void set_gps_pm_lock(unsigned char lock_type);
unsigned char get_gps_pm_lock(void);

#if (!defined(CRANELS_SUPPORT) && !defined(CRANELG_SUPPORT))
/*Applied By Aboot-tiny.    */
char * get_ringbuffer_addr(void);
size_t  get_rx_data_len(void);
#endif

#if (defined(CRANELS_SUPPORT) || defined(CRANELG_SUPPORT))
void gps_module_init(void);
void gps_module_register_ipc_interrupt(void);
unsigned int gps_vuart_get_rxbuffer(unsigned int index, unsigned char *msg);
unsigned int get_ringbuffer_len_by_idx(int index);
#endif

void    gps_module_reset(void);
void    gps_module_poweron(void);
void    gps_module_sleep(void);
void    gps_module_poweroff(void);
void    gps_module_wakeup(void);
void    gps_module_boot(void);

/* GPS Module Status.   */
int       asr_gps_status_get(void);
void    asr_gps_status_set(int type);

/* GPS WorkMode.        */
int     asr_gps_set_workmode2(int mode);
int     asr_gps_set_workmode(int mode);

void    gps_uart_send_raw_msg2(unsigned char *msg, unsigned int len);
void    gps_uart_send_raw_msg(unsigned char *msg, unsigned int len);
void    gps_uart_send_config_cmd(char *msg);
void    set_gpsuart_dma_last_cnt(unsigned int cnt);


void    asr_gps_start_keepalive_timer(void);
void    asr_gps_stop_keepalive_timer(void);

int     asr_gps_send_initc(void);
int     asr_gps_update_agpsdata_default(void);


void    asr_gps_clear_fatalinfo(void);
void    asr_gps_update_fatalinfo(unsigned int type);
void    asr_gps_set_errinfo(unsigned int type, unsigned char *errinfo);

int     asr_gps_send_mcc(unsigned short int mcc);


#endif /* _GPS_HW_CONFIG_H_ */

