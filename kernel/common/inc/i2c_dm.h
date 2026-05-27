#ifndef _I2C_DM_H_
#define _I2C_DM_H_
#include <stdbool.h>
#include <stddef.h>
#include <xscale_types.h>
#include "I2C.h"

void i2cInit(void);
I2C_ReturnCode i2cSend(UINT8 Slave_addr, UINT8 RegAddr, UINT8 RegData);
UINT8 i2cReceive(UINT8 Slave_addr, UINT8 I2CRegAddr);
#endif /* _I2C_DM_H_ */
