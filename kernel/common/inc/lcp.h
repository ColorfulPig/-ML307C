/*
 * lcp.h -- lcp header
 *
 * (C) Copyright [2006-2008] Marvell International Ltd.
 * All Rights Reserved
 *
 */

#ifndef _LCP_H_
#define _LCP_H_

#if defined __cplusplus
extern "C" {
#endif				/*__cplusplus */

#include "ppp_types.h"

/* APIs */
extern void LcpHandleIncomingFrame(PppControlS *pppControl);
extern void LcpSendConfigReq(PppControlS *pppControl);
extern void LcpSendTerminateReq(PppControlS *pppControl);
extern void LcpSendEchoReq(PppControlS *pppControl);
extern void LcpKickoffEchoRequest(PppControlS *pppControl);
extern void CcpHandleIncomingFrame(PppControlS *pppControl);
extern void lcpHandleEchoReqTimeout(UINT32 arg);
extern void lcpSendNextEchoReq(UINT32 arg);

#if defined __cplusplus
}
#endif				/* __cplusplus */
#endif
