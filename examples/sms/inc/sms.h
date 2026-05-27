#ifndef __NETWORK_H__
#define __NETWORK_H__

typedef enum
{
    CM_NETWORK_EVENT_NO_SIM,
    CM_NETWORK_EVENT_SIM_READY,
    CM_NETWORK_EVENT_PDP_ACTIVED,
    CM_NETWORK_EVENT_PDP_DEACTIVED,
    CM_NETWORK_EVENT_PDP_ACTIVE_FAIL,
    CM_NETWORK_EVENT_MAX
} cm_network_event_e;

typedef void (*cm_network_event_cb)(cm_network_event_e state);

#endif


