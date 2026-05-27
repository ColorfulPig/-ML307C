
#include "custom_main.h"
#include "custom_uart.h"
#include "custom_fota.h"
#include "custom_watchdog.h"
#include "custom_network.h"
#include "custom_track.h"
#include "custom_profile.h"
#include "custom_onenet.h"
#include "custom_bms.h"
#include "custom_led.h"
#include "custom_gpio.h"
#include "custom_system.h"
#include "custom_usb.h"
#include "custom_bluetooth.h"
#include "custom_test.h"
#include "custom_cloud.h"
#include "custom_gnss.h"
#include "custom_bms_ota.h"
#include "custom_cloud_lte.h"

int cm_opencpu_entry(void *param)
{
	(void)param;

	custom_uart_init();
	custom_usb_init();
	custom_track_init();
	
	osDelay(ONE_SECONED);		// 等待1秒再打印，因为使用USB打印，操作需要时间
	SYSTEM_printf("######################cm_opencpu_entry########################");
	
	custom_profile_init();
	custom_network_init();
	custom_watchdog_init();
	custom_system_init();
	custom_onenet_init();
	custom_bms_init();
	custom_led_init();
	custom_gpio_init();

	custom_bluetooth_init();
	custom_test_init();
	custom_cloud_init();
	custom_gnss_init();
	custom_bms_ota_init();
	custom_cloud_lte_init();
	custom_fota_init();
	
	return 0;
}

