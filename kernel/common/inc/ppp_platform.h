/*
 * ppp_platform.h -- platform specific header file
 *
 * (C) Copyright [2006-2008] Marvell International Ltd.
 * All Rights Reserved
 *
 */

#ifndef __PPP_PLATFORM_H__
#define __PPP_PLATFORM_H__

#if defined __cplusplus  /* __cplusplus */
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <osa.h>
#include "uart.h"
//#include "common_print.h"
//#include "ppp_types.h"
#include "dialer_task.h"

#define PPP_SUCCESS 1
#define PPP_FAIL 0

#define PPP_SUSPEND OSA_SUSPEND
#define PPP_NO_SUSPEND 0

#define INVALID_CID 0xFF

#ifdef  TRUE
#undef  TRUE
#endif  /* TRUE */
#define TRUE    1

#ifdef  FALSE
#undef  FALSE
#endif  /* FALSE */
#define FALSE   0

#define errExit(msg) do{}while (0)

#ifndef UNUSED
#define UNUSED(a) (void)(a)
#endif

#ifndef PPP_DEBUG
#define PPP_DEBUG
#endif

#define PPP_LOG_DEBUG 3
#define PPP_LOG_CRITIC 2
#define PPP_LOG_ERROR 1

#if (defined(LTEONLY_THIN) || defined(CRANEL_FP_8MRAM) || defined(LTEGSM_THIN))
#define	PPP_LOG_LEVEL 1
#else
#define PPP_LOG_LEVEL 2
#endif


