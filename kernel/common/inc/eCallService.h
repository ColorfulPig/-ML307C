/*--------------------------------------------------------------------------------------------------------------------
(C) Copyright 2020 ASR Ltd. All Rights Reserved
-------------------------------------------------------------------------------------------------------------------*/

#pragma once
#ifdef __cplusplus
extern "C" {
#endif
#include "gbl_types.h"

    void ivs_start_service(void);
    void ivs_stop_service(void);

    void ivs_control_service(int direction, int on);
    void eCallVoiceMsgHandler(UINT8* msgdata, UINT16 msgLength);

#ifdef __cplusplus
}
#endif
