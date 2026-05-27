#ifndef __LWIP_SYSARCH_H__
#define __LWIP_SYSARCH_H__

#include "opt.h"
#include "err.h"
#include "cc.h"
#include "debug.h"
#include "mem.h"
#include <stdlib.h>

typedef UINT32 sys_prot_t;
typedef void * sys_mutex_t;
typedef void * sys_sem_t;
typedef void * sys_mbox_t;
typedef void * sys_thread_t;
typedef void (*eventCallback)   (void *pArgs, int evt);
typedef void (*printCallback)   (char *printstr);
typedef void (*lwip_thread_fn)  (void *pArgs);
typedef err_t (*lwip_run_entry)  (void *pArgs);

/* close by cmiot1325 start */
#if 0
/****************************************************/
/*******external system function declare here********/
/***************************************************/
/*special setting for kestrel platfomr to pass compler*/
#if IS_LWIP_PLATFORM_KESTREL
#define OSASemaphoreCheck(a)    OS_SUCCESS
#define try_wakeup_host()
#define td_send_packet(data, len, cid)

int getUplaneUlDataRate(void);
int getUplaneDlDataRate(void);
#define LteMacutCheckDlDataThroughput() getUplaneUlDataRate()
#define LteMacutCheckUlDataThroughput() getUplaneDlDataRate()
#else
OSA_STATUS OsaSemaphoreCheck( OsaRefT OsaRef);
void try_wakeup_host(void);
void td_send_packet(u8_t *data, u32_t len, int cid);
int LteMacutCheckDlDataThroughput(void);
int LteMacutCheckUlDataThroughput(void);
#endif

#if IS_LWIP_PLATFORM_CRANEW
UINT32 get_32k_timer(void);
static inline u32_t sys_tick_now(void) { return get_32k_timer(); }
#else
static inline u32_t sys_tick_now(void) { return OSAGetTicks(); }
#endif

int ipv4_address_change_trigger(void);
u8_t InProduction_Mode(void);
u8_t IsMiniSystem(void);
char is_4g_mode(void);
BOOL IsGsmRatPowerOn(void);
BOOL IsGsmRatForMIFI(void);

u32_t rti_get_current_tick(void);
u32_t disableInterrupts(void);
void restoreInterrupts(u32_t cpsr);
void RTI_LOG(const char* fmt, ...);
OsaRefT OsaCurrentThreadRef(void);
u32_t OSATaskGetSysParam1(OSATaskRef OsaRef);
void OSATaskSetSysParam1(OSATaskRef OsaRef, u32_t value);
UINT8 IsMbimEnabled(void);
void CommPMSwitch(u8_t ProductPoint);
void lwip_atctl_initial(void);

/****************************************************/
/*******external system function declare end********/
/***************************************************/
#endif
/* close by cmiot1325 end */

/*The interface is adapter to AT cmder*/
struct lwip_thread_msgq_msg_st {
	u32_t msgID;
	void *pArgs;
    printCallback print;
    lwip_run_entry run_entry;
};

extern u8_t lwip_init_flag;
extern u8_t lwip_rtiline_debug;
extern u8_t lwip_mifi_flag;
extern u8_t lwip_hop_limit;
extern u32_t lwip_rng_num;
extern u32_t g_server_ip;

extern msg_t msg_null;
extern UINT8 booting_done;
extern UINT32 uart_initialized;

/** Return code for timeouts from sys_arch_mbox_fetch and sys_arch_sem_wait */
#define SYS_ARCH_TIMEOUT 0xffffffffUL

/** sys_mbox_tryfetch() returns SYS_MBOX_EMPTY if appropriate.
 * For now we use the same magic value, but we allow this to change in future.
 */
#define SYS_MBOX_EMPTY  SYS_ARCH_TIMEOUT
#define SYS_MBOX_NULL   NULL
#define SYS_SEM_NULL    NULL

/*Note:should keep the same value as uart.h define*/
#define UART_INIT_DONE_TEMP_HERE    0xDEAFBEED

/*define DNS set pos*/
#define NETIF_SET_DNS_SERVER           (1)   /*set dns relay server using netif config*/
#define WEB_SET_DNS_SERVER             (2)   /*set dns relay server by webui config*/
#define API_SET_DNS_SERVER             (3)   /*set dns relay server by lwip_api function*/
#define RESET_DEFAULT_DNS_SERVER       (0xFF) /*reset dns relay server by default netif*/

