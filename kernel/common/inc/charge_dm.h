#ifndef _CHARGE_DM_H_
#define _CHARGE_DM_H_
#include <stdbool.h>
#include <stddef.h>
#include <xscale_types.h>

#define Battery_Low_Voltage         3450
#define Battery_Full_Voltage        4350
#define Hours_6                     (6 * 60 * 60)
#define Hours_2                     (2 * 60 * 60)

/** charge manage ic family define **/
#define GUILIN                      0
#define PM812                       1
#define NINGBO                      2
#define GUILINLITE                  3


typedef union counter32 {
    UINT8   cnt_8[4];
    UINT32  cnt_32;
} counter32U;

typedef struct chargerTimer {
    UINT8 volCheck;
    UINT8 curCheck;
    UINT8 trickle;
    UINT8 precharge;
    UINT8 cccv;
    BOOL keep; // emm, Don't remember funtion of "keep"...
} chargerTimerT;

typedef struct chargerThreshold {
    UINT16 termCur; // charge termination current
} chargerThresholdT;

typedef struct chargeManagerT {
    UINT8 family;
    UINT8 id;
    UINT8 (*chargeICIDGet)(void);
    void (*init)(void);
    BOOL need3rdCharge;
    BOOL (*batteryConnectCheck)(void);
    BOOL (*usbConnectCheck)(void);
    BOOL (*powerOnKeyCheck)(UINT8 timeOutValue);
    UINT16 (*batInstantVoltGet)(BOOL usbStatus);
    UINT16 (*batStableVoltGet)(UINT8 *pPercent);
    void (*systemPowerOff)(void);
    BOOL (*chargerFull)(void);
    BOOL (*chargerCC)(void);
    BOOL (*setMPP)(UINT16 limit);
    BOOL (*getChargeStatus)(void);
    UINT32 (*getChargeCurrent)(void);
    void (*closeCharge)(void);
    void (*openCharge)(void);
    void (*handleChargerAbnormal)(void);
    BOOL (*checkChargeReady)(UINT32 intStatus);
} chargeManagerTT;

extern struct chargeManagerT chargeManager[8];

void chargeManagePhase2Init(void);
#endif /* _CHARGE_DM_H_ */
