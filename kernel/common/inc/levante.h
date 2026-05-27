/*------------------------------------------------------------
(C) Copyright [2006-2008] Marvell International Ltd.
All Rights Reserved
------------------------------------------------------------*/

#ifndef _LEVANTE_H_
#define _LEVANTE_H_

#include "global_types.h"
#include "PMChip.h"

void levantePhase1Init(void);
void levantePhase2Init(void);

void levanteMeasurmentControl(BOOL onOff);


BOOL levanteGetTempAndVbatReadings (BOOL both);
void levanteGetTemperatureReadingsReplay (UINT16 t);
void levanteGetTempAndVbatReadingsReplay (BOOL both,UINT16 t, UINT16 v);
void levanteGetVbatReadingsReplay (UINT16 t);
UINT16 levanteAdcVbatConvert(UINT16 ADCRes);
void levanteSetIrqMask(UINT32 RegMask, UINT8 Function);
BOOL levanteConfigUsimV(PMC_UsimV_TYPE voltage);
BOOL levanteEnableUsimV(void);
BOOL levanteUSBDetection( PMCNotifyEventFunc pmcBindFunc );
void levanteReadEvents_CallBack(UINT8 *dataBuffPtr, UINT16 dataSize, UINT16 userId);
BOOL levanteDisableUsimV(void);
void levantePMICReadVBatTempertask ( void   *argv );


#endif  /* _LEVANTE_H_ */