#define LWIP_PLATFORM_DONGLE_MODE      (0)
#define LWIP_PLATFORM_MIFI_MODE        (1)
#define LWIP_PLATFORM_BRIDGE_MODE      (2)
#define LWIP_PLATFORM_DEFINE_END       (3)

#define LWIP_TESTMODE_NORMAL           (0)
#define LWIP_TESTMODE_IOT_MODE         (1)
#define LWIP_TESTMODE_MINIDP           (2)
#define LWIP_TESTMODE_DEFINE_END       (3)

#define LWIP_UPDATE                    (2)
#define LWIP_ENABLE                    (1)
#define LWIP_DISABLE                   (0)
#define LWIP_TASK_PENDING              (0)
#define LWIP_TASK_RUNING               (1)
#define LWIP_TASK_EXITING              (2)
#define NEWLINE "\r\n"

#define _BYTES_ALIGN_8(len)     ((len + 8) - (len + 8) % 8)
#define LWIP_BIT_SHIFT(a, b)    (((b) > 0) ? ((a) << (b)) : ((a) >> (-b)))
#define LWIP_STRPRINT_CHECK(a)  ((a == NULL) ? "NULL" : a)
#define U32_DIFF(a, b) (((a) >= (b)) ? ((a) - (b)) : (((a) + ((b) ^ 0xFFFFFFFF) + 1)))

#if 0
#define MAKE_HASH_KEY(ipaddr, flg, port)                ((ipaddr)&0xFF000000|(flg)|(port))
#define MAKE_HASH_KEY_2(ipaddr, port)                   ((ipaddr)&0xFFFF0000|(port))
#endif
/*ipaddr 8bit; sport 16bit; dport 4bit; flag 4bit*/
#define MAKE_HASH_KEY_NEW(ipaddr, sport, dport, flg)    ((ipaddr)&0xFF000000|(sport)|(dport)|(flg))
#define LWIP_FLAGS_HOTBIT_CHECK(flags, hotbit)    (((flags) & (hotbit)) ? 1 : 0)
#define LWIP_FLAGS_HOTBIT_CLEAR(flags, hotbit)    ((flags) &= ~(hotbit))
#define LWIP_FLAGS_HOTBIT_SET(flags, hotbit)      ((flags) |= (hotbit))


#define IS_LWIP_DONGLE_MODE         (LWIP_PLATFORM_DONGLE_MODE == lwip_mifi_flag)
#define IS_LWIP_MIFI_MODE           (LWIP_PLATFORM_MIFI_MODE == lwip_mifi_flag)
#define IS_LWIP_BRIDGE_MODE         (LWIP_PLATFORM_BRIDGE_MODE == lwip_mifi_flag)
#define IS_NOT_LWIP_MIFI_MODE       (LWIP_PLATFORM_MIFI_MODE != lwip_mifi_flag) /*maybe dongle or bridge, bridge mode based on dongle*/

/* 169.254.x.x */
#define IP_169_254_HEADER           (0xFEA9)
#define IS_IP_169_254(addr)         (((addr)&0xFFFF) == IP_169_254_HEADER)
#define IP_169_254_ADDR             (0x0100FEA9)

/* 127.0.0.1 */
#define IP_LOOPBACK_ADDRESS         (0x100007F)
#define IS_LOOPBACK_IP_ADDR(addr)   ((addr) == IP_LOOPBACK_ADDRESS)

/* 192.168.x.x */
#define IP_192_168_HEADER           (0xA8C0)
#define IS_IP_192_168(addr)         (((addr)&0xFFFF) == IP_192_168_HEADER)
#define IP_192_168_ADDR             (0x0100A8C0)

#define IP4_IS_BROADCAST(ip, pc)    ((((ip)&0xFFFFFF) == ((pc)&0xFFFFFF))&& (((ip)>>24) == 0xFF))
/* ip is packet ip address, pc is pc ip address*/
#define IP4_ROUTER_PC(ip, pc)       (IP4_IS_BROADCAST(ip, pc) || ((ip) == (pc)))
/* addr is in network order */
#define IS_LOCAL_IP_ADDR(addr)      (((addr)&0xFFFFFF) == (htonl(g_server_ip) & 0xFFFFFF))
#define IS_ROUTER_IP_ADDR(addr)     ((addr) == htonl(g_server_ip))
#define is_for_mifi(addr)           (((addr) & 0xFFFFFF) == (dhcpd_router_ip() & 0xFFFFFF))
#define is_broadcast(addr)          ((addr) == 0xFFFFFFFF)
#define LWIP_ROUTER_IP 				(0xC0A80000) /* 192.168.x.x */

