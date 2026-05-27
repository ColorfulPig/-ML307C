#ifndef __CUSTOM_BLUETOOTH_H__
#define __CUSTOM_BLUETOOTH_H__

#include "cm_os.h"

// 115200
#define	BLUETOOTH_COMM	UART2

#define	custom_bluetooth_IsLinkOK()			(bluetooth_LinkState == 1)

#define	custom_bluetooth_IsCommandIdle()	(bluetooth_CommandAck_Wait == 0)
#define	custom_bluetooth_SetCommandIdle()	(bluetooth_CommandAck_Wait = 0)


extern char bluetooth_MAC[20];
extern uint8_t bluetooth_LinkState;
extern uint8_t bluetooth_MAC_Update;
extern uint8_t bluetooth_CommandAck_Wait;

int custom_bluetooth_init(void);
int custom_bluetooth_OnBlock(uint8_t *buf,uint32_t len);
int custom_bluetooth_send(uint8_t *buf, uint16_t len);

#endif


