#ifndef _CHARGE_COMPOUND_H_
#define _CHARGE_COMPOUND_H_
#include <stdbool.h>
//#include <stddef.h>
//#include <xscale_types.h>

#include "charge_dm.h"

/** 3rd charge manage ic family define **/
#define ETA4256                     0
//#define ETA6005                     1

typedef struct charg3rdControlInfoT {
    UINT8 family3rd;
    char *name;
    BOOL enable;
} charg3rdControlInfoTT;

typedef struct charge3rdManagerT { // this struct data will be assign to struct chargeManagerT that is defined
                                      // in pmic without charge function by pmicCombineWith3rdCharger()
    UINT8 family3rd;
    void (*init)(void);
    BOOL (*batteryConnectCheck)(void); // 3rd charge ic maybe has this function to check if battery Connect,
                                       // so add this member in this struct;
                                       // use this function of pmic if 3rd charge ic hasnot this function.
    BOOL (*usbConnectCheck)(void);
    BOOL (*chargerFull)(void);
    void (*closeCharge)(void);
    void (*openCharge)(void);
    void (*charge3rdCombineWithPimc)(void *chargeManager);
} charge3rdManagerTT;

extern struct charge3rdManagerT charge3rdManager[8];

#define reg_read(reg)               *(volatile unsigned int *)(reg)
#define reg_write(reg, val)         *(volatile unsigned int *)(reg) = (val); *(volatile unsigned int *)(reg);

void charge3rdInit(void);
void pmicCombineWith3rdCharger(UINT8 family, void *chargeManager);
#endif /* _CHARGE_COMPOUND_H_ */
