#ifndef __GPS_UART_API_H__
#define __GPS_UART_API_H__

#define UART_PORT_0     0
#define UART_PORT_1     1
#define UART_PORT_2     2
#define UART_PORT_3     3

#ifndef UART_NEW_VERSION
#include "UART_common_type.h"

typedef int (*UartIsrCb)(uint32 port_id);

typedef enum 
{
	uart_rx = 0,
	uart_tx,
	max_dir,
}uartdir;

typedef enum
{
    uart_int_none = 0,
    uart_rx_int,
    uart_tx_int,
    uart_eor_int,
    uart_all_int,
    uart_int_max,
}uartirqtype;

typedef struct
{
  uint32_t port_id;
  uint32_t fifo;
  uint32_t dma;
  uint32_t loopback;
  uartirqtype int_tpye;
  uint32_t highspeed;
  uint32_t flowctrl;
  uint32_t baudrate;
  uint32_t datawidth;
  uint32_t parity;
  UartIsrCb rx_callback;
  UartIsrCb tx_callback;
}uart_cfg;

#define BAUDRATE_9600       9600
#define BAUDRATE_14400      14400
#define BAUDRATE_19200      19200
#define BAUDRATE_38400      38400
#define BAUDRATE_57600      57600
#define BAUDRATE_115200     115200
#define BAUDRATE_230400     230400
#define BAUDRATE_460800     460800
#define BAUDRATE_921600     921600
#define BAUDRATE_1842000    1842000
#define BAUDRATE_3000000    3000000
#define BAUDRATE_3686400    3686400


#define HOST_DOWNLOAD_GPS_FW_BAUDRATE                (BAUDRATE_1842000)
#define HOST_COMMUNICATE_WITH_GPS_BAUDRATE      (BAUDRATE_115200)



typedef enum
{
  UART_PARITY_NONE = 0,
  UART_PARITY_ODD   = 1,
  UART_PARITY_EVEN  = 3,
} UART_PARITY_TYPE;

typedef enum
{
  UART_DATA_5 = 0,
  UART_DATA_6 = 1,
  UART_DATA_7 = 2,
  UART_DATA_8 = 3,
} UART_DATA_WIDTH;

typedef struct
{
  uint32_t base_addr;
  uint32_t apbc_base;
  uint32_t irq;
  uint32_t fpga;
} uart_glb_info;

uint32 gps_uart_send_data(uint32 port_id, uint8 *data, uint32 size);
int gps_uart_getchar_noblock (uint32 port_id);
void gps_uart_basic_init (uart_cfg *port);
void gps_uart_irq_init (uart_cfg *port);
void uart_dma_enable (uint32 port_id, uint32 enable);
unsigned int get_gpsuart_dma_total_cnt(void);
#else
#include "UART_drv.h"
#define HOST_DOWNLOAD_GPS_FW_BAUDRATE           (BAUDRATE_1842000)
#define HOST_COMMUNICATE_WITH_GPS_BAUDRATE      (BAUDRATE_115200)
#endif //UART_NEW_VERSION

#endif