/* 1 tick is 5 ms */
#define SYS_ONE_TICK                (5)
#define SYS_ONE_SECOND              (1000/SYS_ONE_TICK)
#define LWIP_RECORD_TICK            (200) /* 1s */
#define LWIP_RAND       			lwip_sys_rand

void LWIP_LOG(const char *fmt, ...);
void lwipCheckHeapMem( void *pMem );
#if LWIP_RECORED_LINE_FLAG
void lwip_record_line(const char *string, unsigned int line);
void lwip_dump_rtilog(void);
#else
#define lwip_record_line(a, b)
#define lwip_dump_rtilog()
#endif
void lwip_assert_now_callback(void * ctx);
void lwip_record_assert(const char *string, unsigned int line);

#if LWIP_DEBUG
/*LWIP_DIAG, using diagprintf*/
/*debug, define on debug.h, macro*/
/*cat3, using lwip_filename_linenumber format to define.*/
#define LWIP_DIAG(debug, cat3, fmt, args...)            \
{                                                           \
    if (LWIP_DEBUGF_SWITCH_CHECK(debug))                    \
    {                                                       \
        DIAG_FILTER(MIFI, LWIP, cat3, DIAG_INFORMATION)     \
        diagPrintf(fmt, ##args);                            \
		lwip_record_line(__FUNCTION__, __LINE__);		    \
    }                                                       \
}
#else
#define LWIP_DIAG(debug, cat3, fmt, args...)
#endif

/*LWIP_DIAG_S, using diagprintf, but no DEBUG check*/
/*DEBUG check will only do once at begining */
/*debug, define on debug.h, macro*/
/*cat3, using lwip_filename_linenumber format to define.*/
#define LWIP_DIAG_S(cat3, fmt, args...)            \
{                                                  \
    if(!IS_DIAG_BLOCKED){                         \
        DIAG_FILTER(MIFI, LWIP, cat3, DIAG_INFORMATION)     \
        diagPrintf(fmt, ##args);                            \
		lwip_record_line(__FUNCTION__, __LINE__);			\
    }                                                       \
}

#define LWIP_DIAG_STRUCT(debug, cat3, fmt, buffer, length)  \
{                                                           \
    if(!IS_DIAG_BLOCKED){                                  \
        DIAG_FILTER(MIFI, LWIP, cat3, DIAG_INFORMATION)     \
        diagStructPrintf(fmt, buffer, length);              \
    }                                                      \
}

#define RETURN(x, cat3) \
do { \
    LWIP_DIAG(DIAG_ON_DEBUG, cat3, "ERR_RTE in %s, line %d", __FUNCTION__, __LINE__); \
    return (x); \
} while(0)

static inline u8_t lwip_assert_fn(u8_t assertion, const char *function, unsigned int line)
{
	if (1 == assertion) {
		return 0;
	} else {
		lwip_record_assert(function, line);
		return 1;
	}

}

static inline int lwip_is_ready(void) {
    return((1 == lwip_init_flag) ? 1 : 0);
}

#define LWIP_ASSERT(assertion)  lwip_assert_fn((assertion), __FUNCTION__, __LINE__)

/*******************************************************************/
/********************Global inline function define *****************/
/*******************************************************************/
u32_t lwip_sys_rand(void);
void sys_init(void);
void sys_msleep(u32_t ms);
void sys_thread_delete(sys_thread_t thread);

#if LWIP_NO_THREAD_CHECK
static inline int sys_thread_check(sys_thread_t thread) {return 0;}
#else

int sys_thread_check(sys_thread_t thread);
#endif
sys_thread_t sys_thread_new(const char *name, lwip_thread_fn thread,
                                    void *arg, int stacksize, int prio);
#if LWIP_PRAGMA_SECTION_RWDATA
sys_thread_t sys_thread_tcpip(const char *name, lwip_thread_fn thread,
                                    void *arg, int stacksize, int prio);
#endif
#if 0
u32_t sys_disableInterrupts_fn(const char* fun, const int line);
void sys_restoreInterrupts_fn(u32_t cpsr, const char* fun, const int line);
#define sys_disableInterrupts()      sys_disableInterrupts_fn(__FUNCTION__, __LINE__)
#define sys_restoreInterrupts(cpsr)  sys_restoreInterrupts_fn((cpsr), __FUNCTION__, __LINE__)

#if LWIP_SYS_PROTECT_USING_MUTEX
void sys_arch_protect(sys_prot_t* pval);
void sys_arch_unprotect(sys_prot_t val);
#else
void sys_arch_protect_fn(sys_prot_t* pval, const char* fun, const int line);
void sys_arch_unprotect_fn(sys_prot_t val, const char* fun, const int line);
#define sys_arch_protect(pval)      sys_arch_protect_fn((pval), __FUNCTION__, __LINE__)
#define sys_arch_unprotect(val)     sys_arch_unprotect_fn((val), __FUNCTION__, __LINE__)
#endif
#endif

/* close by cmiot1325 start */
#if 0
static inline u32_t sys_disableInterrupts_fn(const char* fun, const int line)
{
	lwip_record_line(fun, line);
    return disableInterrupts();
}
static inline void sys_restoreInterrupts_fn(u32_t cpsr, const char* fun, const int line)
{
	lwip_record_line(fun, line);
    restoreInterrupts(cpsr);
}
#define sys_disableInterrupts()      sys_disableInterrupts_fn(__FUNCTION__, __LINE__)
#define sys_restoreInterrupts(cpsr)  sys_restoreInterrupts_fn((cpsr), __FUNCTION__, __LINE__)

static inline void sys_arch_protect_fn(sys_prot_t* pval, const char* fun, const int line)
{
	lwip_record_line(fun, line);
    *pval = disableInterrupts();
}
static inline void sys_arch_unprotect_fn(sys_prot_t val, const char* fun, const int line)
{
    lwip_record_line(fun, line);
    restoreInterrupts(val);
}
#endif
/* close by cmiot1325 end */
#define sys_arch_protect(pval)      sys_arch_protect_fn((pval), __FUNCTION__, __LINE__)
#define sys_arch_unprotect(val)     sys_arch_unprotect_fn((val), __FUNCTION__, __LINE__)

/* modify by cmiot1325 */
#if 0
static inline int sys_sem_valid(sys_sem_t *sem)
{
    if (OS_SUCCESS == OSASemaphoreCheck (*sem)) {
#if IS_LWIP_PLATFORM_1802_1802S
        lwipCheckHeapMem(*sem);
#endif
        return 1;
    } else {
        return 0;
    }
}

static inline err_t sys_sem_new(sys_sem_t *sem, u8_t count)
{
    OSA_STATUS status;

    status = OSASemaphoreCreate(sem, count, OS_FIFO);
    ASSERT(OS_SUCCESS == status);
    return ERR_OK;
}

static inline void sys_sem_free(sys_sem_t *sem)
{
    if (sys_sem_valid(sem)) {
        OSASemaphoreDelete(*sem);
        *sem = SYS_SEM_NULL;
    } else {
        /*for inline function, must use LWIP_DEBUGF*/
		LWIP_DEBUGF(DIAG_ON_DEBUG, ("lwiperr: sys_sem_free, sys_sem_valid failed, sem=%lx", sem));
	}
}

static inline void sys_sem_set_invalid(sys_sem_t *sem)
{
    *sem = SYS_SEM_NULL;
}

static inline void sys_sem_signal(sys_sem_t *sem)
{
    if (OS_SUCCESS == OSASemaphoreCheck (*sem)) {
        OSASemaphoreRelease(*sem);
    } else {
        /*for inline function, must use LWIP_DEBUGF*/
		LWIP_DEBUGF(DIAG_ON_DEBUG, ("lwiperr: sys_sem_signal, OSASemaphoreCheck failed, sem=%lx", sem));
	}
}

static inline u32_t sys_arch_sem_wait(sys_sem_t *sem, u32_t timeout)
{
    OSA_STATUS  status;
    int count;

    if (OS_SUCCESS != OSASemaphoreCheck (*sem))
	{
        /*for inline function, must use LWIP_DEBUGF*/
        LWIP_DEBUGF(DIAG_ON_DEBUG, ("fatal warning! fun: %s, line: %d, semsig: %lx", __FUNCTION__, __LINE__, *sem));
        return SYS_ARCH_TIMEOUT;
    }

    if (timeout == 0) {
        status = OSASemaphoreAcquire(*sem, OSA_SUSPEND);
    } else {
        count =  (timeout+(SYS_ONE_TICK-1))/SYS_ONE_TICK;
        status = OSASemaphoreAcquire(*sem, count);
    }

    if (OS_SUCCESS == status) {
        return ERR_OK;
    }
    return SYS_ARCH_TIMEOUT;
}

static inline err_t sys_mbox_new(sys_mbox_t *mbox, int size)
{
    OSA_STATUS status;

    status = OSAMsgQCreate( mbox,
#ifdef OSA_QUEUE_NAMES
                            "lwipMbox",
#endif
                            MBOX_MAX_SIZE,
                            size,
                            OS_FIFO);
    ASSERT(OS_SUCCESS == status);
    return ERR_OK;
}

static inline void sys_mbox_poll(sys_mbox_t *mbox,  u32_t *count)
{
    OSAMsgQPoll( *mbox, count);
}

static inline void sys_mbox_post(sys_mbox_t *mbox,  msg_t msg)
{
    msg_t*      msg_ptr;
    msg_ptr = (msg != NULL) ? (&msg) : (&msg_null);

    OSAMsgQSend( *mbox,
                 sizeof(void*),
                 (UINT8*)msg_ptr,
                 OSA_SUSPEND);
}

static inline err_t sys_mbox_trypost(sys_mbox_t *mbox, msg_t msg)
{
    OSA_STATUS  status;
    msg_t*      msg_ptr;
    u32_t       nums = 0;

    msg_ptr = (msg != NULL) ? (&msg) : (&msg_null);
    while (nums++ < 5)
    {
        status = OSAMsgQSend( *mbox,
                              sizeof(void*),
                              (UINT8*)msg_ptr,
                              OSA_NO_SUSPEND);

        if (OS_SUCCESS == status)
        {
            return ERR_OK;
        }
    }
    return ERR_MEM;
}

static inline u32_t sys_arch_mbox_fetch(sys_mbox_t *mbox, msg_t* msg, u32_t timeout)
{
    OSA_STATUS  status;
    int count;

    if (timeout == 0) {
        status = OSAMsgQRecv( *mbox, (UINT8*)msg, sizeof(void*), OSA_SUSPEND);
    } else {
        count =  (timeout+(SYS_ONE_TICK-1))/SYS_ONE_TICK;
        status = OSAMsgQRecv( *mbox, (UINT8*)msg, sizeof(void*), count);
    }

    if (OS_SUCCESS == status) {
        return ERR_OK;
    }

    return SYS_ARCH_TIMEOUT;
}

static inline u32_t sys_arch_mbox_tryfetch(sys_mbox_t *mbox, msg_t* msg)
{
    return sys_arch_mbox_fetch(mbox, msg, 5*SYS_ONE_TICK);
}

static inline u32_t sys_arch_mbox_tryfetch_nosuspend(sys_mbox_t *mbox, msg_t* msg)
{
    OSA_STATUS  status;

    status = OSAMsgQRecv( *mbox, (UINT8*)msg, sizeof(void*), OSA_NO_SUSPEND);

    if (OS_SUCCESS == status) {
        return ERR_OK;
    }
    return SYS_ARCH_TIMEOUT;
}

static inline void sys_mbox_free(sys_mbox_t *mbox)
{
    if (*mbox) {
        OSAMsgQDelete(*mbox);
        *mbox = SYS_MBOX_NULL;
    }
}

static inline int sys_mbox_valid(sys_mbox_t *mbox)
{
    return SYS_MBOX_NULL != *mbox;
}

static inline void sys_mbox_set_invalid(sys_mbox_t *mbox)
{
    *mbox = SYS_MBOX_NULL;
}


static inline u32_t sys_jiffies(void)
{
    return (OSAGetTicks() * SYS_ONE_TICK);
}

static inline u32_t sys_now(void)
{
    return (OSAGetTicks() * SYS_ONE_TICK);
}

static inline u32_t sys_now_tick(void)
{
    return OSAGetTicks();
}

static inline u32_t sys_gen_rand()
{
    u32_t rng = LWIP_RAND() + lwip_rng_num;
    return rng;
}
#endif
#endif /* __LWIP_SYSARCH_H__ */
