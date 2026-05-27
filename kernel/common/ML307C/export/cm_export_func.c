#include "cm_os.h"
#include "cm_uart.h"
#include "cm_mem.h"
#include "cm_sys.h"
#include "cm_rtc.h"
#include "cm_fs.h"
#include "cm_sim.h"
#include "cm_asocket.h"
#include "cm_async_dns.h"
#include "cm_ssl.h"
#include "cm_fota.h"
#include "cm_ota.h"
#include "cm_http.h"
#include "cm_adc.h"
#include "cm_i2c.h"
#include "cm_spi.h"
#include "cm_pwm.h"
#include "cm_keypad.h"
#include "cm_lbs.h"
#include "cm_gpio.h"
#include "cm_iomux.h"
#include "cm_pm.h"
#include "cm_cpuinfo.h"
#include "cm_audio_player.h"
#include "cm_wifiscan.h"
#include "cm_virt_at.h"
#include "cm_sms.h"
#include "cm_modem_info.h"
#include "cm_usb.h"
#include "cm_lcd.h"
#include "cm_lwm2m.h"
#include "cm_ping.h"
#include "sockets.h"
#include "netdb.h"
#include "AudioHAL.h"
#include "vpath_proxy.h"
#include "cm_onewire.h"


int cm_core_stub_init(void)
{
    int test = 0;
    test += (int)osKernelLock;
    test += (int)osKernelUnlock;
    test += (int)osKernelGetTickFreq;
    test += (int)osKernelGetTickCount;
    test += (int)osThreadNew;
    test += (int)osThreadGetName;
    test += (int)osThreadGetId;
    test += (int)osThreadGetState;
    test += (int)osThreadGetStackSize;
    test += (int)osThreadGetStackSpace;
    test += (int)osThreadTerminate;
    test += (int)osThreadSetPriority;
    test += (int)osThreadGetPriority;
    test += (int)osThreadSuspend;
    test += (int)osThreadResume;
    test += (int)osThreadYield;
    test += (int)osThreadGetCount;
    test += (int)osDelay;
    test += (int)osTimerNew;
    test += (int)osTimerStart;
    test += (int)osTimerStop;
    test += (int)osTimerIsRunning;
    test += (int)osTimerDelete;
    test += (int)osEventFlagsNew;
    test += (int)osEventFlagsSet;
    test += (int)osEventFlagsWait;
    test += (int)osEventFlagsDelete;
    test += (int)osMutexNew;
    test += (int)osMutexAcquire;
    test += (int)osMutexRelease;
    test += (int)osMutexDelete;
    test += (int)osSemaphoreNew;
    test += (int)osSemaphoreAcquire;
    test += (int)osSemaphoreRelease;
    test += (int)osSemaphoreGetCount;
    test += (int)osSemaphoreDelete;
    test += (int)osMessageQueueDelete;
    test += (int)osMessageQueueGet;
    test += (int)osMessageQueueNew;
    test += (int)osMessageQueuePut;
    test += (int)cm_uart_open;
    test += (int)cm_uart_register_event;
    test += (int)cm_uart_close;
    test += (int)cm_uart_write;
    test += (int)cm_uart_read;
    test += (int)cm_uart_get_cfg;
    test += (int)cm_uart_get_rxrb_data_len;
    test += (int)cm_uart_clean;
    test += (int)cm_uart_is_sending;
    test += (int)cm_mem_get_heap_stats;
    test += (int)cm_malloc;
    test += (int)cm_realloc;
    test += (int)cm_calloc;
    test += (int)cm_free;
    test += (int)cm_sys_get_imei;
    test += (int)cm_sys_get_sn;
    test += (int)cm_sys_get_cm_ver;
    test += (int)cm_log_printf;
    test += (int)cm_assert;
    test += (int)cm_fs_open;
    test += (int)cm_fs_close;
    test += (int)cm_fs_read;
    test += (int)cm_fs_write;
    test += (int)cm_fs_seek;
    test += (int)cm_fs_delete;
    test += (int)cm_fs_move;
    test += (int)cm_fs_exist;
    test += (int)cm_fs_filesize;
    test += (int)cm_fs_getinfo;
    test += (int)cm_rtc_set_timezone;
    test += (int)cm_rtc_get_timezone;
    test += (int)cm_rtc_set_current_time;
    test += (int)cm_rtc_get_current_time;
    test += (int)cm_rtc_register_alarm_cb;
    test += (int)cm_rtc_enable_alarm;
    test += (int)cm_rtc_set_alarm;
    test += (int)cm_rtc_get_alarm;
    test += (int)cm_sim_get_imsi;
    test += (int)cm_gpio_init;
    test += (int)cm_gpio_deinit;
    test += (int)cm_gpio_set_level;
    test += (int)cm_gpio_get_level;
    test += (int)cm_gpio_set_pull;
    test += (int)cm_gpio_get_pull;
    test += (int)cm_gpio_set_direction;
    test += (int)cm_gpio_get_direction;
    test += (int)cm_gpio_interrupt_register;
    test += (int)cm_gpio_interrupt_enable;
    test += (int)cm_gpio_interrupt_disable;
    test += (int)cm_gpio_ioctl;
    test += (int)cm_iomux_set_pin_func;
    test += (int)cm_iomux_get_pin_func;
    test += (int)cm_iomux_set_pin_cmd;
    test += (int)cm_pm_reboot;
    test += (int)cm_pm_poweroff;
    test += (int)cm_pm_get_power_on_reason;
    test += (int)cm_pm_work_lock;
    test += (int)cm_pm_work_unlock;
    test += (int)cm_pm_init;
    test += (int)cm_pm_powerkey_regist_callback;
    test += (int)cm_keypad_config;
    test += (int)cm_keypad_init;
    test += (int)cm_keypad_register;
    test += (int)cm_keypad_unregister;
    test += (int)cm_keypad_deinit;
    test += (int)cm_keypad_sleep;
    test += (int)cm_pwm_set_clk;
    test += (int)cm_pwm_open_ns;
    test += (int)cm_pwm_close;
    test += (int)cm_lbs_init;
    test += (int)cm_lbs_deinit;
    test += (int)cm_lbs_location;
    test += (int)cm_lbs_get_attr;
    test += (int)cm_asocket_eloop;
    test += (int)cm_asocket_open;
    test += (int)cm_asocket_connect;
    test += (int)cm_asocket_close;
    test += (int)cm_async_dns_set_priority;
    test += (int)cm_async_dns_eloop;
    test += (int)cm_async_dns_request_ex;
    test += (int)cm_async_dns_get_type_by_priority;
    test += (int)cm_get_active_cid;
    test += (int)cm_eloop_create;
    test += (int)cm_eloop_delete;
    test += (int)cm_eloop_wait_event;
    test += (int)cm_eloop_register_event;
    test += (int)cm_eloop_unregister_event;
    test += (int)cm_eloop_post_event;
    test += (int)cm_eloop_default;
    test += (int)cm_get_ipv4v6_local_addr;
    test += (int)lwip_accept;
    test += (int)lwip_bind;
    test += (int)lwip_shutdown;
    test += (int)lwip_shutdown2;
    test += (int)lwip_getsockname;
    test += (int)lwip_getpeername;
    test += (int)lwip_setsockopt;
    test += (int)lwip_getsockopt;
    test += (int)lwip_close;
    test += (int)lwip_close2;
    test += (int)lwip_connect;
    test += (int)lwip_listen;
    test += (int)lwip_recv;
    test += (int)lwip_read;
    test += (int)lwip_recvfrom;
    test += (int)lwip_send;
    test += (int)lwip_sendto;
    test += (int)lwip_socket;
    test += (int)lwip_socket_with_callback;
    test += (int)lwip_write;
    test += (int)lwip_select;
    test += (int)lwip_ioctl;
    test += (int)lwip_fcntl;
    test += (int)lwip_eventfd;
    test += (int)lwip_getthreaderrno;
    test += (int)lwip_getsockvalid;
    test += (int)lwip_getsockerrno;
    test += (int)lwip_getsocktype;
    test += (int)lwip_getsocknetconn;
    test += (int)lwip_getsocktcppcb;
    test += (int)lwip_getsockudppcb;
    test += (int)lwip_get_sock_localport;
    test += (int)lwip_get_sock_remoteport;
    test += (int)lwip_get_sock_isipv6;
    test += (int)lwip_get_sock_pdu_offset;
    test += (int)lwip_trigger_process;
    test += (int)lwip_get_sock_localipXaddr;
    test += (int)lwip_get_sock_remoteipXaddr;
    test += (int)lwip_get_sock_vlanid;
    test += (int)readn;
    test += (int)writen;
    test += (int)readline;
    test += (int)lwip_readv;
    test += (int)lwip_writev;
    test += (int)lwip_recvmsg;
    test += (int)lwip_sendmsg;
    test += (int)ipaddr_addr;
    test += (int)ipaddr_aton;
    test += (int)ipaddr_ntoa;
    test += (int)ip6addr_aton;
    test += (int)ip6addr_ntoa;
    test += (int)inet_ntop;
    test += (int)inet_pton;
    test += (int)lwip_gethostbyname;
    test += (int)lwip_getaddrinfo;
    test += (int)lwip_freeaddrinfo;
    test += (int)get_lwip_errno;
    test += (int)cm_fota_set_url;
    test += (int)cm_fota_read_config;
    test += (int)cm_fota_exec_upgrade;
    test += (int)cm_fota_set_reboot_time;
    test += (int)cm_fota_res_callback_register;
    test += (int)cm_httpclient_uri_encode;
    test += (int)cm_httpclient_uri_encode_component;
    test += (int)cm_httpclient_create;
    test += (int)cm_httpclient_delete;
    test += (int)cm_httpclient_is_busy;
    test += (int)cm_httpclient_set_cfg;
    test += (int)cm_httpclient_terminate;
    test += (int)cm_httpclient_custom_header_set;
    test += (int)cm_httpclient_custom_header_free;
    test += (int)cm_httpclient_specific_header_set;
    test += (int)cm_httpclient_specific_header_free;
    test += (int)cm_httpclient_get_response_code;
    test += (int)cm_httpclient_parse_header;
    test += (int)cm_httpclient_sync_request;
    test += (int)cm_httpclient_sync_free_data;
    test += (int)cm_httpclient_request_start;
    test += (int)cm_httpclient_request_send;
    test += (int)cm_httpclient_request_end;
    test += (int)cm_i2c_open;
    test += (int)cm_i2c_close;
    test += (int)cm_i2c_write;
    test += (int)cm_i2c_read;
    test += (int)cm_adc_read;
    test += (int)cm_adc_vbat_read;
    test += (int)cm_spi_open;
    test += (int)cm_spi_close;
    test += (int)cm_spi_write;
    test += (int)cm_spi_read;
    test += (int)cm_spi_write_then_read;
    test += (int)cm_ssl_setopt;
    test += (int)cm_ssl_close;
    test += (int)cm_ssl_write;
    test += (int)cm_ssl_read;
    test += (int)cm_ssl_check_pending;
    test += (int)cm_ssl_get_bytes_avail;
    test += (int)cm_ssl_conn_with_host;
    test += (int)cm_ssl_list_cipher;
    test += (int)cm_cpuinfo_record_start;
    test += (int)cm_cpuinfo_record_read;
    test += (int)cm_cpuinfo_record_stop;
    test += (int)cm_cpuinfo_get_cpuid;
    test += (int)cm_audio_play_set_cfg;
    test += (int)cm_audio_play_get_cfg;
    test += (int)cm_audio_play_file;
    test += (int)cm_audio_player_pause;
    test += (int)cm_audio_player_resume;
    test += (int)cm_audio_player_stop;
    test += (int)cm_audio_player_stream_open;
    test += (int)cm_audio_player_stream_push;
    test += (int)cm_audio_player_stream_close;
    test += (int)cm_audio_player_stream_clear_close;
    test += (int)cm_audio_player_stream_cb_reg;
    test += (int)cm_wifiscan_cfg;
    test += (int)cm_wifiscan_start;
    test += (int)cm_wifiscan_stop;
    test += (int)cm_wifiscan_query;
    test += (int)cm_virt_at_send_async;
    test += (int)cm_virt_at_send_sync;
    test += (int)cm_virt_at_urc_cb_reg;
    test += (int)cm_virt_at_urc_cb_dereg;
    test += (int)cm_sms_send_txt;
    test += (int)cm_sms_send_pdu;
    test += (int)cm_usb2com_register_recv_cb;
    test += (int)cm_usb2com_send_data;
    test += (int)cm_usb2com_register_status_cb;
    test += (int)cm_usb2com_get_status;
    test += (int)cm_lcd_write_cmd;
    test += (int)cm_lcd_write_data;
    test += (int)cm_lcd_write_buf;
    test += (int)cm_lcd_init;
    test += (int)cm_lcd_deinit;
    test += (int)cm_lcd_read_id;
    test += (int)cm_lwm2m_create;
    test += (int)cm_lwm2m_delete;
    test += (int)cm_lwm2m_add_obj;
    test += (int)cm_lwm2m_del_obj;
    test += (int)cm_lwm2m_discover;
    test += (int)cm_lwm2m_open;
    test += (int)cm_lwm2m_update;
    test += (int)cm_lwm2m_close;
    test += (int)cm_lwm2m_notify_packing;
    test += (int)cm_lwm2m_notify;
    test += (int)cm_lwm2m_read_rsp;
    test += (int)cm_lwm2m_write_rsp;
    test += (int)cm_lwm2m_execute_rsp;
    test += (int)cm_lwm2m_param_rsp;
    test += (int)cm_lwm2m_observe_rsp;
    test += (int)cm_ping_init;
    test += (int)cm_ping_start;
    test += (int)AudioHAL_AifBindCodec_CB;
    test += (int)vpathSetMode;
    test += (int)cm_onewire_init;
    test += (int)cm_onewire_setup_reset;
    test += (int)cm_onewire_write_byte;
    test += (int)cm_onewire_read_byte;
    test += (int)cm_onewire_write_data;
    test += (int)cm_onewire_read_data;
    return test;
}