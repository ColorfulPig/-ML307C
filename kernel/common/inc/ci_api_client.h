#ifndef __CI_API_CLIENT_H__
#define __CI_API_CLIENT_H__

#include "ci_api_types.h"

#define CI_API_VER_MAJOR 2
#define CI_API_VER_MINOR 0

#define ciShRegisterReq 	ciShRegisterReq_client
#define ciShDeregisterReq 	ciShDeregisterReq_client
#define ciShRequest 		ciShRequest_client
#define ciRequest 			ciRequest_client
#define ciRespond 			ciRespond_client

CiReturnCode  ciShRegisterReq_client ( CiShConfirm ciShConfirm, CiShOpaqueHandle opShHandle, CiShFreeReqMem ciShFreeReqMem, CiShOpaqueHandle opShFreeHandle);
CiReturnCode  ciShDeregisterReq_client (CiShHandle handle, CiShOpaqueHandle opShHandle);
CiReturnCode  ciShRequest_client (CiShHandle handle, CiShOper oper, void* reqParas, CiShOpaqueHandle opHandle);   
CiReturnCode  ciRequest_client (CiServiceHandle handle, CiPrimitiveID primId, CiRequestHandle reqHandle, void* paras);
CiReturnCode  ciRespond_client (CiServiceHandle handle, CiPrimitiveID primId, CiIndicationHandle indHandle, void* paras);

#endif
