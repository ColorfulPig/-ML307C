/*
 * ipcp.h -- IPCP header
 *
 * (C) Copyright [2006-2008] Marvell International Ltd.
 * All Rights Reserved
 *
 */
#ifndef _IPCP_H_
#define _IPCP_H_

#if defined __cplusplus
extern "C" {
#endif				/* __cplusplus */

#include "ppp_types.h"

#define SUPPORT_PPP_IPV6
extern void IpcpHandleIncomingFrame(PppControlS *pppControl);
extern void IpcpSendConfigReq(PppControlS *pppControl);
extern void IpcpUpdateIpParams(PppControlS *pppControl, IpcpConnectionParamsS *ipcpConnectionParams);
extern void Ipv6cpHandleIncomingFrame(PppControlS *pppControl);

#if defined __cplusplus
}
#endif				/* __cplusplus */
#endif				/* _IPCP_H_ */
