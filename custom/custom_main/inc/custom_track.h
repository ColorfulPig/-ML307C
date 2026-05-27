#ifndef __CUSTOM_TRACK_H__
#define __CUSTOM_TRACK_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "custom_uart.h"

#define	TRACK_COMM						UART_USB

#define	custom_track_send(data, len)	custom_uart_send(TRACK_COMM, data, len)

#define	BYTETOHEX(ch)			((ch<10)? (ch+'0'):((ch-10)+'A'))

int custom_track_init(void);
int custom_track_printf(const char *_fmt, ...);
int custom_track_printHex(char *_msg, uint8_t *_data, uint16_t _len);
int	custom_track_printRaw(char *_msg, uint8_t *_data, uint16_t _len);
int custom_track_enable(uint8_t _onoff);

#define	SYSTEM_printf(fmt, args...)			do { custom_track_printf("[SYSTEM]" fmt, ##args);}while(0)
#define	SYSTEM_printHex(name,array,len)		do { custom_track_printHex("[SYSTEM]" name,array,len);}while(0)
#define	SYSTEM_printRaw(name,array,len)		do { custom_track_printRaw("[SYSTEM]" name,array,len);}while(0)

#define	FOTA_printf(fmt, args...)			do { custom_track_printf("[FOTA]" fmt, ##args);}while(0)
#define	FOTA_printHex(name,array,len)		do { custom_track_printHex("[FOTA]" name,array,len);}while(0)
#define	FOTA_printRaw(name,array,len)		do { custom_track_printRaw("[FOTA]" name,array,len);}while(0)

#define	GNSS_printf(fmt, args...)			do { custom_track_printf("[GNSS]" fmt, ##args);}while(0)
#define	GNSS_printHex(name,array,len)		do { custom_track_printHex("[GNSS]" name,array,len);}while(0)
#define	GNSS_printRaw(name,array,len)		do { custom_track_printRaw("[GNSS]" name,array,len);}while(0)

#define	cJSON_printf(fmt, args...)			do { custom_track_printf("[cJSON]" fmt, ##args);}while(0)
#define	cJSON_printHex(name,array,len)		do { custom_track_printHex("[cJSON]" name,array,len);}while(0)
#define	cJSON_printRaw(name,array,len)		do { custom_track_printRaw("[cJSON]" name,array,len);}while(0)

#define	LBS_printf(fmt, args...)			do { custom_track_printf("[LBS]" fmt, ##args);}while(0)
#define	LBS_printHex(name,array,len)		do { custom_track_printHex("[LBS]" name,array,len);}while(0)
#define	LBS_printRaw(name,array,len)		do { custom_track_printRaw("[LBS]" name,array,len);}while(0)

#define	ONENET_printf(fmt, args...)			do { custom_track_printf("[ONENET]" fmt, ##args);}while(0)
#define	ONENET_printHex(name,array,len)		do { custom_track_printHex("[ONENET]" name,array,len);}while(0)
#define	ONENET_printRaw(name,array,len)		do { custom_track_printRaw("[ONENET]" name,array,len);}while(0)

#define	BMS_printf(fmt, args...)			do { custom_track_printf("[BMS]" fmt, ##args);}while(0)
#define	BMS_printHex(name,array,len)		do { custom_track_printHex("[BMS]" name,array,len);}while(0)
#define	BMS_printRaw(name,array,len)		do { custom_track_printRaw("[BMS]" name,array,len);}while(0)

#define	BlueTooth_printf(fmt, args...)			do { custom_track_printf("[BT]" fmt, ##args);}while(0)
#define	BlueTooth_printHex(name,array,len)		do { custom_track_printHex("[BT]" name,array,len);}while(0)
#define	BlueTooth_printRaw(name,array,len)		do { custom_track_printRaw("[BT]" name,array,len);}while(0)

#define	Cloud_printf(fmt, args...)			do { custom_track_printf("[Cloud]" fmt, ##args);}while(0)
#define	Cloud_printHex(name,array,len)		do { custom_track_printHex("[Cloud]" name,array,len);}while(0)
#define	Cloud_printRaw(name,array,len)		do { custom_track_printRaw("[Cloud]" name,array,len);}while(0)

#define	Network_printf(fmt, args...)			do { custom_track_printf("[Network]" fmt, ##args);}while(0)
#define	Network_printHex(name,array,len)		do { custom_track_printHex("[Network]" name,array,len);}while(0)
#define	Network_printRaw(name,array,len)		do { custom_track_printRaw("[Network]" name,array,len);}while(0)

#define	TEST_printf(fmt, args...)			do { custom_track_printf("[TEST]" fmt, ##args);}while(0)
#define	TEST_printHex(name,array,len)		do { custom_track_printHex("[TEST]" name,array,len);}while(0)
#define	TEST_printRaw(name,array,len)		do { custom_track_printRaw("[TEST]" name,array,len);}while(0)

#endif