/*debug*/
#ifndef PPP_DEBUG
#undef INFOMSG
#define INFOMSG(fmt, args ...)		do {}while (0)
#undef DBGMSG
#define DBGMSG(fmt, args ...)		do {}while (0)
#undef ERRMSG
#define ERRMSG					do {}while (0)
#else
#undef INFOMSG
#define INFOMSG(fmt,args...) 	\
        {if(PPP_LOG_LEVEL >= PPP_LOG_DEBUG) CPUartLogPrintf("[ppp]"fmt, ##args);}
#undef DBGMSG
#define DBGMSG(fmt,args...) 	\
        {if(PPP_LOG_LEVEL >= PPP_LOG_CRITIC) CPUartLogPrintf("[ppp]"fmt, ##args);}

#undef ERRMSG
#define ERRMSG(fmt,args...) 	\
        {if(PPP_LOG_LEVEL >= PPP_LOG_ERROR) CPUartLogPrintf("[ppp]"fmt, ##args);}

#endif

/* task */
#define PPP_CREATE_TASK(a, b, c)	ppp_create_task(a, b, c)

/*Timer*/
#define PPP_CREATE_TIMER(a)		 ppp_create_timer(a)
#define PPP_ENABLE_TIMER(a, b, c)	ppp_enable_timer(a, b, c)
#define PPP_DISABLE_TIMER(a)		ppp_disable_timer(a)
#define PPP_DELETE_TIMER(a)		ppp_delete_timer(a)

/* Memory */
#define PPP_MALLOC		malloc
#define PPP_FREE(ptr)			free(ptr)
#define PPP_MEMSET(ptr, val, sz)	memset(ptr, val, sz)
#define PPP_MEMCPY(dptr, sptr, sz)	memcpy(dptr, sptr, sz)

/* Semaphore */
#define PPP_INIT_SEM(a, b, c)		ppp_init_semaphore(a, b, c)
#define PPP_ACQUIRE_SEM(a, b)	ppp_acquire_semaphore(a, b)
#define PPP_RELEASE_SEM(a)		ppp_release_semaphore(a)

/* Message Queue */
#define PPP_CREATE_Q_MSG(q_sz, m_sz)	ppp_create_msg_queue(q_sz, m_sz)
#define PPP_DELETE_Q_MSG(qRef)		ppp_delete_msg_queue(qRef)
#define PPP_SEND_Q_MSG(qRef, ptr, sz)	ppp_send_msg(qRef, ptr, sz)
#define PPP_RECV_Q_MSG(qRef, ptr, sz)	ppp_recv_msg(qRef, ptr, sz, PPP_SUSPEND)
#define QUEUE_MESSAGE_PTR(b, n, sz)	((U_CHAR *)b + n * sz)

/* TERMINAL */
#define PPP_TX_MODEM(ptr, sz, isIP)		ppp_tx_modem(ptr, sz, isIP)
/* COMM */
#define PPP_TX_COMM(ptr, sz)		ppp_tx_comm(ptr, sz)

/* ATCMDSRV intercourse */
#define PPP_TERMINATE(x, y)		modem_ppp_terminate(x, y)
#define PPP_CONNECT(x, y)			modem_ppp_connect(x, y)
#define PPP_AUTHENTICATE(a, b)		modem_ppp_authenticate(a, b)
#define PPP_GETIP(x)			modem_ppp_get_ip(x)
#define PPP_CGDFLT()			modem_ppp_cgdflt()

/* random */
#define PPP_RAND()	ppp_rand()

#define PPP_TRACE_BUF(string, p, l)	ppp_trace_buf(string, p, l)

#ifdef PPP_BYPASS_LWIP
#define PPP_RELAY_RX_COMM
#endif

/* Types */
typedef unsigned char  U_CHAR;
typedef unsigned short  U_SHORT;
typedef unsigned int U_INT;
typedef unsigned long long U_LLONG;
typedef long long LLONG;
//typedef int  INT32;
typedef unsigned char BOOL;

/* task */
typedef OSTaskRef PPP_TASK_t;

/* Semaphore */
typedef OSASemaRef SEM_t;

/* Mutex */
typedef OSAMutexRef MUTEX_t;

/* timer */
typedef OSATimerRef TIMER_t;
typedef void (*TIMER_HANDLER)(UINT32);

/* Message Queue */
#if 0
typedef struct {
	SEM_t queue_sema;
	MUTEX_t queue_lock;
	U_INT head;
	U_INT tail;
	U_INT queue_size;
	U_INT msg_size;
	U_INT msg_count;
	U_CHAR *space;
} PPP_MsgQ_t;
#endif

typedef OSAMsgQRef PPP_MsgQ_t;

typedef struct _ppp_msg {
	int  len;
	char *data;
} ppp_msg_t;

/* callback functions */
#if 0
typedef struct {
	char Username[256];
	char Password[256];
} PapAuthenticationParamsS;

typedef struct {
	ChapChallengeS  *challenge;
	ChapResponseS *response;
}ChapAuthenticationParamsS;

typedef struct {
	U_SHORT auth_type;
	union {
		PapAuthenticationParamsS *PapAuthenticationParams;
		ChapAuthenticationParamsS *ChapAuthenticationParams;
	};
} PppAuthenticationParamsS;
#endif

typedef struct _IpConnectionParams {
	unsigned int IpAddress;
	unsigned int PrimaryDns;
	unsigned int SecondaryDns;
	unsigned char Ipv6InterfaceId[8];
} ipConnectionParams;

/* PppInitStack prototype  */
typedef void (*init_ppp_stack_cb) (void);
/* PppDeinitStack prototype  */
typedef void (*deinit_ppp_stack_cb) (void);
/* PppSendTerminateReq prototype  */
typedef void (*terminate_session_cb) (void);
/* PppResetStack prototype  */
typedef void (*reset_ppp_stack_cb) (void);
/* PppSetPdpStat prototype  */
typedef void (*set_pdp_down_cb) (void);
/* PppSetCid prototype  */
typedef void (*set_ppp_cid_cb) (unsigned char);
/* PppMessageReq prototype  */
typedef void (*rx_message_modem_cb) (const unsigned char *, unsigned int);
/* PppRelayMessageFromComm prototype  */
#ifdef PPP_RELAY_RX_COMM
typedef void (*rx_message_comm_cb) (void *, unsigned int);
#else
typedef void (*rx_message_comm_cb) (void *, void *);
#endif
/* PppUpdateIpParams prototype  */
typedef void (*update_ip_params_cb) (ipConnectionParams *);

typedef struct _ppp_cb_funcs {
	init_ppp_stack_cb    init;
	deinit_ppp_stack_cb  deinit;
	terminate_session_cb terminate;
	reset_ppp_stack_cb   reset;
	set_ppp_cid_cb       set_cid;
	set_pdp_down_cb      set_pdp_down;
	rx_message_modem_cb  rx_modem;
	rx_message_comm_cb   rx_comm;
        update_ip_params_cb update_ip;
} ppp_cb_funcs;

typedef enum _atcmdResultE {
	ATCMD_RESULT_ERROR,
	ATCMD_RESULT_OK,
} atcmdResultE;

/***************************** Declarations ************************************/
/* Task */
extern void ppp_create_task(PPP_TASK_t *task_ref, void (*start_routine) (void *), void *arg);
/* Semaphore */
extern void ppp_init_semaphore(SEM_t *sem, int pshared, unsigned int value);
extern int ppp_acquire_semaphore(SEM_t sem, long timeout);
extern void ppp_release_semaphore(SEM_t sem);
/* timer */
extern void ppp_create_timer(TIMER_t *timerid);
extern void ppp_enable_timer(TIMER_t timerid, long ms, TIMER_HANDLER timeout_handler);
extern void ppp_disable_timer(TIMER_t timerid);
extern void ppp_delete_timer(TIMER_t timerid);
/* Message Queue Functions */
extern PPP_MsgQ_t ppp_create_msg_queue(U_INT queue_size, U_INT msg_size);
extern void ppp_delete_msg_queue(PPP_MsgQ_t qRef);
extern int ppp_send_msg(PPP_MsgQ_t qRef, U_CHAR *ptr, U_INT size);
extern int ppp_recv_msg(PPP_MsgQ_t qRef, U_CHAR *ptr, U_INT size, U_INT timeout);
/* Debug PPP Functions */
extern void ppp_trace_buf(char const *string, char *buf, U_INT length);
/* Random Number Generator */
extern U_INT ppp_rand(void);

#if defined __cplusplus
}
#endif /* __cplusplus */

#endif
